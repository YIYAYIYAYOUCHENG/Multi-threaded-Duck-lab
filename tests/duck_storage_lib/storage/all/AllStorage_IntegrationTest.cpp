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

#ifdef DUCK_USE_ROOT_CERN
#include "TTree.h" 
#include "TFile.h" 
#include "RootStorage/RootStorageEvent.h"
#define TEST_OUTPUT_ROOT_FILE "AllStorage_IntegrationTest_RootStorage.root"
#endif

#ifdef DUCK_USE_NAMESPACE
using namespace duck;
#endif
using namespace std;

#define TEST_MAX_LOOP 30
#define TEST_LOG(expr) cout << "[TEST LOG] : " << expr << endl

/* NOTE: this name MUST be the same as the XML */
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

static int check_rootstorage(void)
{
	int error_cnt = 0;
#ifdef DUCK_USE_ROOT_CERN
	RootStorageEvent *event = NULL;

	TFile *f = new TFile (TEST_OUTPUT_ROOT_FILE);
	TTree *t = (TTree*) f->Get("duck_tree");
	TBranch *b= t->GetBranch("duck_event");
	b->SetAddress(&event);
        int n_event = t->GetEntries();
	if (n_event != TEST_MAX_LOOP) {
		TEST_LOG("ERROR: bad GetEntries() result:" << 
			 "expected " << (int) TEST_MAX_LOOP << 
			 " got " << n_event
		);
		error_cnt++;
		goto return_point;
	} else {
		TEST_LOG("CHECK: GetEntries() = OK");
	}

	TEST_LOG("TEST RootStorage Result");
	for (int i = 0; i < n_event; i++) {
		t->GetEntry(i);
		if (event == NULL) {
			TEST_LOG("event is NULL");
			error_cnt++;
			continue;
		}
		vector<Data*> data;
		event->get_data(data);
		if (data.size() != 4) { 
			TEST_LOG("ERROR: bad data.size() " << 
				 "expected " << (int) 4 << 
				 " got " << data.size()
			);
			error_cnt++;
		} else {
			TEST_LOG("CHECK: data.size() = OK");
		}

		data1 = static_cast<DataInt8*>(data[0]);
		data2 = static_cast<DataInt8*>(data[1]);
		data3 = static_cast<DataUInt8*>(data[2]);
		data4 = static_cast<DataUInt8*>(data[3]);
		#define PRINT_HEADER(id)\
		TEST_LOG("Root Post-Processing LOOP [" << i << "]: " << \
			 "DATA INFO " << (int) id << " : " <<\
			 "type=" << data##id->get_type() <<\
			 " type_size=" << data##id->get_type_size() <<\
			 " len=" << data##id->get_len() <<\
			 " name=" << data##id->get_name()\
		);
		if (i == 0) {
			PRINT_HEADER(1);
			PRINT_HEADER(2);
			PRINT_HEADER(3);
			PRINT_HEADER(4);
		}

		TEST_LOG("Root Post-Processing LOOP [" << i << "]: " << 
			 "DATA 1 : " <<
			 data1->data[0] << data1->data[1] << data1->data[2] <<
			 data1->data[3] << data1->data[4] << data1->data[5] <<
			 data1->data[6] << data1->data[7] << data1->data[7] <<
			 data1->data[8] << data1->data[9] 
		);
		TEST_LOG("Root Post-Processing LOOP [" << i << "]: " << 
			 "DATA 2 : " << dec <<
			 (int) data2->data[0] << " " << 
			 (int) data2->data[1] << " " << 
			 (int) data2->data[2] 
		);
		TEST_LOG("Root Post-Processing LOOP [" << i << "]: " << 
			 "DATA 3 : " << dec << (unsigned) data3->data[0]  
		);
		TEST_LOG("Root Post-Processing LOOP [" << i << "]: " << 
			 "DATA 4 : " << dec <<
			 (unsigned) data4->data[0] << " " << 
			 (unsigned) data4->data[1] << " " << 
			 (unsigned) data4->data[2] << " " << 
			 (unsigned) data4->data[3] 
		);
		TEST_LOG("CHECK: TODO: check output against expected data!");
		// TODO: perform actual check!
	}
return_point:
#endif
	return error_cnt;	
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

	TEST_LOG(" -> Storage Library Integration TEST");

	TEST_LOG("Create Data");
	load_data_vector(data);

	try { // Scope operator

	TEST_LOG("Creating Storage Handler");
	StorageHandler manager(xml_filename, data);

	TEST_LOG("Start Storaginging loop");
	unsigned long int loop = 0;
	while (loop++ < TEST_MAX_LOOP) {
		TEST_LOG("STORAGE LOOP [" << loop << "]: BEGIN  " <<
			 "---------------------------------------------------");
		produce_data();
		manager.store();
		TEST_LOG("STORAGE LOOP [" << loop << "]: END    " <<
			 "---------------------------------------------------");
	}
	delete data1;
	delete data2;
	delete data3;
	delete data4;
	// StorageHandler scope ends here: destructors called
	} catch (DuckStorageException e) {
		TEST_LOG("DuckStorageException Caught: " << e.what());
		test_result++;
	} catch (DuckException e2) {
		TEST_LOG("DuckException Caught: " << e2.what());
		test_result++;
	} catch (exception e_gen) {
		TEST_LOG("std::exception Caught: " << e_gen.what());
		test_result++;
	}

	TEST_LOG("Start Check loop");
	test_result += check_rootstorage();
	TEST_LOG("End of Storaging loop");

	return test_result;
}
