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

/*! 
  \file
  \brief Source file for stdGYS char type.

*/

#include "gys_char_type.h"
#include "gys_type_stuff.cpp" // VC7 doesn't support export keyword with templates thus we include cpp-file

#include "gys_string_type.h"
#include "gys_int_type.h"

class GYS_char_user_val: public GYS_user_val
{
private:
	char val;

public:
	GYS_char_user_val(char c) : val(c) { }

	string get_type_name() const {
		return gys_char_type;
	}		

	void* get_user_val() {
		return &val;
	}
};

GYS_ret_val& store(GYS_ret_val& val, char char_user_val)
{
	val.set_user_val( new GYS_char_user_val(char_user_val) );
	return val;
}

// gets raw char data
GYS_ret_val get_char_val(GYS* script)
{
	script->get_token();

	if(script->get_tok_type()==GYS_CHAR)
	{
		return (char)script->get_tok_str()[0];
	}
	else
	{
		script->error("'"+(string)gys_char_type+"' getting: char-val expected: ", script->get_tok_str());
		return GYS_ret_val();
	}	
}

// char+char
GYS_ret_val op_add_char(GYS* script, GYS_ret_val l, GYS_ret_val r)
{
	string s="";

	s+=(char)l;
	s+=(char)r;

	return s;
}

// char*int
GYS_ret_val op_mul_char(GYS* script, GYS_ret_val l, GYS_ret_val r)
{
	int n=r;

	if(n<=0)
		script->warning("'"+l.get_type_name()+"' * '"+r.get_type_name()+"' : right operand is <=0");

	string s="";
	for(;n>0;n--)
		s+=(char)l;
	
	return s;
}

void add_char_type(GYS* s)
{
	s->add_gys_type(gys_char_type, get_char_val); 
	s->add_gys_func(gys_char_type, exec_type_name);

	s->add_binary_op("=", gys_char_type, gys_char_type, op_assign);

	s->add_binary_op("+", gys_char_type, gys_char_type, op_add_char);
	s->add_binary_op("*", gys_char_type, gys_int_type, op_mul_char);

	s->add_binary_op("==", gys_char_type, gys_char_type, op_equal<char, char, bool>);
	s->add_binary_op("!=", gys_char_type, gys_char_type, op_not_equal<char, char, bool>);
	s->add_binary_op(">",  gys_char_type, gys_char_type, op_greater<char, char, bool>);
	s->add_binary_op("<",  gys_char_type, gys_char_type, op_less<char, char, bool>);
	s->add_binary_op(">=", gys_char_type, gys_char_type, op_greater_or_equal<char, char, bool>);
	s->add_binary_op("<=", gys_char_type, gys_char_type, op_less_or_equal<char, char, bool>);
}