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
* @file Escape.h
* @brief Escape Interface Class 
* @author Christian Nastasi c.nastasi@sssup.it, nastasichr@gmail.com
* @author Claudio Salvadori c.salvadori@sssup.it
* @version 1.0
* @date 2012-02-27
*/
#ifndef __Escape_h__
#define __Escape_h__

#include <stdint.h>

#ifdef DUCK_USE_NAMESPACE
namespace duck {
#endif

class Escape {
public:
	inline void configure(unsigned long plain_data_size) 
	{ 
		expected_data_size = plain_data_size;
		on_configure();
	};
	virtual void escape(const uint8_t* in, unsigned long in_len,
			    uint8_t*& out, unsigned long& out_len) = 0;
	virtual void unescape(const uint8_t* in, unsigned long in_len,
			      uint8_t*& out, unsigned long& out_len) = 0;
protected:
	virtual void on_configure(void) {};

	unsigned long expected_data_size;
};

#ifdef DUCK_USE_NAMESPACE
};
#endif
#endif
