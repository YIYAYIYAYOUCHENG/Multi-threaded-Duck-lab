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
* @file DuckIO.h
* @brief duck-lab Input/Ouput Component: Gateway Class
* @author Christian Nastasi c.nastasi@sssup.it, nastasichr@gmail.com
* @author Claudio Salvadori c.salvadori@sssup.it
* @version 1.0
* @date 2012-02-12
*/
#ifndef __DuckIO_h__
#define __DuckIO_h__

#include <vector>
#include <utility>
#include <string>
#include <stdint.h>
#include "DuckThread.h"
#include "DuckBuffer.h"

#ifdef DUCK_USE_NAMESPACE
namespace duck {
#endif

class Data;
class FormatHandler;
class StreamerHandler;
//class HandshakeHandler;

/** 
* @brief Input/Output Module
*
*        This class represents the access point to the Input/Output module of
*        the duck-lab framework. 
* \todo  Remove the title (get_title) method?
* \todo  Add a write() method as well as the underlying components for duck-lab^-1
*/
class DuckIO {
public:
	/** 
	* @brief Create from configuration XML file 
	* 
	* @param xml_file Input XML file name
	*/
	DuckIO(const std::string& xml_file);
	/** 
	* @brief Closes and destroies internal handlers 
	*/
	~DuckIO();
	/** 
	* @brief Get the session title 
	* 
	* @return The session title
	*/
	inline std::string get_title(void) { return title; };

	/** 
	* @brief Perform data acquisition 
	*/
	void read(void);
	/** 
	* @brief Get the data vector
	* 
	* @return A reference to the read data vector
	*/
	std::vector<Data*>& get_data_vector(void); 
	/** 
	* @brief Get the data vector
	* 
	* @return A const reference to the read data vector
	*/
	std::vector<const Data*>& get_const_data_vector(void); 

	/**
	* @brief the streaming operation can be handled by a thread
	*/
	DuckThread *streamer_thread;
	/**
	* @brief the result of streaming would be stored in this buffer
	*/
	std::vector<DuckBuffer *> streaming_buffers;
	/**
	* @brief the formatting operation can be dealt by a rw thread,
	*	whose input buffer is the streaming buffer and output
	*	buffers are in format_buffers.
	*/
	DuckThread *formatter_thread;
	std::vector<DuckBuffer*> formatting_buffers;
	/**
	* the number of times the streaming&formatting operation is repeated
	*/
	unsigned long long loops;

	/**
	* to prepare buffers for threads creation  
	*/
	void set_up_buffers(unsigned long long count);

	/** 
	* to create and start the execution of streaming thread  
	*/
	boost::thread *launch_streaming(void);

	/** 
	* to create and start the execution of formatting thread  
	*/
	boost::thread *launch_formatting(void);
	
	/** 
	* to register a buffer in the output buffers of formatting thread  
	*/	
	void register_a_buffer(DuckBuffer *buffer);
private:
	std::string title;		
	FormatHandler* formatter;
	StreamerHandler* streamer;	
	std::vector<const Data*> const_data_vector;
	//HandshakeHandler* handshake;
	//std::vector<Data*> data;
	//std::vector<Data*> copy_vector;
};

#ifdef DUCK_USE_NAMESPACE
};
#endif

#endif // __DuckIO_h__
