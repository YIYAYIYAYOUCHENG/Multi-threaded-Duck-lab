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
* @file RootStorageEvent.cpp
* @brief RootStorageEvent Implementation File
* @author Christian Nastasi c.nastasi@sssup.it, nastasichr@gmail.com
* @author Claudio Salvadori c.salvadori@sssup.it
* @version 1.0
* @date 2012-02-27
*/
#include "RootStorageEvent.h"

#include "Data.h"
#include "DataFactory.h"
#include "DuckLog.h"

#ifdef DUCK_USE_NAMESPACE
using namespace duck;
#endif

ClassImp(RootStorageEvent)

RootStorageEvent::RootStorageEvent()
{
	data_vector = NULL;
	data_vector_size = 0;
	// Pre-allocate space for large type names (1000 chars)
	typename_string_len = 1000;
	typename_string_buff = new char[typename_string_len];
}

RootStorageEvent::~RootStorageEvent()
{
	if (data_vector != NULL)
		delete[] data_vector;
	if (typename_string_buff != NULL)
		delete[] typename_string_buff;
}

// Serialization/Deserialization method (TObject)
void RootStorageEvent::Streamer(TBuffer &b)
{
	DUCK_DBG("RootStorageEvent: Streamer called");

	TObject::Streamer(b);

	if (b.IsReading()) {
		// DESERIALIZATION
		// Load lenght of data vector
		b >> data_vector_size; 
		DUCK_DBG("    READING MODE : data_cnt  = " << data_vector_size);
		data_vector = new Data*[data_vector_size];
		for (unsigned i = 0; i < data_vector_size; i++) 
			load_data(b, i);
	} else {
		// SERIALIZATION
		DUCK_DBG("    WRITING MODE : data_cnt  = " << data_vector_size);
		// Store lenght of data vector
		b << data_vector_size;
		for (unsigned i = 0; i < data_vector_size; i++)
			save_data(b, i);
	}
}

void RootStorageEvent::save_data(TBuffer &b, unsigned idx) 
{
	unsigned char *buff;	// data_serialized buffer
	unsigned long len;	// data_serialized buffer length
	unsigned str_len = data_vector[idx]->get_type().size();

	// Data serializes itself (allocates buff)
	data_vector[idx]->serialize(buff, len);

	// Store type_name string lenght
	b << str_len;
	// Store type_name string 
	b.WriteString(data_vector[idx]->get_type().c_str());
	// Store data_serialized buffer length
	b << len;
	// Store data_serialized buffer 
	b.WriteArray(buff, len);
	// FIXME: TODO: is buff released by WriteArray? Or shall I release it?
}

void RootStorageEvent::load_data(TBuffer &b, unsigned idx) 
{
	unsigned tmp_len;
	// Load the type name string length
	b >> tmp_len;
	if (tmp_len > typename_string_len) { 
		// If it is the case, expand the space for type names
		delete[] typename_string_buff;
		typename_string_len = tmp_len;
		typename_string_buff = new char[typename_string_len];
	}
	// Load the type name string 
	b.ReadString(typename_string_buff, typename_string_len);
	std::string tname = typename_string_buff;
	DUCK_DBG("    LOADED typename= " << tname);

	// Create the concrete object through the Factory
	Data *d = DataFactory::create(tname);

	unsigned char *buff;	// data_serialized buffer
	unsigned long len;	// data_serialized buffer length
	// Load data_serialized buffer lenght
	b >> len;
	buff = new unsigned char[len];
	// Load data_serialized buffer 
	b.ReadArray(buff);

	// Call the deserialization method of the object
	d->deserialize(buff, len);
	data_vector[idx] = d;
	delete[] buff;
}

void RootStorageEvent::update_data(const std::vector<Data*> &data)
{ 
	DUCK_DBG("RootStorageEvent: update_data");
	std::vector<Data*>::const_iterator d = data.begin();
	for (unsigned i = 0 ; i < data_vector_size; i++, ++d)
		data_vector[i]->copy(*(*d));
} 

void RootStorageEvent::init_data(const std::vector<const Data*> &data)
{ 
	DUCK_DBG("RootStorageEvent: init_data: "
		 "data_len = " << data.size());

	if (data_vector != NULL)
		delete[] data_vector;
	data_vector_size = data.size();
	data_vector = new Data*[data_vector_size];
	std::vector<const Data*>::const_iterator d = data.begin();
	for (unsigned i = 0; d != data.end(); ++d, i++)
		data_vector[i] = (*d)->clone();
}

unsigned long RootStorageEvent::get_event_size(void) const 
{
	// TODO: check if this size calculation is accurate enough!
	unsigned long d_size = 0;
	for (unsigned i = 0; i < data_vector_size; i++)
		d_size += data_vector[i]->get_tot_size();
	d_size += sizeof(RootStorageEvent);
	return d_size;
}
