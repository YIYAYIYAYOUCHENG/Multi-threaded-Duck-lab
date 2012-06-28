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
#  duck-lab duck_storage library: Recursive Makefile (customized for ROOT)
#
#  author(s): Christian Nastasi (c.nastasi@sssup.it, nastasichr@gmail.com) 
#             Claudio Salvadori (c.salvadori@sssup.it)
#  version:   1.0 
#  date:      February 2012
################################################################################

#############################################
# CUSTOM Makefile: Using ROOT-CINT... 

ROOT_EVENT_CLASS 	:= RootStorageEvent

# TODO: This list should be created automatically
ROOT_EVENT_DEPS_H 	:= 

ROOTCINT_INCS	:=

ROOT_EVENT_CLASS_H 		:= $(ROOT_EVENT_CLASS).h
ROOT_EVENT_CLASS_DICT_CPP	:= $(ROOT_EVENT_CLASS)_Dict.cpp

BUILD_DIR		:= $(shell pwd)
ROOTCINT_DIR		:= $(BUILD_DIR)/.rootcint_tmp

#ROOTCINT_INCS	+= $(ROOT_EVENT_DEPS_H)
ROOTCINT_INCS 	+= $(ROOT_EVENT_CLASS_H)

ROOTCINT_DICT_SRCS  := $(notdir $(patsubst %.h, %_Dict.cpp, $(ROOTCINT_INCS)))

ROOTCINT_GET_SRC_H = $(patsubst %_Dict.cpp, %.h, $@)

all_with_rootcint: rootcint_all
	make -f Makefile.build

rootcint_all: $(ROOTCINT_DIR) $(ROOTCINT_DICT_SRCS)
	@echo "----------------------------------------------------------------"
	@echo -n "ROOT Dictionary files generated: "
	@echo $(ROOTCINT_DICT_SRCS) 
	rm -rf .rootcint_tmp;
	perl $(BUILD_DIR)/strip_rootcint_dict.pl\
	         --file=$(ROOT_EVENT_CLASS_DICT_CPP) --class=$(ROOT_EVENT_CLASS)
	@echo "Adding namespace to: " $(ROOT_EVENT_CLASS_DICT_CPP)
	@echo '// Add duck namespace' > .tmp;
	@echo 'namespace duck {} using namespace duck;' >> .tmp;
	cat .tmp $(ROOT_EVENT_CLASS_DICT_CPP) > .tmp2
	mv .tmp2 $(ROOT_EVENT_CLASS_DICT_CPP)
	rm -f .tmp
	@echo "----------------------------------------------------------------"

$(ROOTCINT_DIR):
	mkdir $@
	cp $(ROOTCINT_INCS) $(ROOT_EVENT_DEPS_H) $@

$(ROOT_EVENT_CLASS_DICT_CPP): $(ROOT_EVENT_CLASS_H)
	@echo "ROOTCINT $(patsubst %_Dict.cpp, %, $@)"
	cd $(ROOTCINT_DIR);\
	rootcint -f $@ -c $< $(notdir $(ROOT_EVENT_DEPS_H));\
	cp $@ $(patsubst %.cpp, %.h, $@) $(BUILD_DIR);\
	cd $(BUILD_DIR);

#%_Dict.cpp: 
#	@echo "ROOTCINT $(patsubst %_Dict.cpp, %, $@)"
#	cd $(ROOTCINT_DIR);\
#	rootcint -f $@ -c $(ROOTCINT_GET_SRC_H);\
#	cp $@ $(patsubst %.cpp, %.h, $@) $(BUILD_DIR);\
#	cd $(BUILD_DIR);

#%::
#	echo "Last-resort rule called for $@"

recursive_clean:
	rm -rf $(ROOTCINT_DICT_SRCS)\
	       $(patsubst %.cpp, %.h, $(ROOTCINT_DICT_SRCS))
	make -f Makefile.build recursive_clean
