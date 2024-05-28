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
  \brief Source file for stdGYS string type.
*/

#include "gys_string_type.h"
#include "gys_type_stuff.cpp" // VC7 doesn't support export keyword with templates thus we include cpp-file

#include "gys_int_type.h"
#include "gys_char_type.h"
/*
class GYS_string_user_val: public GYS_user_val
{
private:
	string val;

public:
	GYS_string_user_val(string s) : val(s) { }

	string get_type_name() const {
		return gys_string_type;
	}		

	void* get_user_val() {
		return &val;
	}
};*/

GYS_ret_val& store(GYS_ret_val& val, string string_user_val)
{
	val.set_user_val( new GYS_string_user_val(string_user_val) );
	return val;
}

// gets raw string data
GYS_ret_val get_string_val(GYS* script)
{
	script->get_token();

	if(script->get_tok_type()==GYS_STRING ||
	   script->get_tok_type()==GYS_CHAR ||
	   script->get_tok_type()==GYS_DOUBLE)
	{
		return (string)script->get_tok_str();
	}
	else
	{
		script->error("'"+(string)gys_string_type+"' getting: string-val expected: ", script->get_tok_str());
		return GYS_ret_val();
	}	
}

// string*int=string
GYS_ret_val op_mul_str(GYS* script, GYS_ret_val l, GYS_ret_val r)
{
	int n=r;

	if(n<=0)
		script->warning("'"+l.get_type_name()+"' * '"+r.get_type_name()+"' : right operand is <=0");

	string s="";
	for(;n>0;n--)
		s+=(string)l;
	
	return s;
}

// string*=int
GYS_ret_val op_mul_assign_str(GYS* script, GYS_ret_val l, GYS_ret_val r)
{
	if(!l.is_lvalue())
	{
		script->error("'"+l.get_type_name()+"' *= '"+r.get_type_name()+"' : left operand must be lvalue");
		return l;
	}

	int n=r;

	if(n<=0)
		script->warning("'"+l.get_type_name()+"' *= '"+r.get_type_name()+"' : right operand is <=0");

	string s="";
	for(;n>0;n--)
		s+=(string)l;
	
	return s;
}

// string[int]
GYS_ret_val op_index_str(GYS* script, GYS_ret_val l, GYS_ret_val r)
{
	string s=l;
	unsigned int i=r;

	if(i<0 || i>=s.size()) //error
	{
		script->error("'"+l.get_type_name()+"' ["+r.get_type_name()+"] : invalid index");
		return GYS_ret_val();
	}

	return (char)s[i];
}

void add_string_type(GYS* s)
{
	s->add_gys_type(gys_string_type, get_string_val);
	s->add_gys_func(gys_string_type, exec_type_name);

	s->add_binary_op("=", gys_string_type, gys_string_type, op_assign);

	s->add_binary_op("+", gys_string_type, gys_string_type, op_add<string, string, string>);
	s->add_binary_op("*", gys_string_type, gys_int_type,    op_mul_str);

	s->add_binary_op("*=", gys_string_type, gys_int_type,    op_mul_assign_str);
	s->add_binary_op("+=", gys_string_type, gys_string_type, op_add_assign<string, string, string>);

	s->add_binary_op("==", gys_string_type, gys_string_type, op_equal<string, string, bool>);
	s->add_binary_op("!=", gys_string_type, gys_string_type, op_not_equal<string, string, bool>);
	s->add_binary_op(">",  gys_string_type, gys_string_type, op_greater<string, string, bool>);
	s->add_binary_op("<",  gys_string_type, gys_string_type, op_less<string, string, bool>);
	s->add_binary_op(">=", gys_string_type, gys_string_type, op_greater_or_equal<string, string, bool>);
	s->add_binary_op("<=", gys_string_type, gys_string_type, op_less_or_equal<string, string, bool>);

	s->add_binary_op("[", gys_string_type, gys_int_type, op_index_str);
}