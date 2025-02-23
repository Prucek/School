# Install script for directory: /home/peter/Desktop/School/IZG/izg_lab_06/libs/geGL

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "1")
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

# Set default install directory permissions.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "/usr/bin/objdump")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "/home/peter/Desktop/School/IZG/izg_lab_06/build/libs/geGL/libgeGL.a")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xDevelx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/geGL" TYPE FILE FILES
    "/home/peter/Desktop/School/IZG/izg_lab_06/libs/geGL/src/geGL/Fwd.h"
    "/home/peter/Desktop/School/IZG/izg_lab_06/libs/geGL/src/geGL/Definitions.h"
    "/home/peter/Desktop/School/IZG/izg_lab_06/libs/geGL/src/geGL/Buffer.h"
    "/home/peter/Desktop/School/IZG/izg_lab_06/libs/geGL/src/geGL/VertexArray.h"
    "/home/peter/Desktop/School/IZG/izg_lab_06/libs/geGL/src/geGL/Framebuffer.h"
    "/home/peter/Desktop/School/IZG/izg_lab_06/libs/geGL/src/geGL/Texture.h"
    "/home/peter/Desktop/School/IZG/izg_lab_06/libs/geGL/src/geGL/Sampler.h"
    "/home/peter/Desktop/School/IZG/izg_lab_06/libs/geGL/src/geGL/ProgramPipeline.h"
    "/home/peter/Desktop/School/IZG/izg_lab_06/libs/geGL/src/geGL/Shader.h"
    "/home/peter/Desktop/School/IZG/izg_lab_06/libs/geGL/src/geGL/Program.h"
    "/home/peter/Desktop/School/IZG/izg_lab_06/libs/geGL/src/geGL/ProgramInfo.h"
    "/home/peter/Desktop/School/IZG/izg_lab_06/libs/geGL/src/geGL/Renderbuffer.h"
    "/home/peter/Desktop/School/IZG/izg_lab_06/libs/geGL/src/geGL/OpenGL.h"
    "/home/peter/Desktop/School/IZG/izg_lab_06/libs/geGL/src/geGL/OpenGLUtil.h"
    "/home/peter/Desktop/School/IZG/izg_lab_06/libs/geGL/src/geGL/AsynchronousQuery.h"
    "/home/peter/Desktop/School/IZG/izg_lab_06/libs/geGL/src/geGL/DebugMessage.h"
    "/home/peter/Desktop/School/IZG/izg_lab_06/libs/geGL/src/geGL/OpenGLObject.h"
    "/home/peter/Desktop/School/IZG/izg_lab_06/libs/geGL/src/geGL/geGL.h"
    "/home/peter/Desktop/School/IZG/izg_lab_06/libs/geGL/src/geGL/OpenGLFunctionTable.h"
    "/home/peter/Desktop/School/IZG/izg_lab_06/libs/geGL/src/geGL/OpenGLContext.h"
    "/home/peter/Desktop/School/IZG/izg_lab_06/libs/geGL/src/geGL/OpenGLCapabilities.h"
    "/home/peter/Desktop/School/IZG/izg_lab_06/libs/geGL/src/geGL/FunctionLoaderInterface.h"
    "/home/peter/Desktop/School/IZG/izg_lab_06/libs/geGL/src/geGL/DefaultLoader.h"
    "/home/peter/Desktop/School/IZG/izg_lab_06/libs/geGL/src/geGL/LoaderTableDecorator.h"
    "/home/peter/Desktop/School/IZG/izg_lab_06/libs/geGL/src/geGL/DSATableDecorator.h"
    "/home/peter/Desktop/School/IZG/izg_lab_06/libs/geGL/src/geGL/TrapTableDecorator.h"
    "/home/peter/Desktop/School/IZG/izg_lab_06/libs/geGL/src/geGL/CapabilitiesTableDecorator.h"
    "/home/peter/Desktop/School/IZG/izg_lab_06/libs/geGL/src/geGL/StaticCalls.h"
    "/home/peter/Desktop/School/IZG/izg_lab_06/libs/geGL/src/geGL/GLSLNoise.h"
    "/home/peter/Desktop/School/IZG/izg_lab_06/build/libs/geGL/geGL/gegl_export.h"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xDevelx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/geGL/Generated" TYPE FILE FILES
    "/home/peter/Desktop/School/IZG/izg_lab_06/libs/geGL/src/geGL/Generated/ContextCalls.h"
    "/home/peter/Desktop/School/IZG/izg_lab_06/libs/geGL/src/geGL/Generated/ContextDeclarations.h"
    "/home/peter/Desktop/School/IZG/izg_lab_06/libs/geGL/src/geGL/Generated/FunctionTableCalls.h"
    "/home/peter/Desktop/School/IZG/izg_lab_06/libs/geGL/src/geGL/Generated/LoaderImplementation.h"
    "/home/peter/Desktop/School/IZG/izg_lab_06/libs/geGL/src/geGL/Generated/LoaderLoadingCalls.h"
    "/home/peter/Desktop/School/IZG/izg_lab_06/libs/geGL/src/geGL/Generated/NumberOfFunctions.h"
    "/home/peter/Desktop/School/IZG/izg_lab_06/libs/geGL/src/geGL/Generated/MemberOpenGLFunctions.h"
    "/home/peter/Desktop/School/IZG/izg_lab_06/libs/geGL/src/geGL/Generated/MemberOpenGLPFN.h"
    "/home/peter/Desktop/School/IZG/izg_lab_06/libs/geGL/src/geGL/Generated/OpenGLConstants.h"
    "/home/peter/Desktop/School/IZG/izg_lab_06/libs/geGL/src/geGL/Generated/OpenGLFunctions.h"
    "/home/peter/Desktop/School/IZG/izg_lab_06/libs/geGL/src/geGL/Generated/OpenGLPFN.h"
    "/home/peter/Desktop/School/IZG/izg_lab_06/libs/geGL/src/geGL/Generated/OpenGLTypes.h"
    "/home/peter/Desktop/School/IZG/izg_lab_06/libs/geGL/src/geGL/Generated/TrapCalls.h"
    "/home/peter/Desktop/School/IZG/izg_lab_06/libs/geGL/src/geGL/Generated/TrapImplementation.h"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/geGL/geGLTargets.cmake")
    file(DIFFERENT EXPORT_FILE_CHANGED FILES
         "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/geGL/geGLTargets.cmake"
         "/home/peter/Desktop/School/IZG/izg_lab_06/build/libs/geGL/CMakeFiles/Export/lib/cmake/geGL/geGLTargets.cmake")
    if(EXPORT_FILE_CHANGED)
      file(GLOB OLD_CONFIG_FILES "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/geGL/geGLTargets-*.cmake")
      if(OLD_CONFIG_FILES)
        message(STATUS "Old export file \"$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/geGL/geGLTargets.cmake\" will be replaced.  Removing files [${OLD_CONFIG_FILES}].")
        file(REMOVE ${OLD_CONFIG_FILES})
      endif()
    endif()
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/geGL" TYPE FILE FILES "/home/peter/Desktop/School/IZG/izg_lab_06/build/libs/geGL/CMakeFiles/Export/lib/cmake/geGL/geGLTargets.cmake")
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^()$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/geGL" TYPE FILE FILES "/home/peter/Desktop/School/IZG/izg_lab_06/build/libs/geGL/CMakeFiles/Export/lib/cmake/geGL/geGLTargets-noconfig.cmake")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xDevelx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/geGL" TYPE FILE FILES
    "/home/peter/Desktop/School/IZG/izg_lab_06/build/libs/geGL/geGL/geGLConfig.cmake"
    "/home/peter/Desktop/School/IZG/izg_lab_06/build/libs/geGL/geGL/geGLConfigVersion.cmake"
    )
endif()

