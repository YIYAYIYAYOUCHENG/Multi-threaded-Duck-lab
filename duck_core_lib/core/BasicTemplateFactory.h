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
* @file BasicTemplateFactory.h
* @brief Template class for Factory Method Pattern 
* @author Christian Nastasi c.nastasi@sssup.it, nastasichr@gmail.com
* @author Claudio Salvadori c.salvadori@sssup.it
* @version 1.0
* @date 2012-02-12
*/
#ifndef __BasicTemplateFactory_h__
#define __BasicTemplateFactory_h__

#include "DuckUtils.h"
#include "DuckException.h"
#include "DuckLog.h"

#include <map>
#include <string>

#ifdef DUCK_USE_NAMESPACE
namespace duck {
#endif

/** 
* @brief Utility macro for automatic class registration
* @param class_id      A string identifying the class in the registry.
* @param base_class    The base type (abstract product type) of the concrete 
*                      product class to be registered
* @param class_name    The name of the concrete product class to be registered
* @param config_class  The type of the configuration class
* @param factory_class The name of the abstract factory
*
* This macros registers the class constructor for to a factory registry.
* The macro is meant to be used in the class definition file (.cpp).
* For implementation details refer to \ref DUCK_REGISTER_CLASS.
*
* @note The \a class_name MUST not contain the scope resolution operator(\c ::).
*       Use \c typedef to work-around: e.g.
*       \code
*       typedef My::Complex::Scope::MyClassName Tmp;
*       DUCK_REGISTER_BASIC_CLASS("my_id", BaseClass, Tmp, CfgClass, Factory); 
*       \endcode
*/
#define DUCK_REGISTER_BASIC_CLASS(class_id, base_class, class_name, \
				  config_class, factory_class) \
DUCK_REGISTER_CLASS(class_id, base_class, class_name, factory_class, \
		    config_class)

/** 
* @brief Templated abstract factory class for Factory Method Pattern
* @tparam P The type of the abstract product
* @tparam C The type of the configuration object 
*
* This class implements a basic templated abstract factory for the 
* factory method pattern with automatic method registration.
* The creation method accepts an input configuration object to be
* used as parameter for the product construction process.
* The class exploits static object initialization to register the
* constructors in the factory registry.
*
* The signature of the constructor function MUST be of type:
* <code> P* constructor_method(C); </code>
*
* An abstract factory can be instanciated from this template class as follows:
* \code
* class ConfigClass {...};
* class AbstractProduct {...};
* // ...
* typedef BasicTemplateFactory<AbstractProduct, ConfigClass> AbstractFactory;
* \endcode
*
* The \ref DUCK_REGISTER_BASIC_CLASS macro can be used to automatically 
* generate the factory method from a given product class which has
* the required public constuctor of the same type:
* \code
* // In your ConcreteProduct.h
* // ...
* class ConcreteProduct : public AbstractProduct {
* public:
* 	ConcreteProduct(ConfigClass c) {...};
*       ...
* };
* // ...
* // In your ConcreteProduct.cpp
* // ...
* DUCK_REGISTER_BASIC_CLASS("My_ConcreteProduct_ID", AbstractProduct, ConcreteProduct, ConfigClass, AbstractFactory); 
* \endcode
*
* The concrete object creation is performed by invocation of the \ref create()
* method of the abstract factory:
* \code
* // ...
* ConfigClass config;
* AbstractProduct *p = AbstractFactory::create("My_ConcreteProduct_ID", config);
* \endcode
*     
* @note  The class contains only a static member, therefore it can be considered
*        equivalent to a singleton abstract fatory.
*/
template <typename P, typename C>
class BasicTemplateFactory {
public: // FIXME: leave the creator type as public?
	typedef P *(*creator_t)(C);
public:
	/** 
	* @brief Constructor for creators registration 
	* @param name  String identifying the type in the registry
	* @param func  Pointer to the creator function to be registered
	* @throws DuckException if a constructor is going to be registered
	*         more than once for the same \a name.
	*
	* The constructors register the given creator function with the
	* given name to the abstract factory registry. 
	* \todo  Shall we throw an exception if \a name == ""
	*/
	BasicTemplateFactory(const std::string& name, creator_t func);
	/** 
	* @brief Attempt to create the concrete product 
	* @param name  String identifying the type in the registry
	* @param param Configuration object passed to the constructor function 
	* @return A pointer to the concrete product that has been created
	* @throws DuckException if no constructor function is found in the 
	*         registry
	*
	* The abstract factory attempts to create a concrete product 
	* class of the requested type identified by \a name and 
	* with the given \a param configuration object.
	* This method looks-up in the registy and calls the constructor
	* method registered for the requested type.
	*/
	static P* create(const std::string& name, C param);
private:
	static std::map<std::string, creator_t> *registry;
};

template <typename P, typename C>
BasicTemplateFactory<P, C>::BasicTemplateFactory(const std::string& id, 
						 creator_t f)
{
	using namespace std;
	DUCK_DBG("BasicTemplateFactory: registering product constructor"
		 " method for ID=" << id << " func*=" << hex << f << dec);
	if (!registry)
		registry = new map<string, creator_t>();	
	typename map<string, creator_t>::iterator iter = registry->find(id);
	
    	if (iter != registry->end()) {
    		string s = "BasicTemplateFactory: product class "
			   "constructor already registered for ID=";
		s += id;
		throw DuckException(s);
	}
  	registry->insert(make_pair(id, f));
} 

template <typename P, typename C>
P* BasicTemplateFactory<P, C>::create(const std::string& id, C param)
{
	using namespace std;
	DUCK_DBG("BasicTemplateFactory: calling factory method "
		 "for ID=" << id);
	typename map<string, creator_t>::iterator iter = registry->find(id);
    	if (iter == registry->end()) {
		string s = "BasicTemplateFactory: Class not registered for ID=";
		s += id;
    		throw DuckException(s);
	}
	DUCK_DBG("BasicTemplateFactory: creating with parameter class: " << 
		 " creator*=" << hex << registry->at(id));
	return registry->at(id)(param);
}
	
// TODO: declaration of template static registry object. 
//       Might this definition generate a multiplew symbol definition 
//       at linking time?
template <typename P, typename C>
std::map<std::string, typename BasicTemplateFactory<P, C>::creator_t>* 
BasicTemplateFactory<P, C>::registry;

#ifdef DUCK_USE_NAMESPACE
};
#endif
	
#endif //__BasicTemplateFactory_h__
