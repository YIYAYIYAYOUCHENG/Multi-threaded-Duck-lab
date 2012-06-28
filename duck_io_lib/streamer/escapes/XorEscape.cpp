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
* @file XorEscape.cpp
* @brief XorEscape Implementation File
* @author Christian Nastasi c.nastasi@sssup.it, nastasichr@gmail.com
* @author Claudio Salvadori c.salvadori@sssup.it
* @version 1.0
* @date 2012-02-27
*/
#include "XorEscape.h"
#include "Parser.h"
#include "EscapeFactory.h"
#include "DuckUtils.h"
#include "ParserException.h"
#include "DuckLog.h"

#ifdef DUCK_USE_NAMESPACE
using namespace duck;
#endif

using namespace std;

typedef XorEscape::XorEscapeConfig CfgClass;
DUCK_REGISTER_ESCAPE_CLASSES("xor", XorEscape, CfgClass);

XorEscape::XorEscapeConfig::XorEscapeConfig(Parser &parser)
{
	if (!parser.enter_subnode("xor_option"))
		THROW_PARSER_EXCEPTION("No 'xor_option' found by Parser: curret"
				       " path was:" << parser.print_path());
	string char_coding;
	string str;
	if (!parser.get_node_attribute("char_coding", char_coding))
		THROW_PARSER_EXCEPTION("No 'char_coding' attribute found by "
				       "Parser for node 'xor_option'");
	if (!parser.get_node_attribute("esc_char", str))
		THROW_PARSER_EXCEPTION("No 'esc_char' attribute found by "
				       "Parser for node 'xor_option'");
	if (char_coding == "hex")
		utils::hex_to_bytes(str, escape_sequence, escape_length);
	else /*if (char_coding == "hex") */
		utils::ascii_to_bytes(str, escape_sequence, escape_length);
	/*else 
		TODO: parse the char_coding string properly!
	*/
	// TODO: is this correct? Do we want ONLY single-char escape?
	if (escape_length > 1)
		THROW_PARSER_EXCEPTION("Unsuported escape sequences with "
				       "lenght > 1");
	escape_char = escape_sequence[0];
	parser.exit_subnode();
}

XorEscape::XorEscapeConfig::XorEscapeConfig(const XorEscapeConfig& c) 
{
	if (c.escape_sequence) {
		escape_length = c.escape_length;
		new uint8_t[escape_length];
		memcpy(escape_sequence, c.escape_sequence, c.escape_length);
	}
	escape_char = c.escape_char;
}

XorEscape::XorEscapeConfig::~XorEscapeConfig()
{
	if (escape_sequence)
		delete[] escape_sequence;
}

XorEscape::~XorEscape(void)
{
	if (read_data)
		delete[] read_data;
	if (write_data)
		delete[] write_data;
}

void XorEscape::on_configure(void)
{
	// Pre-allocate buffer for unescaping operation
	if (read_data)
		delete[] read_data;
	read_data = new uint8_t[expected_data_size];

	// Pre-allocate buffer for escaping operation
	if (write_data)
		delete[] write_data;
	// NOTE: '* 2' is because we expect ONLY single-char escape!
	write_data_len = expected_data_size * 2;
	write_data = new uint8_t[write_data_len];
}

void XorEscape::escape(const uint8_t* in, unsigned long in_len,
		       uint8_t*& out, unsigned long& out_len)
{
	// TODO: with current implementation that allows only for single-char
	//	 escaping (escape_char), we are sure that write_data_len 
	//	 is at most twice the space of expected_data_size
	//       (case when every char has to be excapted -> input chars
	//       are all escape chars)
	// TODO: Therefore, we use a simple implementation that does not account
	//	 for possible re-allocation mechanism that should be required
	//	 if the condition no longer applies (see SyncroC.cpp)

	if (!write_data)
		return;
	unsigned long len = (in_len < expected_data_size) ? 
			    in_len : expected_data_size;
	unsigned j = 0;
	for (unsigned long i = 0; i < len; i++) {
		if (in[i] == config.escape_char) {
			write_data[j] = in[i] ^ config.escape_char;
			j++;
		} 
		write_data[j] = in[i];
		j++;
	}
	out = write_data;
	out_len = j;
}

void XorEscape::unescape(const uint8_t* in, unsigned long in_len,
			 uint8_t*& out, unsigned long& out_len)
{
	if (!read_data)
		return;
	unsigned long i = 0;
	unsigned long j = 0;
	while (i < in_len) {
		if (j >= expected_data_size)
			break;
		if (in[i] == config.escape_char) {
			if (i < (in_len - 1)) {
				i++;
				read_data[j] = in[i] ^ config.escape_char;	
			} 
			// else: The escape char is last in the buffer, strange!
			// TODO:?
			//	THROW_PARSER_EXCEPTION(
			//			    "Error: Escape character is"
			//			    "last character; does not "
			//			    "escape anything!");
		} else { 
			read_data[j] = in[i];
		}
		i++;
		j++;
	}
	out = read_data;
	out_len = j;
}
