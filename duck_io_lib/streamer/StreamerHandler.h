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
* @file StreamerHandler.h
* @brief StreamerHandler Module 
* @author Christian Nastasi c.nastasi@sssup.it, nastasichr@gmail.com
* @author Claudio Salvadori c.salvadori@sssup.it
* @version 1.0
* @date 2012-02-27
*/
#ifndef __Streamerhandler_h__
#define __Streamerhandler_h__

#include <string>
#include <stdint.h>
#include <utility>

#ifdef DUCK_USE_NAMESPACE
namespace duck {
#endif

class Parser;
class Stream;
class Policy;
class Escape;

class StreamerHandler {
public:
	
	StreamerHandler(Parser &p, unsigned long packet_size);
	void read(void);
	inline uint8_t* get_rawdata() {return packet;};
	inline unsigned long get_length() {return length;};
	/**
        * @brief StreamerHandler can be the source of a buffer.
        * @return The element to be put in a buffer.
        */
        std::pair<uint8_t*, unsigned long>* get(void);

private:
	uint8_t* packet;
	unsigned long length;
	std::string stream_type;
	std::string policy_type;
	std::string escape_type;
	std::string handshake_type;
	Stream* stream;
	Policy* policy;
	Escape* escape;

	void init_stream(Parser&);
	void init_policy(Parser&);
	void init_escape(Parser&);
	void init_handshake(Parser&);
};

#ifdef DUCK_USE_NAMESPACE
}; 
#endif

#endif // __Streamerhandler_h__
