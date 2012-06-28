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
* @file StorageConfig.cpp
* @brief StorageConfig Implementation File
* @author Christian Nastasi c.nastasi@sssup.it, nastasichr@gmail.com
* @author Claudio Salvadori c.salvadori@sssup.it
* @version 1.0
* @date 2012-02-27
*/
#include "StorageConfig.h"
#include "Data.h"
#include "Parser.h"
#include "ParserException.h"
#include "DuckLog.h"

#ifdef DUCK_USE_NAMESPACE
using namespace duck;
#endif
using namespace std;

StorageConfig::StorageConfig(Parser& parser) 
{ 
	if (!parser.enter_subnode("data2storage"))
		THROW_PARSER_EXCEPTION("No 'data2store' node found by Parser: "
				       "current path was: "  << 
				       parser.print_path());
	string name;
	do {
		if (parser.get_node_name() != "data2storage") 
			continue;
		if (!parser.get_node_attribute("data_name", name))
			THROW_PARSER_EXCEPTION("No 'data_name' attribute found "
					     "by Parser for node 'data2store'");
		data_subset_names.push_back(name);
	} while (parser.enter_next_sibling());

	parser.exit_subnode();
}

void StorageConfig::configure(const vector<const Data*> data)
{
	data_subset.clear();
	vector<string>::const_iterator n = data_subset_names.begin();
	for (; n < data_subset_names.end(); n++) {
		vector<const Data*>::const_iterator d = data.begin();
		for (; d < data.end(); d++) 
			if ((*d)->get_name() == *n)
				data_subset.push_back(*d);
	}
} 
