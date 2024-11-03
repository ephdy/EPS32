# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

# If CMAKE_DISABLE_SOURCE_CHANGES is set to true and the source directory is an
# existing directory in our source tree, calling file(MAKE_DIRECTORY) on it
# would cause a fatal error, even though it would be a no-op.
if(NOT EXISTS "/home/ep_hdy/esp/esp-idf/components/bootloader/subproject")
  file(MAKE_DIRECTORY "/home/ep_hdy/esp/esp-idf/components/bootloader/subproject")
endif()
file(MAKE_DIRECTORY
  "/home/ep_hdy/Code/ESP32/_096LED_SPI/build/bootloader"
  "/home/ep_hdy/Code/ESP32/_096LED_SPI/build/bootloader-prefix"
  "/home/ep_hdy/Code/ESP32/_096LED_SPI/build/bootloader-prefix/tmp"
  "/home/ep_hdy/Code/ESP32/_096LED_SPI/build/bootloader-prefix/src/bootloader-stamp"
  "/home/ep_hdy/Code/ESP32/_096LED_SPI/build/bootloader-prefix/src"
  "/home/ep_hdy/Code/ESP32/_096LED_SPI/build/bootloader-prefix/src/bootloader-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/home/ep_hdy/Code/ESP32/_096LED_SPI/build/bootloader-prefix/src/bootloader-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/home/ep_hdy/Code/ESP32/_096LED_SPI/build/bootloader-prefix/src/bootloader-stamp${cfgdir}") # cfgdir has leading slash
endif()
