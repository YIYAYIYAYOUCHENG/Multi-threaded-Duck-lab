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
* @file StreamFactory.h
* @brief Abstract factory for Stream objects (Factory Method Pattern)
* @author Christian Nastasi c.nastasi@sssup.it, nastasichr@gmail.com
* @author Claudio Salvadori c.salvadori@sssup.it
* @version 1.0
* @date 2012-02-13
*/
#ifndef __StreamFactory_h__
#define __StreamFactory_h__

#include "Stream.h"
#include "StreamConfig.h"
#include "BasicTwoWayTemplateFactory.h"

#ifdef DUCK_USE_NAMESPACE
namespace duck {
#endif

class Parser;

/** 
* @brief Register a concrete Stream product-class to the StreamFactory 
* @param id   Unique identifier in the factory registry
* @param name Concrete Stream product-class 
*/
#define DUCK_REGISTER_STREAM_CLASS(id, name) \
	DUCK_REGISTER_BASIC_TWOWAY_CLASS_PRODUCT(id, Stream, name, \
						 StreamConfig, StreamFactory)

/** 
* @brief Register a concrete StreamConfig config-class to the StreamFactory 
* @param id  Unique identifier in the factory registry
* @param cfg Concrete StreamConfig config-class 
*/
#define DUCK_REGISTER_STREAM_CONFIG_CLASS(id, cfg) \
	DUCK_REGISTER_BASIC_TWOWAY_CLASS_CONFIG(id, StreamConfig, cfg, \
						Parser&, StreamFactory)

/** 
* @brief Register both a Stream and a StreamConfig to the StreamFactory 
* @param id   Unique identifier in the factory registry
* @param name Concrete Stream product-class 
* @param cfg  Concrete StreamConfig config-class 
*/
#define DUCK_REGISTER_STREAM_CLASSES(id, name, cfg) \
	DUCK_REGISTER_BASIC_TWOWAY_CLASSES(id, Stream, name, StreamConfig, \
					   cfg, Parser&, StreamFactory)

/** 
* @brief This factory uses the patter from \ref BasicTwoWayTemplateFactory
*/
typedef BasicTwoWayTemplateFactory<Stream, StreamConfig, Parser&> StreamFactory;

#ifdef DUCK_USE_NAMESPACE
};
#endif
	
#endif //__StreamFactory_h__
