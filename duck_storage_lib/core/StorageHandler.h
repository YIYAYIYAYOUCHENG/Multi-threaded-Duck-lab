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
* @file StorageHandler.h
* @brief StorageHandler Module 
* @author Christian Nastasi c.nastasi@sssup.it, nastasichr@gmail.com
* @author Claudio Salvadori c.salvadori@sssup.it
* @version 1.0
* @date 2012-02-27
*/
#ifndef __StorageHandler_h__
#define __StorageHandler_h__

#include <vector>
#include <string>
#include "Dispatcher.h"
#include "DuckRWThread.h"
#include "FormatHandler.h"
//#include "DuckIO.h"

#ifdef DUCK_USE_NAMESPACE
namespace duck {
#endif

class Data;
class Storage;

class StorageHandler {
public:
	StorageHandler(const std::string& xml_f, 
		       const std::vector<const Data*>& in);
	StorageHandler(const std::string& xml_f,
		       const std::string& xml_schema, 
		       const std::vector<const Data*>& in);
	~StorageHandler();
	inline void store(void) { dispatcher.dispatch(); };
	void set_up_buffers(DuckIO *duck_io, unsigned long long loops) {
		dispatcher.set_up_buffers(duck_io, loops);
	}
	std::vector<boost::thread*> * launch(void) {
		return dispatcher.launch();
	}
private:
	Dispatcher dispatcher;
	std::vector<Storage*> storages;

	void init(const std::string& xml_f,
		  const std::string& xml_schema, 
		  const std::vector<const Data*>& in);
};

#ifdef DUCK_USE_NAMESPACE
};
#endif

#endif /* __StorageHandler_h__ */
