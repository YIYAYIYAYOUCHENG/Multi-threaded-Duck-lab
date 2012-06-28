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
* @file SyncroC.h
* @brief SyncroC Concrete \ref Policy Class 
* @author Christian Nastasi c.nastasi@sssup.it, nastasichr@gmail.com
* @author Claudio Salvadori c.salvadori@sssup.it
* @version 1.0
* @date 2012-02-27
*/
#ifndef __SyncroC_h__
#define __SyncroC_h__

#include <stdint.h>
#include "Policy.h"
#include "PolicyConfig.h"

#ifdef DUCK_USE_NAMESPACE
namespace duck {
#endif

class Parser;

class SyncroC: public Policy {
public:
	class SyncroCConfig : public PolicyConfig {
	public:
		SyncroCConfig(Parser& p);
		SyncroCConfig(const SyncroCConfig& cfg);
		~SyncroCConfig();
	public:
		uint8_t* start_sequence;
		uint8_t* end_sequence;
		unsigned long start_length;
		unsigned long end_length;
	};
public:
	
	SyncroC(const PolicyConfig& c) : 
		Policy(), config(dynamic_cast<const SyncroCConfig&>(c)),
		received_packet(NULL), received_packet_length(0) {};
	~SyncroC(); 
	void send(const uint8_t* packet, unsigned long len);
	void receive(uint8_t*& packet, unsigned long& packet_len);
protected:
	void on_configure(void);
private:
	SyncroCConfig config;
	uint8_t* received_packet;
	unsigned long received_packet_length;
	unsigned long buffer_length;

	void read_start_sequence();
};

#ifdef DUCK_USE_NAMESPACE
};
#endif

#endif //__SyncroC_h__
