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
* @file Length.h
* @brief Length Concrete \ref Policy Class 
* @author Christian Nastasi c.nastasi@sssup.it, nastasichr@gmail.com
* @author Claudio Salvadori c.salvadori@sssup.it
* @version 1.0
* @date 2012-02-27
*/
#ifndef __Length_h__
#define __Length_h__

#include "Policy.h"
#include "PolicyConfig.h"

#ifdef DUCK_USE_NAMESPACE
namespace duck {
#endif

class Parser;

class Length: public Policy {
public:
	class LengthConfig : public PolicyConfig {
	public:
		LengthConfig(Parser& p) {/* Nothing to do */};
	};
public:
	Length(const PolicyConfig &) : Policy(), packet(NULL) {};
	void send(const uint8_t* packet, unsigned long len);
	void receive(uint8_t*& packet, unsigned long& packet_len);
protected:
	void on_configure(void);
private:
	uint8_t* packet;
};

#ifdef DUCK_USE_NAMESPACE
};
#endif

#endif //__Length_h__
