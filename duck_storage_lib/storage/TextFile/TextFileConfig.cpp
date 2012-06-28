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
* @file TextFileConfig.cpp
* @brief TextFileConfig Implementation File
* @author Christian Nastasi c.nastasi@sssup.it, nastasichr@gmail.com
* @author Claudio Salvadori c.salvadori@sssup.it
* @version 1.0
* @date 2012-02-27
*/
#include "TextFileConfig.h"
#include "StorageConfigFactory.h"
#include "ParserException.h"
#include "DuckLog.h"
 
#ifdef DUCK_USE_NAMESPACE
using namespace duck;
#endif
using namespace std;

DUCK_REGISTER_STORAGE_CONFIG_CLASS("text_file", TextFileConfig);

const char* TextFileConfig::storage_name = "text_file";

TextFileConfig::TextFileConfig()
{
	data_separator = "; ";
	data_terminator = "\n";
	out_filename = "";
	out_stream = NULL;
} 

TextFileConfig::TextFileConfig(Parser& parser) : StorageConfig(parser)
{
	if (!parser.enter_subnode())
		THROW_PARSER_EXCEPTION("No child node found by Parser: current "
				       "path was: " << parser.print_path());
	do {
		if (parser.get_node_name() == "text_preset")
			init_preset(parser);
		else if (parser.get_node_name() == "data2storage")
			init_data2storage(parser);
		/* else : TODO: what do we do for unexpected node? */
	} while (parser.enter_next_sibling());
	
	parser.exit_subnode();
	
	if (data_format.size() != data_subset_names.size())
		THROW_PARSER_EXCEPTION("data_subset_names and data_formats have"
				       " different sizes; " << 
				       data_subset_names.size() << "!=" << 
				       data_format.size());
}

void TextFileConfig::init_preset(Parser& parser)
{
	//TODO: add in the xml file the field hardcoded and 
	//implement the related methods!!
	data_separator = "; ";
	data_terminator = "\n";
	out_stream = NULL;
	if (!parser.get_node_attribute("file_name", out_filename))
		THROW_PARSER_EXCEPTION("No 'file_name' attribute found by"
				       " Parser for current node. Path was: " <<
				       parser.print_path());
	DUCK_LOG("TextFileConfig: output_file='" << out_filename << "'");	
}

void TextFileConfig::init_data2storage(Parser& parser)
{
	string s;
	StreamFormat fmt;

	if (!parser.enter_subnode("text_data_preset")) 
		goto init_data2storage_exit; // No preset, add default format

	if (parser.get_node_attribute("separator", s))
		fmt.elem_separator = s;
	if (parser.get_node_attribute("int_base", s)) {
		if (s == "hex")
			fmt.base_flag = ios_base::hex;
		else if (s == "dec")
			fmt.base_flag = ios_base::dec;
	}
	if (parser.get_node_attribute("print_ascii", s))
		fmt.char_ascii = (s == "true") ? true : false;

	parser.exit_subnode(); // Exit: text_data_preset

init_data2storage_exit:
	DUCK_DBG("TextFileConfig: DataPreset: sep='" << fmt.elem_separator <<
		 "' ios_base=" << fmt.base_flag <<
		 " print_ascii=" << fmt.char_ascii); 
	data_format.push_back(fmt);
	return; 
}
