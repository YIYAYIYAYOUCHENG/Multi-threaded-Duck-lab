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
* @file BasicTwoWayTemplateFactory.h
* @brief Utility template class for a 'two-way' factory pattern
* @author Christian Nastasi c.nastasi@sssup.it, nastasichr@gmail.com
* @author Claudio Salvadori c.salvadori@sssup.it
* @version 1.0
* @date 2012-02-12
*/
#ifndef __BasicTwoWayTemplateFactory_h__
#define __BasicTwoWayTemplateFactory_h__

#include "BasicTemplateFactory.h"
#include "DuckException.h"
#include "DuckLog.h"

#include <map>
#include <string>

#ifdef DUCK_USE_NAMESPACE
namespace duck {
#endif

/** 
* @brief Product-class registration in a 'two-way' factory
* @param class_id    A string identifying the class in the registry
* @param base_class  Abstract product class 
* @param class_name  Concrete product class to be registered
* @param base_config Abstract configuration class
* @param factory     Abstract Two-Way Factory
*
* This utility macro registers the concrete product class to 
* the two-way factory. This macro wraps \ref DUCK_REGISTER_BASIC_CLASS, 
* changing the type of the \a base_config parameter to
* a const reference of the same type.
* \todo Shall we keep the const-reference forcing in this macro
*/
#define DUCK_REGISTER_BASIC_TWOWAY_CLASS_PRODUCT(class_id, base_class, \
						 class_name, base_config, \
						 factory) \
DUCK_REGISTER_BASIC_CLASS(class_id, base_class, class_name, const base_config&,\
			  factory)


/** 
* @brief Config-class registration in a 'two-way' factory
* @param class_id          A string identifying the class in the registry
* @param base_config       Abstract config class 
* @param config_class_name Concrete config class to be registered
* @param param             Parameter class for config class creation
* @param factory           Abstract Two-Way Factory
*
* This utility macro registers the concrete config class to 
* the two-way factory. This macro wraps \ref DUCK_REGISTER_BASIC_CLASS, 
* the \a param is passed as it is.
*/
#define DUCK_REGISTER_BASIC_TWOWAY_CLASS_CONFIG(class_id, base_config, \
						config_class_name, param, \
						factory) \
DUCK_REGISTER_BASIC_CLASS(class_id, base_config, config_class_name, param, \
			  factory)


/** 
* @brief Config- and produc-class registration in a 'two-way' factory
* @param class_id          A string identifying the class in the registry
* @param base_class        Abstract product class 
* @param class_name        Concrete product class to be registered
* @param base_config       Abstract config class 
* @param config_class_name Concrete config class to be registered
* @param param             Parameter class for config class creation
* @param factory           Abstract Two-Way Factory
*
* The macro uses the combination of \ref DUCK_REGISTER_BASIC_TWOWAY_CLASS_CONFIG
* and \ref DUCK_REGISTER_BASIC_TWOWAY_CLASS_PRODUCT.
*/
#define DUCK_REGISTER_BASIC_TWOWAY_CLASSES(class_id, base_class, class_name, \
					   base_config, config_class_name, \
					   param, factory) \
DUCK_REGISTER_BASIC_TWOWAY_CLASS_CONFIG(class_id, base_config, \
					config_class_name, param, factory);\
DUCK_REGISTER_BASIC_TWOWAY_CLASS_PRODUCT(class_id, base_class, class_name, \
					 base_config, factory)

/** 
* @brief Templated two-way abstract factory for Factory Method Pattern 
* @tparam PROD       The type of the abstract product
* @tparam CONF       The type of the abstract config
* @tparam CONF_PARAM The type of the parameter for config creation
*
* This is an utility template class to be used for a two-way 
* factory method pattern. This class uses \ref BasicTemplateFactory.
* 
* The 'two-way' factory method pattern consists of two consecutive usage
* of a normal factory method pattern:
* - in the first step a \a CONF class is created from the \a CONF_PARAM 
*   parameter through a \c BasicTemplateFactory<CONF, CONF_PARAM>;
* - in the second step a \a PROD class is created from a
*   <code> const \a CONF & </code> to the object created with the previous step
*   through a \c BasicTemplateFactory<PROD, const CONF &>.
*
* The signature of the constructor functions for config and product class 
* MUST be respectively of type:
* - <code> CONF* constructor_method(CONF_PARAM); </code>
* - <code> PROD* product_constructor_method(const CONF &); </code>
*
* An abstract two-way factory can be instanciated from this template class as 
* follows:
* \code
* class Parameter {...};
* class AbstractConfig {...};
* class AbstractProduct {...};
* // ...
* typedef BasicTwoWayTemplateFactory<AbstractProduct, AbstractConfig, Parameter> AbstractTwoWayFactory;
* \endcode
*
* The \ref DUCK_REGISTER_BASIC_TWOWAY_CLASS_CONFIG and \ref 
* DUCK_REGISTER_BASIC_TWOWAY_CLASS_PRODUCT macros can be used to automatically 
* generate the factory methods respectively from a given config and a given 
* product classes having the required public constuctors:
* \code
* // In your ConcreteConfig.h
* // ...
* class ConcreteConfig : public AbstractConfig {
* public:
* 	ConcreteConfig(Parameter param) {...};
*       ...
* };
* // In your ConcreteProduct.h
* // ...
* class ConcreteProduct : public AbstractProduct {
* public:
* 	ConcreteProduct(const AbstractConfig c&) {
* 		//possible dynamic_cast<const ConcreteConfig&>
*		...
*	};
*       ...
* };
* // ...
* // In your ConcreteConfig.cpp
* // ...
* DUCK_REGISTER_BASIC_TWOWAY_CLASS_CONFIG("My_ConcreteProduct_ID", AbstractConfig, ConcreteConfig, Parameter, AbstractTwoWayFactory); 
* // In your ConcreteProduct.cpp
* // ...
* DUCK_REGISTER_BASIC_TWOWAY_CLASS_PRODUCT("My_ConcreteProduct_ID", AbstractProduct, ConcreteProduct, AbstractConfig, AbstractTwoWayFactory); 
* \endcode
*
* The concrete object creation is performed by invocation of the \ref create()
* method of the abstract factory:
* \code
* // ...
* Parameter param;
* AbstractProduct *p = AbstractTwoWayFactory::create("My_ConcreteProduct_ID", param);
* \endcode
*     
* @note The identifiers (strings) used to register the \a CONF and \a PROD 
*       concrete classes MUST be the same, since the \ref create() method uses 
*       a unique identifier to look-up in the factory registry.
*
* @see BasicTemplateFactory
*
*/
template <typename PROD, typename CONF, typename CONF_PARAM>
class BasicTwoWayTemplateFactory {
public: // FIXME: leave the creators type as public?
	typedef PROD *(*creator_t)(const CONF&);
	typedef CONF *(*configurator_t)(CONF_PARAM);
public:
	/** 
	* @brief Constructor for product-creators registration 
	* @param name  String identifying the type in the registry
	* @param func  Pointer to the creator function to be registered
	* @throws DuckException if a constructor is going to be registered
	*         more than once for the same \a name.
	*
	* The constructors register the given creator function with the
	* given name to the abstract factory registry. 
	* \todo  Shall we throw an exception if \a name == ""
	*/
	BasicTwoWayTemplateFactory(const std::string& name, creator_t f);
	/** 
	* @brief Constructor for config-creators registration 
	* @param name  String identifying the type in the registry
	* @param func  Pointer to the creator function to be registered
	* @throws DuckException if a constructor is going to be registered
	*         more than once for the same \a name.
	*
	* The constructors register the given creator function with the
	* given name to the abstract factory registry. 
	* \todo  Shall we throw an exception if \a name == ""
	*/
	BasicTwoWayTemplateFactory(const std::string& name, configurator_t f);
	/** 
	* @brief Attempt to create the concrete product 
	* @param name  String identifying the type in the registry
	* @param param Parameter object used by the two-way pattern 
	* @return A pointer to the concrete product that has been created
	* @throws DuckException if no constructor function is found in the 
	*         registry for either the config or product case
	*
	* The abstract factory attempts to create a concrete product 
	* class of the requested type identified by \a name and 
	* with the given \a param parameter object.
	* This method looks-up in the registy and calls the constructor
	* method registered for the requested type.
	* The intermediate config product is deleted before returning.
	*/
	static PROD* create(const std::string& name, CONF_PARAM param);
};

template <typename PROD, typename CONF, typename CONF_PARAM>
BasicTwoWayTemplateFactory<PROD, CONF, CONF_PARAM>::
BasicTwoWayTemplateFactory(const std::string& name, creator_t func)
{
	BasicTemplateFactory<PROD, const CONF&>(name, func);
}

template <typename PROD, typename CONF, typename CONF_PARAM>
BasicTwoWayTemplateFactory<PROD, CONF, CONF_PARAM>::
BasicTwoWayTemplateFactory(const std::string& name, configurator_t func)
{
	BasicTemplateFactory<CONF, CONF_PARAM>(name, func);
}

template <typename PROD, typename CONF, typename CONF_PARAM>
PROD* 
BasicTwoWayTemplateFactory<PROD, CONF, CONF_PARAM>::
create(const std::string& n, CONF_PARAM p)
{
	CONF *c = BasicTemplateFactory<CONF, CONF_PARAM>::create(n, p);
	PROD *r = BasicTemplateFactory<PROD, const CONF&>::create(n, *c);
	delete c;
	return r;
}

#ifdef DUCK_USE_NAMESPACE
};
#endif
	
#endif //__BasicTwoWayTemplateFactory_h__
