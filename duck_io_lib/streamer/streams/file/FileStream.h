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
* @file FileStream.h
* @brief FileStream Concrete \ref Stream Class 
* @author Christian Nastasi c.nastasi@sssup.it, nastasichr@gmail.com
* @author Claudio Salvadori c.salvadori@sssup.it
* @version 1.0
* @date 2012-02-27
*/
#ifndef __FileStream_h__
#define __FileStream_h__

#include "Stream.h"
#include "StreamConfig.h"

#include <string>
#include <fstream>
#include <stdint.h>

#ifdef DUCK_USE_NAMESPACE
namespace duck {
#endif

class Parser;

class FileStream: public Stream {
public:
	class FileStreamConfig : public StreamConfig {
	public:
		FileStreamConfig(Parser& p);
	public:
		std::string file_name;
		std::ios_base::openmode open_mode;
	};

public:
	FileStream(const StreamConfig& cfg) 
	: file_config(dynamic_cast<const FileStreamConfig&>(cfg)),
	  file(file_config.file_name.c_str(), file_config.open_mode) {};

	unsigned long write(const uint8_t* packet, unsigned long length);
	unsigned long read(uint8_t* packet, unsigned long length);
	//inline std::ios_base::openmode get_openmode(void) const
//		     		       {return file_config->open_mode;};
	inline bool is_open(void) { return file.is_open(); };
private:
	FileStreamConfig file_config;
	std::fstream file;
};

#ifdef DUCK_USE_NAMESPACE
}; // namespace duck
#endif

#endif /* __FileStream_h__ */
