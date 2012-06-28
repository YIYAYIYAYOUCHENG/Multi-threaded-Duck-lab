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
* @file StorageConfig.h
* @brief StorageConfig Interface Class
* @author Christian Nastasi c.nastasi@sssup.it, nastasichr@gmail.com
* @author Claudio Salvadori c.salvadori@sssup.it
* @version 1.0
* @date 2012-02-27
*/
#ifndef __StorageConfig_h__
#define __StorageConfig_h__

#include "Metadata.h"
#include <vector>
#include <string>

#ifdef DUCK_USE_NAMESPACE
namespace duck {
#endif

class Parser;
class Data;

class StorageConfig { 
public: 
  	StorageConfig() {}; 
  	StorageConfig(Parser& p); 
	virtual ~StorageConfig() {} ;

	void configure(const std::vector<const Data*> data);
	
	inline std::vector<std::string> get_data_subset_names() const
	{ 
		return data_subset_names;
	}; 
	inline std::vector<const Data*> get_data_subset() const 
	{ 
		return data_subset;
	}; 

	virtual std::string get_storage_name(void) const = 0;
protected:
	std::vector<const Data*> data_subset;
	std::vector<std::string> data_subset_names;
};

#ifdef DUCK_USE_NAMESPACE
};
#endif

#endif /* __StorageConfig_h__ */
