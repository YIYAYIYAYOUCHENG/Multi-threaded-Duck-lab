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
#include "StorageHandler.h"
#include "StorageException.h"
#include "DuckConfig.h"

#include "DataInt8.h"
#include "DataUInt8.h"

#include <iostream>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>

#ifdef DUCK_USE_NAMESPACE
using namespace duck;
#endif
using namespace std;

#define TEST_MAX_LOOP 30
#define TEST_LOG(expr) cout << "[TEST LOG] : " << expr << endl

/* NOTE: these names MUST match those in the XML */
static string data1_name = "ascii message";
static string data2_name = "non-ascii data";
static string data3_name = "byte data";
static string data4_name = "byte array data";

static DataInt8 *data1;
static DataInt8 *data2;
static DataUInt8 *data3;
static DataUInt8 *data4;

static void load_data_vector(vector<const Data*> &data)
{
	int8_t init_val[] = {0, 0, 0, 0};
	data1 = new DataInt8(data1_name, 10, NULL);
	data2 = new DataInt8(data2_name, 3, init_val);
	data3 = new DataUInt8(data3_name, 1, (unsigned char*) init_val);
	data4 = new DataUInt8(data4_name, 4, (unsigned char*) init_val);
	data.clear();
	data.push_back(data1);
	data.push_back(data2);
	data.push_back(data3);
	data.push_back(data4);
}

static void produce_data(void)
{
	static unsigned data1_content_idx = 0;

	const char *data1_content[5] = {
		"Test Msg 1", "Test Msg 2", "3 gattini.",
		"son 4 anni", "5 inquinai",
	};

	for (unsigned i = 0; i < 10; i++)
		data1->data[i] = data1_content[data1_content_idx][i];
	data1_content_idx  = (data1_content_idx + 1) % 5;

	for (unsigned i = 0; i < 3; i++)
		data2->data[i]++;

	data3->data[0]--;

	for (unsigned i = 0; i < 4; i++)
		data4->data[i] = (i == 0) ? data4->data[i] + 1 : 
					    data4->data[i - 1] + 2;
}

int main(int argc, char* argv[])
{
	int test_result = 0;
	argc--;
	if (argc < 1) {
		TEST_LOG("The XML file should be bassed as unique argument.");
		TEST_LOG("Usage: " << argv[0] << " <XML_FILE>");
		exit(EXIT_FAILURE);
	}
	const char *xml_filename = argv[1];

	vector<const Data*> data;

	TEST_LOG(" -> Storage Integration Test: __TextFile__");

	TEST_LOG("Create data");
	load_data_vector(data);

	try { // Scope operator

	TEST_LOG("Creating storage handler from XML '" << xml_filename << "'");
	StorageHandler manager(xml_filename, data);

	TEST_LOG("Start storing loop");
	unsigned long int loop = 0;
	while (loop++ < TEST_MAX_LOOP) {
		TEST_LOG("STORE LOOP [" << loop << "]: BEGIN  " <<
			 "---------------------------------------------------");
		produce_data();
		manager.store();
		TEST_LOG("STORE LOOP [" << loop << "]: END    " <<
			 "---------------------------------------------------");
	}
	TEST_LOG("End storing loop");
	delete data1;
	delete data2;
	delete data3;
	delete data4;

	// StorageHandler scope ends here: destructors called
	} catch (StorageException e) {
		TEST_LOG("StorageException Caught: " << e.what());
		test_result++;
	} catch (exception e_gen) {
		TEST_LOG("std::exception Caught: " << e_gen.what());
		test_result++;
	}

	TEST_LOG("Start check loop");
	TEST_LOG("TODO: Check file produced according to input XML file");
	// TODO: do actual test!!!
	TEST_LOG("End check loop");
	return test_result;
}
