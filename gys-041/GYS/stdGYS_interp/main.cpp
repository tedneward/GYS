#include <iostream> // C++ io
#include <fstream> // files

#include <time.h>

#include "..\gys.h"

// stdGYS
#include "..\stdGYS\gys_type.h"
#include "..\stdGYS\gys_func.h"

#include "..\stdGYS\gys_vector_type.h"
#include "..\stdGYS\gys_string_type.h"

void usage();

int main(int argc, char* argv[])
{
	if(argc<2) // check arg num
	{
		usage();
		return 1;
	}

	bool nw=false, ne=false, nf=false;

	// read keys
	for(int i=1; i<argc; i++)
	{
		if( !strcmp(argv[i], "-nw") )      // no warning messages
			nw=true;	
		else if( !strcmp(argv[i], "-ne") ) // no error messages
			ne=true;
		else if( !strcmp(argv[i], "-nf") ) // no fatal error messages
			nf=true;
		else
			break;
	}

	if(i==argc) // error
	{
		cerr<<"Interpreter error: no input file\n";
		return 1;
	}

	// create new script interpreter and set its name
	const char* filename=argv[i];
	GYS s(filename);  
    	
	//load the file we want to interpret
	ifstream fin(filename);

	if(!fin) // error
	{
		cerr<<"Interpreter error: can't open the input file: "<<argv[i]<<endl;
		return 1;
	}

	s.set_input(&fin);           		

	//init interpreter
	if(!nw)	s.set_warning_func(std_gys_warning_callback);
	if(!ne)	s.set_error_func(std_gys_error_callback);
	if(!nf)	s.set_fatal_error_func(std_gys_fatal_error_callback);

	add_stdgys_types(&s); // add types and ops
	add_stdgys_funcs(&s); // add built-in functions

	// read script name and all its arguments
	GYS_vector_val argv_vec(gys_string_type);
	for(; i<argc; i++) 
		argv_vec.push_back((string)argv[i]);
	
	// adding ARGV global var
	s.add_global_var("ARGV", argv_vec);

	// adding VERSION global const
	s.add_global_var("@VERSION", "0.4.0");

	s.interp(); //start interpreting
	
	cout<<endl<<s.get_name()<<" - "<<s.get_error_num()<<" error(s), "<<s.get_warning_num()<<" warning(s)"
																		<<endl<<endl;
	return 0;
}

void usage()
{
	cerr<<"Usage:   gys [-nw -ne -nf] filename [arguments]"<<endl<<endl;
	cerr<<"Options: -nw: no warning messages"<<endl;
	cerr<<"         -ne: no error messages"<<endl;
	cerr<<"         -nf: no fatal error messages"<<endl<<endl;
}