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
* @file FormatHandler.cpp
* @brief FormatHandler Implementation File
* @author Christian Nastasi c.nastasi@sssup.it, nastasichr@gmail.com
* @author Claudio Salvadori c.salvadori@sssup.it
* @version 1.0
* @date 2012-02-27
*/
#include "FormatHandler.h"
#include "Data.h"
#include "DataFactory.h"
#include "Parser.h"
#include "ParserException.h"
#include "DuckIOException.h"
#include "DuckLog.h"

#include <set>
#include <iostream>

#ifdef DUCK_USE_NAMESPACE
using namespace duck;
#endif
using namespace std;

FormatHandler::FormatHandler(Parser& parser)
{
	DUCK_DBG("FormatHandler::FormatHandler");
	init(parser);
	final_check();
}

FormatHandler::~FormatHandler() 
{	
	DUCK_DBG("FormatHandler::~FormatHandler");
}

void FormatHandler::init(Parser& parser) 
{
	DUCK_DBG("FormatHandler::init Parsing XML");

	if (!parser.enter_subnode("data"))
		THROW_PARSER_EXCEPTION("FormatHandler: no 'data' node found by "
				       "Parser: current path was: " << 
				       parser.print_path());
	rawdata_size = 0;	
	do {
		Data *d = DataFactory::create(parser);
		data_vector.push_back(d);
		rawdata_size += d->get_tot_size();
	} while (parser.enter_next_sibling());
	parser.exit_subnode();

	DUCK_LOG("FormatHandler::rawdata_size = " << rawdata_size);
}

void FormatHandler::final_check(void)
{
	DUCK_DBG("FormatHandler::final_check");
	if (data_vector.empty())
		THROW_DUCK_IO_EXCEPTION("Empty data vector after "
					"initialization");
	// Note: use std::set do check duplicates: see std::set::insert()
	set<string> checker; 
	vector<Data*>::const_iterator i = data_vector.begin(); 
	for (; i != data_vector.end(); i++)
		if (!(checker.insert((*i)->get_name()).second))
			THROW_DUCK_IO_EXCEPTION("Duplication of data name");
}

unsigned long FormatHandler::format(uint8_t* raw, unsigned long len)
{
	unsigned long bytes_read = 0;
	vector<Data*>::iterator i = data_vector.begin(); 
	for (; i != data_vector.end(); i++) {
		if (len == 0) {
			DUCK_WARNING("No more bytes available in buffer, but"
				     " there were still Data to be formatted."
				     " Skipping remaining Data.");
			break;
		}
		unsigned long r = (*i)->read_data(raw + bytes_read, len);
		bytes_read += r;
		if (r > len)
			THROW_DUCK_IO_EXCEPTION("Last Data object has read more"
						" bytes than the available "
						"ones. Bytes remaining in "
						"buffer = " << len <<
						" Bytes read by Data = " << r);
		len -= r;
	}
	return bytes_read;
}

vector<const Data*>* FormatHandler::put_get(pair<uint8_t*, unsigned long> *elem)
{
	format(elem->first, elem->second);
	vector<const Data*>* res = new vector<const Data*>();
	vector<Data*>::iterator i = data_vector.begin();
	for(; i != data_vector.end(); i++) 
		res->push_back((*i)->clone());
	return res;
} 

