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

#include "DataUInt16.h"
#include "DataFloat32.h"
#include "DataInt8.h"

#include <iostream>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/wait.h>

#ifdef DUCK_USE_NAMESPACE
using namespace duck;
#endif
using namespace std;

#define TEST_INPUT_XML_FILE "SerialStream_IntegrationTest_ACTUAL.xml"

#define TEST_MAX_LOOP 30
#define TEST_LOG(expr) cout << "[TEST LOG] : " << expr << endl

static char *template_xml_filename;
static int pseudo_terminal_master;
static int child_pid; 
static string serial_port_name;

/* NOTE: these names MUST match those in the XML */
static string test1_data1_name = "name for uint16";
static string test1_data2_name = "floating point";
static string test1_data3_name = "just 4 bytes";

static string test1_data1_type = "uint16";
static string test1_data2_type = "float32";
static string test1_data3_type = "int8";

static const unsigned test1_data_samples = 20;
static const unsigned long test1_data1_len = 2;
static const unsigned long test1_data2_len = 1;
static const unsigned long test1_data3_len = 4;
static uint16_t test1_data1_data[test1_data_samples][test1_data1_len];
static float 	test1_data2_data[test1_data_samples][test1_data2_len];
static int8_t	test1_data3_data[test1_data_samples][test1_data3_len];


static void setup_test1(void)
{
	srandom(time(NULL));
	for (unsigned i = 0; i < test1_data_samples; i++) {
		for (unsigned j = 0; j < test1_data1_len; j++)
			test1_data1_data[i][j] = (uint16_t)rand();
		for (unsigned j = 0; j < test1_data2_len; j++)
			test1_data2_data[i][j] = ((float)rand()) / RAND_MAX;
		for (unsigned j = 0; j < test1_data3_len; j++)
			test1_data3_data[i][j] = (int8_t) (rand() & 0xFF);
	}
}

static void generate_data_test1(uint8_t*& buff, unsigned long& len)
{
	static const unsigned long s1 = test1_data1_len * sizeof(uint16_t);
	static const unsigned long s2 = test1_data2_len * sizeof(float);
	static const unsigned long s3 = test1_data3_len * sizeof(int8_t);
	static const unsigned long s_tot = s1 + s2 + s3; 
	static uint8_t buffer[s_tot];
	static unsigned idx = 0;

	unsigned long k = 0;

	for (unsigned j = 0; j < test1_data1_len; j++) {
		memcpy(buffer + k, &(test1_data1_data[idx][j]), 
		       sizeof(uint16_t));
		k += sizeof(uint16_t);
	}
	for (unsigned j = 0; j < test1_data2_len; j++) {
		memcpy(buffer + k, &(test1_data2_data[idx][j]), 
		       sizeof(float));
		k += sizeof(float);
	}
	for (unsigned j = 0; j < test1_data3_len; j++) {
		memcpy(buffer + k, &(test1_data3_data[idx][j]), 
		       sizeof(int8_t));
		k += sizeof(int8_t);
	}

	buff = buffer;
	len = s_tot;

	idx = (idx + 1) % test1_data_samples;
}

static void check_test1(const std::vector<const Data*> &data)
{
	static unsigned idx = 0;

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

	TEST_DATA_VALUE(d1->data[0], test1_data1_data[idx][0]);
	TEST_DATA_VALUE(d1->data[1], test1_data1_data[idx][1]);
	
	TEST_DATA_VALUE(d2->data[0], test1_data2_data[idx][0]);

	TEST_DATA_VALUE(d3->data[0], test1_data3_data[idx][0]);
	TEST_DATA_VALUE(d3->data[1], test1_data3_data[idx][1]);
	TEST_DATA_VALUE(d3->data[2], test1_data3_data[idx][2]);
	TEST_DATA_VALUE(d3->data[3], test1_data3_data[idx][3]);
	
	idx = (idx + 1) % test1_data_samples;
}

static void init_pseudo_terminal(void)
{
	pseudo_terminal_master = posix_openpt(O_RDWR); 
	if (pseudo_terminal_master < 0) { 
		TEST_LOG("Error " << errno << " on posix_openpt()");
		perror("Exiting on error: ");
		exit(EXIT_FAILURE);
	} 
	if (grantpt(pseudo_terminal_master) != 0) { 
		TEST_LOG("Error " << errno << " on grantpt()");
		perror("Exiting on error: ");
		exit(EXIT_FAILURE);
	} 
	if (unlockpt(pseudo_terminal_master) != 0) { 
		TEST_LOG("Error " << errno << " on unlockpt()");
		perror("Exiting on error: ");
		exit(EXIT_FAILURE);
	} 

	serial_port_name =  ptsname(pseudo_terminal_master);
	TEST_LOG("Serial Port (slave pseudo terminal) = " << serial_port_name);
}

static void fill_xml_properly(void)
{
	string host_endianess;
	int test = 1;
	if (*(char*)&test == 1)
		host_endianess = "little-endian"; 
	else
		host_endianess = "big-endian"; 

	string escaped_name = "";
	for (unsigned i = 0; i < serial_port_name.size(); i++) {
		if (serial_port_name[i] == '/')
			escaped_name += '\\';
		escaped_name += serial_port_name[i];
	}

	string cmd;
	cmd += "sed -e 's/REPLACEBLE_TEST_ENDIANESS/";
	cmd += host_endianess;
	cmd += "/g' ";
	cmd += template_xml_filename;
	cmd += " > " TEST_INPUT_XML_FILE "; ";
	
	cmd += "sed -e 's/REPLACEBLE_TEST_SERIALNAME/";
	cmd += escaped_name;
	cmd += "/g' " TEST_INPUT_XML_FILE " > tmp_file; ";
	cmd += "mv tmp_file " TEST_INPUT_XML_FILE;

	TEST_LOG("Creating actual XML with: " << cmd);
	system(cmd.c_str());
}

static int father_process(void) 
{
	uint8_t *buf = NULL;
	unsigned long len = 0;
	unsigned long loop = 0;

	TEST_LOG("FATHER [fake serial end-point]: START writing loop");
	while (loop++ < TEST_MAX_LOOP) {
		generate_data_test1(buf, len);

		if (!buf) {
			TEST_LOG("ERROR: got NULL buffer from "
				 "generate_data_test1()");
			exit(EXIT_FAILURE);
		}
		int w = write(pseudo_terminal_master, buf, len); 
		TEST_LOG("FATHER [fake serial end-point]: LOOP=" <<
			 loop << " Written " << w << " bytes");
		usleep(250000);
	}
	TEST_LOG("FATHER [fake serial end-point]: END writing loop");
	int child_test_result;
	waitpid(child_pid, &child_test_result, 0);
	if (WIFEXITED(child_test_result))
		return WEXITSTATUS(child_test_result);
	return EXIT_FAILURE;
}

static int child_process(void)
{
	int test_result = 0;
	try { // Scope operator

	TEST_LOG("Creating DuckIO from XML '" TEST_INPUT_XML_FILE "'");
	DuckIO reader(TEST_INPUT_XML_FILE);

	TEST_LOG("Start reading loop");
	unsigned long int loop = 0;
	while (loop++ < TEST_MAX_LOOP) {
		usleep(250000);
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
	} catch (DuckException e) {
		TEST_LOG("DuckException Caught: " << e.what());
		test_result++;
	} catch (exception e_gen) {
		TEST_LOG("std::exception Caught: " << e_gen.what());
		test_result++;
	}
	return test_result;
}

int main(int argc, char* argv[])
{
	argc--;
	if (argc < 1) {
		TEST_LOG("The template XML file should be bassed"
			 " as unique argument.");
		TEST_LOG("Usage: " << argv[0] << " <TEMPLATE_XML_FILE>");
		exit(EXIT_FAILURE);
	}
	template_xml_filename = argv[1];

	vector<const Data*> data;

	TEST_LOG(" -> Stream Integration Test: __SerialStream__");

	TEST_LOG("Setup test data");
	setup_test1();

	TEST_LOG("Setup PTY");
	init_pseudo_terminal();
	
	TEST_LOG("Write PTY slave name to XML");
	fill_xml_properly();

	TEST_LOG("FORK");
	TEST_LOG("father [loopback serial] -> writes to PTY master");
	TEST_LOG("child  [duck-lab w/ serial]  -> reads from PTY slave");

	child_pid = fork();
	if (child_pid < 0) {
		TEST_LOG("Error " << errno << " on fork()");
		perror("Exiting on error: ");
		exit(EXIT_FAILURE);
	}
	
	int retv;
	if (child_pid) {
		// Father: 
		retv = father_process();
	} else {
		// Child: Close the master side of the PTY 
		close(pseudo_terminal_master); 
		retv = child_process();
	} 
	return retv;
}
