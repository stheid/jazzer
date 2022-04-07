#!/usr/bin/env sh
# Copyright 2022 Code Intelligence GmbH
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

# List all files in the jar and exclude an allowed list of files.
# Since grep fails if there is no match, ! ... | grep ... fails if there is a
# match.
! external/local_jdk/bin/jar tf "$1" | \
  grep -v \
    -e '^build-data.properties$' \
    -e '^com/$' \
    -e '^com/code_intelligence/$' \
    -e '^com/code_intelligence/jazzer/' \
    -e '^jaz/' \
    -e '^META-INF/' \
    -e '^xlink.xsd$' \
    -e '^viz.xsd$' \
    -e '^tables.out/' \
    -e '^peephole.dat$' \
    -e '^parser.dat$' \
    -e '^lexer.dat$' \
    -e '^javabase.txt$' \
    -e '^graphml.xsd$' \
    -e '^gexf.xsd$' \
    -e '^XPP3_1.1.3.4d_b4_MIN_VERSION$' \
    -e '^.gitkeep$' \
    -e '^soot/' \
    -e '^module-info.class$'
