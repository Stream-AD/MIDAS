# ------------------------------------------------------------------------------
# Copyright 2020 Rui Liu (liurui39660) and Siddharth Bhatia (bhatiasiddharth)
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
# 	http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
# ------------------------------------------------------------------------------

CMAKE_MINIMUM_REQUIRED(VERSION 3.15)
SET(CMAKE_TOOLCHAIN_FILE $ENV{VCPKG_CMAKE}) # Put before PROJECT()
PROJECT(MIDAS)

SET(CMAKE_CXX_STANDARD 20)

INCLUDE_DIRECTORIES(
	src
	$ENV{VCPKG_INCLUDE_DIR}
)

FIND_PACKAGE(TBB)
FIND_PACKAGE(OpenMP)
IF(TBB_FOUND)
	LINK_LIBRARIES(TBB::tbb)
	ADD_COMPILE_DEFINITIONS(ParallelizationProvider_IntelTBB)
ELSEIF(OpenMP_FOUND)
	LINK_LIBRARIES(OpenMP::OpenMP_CXX)
	ADD_COMPILE_DEFINITIONS(ParallelizationProvider_OpenMP)
ENDIF()

IF(WIN32)
	ADD_COMPILE_OPTIONS(/wd4244) # int to float may lose precision. Yes, yes, I know, it is on purpose
ELSEIF(UNIX)
	ADD_COMPILE_OPTIONS(-Wno-unused-result) # Why do we need the return value from fscanf() or system()?
ENDIF()

ADD_COMPILE_DEFINITIONS(
	SOLUTION_DIR="${CMAKE_SOURCE_DIR}/"
)

ADD_EXECUTABLE(Demo example/Demo.cpp)
ADD_EXECUTABLE(Experiment example/Experiment.cpp)
ADD_EXECUTABLE(Reproducible example/Reproducible.cpp)