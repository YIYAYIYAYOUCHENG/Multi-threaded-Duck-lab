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

#include "DuckConfig.h"

using namespace std;

static char *application_command;

static void print_help(void)
{
	cout << "duck-lab Lib: configuration utility" << endl;
	cout << "Usage: " << application_command << 
		" [-h | --help] [--cflags] [--ldflags]" << endl;
	cout << endl;
	cout << "       -h | --help : Print this help" << endl;
	cout << "       --cflags    : Get compiler flags" << endl;
	cout << "       --ldflags   : Get linker flags" << endl;
	cout << endl;
}

static void exit_error(const char *msg)
{
	cout << endl;
	cout << "Error: " << msg << endl; 
	cout << endl;
	print_help();
	exit(EXIT_FAILURE);
}

static void exit_help(void)
{
	cout << endl;
	print_help();
	exit(EXIT_SUCCESS);
}

/******************************************************************************/
/*                                    MAIN                                    */
/******************************************************************************/
int main(int argc, char* argv[])
{
	bool print_cflags = false;
	bool print_ldflags = false;

	application_command = argv[0];
	if (argc < 2)
		exit_error("Not enough arguments to execute command");
	
	for (int id = 1; id < argc; id++) { 
		string s = argv[id];
		if (s == "-h" || s == "--help") 
			exit_help();
		else if (s == "--cflags")
			print_cflags = true;
		else if (s == "--ldflags")
			print_ldflags = true;
		else
			exit_error("Invalid argument(s)");
	}
	
	if (print_cflags)
		cout << DUCK_CONFIG_DISTRIB_CFLAGS << " " ;
	if (print_ldflags)
		cout << DUCK_CONFIG_DISTRIB_LDFLAGS << " " ;
	cout << endl;
        exit(EXIT_SUCCESS);
}
