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
* @file SyncroC.cpp
* @brief SyncroC Implementation File
* @author Christian Nastasi c.nastasi@sssup.it, nastasichr@gmail.com
* @author Claudio Salvadori c.salvadori@sssup.it
* @version 1.0
* @date 2012-02-27
*/
#include "SyncroC.h"
#include "Parser.h"
#include "Stream.h"
#include "PolicyFactory.h"
#include "DuckUtils.h"
#include "DuckException.h"
#include "DuckLog.h"

#include <stdlib.h>
#include <string>
#include <string.h>

#define N_MAX	100000

#ifdef DUCK_USE_NAMESPACE
using namespace duck;
#endif

using namespace std;

typedef SyncroC::SyncroCConfig CfgClass;
DUCK_REGISTER_POLICY_CLASSES("start_end", SyncroC, CfgClass);


SyncroC::SyncroCConfig::SyncroCConfig(Parser &parser) 
: start_sequence(NULL), end_sequence(NULL), start_length(0), end_length(0)
{
	if (!parser.enter_subnode("start_end_option"))
		THROW_DUCK_EXCEPTION("No 'start_end_option' found by Parser: "
				     "curret path was:" << parser.print_path());
	string char_coding;
	string start;
	string end;
	if (!parser.get_node_attribute("char_coding", char_coding))
		THROW_DUCK_EXCEPTION("No 'char_coding' attribute found by "
				     "Parser for node 'start_end_option'");
	if (!parser.get_node_attribute("start_chars", start))
		THROW_DUCK_EXCEPTION("No 'start' attribute found by Parser for "
				     "node 'start_end_option'");
	if (!parser.get_node_attribute("end_chars", end))
		THROW_DUCK_EXCEPTION("No 'end' attribute found by Parser for "
				     "node 'start_end_option'");
	if (char_coding == "hex") {
		utils::hex_to_bytes(start, start_sequence, start_length);
		utils::hex_to_bytes(end, end_sequence, end_length);
	} else /*if (char_coding == "hex") */{
		utils::ascii_to_bytes(start, start_sequence, start_length);
		utils::ascii_to_bytes(end, end_sequence, end_length);
	} /*else {
		TODO: parse the char_coding string properly!
	}*/

	parser.exit_subnode();
}

SyncroC::SyncroCConfig::SyncroCConfig(const SyncroCConfig& c) 
{
	if (c.start_sequence) {
		start_length = c.start_length;
		new uint8_t[start_length];
		memcpy(start_sequence, c.start_sequence, c.start_length);
	}
	if (c.end_sequence) {
		end_length = c.end_length;
		new uint8_t[end_length];
		memcpy(end_sequence, c.end_sequence, c.end_length);
	}
}

SyncroC::SyncroCConfig::~SyncroCConfig()
{
	if (start_sequence)
		delete[] start_sequence;
	if (end_sequence)
		delete[] end_sequence;
}

SyncroC::~SyncroC()
{
	if (received_packet)
		delete[] received_packet;
}

void SyncroC::on_configure(void)
{
	// Pre-allocate buffer for packet reception
	received_packet_length = expected_packet_length;
	buffer_length = expected_packet_length + config.end_length;
	received_packet = new uint8_t[buffer_length];
}


void SyncroC::send(const uint8_t* packet, unsigned long len)
{
	if (!stream)
		return; // TODO: generate exception?

	// TODO: decide which solution we should adopt:
	//       1) mem-copy all bytes in a tmp buffer passed to the stream
	//       2) call three times the stream->write()

	// Solution 1
	#if 0
	const unsigned long tot_len = len + config.start_length + config.end_length
	uint8_t* pkt = new uint8_t[tot_len];
	uint8_t* pkt_ptr = pkt;
	memcpy(pkt_ptr, start_sequence, config.start_length);
	pkt_ptr += config.start_length;
	memcpy(pkt_ptr, packet, len);
	pkt_ptr += len;
	memcpy(pkt_ptr, config.end_sequence, config.end_length);
	unsigned long w = stream->write(pkt, tot_len);
	delete[] pkt;
	#endif

	// Solution 2	
	unsigned long w = 0;
	w += stream->write(config.start_sequence, config.start_length);
	w += stream->write(packet, len);
	w += stream->write(config.end_sequence, config.end_length);
}

void SyncroC::read_start_sequence(void)
{
	uint8_t byte;
	unsigned long i = 0;	
	while (i < config.start_length) {
		if (stream->read(&byte, 1) != 1)
			THROW_DUCK_EXCEPTION("More than 1 byte was read by the"
					     " stream sub-module");
		if (byte != config.start_sequence[i]) { 
			// Byte is out of sequence. Check if the bad byte is
			// still a valid first sequence element.
			i = (byte == config.start_sequence[0]) ? 1 : 0;
			continue;
		}
		i++;
		// TODO: make an error after N_MAX attempts
	}
}

void SyncroC::receive(unsigned char*& out_packet, unsigned long& out_len)
{
	vector<uint8_t*> buffers; // Set of extra buffers

	// PART 1: Look for the start sequence
	read_start_sequence();

	// PART 2: Read packet bytes while looking for end sequence

	// Attempt to read up to 'buffer_length' bytes in 'received_packet'
	// If space is not sufficient, use extra buffers
	uint8_t *buf = received_packet;	// Current buffer
	unsigned long buf_pos = 0;	// Position in current buffer
	unsigned long seq_pos = 0;	// Position in sequence (end_sequence)
	while (seq_pos < config.end_length) {
		uint8_t byte;
		// Read a single byte
		if (stream->read(&byte, 1) != 1)
			THROW_DUCK_EXCEPTION("More than 1 byte was read by the"
					     " stream sub-module");
		buf[buf_pos] = byte; 	// Store byte in current buffer
		if (byte != config.end_sequence[seq_pos]) {
			// Byte is out of sequence. Check if the bad byte is
			// still a valid firt sequence element.
			seq_pos = (byte == config.end_sequence[0]) ? 1 : 0;
		}
		seq_pos++;
		buf_pos++;
		if (buf_pos == buffer_length) {
			// Current buffer is finished, 
			// add a new one to the set of extra buffers
			buffers.push_back(new uint8_t[buffer_length]);
			buf = buffers.back();
			buf_pos = 0;
		}
	}

	// Length of received packet (end_sequence is stripped):
	// LEN = (SINGLE_BUFFER_SIZE * #EXTRA_BUFFERS) + EXPEXTED_PACKET_SIZE
	//     = (SINGLE_BUFFER_SIZE * #EXTRA_BUFFERS) + SINGLE_BUFFER_SIZE 
	//	                                       - END_SEQUENCE_SIZE 
	//     = (SINGLE_BUFFER_SIZE * (#EXTRA_BUFFERS + 1)) - END_SEQUENCE_SIZE
	unsigned long len = ((buffers.size() + 1) * buffer_length) - 
			    config.end_length;

	// TODO!!
	if (len != expected_packet_length)
		;// TODO: FIXME: now what? What do we do??  PANIC!!

	// NOTE: Check if re-allocation in required
	// 	 The problem occurs when the length of the received packet is 
	//       larger than the last expected length (which was either the
	//	 initial expected_packet_length or the length of 
	//       the previous received packet)
	// NOTE: In case re-allocation is NOT required, the bytes are already
	//       available in the reception buffer
	if (len > received_packet_length) { 
		// Create a new reception buffer which can hold new packets
		// of the same size plus the end_sequence
		unsigned long tmp_length = len + config.end_length;
		uint8_t* tmp_packet = new uint8_t[tmp_length];

		// Copy the first buffer ('received_packet') in the new one	
		memcpy(tmp_packet, received_packet, buffer_length);
		// For each extra buffer (in 'buffers'): 
		// - copy its content in the new buffer
		// - free the memory of the extra buffer
		vector<uint8_t*>::iterator it = buffers.begin();
		uint8_t* p = tmp_packet;
		for (; it < buffers.end(); it++) {
			memcpy(p, *it, buffer_length); 
			p += buffer_length;
			delete[] *it;
		}
		// Remove previous reception buffer
		if (received_packet)
			delete[] received_packet;
		// Update reception buffer
		received_packet = tmp_packet;	// new reception buffer
		received_packet_length = len;	// new epected length
		buffer_length = tmp_length;	// new buffer size
	}

	out_packet = received_packet;
	out_len = len;
}
