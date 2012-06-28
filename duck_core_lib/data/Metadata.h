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
* @file Metadata.h
* @brief Metadata \ref Data Base Class
* @author Christian Nastasi c.nastasi@sssup.it, nastasichr@gmail.com
* @author Claudio Salvadori c.salvadori@sssup.it
* @version 1.0
* @date 2012-02-27
*/
#ifndef __Metadata_h__
#define __Metadata_h__

#include <string>

/** 
* @brief Basic \ref Data information
*
* The core element of the duck-lab framework is the \ref Data object.
* This class represent the metadata describing the basic features of a 
* \ref Data object.
* 
* Each \ref Data class hides an internal representation of itself called 
* \e hidden-type.
* 
* The metadata information for a \ref Data class are:
* - a unique identifier describing the \ref Data class;
* - the size of a single \e hidden-type element in bytes;
* - the number of \e hidden-type elements to be instantiated.
*/
class Metadata {
public:
	/** 
	* @brief Initialize metadata 
	* 
	* @param t    The type of data
	* @param size Size of the \e hidden-type elment in bytes
	* @param len  Number of \e hidden-type elements 
	* @todo FIXME: Change std::string to const-ref
	*/
	Metadata(std::string t, unsigned long size, unsigned long len) 
		: type(t), type_size(size), length(len) { };

	/** 
	* @brief Get data-type 
	* @return The data-type
	*/
	inline std::string get_type(void) const { return type; };
	/** 
	* @brief Get the size of the \e hidden-type 
	* @return Size in bytes
	*/
	inline unsigned long get_type_size(void) const { return type_size; };
	/** 
	* @brief Get the data length 
	* @return The number of \e hidden-type elements in the current instance 
	*/
	inline unsigned long get_len(void) const { return length; };
	/** 
	* @brief Total size of the current instance 
	*        (\ref type_size times \ref length)
	* @return Size in bytes
	*/
	inline unsigned long get_tot_size(void) const 
						{ return length * type_size; };
protected:
	/*! The data-type unique ID */
	std::string type;	
	/*! The size of the \e hidden-type in bytes */
	unsigned long type_size;
	/*! Number of \e hidden-type elements in the current instance */
	unsigned long length;	
};

#endif /* __Metadata_h__ */
