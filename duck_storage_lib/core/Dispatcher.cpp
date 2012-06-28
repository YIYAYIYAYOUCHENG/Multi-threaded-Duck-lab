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
* @file Dispatcher.cpp
* @brief Dispatcher Implementation File
* @author Christian Nastasi c.nastasi@sssup.it, nastasichr@gmail.com
* @author Claudio Salvadori c.salvadori@sssup.it
* @version 1.0
* @date 2012-02-27
*/
#include "Dispatcher.h"
#include "DuckLog.h"
#include "DuckStorageException.h"
#include <set>
#include "DuckReadingThread.h"

#ifdef DUCK_USE_NAMESPACE
using namespace duck;
#endif
using namespace std;

Dispatcher::Dispatcher()
{
}

Dispatcher::~Dispatcher()
{
	cleanup();
}

void Dispatcher::cleanup(void)
{
	DUCK_DBG("Dispatcher: clean-up");

	vector<Entry>::iterator e = registry.begin();
	for ( ; e != registry.end(); ++e) {
		if (e->lookup)
			delete[] e->lookup;
	}
	registry.clear();
	data_set.clear();
}

void Dispatcher::register_data(const std::vector<const Data*> &data)
{
	DUCK_DBG("Dispatcher: attempt to register data");

	if (get_data_cnt() != 0)
		THROW_DUCK_STORAGE_EXCEPTION("Attempt to register data twice");
	set<string> check;
	vector<const Data*>::const_iterator it = data.begin();
	for (; it != data.end(); ++it) {
		if (*it == NULL) {
			data_set.clear();
			THROW_DUCK_STORAGE_EXCEPTION("Attempt to register "
						     "NULL data");
		}
		if ((*it)->get_name() == "") {
			data_set.clear();
			THROW_DUCK_STORAGE_EXCEPTION("Attempt to register "
						     "un-named data "
						     "(name=\"\")");
		}
		pair<set<string>::iterator, bool> p;
		p = check.insert((*it)->get_name());
		if (!p.second) {
			data_set.clear();
			THROW_DUCK_STORAGE_EXCEPTION("Attempt to register data "
						     "with a name already "
						     "registered (name='" << 
						     (*it)->get_name() << "')");
		}
		DUCK_DBG("Dispatcher: register data:" <<
			 " type=" << (*it)->get_type() <<
			 " type_size=" << (*it)->get_type_size() <<
			 " len=" << (*it)->get_len() <<
			 " name=" << (*it)->get_name() 
		);
		data_set.push_back(*it);
	}
}

void Dispatcher::subscribe(Storage *r, const vector<string> &subset)
{
	DUCK_DBG("Dispatcher: attempt to subscribe a new storage");

	if (r == NULL)
		return; // TODO: Throw an exception instead?

	struct Entry e;
//	e.buffer = new DuckBuffer<vector<const Data*>*>();
	//e.storage_thread = 
	//	new DuckReadingThread<vector<const Data*>*, struct Entry>
	//					(e.buffer, &e, loops);
	e.storage = r;

	if (data_set.size() == 0 && subset.size() != 0)
		THROW_DUCK_STORAGE_EXCEPTION("Data-set is empty: maybe you are "
					     "subscribing before calling the "
					     "register_data().");

	// With this we support storages that have no input data
	if (subset.size() == 0) {
		DUCK_DBG("Dispatcher: subscribe for ALL DATA");
		e.lookup = NULL;
		vector<const Data*>::iterator it = data_set.begin();
		for (; it != data_set.end(); ++it) {
			DUCK_DBG("Dispatcher: subscribe add-data:" <<
				 " type=" << (*it)->get_type() <<
				 " type_size=" << (*it)->get_type_size() <<
				 " len=" << (*it)->get_len() <<
				 " name=" << (*it)->get_name() 
			);
			e.data_subset.push_back((*it)->clone());
		}
		registry.push_back(e);
		return;
	}

	// Subscribe for a subset of data
	e.lookup = new unsigned[subset.size()];

	vector<string>::const_iterator name = subset.begin();
	for (unsigned i = 0; name != subset.end(); ++name, i++) {
		if (*name == "") {
			delete[] e.lookup;
			THROW_DUCK_STORAGE_EXCEPTION("Attempt to subscribe for "
						     "an un-named data "
						     "(name=\"\")");
		}
		// Look for the 'name' in the complete 'data_set'
		vector<const Data*>::iterator it = data_set.begin();
		unsigned pos = 0;
		for (; it != data_set.end(); ++it, pos++) {
			if ((*it)->get_name() != *name)
				continue;
			DUCK_DBG("Dispatcher: subscribe add-data:" <<
				 " type=" << (*it)->get_type() <<
				 " type_size=" << (*it)->get_type_size() <<
				 " len=" << (*it)->get_len() <<
				 " name=" << (*it)->get_name() 
			);
			e.lookup[i] = pos;
			e.data_subset.push_back((*it)->clone());
			break;
		}
		if (pos == data_set.size()) {
			delete[] e.lookup;
			THROW_DUCK_STORAGE_EXCEPTION("Attempt to subscribe for "
						     "data name (name='" <<
						     *name << "') NOT in the "
						     "data-set");
		}
	}
	registry.push_back(e);
}
	

void Dispatcher::dispatch(void)
{
	DUCK_DBG("Dispatcher: dispatch data");

	if (get_data_cnt() == 0)
		THROW_DUCK_STORAGE_EXCEPTION("No data registered.");

	vector<Entry>::iterator e = registry.begin();
	for (; e != registry.end(); ++e) {
		if (!e->lookup) {
			vector<Data*>::iterator d1 = e->data_subset.begin();
			vector<const Data*>::iterator d2 = data_set.begin();
			for (; d1 != e->data_subset.end(); ++d1, ++d2)
				(*d1)->copy(*(*d2));
		} else {
			vector<Data*>::iterator d = e->data_subset.begin();
			for (unsigned i = 0; d != e->data_subset.end(); 
			     ++d, i++) {
				unsigned p = e->lookup[i];
				(*d)->copy(*(data_set[p]));
			}
		}
		DUCK_DBG("Dispatcher: dispatch to Storage*=" << 
			 hex << e->storage << dec);
		e->storage->store(e->data_subset);
	}
}

void Dispatcher::set_up_buffers(DuckIO *duck_io, unsigned long long loops) {
	vector<Entry>::iterator e = registry.begin();
	for(; e != registry.end(); ++e) {
		e->buffer = new
			DuckBufferTemplate<vector<const Data*>*>();
		duck_io->register_a_buffer(e->buffer);
	}
	this->loops = loops;
}

vector<boost::thread*>* Dispatcher::launch(void) {
	vector<boost::thread*>* vec = new vector<boost::thread*>();
	vector<Entry>::iterator e = registry.begin();
	for(; e != registry.end(); ++e) {
		e->storage_thread = new DuckReadingThread<
			vector<const Data*>*, struct Entry>
					(e->buffer, &(*e), loops);
		vec->push_back(e->storage_thread->launch());
	}
	return vec;
}
