/*M///////////////////////////////////////////////////////////////////////////////////////
//
//  IMPORTANT: READ BEFORE DOWNLOADING, COPYING, INSTALLING OR USING.
//
//  By downloading, copying, installing or using the software you agree to this license.
//  If you do not agree to this license, do not download, install,
//  copy or use the software.
//
//
//                           License Agreement
//                For Open Source Computer Vision Library
//
// Copyright (C) 2000-2008, Intel Corporation, all rights reserved.
// Copyright (C) 2009, Willow Garage Inc., all rights reserved.
// Third party copyrights are property of their respective owners.
//
// Redistribution and use in source and binary forms, with or without modification,
// are permitted provided that the following conditions are met:
//
//   * Redistribution's of source code must retain the above copyright notice,
//     this list of conditions and the following disclaimer.
//
//   * Redistribution's in binary form must reproduce the above copyright notice,
//     this list of conditions and the following disclaimer in the documentation
//     and/or other materials provided with the distribution.
//
//   * The name of the copyright holders may not be used to endorse or promote products
//     derived from this software without specific prior written permission.
//
// This software is provided by the copyright holders and contributors "as is" and
// any express or implied warranties, including, but not limited to, the implied
// warranties of merchantability and fitness for a particular purpose are disclaimed.
// In no event shall the Intel Corporation or contributors be liable for any direct,
// indirect, incidental, special, exemplary, or consequential damages
// (including, but not limited to, procurement of substitute goods or services;
// loss of use, data, or profits; or business interruption) however caused
// and on any theory of liability, whether in contract, strict liability,
// or tort (including negligence or otherwise) arising in any way out of
// the use of this software, even if advised of the possibility of such damage.
//
//M*/


#include <header.h>
#include "opengl.hpp"
#include "gl_core_3_1.hpp"

using namespace cv;
using namespace cv::ogl;

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

inline
cv::ogl::Buffer::Buffer(int arows, int acols, int atype, Target target, bool autoRelease) : rows_(0), cols_(0), type_(0)
{
    create(arows, acols, atype, target, autoRelease);
}

inline
cv::ogl::Buffer::Buffer(Size asize, int atype, Target target, bool autoRelease) : rows_(0), cols_(0), type_(0)
{
    create(asize, atype, target, autoRelease);
}

inline
void cv::ogl::Buffer::create(Size asize, int atype, Target target, bool autoRelease)
{
    create(asize.height, asize.width, atype, target, autoRelease);
}

inline
int cv::ogl::Buffer::rows() const
{
    return rows_;
}

inline
int cv::ogl::Buffer::cols() const
{
    return cols_;
}

inline
cv::Size cv::ogl::Buffer::size() const
{
    return Size(cols_, rows_);
}

inline
bool cv::ogl::Buffer::empty() const
{
    return rows_ == 0 || cols_ == 0;
}

inline
int cv::ogl::Buffer::type() const
{
    return type_;
}

inline
int cv::ogl::Buffer::depth() const
{
    return CV_MAT_DEPTH(type_);
}

inline
int cv::ogl::Buffer::channels() const
{
    return CV_MAT_CN(type_);
}

inline
int cv::ogl::Buffer::elemSize() const
{
    return CV_ELEM_SIZE(type_);
}

inline
int cv::ogl::Buffer::elemSize1() const
{
    return CV_ELEM_SIZE1(type_);
}

///////

inline
cv::ogl::Texture2D::Texture2D(int arows, int acols, Format aformat, bool autoRelease) : rows_(0), cols_(0), format_(NONE)
{
    create(arows, acols, aformat, autoRelease);
}

inline
cv::ogl::Texture2D::Texture2D(Size asize, Format aformat, bool autoRelease) : rows_(0), cols_(0), format_(NONE)
{
    create(asize, aformat, autoRelease);
}

inline
void cv::ogl::Texture2D::create(Size asize, Format aformat, bool autoRelease)
{
    create(asize.height, asize.width, aformat, autoRelease);
}

inline
int cv::ogl::Texture2D::rows() const
{
    return rows_;
}

inline
int cv::ogl::Texture2D::cols() const
{
    return cols_;
}

inline
cv::Size cv::ogl::Texture2D::size() const
{
    return Size(cols_, rows_);
}

inline
bool cv::ogl::Texture2D::empty() const
{
    return rows_ == 0 || cols_ == 0;
}

inline
cv::ogl::Texture2D::Format cv::ogl::Texture2D::format() const
{
    return format_;
}

///////

// WARNING: unreachable code using Ninja
#if defined _MSC_VER && _MSC_VER >= 1920
#pragma warning(push)
#pragma warning(disable: 4702)
#endif
inline
cv::ogl::Arrays::Arrays() : size_(0)
{}
#if defined _MSC_VER && _MSC_VER >= 1920
#pragma warning(pop)
#endif

inline
int cv::ogl::Arrays::size() const
{
    return size_;
}

inline
bool cv::ogl::Arrays::empty() const
{
    return size_ == 0;
}



#if defined(_MSC_VER)
#pragma warning(disable : 4702)  // unreachable code
#endif

namespace
{
#ifndef HAVE_OPENGL
inline static CV_NORETURN void throw_no_ogl() { CV_Error(cv::Error::OpenGlNotSupported, "The library is compiled without OpenGL support"); }
#elif defined _DEBUG
inline static bool checkError(const char* file, const int line, const char* func = 0)
{
    GLenum err = gl::GetError();
    if (err != gl::NO_ERROR_)
    {
        const char* msg;
        switch (err)
        {
        case gl::INVALID_ENUM:
            msg = "An unacceptable value is specified for an enumerated argument";
            break;
        case gl::INVALID_VALUE:
            msg = "A numeric argument is out of range";
            break;
        case gl::INVALID_OPERATION:
            msg = "The specified operation is not allowed in the current state";
            break;
        case gl::OUT_OF_MEMORY:
            msg = "There is not enough memory left to execute the command";
            break;
        default:
            msg = "Unknown error";
        };
        cv::error(Error::OpenGlApiCallError, func, msg, file, line);
    }
    return true;
}
#endif // HAVE_OPENGL
} // namespace

#define CV_CheckGlError() CV_DbgAssert( (checkError(__FILE__, __LINE__, CV_Func)) )

#ifdef HAVE_OPENGL
namespace
{
    const GLenum gl_types[] = { gl::UNSIGNED_BYTE, gl::BYTE, gl::UNSIGNED_SHORT, gl::SHORT, gl::INT, gl::FLOAT, gl::DOUBLE };
}
#endif


////////////////////////////////////////////////////////////////////////
// ogl::Buffer

#ifndef HAVE_OPENGL

class cv::ogl::Buffer::Impl
{
};

#else

class cv::ogl::Buffer::Impl
{
public:
    static const Ptr<Impl>& empty();

    Impl(GLuint bufId, bool autoRelease);
    Impl(GLsizeiptr size, const GLvoid* data, GLenum target, bool autoRelease);
    ~Impl();

    void bind(GLenum target) const;

    void copyFrom(GLuint srcBuf, GLsizeiptr size);

    void copyFrom(GLsizeiptr size, const GLvoid* data);
    void copyTo(GLsizeiptr size, GLvoid* data) const;

    void* mapHost(GLenum access);
    void unmapHost();

    void setAutoRelease(bool flag) { autoRelease_ = flag; }

    GLuint bufId() const { return bufId_; }

private:
    Impl();

    GLuint bufId_;
    bool autoRelease_;

};

const Ptr<cv::ogl::Buffer::Impl>& cv::ogl::Buffer::Impl::empty()
{
    static Ptr<Impl> p(new Impl);
    return p;
}

cv::ogl::Buffer::Impl::Impl() : bufId_(0), autoRelease_(false)
{
}

cv::ogl::Buffer::Impl::Impl(GLuint abufId, bool autoRelease) : bufId_(abufId), autoRelease_(autoRelease)
{
    CV_Assert( gl::IsBuffer(abufId) == gl::TRUE_ );
}

cv::ogl::Buffer::Impl::Impl(GLsizeiptr size, const GLvoid* data, GLenum target, bool autoRelease) : bufId_(0), autoRelease_(autoRelease)
{
    gl::GenBuffers(1, &bufId_);
    

    CV_Assert( bufId_ != 0 );

    gl::BindBuffer(target, bufId_);
    

    gl::BufferData(target, size, data, gl::DYNAMIC_DRAW);
    

    gl::BindBuffer(target, 0);
    
}

cv::ogl::Buffer::Impl::~Impl()
{
    if (autoRelease_ && bufId_)
        gl::DeleteBuffers(1, &bufId_);
}

void cv::ogl::Buffer::Impl::bind(GLenum target) const
{
    gl::BindBuffer(target, bufId_);
    
}

void cv::ogl::Buffer::Impl::copyFrom(GLuint srcBuf, GLsizeiptr size)
{
    gl::BindBuffer(gl::COPY_WRITE_BUFFER, bufId_);
    

    gl::BindBuffer(gl::COPY_READ_BUFFER, srcBuf);
    

    gl::CopyBufferSubData(gl::COPY_READ_BUFFER, gl::COPY_WRITE_BUFFER, 0, 0, size);
    
}

void cv::ogl::Buffer::Impl::copyFrom(GLsizeiptr size, const GLvoid* data)
{
    gl::BindBuffer(gl::COPY_WRITE_BUFFER, bufId_);
    

    gl::BufferSubData(gl::COPY_WRITE_BUFFER, 0, size, data);
    
}

void cv::ogl::Buffer::Impl::copyTo(GLsizeiptr size, GLvoid* data) const
{
    gl::BindBuffer(gl::COPY_READ_BUFFER, bufId_);
    

    gl::GetBufferSubData(gl::COPY_READ_BUFFER, 0, size, data);
    
}

void* cv::ogl::Buffer::Impl::mapHost(GLenum access)
{
    gl::BindBuffer(gl::COPY_READ_BUFFER, bufId_);
    

    GLvoid* data = gl::MapBuffer(gl::COPY_READ_BUFFER, access);
    

    return data;
}

void cv::ogl::Buffer::Impl::unmapHost()
{
    gl::UnmapBuffer(gl::COPY_READ_BUFFER);
}

#ifdef HAVE_CUDA

void cv::ogl::Buffer::Impl::copyFrom(const void* src, size_t spitch, size_t width, size_t height, cudaStream_t stream)
{
    cudaResource_.registerBuffer(bufId_);
    cudaResource_.copyFrom(src, spitch, width, height, stream);
}

void cv::ogl::Buffer::Impl::copyTo(void* dst, size_t dpitch, size_t width, size_t height, cudaStream_t stream) const
{
    cudaResource_.registerBuffer(bufId_);
    cudaResource_.copyTo(dst, dpitch, width, height, stream);
}

void* cv::ogl::Buffer::Impl::mapDevice(cudaStream_t stream)
{
    cudaResource_.registerBuffer(bufId_);
    return cudaResource_.map(stream);
}

void cv::ogl::Buffer::Impl::unmapDevice(cudaStream_t stream)
{
    cudaResource_.unmap(stream);
}

#endif // HAVE_CUDA

#endif // HAVE_OPENGL

cv::ogl::Buffer::Buffer() : rows_(0), cols_(0), type_(0)
{
#ifndef HAVE_OPENGL
    throw_no_ogl();
#else
    impl_ = Impl::empty();
#endif
}

cv::ogl::Buffer::Buffer(int arows, int acols, int atype, unsigned int abufId, bool autoRelease) : rows_(0), cols_(0), type_(0)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(arows);
    CV_UNUSED(acols);
    CV_UNUSED(atype);
    CV_UNUSED(abufId);
    CV_UNUSED(autoRelease);
    throw_no_ogl();
#else
    impl_.reset(new Impl(abufId, autoRelease));
    rows_ = arows;
    cols_ = acols;
    type_ = atype;
#endif
}

cv::ogl::Buffer::Buffer(Size asize, int atype, unsigned int abufId, bool autoRelease) : rows_(0), cols_(0), type_(0)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(asize);
    CV_UNUSED(atype);
    CV_UNUSED(abufId);
    CV_UNUSED(autoRelease);
    throw_no_ogl();
#else
    impl_.reset(new Impl(abufId, autoRelease));
    rows_ = asize.height;
    cols_ = asize.width;
    type_ = atype;
#endif
}

cv::ogl::Buffer::Buffer(InputArray arr, Target target, bool autoRelease) : rows_(0), cols_(0), type_(0)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(arr);
    CV_UNUSED(target);
    CV_UNUSED(autoRelease);
    throw_no_ogl();
#else
    const int kind = arr.kind();

    switch (kind)
    {
    case _InputArray::OPENGL_BUFFER:
    case _InputArray::CUDA_GPU_MAT:
        copyFrom(arr, target, autoRelease);
        break;

    default:
        {
            Mat mat = arr.getMat();
            CV_Assert( mat.isContinuous() );
            const GLsizeiptr asize = mat.rows * mat.cols * mat.elemSize();
            impl_.reset(new Impl(asize, mat.data, target, autoRelease));
            rows_ = mat.rows;
            cols_ = mat.cols;
            type_ = mat.type();
            break;
        }
    }
#endif
}

void cv::ogl::Buffer::create(int arows, int acols, int atype, Target target, bool autoRelease)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(arows);
    CV_UNUSED(acols);
    CV_UNUSED(atype);
    CV_UNUSED(target);
    CV_UNUSED(autoRelease);
    throw_no_ogl();
#else
    if (rows_ != arows || cols_ != acols || type_ != atype)
    {
        const GLsizeiptr asize = arows * acols * CV_ELEM_SIZE(atype);
        impl_.reset(new Impl(asize, 0, target, autoRelease));
        rows_ = arows;
        cols_ = acols;
        type_ = atype;
    }
#endif
}

void cv::ogl::Buffer::release()
{
#ifdef HAVE_OPENGL
    if (impl_)
        impl_->setAutoRelease(true);
    impl_ = Impl::empty();
    rows_ = 0;
    cols_ = 0;
    type_ = 0;
#endif
}

void cv::ogl::Buffer::setAutoRelease(bool flag)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(flag);
    throw_no_ogl();
#else
    impl_->setAutoRelease(flag);
#endif
}

void cv::ogl::Buffer::copyFrom(InputArray arr, Target target, bool autoRelease)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(arr);
    CV_UNUSED(target);
    CV_UNUSED(autoRelease);
    throw_no_ogl();
#else
    const int kind = arr.kind();

    const Size asize = arr.size();
    const int atype = arr.type();
    create(asize, atype, target, autoRelease);

    switch (kind)
    {
    case _InputArray::OPENGL_BUFFER:
        {
            ogl::Buffer buf = arr.getOGlBuffer();
            impl_->copyFrom(buf.bufId(), asize.area() * CV_ELEM_SIZE(atype));
            break;
        }


    default:
        {
            Mat mat = arr.getMat();
            CV_Assert( mat.isContinuous() );
            impl_->copyFrom(asize.area() * CV_ELEM_SIZE(atype), mat.data);
        }
    }
#endif
}

void cv::ogl::Buffer::copyFrom(InputArray arr, cuda::Stream& stream, Target target, bool autoRelease)
{

}

void cv::ogl::Buffer::copyTo(OutputArray arr) const
{

}

void cv::ogl::Buffer::copyTo(OutputArray arr, cuda::Stream& stream) const
{

}



void cv::ogl::Buffer::bind(Target target) const
{
#ifndef HAVE_OPENGL
    CV_UNUSED(target);
    throw_no_ogl();
#else
    impl_->bind(target);
#endif
}

void cv::ogl::Buffer::unbind(Target target)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(target);
    throw_no_ogl();
#else
    gl::BindBuffer(target, 0);
    
#endif
}

Mat cv::ogl::Buffer::mapHost(Access access)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(access);
    throw_no_ogl();
#else
    return Mat(rows_, cols_, type_, impl_->mapHost(access));
#endif
}

void cv::ogl::Buffer::unmapHost()
{
#ifndef HAVE_OPENGL
    throw_no_ogl();
#else
    return impl_->unmapHost();
#endif
}

void cv::ogl::Buffer::unmapDevice()
{
}

void cv::ogl::Buffer::unmapDevice(cuda::Stream& stream)
{
}

unsigned int cv::ogl::Buffer::bufId() const
{
#ifndef HAVE_OPENGL
    throw_no_ogl();
#else
    return impl_->bufId();
#endif
}


//////////////////////////////////////////////////////////////////////////////////////////
// ogl::Texture

#ifndef HAVE_OPENGL

class cv::ogl::Texture2D::Impl
{
};

#else

class cv::ogl::Texture2D::Impl
{
public:
    static const Ptr<Impl> empty();

    Impl(GLuint texId, bool autoRelease);
    Impl(GLint internalFormat, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid* pixels, bool autoRelease);
    ~Impl();

    void copyFrom(GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid *pixels);
    void copyTo(GLenum format, GLenum type, GLvoid* pixels) const;

    void bind() const;

    void setAutoRelease(bool flag) { autoRelease_ = flag; }

    GLuint texId() const { return texId_; }

private:
    Impl();

    GLuint texId_;
    bool autoRelease_;
};

const Ptr<cv::ogl::Texture2D::Impl> cv::ogl::Texture2D::Impl::empty()
{
    static Ptr<Impl> p(new Impl);
    return p;
}

cv::ogl::Texture2D::Impl::Impl() : texId_(0), autoRelease_(false)
{
}

cv::ogl::Texture2D::Impl::Impl(GLuint atexId, bool autoRelease) : texId_(atexId), autoRelease_(autoRelease)
{
    CV_Assert( gl::IsTexture(atexId) == gl::TRUE_ );
}

cv::ogl::Texture2D::Impl::Impl(GLint internalFormat, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid* pixels, bool autoRelease) : texId_(0), autoRelease_(autoRelease)
{
    gl::GenTextures(1, &texId_);
    

    CV_Assert(texId_ != 0);

    gl::BindTexture(gl::TEXTURE_2D, texId_);
    

    gl::PixelStorei(gl::UNPACK_ALIGNMENT, 1);
    

    gl::TexImage2D(gl::TEXTURE_2D, 0, internalFormat, width, height, 0, format, type, pixels);
    

    gl::GenerateMipmap(gl::TEXTURE_2D);
    
}

cv::ogl::Texture2D::Impl::~Impl()
{
    if (autoRelease_ && texId_)
        gl::DeleteTextures(1, &texId_);
}

void cv::ogl::Texture2D::Impl::copyFrom(GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid *pixels)
{
    gl::BindTexture(gl::TEXTURE_2D, texId_);
    

    gl::PixelStorei(gl::UNPACK_ALIGNMENT, 1);
    

    gl::TexSubImage2D(gl::TEXTURE_2D, 0, 0, 0, width, height, format, type, pixels);
    

    gl::GenerateMipmap(gl::TEXTURE_2D);
    
}

void cv::ogl::Texture2D::Impl::copyTo(GLenum format, GLenum type, GLvoid* pixels) const
{
    gl::BindTexture(gl::TEXTURE_2D, texId_);
    

    gl::PixelStorei(gl::PACK_ALIGNMENT, 1);
    

    gl::GetTexImage(gl::TEXTURE_2D, 0, format, type, pixels);
    
}

void cv::ogl::Texture2D::Impl::bind() const
{
    gl::BindTexture(gl::TEXTURE_2D, texId_);
    
}

#endif // HAVE_OPENGL

cv::ogl::Texture2D::Texture2D() : rows_(0), cols_(0), format_(NONE)
{
#ifndef HAVE_OPENGL
    throw_no_ogl();
#else
    impl_ = Impl::empty();
#endif
}

cv::ogl::Texture2D::Texture2D(int arows, int acols, Format aformat, unsigned int atexId, bool autoRelease) : rows_(0), cols_(0), format_(NONE)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(arows);
    CV_UNUSED(acols);
    CV_UNUSED(aformat);
    CV_UNUSED(atexId);
    CV_UNUSED(autoRelease);
    throw_no_ogl();
#else
    impl_.reset(new Impl(atexId, autoRelease));
    rows_ = arows;
    cols_ = acols;
    format_ = aformat;
#endif
}

cv::ogl::Texture2D::Texture2D(Size asize, Format aformat, unsigned int atexId, bool autoRelease) : rows_(0), cols_(0), format_(NONE)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(asize);
    CV_UNUSED(aformat);
    CV_UNUSED(atexId);
    CV_UNUSED(autoRelease);
    throw_no_ogl();
#else
    impl_.reset(new Impl(atexId, autoRelease));
    rows_ = asize.height;
    cols_ = asize.width;
    format_ = aformat;
#endif
}

cv::ogl::Texture2D::Texture2D(InputArray arr, bool autoRelease) : rows_(0), cols_(0), format_(NONE)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(arr);
    CV_UNUSED(autoRelease);
    throw_no_ogl();
#else
    const int kind = arr.kind();

    const Size asize = arr.size();
    const int atype = arr.type();

    const int depth = CV_MAT_DEPTH(atype);
    const int cn = CV_MAT_CN(atype);

    CV_Assert( depth <= CV_32F );
    CV_Assert( cn == 1 || cn == 3 || cn == 4 );

    const Format internalFormats[] =
    {
        NONE, DEPTH_COMPONENT, NONE, RGB, RGBA
    };
    const GLenum srcFormats[] =
    {
        0, gl::DEPTH_COMPONENT, 0, gl::BGR, gl::BGRA
    };

    switch (kind)
    {
    case _InputArray::OPENGL_BUFFER:
        {
            ogl::Buffer buf = arr.getOGlBuffer();
            buf.bind(ogl::Buffer::PIXEL_UNPACK_BUFFER);
            impl_.reset(new Impl(internalFormats[cn], asize.width, asize.height, srcFormats[cn], gl_types[depth], 0, autoRelease));
            ogl::Buffer::unbind(ogl::Buffer::PIXEL_UNPACK_BUFFER);
            break;
        }

    default:
        {
            Mat mat = arr.getMat();
            CV_Assert( mat.isContinuous() );
            ogl::Buffer::unbind(ogl::Buffer::PIXEL_UNPACK_BUFFER);
            impl_.reset(new Impl(internalFormats[cn], asize.width, asize.height, srcFormats[cn], gl_types[depth], mat.data, autoRelease));
            break;
        }
    }

    rows_ = asize.height;
    cols_ = asize.width;
    format_ = internalFormats[cn];
#endif
}

void cv::ogl::Texture2D::create(int arows, int acols, Format aformat, bool autoRelease)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(arows);
    CV_UNUSED(acols);
    CV_UNUSED(aformat);
    CV_UNUSED(autoRelease);
    throw_no_ogl();
#else
    if (rows_ != arows || cols_ != acols || format_ != aformat)
    {
        ogl::Buffer::unbind(ogl::Buffer::PIXEL_UNPACK_BUFFER);
        impl_.reset(new Impl(aformat, acols, arows, aformat, gl::FLOAT, 0, autoRelease));
        rows_ = arows;
        cols_ = acols;
        format_ = aformat;
    }
#endif
}

void cv::ogl::Texture2D::release()
{
#ifdef HAVE_OPENGL
    if (impl_)
        impl_->setAutoRelease(true);
    impl_ = Impl::empty();
    rows_ = 0;
    cols_ = 0;
    format_ = NONE;
#endif
}

void cv::ogl::Texture2D::setAutoRelease(bool flag)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(flag);
    throw_no_ogl();
#else
    impl_->setAutoRelease(flag);
#endif
}

void cv::ogl::Texture2D::copyFrom(InputArray arr, bool autoRelease)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(arr);
    CV_UNUSED(autoRelease);
    throw_no_ogl();
#else
    const int kind = arr.kind();

    const Size asize = arr.size();
    const int atype = arr.type();

    const int depth = CV_MAT_DEPTH(atype);
    const int cn = CV_MAT_CN(atype);

    CV_Assert( depth <= CV_32F );
    CV_Assert( cn == 1 || cn == 3 || cn == 4 );

    const Format internalFormats[] =
    {
        NONE, DEPTH_COMPONENT, NONE, RGB, RGBA
    };
    const GLenum srcFormats[] =
    {
        0, gl::DEPTH_COMPONENT, 0, gl::BGR, gl::BGRA
    };

    create(asize, internalFormats[cn], autoRelease);

    switch(kind)
    {
    case _InputArray::OPENGL_BUFFER:
        {
            ogl::Buffer buf = arr.getOGlBuffer();
            buf.bind(ogl::Buffer::PIXEL_UNPACK_BUFFER);
            impl_->copyFrom(asize.width, asize.height, srcFormats[cn], gl_types[depth], 0);
            ogl::Buffer::unbind(ogl::Buffer::PIXEL_UNPACK_BUFFER);
            break;
        }

    default:
        {
            Mat mat = arr.getMat();
            CV_Assert( mat.isContinuous() );
            ogl::Buffer::unbind(ogl::Buffer::PIXEL_UNPACK_BUFFER);
            impl_->copyFrom(asize.width, asize.height, srcFormats[cn], gl_types[depth], mat.data);
        }
    }
#endif
}

void cv::ogl::Texture2D::copyTo(OutputArray arr, int ddepth, bool autoRelease) const
{
#ifndef HAVE_OPENGL
    CV_UNUSED(arr);
    CV_UNUSED(ddepth);
    CV_UNUSED(autoRelease);
    throw_no_ogl();
#else
    const int kind = arr.kind();

    const int cn = format_ == DEPTH_COMPONENT ? 1: format_ == RGB ? 3 : 4;
    const GLenum dstFormat = format_ == DEPTH_COMPONENT ? gl::DEPTH_COMPONENT : format_ == RGB ? gl::BGR : gl::BGRA;

    switch(kind)
    {
    case _InputArray::OPENGL_BUFFER:
        {
            ogl::Buffer& buf = arr.getOGlBufferRef();
            buf.create(rows_, cols_, CV_MAKE_TYPE(ddepth, cn), ogl::Buffer::PIXEL_PACK_BUFFER, autoRelease);
            buf.bind(ogl::Buffer::PIXEL_PACK_BUFFER);
            impl_->copyTo(dstFormat, gl_types[ddepth], 0);
            ogl::Buffer::unbind(ogl::Buffer::PIXEL_PACK_BUFFER);
            break;
        }

    default:
        {
            arr.create(rows_, cols_, CV_MAKE_TYPE(ddepth, cn));
            Mat mat = arr.getMat();
            CV_Assert( mat.isContinuous() );
            ogl::Buffer::unbind(ogl::Buffer::PIXEL_PACK_BUFFER);
            impl_->copyTo(dstFormat, gl_types[ddepth], mat.data);
        }
    }
#endif
}

void cv::ogl::Texture2D::bind() const
{
#ifndef HAVE_OPENGL
    throw_no_ogl();
#else
    impl_->bind();
#endif
}

unsigned int cv::ogl::Texture2D::texId() const
{
#ifndef HAVE_OPENGL
    throw_no_ogl();
#else
    return impl_->texId();
#endif
}


////////////////////////////////////////////////////////////////////////
// ogl::Arrays

void cv::ogl::Arrays::setVertexArray(InputArray vertex)
{
    const int cn = vertex.channels();
    const int depth = vertex.depth();

    CV_Assert( cn == 2 || cn == 3 || cn == 4 );
    CV_Assert( depth == CV_16S || depth == CV_32S || depth == CV_32F || depth == CV_64F );

    if (vertex.kind() == _InputArray::OPENGL_BUFFER)
        vertex_ = vertex.getOGlBuffer();
    else
        vertex_.copyFrom(vertex);

    size_ = vertex_.size().area();
}

void cv::ogl::Arrays::resetVertexArray()
{
    vertex_.release();
    size_ = 0;
}

void cv::ogl::Arrays::setColorArray(InputArray color)
{
    const int cn = color.channels();

    CV_Assert( cn == 3 || cn == 4 );

    if (color.kind() == _InputArray::OPENGL_BUFFER)
        color_ = color.getOGlBuffer();
    else
        color_.copyFrom(color);
}

void cv::ogl::Arrays::resetColorArray()
{
    color_.release();
}

void cv::ogl::Arrays::setNormalArray(InputArray normal)
{
    const int cn = normal.channels();
    const int depth = normal.depth();

    CV_Assert( cn == 3 );
    CV_Assert( depth == CV_8S || depth == CV_16S || depth == CV_32S || depth == CV_32F || depth == CV_64F );

    if (normal.kind() == _InputArray::OPENGL_BUFFER)
        normal_ = normal.getOGlBuffer();
    else
        normal_.copyFrom(normal);
}

void cv::ogl::Arrays::resetNormalArray()
{
    normal_.release();
}

void cv::ogl::Arrays::setTexCoordArray(InputArray texCoord)
{
    const int cn = texCoord.channels();
    const int depth = texCoord.depth();

    CV_Assert( cn >= 1 && cn <= 4 );
    CV_Assert( depth == CV_16S || depth == CV_32S || depth == CV_32F || depth == CV_64F );

    if (texCoord.kind() == _InputArray::OPENGL_BUFFER)
        texCoord_ = texCoord.getOGlBuffer();
    else
        texCoord_.copyFrom(texCoord);
}

void cv::ogl::Arrays::resetTexCoordArray()
{
    texCoord_.release();
}

void cv::ogl::Arrays::release()
{
    resetVertexArray();
    resetColorArray();
    resetNormalArray();
    resetTexCoordArray();
}

void cv::ogl::Arrays::setAutoRelease(bool flag)
{
#ifndef HAVE_OPENGL
    CV_UNUSED(flag);
    throw_no_ogl();
#else
    vertex_.setAutoRelease(flag);
    color_.setAutoRelease(flag);
    normal_.setAutoRelease(flag);
    texCoord_.setAutoRelease(flag);
#endif
}

void cv::ogl::Arrays::bind() const
{
#ifndef HAVE_OPENGL
    throw_no_ogl();
#else
    CV_Assert( texCoord_.empty() || texCoord_.size().area() == size_ );
    CV_Assert( normal_.empty() || normal_.size().area() == size_ );
    CV_Assert( color_.empty() || color_.size().area() == size_ );

    if (texCoord_.empty())
    {
        gl::DisableClientState(gl::TEXTURE_COORD_ARRAY);
        
    }
    else
    {
        gl::EnableClientState(gl::TEXTURE_COORD_ARRAY);
        

        texCoord_.bind(ogl::Buffer::ARRAY_BUFFER);

        gl::TexCoordPointer(texCoord_.channels(), gl_types[texCoord_.depth()], 0, 0);
        
    }

    if (normal_.empty())
    {
        gl::DisableClientState(gl::NORMAL_ARRAY);
        
    }
    else
    {
        gl::EnableClientState(gl::NORMAL_ARRAY);
        

        normal_.bind(ogl::Buffer::ARRAY_BUFFER);

        gl::NormalPointer(gl_types[normal_.depth()], 0, 0);
        
    }

    if (color_.empty())
    {
        gl::DisableClientState(gl::COLOR_ARRAY);
        
    }
    else
    {
        gl::EnableClientState(gl::COLOR_ARRAY);
        

        color_.bind(ogl::Buffer::ARRAY_BUFFER);

        const int cn = color_.channels();

        gl::ColorPointer(cn, gl_types[color_.depth()], 0, 0);
        
    }

    if (vertex_.empty())
    {
        gl::DisableClientState(gl::VERTEX_ARRAY);
        
    }
    else
    {
        gl::EnableClientState(gl::VERTEX_ARRAY);
        

        vertex_.bind(ogl::Buffer::ARRAY_BUFFER);

        gl::VertexPointer(vertex_.channels(), gl_types[vertex_.depth()], 0, 0);
        
    }

    ogl::Buffer::unbind(ogl::Buffer::ARRAY_BUFFER);
#endif
}
