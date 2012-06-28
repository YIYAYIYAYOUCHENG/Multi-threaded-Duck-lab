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
* @file DataConfig.h
* @brief DataConfig Interface Class
* @author Christian Nastasi c.nastasi@sssup.it, nastasichr@gmail.com
* @author Claudio Salvadori c.salvadori@sssup.it
* @version 1.0
* @date 2012-02-27
*/
#ifndef __DataConfig_h__
#define __DataConfig_h__

#include <string>

#ifdef DUCK_USE_NAMESPACE
namespace duck {
#endif

class Parser;

/** 
* @brief Interface class for \ref Data configuration
*
* This class represents the basic configurator for \ref Data objects, since
* any \ref Data object can be created from a DataConfig object.
*
* A DataConfig class hold configuration information about the \ref Data object. 
* The DataConfig object is generally created by a factory method registered to
* the \ref DataFactory which uses a \ref Parser object to initialize the
* configuration information.
* The DataConfig object is meant to be used as configuration parameter 
* for a factory method registered to the \ref DataFactory. 
*
* This paradigm allows to decouple the logic to read the configuration 
* parameters from the XML (through the \ref Parser) from the logic of the 
* \ref Data itself.
*
* @todo Say that this class might be inherited to have configuration classes for
*       specific \ref Data classes.
* @see Data
* @see DataFactory
*/
class DataConfig { 
public: 
	/** 
	* @brief Create configuration from parameters
	* @param t The data-type name
	*/
	DataConfig(const std::string &t) 
		: type(t), name(""), big_endian(false), length(0) { };
	/** 
	* @brief Create configuration from parameters
	* @param t   The data-type name
	* @param n   The data name
	* @param len The data length
	*/
	DataConfig(const std::string &t, const std::string &n, unsigned len) 
		: type(t), name(n), big_endian(false), length(len) { };
	/** 
	* @brief Create configuration from parameters
	* @param t   The data-type name
	* @param n   The data name
	* @param be  Whether data soruce is big-endian or not
	* @param len The data length
	*/
	DataConfig(const std::string &t, const std::string &n, 
                   bool be, unsigned len) 
		: type(t), name(n), big_endian(be), length(len) { };
	inline std::string get_type(void) const {return type;};
	inline std::string get_name(void) const {return name;};
	inline bool is_big_endian(void) const {return big_endian;};
	inline unsigned get_length(void) const {return length;};

	/** 
	* @brief   Read data-type from \ref Parser 
	* @param p The XML \ref Parser object
	* @return  The data-type name read from the XML
	*/
	static std::string get_type(Parser &p);
	/** 
	* @brief   DataConfig default factory method
	* 
	* This can be used as default constructor method to be registered to the
	* \ref DataFactory for those \ref Data classes that do not require
	* a specific DataConfig class. 
	* This method uses the \ref DataConfig::DataConfig(Parser& parser)
	* constructor.
	* 
	* @param p The XML \ref Parser object
	* @return  The created DataConfig object 
	*/
	static DataConfig *default_configurator(Parser &p);
protected:
	/** 
	* @brief Read basic configuration from \ref Parser 
	*
	* This constructor uses the \a parser object to read some basic 
	* configuration parameters from the XML file.
	* The parameters read are:
	* - \ref type
	* - \ref name
	* - \ref big_endian
	* - \ref length
	*
	* @param parser The XML \ref Parser object
	*/
	DataConfig(Parser& parser);

	/*! Data-type name */
	std::string type; 
	/*! Data name */
	std::string name; 
	/*! Endianess of the source (big or not?) */
	bool big_endian;  
	/*! Data length */
	unsigned length;  
};

#ifdef DUCK_USE_NAMESPACE
}; // namespace duck
#endif

#endif /* __DataConfig_h__ */
