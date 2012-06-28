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
#include "DuckIO.h"
#include "DuckConfig.h"
#include "DuckException.h"
#include "StreamException.h"

#include "DataUInt16.h"
#include "DataFloat32.h"
#include "DataInt8.h"

#include <iostream>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#ifdef DUCK_USE_NAMESPACE
using namespace duck;
#endif
using namespace std;

#define TEST_MAX_LOOP 30
#define TEST_LOG(expr) cout << "[TEST LOG] : " << expr << endl

/* NOTE: these names MUST match those in the XML */
static string test1_data1_name = "name for uint16";
static string test1_data2_name = "floating point";
static string test1_data3_name = "just 4 bytes";

static string test1_data1_type = "uint16";
static string test1_data2_type = "float32";
static string test1_data3_type = "int8";

static unsigned long test1_data1_len = 2;
static unsigned long test1_data2_len = 1;
static unsigned long test1_data3_len = 4;

static uint16_t test1_data1_data[] = {0xABCD, 0x0123};	 	// big-endian
static float 	test1_data2_data[] = {1.256 /* = 0x3FA0C49C */};// little-endian
static int8_t	test1_data3_data[] = {(int8_t)0xAA, (int8_t)0xBB,
				      (int8_t)0xCC, (int8_t)0xDD};	
				      // little-endian

static void check_test1(const std::vector<const Data*> &data)
{
	#define TEST_ASSERT_FAIL(expr, error_msg) \
	do {\
		if (!(expr)) {\
			TEST_LOG("TEST_ASSERT FAIULURE :"#expr);\
			TEST_LOG("                     :" << error_msg);\
			exit(EXIT_FAILURE);\
		}\
	} while(0)
	
	#define TEST_DATA_COMMONS(d_ptr, type, len, name)	\
	do { \
	TEST_ASSERT_FAIL(d_ptr != NULL, "Data pointer "#d_ptr" was NULL"); \
	TEST_ASSERT_FAIL(d_ptr->get_type() == type, "Bad data type for "#d_ptr\
			 ": exptected type=" << type << " but was " << \
			 d_ptr->get_type());\
	TEST_ASSERT_FAIL(d_ptr->get_len() == len, "Bad data length for "#d_ptr\
			 ": exptected len=" << len << " but was " << \
			 d_ptr->get_len());\
	TEST_ASSERT_FAIL(d_ptr->get_name() == name, "Bad data name for "#d_ptr\
			 ": exptected name=" << name << " but was " << \
			 d_ptr->get_name());\
	} while (0)
	

	TEST_ASSERT_FAIL(data.size() == 3, "Unexpected data vector size: "
			 "expected=3 but was " << data.size());

	const Data* data1 = data[0];
	const Data* data2 = data[1];
	const Data* data3 = data[2];
	
	TEST_DATA_COMMONS(data1, test1_data1_type, test1_data1_len, 
			  test1_data1_name);
	TEST_DATA_COMMONS(data2, test1_data2_type, test1_data2_len, 
			  test1_data2_name);
	TEST_DATA_COMMONS(data3, test1_data3_type, test1_data3_len, 
			  test1_data3_name);

	const DataUInt16 *d1 = dynamic_cast<const DataUInt16*>(data1);
	const DataFloat32 *d2 = dynamic_cast<const DataFloat32*>(data2);
	const DataInt8 *d3 = dynamic_cast<const DataInt8*>(data3);

	TEST_ASSERT_FAIL(d1!=0,"dynamic_cast<const DataUInt16*>(data1) error");
	TEST_ASSERT_FAIL(d2!=0,"dynamic_cast<const DataFloat32*>(data2) error");
	TEST_ASSERT_FAIL(d3!=0,"dynamic_cast<const DataUInt16*>(data3) error");


	#define TEST_DATA_VALUE(d, v)\
	TEST_ASSERT_FAIL(d == v, "Bad data: Expected " << v << " got " << d << \
			 " - hexadecimal: " << hex << v << " != " << hex << d)

	TEST_DATA_VALUE(d1->data[0], test1_data1_data[0]);
	TEST_DATA_VALUE(d1->data[1], test1_data1_data[1]);
	
	TEST_DATA_VALUE(d2->data[0], test1_data2_data[0]);

	TEST_DATA_VALUE(d3->data[0], test1_data3_data[0]);
	TEST_DATA_VALUE(d3->data[1], test1_data3_data[1]);
	TEST_DATA_VALUE(d3->data[2], test1_data3_data[2]);
	TEST_DATA_VALUE(d3->data[3], test1_data3_data[3]);
}

int main(int argc, char* argv[])
{
	int test_result = 0;
	argc--;
	if (argc < 1) {
		TEST_LOG("The XML file should be bassed as unique argument.");
		TEST_LOG("Usage: " << argv[0] << "<XML_FILE>");
		exit(EXIT_FAILURE);
	}
	const char *xml_filename = argv[1];

	vector<const Data*> data;

	TEST_LOG(" -> Stream Integration Test: __FileStream__");

	try { // Scope operator

	TEST_LOG("Creating DuckIO from XML '" << xml_filename << "'");
	DuckIO reader(xml_filename);

	TEST_LOG("Start reading loop");
	unsigned long int loop = 0;
	while (loop++ < TEST_MAX_LOOP) {
		TEST_LOG("READING LOOP [" << loop << "]: BEGIN  " <<
			 "---------------------------------------------------");
		TEST_LOG("read()");
		reader.read();
		TEST_LOG("get_const_data_vector()");
		std::vector<const Data*> data = reader.get_const_data_vector();
		TEST_LOG("Check read data");
		check_test1(data);
		TEST_LOG("READING LOOP [" << loop << "]: END    " <<
			 "---------------------------------------------------");
	}
	TEST_LOG("End reading loop");

	// DuckIO scope ends here: destructors called
	} catch (StreamException e) {
		TEST_LOG("StreamException Caught: " << e.what());
		if (e.get_code() != StreamException::EOF_REACHED)
			test_result++;
	} catch (DuckException e) {
		TEST_LOG("DuckException Caught: " << e.what());
		test_result++;
	} catch (exception e_gen) {
		TEST_LOG("std::exception Caught: " << e_gen.what());
		test_result++;
	}

	return test_result;
}
