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
* @file DataConfig.cpp
* @brief DataConfig Implementation File
* @author Christian Nastasi c.nastasi@sssup.it, nastasichr@gmail.com
* @author Claudio Salvadori c.salvadori@sssup.it
* @version 1.0
* @date 2012-02-27
*/
#include "DataConfig.h"
#include "Parser.h"
#include "ParserException.h"

#ifdef DUCK_USE_NAMESPACE
using namespace duck;
#endif
using namespace std;

DataConfig *DataConfig::default_configurator(Parser &p)
{
	return new DataConfig(p);
}

string DataConfig::get_type(Parser &parser) 
{
	// check current xml path
	if (parser.get_node_name() != "data")
		THROW_PARSER_EXCEPTION("DataConfig: Parser error: "
				       "current node is not 'data'");
	string tmp;
	// GET data_type
	if (!parser.get_node_attribute("data_type", tmp))
		THROW_PARSER_EXCEPTION("DataConfig: Parser error: data node "
				       "with no 'data_type' attribute");
	return tmp;
}

DataConfig::DataConfig(Parser& parser)
{
	// check current xml path
	if (parser.get_node_name() != "data")
		THROW_PARSER_EXCEPTION("DataConfig: Parser error: "
				       "current node is not 'data'");
	// GET data_type
	if (!parser.get_node_attribute("data_type", type))
		THROW_PARSER_EXCEPTION("DataConfig: Parser error: data node "
				       "with no 'data_type' attribute");
	// GET data_name
	if (!parser.get_node_attribute("data_name", name))
		THROW_PARSER_EXCEPTION("DataConfig: Parser error: data node "
				       "with no 'data_name' attribute");
	// GET data_len
	string tmp;
	if (!parser.get_node_attribute("data_len", tmp))
		THROW_PARSER_EXCEPTION("DataConfig: Parser error: data node "
				       "with no 'data_len' attribute");
	length = atol(tmp.c_str());
	// GET endianess
	if (!parser.get_node_attribute("endianess", tmp))
		THROW_PARSER_EXCEPTION("DataConfig: Parser error: data node "
				       "with no 'endianess' attribute");
	if (tmp == "big-endian")
		big_endian = true;
	else if (tmp == "little-endian")
		big_endian = false;
	else
		THROW_PARSER_EXCEPTION("DataConfig: Parser error: data node "
				       "with bad 'endianess' attribute value");
}
