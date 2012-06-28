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
* @file StorageHandler.cpp
* @brief StorageHandler Implementation File
* @author Christian Nastasi c.nastasi@sssup.it, nastasichr@gmail.com
* @author Claudio Salvadori c.salvadori@sssup.it
* @version 1.0
* @date 2012-02-27
*/
#include "StorageHandler.h"
#include "Data.h"
#include "Parser.h"
#include "Storage.h"
#include "StorageConfig.h"
#include "StorageFactory.h"
#include "StorageConfigFactory.h"
#include "ParserException.h"
#include "DuckLog.h"
#include "DuckConfig.h"

#define DUCK_SCHEMA DUCK_INSTALL_DIR"data/duck_schema.xsd" 

#ifdef DUCK_USE_NAMESPACE
using namespace duck;
#endif
using namespace std;

StorageHandler::StorageHandler(const string& xml_f, 
			       const vector<const Data*> &input)
{
	init(xml_f, DUCK_SCHEMA, input);
}

StorageHandler::StorageHandler(const string& xml_f, 
			       const std::string& xml_schema,
			       const vector<const Data*>& input)
{
	init(xml_f, xml_schema, input);
}


void StorageHandler::init(const string& xml_f, 
			  const std::string& xml_schema,
			  const vector<const Data*> &input)
{
	// Register input data to the dispatcher
	dispatcher.register_data(input);

	// Create Parser from XML file
	Parser parser(xml_schema, xml_f, "");
	
	// Enter <storage_description> node 
	if (!parser.enter_subnode("storage_description"))
		THROW_PARSER_EXCEPTION("No 'storage_description' node "
				       "found by Parser: current path was: " << 
				       parser.print_path());

	// Enter first child (first storager) of <storage_description>
	if (!parser.enter_subnode())
		THROW_PARSER_EXCEPTION("No child node found for node "
				       "'storage_description' by Parser: "
				       "current path was: " << 
				       parser.print_path());
	// Iterate on the children of <storage_description> (all the storagers)
	do {
		// Create StorageConfig object from node name: <STORAGER_NAME>
		string storager = parser.get_node_name();
		DUCK_DBG("StorageHandler: Creating StorageConfig of type = " << 
			 storager);
		StorageConfig *cfg;
		cfg = StorageConfigFactory::create(storager, parser);
		if (!cfg)
			continue; // TODO: generate exception?
		DUCK_DBG("StorageHandler: Calling configure on StorageConfig"); 
		// Configure StorageConfig from input data	
		cfg->configure(input);
		// Create Storage object from StorageConfig object
		Storage *s;
		DUCK_LOG("StorageHandler: Creating Storage of type = " << 
			 cfg->get_storage_name());
		s = StorageFactory::create(cfg->get_storage_name(), *cfg);
		if (!s) {
			delete cfg;
			continue; // TODO: generate exception?
		}
		DUCK_DBG("StorageHandler: Storage subscribing for data");
		// Subscribe Storage for its subset of data to the Dispatcher
		dispatcher.subscribe(s, cfg->get_data_subset_names());
		// Record storage for final destruction
		storages.push_back(s);
		delete cfg;

	} while (parser.enter_next_sibling());

	parser.exit_subnode(); 
	parser.exit_subnode();
}
		
StorageHandler::~StorageHandler()
{
	for (unsigned long i = 0; i < storages.size(); i++)
		delete storages[i];
}
