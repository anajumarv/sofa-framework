cmake_minimum_required(VERSION 2.8)

list(APPEND HEADER_FILES "myopencl.h")
list(APPEND HEADER_FILES "OpenCLKernel.h")
list(APPEND HEADER_FILES "OpenCLProgram.h")
list(APPEND HEADER_FILES "OpenCLProgramParser.h")
list(APPEND HEADER_FILES "OpenCLTypes.h")
list(APPEND HEADER_FILES "gpuopencl.h")
list(APPEND HEADER_FILES "OpenCLCommon.h")

list(APPEND SOURCE_FILES "myopencl.cpp")
list(APPEND SOURCE_FILES "OpenCLCommon.cpp")
list(APPEND SOURCE_FILES "OpenCLProgram.cpp")
list(APPEND SOURCE_FILES "OpenCLMouseInteraction.cpp")
