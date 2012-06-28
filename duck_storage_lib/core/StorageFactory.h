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
* @file StorageFactory.h
* @brief StorageFactory facility
* @author Christian Nastasi c.nastasi@sssup.it, nastasichr@gmail.com
* @author Claudio Salvadori c.salvadori@sssup.it
* @version 1.0
* @date 2012-02-27
*/
#ifndef __StorageFactory_h__
#define __StorageFactory_h__

#include "BasicTemplateFactory.h"

#define DUCK_REGISTER_STORAGE_CLASS(c_id, c_name)\
	DUCK_REGISTER_BASIC_CLASS(c_id, Storage, c_name, \
				  const StorageConfig&, StorageFactory)

#ifdef DUCK_USE_NAMESPACE
namespace duck {
#endif

class Storage; 
class StorageConfig; 

typedef BasicTemplateFactory<Storage, const StorageConfig&> StorageFactory;

#ifdef DUCK_USE_NAMESPACE
};
#endif

#endif /* __StorageFactory_h__ */
