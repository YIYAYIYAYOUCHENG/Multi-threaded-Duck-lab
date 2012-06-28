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
* @file XorEscape.h
* @brief XorEscape Concrete \ref Escape Class 
* @author Christian Nastasi c.nastasi@sssup.it, nastasichr@gmail.com
* @author Claudio Salvadori c.salvadori@sssup.it
* @version 1.0
* @date 2012-02-27
*/
#ifndef __XorEscape_h__
#define __XorEscape_h__

#include "Escape.h" 
#include "EscapeConfig.h" 

#include <cstddef>

#ifdef DUCK_USE_NAMESPACE
namespace duck {
#endif

class Parser;

class XorEscape: public Escape {
public:
	class XorEscapeConfig : public EscapeConfig {
	public:
		XorEscapeConfig(Parser&);
		XorEscapeConfig(const XorEscapeConfig&);
		~XorEscapeConfig();
	public:
		uint8_t* escape_sequence;
		unsigned long escape_length;
		// FIXME: if we want ONLY single-char remove the escape_sequence
		//        otherwise remove escape_char!
		uint8_t escape_char; 
	};

public:
	XorEscape(const EscapeConfig& c) 
	: config(dynamic_cast<const XorEscapeConfig&>(c)), read_data(NULL),
	  write_data(NULL), write_data_len(0) {};
	~XorEscape();

	void escape(const uint8_t* in, unsigned long in_len,
		    uint8_t*& out, unsigned long& out_len);
	void unescape(const uint8_t* in, unsigned long in_len,
		      uint8_t*& out, unsigned long& out_len);
protected:
	void on_configure(void);

private:
	XorEscapeConfig config;
	uint8_t* read_data;
	uint8_t* write_data;
	unsigned long write_data_len;
};

#ifdef DUCK_USE_NAMESPACE
};
#endif
#endif
