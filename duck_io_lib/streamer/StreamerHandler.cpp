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
* @file StreamerHandler.cpp
* @brief StreamerHandler Implementation File 
* @author Christian Nastasi c.nastasi@sssup.it, nastasichr@gmail.com
* @author Claudio Salvadori c.salvadori@sssup.it
* @version 1.0
* @date 2012-02-27
*/
#include "StreamerHandler.h"
#include "Parser.h"
#include "Stream.h"
#include "Escape.h"
#include "Policy.h"
#include "StreamFactory.h"
#include "PolicyFactory.h"
#include "EscapeFactory.h"
//#include "Handshake.h"
#include "DuckLog.h"
//#include "duck-lab.cpp"
#include "ParserException.h"
#include "DuckIOException.h"

#include <string.h>
#include <iostream>

#ifdef DUCK_USE_NAMESPACE
using namespace duck;
#endif
using namespace std;

StreamerHandler::StreamerHandler(Parser &p, unsigned long packet_size) 
{
	// chris TODO: is this constructor even called? Why?
	if (packet_size == 0)
		THROW_PARSER_EXCEPTION("Cannot create with packet_size=0");
	init_stream(p);
	init_policy(p);
	init_escape(p);
	init_handshake(p);

	policy->configure(stream, packet_size);
	if (escape)
		escape->configure(packet_size);
}


void StreamerHandler::init_stream(Parser &p)
{
	if (!p.enter_subnode("stream")) 
		THROW_PARSER_EXCEPTION("no 'stream' node found in XML");
	if (!p.get_node_attribute("stream_type", stream_type))
		THROW_PARSER_EXCEPTION("no 'stream_type' "
				       "attribute for the stream node");
	stream = StreamFactory::create(stream_type, p);
	p.exit_subnode();
}

void StreamerHandler::init_policy(Parser &p)
{
	if (!p.enter_subnode("acquisition_option")) 
		THROW_PARSER_EXCEPTION("no 'acquisition_option' node found "
				       "in XML");
	if (!p.get_node_attribute("acquisition_type", policy_type))
		THROW_PARSER_EXCEPTION("no 'acquisition_type' attribute for the"
				       " acquisition_option node");
	policy = PolicyFactory::create(policy_type, p);
	p.exit_subnode();
}

void StreamerHandler::init_escape(Parser &p)
{
	if (!p.enter_subnode("escape") || 
	    !p.get_node_attribute("escape_type", escape_type)) {
		escape_type = "";
		escape = NULL;
	} else {
		escape = EscapeFactory::create(escape_type, p);
	}
	p.exit_subnode();
}

void StreamerHandler::init_handshake(Parser &p)
{
	//if (!p.enter_subnode("init")) 
	//	THROW_PARSER_EXCEPTION("no 'init' node found in XML");
	//if (!p.get_node_attribute("handshake", handshake_type))
	//	handshake_type = "";
	//	handshake = NULL;
	//} else {
	//	if (handshake_type != "no")
	//		handshake = HanshakeFactory::create(handshake_type, p);
	//}
	//p.exit_subnode();
}


void StreamerHandler::read(void)
{
	uint8_t *buf;
	unsigned long len;

	policy->receive(buf, len);
	if (escape)
		escape->unescape(buf, len, buf, len);
	packet = buf;
	length = len;
}

pair<uint8_t*, unsigned long>* StreamerHandler::get(void)
{
	uint8_t *elem = new uint8_t[length];
	unsigned int i;

	read();
	elem = new uint8_t[length];
	for( i = 0; i < length; i++)
		elem[i] = packet[i];

	return new pair<uint8_t*, unsigned long>(elem, length);
}
