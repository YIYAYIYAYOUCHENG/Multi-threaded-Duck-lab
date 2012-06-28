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
* @file DuckLog.cpp
* @brief DuckLog Implementation File
* @author Christian Nastasi c.nastasi@sssup.it, nastasichr@gmail.com
* @author Claudio Salvadori c.salvadori@sssup.it
* @version 1.0
* @date 2012-02-27
*/
#include "DuckLog.h"
#include <fstream>
#include <ctime>

#ifdef DUCK_USE_NAMESPACE
using namespace duck;
#endif
using namespace std;

Logger* Logger::Default::def_logger = NULL;

ofstream* Logger::fake_stream = NULL;

Logger::Logger() : is_active(true), is_my_stream(false) 
{
	if (!fake_stream)
		fake_stream = new ofstream();
	logger_stream = &(clog);
}
Logger::Logger(const string& f) : is_active(true), is_my_stream(true) 
{
	if (!fake_stream)
		fake_stream = new ofstream();
	logger_stream = new ofstream(f.c_str(), 
				     ios_base::out | ios_base::app);
	print_header();
}
Logger::Logger(ostream& os) : is_active(true), is_my_stream(false) 
{
	if (!fake_stream)
		fake_stream = new ofstream();
	logger_stream = &(os);
}
Logger::~Logger()
{
	logger_stream->flush();
	if (is_my_stream)
		delete logger_stream;
}

void Logger::print_header(void)
{
	ostream& os = *logger_stream;
	time_t t;
	time(&t);

	// TODO: print more informations? Log filename?
	os << "****************************************" << endl;
	os << "* duck-lab lib" << endl;
	os << "* Log: " << ctime(&t); // ctime() puts a trailing '\n'
	os << "****************************************" << endl;
}
	
Logger::Default::~Default()
{
	def_logger->stream() << endl; // Add '\n' at the end of default log
	if (def_logger)
		delete def_logger;
}

// NOTE: trick to force calling the destructor at the end of program execution
static class _fake_class_to_call_destructor
{
	Logger::Default df;
} _fake ;
