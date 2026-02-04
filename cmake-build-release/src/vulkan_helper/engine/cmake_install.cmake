# Install script for directory: C:/Users/nudet/Desktop/Main/Developments/cpp/Projects/VulkanHelper/src/vulkan_helper/engine

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "C:/Program Files (x86)/VulkanHelper")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
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

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

# Set path to fallback-tool for dependency-resolution.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "C:/Program Files/JetBrains/CLion 2025.1/bin/mingw/bin/objdump.exe")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/nudet/Desktop/Main/Developments/cpp/Projects/VulkanHelper/cmake-build-release/src/vulkan_helper/engine/buffer/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/nudet/Desktop/Main/Developments/cpp/Projects/VulkanHelper/cmake-build-release/src/vulkan_helper/engine/cmd/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/nudet/Desktop/Main/Developments/cpp/Projects/VulkanHelper/cmake-build-release/src/vulkan_helper/engine/configurator/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/nudet/Desktop/Main/Developments/cpp/Projects/VulkanHelper/cmake-build-release/src/vulkan_helper/engine/context/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/nudet/Desktop/Main/Developments/cpp/Projects/VulkanHelper/cmake-build-release/src/vulkan_helper/engine/descriptor/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/nudet/Desktop/Main/Developments/cpp/Projects/VulkanHelper/cmake-build-release/src/vulkan_helper/engine/executor/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/nudet/Desktop/Main/Developments/cpp/Projects/VulkanHelper/cmake-build-release/src/vulkan_helper/engine/graphics/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/nudet/Desktop/Main/Developments/cpp/Projects/VulkanHelper/cmake-build-release/src/vulkan_helper/engine/manage/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/nudet/Desktop/Main/Developments/cpp/Projects/VulkanHelper/cmake-build-release/src/vulkan_helper/engine/pipeline/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/nudet/Desktop/Main/Developments/cpp/Projects/VulkanHelper/cmake-build-release/src/vulkan_helper/engine/repo/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/nudet/Desktop/Main/Developments/cpp/Projects/VulkanHelper/cmake-build-release/src/vulkan_helper/engine/sampler/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("C:/Users/nudet/Desktop/Main/Developments/cpp/Projects/VulkanHelper/cmake-build-release/src/vulkan_helper/engine/sync/cmake_install.cmake")
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
if(CMAKE_INSTALL_LOCAL_ONLY)
  file(WRITE "C:/Users/nudet/Desktop/Main/Developments/cpp/Projects/VulkanHelper/cmake-build-release/src/vulkan_helper/engine/install_local_manifest.txt"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
endif()
