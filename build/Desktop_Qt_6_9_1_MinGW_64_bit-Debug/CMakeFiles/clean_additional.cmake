# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\Configurator_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\Configurator_autogen.dir\\ParseCache.txt"
  "Configurator_autogen"
  )
endif()
