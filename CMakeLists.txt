cmake_minimum_required(VERSION 3.0.0)
project(Play VERSION 0.1.0)

include(CTest)
enable_testing()


set(SRC_FILES
    "main.cpp"
    "LGFX_SDL.cpp"
)


# LovyanGFX
add_definitions(-DLGFX_SDL)
file(GLOB LGFX_TARGET_FILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} CONFIGURE_DEPENDS 
    Lib/LovyanGFX/src/lgfx/Fonts/efont/*.c
    Lib/LovyanGFX/src/lgfx/Fonts/IPA/*.c
    Lib/LovyanGFX/src/lgfx/utility/*.c
    Lib/LovyanGFX/src/lgfx/v1/*.cpp
    Lib/LovyanGFX/src/lgfx/v1/misc/*.cpp
    Lib/LovyanGFX/src/lgfx/v1/panel/Panel_Device.cpp
    Lib/LovyanGFX/src/lgfx/v1/panel/Panel_FrameBufferBase.cpp
    Lib/LovyanGFX/src/lgfx/v1/platforms/sdl/*.cpp
)


add_executable(Play ${SRC_FILES} ${LGFX_TARGET_FILES})
target_include_directories(Play PUBLIC "Lib/LovyanGFX/src/")


# SDL2
find_package(SDL2 REQUIRED SDL2)
include_directories(${SDL2_INCLUDE_DIRS})
target_link_libraries(Play -lpthread ${SDL2_LIBRARIES})


set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
