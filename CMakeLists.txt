# =================================
# Generic Setup
# =================================

# Setting environment rules
cmake_minimum_required (VERSION 3.20)
# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()
project ("WakuWaku")

# Set the standard to C++20
# Set the standard globally to C++20 for all sub-folders/targets
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON) # Forces it to error if C++20 isn't available
set(CMAKE_CXX_EXTENSIONS OFF)  # Stick to clean, standard C++ (no compiler-specific stuff)     

# =================================
# CEF Setup
# =================================

set(CEF_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/cef") # set cef root
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CEF_ROOT}/cmake") # Add CEF modules to search path
find_package(CEF REQUIRED)
add_subdirectory(${CEF_ROOT}/libcef_dll libcef_dll_wrapper) # build CEF's wrapper library


# =================================
# Building Executable
# =================================

add_executable(WakuWaku
    "main.cpp"   
)

target_link_libraries(WakuWaku
        PRIVATE 
        libcef_dll_wrapper # CEF's C++ wrapper
        ${CEF_STANDARD_LIBS} # CEF's platform-specific libs)
)

# =================================
# Copy CEF binaries and resources next to my executable
# =================================

# These macros are defined by find_package(CEF)
# They copy libcef.dll, resources, etc. to your output directory
COPY_FILES("WakuWaku" "${CEF_BINARY_FILES}" "${CEF_BINARY_DIR}" "$<TARGET_FILE_DIR:WakuWaku>")
COPY_FILES("WakuWaku" "${CEF_RESOURCE_FILES}" "${CEF_RESOURCE_DIR}" "$<TARGET_FILE_DIR:WakuWaku>")