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
* @file EscapeFactory.h
* @brief Abstract factory for Escape objects (Factory Method Pattern)
* @author Christian Nastasi c.nastasi@sssup.it, nastasichr@gmail.com
* @author Claudio Salvadori c.salvadori@sssup.it
* @version 1.0
* @date 2012-02-13
*/
#ifndef __EscapeFactory_h__
#define __EscapeFactory_h__

#include "Escape.h"
#include "EscapeConfig.h"
#include "BasicTwoWayTemplateFactory.h"

#ifdef DUCK_USE_NAMESPACE
namespace duck {
#endif

class Parser;

/** 
* @brief Register a concrete Escape product-class to the EscapeFactory 
* @param id   Unique identifier in the factory registry
* @param name Concrete Escape product-class 
*/
#define DUCK_REGISTER_ESCAPE_CLASS(id, name) \
	DUCK_REGISTER_BASIC_TWOWAY_CLASS_PRODUCT(id, Escape, name, \
						 EscapeConfig, EscapeFactory)

/** 
* @brief Register a concrete EscapeConfig config-class to the EscapeFactory 
* @param id  Unique identifier in the factory registry
* @param cfg Concrete EscapeConfig config-class 
*/
#define DUCK_REGISTER_ESCAPE_CONFIG_CLASS(id, cfg) \
	DUCK_REGISTER_BASIC_TWOWAY_CLASS_CONFIG(id, EscapeConfig, cfg, \
						Parser&, EscapeFactory)

/** 
* @brief Register both a Escape and a EscapeConfig to the EscapeFactory 
* @param id   Unique identifier in the factory registry
* @param name Concrete Escape product-class 
* @param cfg  Concrete EscapeConfig config-class 
*/
#define DUCK_REGISTER_ESCAPE_CLASSES(id, name, cfg) \
	DUCK_REGISTER_BASIC_TWOWAY_CLASSES(id, Escape, name, EscapeConfig, \
					   cfg, Parser&, EscapeFactory)

/** 
* @brief This factory uses the patter from \ref BasicTwoWayTemplateFactory
*/
typedef BasicTwoWayTemplateFactory<Escape, EscapeConfig, Parser&> EscapeFactory;

#ifdef DUCK_USE_NAMESPACE
};
#endif
	
#endif //__EscapeFactory_h__
