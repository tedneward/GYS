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
    \brief Source file for stdGYS IO functions.
*/

#include "gys_io_func.h"

#include "gys_int_type.h"
#include "gys_double_type.h"
#include "gys_char_type.h"
#include "gys_string_type.h"

#include <conio.h>

GYS_ret_val exec_print(GYS* script)
{
	script->exam("(");
	
	cout << (string)script->get_val(gys_string_type);
				
	script->exam(")");	
	
	return GYS_ret_val();
}	

GYS_ret_val exec_println(GYS* script)
{
	script->exam("(");
	
	cout << (string)script->get_val(gys_string_type) << '\n';
				
	script->exam(")");	
	
	return GYS_ret_val();
}	

GYS_ret_val exec_putch(GYS* script) 
{
	script->exam("("); 

	_putch((char)script->get_val(gys_char_type));

	script->exam(")");

	return GYS_ret_val();
}

GYS_ret_val exec_getch(GYS* script)
{
	script->exam("("); 

	// waiting for entering the key
	char ch=_getch();

	script->exam(")");
	
	return ch;
}

GYS_ret_val exec_kbhit(GYS* script)
{
	script->exam("("); 

	script->exam(")");
	
	return (int)_kbhit();
}

/*
GYS_ret_val exec_match(GYS* script)
{
	script->exam("("); 

	string str=script->get_val(gys_string_type).get_str_val();

	script->exam(",");

	string pattern=script->get_val(gys_string_type).get_str_val();

	script->exam(")");
	
	return (bool)match((char*)str.c_str(), (char*)pattern.c_str());
}*/

GYS_ret_val exec_toupper(GYS* s)
{
	s->exam("("); 

	char ch=s->get_val(gys_char_type);

	s->exam(")"); 

	return (char)toupper(ch);
}

GYS_ret_val exec_tolower(GYS* s)
{
	s->exam("("); 

	char ch=s->get_val(gys_char_type);

	s->exam(")"); 

	return (char)tolower(ch);
}

GYS_ret_val exec_readb(GYS* script) // read bool
{
	script->exam("("); 

	bool result;
	cin>>result;

	script->exam(")");

	return result;
}

GYS_ret_val exec_readd(GYS* script) // read double
{
	script->exam("("); 

	// enter the double
	double result;
	cin>>result;

	script->exam(")");

	return result; // double
}

GYS_ret_val exec_readi(GYS* script) // read int
{
	script->exam("("); 

	int result;
	cin>>result;

	script->exam(")");

	return result; // int
}

GYS_ret_val exec_reads(GYS* script) // read string
{
	script->exam("("); 

	// enter the string
	string result="";
	cin>>result;

	script->exam(")");

	return result; // string
}

GYS_ret_val exec_readc(GYS* script) // read char
{
	script->exam("("); 

	char c=' ';
	cin>>c;

	script->exam(")");

	return c; // char
}

void add_io_funcs(GYS* script)
{
	script->add_gys_func("print",   exec_print); 
	script->add_gys_func("println", exec_println); 
	
	script->add_gys_func("putch", exec_putch); 
	
	script->add_gys_func("toupper", exec_toupper); 
	script->add_gys_func("tolower", exec_tolower); 

	script->add_gys_func("readb", exec_readb); 
	script->add_gys_func("readd", exec_readd); 
	script->add_gys_func("readi", exec_readi); 
	script->add_gys_func("reads", exec_reads); 
	script->add_gys_func("readc", exec_readc);

	script->add_gys_func("getch", exec_getch);
	script->add_gys_func("kbhit", exec_kbhit);

	//script->add_gys_func("match", exec_match);
}