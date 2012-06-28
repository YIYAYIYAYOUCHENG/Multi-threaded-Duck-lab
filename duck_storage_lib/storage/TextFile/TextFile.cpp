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
* @file TextFile.cpp
* @brief TextFile Implementation File
* @author Christian Nastasi c.nastasi@sssup.it, nastasichr@gmail.com
* @author Claudio Salvadori c.salvadori@sssup.it
* @version 1.0
* @date 2012-02-27
*/
#include "StorageFactory.h"
#include "TextFile.h"
#include "StorageException.h"

#include "DataInt8.h"
#include "DataUInt8.h"
#include "DataInt16.h"
#include "DataUInt16.h"
#include "DataInt32.h"
#include "DataUInt32.h"
#include "DataInt64.h"
#include "DataUInt64.h"
#include "DataFloat32.h"
#include "DataFloat64.h"

#include <fstream>

#ifdef DUCK_USE_NAMESPACE
using namespace duck;
#endif
using namespace std;

DUCK_REGISTER_STORAGE_CLASS(TextFileConfig::storage_name, TextFile);

TextFile::TextFile(const StorageConfig& c)
{
	const TextFileConfig& cfg = dynamic_cast<const TextFileConfig&>(c);

	DUCK_DBG("Constructor");

	init_typefunc_tab();

	if (cfg.get_out_filename() != "") 
		file_out_stream = new ofstream(cfg.get_out_filename().c_str());
	else
		file_out_stream = NULL;
	out_stream = cfg.get_out_stream(); 

	vector<const Data*> meta = cfg.get_data_subset(); 
	vector<TextFileConfig::StreamFormat> fmt = cfg.get_formats(); 

	print_functions = new print_func_t[meta.size()];
	print_formats = new Format[meta.size()];

	vector<const Data*>::const_iterator d = meta.begin();
	vector<TextFileConfig::StreamFormat>::const_iterator f = fmt.begin();
	bool data_outstream = false;
	for (unsigned index = 0; d != meta.end(); ++d, ++f, index++) {
		const Metadata *m = *d;
		if (!check_metadata(m)) {
			delete[] print_functions;
			delete[] print_formats;
			print_functions = NULL;
			print_formats = NULL;
			THROW_STORAGE_EXCEPTION_2(
				StorageException::DATA_BADTYPE,
				"Bad data type for this storage");
		}
		config_print(m, *f, index);
		if (f->out_stream != NULL)
			data_outstream = true;
	}
	data_sep = cfg.get_data_separator();
	data_term = cfg.get_data_terminator();
	
	if (file_out_stream == NULL && out_stream == NULL && !data_outstream) {
 		delete[] print_functions;
		delete[] print_formats;
		print_functions = NULL;
		print_formats = NULL;
		THROW_STORAGE_EXCEPTION_1("Attempt to configure a TextFile "
				          "without any output stream.");
	}
} 

TextFile::~TextFile()
{
	DUCK_DBG("Destructor");

	if (print_functions)
		delete[] print_functions;
	if (print_formats)
		delete[] print_formats;
	if (file_out_stream) {
		file_out_stream->close(); // TODO: do we have to close here?
		delete file_out_stream;
	}
}

void TextFile::store(const vector<Data*>& data)
{
	DUCK_DBG("Attempt to STORE");

	// TODO: no check that data.size() is the same as the metadata.size()
	string out_string = "";
	vector<Data*>::const_iterator d = data.begin();
        for (unsigned i = 0; d != data.end(); ++d, i++) {
		out_string += print_functions[i](*d, print_formats[i]);
		out_string += data_sep;
	}
	out_string += data_term;
	if (out_stream != NULL) {
		(*out_stream) << out_string;
		out_stream->flush();
	}
	if (file_out_stream != NULL) {
		(*file_out_stream) << out_string;
		file_out_stream->flush();
	}

	DUCK_DBG("STORE->" << out_string);
}

bool TextFile::check_metadata(const Metadata* m)
{
	DUCK_DBG("check metadata:" <<
		 " type=" << m->get_type() <<
		 " type_size=" << m->get_type_size() <<
		 " len=" << m->get_len() 
	);

	// TODO Check type, type_size, len
	if (typefunc_tab->find(m->get_type()) == typefunc_tab->end()) 
		return false;  // TODO: do something else?
	return true;
}

void TextFile::config_print(const Metadata* m, 
			    const TextFileConfig::StreamFormat& f,
			    unsigned idx)
{
	DUCK_DBG("config print:" <<
		 " type=" << m->get_type() <<
		 " type_size=" << m->get_type_size() <<
		 " len=" << m->get_len() 
	);

	// Attach the proper print function
	print_functions[idx] = (*typefunc_tab)[m->get_type()];
	// Set the element separator
	print_formats[idx].sep = f.elem_separator;
	// Apply the ios_base::fmtflags to the ostringstream
	for (unsigned i = 0; i < f.gen_flags.size(); i++)
		print_formats[idx].formatter.setf(f.gen_flags[i]);
	print_formats[idx].formatter.setf(f.base_flag, ios_base::basefield);
	print_formats[idx].formatter.setf(f.float_flag, ios_base::floatfield);
	print_formats[idx].formatter.setf(f.adjust_flag, ios_base::adjustfield);
	// Copy ithe output stream
	print_formats[idx].out_stream = f.out_stream;
	// TODO: this is the work-around for char and uchar (non-ASCII print)
	if (!f.char_ascii) {
		if (m->get_type() == "int8")
			print_functions[idx] = print_int8_noascii;
		else if (m->get_type() == "uint8") 
			print_functions[idx] = print_uint8_noascii;
	}
}

template<class T> 
inline string TextFile::print_template(const Data* dt, Format& f)
{ 
	const T* d = static_cast<const T*>(dt);
	const unsigned cnt = d->get_len();
	f.formatter.str(""); // Clean the string stream 
	for (unsigned i = 0; i < cnt; i++)
		f.formatter << d->data[i] << f.sep;
	if (f.out_stream != NULL) {
		(*f.out_stream) << f.formatter.str();
		f.out_stream->flush();
	}
	return f.formatter.str();
}

string TextFile::print_int8(const Data* data, Format& f)
{
	return print_template<DataInt8>(data, f);
}

string TextFile::print_uint8(const Data* data, Format& f)
{
	return print_template<DataUInt8>(data, f);
}

string TextFile::print_int16(const Data* data, Format& f)
{
	return print_template<DataInt16>(data, f);
}

string TextFile::print_uint16(const Data* data, Format& f)
{
	return print_template<DataUInt16>(data, f);
}
string TextFile::print_int32(const Data* data, Format& f)
{
	return print_template<DataInt32>(data, f);
}

string TextFile::print_uint32(const Data* data, Format& f)
{
	return print_template<DataUInt32>(data, f);
}
string TextFile::print_int64(const Data* data, Format& f)
{
	return print_template<DataInt64>(data, f);
}

string TextFile::print_uint64(const Data* data, Format& f)
{
	return print_template<DataUInt64>(data, f);
}
string TextFile::print_float32(const Data* data, Format& f)
{
	return print_template<DataFloat32>(data, f);
}

string TextFile::print_float64(const Data* data, Format& f)
{
	return print_template<DataFloat64>(data, f);
}

string TextFile::print_int8_noascii(const Data* data, Format& f)
{ 
	const DataInt8* d = static_cast<const DataInt8*> (data);
	const unsigned cnt = d->get_len();
	f.formatter.str(""); // Clean the string stream 
	for (unsigned i = 0; i < cnt; i++)
		f.formatter << (int16_t) d->data[i] << f.sep;
	if (f.out_stream != NULL) {
		(*f.out_stream) << f.formatter.str();
		f.out_stream->flush();
	}
	return f.formatter.str();
}

string TextFile::print_uint8_noascii(const Data* data, Format& f)
{ 
	const DataUInt8* d = static_cast<const DataUInt8*> (data);
	const unsigned cnt = d->get_len();
	f.formatter.str(""); // Clean the string stream 
	for (unsigned i = 0; i < cnt; i++)
		f.formatter << (uint16_t) d->data[i] << f.sep;
	if (f.out_stream != NULL) {
		(*f.out_stream) << f.formatter.str();
		f.out_stream->flush();
	}
	return f.formatter.str();
}

void TextFile::init_typefunc_tab(void)
{
	if (typefunc_tab != NULL)
		return;
	typefunc_tab = new map<string, print_func_t>;

	(*typefunc_tab)[string("int8")] = print_int8;
	(*typefunc_tab)[string("uint8")] = print_uint8;
	(*typefunc_tab)[string("int16")] = print_int16;
	(*typefunc_tab)[string("uint16")] = print_uint16;
	(*typefunc_tab)[string("int32")] = print_int32;
	(*typefunc_tab)[string("uint32")] = print_uint32;
	(*typefunc_tab)[string("int64")] = print_int64;
	(*typefunc_tab)[string("uint64")] = print_uint64;
	(*typefunc_tab)[string("float32")] = print_float32;
	(*typefunc_tab)[string("float64")] = print_float64;
}
	
std::map<std::string, TextFile::print_func_t>* TextFile::typefunc_tab;
