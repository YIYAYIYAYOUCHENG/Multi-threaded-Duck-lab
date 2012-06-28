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
* @file StorageException.h
* @brief Storage Component: Base Exception Class  
* @author Christian Nastasi c.nastasi@sssup.it, nastasichr@gmail.com
* @author Claudio Salvadori c.salvadori@sssup.it
* @version 1.0
* @date 2012-02-27
*/
#ifndef __StorageException_h__
#define __StorageException_h__

#include "DuckLog.h"
#include "DuckStorageException.h"

#ifdef DUCK_USE_NAMESPACE
namespace duck {
#endif

#define THROW_STORAGE_EXCEPTION_1(expr) 	\
do {						\
	std::ostringstream oss;			\
	__THROW_DUCK_EXCEPTION_HEADER(oss);	\
	oss << expr;				\
	throw StorageException(oss.str());	\
} while (0)

#define THROW_STORAGE_EXCEPTION_2(code, expr) 	\
do {						\
	std::ostringstream oss;			\
	__THROW_DUCK_EXCEPTION_HEADER(oss);	\
	oss << expr;				\
	throw StorageException(oss.str(), code);\
} while (0)

class StorageException : public DuckStorageException {
public:
	enum {
		UNKNOWN = 0,
		DATA_BADTYPE,
		// TODO: complete list?
	};

        StorageException(const std::string& msg) throw();
        StorageException(const std::string& msg, unsigned code) throw();
        StorageException(unsigned code) throw();
        ~StorageException(void) throw();
        inline unsigned get_code(void) const throw() { return code; };
private:
       	StorageException(void) throw();
	unsigned code;
};

#ifdef DUCK_USE_NAMESPACE
}; // namespace duck
#endif

#endif /* __StorageException_h__ */
