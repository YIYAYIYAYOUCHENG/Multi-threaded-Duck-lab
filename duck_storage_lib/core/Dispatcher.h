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
* @file Dispatcher.h
* @brief Data Dispatcher Sub-Module  
* @author Christian Nastasi c.nastasi@sssup.it, nastasichr@gmail.com
* @author Claudio Salvadori c.salvadori@sssup.it
* @version 1.0
* @date 2012-02-27
*/
#ifndef __Dispatcher_h__
#define __Dispatcher_h__

#include "Storage.h"
#include "Data.h"
#include "DuckStorageException.h"
#include "FormatHandler.h"
#include "DuckIO.h"

#ifdef DUCK_USE_NAMESPACE
namespace duck {
#endif

class Dispatcher { 
public: 
	Dispatcher();
	~Dispatcher();
	inline unsigned get_storage_cnt(void) const { return registry.size(); };
	inline unsigned get_data_cnt(void) const { return data_set.size(); };
	void register_data(const std::vector<const Data*> &data);
	void subscribe(Storage *r, const std::vector<std::string> &data_subset);
	void cleanup(void);
	void dispatch(void);
	/**
	*  to set up buffers for reading threads  
	*/
	void set_up_buffers(DuckIO *duck_io, unsigned long long loops);
	/**
	*  to create and start storage(reading) threads  
	*/
	std::vector<boost::thread*>* launch(void);
	/**
	*  the number of repetition for dispatching operation 
	*/
	unsigned long long loops;

private:
	struct Entry {
		Storage *storage;
		std::vector<Data*> data_subset;
		/**
		* each storage can be associated with a thread of execution
		*/
		DuckThread *storage_thread;
		DuckBuffer *buffer;
		/* 
		* Having this ''put'' method, a "struct Entry" can be
		* the destation in a reading thread.
		*/ 
		void put(std::vector<const Data*> *data_set) {
			if( !lookup) {
				std::vector<Data*>::iterator d1 = 
							data_subset.begin();
				std::vector<const Data*>::iterator d2 = 
							(*data_set).begin();
				for(; d1 != data_subset.end(); ++d1, ++d2)
					(*d1)->copy(*(*d2));
			}
			else {
				std::vector<Data*>::iterator d = 
							data_subset.begin();
				for(unsigned i = 0; d != data_subset.end();
							++d, i++) {
					unsigned p = lookup[i];
					(*d)->copy(*((*data_set)[p]));
				}
			}
			DUCK_DBG("Dispatcher: dispatch to Storage*=" <<
					std::hex << storage << std::dec);
			storage->store(data_subset);
		}
		unsigned *lookup;
	};

	std::vector<const Data*> data_set;
	std::vector<Entry> registry;
};

#ifdef DUCK_USE_NAMESPACE
};
#endif

#endif /* __Dispatcher_h__ */
