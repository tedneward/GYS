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
  \brief Source file for stdGYS int type.

*/

#include "gys_int_type.h"
#include "gys_type_stuff.cpp" // VC7 doesn't support export keyword with templates thus we include cpp-file

#include "gys_double_type.h"

class GYS_int_user_val: public GYS_user_val
{
private:
	int val;

public:
	GYS_int_user_val(int i) : val(i) { }

	string get_type_name() const {
		return gys_int_type;
	}		

	void* get_user_val() {
		return &val;
	}
};

GYS_ret_val& store(GYS_ret_val& val, int int_user_val)
{
	val.set_user_val( new GYS_int_user_val(int_user_val) );
	return val;
}

// gets raw int data
GYS_ret_val get_int_val(GYS* script)
{
	script->get_token();

	if(script->get_tok_type()==GYS_DOUBLE)
	{
		return (int)script->get_tok_val();
	}
	else
	{
		script->error("'"+(string)gys_int_type+"' getting: numeric-val expected: ", script->get_tok_str());
		return GYS_ret_val();
	}	
}

void add_int_type(GYS* s)
{
	s->add_gys_type(gys_int_type, get_int_val); 
	s->add_gys_func(gys_int_type, exec_type_name);

	s->add_binary_op("=",  gys_int_type, gys_double_type, op_assign);
	s->add_binary_op("+=", gys_int_type, gys_double_type, op_add_assign<int, double, int>);
	s->add_binary_op("-=", gys_int_type, gys_double_type, op_sub_assign<int, double, int>);
	s->add_binary_op("*=", gys_int_type, gys_double_type, op_mul_assign<int, double, int>);
	s->add_binary_op("/=", gys_int_type, gys_double_type, op_div_assign<int, double, int>);
	s->add_binary_op("%=", gys_int_type, gys_int_type,    op_mod_assign<int, int, int>);

	s->add_binary_op("+", gys_int_type, gys_double_type, op_add<int, double, double>);
	s->add_binary_op("-", gys_int_type, gys_double_type, op_sub<int, double, double>);
	s->add_binary_op("*", gys_int_type, gys_double_type, op_mul<int, double, double>);
	s->add_binary_op("/", gys_int_type, gys_double_type, op_div<int, double, double>);
	s->add_binary_op("%", gys_int_type, gys_int_type,    op_mod<int, int, int>);

	s->add_binary_op("==", gys_int_type, gys_double_type, op_equal<int ,double, bool>);
	s->add_binary_op("!=", gys_int_type, gys_double_type, op_not_equal<int, double, bool>);
	s->add_binary_op(">",  gys_int_type, gys_double_type, op_greater<int, double, bool>);
	s->add_binary_op("<",  gys_int_type, gys_double_type, op_less<int, double, bool>);
	s->add_binary_op(">=", gys_int_type, gys_double_type, op_greater_or_equal<int, double, bool>);
	s->add_binary_op("<=", gys_int_type, gys_double_type, op_less_or_equal<int, double, bool>);
	
	s->add_binary_op("&&", gys_int_type, gys_double_type, op_and<int, double, bool>);
	s->add_binary_op("||", gys_int_type, gys_double_type, op_or<int, double, bool>);

	s->add_post_unary_op("++", gys_int_type, op_post_inc<int>);
	s->add_post_unary_op("--", gys_int_type, op_post_dec<int>);

	s->add_pre_unary_op("+",  gys_int_type, op_plus_unar<int>);
	s->add_pre_unary_op("-",  gys_int_type, op_minus_unar<int>);
	s->add_pre_unary_op("!",  gys_int_type, op_not<int>);
	s->add_pre_unary_op("++", gys_int_type, op_pre_inc_unar<int>);
	s->add_pre_unary_op("--", gys_int_type, op_pre_dec_unar<int>);
}