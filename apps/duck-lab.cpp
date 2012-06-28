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
#include <iostream>
#include <string>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <argp.h>

#include "DuckIO.h"
#include "DuckStorage.h"
#include "DuckIOException.h"
#include "DuckStorageException.h"

#ifdef DUCK_USE_NAMESPACE
using namespace duck;
#endif
using namespace std;

static bool duck_running_loop;

static struct duck_args_t {
	string xml_file_name;
	unsigned long long nevents;
	bool silent;
} duck_args;

/******************************************************************************/
/*                              Utility Functions                             */
/******************************************************************************/
#define SYSTEM_INFO(msg) 				\
do {							\
	if (!duck_args.silent)				\
		cout << "[duck-lab] : " << msg << endl;	\
} while (0)

#define SYSTEM_INFO2(msg) 				\
do {							\
	if (!duck_args.silent)				\
		cout << "[duck-lab] : " << msg;		\
} while (0)

#define SYSTEM_ERROR(msg) 				\
do {							\
	cout << "[duck-lab] : ERROR : " << msg << 	\
		endl << "Exiting" << endl << endl;	\
	exit(EXIT_FAILURE);				\
} while (0) 

#define SYSTEM_WARNING(msg) 					\
do {								\
	cout << "[duck-lab] : WARNING : " << msg << endl;	\
} while (0) 

#define SYSTEM_NEWLINE()			        \
do {							\
	cout << endl;					\
} while (0) 

static void print_progress(unsigned long long loop_count) 
{
	static unsigned int cnt = 0;
	static bool do_init = true;

	if (do_init) {
		cout << "    ";
		do_init = false;
	}
	if (loop_count % 10) 
		return;
	cout << "\b\b\b";
	switch (cnt) {
	case 0 : cout << "-"; break;
	case 1 : cout << "\\"; break;
	case 2 : cout << "|"; break;
	case 3 : cout << "/"; break;
	}
	cout << "  ";
	cout.flush();
	cnt = (cnt + 1) % 4;
}

/******************************************************************************/
/*                             Argument Parsing                               */
/******************************************************************************/

// Documentation variables
const char *argp_program_version = 	"duck-lab 1.0";
const char *argp_program_bug_address = 	"<c.salvadori@sssup.it> "
					"<c.nastasi@sssup.it>";
static const char *doc = "duck-lab - Data AcQuisition System Framework "
			 "\vTODO write a detailed description\n"
			 "Start data acquisition using the CONFIG xml file.";
static const char *args_doc = "CONFIG";

// Options defition
static struct argp_option options[] = {
	{"quiet",    'q', 0,      0,  "Don't produce any output" },
	{"silent",   's', 0,      OPTION_ALIAS },
	//{"verbose",  'v', 0,      0,  "Produce verbose output" },
	{"numevent",   'n', "NEVT", OPTION_ARG_OPTIONAL,
	"Number of events to acquire. Default is infinite (NEVT=0)." },
	{0}
};

// Single option parsing
static error_t parse_opt(int key, char *arg, struct argp_state *state)
{
	/* Get the input argument from argp_parse, which we
	   know is a pointer to our arguments structure. */
	struct duck_args_t *out_args = (struct duck_args_t *) state->input;
	
	switch (key) {
	case 'q': 
	case 's':
		out_args->silent = true;
		break;
	//case 'v':
	//	out_args->verbose = 1;
	//	break;
	case 'n':
		if (!arg) {
			SYSTEM_WARNING("Empty argument for -n option");
			return EINVAL;
		}
		char *res;
		out_args->nevents = strtoull(arg, &res, 10);
		if (res[0] != '\0') {
			SYSTEM_WARNING("Bad argument for -n option");
			return EINVAL;
		}
		break;
	case ARGP_KEY_ARG:
		if (state->arg_num >= 1)	// Too many arguments
	        	argp_usage(state);
		out_args->xml_file_name = arg;
		break;
	case ARGP_KEY_END:
		if (state->arg_num < 1) 	// Not enough arguments
			argp_usage(state);
		break;
	default:
		return ARGP_ERR_UNKNOWN;
	}
	return 0;
}

// Define the argp structure used for argument parsing
static struct argp arg_parse_info = {options, parse_opt, args_doc, doc};

/******************************************************************************/
/*                           Posix Signals Handling                           */
/******************************************************************************/
static void close_handler(int signo)
{	
	SYSTEM_INFO(endl << "Catching SIGINT. Force stopping duck-lab ...");
	duck_running_loop = false;
}

static void init_handler(void)
{
	struct sigaction nsig, vsig;
        nsig.sa_handler = close_handler;
        nsig.sa_flags = 0;
        sigemptyset(&(nsig.sa_mask));

        if (sigaction(SIGINT, &nsig, &vsig) == -1) 
        	SYSTEM_ERROR("SIGINT hangler installation error");
}

/******************************************************************************/
/*                                    MAIN                                    */
/******************************************************************************/
int main(int argc, char* argv[])
{
	// --------------------- Install signal handler --------------------- //
	init_handler();

	// ----------------------- Argument parsing ------------------------- //
	// Default values
	duck_args.xml_file_name = "";
	duck_args.nevents = 0; 
	duck_args.silent = false;
	// Call opt parser
	int err = argp_parse(&arg_parse_info, argc, argv, 0, 0, &duck_args);
	if (err != 0)
		SYSTEM_ERROR("Argument parsing failure : " << strerror(err));
	// Check values 
	if (duck_args.xml_file_name == "")
		SYSTEM_ERROR("Input XML filename is empty");
	SYSTEM_INFO("Input xml file = '" << duck_args.xml_file_name << "'");
	SYSTEM_INFO("Num of events = " << duck_args.nevents << "(0=infinity)");

	{ // SCOPE OPERATOR

	// --------------------- duck-lab init ---------------------- //
	// Create data acquisition manager
	DuckIO reader(duck_args.xml_file_name); 
	// Create data storager
	DuckStorage storager(duck_args.xml_file_name, 
			     reader.get_const_data_vector());

	reader.set_up_buffers(duck_args.nevents);	
	storager.set_up_buffers(&reader, duck_args.nevents);

	boost::thread *t1;
	boost::thread *t2;
	std::vector<boost::thread*> *t3;


	t1 = reader.launch_streaming();
	t2 = reader.launch_formatting();
	t3 = storager.launch();
	
	t1->join();
	t2->join();
	for( std::vector<boost::thread*>::iterator it = t3->begin();
				it != t3->end(); it ++)
		(*it)->join();

        exit(EXIT_SUCCESS);
	
	// --------------------- duck-lab loop ---------------------- //
	unsigned long long loop_count = 0;
	unsigned long long max_loop_count = duck_args.nevents;
	duck_running_loop = true;
	SYSTEM_INFO2("Acquiring data");
	while (duck_running_loop) {
		try {
			reader.read();
			storager.store();
		// TODO: Add StreamException and check EOF condition?
		} catch (DuckIOException e_io) {
			SYSTEM_NEWLINE();
			SYSTEM_WARNING("DuckIOException Caught: " << 
				       e_io.what());
			break;
		} catch (DuckStorageException e_storage) {
			SYSTEM_NEWLINE();
			SYSTEM_WARNING("DuckStorageException Caught: " << 
				       e_storage.what());
			break;
		} catch (DuckException e_duck_gen) {
			SYSTEM_NEWLINE();
			SYSTEM_WARNING("DuckException Caught: " << 
				       e_duck_gen.what());
			break;
		} catch (exception e_gen) {
			SYSTEM_NEWLINE();
			SYSTEM_WARNING("std::exception Caught: " << 
				       e_gen.what());
			break;
		}
		loop_count++;
		if (max_loop_count)
			if (loop_count >= max_loop_count)
				duck_running_loop = false;
		if (!duck_args.silent)
			print_progress(loop_count);
	}

	// -------------------- duck-lab closing -------------------- //
	SYSTEM_INFO("Stopping duck-lab");
	SYSTEM_INFO("Total number of acquired event = " << loop_count);
	SYSTEM_INFO("Exiting");

	} // END OF SCOPE OPERATOR: calls reader and storager destructors

	SYSTEM_INFO("Done");
	cout << endl;
	
        exit(EXIT_SUCCESS);
}
