# ------------------------------------------------------------------------------
# Copyright 2020 Rui LIU
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

PROJECT(CPU)

FIND_PACKAGE(TBB REQUIRED)
FIND_PACKAGE(OpenMP REQUIRED)
IF(TBB_FOUND)
	LINK_LIBRARIES(TBB::tbb)
	ADD_COMPILE_DEFINITIONS(ParallelProvider_IntelTBB)
ELSEIF(OpenMP_FOUND)
	LINK_LIBRARIES(OpenMP::OpenMP_CXX)
	ADD_COMPILE_DEFINITIONS(ParallelProvider_OpenMP)
ELSE()
	ADD_COMPILE_DEFINITIONS(ParallelProvider_None)
ENDIF()

ADD_COMPILE_DEFINITIONS(
	PROJECT_DIR="${PROJECT_SOURCE_DIR}/"
)

IF(WIN32)
	ADD_COMPILE_OPTIONS(
		/wd4244  # int to float may lose precision Yes, yes, I know, it is on purpose
		/Qvec-report:1
	)
ELSEIF(UNIX)
	ADD_COMPILE_OPTIONS(-Wno-unused-result) # Why do we need the return value from fscanf() or system()?
ENDIF()

ADD_EXECUTABLE(${PROJECT_NAME}_Demo ${CMAKE_SOURCE_DIR}/example/Demo.cpp)
ADD_EXECUTABLE(${PROJECT_NAME}_AttackExperiment ${CMAKE_SOURCE_DIR}/example/AttackExperiment.cpp)
ADD_EXECUTABLE(${PROJECT_NAME}_TwitterExperiment ${CMAKE_SOURCE_DIR}/example/TwitterExperiment.cpp)