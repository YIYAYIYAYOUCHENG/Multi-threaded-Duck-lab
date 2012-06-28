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
* @file Data.cpp
* @brief Data Implementation File
* @author Christian Nastasi c.nastasi@sssup.it, nastasichr@gmail.com
* @author Claudio Salvadori c.salvadori@sssup.it
* @version 1.0
* @date 2012-02-27
*/
#include "Data.h"
#include "DuckException.h"
#include "DuckLog.h"
#include <string.h>

#ifdef DUCK_USE_NAMESPACE
using namespace duck;
#endif
using namespace std;

void Data::write_serializable_header(uint8_t* buff)
{
	const unsigned str_len = name.size();
	const char *str_buf = name.c_str();

	DUCK_DBG("Serialize header: name-len=" << str_len);
	memcpy(buff, &str_len, sizeof(unsigned)); 
	serializable_endianess(buff, 1lu, sizeof(unsigned));
	buff += sizeof(unsigned);

	DUCK_DBG("Serialize header: name=" << str_buf);
	memcpy(buff, str_buf, str_len); 
	// Note: no need for endianess for char[]
	buff += str_len;

	DUCK_DBG("Serialize header: data-len=" << length);
	memcpy(buff, &length, sizeof(unsigned long)); 
	serializable_endianess(buff, 1lu, sizeof(unsigned long));
}

unsigned long Data::read_serializable_header(uint8_t* buff, unsigned long len)
{
	unsigned min_len = sizeof(unsigned) +     // Data-name string-len field
			   sizeof(unsigned long); // Data len field
	if (len < min_len)
		THROW_DUCK_EXCEPTION("Too few bytes to " 
				     "deserialize object header: Expected = " <<
				     min_len << "  but len = " << len << 
				     ". In file" __FILE__ ":" << __LINE__);
	
	unsigned str_len;
	unsigned long cnt = 0;
	
	memcpy(&str_len, buff, sizeof(unsigned));
	serializable_endianess((uint8_t*)&str_len, 1lu, sizeof(unsigned));
	cnt += sizeof(unsigned);
	DUCK_DBG("Deserialize header: name-len=" << str_len);

	min_len += str_len;
	if (len < min_len) 
		THROW_DUCK_EXCEPTION("Too few bytes to " << 
				     "deserialize object header: Expected = " <<
				     min_len << "  but len = " << len << 
				     ". In file" __FILE__ ":" << __LINE__);

	char *str_buf = new char[str_len + 1];
	str_buf[str_len] = '\0';

	memcpy(str_buf, buff + cnt, str_len);
	// Note: no need for endianess for char[]
	cnt += str_len;
	
	name = str_buf;
	DUCK_DBG("Deserialize header: name=" << name);

	memcpy(&length, buff + cnt, sizeof(unsigned long));
	serializable_endianess((uint8_t*)&length, 1lu, sizeof(unsigned long));
	cnt += sizeof(unsigned long);
	DUCK_DBG("Deserialize header: data-len=" << length);

	return cnt;
}
