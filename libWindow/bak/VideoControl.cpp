#include "VideoControl.h"
#include <wx/dcbuffer.h>
#include "ffplaycore.h"
#include <FiltreEffet.h>
#include <GLTexture.h>
#if defined(__WXMSW__)
#include "../include/config_id.h"
#else
#include <config_id.h>
#endif
#include "LoadingResource.h"

wxDEFINE_EVENT(EVENT_ENDVIDEOTHREAD, wxCommandEvent);

using namespace Regards::Window;
//#define DBLBUFFERPICTURE
int videoargs[] = { WX_GL_RGBA, WX_GL_DOUBLEBUFFER, WX_GL_DEPTH_SIZE, 16 };

CVideoControl::CVideoControl(wxWindow* parent, wxWindowID id, CVideoEffectParameter * videoEffectParameter, IVideoInterface * eventPlayer)
: wxGLCanvas(parent, wxID_ANY, videoargs,  wxDefaultPosition, wxDefaultSize, wxFULL_REPAINT_ON_RESIZE)
{
	config = CParamInit::getInstance();
    Connect(wxEVT_PAINT, wxPaintEventHandler(CVideoControl::OnPaint));
    Connect(wxEVT_SIZE, wxSizeEventHandler(CVideoControl::OnSize));
    Connect(EVENT_ENDVIDEOTHREAD, wxCommandEventHandler(CVideoControl::EndVideoThread));
    Connect(EVENT_VIDEOSTART, wxCommandEventHandler(CVideoControl::VideoStart));
    Connect(EVENT_REFRESHSCREEN, wxCommandEventHandler(CVideoControl::RefreshScreen));
	pause = false;
	videoEnd = true;
	this->eventPlayer = eventPlayer;
	this->videoEffectParameter = videoEffectParameter;
	//videoRenderOpenGL = new CRenderVideoInterfaceOpenGL();

    if (config->GetVideoLibrary() == LIBOPENGL)
    {
        openGLMode = true;
        openCLMode = false;
    }
    else
    {
        openGLMode = false;
        openCLMode = false;
    }



}

SDL_Rect CVideoControl::calculate_display_rect(float aspect_ratio, int angle)
{
	//float aspect_ratio;
	SDL_Rect rect;
	int width, height, x, y;
	int bitmapWidth = 0;
	int bitmapHeight =0;
	
	if (openGLMode || openCLMode)
	{
		bitmapWidth = bitmap->GetBitmapWidth();
		bitmapHeight = bitmap->GetBitmapHeight();
	}
	else
	{
		bitmapWidth = bmp.GetWidth();
		bitmapHeight = bmp.GetHeight();
	}
	
	if(angle == 90 || angle == 270)
	{
		int temp = bitmapWidth;
		bitmapWidth = bitmapHeight;
		bitmapHeight = temp;
	}

	if (aspect_ratio <= 0.0)
		aspect_ratio = 1.0;
	
	aspect_ratio *= (float)bitmapWidth / (float)bitmapHeight;

	/* XXX: we suppose the screen has a 1.0 pixel ratio */
	height = getHeight();
	width = ((int)rint(height * aspect_ratio)) & ~1;
	if (width > getWidth()) {
		width = getWidth();
		height = ((int)rint(width / aspect_ratio)) & ~1;
	}
	x = (getWidth() - width) / 2;
	y = (getHeight() - height) / 2;
	rect.x = x;
	rect.y = y;
	rect.w = FFMAX(width,  1);
	rect.h = FFMAX(height, 1);
	return rect;
}

void CVideoControl::PlayVideo(CVideoControl * sdlWindow)
{
    ffmfc_play(sdlWindow, sdlWindow->filename.ToStdString());
    wxCommandEvent event(EVENT_ENDVIDEOTHREAD);
    wxPostEvent(sdlWindow, event);
}


void CVideoControl::EndVideoThread(wxCommandEvent& event)
{
    muVideo.lock();
    if(threadVideo != nullptr)
    {
        threadVideo->join();
        delete threadVideo;
        threadVideo = nullptr;
    }
    muVideo.unlock();
    videoEnd = true;
    if (eventPlayer != nullptr)
    {
        eventPlayer->OnPositionVideo(0);
        eventPlayer->OnVideoEnd();
    }
}


CVideoControl::~CVideoControl()
{
	quitWindow = true;
	exit = true;
	if (!videoEnd)
	{
        ffmfc_quit();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        muVideo.lock();
        if(threadVideo != nullptr)
            if (threadVideo->joinable())
                threadVideo->join();
        muVideo.unlock();
	}
	//delete(videoRenderOpenGL);
	delete(bitmap);
}


void CVideoControl::Rotate90()
{
	angle += 90;
	angle = angle % 360;
}

void CVideoControl::Rotate270()
{
	angle += 270;
	angle = angle % 360;
}
void CVideoControl::FlipVertical()
{
	flipV = !flipV;
}

void CVideoControl::FlipHorizontal()
{
	flipH = !flipH;
}

int CVideoControl::PlayMovie(const wxString &movie)
{
	if (filename != movie || videoEnd)
	{
        videoStart = false;
        newVideo = true;
        
        if(!videoEnd)
        {
            ffmfc_quit();
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
        if (openGLMode)
            ffmfc_SetOutputMode(32);
        else
            ffmfc_SetOutputMode(24);
        videoEnd = false;
        filename = movie;
        startVideoThread = false;
    //threadVideo = std::thread(PlayVideo, this);
	}
    this->Refresh();
	return 0;
}

void CVideoControl::VideoStart(wxCommandEvent& event)
{
	eventPlayer->OnVideoStart();
	pause = false;
    videoStart =true;
}

int CVideoControl::getWidth()
{
     return GetSize().x;
}
int CVideoControl::getHeight()
{
    return GetSize().y;
}

void CVideoControl::UpdateScreenRatio()
{
    Refresh();
}

CVideoWindowContext& CVideoControl::GetContext(wxGLCanvas *canvas)
{
    CVideoWindowContext *glContext;
    
    if ( !m_glContext )
    {
        // Create the OpenGL context for the first mono window which needs it:
        // subsequently created windows will all share the same context.
        m_glContext = new CVideoWindowContext(canvas);
    }
    glContext = m_glContext;
    
    glContext->SetCurrent(*canvas);
    
    return *glContext;
}

void CVideoControl::DrawBitmap()
{
	muVideo.lock();
    if (!startVideoThread && threadVideo == nullptr)
    {
        threadVideo = new std::thread(PlayVideo, this);
        startVideoThread = true;
    }
    muVideo.unlock();
    

	muBitmap.lock();


	if (openGLMode)
	{
		if (bitmap != nullptr)
		{
#ifdef DBLBUFFERPICTURE
			wxBitmap bitmapLocal = wxBitmap(getWidth(), getHeight());
			wxBufferedPaintDC dc(this, bitmapLocal);
			CWindowMain::FillRect(&dc, this->GetClientRect(), wxColour(0, 0, 0));
			CVideoWindowContext& videoContext = GetContext(this);
			wxImage renderPicture = RenderOpenGL();
			dc.DrawBitmap(renderPicture,0,0);
#else
			CVideoWindowContext& videoContext = GetContext(this);
			wxImage renderPicture = RenderOpenGL();
#endif			

		}
		else
		{
			wxPaintDC dc(this);
			CWindowMain::FillRect(&dc, this->GetClientRect(), wxColour(0, 0, 0));
		}
	}
	else
	{
		wxImage imageToDisplay;
		wxBitmap bitmapLocal = wxBitmap(getWidth(), getHeight());
		wxBufferedPaintDC dc(this, bitmapLocal);
		
			
		if (bmp.IsOk())
		{
			imageToDisplay = bmp.Copy();
		}
		//wxBufferedPaintDC dc(this, bitmapLocal);
		if (bmp.IsOk())
		{	
           int localAngle = angle + videoEffectParameter->rotation;
            
			SDL_Rect outDisplay;

			if (localAngle == 90)
			{
				imageToDisplay = imageToDisplay.Rotate90();
				imageToDisplay = imageToDisplay.Rotate180();
			}
			else if (localAngle == 180)
				imageToDisplay = imageToDisplay.Rotate180();
			else if (localAngle == 270)
			{
				imageToDisplay = imageToDisplay.Rotate90();
			}

			if (flipV)
				imageToDisplay = imageToDisplay.Mirror(false);

			if (flipH)
				imageToDisplay = imageToDisplay.Mirror();
				
			//Calcul du displayRect
			outDisplay = calculate_display_rect(aspectRatio,localAngle);

			CWindowMain::FillRect(&dc, this->GetClientRect(), wxColour(0, 0, 0));
			imageToDisplay = imageToDisplay.Scale(outDisplay.w, outDisplay.h);
			dc.DrawBitmap(imageToDisplay, outDisplay.x, outDisplay.y);
		}
		else
		{
			CWindowMain::FillRect(&dc, this->GetClientRect(), wxColour(0, 0, 0));
		}
	}
	muBitmap.unlock();
}

void CVideoControl::OnPaint(wxPaintEvent& event)
{
	if (quitWindow)
        return;
    
	DrawBitmap();

}

wxImage CVideoControl::RenderOpenGL()
{
	wxImage anImage = wxImage(getWidth(),getHeight());
    if (bitmap != nullptr)
    {
        bool localFlipV = flipV;
        bool localFlipH = flipH;
        SDL_Rect outDisplay;
		
        
        if(videoEffectParameter->rotation < 0)
            videoEffectParameter->rotation = 0;
        
        int localAngle = angle + videoEffectParameter->rotation;
        
        if(textureWidth != bitmap->GetBitmapWidth() || textureHeight != bitmap->GetBitmapHeight())
        {
            if(m_nTextureVideoID != 0)
            {
                glDeleteTextures(1, &m_nTextureVideoID);
                glFinish();
            }

            m_nTextureVideoID = 0;
            newVideo = false;
            textureWidth = bitmap->GetBitmapWidth();
            textureHeight = bitmap->GetBitmapHeight();
        }
        
        if(m_nTextureVideoID == 0)
        {
            glGenTextures(1, &m_nTextureVideoID);
            glBindTexture(GL_TEXTURE_2D, m_nTextureVideoID);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
            glTexImage2D(GL_TEXTURE_2D, 0, 4, textureWidth, textureHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, bitmap->GetPtBitmap());
            newVideo = false;
            
        }
        else
        {
            glBindTexture(GL_TEXTURE_2D, m_nTextureVideoID);
            glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, textureWidth, textureHeight, GL_RGBA, GL_UNSIGNED_BYTE, bitmap->GetPtBitmap());
        }
#ifdef DBLBUFFERPICTURE
		GLTexture * textureSource = new GLTexture(getWidth(), getHeight());
#endif
        
        
        glViewport( 0, 0, getWidth(), getHeight() );
        glMatrixMode( GL_PROJECTION );   // Select the projection matrix
        glLoadIdentity();                // and reset it
        // Calculate the aspect ratio of the view
        gluPerspective( 45.0f, getWidth() / getHeight(), 0.1f, 100.0f );
        glMatrixMode( GL_MODELVIEW );    // Select the modelview matrix
        glLoadIdentity();
        
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_TEXTURE_2D);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluOrtho2D(0, getWidth(), 0, getHeight());
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
				
	     //Calcul du displayRect
		 outDisplay = calculate_display_rect(aspectRatio,localAngle);
        
        if(m_pShader == nullptr)
        {
            m_pShader = new GLSLShader();
            m_pShader->CreateProgram("IDR_GLSL_SHADER_RENDER_BITMAP", GL_FRAGMENT_PROGRAM_ARB);
        }
        
        m_pShader->EnableShader();
        
        if (!m_pShader->SetTexture("Picture", m_nTextureVideoID))
        {
            //TRACE( L"SetTexture failed" );
        }
        
        if (!m_pShader->SetParam("fWidth", (float)bitmap->GetBitmapWidth()))
        {
            //TRACE( L"SetTexture failed" );
        }
        
        if (!m_pShader->SetParam("fHeight", (float)bitmap->GetBitmapHeight()))
        {
            //TRACE( L"SetTexture failed" );
        }
        if (!m_pShader->SetParam("fWidthOutput", getWidth()))
        {
            //TRACE( L"SetTexture failed" );
        }
        
        if (!m_pShader->SetParam("fHeightOutput", getHeight()))
        {
            //TRACE( L"SetTexture failed" );
        }
        if (!m_pShader->SetParam("fAngle", (float)localAngle))
        {
            //TRACE( L"SetTexture failed" );
        }
        
        if (!m_pShader->SetIntegerParam("flipV", 0))
        {
            //TRACE( L"SetTexture failed" );
        }
        
        if (!m_pShader->SetIntegerParam("flipH", 0))
        {
            //TRACE( L"SetTexture failed" );
        }
        
        if (!m_pShader->SetIntegerParam("BicubicEnable", 1))
        {
            //TRACE( L"SetTexture failed" );
        }
        
        glPushMatrix();
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        glEnableClientState(GL_VERTEX_ARRAY);
        
        int x = outDisplay.x;
        int y = outDisplay.y;
        int width = outDisplay.x + outDisplay.w;
        int height = outDisplay.y + outDisplay.h;
        
        
        if(!localFlipV)
        {
            int old = y;
            y = height;
            height = old;
        }
        
            GLfloat vertices[] = {
                static_cast<GLfloat>(x), static_cast<GLfloat>(y),
                static_cast<GLfloat>(width), static_cast<GLfloat>(y),
                static_cast<GLfloat>(width), static_cast<GLfloat>(height),
                static_cast<GLfloat>(x), static_cast<GLfloat>(height) };
        
            glVertexPointer(2, GL_FLOAT, 0, vertices);
        
        if(localFlipH)
        {
            GLfloat texVertices[] = {
                1, 0,
                0, 0,
                0, 1,
                1, 1 };
            
        
            glTexCoordPointer(2, GL_FLOAT, 0, texVertices);
        }
        else
        {
            GLfloat texVertices[] = {
                0, 0,
                1, 0,
                1, 1,
                0, 1 };
            
            
            glTexCoordPointer(2, GL_FLOAT, 0, texVertices);
        }
        
        glDrawArrays(GL_QUADS, 0, 4);
        
        glDisableClientState(GL_VERTEX_ARRAY);
        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
        
        glPopMatrix();
        
        m_pShader->DisableShader();
        
        glFlush();
		
#ifdef DBLBUFFERPICTURE
		
		glBindTexture(GL_TEXTURE_2D, textureSource->GetTextureID());					// Bind To The Blur Texture
		glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 0, 0, getWidth(), getHeight(), 0);
		
		uint8_t * m_bData = textureSource->GetRGBData();
		int posData = 0;
		int widthSrcSize = getWidth() * 3;
		bool invertY = true;
		if(m_bData != nullptr)
		{
			for (int y = 0; y < getHeight(); y++)
			{
				if(invertY)
					posData = ((getHeight() - y) * widthSrcSize) - widthSrcSize;
				else
					posData = y * widthSrcSize;

				for (int x = 0; x < getWidth(); x++)
				{
					anImage.SetRGB(x, y, m_bData[posData], m_bData[posData + 1], m_bData[posData+2]);
					posData += 3;
				}
			}
			delete[] m_bData;
		}
		
		delete textureSource;
		
		SwapBuffers();
#else
		SwapBuffers();
#endif		
    }
	return anImage;
}

void CVideoControl::OnPlay()
{
	if (pause && !videoEnd)
	{
		ffmfc_play_pause();
	}
	else if(videoEnd)
    {
        videoEnd = false;
        startVideoThread = false;
        this->Refresh();
        //threadVideo = std::thread(PlayVideo, this);
    }

	pause = false;
}

void CVideoControl::OnStop()
{
	exit = true;
	videoEnd = false;
    ffmfc_quit();
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
        
    videoEnd = true;

    exit = false;
}

void CVideoControl::OnPause()
{
	if (!pause)
	{
		ffmfc_play_pause();
	}
	pause = true;
}

void CVideoControl::SetVideoDuration(int64_t duration)
{
	if (eventPlayer != nullptr)
		eventPlayer->SetVideoDuration(duration);
}

void CVideoControl::SetVideoPosition(int64_t pos)
{
	ffmfc_SetTimePosition(pos);
}

void CVideoControl::SetPos(int64_t pos)
{
	if (eventPlayer != nullptr)
		eventPlayer->OnPositionVideo(pos);
}

void CVideoControl::VolumeUp()
{
	ffmfc_VolumeUp();
}

void CVideoControl::VolumeDown()
{
	ffmfc_VolumeDown();
}

int CVideoControl::GetVolume()
{
	return ffmfc_GetVolume();
}

void CVideoControl::RefreshScreen(wxCommandEvent& event)
{
	if (quitWindow)
    {
        CBitmapToShow * bitmapToShow = (CBitmapToShow *)event.GetClientData();
        delete[] bitmapToShow->data;
        delete bitmapToShow;
        return;
    }
    
    
    CBitmapToShow * bitmapToShow = (CBitmapToShow *)event.GetClientData();


	muBitmap.lock();

	aspectRatio = bitmapToShow->aspect_ratio;

	if (openGLMode || openCLMode)
	{
		if (bitmap == nullptr)
			bitmap = new CRegardsBitmap();

		bitmap->SetBitmap(bitmapToShow->data, bitmapToShow->width, bitmapToShow->height, false, false);
	}
	else
	{
		// create a bitmap from our pixel data
		bmp = wxImage(bitmapToShow->width, bitmapToShow->height,
			bitmapToShow->data);
        
        //delete[] bitmapToShow->data;
	}
	muBitmap.unlock();

    
    delete bitmapToShow;

	//this->Refresh();
	//this->Update();
	
	DrawBitmap();

}

void CVideoControl::OnSize(wxSizeEvent& event)
{
	int width = event.GetSize().GetWidth();
	int height = event.GetSize().GetHeight();
    if(videoStart)
        ffmfc_videoDisplaySize(width, height);
}
