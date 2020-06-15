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

ADD_COMPILE_DEFINITIONS(
	SOLUTION_DIR="${CMAKE_SOURCE_DIR}/"
)

ADD_SUBDIRECTORY(src)