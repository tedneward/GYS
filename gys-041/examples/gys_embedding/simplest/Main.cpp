#include <iostream> // C++ io
#include <fstream>  // ifstream

#include "gys.h" // GYS interface

int main(int argc, char* argv[])
{
	// create new script interpreter and set its name=input filename
	const char* filename="foo.gys";
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
	s.set_warning_func(std_gys_warning_callback);
	s.set_error_func(std_gys_error_callback);
	s.set_fatal_error_func(std_gys_fatal_error_callback);

	s.interp(); // start interpreting
		
	return 0;
}