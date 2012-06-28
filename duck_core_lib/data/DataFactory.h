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
* @file DataFactory.h
* @brief DataFactory Facility 
* @author Christian Nastasi c.nastasi@sssup.it, nastasichr@gmail.com
* @author Claudio Salvadori c.salvadori@sssup.it
* @version 1.0
* @date 2012-02-27
*/
#ifndef __DataFactory_h__
#define __DataFactory_h__

#include <string>
#include <map>

#ifdef DUCK_USE_NAMESPACE
namespace duck {
#endif

class Data; 
class DataConfig;
class Parser;

#define __CONCAT1(x,y) x##y
#define __CONCAT2(prefix, suffix) __CONCAT1(prefix, suffix)

#define __VOID_CREATOR(class_name) __CONCAT2(class_name, _void_creator) 
#define __CONFIG_CREATOR(class_name) __CONCAT2(class_name, _config_creator) 
#define __CONFIG_MAKER(class_config) __CONCAT2(class_config, _config_maker) 
#define __REGISTER(class_name) __CONCAT2(class_name, _register)  

#define __REGISTER_DATA_CLASS(c_name, c_id, c_config) \
static Data* __VOID_CREATOR(c_name)(void) \
{\
	return new c_name();\
}\
static Data* __CONFIG_CREATOR(c_name)(const DataConfig &cfg) \
{\
	return new c_name(cfg);\
}\
static DataFactory __REGISTER(c_name)(	c_id, \
					#c_name, #c_name".h", \
					__VOID_CREATOR(c_name),\
					__CONFIG_CREATOR(c_name),\
					c_config)

#define DUCK_REGISTER_DATA_CLASS_WITH_CONFIGURATOR(c_name, c_id, c_config) \
static DataConfig *__CONFIG_MAKER(class_config)(Parser& p)\
{\
	return new c_config(p);\
}\
__REGISTER_DATA_CLASS(c_name, c_id, __CONFIG_MAKER(c_config))

#define DUCK_REGISTER_DATA_CLASS(c_name, c_id) \
	__REGISTER_DATA_CLASS(c_name, c_id, NULL)

class DataFactory { 
private:
	struct Record {
		Data *(*void_creator)(void);
		Data *(*config_creator)(const DataConfig&);
		DataConfig *(*configurator)(Parser&);
		std::string class_file;
		std::string class_name;
	};
public: 
	DataFactory(const std::string &type, 
		    const std::string& class_name,
		    const std::string& class_file,
		    Data *(*v_creator)(void), 
		    Data *(*c_creator)(const DataConfig&),
		    DataConfig *(*cfg_maker)(Parser&));
	static Data *create(const std::string& type);
	static Data *create(Parser &p);
	static std::string class_inc_file(const std::string& type);
	static std::string class_name(const std::string& type);
private:
	static std::map<std::string, Record> *registry; 
	static void fail_if_not_in_registry(const std::string& type);
};

#ifdef DUCK_USE_NAMESPACE
}; // namespace duck
#endif

#endif /* __DataFactory_h__ */
