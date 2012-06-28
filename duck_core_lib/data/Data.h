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
* @file Data.h
* @brief Data Interface Class
* @author Christian Nastasi c.nastasi@sssup.it, nastasichr@gmail.com
* @author Claudio Salvadori c.salvadori@sssup.it
* @version 1.0
* @date 2012-02-27
*/
#ifndef __Data_h__
#define __Data_h__

#include "Metadata.h"
#include "DataConfig.h"
#include "Endianess.h"
#include <string>
#include <stdint.h>

#ifdef DUCK_USE_NAMESPACE
namespace duck {
#endif

/** 
* @brief Basic \ref Data Interface Class
*
* This class represents the interface class which describes an abstract data
* type in the duck-lab framework.
* All concrete data classes have to derive from this class and implement the 
* pure virtual methods thereof.
*
* The idea of Data class is that it is based on an internal representation of 
* the actual data that we call \b \e hidden-type.
* The interactions of other duck-lab modules with the \e hidden-type take place 
* only through the methods exported in this interface class.
* In other words, the duck-lab framework is not aware of the internal 
* representation of the data and any operation on this internal representation
* is regulated by the virtual methods of the \ref Data interface class.
*
* @see Metadata 
* @see DataConfig 
* @todo Make an example of how the Data class is meant to be derived and used!
* @todo Have the unformat_data() method (which is basically the DAQ^-1)
*/
class Data : public Metadata {
public:
	/** 
	* @brief Initializes \ref Data and \ref Metadata members 
	* @param c         The Data configuration object
	* @param type_size The size of a single \e hidden-type element
	*/
	Data(const DataConfig& c, unsigned long type_size) 
	: Metadata(c.get_type(), type_size, c.get_length()), name(c.get_name()),
	  endian(c.is_big_endian())
	{ };
	/** 
	* @brief Empty virtual destructor (polymorphism)
	*/
	virtual ~Data() {};

	/** 
	* @brief Get \ref Metadata of the current object 
	* @return The object metadata 
	* @todo Is this method really necessary? Can't we just use Data as 
	*       Metadata from the outside?
	*/
	inline const Metadata& get_metadata(void) const { return *this; };
	/** 
	* @brief  Get the data name for this instance
	* @return The data name 
	*/
	inline std::string get_name(void) const { return name; };
	/** 
	* @brief Read data from bytes 
	* 
	* This method attempts to use the bytes in the input buffer to format
	* the \e hidden-type. It fixes the endianess (order of bytes) according
	* to the specification of the source endianess 
	* (\ref DataConfig::is_big()) through the \ref Endianess module.
	* The actual formatting of the \e hidden-type is performed through the
	* \ref Data::format_data() pure virtual method. 
	* 
	* @param bytes The buffer containing the bytes to read the data
	* @param len   The length of the \a bytes buffer
	* @return The number of bytes read from the \a bytes buffer
	*/
	inline unsigned long read_data(uint8_t* bytes, unsigned long len);

	/** 
	* @brief Clone the current Data instance
	*
	* This method creates a new \ref Data object which is the exact copy of
	* the current \ref Data instance.
	* The new object Metadata, \ref name and internal \e hidden-type
	* elements MUST be equivalent to those of the current instance.
	*
	* @return A cloned object 
	* @note The derived class MUST implement this method.
	*/
	virtual Data* clone(void) const = 0;
	/** 
	* @brief Copy into the current Data instance
	* 
	* This method copies the content of the input \a src Data into the 
	* current instance. Deep-copy of the \e hidden-type elements MUST be
	* performed by this method.
	* @todo Take a decision about the relationship between src and dst 
	*       metadata. Do we want them to be the same? Do we leave that to
	*       derived class?
	* 
	* @param src The Data object from which the copy shall be made
	* @note The derived class MUST implement this method.
	*/
	virtual void copy(const Data& src) = 0;
	/** 
	* @brief Data serialization 
	* 
	* This method serializes the object into a byte array. 
	* General information about the object (\ref Metadata and \ref name) can
	* be serialized through the protected Data::write_serializable_header().
	* The specific \e hidden-type serialization MUST be performed by the
	* derived class.
	* 
	* @param[out] buff An \c new[] allocated bytes buffer containing the 
	*                  serialized object.
	* @param[out] len  The length of the \a buff buffer
	* @todo Have a Data (static) method to allocate the memory with new[] so
	*       that derived classes use that method instead of doing explicit 
	*       allocation of their own. Maybe it's not that important, though.
	* @todo We might also provide a deallocation method (for symmetry).
	* @post The \a buff is allocated and the ownership is passed to the 
	*       caller which is responsible for deallocating the memory through 
	*       delete[].
	*/
	virtual void serialize(uint8_t*& buff, unsigned long& len) = 0;
	/** 
	* @brief Data de-serialization
	* 
	* This method de-serializes the object from a byte array. 
	* General information about the object (\ref Metadata and \ref name) can
	* be de-serialized through the protected
	* Data::read_serializable_header().
	* The specific \e hidden-type de-serialization MUST be performed by the
	* derived class.
	* 
	* @param[in] buff A byte buffer containing the serialized object.
	* @param[in] len  The length of \a buff
	* @pre The \a buff buffer should contain enough bytes to de-serialize
	*      the specific Data object.
	*/
	virtual void deserialize(uint8_t* buff, unsigned long len) = 0;

protected:
	/** 
	* @brief Standard endianess conversion for serialization
	* 
	* Current implementation is little-endian. 
	* 
	* @param buff  Byte array
	* @param len   Length of \a buff
	* @param tsize Size of the \e hidden-type
	*/
	static inline void serializable_endianess(uint8_t* buff, 
						  unsigned long len, 
						  unsigned long tsize);
	/** 
	* @brief Get lenght of serialization header 
	* @return The lenght of the header in bytes 
	*/
	inline unsigned long get_serializable_header_length(void);
	/** 
	* @brief Write the serialization header to the \a buff buffer
	* @param[out] buff The output buffer to store the header
	* @pre The \a buff buffer MUST be able to contain at least
	*      Data::get_serializable_header_length() bytes
	*/
	void write_serializable_header(uint8_t* buff);
	/** 
	* @brief Initialize the \ref Data object from the serialization header
	* @param[in] buff The input buffer containing the header
	* @param[in] len  The length of \a buff in bytes
	* @return    The number of bytes read from \a buff 
	* @pre The \a buff buffer MUST be able to contain at least
	*      Data::get_serializable_header_length() bytes
	* @throw DuckException if the pre-condition is not satisfied
	*/
	unsigned long read_serializable_header(uint8_t* buff,unsigned long len);

	/** 
	* @brief Format the internal \e hidden-type elements from bytes
	* 
	* This method is called when a Data object has to be formatted from a
	* set of bytes.  
	* 
	* @param buff The input bytes buffer
	* @param len  The length of \a buff
	* @return     The number of bytes used from \ref buff to format the Data
	* @pre The input \a buff buffer should contain enough bytes to allow
	*      formatting of the specific Data class.
	*/
	virtual unsigned long format_data(const uint8_t* buff, 
					  unsigned long len) = 0;
protected:
	/*! The name associated to this Data instance (variable name) */
	std::string name;	
private:
	Endianess endian;
};

inline unsigned long Data::read_data(uint8_t* bytes, unsigned long len)
{
	// TODO: check that len >= lenght * type_size
	//       if so, the check inside format_data is useless, thus remove it
	endian.process(bytes, length, type_size);
	return format_data(bytes, len);
}

inline void Data::serializable_endianess(uint8_t* buff, unsigned long len, 
					 unsigned long tsize)
{
	Endianess::Local::to_little(buff, len, tsize);
}

inline unsigned long Data::get_serializable_header_length(void)
{
	return 	sizeof(unsigned) + 	// Data-name string lenght field
		name.size() + 		// Data-name string field
		sizeof(unsigned long); 	// Data lenght field
}

#ifdef DUCK_USE_NAMESPACE
}; // namespace duck
#endif

#endif /* __Data_h__ */
