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
* @file PolicyFactory.h
* @brief Abstract factory for Policy objects (Factory Method Pattern)
* @author Christian Nastasi c.nastasi@sssup.it, nastasichr@gmail.com
* @author Claudio Salvadori c.salvadori@sssup.it
* @version 1.0
* @date 2012-02-13
*/
#ifndef __PolicyFactory_h__
#define __PolicyFactory_h__

#include "Policy.h"
#include "PolicyConfig.h"
#include "BasicTwoWayTemplateFactory.h"

#ifdef DUCK_USE_NAMESPACE
namespace duck {
#endif

class Parser;

/** 
* @brief Register a concrete Policy product-class to the PolicyFactory 
* @param id   Unique identifier in the factory registry
* @param name Concrete Policy product-class 
*/
#define DUCK_REGISTER_POLICY_CLASS(id, name) \
	DUCK_REGISTER_BASIC_TWOWAY_CLASS_PRODUCT(id, Policy, name, \
						 PolicyConfig, PolicyFactory)

/** 
* @brief Register a concrete PolicyConfig config-class to the PolicyFactory 
* @param id  Unique identifier in the factory registry
* @param cfg Concrete PolicyConfig config-class 
*/
#define DUCK_REGISTER_POLICY_CONFIG_CLASS(id, cfg) \
	DUCK_REGISTER_BASIC_TWOWAY_CLASS_CONFIG(id, PolicyConfig, cfg, \
						Parser&, PolicyFactory)

/** 
* @brief Register both a Policy and a PolicyConfig to the PolicyFactory 
* @param id   Unique identifier in the factory registry
* @param name Concrete Policy product-class 
* @param cfg  Concrete PolicyConfig config-class 
*/
#define DUCK_REGISTER_POLICY_CLASSES(id, name, cfg) \
	DUCK_REGISTER_BASIC_TWOWAY_CLASSES(id, Policy, name, PolicyConfig, \
					   cfg, Parser&, PolicyFactory)

/** 
* @brief This factory uses the patter from \ref BasicTwoWayTemplateFactory
*/
typedef BasicTwoWayTemplateFactory<Policy, PolicyConfig, Parser&> PolicyFactory;

#ifdef DUCK_USE_NAMESPACE
};
#endif
	
#endif //__PolicyFactory_h__

