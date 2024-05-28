/******************************************************************************
 *	 GYS - library for developing interpreters of scripting languages,        *
 *                                                                            *
 *	 Copyright (C) 2004  Govoruschenko Yuri Y.                                *
 *	 E-mail: ygy@users.sourceforge.net                                        *
 *	 Web: gys.sourceforge.net                                                 *
 *                                                                            *
 *   This library is free software; you can redistribute it and/or            *
 *   modify it under the terms of the GNU Lesser General Public               *
 *   License as published by the Free Software Foundation; either             *
 *   version 2.1 of the License, or (at your option) any later version.       *
 *                                                                            *
 *   This library is distributed in the hope that it will be useful,          *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of           *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU        *
 *   Lesser General Public License for more details.                          *
 *                                                                            *
 *   You should have received a copy of the GNU Lesser General Public         *
 *   License along with this library; if not, write to the Free Software      *
 *   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA *
 ******************************************************************************/

/*! \file
    \brief Source file for stdGYS general functions.
*/

#include "gys_general_func.h"

#include "gys_bool_type.h"
#include "gys_int_type.h"
#include "gys_string_type.h"

#include <fstream>
#include <sstream>

GYS_ret_val exec_import(GYS* s)
{
	s->exam("(");
	
	string filename=s->get_val(gys_string_type);
		
	GYS source_script(filename);
	ifstream in(filename.c_str());

	if(!in)
		s->error("import(): can't open file: ", filename);
	else
	{
		source_script.set_input(&in);		         
	
		s->copy_params_to_sub(&source_script);

		source_script.interp(); // run
	}

	s->exam(")");

	source_script.copy_vars_to_script(s); // importing variables

	return GYS_ret_val(); 
}

GYS_ret_val exec_run_script(GYS* s)
{
	s->exam("(");
	
	string filename=s->get_val(gys_string_type);
		
	GYS source_script(filename);
	ifstream in(filename.c_str());

	if(!in)
		s->error("run_script(): can't open file: ", filename);
	else
	{
		source_script.set_input(&in);		         
	
		s->copy_params_to_sub(&source_script);

		source_script.interp(); // run
	}

	s->exam(")");

	return GYS_ret_val(); 
}

GYS_ret_val exec_run(GYS* s)
{
	s->exam("(");
	
	string code=s->get_val(gys_string_type);

	s->exam(")");

	GYS source_script("script");		

	istringstream in(code);
	source_script.set_input(&in);		         
	
	s->copy_params_to_sub(&source_script);

	source_script.disable(GYS_ESC); // no processing of escape sequence
	source_script.interp(); // run
	
	return GYS_ret_val(); 
}

GYS_ret_val exec_defined(GYS* s)
{
	s->exam("(");

	s->exam(GYS_NAME);
	string name=s->get_tok_str();

	s->exam(")");

	return (bool)(s->is_func(name) || s->is_typename(name) || s->is_var(name));
}

GYS_ret_val exec_wait(GYS* script)
{
	script->exam("(");

	int val=script->get_val(gys_int_type);
				
	if(val>=0) // all right
		_sleep(val);
	else           // wrong argument
		script->warning("wait() does nothing: argument must be >=0.");
			
	script->exam(")");

	return GYS_ret_val();
}

GYS_ret_val exec_system(GYS* script)
{
	script->exam("(");

	string cmd=script->get_val(gys_string_type);
	int result=system(cmd.c_str());

	script->exam(")");

	return result; // if 0 - OK
}

void add_general_funcs(GYS* script)
{
	script->add_gys_func("import",     exec_import); 
	script->add_gys_func("run_script", exec_run_script); 
	script->add_gys_func("run",        exec_run);	

	script->add_gys_func("defined", exec_defined);
	script->add_gys_func("wait",    exec_wait);
	script->add_gys_func("system",  exec_system);
}
