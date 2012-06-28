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
* @file DuckStorageException.h
* @brief DuckStorageException facility 
* @author Christian Nastasi c.nastasi@sssup.it, nastasichr@gmail.com
* @author Claudio Salvadori c.salvadori@sssup.it
* @version 1.0
* @date 2012-02-27
*/
#ifndef __DuckStorageException_h__
#define __DuckStorageException_h__

#include "DuckLog.h"
#include "DuckException.h"

#ifdef DUCK_USE_NAMESPACE
namespace duck {
#endif

#define THROW_DUCK_STORAGE_EXCEPTION(expr) 	\
do {						\
	std::ostringstream oss;			\
	__THROW_DUCK_EXCEPTION_HEADER(oss);	\
	oss << expr;				\
	throw DuckStorageException(oss.str());	\
} while (0)

class DuckStorageException : public DuckException {
public:
        DuckStorageException(const std::string& msg) throw() 
	: DuckException(msg) {};
        ~DuckStorageException(void) throw() {};
private:
       	DuckStorageException(void) throw();
};

#ifdef DUCK_USE_NAMESPACE
}; // namespace duck
#endif

#endif /* __DuckStorageException_h__ */
