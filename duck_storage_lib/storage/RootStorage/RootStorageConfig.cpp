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
* @file RootStorageConfig.cpp
* @brief RootStorageConfig Implementation File
* @author Christian Nastasi c.nastasi@sssup.it, nastasichr@gmail.com
* @author Claudio Salvadori c.salvadori@sssup.it
* @version 1.0
* @date 2012-02-27
*/
#include "RootStorageConfig.h"
#include "StorageConfigFactory.h"
#include "Parser.h"
#include "DuckLog.h"
#include "ParserException.h"

#ifdef DUCK_USE_NAMESPACE
using namespace duck;
#endif
using namespace std;

DUCK_REGISTER_STORAGE_CONFIG_CLASS("root_storage", RootStorageConfig);

const char* RootStorageConfig::storage_name = "root_storage";

RootStorageConfig::RootStorageConfig()
{
	tree_name = "duck_tree";
	tree_title = "Default DUCK TTree Title";
	tree_autosave = 0;
	event_name = "duck_event";
	root_filename = "duck_rootfile.root";
}

RootStorageConfig::RootStorageConfig(const vector<const Data*> &data)
{
	tree_name = "duck_tree";
	tree_title = "Default DUCK TTree Title";
	tree_autosave = 0;
	event_name = "duck_event";
	root_filename = "duck_rootfile.root";
} 

RootStorageConfig::RootStorageConfig(Parser& parser) : StorageConfig(parser)
{
	if (!parser.enter_subnode("root_preset"))
		THROW_PARSER_EXCEPTION("No 'root_preset' node found by Parser: "
				       "current path was: " << 
				       parser.print_path());
	init_preset(parser);
	parser.exit_subnode();
}

void RootStorageConfig::init_preset(Parser& parser)
{
	//TODO: add in the xml file the field hardcoded and 
	tree_name = "duck_tree";
	tree_title = "Default DUCK TTree Title";
	tree_autosave = 0;
	event_name = "duck_event";
	if (!parser.get_node_attribute("file_name", root_filename)) {
		root_filename = "duck_rootfile.root";
		DUCK_WARNING("File name returned: DEFAULT = '" <<
			     root_filename << "'");
	}
	if (!parser.get_node_attribute("gen_postproc_file", postproc_file))
		postproc_file = "";
}
