cmake_minimum_required(VERSION 3.12)
project(YourProjectName)

# Add your existing project files
add_executable(YourProjectName WIN32 
			"WindowsProject1.cpp"
			"WindowsProject1.rc"
			"framework.h"
			"Resource.h"
			"WindowsProject1.cpp"
)
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS")
