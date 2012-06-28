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
* @file Parser.cpp
* @brief Parser Implementation File
* @author Christian Nastasi c.nastasi@sssup.it, nastasichr@gmail.com
* @author Claudio Salvadori c.salvadori@sssup.it
* @version 1.0
* @date 2012-02-27
*/
#include "Parser.h"
#include "ParserException.h"
#include "DuckLog.h"

// chris FIXME: are all this include relevant?
#include "xercesc/parsers/XercesDOMParser.hpp"
#include "xercesc/dom/DOMElement.hpp"
#include "xercesc/dom/DOMLocator.hpp"
#include "xercesc/dom/DOMException.hpp"
#include "xercesc/dom/DOMNamedNodeMap.hpp" 
#include "xercesc/util/XMLString.hpp"
#include "xercesc/util/PlatformUtils.hpp"
#include "xercesc/util/TransService.hpp"
#include "xercesc/dom/DOMNodeList.hpp"
#include "xercesc/dom/DOMText.hpp"
#include "xercesc/sax/SAXParseException.hpp"

#include <sstream>

#ifdef DUCK_USE_NAMESPACE
using namespace duck;
#endif
using namespace std;

XERCES_CPP_NAMESPACE_USE; 

Parser::Parser(string schema, string file, string file_type)
{
	//chris: FIXME: I didn't understand the following comment :
	//TODO: file_type is the type of configuration file used. We decide to
	// convert that in xml using the schema fixed by the schema parameter.
	DUCK_LOG("schema='" << schema << "' file='" <<
		 file << "' file_type='" << file_type << "'");

	// Note: calls to this one MUST occur before xml_dom_parser creation
	//       This apparently seems to be an issue with the internals
	//       of the xerces library. We keep the pointer-based implementation
	//       as follows, and we do the same thing for xml_error_handler.
	XMLPlatformUtils::Initialize();

	// Initially, do not have transocoder (TODO: using default one?)
	transcoder = NULL;
	xml_dom_parser = new XercesDOMParser();
	xml_error_handler = new ParserErrorHandler();

	// Initializes xml_dom_parser
	if (!schema.empty()) {
		DUCK_DBG("Initializing parser with schema: " << schema);
		xml_dom_parser->setValidationScheme(XercesDOMParser::Val_Always);
		xml_dom_parser->setDoNamespaces(true);
		xml_dom_parser->setDoSchema(true);
		const char *s = schema.c_str();
		xml_dom_parser->setExternalNoNamespaceSchemaLocation(s);
	} else {
		DUCK_DBG("Initializing parser without schema.");
	}
	xml_dom_parser->setCreateEntityReferenceNodes(false);
	xml_dom_parser->setErrorHandler(xml_error_handler);

	// Parser xml
	xml_dom_parser->parse(file.c_str());
	int errorCount = xml_dom_parser->getErrorCount();
	if (errorCount > 0)
		THROW_PARSER_EXCEPTION(errorCount << " error(s) occurred while "
				       "parsing XML file '" << file << "' with "
				       "schema '" << schema << "'.");

	// Set current path to the root
	DOMNode* root = xml_dom_parser->getDocument();
	current_path.push(PathElem(root, root));
}

Parser::~Parser()
{
	DUCK_DBG("Closing parser");
	if (transcoder)
		delete transcoder;
	if (xml_dom_parser)
		delete xml_dom_parser;
	if (xml_error_handler)
		delete xml_error_handler;
}

void Parser::exit_subnode(void)
{
	if (!current_path.empty())
		current_path.pop();
}

bool Parser::enter_subnode(const string& target_name) 
{
	DOMNode* n;
	n = find_node_from_node(current_path.top().current_node, target_name);
	if (n == NULL)  
		return false;
	current_path.push(PathElem(n, n));
	return true;
}

bool Parser::enter_subnode(void) 
{
	DOMNode* n = get_first_child_node(current_path.top().current_node); 
	if (n == NULL)  
		return false;
	current_path.push(PathElem(n, n));
	return true;
}

void Parser::restart_sibling_exploration(void)
{
	current_path.top().current_node = current_path.top().first_node;
} 

bool Parser::enter_next_sibling(void) 
{
	DOMNode* n = get_next_sibling(current_path.top().current_node); 
	if (n == NULL)  
		return false;
	current_path.top().current_node = n;
	DUCK_DBG("Entered next sibling: PATH=" << print_path());
	return true;
}

bool Parser::get_node_attribute(const string &a, string &v)
{
	return get_attribute(current_path.top().current_node, a, v);
}

string Parser::get_node_name(void)
{
	return get_node_name(current_path.top().current_node);
}

string Parser::get_node_text(void)
{
	return get_node_text(current_path.top().current_node);
}

string Parser::print_path(void)
{
	string str = "";
	stack<PathElem> path = current_path;
	
	while (!path.empty()) {
		str = get_node_name(path.top().current_node) + "/" + str;
		path.pop();
	}
	str = "/" + str;
	return str;
}

inline bool Parser::is_target_node(const DOMNode* node, const std::string& t)
{
	if (node->getNodeType() != DOMNode::ELEMENT_NODE) 
		return false;
	if (t != xmlstr_2_str(node->getNodeName()))
		return false;
	return true;
}

DOMNode* Parser::get_first_child_node(const DOMNode* node)
{
	DOMNode* child = node->getFirstChild();
    	for(; child != NULL; child = child->getNextSibling())
		if (child->getNodeType() == DOMNode::ELEMENT_NODE)
			return child;
	return NULL;
}

DOMNode* Parser::get_child_node(const DOMNode* node, const string& targetname)
{
	DOMNode* child = node->getFirstChild();
    	for(; child != NULL; child = child->getNextSibling())
		if (is_target_node(child, targetname))
			return child;
	return NULL;
}

// Recursive Function
DOMNode* Parser::find_node_from_node(DOMNode* node, const std::string& id)
{
	if (is_target_node(node, id)) {
		// Recursion termiation 1 : target is found
		DUCK_DBG("Target node found with id=" << id);
		return node;
	}
	DOMNode* child = node->getFirstChild();
    	for (; child != NULL; child = child->getNextSibling()) {
		// Recursive call
		DOMNode* res = find_node_from_node(child, id);
		if (res == NULL)
			continue;
		else
			return res;
	}
	// Recursion termiation 2 : target is not found
	return NULL;
}

bool Parser::get_attribute(const DOMNode* node, const string &a, string &v)
{
	DOMNamedNodeMap* attributes;
	attributes = node->getAttributes();
	v = "";
	if (attributes == NULL)
		return false;
	int len = attributes->getLength();
	for (int i = 0; i < len; i++) {
		DOMNode*  attribute = attributes->item(i);
		string name = xmlstr_2_str(attribute->getNodeName());
		v = xmlstr_2_str(attribute->getNodeValue());
		if (name == a){
			DUCK_DBG("Attribute found with name= " << a << 
				 " pos=" << i << " value=" << v);
			return true;
		}
	}
	DUCK_DBG("Attribute NOT found with name= " << a);
	return false; 
}

DOMNode* Parser::get_next_sibling(const DOMNode *node)
{
	DOMNode* sibling = node->getNextSibling();
    	for(; sibling != NULL; sibling = sibling->getNextSibling())
		if (sibling->getNodeType() == DOMNode::ELEMENT_NODE)
			return sibling;
	return NULL;
}

void Parser::get_child_vector(const DOMNode* node, vector<DOMNode*>& nodes)
{
	DOMNodeList *children = node->getChildNodes();
     	const XMLSize_t node_count = children->getLength();
	for (XMLSize_t i = 0; i < node_count; ++i) {
		DOMNode* currentNode = children->item(i);
		// chris FIXME: why is the first condition:
		//		'currentNode->getNodeType()'?
		//              REMOVE this hard-coded check!
        	if (currentNode->getNodeType() && 
		    currentNode->getNodeType() == DOMNode::ELEMENT_NODE)
			nodes.push_back(currentNode);
	}
}

string Parser::get_node_name(const DOMNode* node)
{
	return xmlstr_2_str(node->getNodeName());
}

string Parser::get_node_text(const DOMNode* node)
{
    	for (DOMNode* child = node->getFirstChild(); child != NULL; 
	     child = child->getNextSibling())
		if (child->getNodeType() == DOMNode::TEXT_NODE) 
			return xmlstr_2_str(child->getNodeValue());
	return "";
}

void Parser::set_transcoder(char *codepage)
{
	XMLTransService::Codes resCode;
	transcoder = XMLPlatformUtils::fgTransService->makeNewTranscoderFor(
						      codepage, resCode, 65536);
	// TODO: shall we ignore resCode or throw exc if something wrong?
}

string Parser::xmlstr_2_str(const XMLCh* s)
{	
// chris FIXME: this function alway HAS transcoder == NULL, since none
//              is calling set_transcoder() PRIVATE method. shal we remove this?

	if (transcoder == NULL)
		return XMLString::transcode(s);
	
	// TODO: have we checked this code?
	unsigned int tmp;
	unsigned int len = XMLString::stringLen(s);
	char ret[len + 1]; // FIXME: it was large: len * 2 (WHY?)
	ret[len] = '\0';
	transcoder->transcodeTo(s, (XMLSize_t)len, 
				(XMLByte*)ret, (XMLSize_t)len, 
				(XMLSize_t&)tmp, XMLTranscoder::UnRep_Throw);
	// FIXME: check tmp and transcoder to see if we got a problem?
	return ret;
}

void Parser::ParserErrorHandler::warning(const SAXParseException& toCatch)
{
	DUCK_WARNING("XML WARNING in file '" << 
		     XMLString::transcode(toCatch.getSystemId()) << 
		     "' , line=" << toCatch.getLineNumber() << " column=" << 
		     toCatch.getColumnNumber() << endl << "Message: " <<
		     XMLString::transcode(toCatch.getMessage()) << endl
	);
	// TODO: generate an exception? Or, is it safe to carry on?
}

void Parser::ParserErrorHandler::error(const SAXParseException& toCatch)
{
	ostringstream ss;

	ss << "XML ERROR in file '" << 
	      XMLString::transcode(toCatch.getSystemId()) << "' , line=" <<
	      toCatch.getLineNumber() << " column=" << 
	      toCatch.getColumnNumber() << endl << "Message: " <<
	      XMLString::transcode(toCatch.getMessage()) << endl;

	DUCK_WARNING(ss.str());
	THROW_PARSER_EXCEPTION(ss.str());
}

void Parser::ParserErrorHandler::fatalError(const SAXParseException& toCatch)
{
	ostringstream ss;

	ss << "XML FATAL ERROR: in file '" << 
	      XMLString::transcode(toCatch.getSystemId()) << "' , line=" <<
	      toCatch.getLineNumber() << " column=" << 
	      toCatch.getColumnNumber() << endl << "Message: " <<
	      XMLString::transcode(toCatch.getMessage()) << endl;

	DUCK_WARNING(ss.str());
	THROW_PARSER_EXCEPTION(ss.str());
}

void Parser::ParserErrorHandler::resetErrors()
{
	// Nothing to do
}
