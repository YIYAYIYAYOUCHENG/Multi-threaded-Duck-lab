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
* @file Length.cpp
* @brief Length Implementation File
* @author Christian Nastasi c.nastasi@sssup.it, nastasichr@gmail.com
* @author Claudio Salvadori c.salvadori@sssup.it
* @version 1.0
* @date 2012-02-27
*/
#include "Length.h"
#include "PolicyFactory.h"
#include "Stream.h"
#include "DuckException.h"
#include "DuckLog.h"

#ifdef DUCK_USE_NAMESPACE
using namespace duck;
#endif

typedef Length::LengthConfig CfgClass;
DUCK_REGISTER_POLICY_CLASSES("length", Length, CfgClass);

void Length::on_configure(void)
{
	if (packet)
		delete[] packet; // BTW, this should never happen!
	packet = new uint8_t[expected_packet_length];
}

void Length::send(const uint8_t* packet, unsigned long len)
{
	if (!stream)
		return; // TODO: generate exception?
	stream->write(packet, len); //TODO: ignoring retval
}

void Length::receive(uint8_t*& pkt, unsigned long& len)
{
	if (!packet || !stream)
		return;
	unsigned long l = stream->read(packet, expected_packet_length);
	if (l != expected_packet_length)
		; // TODO: what do we do???? bad STUFF!!
	len = l;
	pkt = packet;
}
