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
* @file DuckLog.h
* @brief duck-lab Logging/Debugging Facility 
* @author Christian Nastasi c.nastasi@sssup.it, nastasichr@gmail.com
* @author Claudio Salvadori c.salvadori@sssup.it
* @version 1.0
* @date 2012-02-27
*/
#ifndef __DuckLog_h__
#define __DuckLog_h__

#include "DuckConfig.h"

#ifndef __cplusplus
#error C++ is required to compile duck-lab
#endif

#ifndef __GNUG__
#define __PRETTY_FUNCTION__ __func__
#endif

#include <string>
#include <fstream>

#if DUCK_WARNING_LEVEL == 1

#include <iostream>
#define DUCK_WARNING_ENABLED 

#define DUCK_WARNING(expr) 						\
do {									\
	Logger::Default::stream() << "DUCK_WARNING: " <<		\
		 __func__ << " : " << expr <<				\
		 std::dec << std::endl;					\
} while (0)

#elif DUCK_WARNING_LEVEL == 2

#include <iostream>
#define DUCK_WARNING_ENABLED 

#define DUCK_WARNING(expr) 					\
do {								\
	Logger::Default::stream() << "DUCK_WARNING["		\
		<< __FILE__ << ":" << __LINE__ << "] "		\
		<< __PRETTY_FUNCTION__ << " : " << expr 	\
		<< std::dec << std::endl;			\
} while (0)

#else 

#define DUCK_WARNING(expr)

#endif /* DUCK_WARNING_LEVEL */


#if DUCK_LOG_LEVEL == 1

#include <iostream>
#define DUCK_LOG_ENABLED 

#define DUCK_LOG(expr) 					\
do {							\
	Logger::Default::stream() << "DUCK_LOG: " <<	\
		 __func__ << " : " << expr <<		\
		std::dec << std::endl;			\
} while (0)

#elif DUCK_LOG_LEVEL == 2

#include <iostream>
#define DUCK_LOG_ENABLED 

#define DUCK_LOG(expr) 							   \
do {									   \
	Logger::Default::stream() << "DUCK_LOG[" 			   \
		<< __FILE__ << ":" << __LINE__ << "] "			   \
		<< __PRETTY_FUNCTION__ << " : " << expr 		   \
		<< std::dec << std::endl;				   \
} while (0)

#else 

#define DUCK_LOG(expr)

#endif /* DUCK_LOG_LEVEL */


#if DUCK_DEBUG_LEVEL == 1

#include <iostream>
#define DUCK_DEBUG_ENABLED 

#define DUCK_DBG(expr) 					\
do {							\
	Logger::Default::stream() << "DUCK_DEBUG: " <<	\
		 __func__ << " : " << expr <<		\
		std::dec << std::endl;			\
} while (0)

#elif DUCK_DEBUG_LEVEL == 2

#include <iostream>
#define DUCK_DEBUG_ENABLED 

#define DUCK_DBG(expr) 							     \
do {									     \
	Logger::Default::stream() << "DUCK_DEBUG[" 			     \
		<< __FILE__ << ":" << __LINE__ << "] "			     \
		<< __PRETTY_FUNCTION__ << " : " << expr 		     \
		<< std::dec << std::endl;				     \
} while (0)

#else 

#define DUCK_DBG(expr)

#endif /* DUCK_DEBUG_LEVEL */

#ifdef DUCK_USE_NAMESPACE
namespace duck {
#endif

// NOTE: we use pointers rather than static objects to avoid the C++ 
//       static-objects order initialization fiasco
/** 
* @brief Logging class for duck-lab framework
*
* This class implements the logging system which is used by the other 
* modules (classes) of the duck-lab framework.
* \code
* TODO
* \endcode
* @todo Make an example of usage of this class
* @todo Make example of usages of the macros? Maybe not
*/
class Logger {
public:
	/** 
	* @brief This is the default \ref Logger class
	* 
	* This is a single-tone-like class meant to be used for log messages 
	* through a \e default logger.
	* The class exports the static version of the \ref Logger class methods.
	* 
	* The output stream is initialized according to the definition of the
	* \c DUCK_DAFAULT_LOGGER_FILENAME pre-compiler macro. 
	* - if the macro is not defined then the \ref Logger::Logger() is used;
	* - otherwise the macro must be defined to a string-literal representing
	*   filename parameter for \ref Logger::Logger(const std::string& f).
	*/
	class Default {
	public:
		~Default();
		/** 
		* @brief Wraps \ref Logger::active(). 
		*/
		static inline void active(void); 
		/** 
		* @brief Wraps \ref Logger::inactive(). 
		*/
		static inline void inactive(void); 
		/** 
		* @brief Wraps \ref Logger::stream(). 
		*/
		static inline std::ostream& stream(void); 
	private:
		static inline void check_logger(void); 
		static Logger *def_logger;
	};

public:
	/** 
	* @brief  Create logger to <code>std::clog</code> stream
	*/
	Logger(); 
	/** 
	* @brief Create logger to output file
	* 
	* @param f Name of the file which should contain the logger output
	*/
	Logger(const std::string& f); 
	/** 
	* @brief Create logger to given output stream 
	* 
	* @param os \c std::ostream to which logger output has to be redirected
	*/
	Logger(std::ostream& os); 
	/** 
	* @brief Flush and close (if appropriate) the stream 
	* 
	* The stream will be closed if the logger has been created with the 
	* \ref Logger::Logger(const std::string& f) constructor. 
	*/
	~Logger();

	/** 
	* @brief De-activate the logger 
	*
	* Any further write to the logger stream will have no effect until the
	* logger is set to active through the \ref Logger::active() method.
	*/
	inline void inactive(void) 
	{
		logger_stream->flush();
		is_active = false;
	}
	/** 
	* @brief (Re-)Activate the logger 
	*/
	inline void active(void)
	{
		is_active = true;
	}
	/** 
	* @brief Get the logger stream
	*
	* This method has to be used to access the logger stream which is 
	* necessary to perform the actual logging (write) operations.
	* 
	* @return The stream to used for logging
	*/
	inline std::ostream& stream(void) 
	{
		if (!is_active)
			return *fake_stream;
		return *logger_stream;
	}
private:
	void print_header(void);

	bool is_active;
	bool is_my_stream;
	std::ostream *logger_stream;

	static std::ofstream* fake_stream;
};

inline void Logger::Default::active(void) 
{
	check_logger();
	def_logger->active();
}
inline void Logger::Default::inactive(void) 
{
	check_logger();
	def_logger->inactive();
}
inline std::ostream& Logger::Default::stream(void) 
{
	check_logger();
	return def_logger->stream();
}
inline void Logger::Default::check_logger(void) 
{
	if (!def_logger)
		#ifdef DUCK_DAFAULT_LOGGER_FILENAME
		def_logger = new Logger(DUCK_DAFAULT_LOGGER_FILENAME);
		#else
		def_logger = new Logger();
		#endif
}

#ifdef DUCK_USE_NAMESPACE
};
#endif

#endif /* __DuckLog_h__ */
