// ReSharper disable All
#include <header.h>
// stdafx.h : fichier Include pour les fichiers Include système standard,
// ou les fichiers Include spécifiques aux projets qui sont utilisés fréquemment,
// et sont rarement modifiés
//
#include "RenderOpenGL.h"
#ifdef __APPLE__
#include <OpenCL/OpenCL.h>
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif`

#ifdef __APPLE__
#include <OpenCL/cl_gl.h>
#include <OpenGL/OpenGL.h>
#include <OpenCL/cl_gl_ext.h>
#elif defined(__WXGTK__)
#if wxUSE_GLCANVAS_EGL == 1
#include <EGL/egl.h>
#endif
#endif


#include <utility.h>

#include <ParamInit.h>
#include <RegardsConfigParam.h>

#if defined (__APPLE__) || defined(MACOSX)
static const char* CL_GL_SHARING_EXT = "cl_APPLE_gl_sharing";
#else
static const char* CL_GL_SHARING_EXT = "cl_khr_gl_sharing";
#endif

extern string platformName;
extern cv::ocl::OpenCLExecutionContext clExecCtx;
extern bool isOpenCLInitialized;
using namespace Regards::OpenGL;

CRenderOpenGL::CRenderOpenGL(wxGLCanvas* canvas, const bool& testOpenCLOpenGLInterop)
	: wxGLContext(canvas), base(0), myGLVersion(0), mouseUpdate(nullptr)
{
	width = 0;
	height = 0;
	this->openclOpenGLInterop = false;
	textureDisplay = nullptr;
	this->testOpenCLOpenGLInterop = testOpenCLOpenGLInterop;
}


bool CRenderOpenGL::IsInit()
{
	return isInit;
}

GLTexture* CRenderOpenGL::GetTextureDisplay()
{
	return textureDisplay;
}

bool CRenderOpenGL::GetOpenGLInterop()
{
	return openclOpenGLInterop;
}


wxString GetDeviceInfo(cl_device_id device, cl_device_info param_name)
{
	try
	{
		// Get the length for the i-th device name
		size_t device_name_length = 0;
		cl_int err = clGetDeviceInfo(
			device,
			param_name,
			0,
			nullptr,
			&device_name_length
		);
		Error::CheckError(err);

		// Get the name itself for the i-th device
		// use vector for automatic memory management
		vector<char> device_name(device_name_length);
		err = clGetDeviceInfo(
			device,
			param_name,
			device_name_length,
			&device_name[0],
			nullptr
		);
		Error::CheckError(err);

		return wxString(&device_name[0]);
	}
	catch (...)
	{
	}

	return "";
}

cl_device_id GetListOfDevice(cl_platform_id platform, cl_device_type device_type, int& found)
{
	found = -1;

	cl_uint num_of_devices;

	cl_int err = clGetDeviceIDs(
		platform,
		device_type,
		0,
		nullptr,
		&num_of_devices
	);

	Error::CheckError(err);

	vector<cl_device_id> devices(num_of_devices);

	err = clGetDeviceIDs(
		platform,
		device_type,
		num_of_devices,
		&devices[0],
		nullptr
	);
	Error::CheckError(err);

	for (cl_uint i = 0; i < num_of_devices; ++i)
	{
		int supported = 0;
		cl_device_type type;
		clGetDeviceInfo(devices[i], CL_DEVICE_TYPE, sizeof(type), &type, nullptr);
		wxString deviceName = GetDeviceInfo(devices[i], CL_DEVICE_NAME);
		if (deviceName == "")
			continue;

		if (type == CL_DEVICE_TYPE_GPU)
		{
			wxString supportExt = GetDeviceInfo(devices[i], CL_DEVICE_EXTENSIONS);
			supported = supportExt.find(CL_GL_SHARING_EXT);
			if (supported > 0)
				supported = 1;
			else
				supported = 0;
		}

		if (!supported)
			continue;

		found = i;
		//printf("Device found : %s \n", CConvertUtility::ConvertToUTF8(deviceName));
		break;
	}

	if (found == -1)
		return nullptr;
	return devices[found];
}

void CRenderOpenGL::initializeContextFromGL()
{
#if defined(__APPLE__) || defined(__MACOSX)

    printf("initializeContextFromGL 1\n");
	cl_uint numPlatforms;
	cl_int status = clGetPlatformIDs(0, NULL, &numPlatforms);
	if (status != CL_SUCCESS)
		CV_Error(cv::Error::OpenCLInitError, "OpenCL: Can't get number of platforms");
	if (numPlatforms == 0)
		CV_Error(cv::Error::OpenCLInitError, "OpenCL: No available platforms");

	std::vector<cl_platform_id> platforms(numPlatforms);
	status = clGetPlatformIDs(numPlatforms, &platforms[0], NULL);
	if (status != CL_SUCCESS)
		CV_Error(cv::Error::OpenCLInitError, "OpenCL: Can't get number of platforms");

	// TODO Filter platforms by name from OPENCV_OPENCL_DEVICE
     printf("initializeContextFromGL 2\n");
	int found = -1;
	cl_device_id device = NULL;
	device = GetListOfDevice(platforms[0], CL_DEVICE_TYPE_GPU, found);

	if (found < 0)
		CV_Error(cv::Error::OpenCLInitError, "OpenCL: Can't create context for OpenGL interop");

	cl_context context = NULL;

	// get OpenGL share group
	CGLContextObj cgl_current_context = CGLGetCurrentContext();
	CGLShareGroupObj cgl_share_group = CGLGetShareGroup(cgl_current_context);
    
     printf("initializeContextFromGL 3\n");

	cl_context_properties properties[] = {
		CL_CONTEXT_PROPERTY_USE_CGL_SHAREGROUP_APPLE,
		(cl_context_properties)cgl_share_group,
		0
	};
	// create context
	context = clCreateContext(properties, 1, &device, NULL, NULL, &status);
	if (status != CL_SUCCESS)
	{
		clReleaseDevice(device);
		CV_Error(cv::Error::OpenCLInitError, "OpenCL: Can't create context for OpenGL interop");
	}

     printf("initializeContextFromGL 4\n");

	cl_platform_id platform = platforms[0];
	std::string platformName = cv::ocl::PlatformInfo(&platform).name();

	cv::ocl::OpenCLExecutionContext clExecCtx = cv::ocl::OpenCLExecutionContext::create(platformName, platform, context, device);
	clReleaseDevice(device);
	clReleaseContext(context);
	clExecCtx.bind();

     printf("initializeContextFromGL 5\n");
#else

	cl_uint numPlatforms;
	cl_int status = clGetPlatformIDs(0, NULL, &numPlatforms);
	if (status != CL_SUCCESS)
		CV_Error(cv::Error::OpenCLInitError, "OpenCL: Can't get number of platforms");
	if (numPlatforms == 0)
		CV_Error(cv::Error::OpenCLInitError, "OpenCL: No available platforms");

	std::vector<cl_platform_id> platforms(numPlatforms);
	status = clGetPlatformIDs(numPlatforms, &platforms[0], NULL);
	if (status != CL_SUCCESS)
		CV_Error(cv::Error::OpenCLInitError, "OpenCL: Can't get number of platforms");

	// TODO Filter platforms by name from OPENCV_OPENCL_DEVICE

	int found = -1;
	cl_device_id device = NULL;
	cl_context context = NULL;

	vector<int> platformCompatible;

	for (int i = 0; i < (int)numPlatforms; i++)
	{
		// query platform extension: presence of "cl_khr_gl_sharing" extension is required
		{
			cv::AutoBuffer<char> extensionStr;

			size_t extensionSize;
			status = clGetPlatformInfo(platforms[i], CL_PLATFORM_EXTENSIONS, 0, NULL, &extensionSize);
			if (status == CL_SUCCESS)
			{
				extensionStr.allocate(extensionSize + 1);
				status = clGetPlatformInfo(platforms[i], CL_PLATFORM_EXTENSIONS, extensionSize,
					(char*)extensionStr.data(), NULL);
			}
			if (status != CL_SUCCESS)
				CV_Error(cv::Error::OpenCLInitError, "OpenCL: Can't get platform extension string");

			if (!strstr((const char*)extensionStr.data(), CL_GL_SHARING_EXT))
				continue;
		}

		clGetGLContextInfoKHR_fn clGetGLContextInfoKHR = (clGetGLContextInfoKHR_fn)
			clGetExtensionFunctionAddressForPlatform(platforms[i], "clGetGLContextInfoKHR");
		if (!clGetGLContextInfoKHR)
			continue;

		platformCompatible.push_back(i);
	}

	for (int j = 0; j < platformCompatible.size(); j++)
	{
		int i = platformCompatible[j];
		std::string platformName = cv::ocl::PlatformInfo(&platforms[i]).name();

		clGetGLContextInfoKHR_fn clGetGLContextInfoKHR = (clGetGLContextInfoKHR_fn)
			clGetExtensionFunctionAddressForPlatform(platforms[i], "clGetGLContextInfoKHR");

#ifdef WIN32
		// Create CL context properties, add WGL context & handle to DC
		cl_context_properties properties[] = {
			CL_CONTEXT_PLATFORM, (cl_context_properties)platforms[i], // OpenCL platform
			CL_GL_CONTEXT_KHR, (cl_context_properties)wglGetCurrentContext(), // WGL Context
			CL_WGL_HDC_KHR, (cl_context_properties)wglGetCurrentDC(), // WGL HDC
			0
		};

		/*
		// create context properties listing the platform and current OpenGL display
		cl_context_properties properties[] = {
			CL_CONTEXT_PLATFORM, (cl_context_properties)platform.id(),
		#if defined(__linux__)
			CL_GL_CONTEXT_KHR, (cl_context_properties)glXGetCurrentContext(),
			CL_GLX_DISPLAY_KHR, (cl_context_properties)glXGetCurrentDisplay(),
		#elif defined(_WIN32)
			CL_GL_CONTEXT_KHR, (cl_context_properties)wglGetCurrentContext(),
			CL_WGL_HDC_KHR, (cl_context_properties)wglGetCurrentDC(),
		#endif
			0
		};
		*/

#elif defined(__WXGTK__)

#if wxUSE_GLCANVAS_EGL == 1
		EGLContext eglContext = eglGetCurrentContext();
		EGLDisplay eglDisplay = eglGetCurrentDisplay();

		cl_context_properties properties[] = { CL_CONTEXT_PLATFORM, (cl_context_properties)platforms[i],
		CL_GL_CONTEXT_KHR, (cl_context_properties)eglContext,
		CL_EGL_DISPLAY_KHR,  (cl_context_properties)eglDisplay, 0 };

#else
		// Create CL context properties, add GLX context & handle to DC
		GLXContext glxcontext = glXGetCurrentContext();
		Display* display = glXGetCurrentDisplay();

		cl_context_properties properties[] = {
		 CL_GL_CONTEXT_KHR, (cl_context_properties)glxcontext, // GLX Context
		 CL_GLX_DISPLAY_KHR, (cl_context_properties)display, // GLX Display
		 CL_CONTEXT_PLATFORM, (cl_context_properties)platforms[i], // OpenCL platform
		 0
		};

#endif
#endif

		// query device
		device = NULL;
		status = clGetGLContextInfoKHR(properties, CL_CURRENT_DEVICE_FOR_GL_CONTEXT_KHR, sizeof(cl_device_id),
			&device, 0);
		if (status != CL_SUCCESS)
			continue;

		// create context
		context = clCreateContext(properties, 1, &device, NULL, NULL, &status);
		if (status != CL_SUCCESS)
		{
			clReleaseDevice(device);
			found = -1;
		}
		else
		{
			found = i;
			break;
		}
	}

	if (found < 0)
		CV_Error(cv::Error::OpenCLInitError, "OpenCL: Can't create context for OpenGL interop");

	cl_platform_id platform = platforms[found];
	platformName = cv::ocl::PlatformInfo(&platform).name();

	clExecCtx = cv::ocl::OpenCLExecutionContext::create(
		platformName, platform, context, device);
	clReleaseDevice(device);
	clReleaseContext(context);
	clExecCtx.bind();

#endif
}

void CRenderOpenGL::CreateDefaultOpenCLContext()
{
	openclOpenGLInterop = false;
	cv::ocl::Context context;
	if (!context.create(cv::ocl::Device::TYPE_GPU))
		isOpenCLInitialized = false;
	else
		isOpenCLInitialized = true;

	if (!isOpenCLInitialized)
	{
		if (!context.create(cv::ocl::Device::TYPE_CPU))
			isOpenCLInitialized = false;
		else
			isOpenCLInitialized = true;
	}

	if (isOpenCLInitialized)
	{
		cv::ocl::Device(context.device(0));
	}

	clExecCtx = cv::ocl::OpenCLExecutionContext::getCurrent();
}


void CRenderOpenGL::Init(wxGLCanvas* canvas)
{
	if (!isInit)
	{
		SetCurrent(*canvas);

		int epoxyversion = epoxy_gl_version();
		bool pboSupported = epoxy_has_gl_extension("GL_ARB_pixel_buffer_object");

        printf("CRenderOpenGL::Init \n");

		CRegardsConfigParam* regardsParam = CParamInit::getInstance();
		if (regardsParam != nullptr)
		{
             printf("CRenderOpenGL::Init 1 OpenCL Support : %d GetIsOpenCLOpenGLInteropSupport : %d \n",regardsParam->GetIsOpenCLSupport(), regardsParam->GetIsOpenCLOpenGLInteropSupport());
            
			if (regardsParam->GetIsOpenCLSupport())
			{
				if (cv::ocl::haveOpenCL() && !isOpenCLInitialized && regardsParam->GetIsOpenCLOpenGLInteropSupport())
				{
                     printf("CRenderOpenGL::Init 2 \n");
					try
					{
						initializeContextFromGL();
						isOpenCLInitialized = true;
						openclOpenGLInterop = true;
						cv::ocl::Device(clExecCtx.getContext().device(0));
					}
					catch (cv::Exception& e)
					{
						const char* err_msg = e.what();
						std::cout << "exception caught: " << err_msg << std::endl;
						std::cout << "wrong file format, please input the name of an IMAGE file" << std::endl;
						CreateDefaultOpenCLContext();
					}

				}



				if (!isOpenCLInitialized)
				{
					regardsParam->SetIsOpenCLSupport(false);
				}
				regardsParam->SetIsOpenCLOpenGLInteropSupport(openclOpenGLInterop);
			}
		}
		
		myGLVersion = 0;
		version = glGetString(GL_VERSION);
		sscanf(CConvertUtility::ConvertToUTF8(version), "%f", &myGLVersion);
		isInit = true;


		textureDisplay = new GLTexture(width, height, openclOpenGLInterop);
	}
}

GLSLShader* CRenderOpenGL::CreateShader(const wxString& shaderName, GLenum glSlShaderType_i)
{
	auto m_pShader = new GLSLShader();
	m_pShader->CreateProgram(shaderName, glSlShaderType_i);
	return m_pShader;
}

GLSLShader* CRenderOpenGL::FindShader(const wxString& shaderName, GLenum glSlShaderType_i)
{
	for (COpenGLShader* shader : listShader)
	{
		if (shader->shaderName == shaderName)
			return shader->m_pShader;
	}

	auto openGLShader = new COpenGLShader();
	openGLShader->m_pShader = CreateShader(shaderName, glSlShaderType_i);
	openGLShader->shaderName = shaderName;

	listShader.push_back(openGLShader);

	return openGLShader->m_pShader;
}

CRenderOpenGL::~CRenderOpenGL()
{
	if (textureDisplay != nullptr)
		delete(textureDisplay);

	for (COpenGLShader* shader : listShader)
		delete shader;

	listShader.clear();
}

wxGLContext* CRenderOpenGL::GetGLContext()
{
	return this;
}

void CRenderOpenGL::Print(int x, int y, const char* text)
{
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//set the position of the text in the window using the x and y coordinates
	glRasterPos2f(x, height - 10);
	//get the length of the string to display
	int len = static_cast<int>(strlen(text));

	//loop to display character by character
	for (auto i = 0; i < len; i++)
	{
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, text[i]);
	}
};


GLvoid CRenderOpenGL::ReSizeGLScene(GLsizei width, GLsizei height) // Resize And Initialize The GL Window
{
	if (height == 0) // Prevent A Divide By Zero By
	{
		height = 1; // Making Height Equal One
	}

	glViewport(0, 0, width, height); // Reset The Current Viewport

	glMatrixMode(GL_PROJECTION); // Select The Projection Matrix
	glLoadIdentity(); // Reset The Projection Matrix

	// Calculate The Aspect Ratio Of The Window
	gluPerspective(45.0f, static_cast<GLfloat>(width) / static_cast<GLfloat>(height), 0.1f, 100.0f);

	glMatrixMode(GL_MODELVIEW); // Select The Modelview Matrix
	glLoadIdentity(); // Reset The Modelview Matrix
}


void CRenderOpenGL::SetData(cv::Mat& bitmap)
{
	textureDisplay->SetData(bitmap);
}

bool CRenderOpenGL::SetData(cv::UMat& bitmap)
{
	return textureDisplay->SetData(bitmap);
}


GLTexture* CRenderOpenGL::GetDisplayTexture(const int& width, const int& height, const bool& openclOpenGLInterop)
{
	return textureDisplay;
}

int CRenderOpenGL::GetWidth()
{
	return width;
}

int CRenderOpenGL::GetHeight()
{
	return height;
}

void CRenderOpenGL::CreateScreenRender(const int& width, const int& height, const CRgbaquad& color)
{
	if (this->width != width || this->height != height)
	{
		this->width = width;
		this->height = height;
		ReSizeGLScene(width, height);
        

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_TEXTURE_2D);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluOrtho2D(0, width, 0, height);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
	}

    glClearColor(color.GetFRed() / 255.0f, color.GetFGreen() / 255.0f, color.GetFBlue() / 255.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(color.GetFRed() / 255.0f, color.GetFGreen() / 255.0f, color.GetFBlue() / 255.0f, 1.0f);

}

void CRenderOpenGL::RenderQuad(GLTexture* texture, int left, int top, bool inverted)
{
	glPushMatrix();
	glEnableClientState(GL_TEXTURE_COORD_ARRAY_EXT);
	glEnableClientState(GL_VERTEX_ARRAY);

	GLfloat vertices[] = {
		static_cast<GLfloat>(left), static_cast<GLfloat>(top),
		static_cast<GLfloat>(texture->GetWidth()) + static_cast<GLfloat>(left), static_cast<GLfloat>(top),
		static_cast<GLfloat>(texture->GetWidth()) + static_cast<GLfloat>(left),
		static_cast<GLfloat>(texture->GetHeight()) + static_cast<GLfloat>(top),
		static_cast<GLfloat>(left), static_cast<GLfloat>(texture->GetHeight()) + static_cast<GLfloat>(top)
	};

	GLfloat texVertices[8];

	if (inverted)
	{
		GLfloat vertices[] = {
			0, 1,
			1, 1,
			1, 0,
			0, 0
		};
		memcpy(&texVertices, &vertices, sizeof(GLfloat) * 8);
	}
	else
	{
		GLfloat vertices[] = {
			0, 0,
			1, 0,
			1, 1,
			0, 1
		};
		memcpy(&texVertices, &vertices, sizeof(GLfloat) * 8);
	}
	glVertexPointer(2, GL_FLOAT, 0, vertices);
	glTexCoordPointer(2, GL_FLOAT, 0, texVertices);

	glDrawArrays(GL_QUADS, 0, 4);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);

	glPopMatrix();

	glFlush();
}


void CRenderOpenGL::RenderQuad(GLTexture* texture, const int& width, const int& height, const bool& flipH,
                               const bool& flipV, int left, int top, bool inverted)
{
	glPushMatrix();
	glEnableClientState(GL_TEXTURE_COORD_ARRAY_EXT);
	glEnableClientState(GL_VERTEX_ARRAY);

	GLfloat vertices[] = {
		static_cast<GLfloat>(left), static_cast<GLfloat>(top),
		static_cast<GLfloat>(width) + static_cast<GLfloat>(left), static_cast<GLfloat>(top),
		static_cast<GLfloat>(width) + static_cast<GLfloat>(left),
		static_cast<GLfloat>(height) + static_cast<GLfloat>(top),
		static_cast<GLfloat>(left), static_cast<GLfloat>(height) + static_cast<GLfloat>(top)
	};

	GLfloat texVertices[8];

	if (inverted)
	{
		GLfloat vertices[] = {
			0, 1,
			1, 1,
			1, 0,
			0, 0
		};

		if (flipV)
		{
			vertices[1] = 0;
			vertices[3] = 0;
			vertices[5] = 1;
			vertices[7] = 1;
		}

		if (flipH)
		{
			vertices[0] = 1;
			vertices[2] = 0;
			vertices[4] = 0;
			vertices[6] = 1;
		}

		memcpy(&texVertices, &vertices, sizeof(GLfloat) * 8);
	}
	else
	{
		GLfloat vertices[] = {
			0, 0,
			1, 0,
			1, 1,
			0, 1
		};

		if (flipV)
		{
			vertices[1] = 1;
			vertices[3] = 1;
			vertices[5] = 0;
			vertices[7] = 0;
		}

		if (flipH)
		{
			vertices[0] = 1;
			vertices[2] = 0;
			vertices[4] = 0;
			vertices[6] = 1;
		}

		memcpy(&texVertices, &vertices, sizeof(GLfloat) * 8);
	}

	glVertexPointer(2, GL_FLOAT, 0, vertices);
	glTexCoordPointer(2, GL_FLOAT, 0, texVertices);

	glDrawArrays(GL_QUADS, 0, 4);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);

	glPopMatrix();

	glFlush();
}


void CRenderOpenGL::RenderQuad(GLTexture* texture, const bool& flipH, const bool& flipV, int left, int top,
                               bool inverted)
{
	glPushMatrix();
	glEnableClientState(GL_TEXTURE_COORD_ARRAY_EXT);
	glEnableClientState(GL_VERTEX_ARRAY);

	GLfloat vertices[] = {
		static_cast<GLfloat>(left), static_cast<GLfloat>(top),
		static_cast<GLfloat>(texture->GetWidth()) + static_cast<GLfloat>(left), static_cast<GLfloat>(top),
		static_cast<GLfloat>(texture->GetWidth()) + static_cast<GLfloat>(left),
		static_cast<GLfloat>(texture->GetHeight()) + static_cast<GLfloat>(top),
		static_cast<GLfloat>(left), static_cast<GLfloat>(texture->GetHeight()) + static_cast<GLfloat>(top)
	};

	GLfloat texVertices[8];

	if (inverted)
	{
		GLfloat vertices[] = {
			0, 1,
			1, 1,
			1, 0,
			0, 0
		};

		if (flipV)
		{
			vertices[1] = 0;
			vertices[3] = 0;
			vertices[5] = 1;
			vertices[7] = 1;
		}

		if (flipH)
		{
			vertices[0] = 1;
			vertices[2] = 0;
			vertices[4] = 0;
			vertices[6] = 1;
		}

		memcpy(&texVertices, &vertices, sizeof(GLfloat) * 8);
	}
	else
	{
		GLfloat vertices[] = {
			0, 0,
			1, 0,
			1, 1,
			0, 1
		};

		if (flipV)
		{
			vertices[1] = 1;
			vertices[3] = 1;
			vertices[5] = 0;
			vertices[7] = 0;
		}

		if (flipH)
		{
			vertices[0] = 1;
			vertices[2] = 0;
			vertices[4] = 0;
			vertices[6] = 1;
		}

		memcpy(&texVertices, &vertices, sizeof(GLfloat) * 8);
	}

	glVertexPointer(2, GL_FLOAT, 0, vertices);
	glTexCoordPointer(2, GL_FLOAT, 0, texVertices);

	glDrawArrays(GL_QUADS, 0, 4);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);

	glPopMatrix();

	glFlush();
}

/*
void CRenderOpenGL::RenderToTexture()
{
	GLTexture* displayTexture = GetGLTexture();
	textureDisplay->Enable();
	//textureSource->Enable();
	glBindTexture(GL_TEXTURE_2D, displayTexture->GetTextureID()); // Bind To The Blur Texture
	//glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, width, height);
	glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 0, 0, width, height, 0);
	//textureSource->Disable();
	textureDisplay->Disable();
}
*/
void CRenderOpenGL::RenderToScreen(IMouseUpdate* mousUpdate, CEffectParameter* effectParameter, const int& left,
                                   const int& top, const bool& inverted)
{
	bool renderPreview = false;
	textureDisplay->Enable();

	if (!renderPreview)
	{
		RenderQuad(textureDisplay, left, top, inverted);
	}

	textureDisplay->Disable();
}

GLTexture* CRenderOpenGL::GetGLTexture()
{
	return textureDisplay;
}
