/*
 * This file is part of duck-lab.
 * 
 * Copyright (c) 
 * 2012 - Christian Nastasi, Claudio Salvadori 
 *
 * duck-lab is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * duck-lab is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301  USA
 */
/** 
* @file RootStorageConfig.h
* @brief RootStorageConfig Concrete \ref StorageConfig Class 
* @author Christian Nastasi c.nastasi@sssup.it, nastasichr@gmail.com
* @author Claudio Salvadori c.salvadori@sssup.it
* @version 1.0
* @date 2012-02-27
*/
#ifndef __RootStorageConfig_h__
#define __RootStorageConfig_h__

#include "StorageConfig.h"
#include <string>
#include <stdint.h>

#ifdef DUCK_USE_NAMESPACE
namespace duck {
#endif

class StorageConfig;
class Parser;

class RootStorageConfig: public StorageConfig {
public:
	RootStorageConfig();
	RootStorageConfig(const std::vector<const Data*> &data);
	RootStorageConfig(Parser&);

	std::string get_storage_name(void) const { return storage_name; };

	inline std::string get_tree_name(void) const {return tree_name;};
	inline std::string get_tree_title(void) const {return tree_title;};
	inline int64_t get_tree_autosave(void) const {return tree_autosave;};
	inline std::string get_event_name(void) const {return event_name;};
	inline std::string get_root_filename(void) const 
							{return root_filename;};
	inline std::string get_postproc_filename(void) const 
							{return postproc_file;};
private:
	void init_preset(Parser& p);
public:
	static const char *storage_name;
private:
	int64_t tree_autosave;
	std::string root_filename;
	std::string tree_name;
	std::string tree_title;
	std::string event_name;
	std::string postproc_file;
};

#ifdef DUCK_USE_NAMESPACE
};
#endif

#endif /* __RootStorageConfig_h__ */
