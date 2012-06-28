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
* @file DuckException.h
* @brief duck-lab Base Exception Class
* @author Christian Nastasi c.nastasi@sssup.it, nastasichr@gmail.com
* @author Claudio Salvadori c.salvadori@sssup.it
* @version 1.0
* @date 2012-02-27
*/
#ifndef __DuckException_h__
#define __DuckException_h__

#include "DuckLog.h"

#include <exception>
#include <string>
#include <sstream>
#include <iostream>

#ifdef DUCK_USE_NAMESPACE
namespace duck {
#endif

#ifdef DUCK_LOG_ENABLED
#define __THROW_DUCK_EXCEPTION_HEADER(s) s << "Thrown by " << \
					      __PRETTY_FUNCTION__ << " : " 
#else
#define __THROW_DUCK_EXCEPTION_HEADER(s)  
#endif

#define THROW_DUCK_EXCEPTION(expr) 		\
do {						\
	std::ostringstream oss;			\
	__THROW_DUCK_EXCEPTION_HEADER(oss);	\
	oss << expr;				\
	throw DuckException(oss.str());		\
} while (0)

/** 
* @brief Base Exception class for the duck-lab framework
*/
class DuckException : public std::exception {
public:
	/** 
	* @brief Create exception with message string
	* @param msg 
	*/
        DuckException(const std::string& msg) throw();
        ~DuckException(void) throw();
	/** 
	* @brief Get descriptive message string
	* @return Exception message string
	*/
        const char *what(void) const throw();
protected:
        std::string errMessage;
private:
       	DuckException(void) throw();
};

//std::ostream& operator<<(std::ostream &o, const DuckException &e);



#ifdef DUCK_USE_NAMESPACE
}; // namespace duck
#endif

#endif /* __DuckException_h__ */
