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
* @file RootStorageEvent.h
* @brief Helper Class for ROOT Event Serialization 
* @author Christian Nastasi c.nastasi@sssup.it, nastasichr@gmail.com
* @author Claudio Salvadori c.salvadori@sssup.it
* @version 1.0
* @date 2012-02-27
*/
#ifndef __RootStorageEvent_h__
#define __RootStorageEvent_h__

#include "TObject.h"
#include <vector>
#include <string>

#ifdef DUCK_USE_NAMESPACE
namespace duck {
#endif

class Data;

class RootStorageEvent : public TObject {
public:
	RootStorageEvent();
	~RootStorageEvent();

	unsigned long get_event_size(void) const ;
	inline void get_data(std::vector<Data*>& vec) const
	{ 
		vec = std::vector<Data*>(data_vector_size);
		for (unsigned i = 0; i < data_vector_size; i++)
			vec[i] = data_vector[i];
	};

private:
	void update_data(const std::vector<Data*> &data);
	void init_data(const std::vector<const Data*> &data);
	void save_data(TBuffer &b, unsigned idx); 
	void load_data(TBuffer &b, unsigned idx); 

	Data **data_vector;
	unsigned data_vector_size;
	unsigned typename_string_len;
	char *typename_string_buff;

public:
	friend class RootStorage;

	ClassDef(RootStorageEvent, 1)
};

#ifdef DUCK_USE_NAMESPACE
};
#endif

#endif /* __RootStorageEvent_h__ */
