##############################################################################
#
# Library:   TubeTK
#
# Copyright 2010 Kitware Inc. 28 Corporate Drive,
# Clifton Park, NY, 12065, USA.
#
# All rights reserved.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#       http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
##############################################################################

# This script expects ITK_BUILD_DIR and TubeTK_BUILD_DIR to be passed as cmake
# variables.
# It verifies that ITK_BUILD_DIR and TubeTK_BUILD_DIR are set as environment
# variables and that their values match the values passed to the script.
if( NOT DEFINED ENV{ITK_BUILD_DIR})
  message(FATAL_ERROR "ITK_BUILD_DIR environment variable not set.")
endif()
get_filename_component(ITK_BUILD_DIR_ENV_ABS "$ENV{ITK_BUILD_DIR}" ABSOLUTE)
get_filename_component(ITK_BUILD_DIR_ABS "${ITK_BUILD_DIR}" ABSOLUTE)
if(NOT "${ITK_BUILD_DIR_ENV_ABS}" STREQUAL "${ITK_BUILD_DIR_ABS}")
  message(FATAL_ERROR "ITK_BUILD_DIR environment variable does "
          "not match ITK_DIR set in this project. Expected:\n"
          "${ITK_BUILD_DIR_ABS}\ngot:\n${ITK_BUILD_DIR_ENV_ABS}")
endif()
if( NOT DEFINED ENV{TubeTK_BUILD_DIR})
  message(FATAL_ERROR "TubeTK_BUILD_DIR environment variable not set.")
endif()
get_filename_component(TubeTK_BUILD_DIR_ABS "${TubeTK_BUILD_DIR}" ABSOLUTE)
get_filename_component(TubeTK_BUILD_DIR_ENV_ABS "$ENV{TubeTK_BUILD_DIR}" ABSOLUTE)
if(NOT "${TubeTK_BUILD_DIR_ENV_ABS}" STREQUAL "${TubeTK_BUILD_DIR_ABS}")
  message(FATAL_ERROR "TubeTK_BUILD_DIR environment variable does "
          "not match current directory path. Expected:\n"
          "${TubeTK_BUILD_DIR_ABS}\ngot:\n${TubeTK_BUILD_DIR_ENV_ABS}")
endif()
