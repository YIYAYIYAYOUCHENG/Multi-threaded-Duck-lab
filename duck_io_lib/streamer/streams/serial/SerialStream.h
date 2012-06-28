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
* @file SerialStream.h
* @brief SerialStream Concrete \ref Stream Class 
* @author Christian Nastasi c.nastasi@sssup.it, nastasichr@gmail.com
* @author Claudio Salvadori c.salvadori@sssup.it
* @version 1.0
* @date 2012-02-27
*/
#ifndef __SerialStream_h__
#define __SerialStream_h__

#include "Stream.h"
#include "StreamConfig.h"

#include <string> 
#include <termios.h> 

#ifdef DUCK_USE_NAMESPACE
namespace duck {
#endif

class Parser;

class SerialStream: public Stream {
public:
	class SerialStreamConfig : public StreamConfig {
	public:
		SerialStreamConfig(Parser& p);
	public:
		std::string port_name;
		std::string baud_rate;
		std::string data_bits;
		std::string parity_bit;
		std::string stop_bits;
	};
public:
	SerialStream(const StreamConfig& cfg);
	~SerialStream();
	static Stream* create_obj(void* parser, void* node);

	unsigned long write(const uint8_t* packet, unsigned long length);
	unsigned long read(uint8_t* packet, unsigned long length);
	//inline bool is_open(void) 
	//{
	//	if(serial)
	//		return true;
	//	return false;
	//};
private:
	int serial;		
	struct termios options;
private:
	void init_baud_rate(const std::string&);
	void init_data_bits(const std::string&);
	void init_parity_bit(const std::string&);
	void init_stop_bits(const std::string&);
};

#ifdef DUCK_USE_NAMESPACE
}; // namespace duck
#endif

#endif // __SerialStream_h__
