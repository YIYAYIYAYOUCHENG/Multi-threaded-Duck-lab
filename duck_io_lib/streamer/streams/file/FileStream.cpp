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
* @file FileStream.cpp
* @brief FileStream Implementation File
* @author Christian Nastasi c.nastasi@sssup.it, nastasichr@gmail.com
* @author Claudio Salvadori c.salvadori@sssup.it
* @version 1.0
* @date 2012-02-27
*/
#include "FileStream.h"
#include "Parser.h"
#include "StreamFactory.h"
#include "StreamException.h"
#include "ParserException.h"
#include "DuckLog.h"

#include <string.h>
#include <errno.h>

#ifdef DUCK_USE_NAMESPACE
using namespace duck;
#endif

using namespace std;

typedef FileStream::FileStreamConfig CfgClass;
DUCK_REGISTER_STREAM_CLASSES("file_stream", FileStream, CfgClass);

FileStream::FileStreamConfig::FileStreamConfig(Parser &parser)
{
	if (!parser.enter_subnode("file_stream"))
		THROW_PARSER_EXCEPTION("No 'file_stream' found by Parser: "
				       " curret path was:" << 
				       parser.print_path());
	if (!parser.get_node_attribute("input_file", file_name))
		THROW_PARSER_EXCEPTION("No 'input_file' found by Parser "
				       "for node 'file_stream' ");

	// TODO: read from XML (Parser) other paramters...

	open_mode = (ios_base::openmode)0;
	open_mode |= ios_base::in;
	open_mode |= ios_base::binary;
	//open_mode |= ios_base::app;
	//open_mode |= ios_base::ate;
	//open_mode |= ios_base::out;
	//open_mode |= ios_base::trunc; 

	parser.exit_subnode();
}

unsigned long FileStream::write(const uint8_t* packet, unsigned long length) 
{
	// TODO: do more check on file stream. Check if configured as output?
	DUCK_LOG("FileStream::write");

	file.write((const char*)packet, length);
	if (file.bad())
		THROW_STREAM_EXCEPTION_1("Error in file writing!");
	return length;
}

unsigned long FileStream::read(uint8_t* packet, unsigned long length) 
{
	// TODO: do more check on file stream. Check if configured as input?
	file.read((char*)packet, length);
	DUCK_LOG("FileStream: reading " << length << " bytes");
	if (file.eof())
		// TODO: close and release before throwing?
		THROW_STREAM_EXCEPTION_2(StreamException::EOF_REACHED, 
				         "The FileStream has reached EOF.");
	if (file.fail())
		THROW_STREAM_EXCEPTION_1("File failure while reading. "
				         "Last error was: " << strerror(errno));
	if (file.bad())
		THROW_STREAM_EXCEPTION_1("File was bad while reading. "
				         "Last error was: " << strerror(errno));
	return length;
}
