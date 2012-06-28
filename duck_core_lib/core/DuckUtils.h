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
* @file DuckUtils.h
* @brief General Utilities
* @author Christian Nastasi c.nastasi@sssup.it, nastasichr@gmail.com
* @author Claudio Salvadori c.salvadori@sssup.it
* @version 1.0
* @date 2012-02-27
*/
#ifndef __DuckUtils_h__
#define __DuckUtils_h__

#define __CONCAT1(x,y) x##y
#define __CONCAT2(prefix, suffix) __CONCAT1(prefix, suffix)

#define __CREATOR(class_name) __CONCAT2(class_name, _creator) 
#define __REGISTER(class_name) __CONCAT2(class_name, _register)  

#define DUCK_REGISTER_CLASS(class_id, base_class, class_name, class_factory,\
			    constructor_param_type) \
static base_class* __CREATOR(class_name)(constructor_param_type p) \
{\
	return new class_name(p);\
};\
static class_factory __REGISTER(class_name)(class_id, __CREATOR(class_name));

#include <string>
#include <stdint.h>

#ifdef DUCK_USE_NAMESPACE
namespace duck {
#endif

namespace utils {

/** 
* @brief Converts an ASCII string to byte array
* @param[in] in   Input ASCII string treated
* @param[out] out Allocated buffer containing the converted bytes
* @param[out] len Length of the \a out array
* @post This function allocation \a len bytes through the \c new[] operator.
*       Memory ownership is passed to the caller, which has to free it throught
*       the delete[] operator. 
*/
void ascii_to_bytes(const std::string& in, uint8_t*& out, unsigned long& len);
/** 
* @brief Converts an HEX string to byte array 
* 
* The HEX string is a string which contains digits that will be interpreted as 
* hexadecimal numbers and written as bytes in the \a out buffer.
* 
* @param[in] in   Input HEX string treated
* @param[out] out Allocated buffer containing the converted bytes
* @param[out] len Length of the \a out array
* @post This function allocation \a len bytes through the \c new[] operator.
*       Memory ownership is passed to the caller, which has to free it throught
*       the delete[] operator. 
*/
void hex_to_bytes(const std::string& in, uint8_t*& out, unsigned long& len);
};

#ifdef DUCK_USE_NAMESPACE
};
#endif

#endif /* __DuckUtils_h__ */
