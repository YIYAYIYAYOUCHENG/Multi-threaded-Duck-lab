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
* @file DataFactory.cpp
* @brief DataFactory Implementation File
* @author Christian Nastasi c.nastasi@sssup.it, nastasichr@gmail.com
* @author Claudio Salvadori c.salvadori@sssup.it
* @version 1.0
* @date 2012-02-27
*/
#include "DataFactory.h"
#include "DataConfig.h"
#include "DuckException.h"
#include "DuckLog.h"

// Define a short-cut for the type of std::map
#undef _MAP_TYPE
#define _MAP_TYPE std::string, DataFactory::Record

using namespace std;
#ifdef DUCK_USE_NAMESPACE
using namespace duck;
#endif

map<_MAP_TYPE> *DataFactory::registry;

DataFactory::DataFactory(const std::string &type, const std::string& class_name,
			 const std::string& inc_file,
		    	 Data *(*void_c)(void), 
			 Data *(*config_c)(const DataConfig&),
			 DataConfig *(*cfg)(Parser&) = NULL)
{
	DUCK_DBG("DataFactory: register factory methods for ID=" << type);
	if (!registry)
		registry = new map<_MAP_TYPE>();	
	map<_MAP_TYPE>::iterator iter = registry->find(type);
    	if (iter != registry->end())
		THROW_DUCK_EXCEPTION("Concrete Data class = '" << type << 
				     "' already registered");
	DataFactory::Record r;
	r.void_creator = void_c;
	r.config_creator = config_c;
	r.configurator = (cfg != NULL) ? cfg : 
					 &(DataConfig::default_configurator);
	r.class_file = inc_file; 
	r.class_name = class_name;
    	registry->insert(make_pair(type, r));
}
	
void DataFactory::fail_if_not_in_registry(const std::string& type)
{
	map<_MAP_TYPE>::iterator iter = registry->find(type);
    	if (iter == registry->end())
		THROW_DUCK_EXCEPTION("Concrete Data class = '" << type << 
				     "' was NOT registered");
}

Data *DataFactory::create(Parser &p)
{
	string type = DataConfig::get_type(p);
	DUCK_DBG("DataFactory: Attempt to call factory method ID=" << type);
	fail_if_not_in_registry(type);
	Record& r = registry->at(type);
	DUCK_DBG("DataFactory: create from Parser (XML) : " << 
		 " configurator*=" << hex << r.configurator <<
		 " creator*=" << hex << r.config_creator << dec 
	);
	DataConfig *config = r.configurator(p);
	Data *data = r.config_creator(*config);
	delete config;
	return data;
}

Data *DataFactory::create(const string& type)
{	
	DUCK_DBG("DataFactory: Attempt to call factory method ID=" << type);
	fail_if_not_in_registry(type);
	DUCK_DBG("DataFactory: call method" << 
		 " func*=" << hex << registry->at(type).void_creator << dec 
	);
 	return registry->at(type).void_creator();
}

string DataFactory::class_inc_file(const string& type)
{	
	DUCK_DBG("DataFactory: Attempt to get inc_file for ID=" << type);
	fail_if_not_in_registry(type);
 	return registry->at(type).class_file;
}

string DataFactory::class_name(const string& type)
{	
	DUCK_DBG("DataFactory: Attempt to get class_name for ID=" << type);
	fail_if_not_in_registry(type);
 	return registry->at(type).class_name;
}
