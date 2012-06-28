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
* @file TextFileConfig.h
* @brief TextFileConfig Concrete \ref StorageConfig Class 
* @author Christian Nastasi c.nastasi@sssup.it, nastasichr@gmail.com
* @author Claudio Salvadori c.salvadori@sssup.it
* @version 1.0
* @date 2012-02-27
*/
#ifndef __TextFileConfig_h__
#define __TextFileConfig_h__

#include "StorageConfig.h"
#include "Parser.h"
#include "Data.h"
#include <ios>
#include <ostream>
#include <utility>
#include <string>

#ifdef DUCK_USE_NAMESPACE
namespace duck {
#endif

class TextFileConfig: public StorageConfig {
public:
	class StreamFormat {
	public:
		StreamFormat() : out_stream(NULL), elem_separator(" "), 
				 base_flag(std::ios_base::dec), 
				 float_flag(std::ios_base::fixed),
				 adjust_flag(std::ios_base::internal),
				 char_ascii(false) {};
		std::ostream *out_stream;
		std::string elem_separator;
		std::vector<std::ios_base::fmtflags> gen_flags;
		std::ios_base::fmtflags base_flag;
		std::ios_base::fmtflags float_flag;
		std::ios_base::fmtflags adjust_flag;
		bool char_ascii;
	};

	TextFileConfig();
	TextFileConfig(Parser& parser);

	std::string get_storage_name(void) const { return storage_name; };

	inline void set_out_filename(const std::string &s) {out_filename = s;};
	inline void set_out_stream(std::ostream *s) {out_stream = s;};
	inline std::string get_out_filename(void) const {return out_filename;};
	inline std::ostream* get_out_stream(void) const {return out_stream;};
	inline std::string get_data_separator(void) const 
		{ return data_separator; };
	inline std::string get_data_terminator(void) const 
		{ return data_terminator; };
	inline std::vector<StreamFormat> get_formats(void) const
		{ return data_format; };

private:
	void init_preset(Parser& p);
	void init_data2storage(Parser& p);
public:
	static const char *storage_name;
private:
	std::string out_filename;
	std::ostream *out_stream;
	std::string data_separator;
	std::string data_terminator;
	std::vector<StreamFormat> data_format;
};

#ifdef DUCK_USE_NAMESPACE
};
#endif

#endif /* __TextFileConfig_h__ */
