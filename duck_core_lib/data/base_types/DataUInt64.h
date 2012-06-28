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
* @file DataUInt64.h
* @brief DataUInt64 Concrete \ref Data Class 
* @author Christian Nastasi c.nastasi@sssup.it, nastasichr@gmail.com
* @author Claudio Salvadori c.salvadori@sssup.it
* @version 1.0
* @date 2012-02-27
*/
#ifndef __DataUInt64_h__
#define __DataUInt64_h__

#include "DataTemplate.h"

#include <string>
#include <stdint.h>

/** 
* @brief The data-type unique identifier (\ref Metadata::type) for DataUInt64.
*/
#define DATA_UINT64_ID "uint64"
/** 
* @brief The \e hidden-type used to instantiate DataUInt64 as \ref DataTemplate.
*/
#define DATA_UINT64_TYPE uint64_t

#ifdef DUCK_USE_NAMESPACE
namespace duck {
#endif

/** 
* @brief \ref Data concrete class for UInt64 \e hidden-type
*/
class DataUInt64 : public DataTemplate<DATA_UINT64_TYPE> {
public:
	/** 
	* @brief Create empty data 
	*/
	DataUInt64();
	/** 
	* @brief Create data from parameters 
	* @param n      The name of the Data instance
	* @param len    The lenght of the Data instance
	* @param init_d The initialization vector for the \e hidden-type
	*               containing at least Data::length elements
	*/
	DataUInt64(const std::string &n, unsigned len, DATA_UINT64_TYPE *init_d);
	/** 
	* @brief Create from configuration 
	* @param c The configuration object 
	*/
	DataUInt64(const DataConfig& c);
	/** 
	* @brief Create from configuration and initialization vector 
	* @param c         The configuration object
	* @param init_d The initialization vector for the \e hidden-type
	*               containing at least Data::length elements
	*/
	DataUInt64(const DataConfig& c, DATA_UINT64_TYPE *init_data);
	/** 
	* @brief Destructor 
	*/
	~DataUInt64();
};

#ifdef DUCK_USE_NAMESPACE
}; // namespace duck
#endif

#endif /* __DataUInt64_h__ */
