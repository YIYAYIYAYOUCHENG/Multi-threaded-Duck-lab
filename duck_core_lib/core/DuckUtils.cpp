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
* @file DuckUtils.cpp
* @brief DuckUtils Implementation File
* @author Christian Nastasi c.nastasi@sssup.it, nastasichr@gmail.com
* @author Claudio Salvadori c.salvadori@sssup.it
* @version 1.0
* @date 2012-02-27
*/
#include "DuckUtils.h"
#include <sstream>
#include <string.h>

#ifdef DUCK_USE_NAMESPACE
using namespace duck;
#endif

using namespace std;
using namespace duck::utils;

void duck::utils::ascii_to_bytes(const string& in, uint8_t*& out, 
				 unsigned long & out_len)
{
	out_len = in.size();
	out = new uint8_t[out_len];
	memcpy(out, in.c_str(), out_len);
}

void duck::utils::hex_to_bytes(const string& in, uint8_t*& out, 
			       unsigned long& out_len)
{
	out_len = in.size() / 2;
	out = new uint8_t[out_len];
	uint8_t *byte = out;
	for (unsigned long i = 0; i < in.size(); i += 2) {
		istringstream ss(in.substr(i, 2));
		ss >> hex >> byte;
		byte++;
	}
}
