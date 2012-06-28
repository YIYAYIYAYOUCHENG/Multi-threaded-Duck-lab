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
* @file Endianess.cpp
* @brief Endianess Implementation File 
* @author Christian Nastasi c.nastasi@sssup.it, nastasichr@gmail.com
* @author Claudio Salvadori c.salvadori@sssup.it
* @version 1.0
* @date 2012-02-27
*/
#include "Endianess.h"
#include "DuckException.h"
#include "DuckLog.h"
#include <string.h>

#ifdef DUCK_USE_NAMESPACE
using namespace duck;
#endif

bool Endianess::Local::is_big;
void (*Endianess::Local::fcn_2_l)(uint8_t*, unsigned long, unsigned long);
void (*Endianess::Local::fcn_2_b)(uint8_t*, unsigned long, unsigned long);

// NOTE: this initializes the static members at program start-up!
static Endianess::Local init_object;
// TODO: Shall we implement a safer version?
//       If the static object initialization mechanism is not used to 
//       call this constructor (at startup) we still need to call it
//       before any call to either the to_local() and to_big() static methods.
//       We might add a static bool member has initialization flag, and check
//       its value in those to_*() functions, and throwing exceptions in case
// NOTE: If Endianess class is used during other static object initialization
//       the current implementation is not safe with respect to the 
//       static object initialization order fiasco.
Endianess::Local::Local()
{
	int test = 1;
	if (*((char*)(&test)) == 1) {
		is_big = false;
		DUCK_DBG("Endianess::Local::Local() HOST little-endian");
	} else  {
		is_big = true;
		DUCK_DBG("Endianess::Local::Local() HOST big-endian");
	}
	fcn_2_l = (is_big) ? (&Endianess::swap) : NULL;
	fcn_2_b = (!is_big) ? (&Endianess::swap) : NULL;
}

Endianess::Endianess(bool src_big)
{
	fcn = (Endianess::Local::is_big != src_big) ? (&Endianess::swap) : NULL;
}

void Endianess::swap(uint8_t* data, unsigned long len, unsigned long type_size)
{
	if (type_size <= 1)
		return; // No need to do endianess swap for single byte!
	uint8_t* tmp = new uint8_t[type_size];
	unsigned long i = len;
	while (i-- != 0) {
		memcpy(tmp, data, sizeof(uint8_t) * type_size);
		unsigned long j = type_size;
		while (j-- != 0) {
			*data = tmp[j];
			data++;
		}
	}
	delete[] tmp;
}
