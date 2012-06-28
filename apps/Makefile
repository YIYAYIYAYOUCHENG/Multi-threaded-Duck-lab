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
#
#  DUCK Makefile 
#
#  Version 0.1 
#  Mon Apr 12 2010 15:21 
#  Copyright  2010  Claudio Salvadori, Christian Nastasi 
#  c.salvadori@sssup.it
#  c.nastasi@sssup.it
#
################################################################################

################################################################################
#
#  This program is free software; you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation; either version 2 of the License, or
#  (at your option) any later version.
#  
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU Library General Public License for more details.
#  
#  You should have received a copy of the GNU General Public License
#  along with this program; if not, write to the Free Software
#  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
#
################################################################################

MAIN_DIR 	:= $(shell pwd)
ifndef DUCK_DIR
DUCK_DIR	:= $(MAIN_DIR)/../
endif

include $(DUCK_DIR)/Makefile.vars

INSTALL_DIR 	:= $(BIN_DIR)
SRC_DIR  	:= $(shell pwd)
SRC_FILES  	:= $(shell ls $(SRC_DIR)/*.cpp)
SUB_DIRS 	:= 

########################## 
# Compiler Option

INC_DIRS	+= $(DUCK_ALL_LIB_INC) 
CXXFLAGS 	+= ${INC_DIRS:%=-I%}
LDFLAGS		+= $(DUCK_ALL_LIB_LDFLAGS)

##########################
# Makefile Rules 

all: $(INSTALL_DIR) binaries 

$(INSTALL_DIR):
	mkdir $(INSTALL_DIR)

include Makefile.rules

clean:	recursive_clean 
	rm -rf $(INSTALL_DIR)

strip:
	@echo 
	@echo "Stripping executable(s)!"
	@echo "TODO"
	#TODO
	@echo Done!  
	@echo 

#install:
#	@echo
#	@echo "**************************************************"
#	@echo -n "Installing library: " $(DUCK_LIB) " ..."
#	@cp ./$(DUCK_LIB) ../lib/
#	@echo " done"
#	@echo "**************************************************"
#	@echo 
