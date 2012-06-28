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
* @file DataTemplate.h
* @brief Utility template class for simple Data types
* @author Christian Nastasi c.nastasi@sssup.it, nastasichr@gmail.com
* @author Claudio Salvadori c.salvadori@sssup.it
* @version 1.0
* @date 2012-02-16
*/
#ifndef __DataTemplate_h__
#define __DataTemplate_h__

#include "Data.h"
#include "DuckException.h"
#include "DuckLog.h"

#include <string.h>

#ifdef DUCK_USE_NAMESPACE
namespace duck {
#endif

/** 
* @brief    Utility template class for simple basic data types
* @tparam T The type basic data (the \ref Data \e hidden-type)
*
* @todo     Add more descriptions.
*/
template <class T>
class DataTemplate : public Data {
public:
	/** 
	* @brief Pointer to an array of basic data
	*/
	T *data; 

	/** 
	* @brief Initialization from a \ref DataConfig 
	* 
	* Calls the \ref Data::Data(...) constructor and 
	* with the given \a cfg and setting type size to \c sizeof(T).
	* The \ref DataTemplate::data array is allocated according to the
	* lenght specified by \a cfg.
	*
	* @param cfg The configuration object
	*/
	DataTemplate(const DataConfig& cfg);
	/** 
	* @brief Initialization with initial basic data 
	*
	* Same implementation as 
	* \ref DataTemplate::DataTemplate(const DataConfig& cfg) but with 
	* initialization of the value in the basic data array.
	*
	* @param cfg The configuration object
	* @param init_data An initialization basic data buffer
	* @pre   The \a init_data buffer should contain at least a minimum 
	*        amount of basic data \a T which is equal to the lenght 
	*        specified by \a cfg.
	* @todo Shall we put a const keywork to the initialization T* array:
	*       <code>const T *init_data</code>
	*/
	DataTemplate(const DataConfig& cfg, T *init_data);
	/** 
	* @brief Destroys the basic data array 
	*/
	~DataTemplate();
	/** 
	* @brief Create a new object of the same type and with same data values
	* @return The cloned object of the same type 
	*/
	Data* clone(void) const;
	/** 
	* @brief Copy the content of the \a src object in the current one 
	* @param src The input Data object
	* @pre The input \src object MUST be of the same type and same lenght
	*      of the current object.
	* @throw DuckException if the pre-condition is not satisfied
	*/
	void copy(const Data &src);
	/** 
	* @brief Serializes the object
	* @param[out] buff A byte buffer containing the serialized object
	* @param[out] len  The lenght of \a buff in bytes
	* @note The output buffer \a buff \b MUST \b NOT be destroyed 
	*       by the caller.
	* @note The method takes care of the endianess as specified by
	*       \ref Data::serializable_endianess(...) .
	*/
	void serialize(uint8_t*& buff, unsigned long& len);
	/** 
	* @brief Deserialize the object 
	* @param[in] buff A byte buffer containing the serialized object
	* @param[in] len  The lenght of \a buff in bytes
	* @pre The number of available bytes in the buffer (\a len) should  
	*      account for the serialization header and the basic data array:
	*      \ref Data::get_serializable_header_length() \c +  
	*      \ref Metadata::get_tot_size()
	* @post The \ref Data object is properly filled with: name, length
	*       and the basic data array
	* @throw DuckException if the pre-condition is not satisfied
	*/
	void deserialize(uint8_t* buff, unsigned long len);
protected:
	/** 
	* @brief Format the basic data array from a byte buffer
	*
	* Format the basic data array \ref data from the byte array \a buff of 
	* size \a len. 
	* This method is called by the \ref Data::read_data() in 
	* the data formatting process. Therefore it is not meant to be used 
	* otherwise.
	*
	* @param buff The input byte buffer
	* @param len  The lenght of the input byte buffer
	* @return     The number of bytes used from \a buff to format the object
	* @pre        The number of available bytes (\a len) MUST be at least of
	*             \ref Metadata::get_tot_size(), which is  
	*             <code>lenght * sizeof(T)</code>.
	* @throw DuckException if the pre-condition is not satisfied
	* @note  The input \a buff is \b ALREADY endianess-converted by the 
	*        calling method \ref Data::read_data().
	* @see FormatHandler
	*/
	unsigned long format_data(const uint8_t* buff, unsigned long len);

private:
	uint8_t *serializable_buffer;
	unsigned serializable_header_len;

	DataTemplate(const DataTemplate&);

	inline void init(unsigned long len); 
	inline void data_deep_copy(T* in_data, unsigned long len); 
	inline void init_serializable_buffer(void); 
};

template <class T>
inline void DataTemplate<T>::init(unsigned long len) 
{
	serializable_buffer = NULL;
	serializable_header_len = 0;
	data = (len) ? new T[len] : NULL;
}

template <class T>
inline void DataTemplate<T>::data_deep_copy(T* in_data, unsigned long len) 
{
	if (!in_data)
		return;
	for (unsigned long i = 0; i < len; i++) 
		data[i] = in_data[i];
}
	
template <class T>
inline void DataTemplate<T>::init_serializable_buffer(void) 
{
	// If buffer is already allocated, nothing to do
	if (serializable_buffer != NULL)
		return;

	// Common Header fields
	serializable_header_len = get_serializable_header_length();

	// Allocate serialization buffer for data + header
	const unsigned len = serializable_header_len + get_tot_size();
	serializable_buffer = new uint8_t[len];

	// Add serialization header
	write_serializable_header(serializable_buffer);
}

template <class T>
DataTemplate<T>::DataTemplate(const DataConfig& c) 
	: Data(c, sizeof(T))
{ 
	init(length);
}

template <class T>
DataTemplate<T>::DataTemplate(const DataConfig& c, T *init_data) 
	: Data(c, sizeof(T))
{ 
	init(length);
	data_deep_copy(init_data, length);
}

template <class T>
DataTemplate<T>::DataTemplate(const DataTemplate& src) 
	: Data(src)
{
	init(length);
	data_deep_copy(src.data, length);
}

template <class T>
DataTemplate<T>::~DataTemplate()
{
	if (serializable_buffer != NULL);
		delete[] serializable_buffer;
	if (data != NULL)
		delete[] data;
}

template <class T>
Data* DataTemplate<T>::clone(void) const
{
	DUCK_DBG(" type=" << type <<
		 " length=" << length <<
		 " name=" << name <<
		 " data_ptr=" << std::hex << (void*) data << std::dec
	);
	// Use copy constructor
	return new DataTemplate(*this);
}

template <class T>
void DataTemplate<T>::copy(const Data &in)
{
	DUCK_DBG("SRC[" <<
		 "type=" << in.get_type() <<
		 " length=" << in.get_len() <<
		 " name=" << in.get_name() << "] and DST[" <<
		 "type=" << get_type() <<
		 " length=" << get_len() <<
		 " name=" << get_name() << "]"
	);
	// TODO: current policy is: type and length MUST match! Is this OK?
	//	 NOTE: type_size is not checked, should not be required, right?
	if (type != in.get_type())
		THROW_DUCK_EXCEPTION("Type mismatch in Data copy: " << type <<
				     " != " << in.get_type());
	if (length != in.get_len()) 
		THROW_DUCK_EXCEPTION("Length mismatch in Data copy: " << 
				     length << " != " << in.get_len());

	const DataTemplate &src = static_cast<const DataTemplate&> (in);
	// TODO: this is the other (OLD) policy: extend/reduce data length
	//if (src.length != length) {
	//	delete data;
	//	length = src.length;
	//	data = new T[length];
	//}
	data_deep_copy(src.data, length); 
}

template <class T>
unsigned long DataTemplate<T>::format_data(const uint8_t* buff, 
					   unsigned long len)
{
	// chris TODO: what do we do in the current conditions: 
	//	       if (length == 0 || data == NULL || name == "")
	//             Simply return 0? Throw exception? 
	//             Action is the same for un-named data (name=="") ?

	if (length == 0 || data == NULL)
		return 0;

	// Note: sizeof(T) == get_type_size()
	unsigned long data_len = length * sizeof(T);
	if (len < data_len)
	    	THROW_DUCK_EXCEPTION("Too few bytes to format the data object");
	memcpy(data, buff, data_len);
	return data_len;
}

template <class T>
void DataTemplate<T>::serialize(uint8_t*& buff, unsigned long& len)
{
	// Initialize the serialization buffer (if necessary) 
	init_serializable_buffer();

	DUCK_DBG(" type=" << type <<
		 " length=" << length <<
		 " name=" << name <<
		 " data_ptr=" << std::hex << (void*) data << std::dec <<
		 " serialized_header=" << serializable_header_len << "Bytes" <<
		 " serialized_data=" << get_tot_size() << "Bytes" 
	);

	// Note: length and buffer are set in the init_serializable_buffer() 
	len = serializable_header_len + get_tot_size();
	buff = serializable_buffer;

	// Copy data data in buffer and fix endianess
	memcpy(buff + serializable_header_len, data, get_tot_size());
	serializable_endianess(buff + serializable_header_len, 
			       length, get_type_size());
}

template <class T>
void DataTemplate<T>::deserialize(uint8_t* buff, unsigned long len)
{
	// Read the common header (start building the Data object)
	unsigned long header_len = read_serializable_header(buff, len);

	if (len <  header_len + get_tot_size())
		THROW_DUCK_EXCEPTION("DataTemplate: Too few bytes to " << 
				    "deserialize object: Expected = " <<
				    (header_len + get_tot_size()) <<
				    "  but len = " << len << 
				    ". In file" __FILE__ ":" << __LINE__);

	// Allocate buffer for new data (to be deserialized)
	if (data != NULL)
		delete[] data;
	data = new T[length];

	// (Deserialize) Read from buffer and fix endianess
	memcpy(data, buff + header_len, get_tot_size());
	serializable_endianess(buff + header_len, length, get_type_size());
}

#ifdef DUCK_USE_NAMESPACE
}; // namespace duck
#endif

#endif /* __DataTemplate_h__ */
