cmake_minimum_required(VERSION 2.8)

if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
  if(DEFINED ENV{VITASDK})
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VITASDK}/share/vita.toolchain.cmake" CACHE PATH "toolchain file")
  else()
    message(FATAL_ERROR "Please define VITASDK to point to your SDK path!")
  endif()
endif()

project(vita-timer)
include("${VITASDK}/share/vita.cmake" REQUIRED)

set(VITA_APP_NAME "vita-timer")
set(VITA_TITLEID  "VITATIMER")
set(VITA_VERSION  "01.00")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=gnu11")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

file(GLOB_RECURSE ASSETS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} assets/*)

add_executable(${PROJECT_NAME}
  src/screen_main.cc
  src/engine.cc
  src/main.cc
)

target_link_libraries(${PROJECT_NAME}
  SDL2
  SDL2_image
  vita2d
  jpeg
  png
  z
  m
  SceDisplay_stub
  SceCtrl_stub
  SceTouch_stub
  SceSysmodule_stub
  SceGxm_stub
  SceCommonDialog_stub
  SceAppUtil_stub
  ScePgf_stub
  ScePvf_stub
  SceAudio_stub
  SceHid_stub
)

FOREACH(ITEM ${ASSETS})
  list(APPEND VPKFILES "FILE")
  list(APPEND VPKFILES ${ITEM})
  list(APPEND VPKFILES ${ITEM})
ENDFOREACH()

vita_create_self(${PROJECT_NAME}.self ${PROJECT_NAME})
vita_create_vpk(${PROJECT_NAME}.vpk ${VITA_TITLEID} ${PROJECT_NAME}.self
  VERSION ${VITA_VERSION}
  NAME ${VITA_APP_NAME}
  FILE sce_sys/icon0.png sce_sys/icon0.png
  FILE sce_sys/pic0.png sce_sys/pic0.png
  FILE sce_sys/livearea/contents/bg.png sce_sys/livearea/contents/bg.png
  FILE sce_sys/livearea/contents/startup.png sce_sys/livearea/contents/startup.png
  FILE sce_sys/livearea/contents/template.xml sce_sys/livearea/contents/template.xml
  ${VPKFILES}
)
