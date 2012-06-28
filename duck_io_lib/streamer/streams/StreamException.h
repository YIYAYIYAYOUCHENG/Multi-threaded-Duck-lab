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
* @file StreamException.h
* @brief StreamException facility 
* @author Christian Nastasi c.nastasi@sssup.it, nastasichr@gmail.com
* @author Claudio Salvadori c.salvadori@sssup.it
* @version 1.0
* @date 2012-02-27
*/
#ifndef __StreamException_h__
#define __StreamException_h__

#include "DuckLog.h"
#include "DuckIOException.h"

#ifdef DUCK_USE_NAMESPACE
namespace duck {
#endif

#define THROW_STREAM_EXCEPTION_1(expr) 		\
do {						\
	std::ostringstream oss;			\
	__THROW_DUCK_EXCEPTION_HEADER(oss);	\
	oss << expr;				\
	throw StreamException(oss.str());	\
} while (0)

#define THROW_STREAM_EXCEPTION_2(code, expr) 	\
do {						\
	std::ostringstream oss;			\
	__THROW_DUCK_EXCEPTION_HEADER(oss);	\
	oss << expr;				\
	throw StreamException(oss.str(), code);	\
} while (0)

class StreamException : public DuckIOException {
public:
	enum {
		UNKNOWN = 0,
		EOF_REACHED,
		// TODO: have more error codes
	};
        StreamException(const std::string& msg) throw();
        StreamException(const std::string& msg, unsigned code) throw();
        ~StreamException(void) throw();
        inline unsigned get_code(void) const throw() { return code; };
private:
       	StreamException(void) throw();
	unsigned code;
};

#ifdef DUCK_USE_NAMESPACE
}; // namespace duck
#endif

#endif /* __StreamException_h__ */
