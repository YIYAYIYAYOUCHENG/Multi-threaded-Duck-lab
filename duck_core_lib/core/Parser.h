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
* @file Parser.h
* @brief XML Parser Class
* @author Christian Nastasi c.nastasi@sssup.it, nastasichr@gmail.com
* @author Claudio Salvadori c.salvadori@sssup.it
* @version 1.0
* @date 2012-02-27
*/
#ifndef __Parser_h__
#define __Parser_h__

#include "xercesc/parsers/XercesDOMParser.hpp"
#include "xercesc/sax/ErrorHandler.hpp"
#include "xercesc/sax/SAXParseException.hpp"
#include "xercesc/util/TransService.hpp"

#include <vector>
#include <string>
#include <stack>

#ifdef DUCK_USE_NAMESPACE
namespace duck {
#endif

/** 
* @brief XML parser class
*
* This class is a XML parser which allows to visit the XML-tree nodes with a  
* generic interface.
*
* @todo The class should use a PIMPL idiom to hide the implementation details 
*       related to the XERCES library usage.
*/
class Parser {
public:
	/** 
	* @brief Create the parser from an XML file and a schema
	* @todo What is the file_type parameter of this constructor? 
	*       It's not even used! REMOVE THIS?
	* @todo What is the behaviour if no schema (empty string) is used? 
	* @param schema    The XSD filename to be used as XML schema. If empty,
	*                  no schema is used.
	* @param file      The input XML filename
	* @param file_type ?
	* @throw ParserException If one or more errors occur while parsing 
	*        the XML file.
	*/
	// FIXME: remove the file_type parameter?
	Parser(std::string schema, std::string file, std::string file_type);
	/** 
	* @brief Close the parser internal structures 
	*/
	~Parser();

	/** 
	* @brief Enter first-child of current node
	* 
	* @return \c true if operation succeeded, \c false otherwise.
	*/
	bool enter_subnode(void);
	/** 
	* @brief Enter named child of current node
	* @param target_name Name of the child node to be entered. 
	* @return \c true if operation succeeded, \c false otherwise.
	*/
	bool enter_subnode(const std::string& target_name);
	/** 
	* @brief Exit current node, going up one level.
	*/
	void exit_subnode(void);
	/** 
	* @brief Enter first sibling node (first child of father-node)
	*/
	void restart_sibling_exploration(void);
	/** 
	* @brief  Enter next sibling node of current node.
	* @return \c true if operation succeeded, \c false otherwise.
	*/
	bool enter_next_sibling(void);
	/** 
	* @brief  Print the current visiting path 
	* @return Path (slash-separated /) to the current visited node
	*/
	std::string print_path(void);

	/** 
	* @brief  Get current node name 
	* @return The name of the node 
	*/
	std::string get_node_name(void);
	/** 
	* @brief  Get current node text 
	* @return The text of the node 
	*/
	std::string get_node_text(void);
	/** 
	* @brief Get current node attribute
	* @param[in]  a_name The name of the attribute (key)
	* @param[out] value  The value of the attribute
	* @return \c true if operation succeeded, \c false otherwise.
	*/
	bool get_node_attribute(const std::string& a_name, std::string& value);
	
private:
	class ParserErrorHandler : public XERCES_CPP_NAMESPACE_QUALIFIER ErrorHandler {
		void warning(XERCES_CPP_NAMESPACE_QUALIFIER SAXParseException 
			     const& toCatch);
		void error(XERCES_CPP_NAMESPACE_QUALIFIER SAXParseException 
			   const& toCatch);
		void fatalError(XERCES_CPP_NAMESPACE_QUALIFIER SAXParseException
				const& toCatch);
		void resetErrors();
	};
	
	struct PathElem {
		PathElem(XERCES_CPP_NAMESPACE_QUALIFIER DOMNode* f, 
			 XERCES_CPP_NAMESPACE_QUALIFIER DOMNode* c) : 
			first_node(f), current_node(c) {};
		XERCES_CPP_NAMESPACE_QUALIFIER DOMNode* first_node;
		XERCES_CPP_NAMESPACE_QUALIFIER DOMNode* current_node;
	};

private:
	std::stack<PathElem> current_path;
	XERCES_CPP_NAMESPACE_QUALIFIER XercesDOMParser *xml_dom_parser;		
	XERCES_CPP_NAMESPACE_QUALIFIER XMLTranscoder *transcoder;		
	ParserErrorHandler *xml_error_handler;

private:
	// chris TODO: Move this elsewhere using PIMPL IDIOM
	//             Hopefully after that all this dirty stuff about XERCES will
	//	       be hidden to the Parser user    

	XERCES_CPP_NAMESPACE_QUALIFIER DOMNode* 
	find_node_from_node(XERCES_CPP_NAMESPACE_QUALIFIER DOMNode* node, 
			    const std::string& t);

	XERCES_CPP_NAMESPACE_QUALIFIER DOMNode* 
	get_first_child_node(const XERCES_CPP_NAMESPACE_QUALIFIER DOMNode* node);

	XERCES_CPP_NAMESPACE_QUALIFIER DOMNode* 
	get_child_node(const XERCES_CPP_NAMESPACE_QUALIFIER DOMNode* node, 
		       const std::string& t);

	XERCES_CPP_NAMESPACE_QUALIFIER DOMNode* 
	get_next_sibling(const XERCES_CPP_NAMESPACE_QUALIFIER DOMNode *node);

	void get_child_vector(const XERCES_CPP_NAMESPACE_QUALIFIER DOMNode* node, 
			      std::vector<XERCES_CPP_NAMESPACE_QUALIFIER DOMNode*>& ls);

	std::string get_node_name(const XERCES_CPP_NAMESPACE_QUALIFIER DOMNode* node);

	std::string get_node_text(const XERCES_CPP_NAMESPACE_QUALIFIER DOMNode* node);

	bool get_attribute(const XERCES_CPP_NAMESPACE_QUALIFIER DOMNode* node, 
			   const std::string& attrname, std::string& value);

	inline bool is_target_node(const XERCES_CPP_NAMESPACE_QUALIFIER DOMNode* node, 
				   const std::string& t);

	// chris FIXME: remove this function and use XMLString::transcode directly?
	//              See Parser.cpp comment on this function
	std::string xmlstr_2_str(const XMLCh* s);
	// chris FIXME: who calls this method? Apparently none, remove it?
	void set_transcoder(char *codepage);
};

#ifdef DUCK_USE_NAMESPACE
};
#endif
	
#endif
