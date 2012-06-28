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
* @file FormatHandler.h
* @brief FormatHandler Module
* @author Christian Nastasi c.nastasi@sssup.it, nastasichr@gmail.com
* @author Claudio Salvadori c.salvadori@sssup.it
* @version 1.0
* @date 2012-02-27
*/
#ifndef __FormatHandler_h__
#define __FormatHandler_h__

#include "Data.h"
#include <string>
#include <vector>
#include <utility>

#ifdef DUCK_USE_NAMESPACE
namespace duck {
#endif

class Data;
class Parser;

class FormatHandler { 
public: 
	FormatHandler(Parser& parser);
	~FormatHandler();
	
	unsigned long format(uint8_t* rawdata, unsigned long len);
	unsigned long unformat(uint8_t* rawdata) {return 0;}; //TODO: for Daq-1
	
	inline std::vector<Data*>& get_data_vector(void) 
						{return data_vector;};
	inline unsigned long get_data_requested_bytes(void) 
						{return rawdata_size;};
	std::vector< const Data*>*  
			put_get(std::pair<uint8_t*, unsigned long> *elem);
	
private:
	unsigned long rawdata_size; 			
	std::vector<Data*> data_vector; 

	void init(Parser& parser);
	void final_check(void);
};
#ifdef DUCK_USE_NAMESPACE
};
#endif
#endif
