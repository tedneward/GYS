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
  \brief Source file for stdGYS bool type.

*/

#include "gys_bool_type.h"
#include "gys_type_stuff.cpp" // VC7 doesn't support export keyword with templates thus we include cpp-file

#include "gys_int_type.h"

class GYS_bool_user_val: public GYS_user_val
{
private:
	bool val;

public:
	GYS_bool_user_val(bool b) : val(b) { }

	string get_type_name() const {
		return gys_bool_type;
	}		

	void* get_user_val() {
		return &val;
	}
};

GYS_ret_val& store(GYS_ret_val& val, bool bool_user_val)
{
	val.set_user_val( new GYS_bool_user_val(bool_user_val) );
	return val;
}

// gets raw bool data
GYS_ret_val get_bool_val(GYS* script)
{
	script->get_token();

	if(script->get_tok_type()==GYS_DOUBLE)
	{
		return (bool)script->get_tok_val();
	}
	else
	{
		script->error("'"+(string)gys_bool_type+"' getting: numeric-val expected: ", script->get_tok_str());
		return GYS_ret_val();
	}	
}

void add_bool_type(GYS* s)
{
	s->add_gys_type(gys_bool_type, get_bool_val); 
	s->add_gys_func(gys_bool_type, exec_type_name);

	s->add_binary_op("=", gys_bool_type, gys_bool_type, op_assign);

	s->add_binary_op("==", gys_bool_type, gys_bool_type, op_equal<bool, bool, bool>);
	s->add_binary_op("!=", gys_bool_type, gys_bool_type, op_not_equal<bool, bool, bool>);
	s->add_binary_op(">",  gys_bool_type, gys_bool_type, op_greater<bool, bool, bool>);
	s->add_binary_op("<",  gys_bool_type, gys_bool_type, op_less<bool, bool, bool>);
	s->add_binary_op(">=", gys_bool_type, gys_bool_type, op_greater_or_equal<bool, bool, bool>);
	s->add_binary_op("<=", gys_bool_type, gys_bool_type, op_less_or_equal<bool, bool, bool>);
	
	s->add_binary_op("&&", gys_bool_type, gys_bool_type, op_and<bool, bool, bool>);
	s->add_binary_op("||", gys_bool_type, gys_bool_type, op_or<bool, bool, bool>);

	s->add_pre_unary_op("!",  gys_bool_type, op_not<bool>);
	s->add_pre_unary_op("+",  gys_bool_type, op_plus_unar<bool>);
}