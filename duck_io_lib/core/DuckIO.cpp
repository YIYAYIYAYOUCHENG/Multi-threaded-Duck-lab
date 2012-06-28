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
* @file DuckIO.cpp
* @brief DuckIO Implementation File
* @author Christian Nastasi c.nastasi@sssup.it, nastasichr@gmail.com
* @author Claudio Salvadori c.salvadori@sssup.it
* @version 1.0
* @date 2012-02-27
*/
#include "DuckIO.h"
#include "Data.h"
#include "Parser.h"
#include "ParserException.h"
#include "DuckConfig.h"
#include "DuckLog.h"
#include "FormatHandler.h"
#include "StreamerHandler.h"
#include "DuckWritingThread.h"
#include "DuckRWThread.h"

#undef DUCK_SCHEMA
#define DUCK_SCHEMA DUCK_INSTALL_DIR"data/duck_schema.xsd" 

#ifdef DUCK_USE_NAMESPACE
using namespace duck;
#endif
using namespace std;

DuckIO::DuckIO(const string& xml_file)
{
	// Create Parser from XML file
	Parser parser(DUCK_SCHEMA, xml_file, "");

	// Read <definition> node (title)
	parser.enter_subnode("definition");		// TODO: ignore retval?
	if (!parser.get_node_attribute("title", title))
		THROW_PARSER_EXCEPTION("title attribute not found in XML!");
	DUCK_LOG("DuckIO::init: title: " << title);
	parser.exit_subnode();

	// Crate FormaHandler from <message_description> node
	parser.enter_subnode("message_description");	// TODO: ignore retval?
	formatter = new FormatHandler(parser);
	parser.exit_subnode();

	unsigned long expected_bytes = formatter->get_data_requested_bytes();

	// Crate StreamerHandler from <stream_definition> node
	parser.enter_subnode("stream_definition");	// TODO: ignore retval?
	streamer = new StreamerHandler(parser,  expected_bytes);
	parser.exit_subnode();

	vector<Data*>::const_iterator d = formatter->get_data_vector().begin();
	for (; d != formatter->get_data_vector().end(); d++)
		 const_data_vector.push_back(*d);
}

DuckIO::~DuckIO()
{
	if(formatter)
		delete formatter;
	if(streamer)
		delete streamer;
	if( streamer_thread)
		delete streamer_thread;
	if( formatter_thread)
		delete formatter_thread;
	vector<DuckBuffer*>::iterator it;
	for( it = streaming_buffers.begin(); 
			it != streaming_buffers.end(); ++it)
		if( !(*it))
			delete *it; 
	for( it = formatting_buffers.begin(); 
			it != formatting_buffers.end(); ++it)
		if( !(*it))
			delete *it; 
}

void DuckIO::set_up_buffers(unsigned long long count)
{

	DuckBuffer *streaming_buffer =	
		new DuckBufferTemplate<pair<uint8_t, unsigned long>*>();

	streaming_buffers.push_back(streaming_buffer);

	loops = count;
}

boost::thread* DuckIO::launch_streaming(void) {
	streamer_thread =
	new DuckWritingThread<std::pair<uint8_t*, unsigned long>*, 
			StreamerHandler> (streamer, &streaming_buffers, loops);

	return streamer_thread->launch();
}

boost::thread* DuckIO::launch_formatting(void) {
	formatter_thread =
		new DuckRWThread<std::pair<uint8_t*, unsigned long>*, 
			FormatHandler, std::vector<const Data*>*> (
		streaming_buffers[0], formatter, &formatting_buffers, loops);
	return formatter_thread->launch();
}

void DuckIO::register_a_buffer(DuckBuffer *buffer) {
	formatting_buffers.push_back(buffer);
}


vector<Data*>& DuckIO::get_data_vector(void) 
{
	return formatter->get_data_vector();
}

vector<const Data*>& DuckIO::get_const_data_vector(void) 
{
	return const_data_vector;
}

void DuckIO::read(void)
{
	streamer->read();
	formatter->format(streamer->get_rawdata(), streamer->get_length());
	// TODO: ignoring reaturn value... does it matter?
}
