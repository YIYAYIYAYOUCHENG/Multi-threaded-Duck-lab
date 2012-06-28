#
# This file is part of duck-lab.
# 
# Copyright (c) 
# 2012 - Christian Nastasi, Claudio Salvadori 
#
# duck-lab is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 2.1 of the License, or (at your option) any later version.
#
# duck-lab is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public
# License along with this library; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
# MA 02110-1301  USA
#
################################################################################
#  duck-lab Tests: Makefile 
#
#  author(s): Christian Nastasi (c.nastasi@sssup.it, nastasichr@gmail.com) 
#             Claudio Salvadori (c.salvadori@sssup.it)
#  version:   1.0 
#  date:      February 2012
################################################################################

MAIN_DIR 	:= $(shell pwd)
ifndef DUCK_DIR
DUCK_DIR	:= $(MAIN_DIR)/../
endif

include $(DUCK_DIR)/Makefile.vars


##########################
# Check makefile invocation

ALLOWED_MAKECMDGOALS := all tests clean

ifneq ($(call variable_has_only, $(MAKECMDGOALS), $(ALLOWED_MAKECMDGOALS)),true)
$(info )
$(info ************************************************************************)
$(info ERROR while invocating 'make')
$(info Bad required goals -> $(MAKECMDGOALS))
$(info Allowed goals are  -> $(ALLOWED_MAKECMDGOALS))
$(info ************************************************************************)
$(error Invalid makefile invocation)
endif

##########################
# Make Variables

SRC_DIR  	:= $(shell pwd)
SRC_FILES  	:= 
SUB_DIRS 	:= duck_io_lib duck_storage_lib

TEST_LOG_FILE	:= TestResult.log

########################## 
# Compiler Option

INC_DIRS	+= $(DUCK_CORE_LIB_INC) 
INC_DIRS	+= $(DUCK_IO_LIB_INC) 
INC_DIRS	+= $(DUCK_STORAGE_LIB_INC) 

CXXFLAGS 	+= ${INC_DIRS:%=-I%}
CXXFLAGS 	+= $(DUCK_STORAGE_CXXFLAGS)

LDFLAGS		+= $(DUCK_CORE_LIB_LDFLAGS)
LDFLAGS		+= $(DUCK_IO_LIB_LDFLAGS)
LDFLAGS		+= $(DUCK_STORAGE_LIB_LDFLAGS)

##########################
# Makefile Rules 
.PHONY: all tests clean test_print_header doc 

all: binaries 

tests: clean all test_print_header recursive_tests

test_print_header:
	@echo -n "----------------------------------------" >> $(TEST_LOG_FILE)
	@echo    "----------------------------------------" >> $(TEST_LOG_FILE)
	@echo "Exetuing tests: " $(shell date) >> $(TEST_LOG_FILE)
	@echo "" >> $(TEST_LOG_FILE)

include Makefile.rules

doc:
	$(DOXYGEN) $(DOXYGEN_CONFIG)

clean:	recursive_clean 

exec_tests:
