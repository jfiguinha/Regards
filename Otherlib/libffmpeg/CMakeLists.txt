add_library(wxFFmpeg SHARED
    wxFFmpegView.cc
    GLManager.cc
    shader.cc
    Movie.cc
    Audio.cc
    Video.cc
    )

target_include_directories(wxFFmpeg
    PRIVATE
    ${CMAKE_SOURCE_DIR}/dep/glew/include
    PUBLIC
    ${CMAKE_SOURCE_DIR}/include
    )

target_link_libraries(wxFFmpeg PUBLIC
    ${wxWidgets_LIBRARIES}
    FFmpeg::FFmpeg
    glew
    OpenAL
    )
