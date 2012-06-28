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
* @file SerialStream.cpp
* @brief SerialStream Implementation File
* @author Christian Nastasi c.nastasi@sssup.it, nastasichr@gmail.com
* @author Claudio Salvadori c.salvadori@sssup.it
* @version 1.0
* @date 2012-02-27
*/
#include "SerialStream.h"
#include "Parser.h"
#include "StreamFactory.h"
#include "StreamException.h"
#include "ParserException.h"
#include "DuckLog.h"

#include <unistd.h>  
#include <fcntl.h>   
#include <string.h>
#include <errno.h>   
#include <sys/stat.h>
#include <sys/types.h>

#ifdef DUCK_USE_NAMESPACE
using namespace duck;
#endif

using namespace std;

typedef SerialStream::SerialStreamConfig CfgClass;
DUCK_REGISTER_STREAM_CLASSES("serial", SerialStream, CfgClass);

SerialStream::SerialStreamConfig::SerialStreamConfig(Parser &parser)
{
	if (!parser.enter_subnode("serial_stream"))
		THROW_PARSER_EXCEPTION("No 'serial_stream' found by Parser: "
				       "current path was:" << 
				       parser.print_path());
	#define __GET_ATTRIBUTE(a)\
	do {\
	if (!parser.get_node_attribute(#a, a))\
		THROW_PARSER_EXCEPTION("No '"#a"' attribute found by Parser "\
				       "for node 'serial_stream'");\
	} while(0)

	__GET_ATTRIBUTE(port_name);
	__GET_ATTRIBUTE(baud_rate);
	__GET_ATTRIBUTE(data_bits);
	__GET_ATTRIBUTE(parity_bit);
	__GET_ATTRIBUTE(stop_bits);
	#undef __GET_ATTRIBUTE	

	parser.exit_subnode();
}

SerialStream::SerialStream(const StreamConfig& cfg)
{
	const SerialStreamConfig& c = 
				dynamic_cast<const SerialStreamConfig&>(cfg);
	
	serial = open(c.port_name.c_str(), O_RDWR | O_NOCTTY | O_SYNC); 
	DUCK_DBG("We opened serial on descriptor " << serial);
	if (serial == -1) 
		THROW_STREAM_EXCEPTION_1("Error while opening serial port. "
				         "Last error was: " << strerror(errno));

	fcntl(serial, F_SETFL, 0);

	tcgetattr(serial, &options);
	options.c_cflag = 0x00;
	options.c_iflag = 0x00;
	options.c_oflag = 0x00;
	options.c_lflag = 0x00;
	options.c_line  = 0x00;
	options.c_cflag = CREAD | CLOCAL;

	init_baud_rate(c.baud_rate);
	init_data_bits(c.data_bits);
	init_parity_bit(c.parity_bit);
	init_stop_bits(c.stop_bits);

	options.c_iflag |= IGNBRK;  
	options.c_oflag |= OPOST; 
	options.c_cc[VTIME] = 0;
	options.c_cc[VMIN] = 1;

	tcsetattr(serial, TCSANOW, &options);
}

SerialStream::~SerialStream() 
{
	DUCK_DBG("SerialStream::~SerialStream");
	close(serial);
}

void SerialStream::init_baud_rate(const string& v)
{
	speed_t baud;
	if (v == "9600") 
		baud = B9600;	//0000015
	else if (v == "19200") 
		baud = B19200;	//0000016
	else if (v == "38400")
		baud = B38400; 	//0000017
	else if (v == "57600")
		baud = B57600; 	//0010001
	else if (v == "115200")
		baud = B115200; //0010002
	else
		THROW_STREAM_EXCEPTION_1("Unsupported baud rate!");
	options.c_cflag |= baud;
	cfsetispeed(&options, baud);
}

void SerialStream::init_data_bits(const string& v)
{
	if (v == "5") 
		options.c_cflag |= CS5;
	else if (v == "6") 
		options.c_cflag |= CS6;
	else if (v == "7")
		options.c_cflag |= CS7;
	else if (v == "8")
		options.c_cflag |= CS8;
	else
		THROW_STREAM_EXCEPTION_1("Unsupported number of data bits!");
}

void SerialStream::init_parity_bit(const string& v)
{
	if (v == "odd")
		options.c_cflag |= (PARENB | PARODD);
	else if (v == "even") {
		options.c_cflag |= PARENB;
		options.c_cflag &= ~PARODD;
	} else if (v == "none")
		options.c_cflag &= (~PARENB | ~PARODD);
	else
		THROW_STREAM_EXCEPTION_1("Unsupported parity bit type!");
}

void SerialStream::init_stop_bits(const string& v)
{
	if (v == "1") 
		options.c_cflag &= ~CSTOPB;
	else if (v == "2")
		options.c_cflag |= CSTOPB;
	else
		THROW_STREAM_EXCEPTION_1("Uncorrect number of stop bits!");
}

unsigned long SerialStream::write(const uint8_t* packet, unsigned long length) 
{
	DUCK_LOG("SerialStream::write: Packet size:" << dec << length);
	int n = ::write(serial, packet, length);
	if (n <= 0)
		THROW_STREAM_EXCEPTION_1("File failure while Writing. "
				         "Last error was: " << strerror(errno));
	return (unsigned long)n;
}

unsigned long SerialStream::read(uint8_t* packet, unsigned long length) 
{
	DUCK_LOG("SerialStream::read: waiting for " << length << " bytes");
	unsigned long remaining_bytes = length;
	uint8_t* pointer = packet;
	while (remaining_bytes > 0) {
		// TODO: limit the max #loops?
		int n = ::read(serial, pointer, 1);	
		if (n <= 0)
			THROW_STREAM_EXCEPTION_1("File failure while Writing. "
					         "Last error was: " << 
						 strerror(errno));
		// TODO: intercept errno and produce differt exceptions: EOF?
		pointer += n;
		if ((unsigned long) n > remaining_bytes)
			break; // Should not happen, but just in case
		remaining_bytes -= n;
	}
	return length;
}
