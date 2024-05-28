#include <iostream> // C++ io
#include <fstream>  // ifstream

#include "gys.h" // GYS interface
#include "stdGYS\gys_type.h" // stdGYS types
#include "stdGYS\gys_func.h" // stdGYS functions

int main(int argc, char* argv[])
{
	// create new script interpreter and set its name=input filename
	const char* filename="main.gys";
	GYS s(filename);  
    	
	// load the file we want to interpret
	ifstream fin(filename);

	if(!fin) // error
	{
		cerr<<"error: can't open the input file: "<<filename<<endl;
		return 1;
	}

	// set GYS input
	s.set_input(&fin);           		

	// init interpreter's error and warning handlers (here we use std handlers)
	//s.set_warning_func(std_gys_warning_callback); // I've commented this to get clear output 
	s.set_error_func(std_gys_error_callback);
	s.set_fatal_error_func(std_gys_fatal_error_callback);

	// attaching stdGYS
	add_stdgys_types(&s); // adding std types, operators and converters
	add_stdgys_funcs(&s); // adding std built-in functions

	s.interp(); // start interpreting
		
	return 0;
}