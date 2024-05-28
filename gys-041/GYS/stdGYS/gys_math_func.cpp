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
    \brief Source file for stdGYS math functions.
*/

#include "gys_math_func.h"

#include "gys_int_type.h"
#include "gys_double_type.h"

#include <math.h>
#include <time.h>

GYS_ret_val exec_abs(GYS* script)
{
	script->exam("(");

	double result=script->get_val(gys_double_type);

	script->exam(")");

	return result<0 ? (-result) : result;
}

GYS_ret_val exec_exp(GYS* script) 
{
	script->exam("(");

	double result=exp((double)script->get_val(gys_double_type));

	script->exam(")");

	return result;
}

GYS_ret_val exec_log(GYS* script) 
{
	script->exam("(");

	double result=log((double)script->get_val(gys_double_type));

	script->exam(")");

	return result;
}

GYS_ret_val exec_pow(GYS* script) 
{
	script->exam("(");

	double x=script->get_val(gys_double_type);

	script->exam(",");

	double y=script->get_val(gys_double_type);

	script->exam(")");

	return pow(x, y);
}

GYS_ret_val exec_sqrt(GYS* script)
{
	script->exam("(");

	double result=sqrt((double)script->get_val(gys_double_type));

	script->exam(")");

	return result;
}

GYS_ret_val exec_max(GYS* script) 
{
	script->exam("(");

	double x=script->get_val(gys_double_type);

	script->exam(",");

	double y=script->get_val(gys_double_type);

	script->exam(")");

	return (double)__max(x, y);
}

GYS_ret_val exec_min(GYS* script) 
{
	script->exam("(");

	double x=script->get_val(gys_double_type);

	script->exam(",");

	double y=script->get_val(gys_double_type);

	script->exam(")");

	return (double)__min(x, y);
}

GYS_ret_val exec_mod(GYS* script)
{
	script->exam("(");

	int x=script->get_val(gys_int_type);

	script->exam(",");

	int y=script->get_val(gys_int_type);

	if(y==0) 
	{
		x=0;
		y=1;
		script->error("mod(): mod by 0, return 0 as a result.");
	}

	script->exam(")");

	return x%y; // int
}

GYS_ret_val exec_floor(GYS* script)
{
	script->exam("(");

	double result=floor( (double)script->get_val(gys_double_type) );

	script->exam(")");
	
	return result;
}

GYS_ret_val exec_ceil(GYS* script)
{
	script->exam("(");

	double result=ceil( (double)script->get_val(gys_double_type) );

	script->exam(")");
	
	return result;
}

GYS_ret_val exec_srand(GYS* script)
{
	script->exam("(");

	script->get_token();

	int seed;

	if(script->get_tok_str()==")" && script->get_tok_type()==GYS_DELIMETER)
		seed=time(0);
	else
	{
		script->putback_tok();
		seed=(int)script->get_val(gys_int_type);

		script->exam(")");
	}	

	srand(seed);

	return GYS_ret_val();
}

GYS_ret_val exec_rand(GYS* script)
{
	script->exam("(");
	script->exam(")");

	return (int)rand();
}

GYS_ret_val exec_sin(GYS* script)
{
	script->exam("(");

	double result=sin((double)script->get_val(gys_double_type));

	script->exam(")");

	return result;
}

GYS_ret_val exec_asin(GYS* script)
{
	script->exam("(");

	double result=asin((double)script->get_val(gys_double_type));

	script->exam(")");
	
	return result;
}

GYS_ret_val exec_sinh(GYS* script)
{
	script->exam("(");

	double result=sinh((double)script->get_val(gys_double_type));

	script->exam(")");

	return result;
}

GYS_ret_val exec_cos(GYS* script)
{
	script->exam("(");

	double result=cos((double)script->get_val(gys_double_type));

	script->exam(")");

	return result;
}

GYS_ret_val exec_acos(GYS* script)
{
	script->exam("(");

	double result=acos((double)script->get_val(gys_double_type));

	script->exam(")");

	return result;
}

GYS_ret_val exec_cosh(GYS* script)  
{
	script->exam("(");

	double result=cosh((double)script->get_val(gys_double_type));

	script->exam(")");

	return result;
}

GYS_ret_val exec_tan(GYS* script) 
{
	script->exam("(");

	double result=tan((double)script->get_val(gys_double_type));

	script->exam(")");

	return result;
}

GYS_ret_val exec_atan(GYS* script)
{
	script->exam("(");

	double result=atan((double)script->get_val(gys_double_type));

	script->exam(")");

	return result;
}

GYS_ret_val exec_tanh(GYS* script)
{
	script->exam("(");

	double result=tanh((double)script->get_val(gys_double_type));

	script->exam(")");

	return result;
}

void add_math_funcs(GYS* script)
{
	script->add_gys_func("abs",   exec_abs);
	script->add_gys_func("exp",   exec_exp);
	script->add_gys_func("log",   exec_log);
	script->add_gys_func("pow",   exec_pow);
	script->add_gys_func("sqrt",  exec_sqrt);
	script->add_gys_func("max",   exec_max);
	script->add_gys_func("min",   exec_min);
	script->add_gys_func("mod",   exec_mod);
	script->add_gys_func("floor", exec_floor);
	script->add_gys_func("ceil",  exec_ceil);

	script->add_gys_func("srand", exec_srand);
	script->add_gys_func("rand",  exec_rand);

	script->add_gys_func("sin",  exec_sin);
	script->add_gys_func("asin", exec_asin);
	script->add_gys_func("sinh", exec_sinh);
	script->add_gys_func("cos",  exec_cos);
	script->add_gys_func("acos", exec_acos);
	script->add_gys_func("cosh", exec_cosh);
	script->add_gys_func("tan",  exec_tan);
	script->add_gys_func("atan", exec_atan);
	script->add_gys_func("tanh", exec_tanh);
}
