cmake_minimum_required(VERSION 3.16)
project(FinSync VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Create executable using native Win32 API (no external libraries needed!)
add_executable(${PROJECT_NAME} WIN32 FinSyncWin32_Fixed.cpp)

# Link Windows libraries (built into Windows)
target_link_libraries(${PROJECT_NAME} PRIVATE comctl32 gdi32)

# Set output directory
set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
)
