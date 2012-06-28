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
* @file TextFile.h
* @brief TextFile Concrete \ref Storage Class 
* @author Christian Nastasi c.nastasi@sssup.it, nastasichr@gmail.com
* @author Claudio Salvadori c.salvadori@sssup.it
* @version 1.0
* @date 2012-02-27
*/
#ifndef __TextFile_h__
#define __TextFile_h__

#include "Storage.h"
#include "TextFileConfig.h"
#include <ostream>
#include <string>
#include <sstream>

#ifdef DUCK_USE_NAMESPACE
namespace duck {
#endif

class TextFile: public Storage {
private:
	class Format {
	public:
		std::string sep;
		std::ostringstream formatter;
		std::ostream *out_stream;
	};
	typedef std::string (*print_func_t)(const Data*, Format&);
public:

	TextFile(const StorageConfig& cfg);
	~TextFile();

	void store(const std::vector<Data*> &data);
private:

	static std::map<std::string, print_func_t> *typefunc_tab;
	std::ostream *out_stream;
	std::ofstream *file_out_stream;
	std::string data_sep;
	std::string data_term;
	Format *print_formats;
	print_func_t *print_functions;

	void init_typefunc_tab(void);
	bool check_metadata(const Metadata* m);
	void config_print(const Metadata* m, 
			  const TextFileConfig::StreamFormat& f, unsigned i);
	template<class T> 
	inline static std::string print_template(const Data* dt, Format& f);

	static std::string print_int8_noascii(const Data* data, Format& f);
	static std::string print_uint8_noascii(const Data* data, Format& f);
	static std::string print_int8(const Data* data, Format&);
	static std::string print_uint8(const Data* data, Format&);
	static std::string print_int16(const Data* data, Format&);
	static std::string print_uint16(const Data* data, Format&);
	static std::string print_int32(const Data* data, Format&);
	static std::string print_uint32(const Data* data, Format&);
	static std::string print_int64(const Data* data, Format&);
	static std::string print_uint64(const Data* data, Format&);
	static std::string print_float32(const Data* data, Format&);
	static std::string print_float64(const Data* data, Format&);
};

#ifdef DUCK_USE_NAMESPACE
};
#endif

#endif /* __TextFile_h__ */
