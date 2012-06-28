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
#  duck-lab Makefile 
#
#  author(s): Christian Nastasi (c.nastasi@sssup.it, nastasichr@gmail.com) 
#             Claudio Salvadori (c.salvadori@sssup.it)
#  version:   1.0 
#  date:      February 2012
################################################################################

MAIN_DIR 	:= $(shell pwd)
DUCK_DIR	:= $(MAIN_DIR)/

include $(DUCK_DIR)/Makefile.vars

# Add because of CFG_FILE
DUCK_DISTRIB_CFLAGS += -I$(call clean_path, $(DUCK_INSTALL_INC))

##########################
# Check makefile invocation

ALLOWED_MAKECMDGOALS := all install clean test cleantest doc cleandoc uninstall

ifneq ($(call variable_has_only, $(MAKECMDGOALS), $(ALLOWED_MAKECMDGOALS)),true)
$(info )
$(info ************************************************************************)
$(info ERROR while invocating 'make')
$(info Bad required goals -> $(MAKECMDGOALS))
$(info Allowed goals are  -> $(ALLOWED_MAKECMDGOALS))
$(info ************************************************************************)
$(error Invalid makefile invocation)
endif

################################################################################
#                                   Rules                                      #
################################################################################

.PHONY: all install clean test cleantest doc cleandoc
.PHONY: duck_libs_dirs duck_app duck_clean_lib_dirs duck_clean_app
.PHONY: install_inc check_build

######################### Build APPS & LIBS Rules ##############################
all: $(LIB_DIR) $(BIN_DIR) $(CFG_FILE) duck_libs_dirs duck_app 
	@echo "---------------------------------------------"
	@echo "All building is done" 
	@echo "---------------------------------------------"
	@echo "Duck properly built" > $(BUILD_LOG_FILE) 
	@date >> $(BUILD_LOG_FILE) 

$(BIN_DIR): 
	mkdir $@

$(LIB_DIR): 
	mkdir $@

$(CFG_FILE): $(DUCK_DIR)/Makefile.conf 
	$(MAKEVERBOSE)echo -n '' > $@
	$(MAKEVERBOSE)echo '// DUCK: Build Configuration' >> $@
	$(MAKEVERBOSE)echo '//       $(shell date)' >> $@
	$(MAKEVERBOSE)echo '' >> $@
	$(MAKEVERBOSE)echo '#define DUCK_INSTALL_DIR "'$(DUCK_INSTALL_DIR)'"'>>$@
	$(MAKEVERBOSE)echo '#define DUCK_INSTALL_INC "'$(DUCK_INSTALL_INC)'"'>>$@
	$(MAKEVERBOSE)echo '#define DUCK_INSTALL_LIB "'$(DUCK_INSTALL_LIB)'"'>>$@
	$(MAKEVERBOSE)echo '#define DUCK_INSTALL_BIN "'$(DUCK_INSTALL_BIN)'"'>>$@
	$(MAKEVERBOSE)echo '' >> $@
	$(MAKEVERBOSE)for i in $(DUCK_ENABLES_LIST); do\
            echo '#define '$$i >> $@ ;\
	done;
	$(MAKEVERBOSE)echo '' >> $@
	$(MAKEVERBOSE)echo '#define DUCK_CONFIG_DISTRIB_CFLAGS "'$(DUCK_DISTRIB_CFLAGS)'"'>>$@
	$(MAKEVERBOSE)echo '#define DUCK_CONFIG_DISTRIB_LDFLAGS "'$(DUCK_DISTRIB_LDFLAGS)'"'>>$@
ifdef DUCK_DAFAULT_LOG_FILE
	$(MAKEVERBOSE)echo '' >> $@
	$(MAKEVERBOSE)echo '#define DUCK_DAFAULT_LOGGER_FILENAME "'$(DUCK_DAFAULT_LOG_FILE)'"'>>$@
endif

duck_libs_dirs:
	$(MAKEVERBOSE)for i in $(DUCK_ALL_LIB_DIRS); do\
            cd $$i && make $(MAKEOPT) && cd -;\
	done;

duck_app: 
	$(MAKEVERBOSE)cd $(APP_DIR); make $(MAKEOPT); cd -;


############################# Build TESTS Rules ################################
test: all
	$(MAKEVERBOSE)cd $(TEST_DIR); make tests $(MAKEOPT); cd -;

cleantest: 
	$(MAKEVERBOSE)cd $(TEST_DIR); make clean $(MAKEOPT); cd -;


######################### Clean APPS & LIBS Rules ##############################
clean: duck_clean_lib_dirs duck_clean_app cleantest
	rm -rf $(BIN_DIR)
	rm -rf $(LIB_DIR)
	rm -f $(CFG_FILE)
	rm -f $(BUILD_LOG_FILE)

duck_clean_lib_dirs:
	$(MAKEVERBOSE)for i in $(DUCK_ALL_LIB_DIRS); do\
            cd $$i && \
	    make clean $(MAKEOPT) && \
	    cd -;\
	done;

duck_clean_app:
	$(MAKEVERBOSE)cd $(APP_DIR); make clean $(MAKEOPT); cd -;


######################### Build DOCUMENTATION Rules ############################
doc:
	$(DOXYGEN) $(DOXYGEN_CONFIG)

cleandoc:
	rm -rf doc


######################### Install APPS & LIBS Rules ############################

ACTUAL_INSTALLED_BIN_DIR := $(DUCK_INSTALL_DIR)$(call get_last_dir, $(BIN_DIR))
ACTUAL_INSTALLED_LIB_DIR := $(DUCK_INSTALL_DIR)$(call get_last_dir, $(LIB_DIR))

ACTUAL_INSTALLED_BIN_FILES := $(call list_files, $(BIN_DIR))
ACTUAL_INSTALLED_LIB_FILES := $(call list_files, $(LIB_DIR))

install: check_build $(DUCK_INSTALL_DIR) $(DUCK_INSTALL_BIN) $(DUCK_INSTALL_LIB) install_inc
	cp -rv $(BIN_DIR) $(DUCK_INSTALL_DIR)
	cp -rv $(LIB_DIR) $(DUCK_INSTALL_DIR)
	cp -rv $(DATA_DIR) $(DUCK_INSTALL_DIR)
	cp -rv $(EXAMPLE_DIR) $(DUCK_INSTALL_DIR)
	for i in $(ACTUAL_INSTALLED_BIN_FILES); do\
	    ln -fs $(ACTUAL_INSTALLED_BIN_DIR)/$$i $(DUCK_INSTALL_BIN); \
	done;
	for i in $(ACTUAL_INSTALLED_LIB_FILES); do\
	    ln -fs $(ACTUAL_INSTALLED_LIB_DIR)/$$i $(DUCK_INSTALL_LIB); \
	done;
	
check_build:
	@if [ ! -e $(BUILD_LOG_FILE) ]; then\
	    echo "";\
	    echo "Error while checking current build: "$(BUILD_LOG_FILE)" not found!";\
	    echo "Have you executed 'make' before 'make install'?";\
	    echo "";\
	    return 1;\
	fi;

# Get subtree (list of directories) excluding hidden dirs
get_subtree 	= $(shell find $1 -type d | grep "/\.[^/\.]\+" -v)
# Get files excluding hidden dirs and files
get_files 	= $(shell find $1 -type f -name $2 | grep "/\.[^/\.]\+/" -v)

CREATE_INC_DIRS 	:= $(foreach libdir, $(DUCK_ALL_LIB_DIRS), $(patsubst $(DUCK_DIR)/%, %, $(call get_subtree, $(libdir))))
ACTUAL_CREATE_INC_DIRS 	:= $(patsubst %, $(DUCK_INSTALL_INC)%, $(CREATE_INC_DIRS))
COPY_INC_FILES		:= $(foreach libdir, $(DUCK_ALL_LIB_DIRS), $(patsubst $(DUCK_DIR)/%, %, $(call get_files, $(libdir), "*.h")))

install_inc: $(DUCK_INSTALL_INC)
	for i in $(ACTUAL_CREATE_INC_DIRS); do\
	    mkdir -p $$i; \
	done;
	for i in $(COPY_INC_FILES); do\
	    cp -v $(DUCK_DIR)/$$i $(DUCK_INSTALL_INC)/$$i; \
	done;
	cp -v $(CFG_FILE) $(DUCK_INSTALL_INC)

uninstall:
	@echo "REMOVING installation files"
	rm -rf $(DUCK_INSTALL_DIR)
	rm -rf $(ACTUAL_CREATE_INC_DIRS)
	rm -rf $(DUCK_INSTALL_INC)/$(CFG_FILE) 

$(DUCK_INSTALL_DIR):
	@echo "CREATING " $@
	mkdir $@

$(DUCK_INSTALL_BIN):
	@echo "CREATING " $@
	mkdir $@

$(DUCK_INSTALL_LIB):
	@echo "CREATING " $@
	mkdir $@

$(DUCK_INSTALL_INC):
	@echo "CREATING " $@
	mkdir $@



#TODO: Write rule to strip debug simbols? maybe just before install?
