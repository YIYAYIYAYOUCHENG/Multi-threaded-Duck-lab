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
* @file Endianess.h
* @brief Endianess Utility Class
* @author Christian Nastasi c.nastasi@sssup.it, nastasichr@gmail.com
* @author Claudio Salvadori c.salvadori@sssup.it
* @version 1.0
* @date 2012-02-27
*/
#ifndef __Endianess_h__
#define __Endianess_h__

#include <string>
#include <iostream>
#include <stdint.h>

#ifdef DUCK_USE_NAMESPACE
namespace duck {
#endif

/** 
* @brief Endianess converter class 
*
* This class allows for endianess conversion. 
* It is used by the \ref Data objects to re-order the bytes according to the 
* endianess specification of the data-source and the endianess of the 
* destination (host where the duck-lab is running).
*/
class Endianess {
public:
	/** 
	* @brief Single-tone-like conversion class 
	*/
	struct Local {
		/** 
		* @brief Determine the host endianess (at run-time)
		* @note This exploits C++ static object initialization, meaning
		*       that a fake instance of Endianess::Local class is 
		*       statically defined in Endianess.cpp just for the sake of
		*       call this constructor.
		*/
		Local();
		/** 
		* @brief Converts from host-endianess to little-endian 
		* 
		* @param uint8_t
		* @param long
		* @param long
		*/
		static inline void to_little(uint8_t*, unsigned long, 
					     unsigned long);
		/** 
		* @brief Converts from host-endianess to big-endian 
		* 
		* @param uint8_t
		* @param long
		* @param long
		*/
		static inline void to_big(uint8_t*, unsigned long, 
					  unsigned long);
	private:
		friend class Endianess;
		static bool is_big;
		static void (*fcn_2_l)(uint8_t*, unsigned long, unsigned long);
		static void (*fcn_2_b)(uint8_t*, unsigned long, unsigned long);
	};
public:	
	/** 
	* @brief Create endianess converter 
	* @param big_endian Whether or not the source is big-endian
	*/
	Endianess(bool big_endian);

	/** 
	* @brief Perform endianess conversion 
	* 
	* Conversion occurs at \a type_size level, which is: 
	* - a single basic element is assumed to be of size \a type_size
	* - byte re-ordering takes place for each basic element
	* 
	* @param data      Byte array to be converted
	* @param len       Length of the \a data array
	* @param type_size The size of a basic element
	* @todo TODO write preconditions: I think it's len multiple of type_size
	*/
	inline void process(uint8_t* data,  unsigned long len, 
			    unsigned long type_size);

private:
	void (*fcn)(uint8_t*, unsigned long, unsigned long);
	static void swap(uint8_t*,  unsigned long, unsigned long);
};

inline void Endianess::process(uint8_t* data,  unsigned long len, 
			       unsigned long type_size)
{
	if (fcn != NULL)
		fcn(data, len, type_size);
}

inline void Endianess::Local::to_little(uint8_t* data, unsigned long len,
					unsigned long type_size)
{
	if (fcn_2_l != NULL)
		fcn_2_l(data, len, type_size);
}

inline void Endianess::Local::to_big(uint8_t* data, unsigned long len,
				     unsigned long type_size)
{
	if (fcn_2_b != NULL)
		fcn_2_b(data, len, type_size);
}

#ifdef DUCK_USE_NAMESPACE
}; // namespace duck
#endif

#endif /* __Endianess_h__ */
