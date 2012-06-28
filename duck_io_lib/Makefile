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
#  duck-lab duck_io library: Makefile 
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
# Make Variables

SRC_DIR  	:= $(shell pwd)
SRC_FILES  	:= 
SUB_DIRS 	:= core formatter streamer  

INSTALL_TARGET	:= $(LIB_DIR)/$(DUCK_IO_LIB)

########################## 
# Compiler Option

INC_DIRS	+= $(DUCK_CORE_LIB_INC) 
INC_DIRS	+= $(DUCK_THREADS_LIB_INC) 
INC_DIRS	+= $(DUCK_IO_LIB_INC) 

CXXFLAGS 	+= ${INC_DIRS:%=-I%}
LDFLAGS		+= $(DUCK_CORE_LIB_LDFLAGS)
LDFLAGS		+= $(DUCK_THREADS_LIB_LDFLAGS)

##########################
# Makefile Rules 

all: $(INSTALL_TARGET) 

include Makefile.rules

$(INSTALL_TARGET): $(SUB_LIBS)
	@echo "**************************************************"
	@echo -n "Building Library:" $@ " ... "
	$(MAKEVERBOS)$(LD) $(LDFLAGS) -shared $^ -o $@ 
	@echo "done"
	@echo "**************************************************"

clean:	recursive_clean 

