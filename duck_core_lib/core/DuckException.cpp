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
* @file DuckException.cpp
* @brief DuckException Implementation File
* @author Christian Nastasi c.nastasi@sssup.it, nastasichr@gmail.com
* @author Claudio Salvadori c.salvadori@sssup.it
* @version 1.0
* @date 2012-02-27
*/
#include "DuckException.h"

#ifdef DUCK_USE_NAMESPACE
using namespace duck;
#endif

DuckException::DuckException(const std::string& msg) throw() : errMessage(msg) 
{ 
}

DuckException::~DuckException(void) throw() 
{ 
}

const char *DuckException::what(void) const throw() 
{ 
	return errMessage.c_str(); 
}
/*
std::ostream& operator <<(std::ostream &o, const DuckException  &e) {
        o << e.what();
        return o;
}
*/
