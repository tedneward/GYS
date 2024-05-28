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
    \brief Source file for stdGYS conversion functions.
*/

#include "gys_convert_func.h"

#include "gys_int_type.h"
#include "gys_double_type.h"
#include "gys_char_type.h"
#include "gys_string_type.h"

GYS_ret_val exec_stod(GYS* script) // string into double
{
	script->exam("(");

	double result=strtod(((string)script->get_val(gys_string_type)).c_str(), 0);

	script->exam(")");

	return result;
}


GYS_ret_val exec_stoi(GYS* script) // string into int
{
	script->exam("(");

	int result=strtol(((string)script->get_val(gys_string_type)).c_str(), 0, 10);

	script->exam(")");

	return result;
}

GYS_ret_val exec_ctoi(GYS* script) // char into int (ASCII)
{
	script->exam("(");

	char c=script->get_val(gys_char_type);
	
	script->exam(")");

	return (int)c;
}

GYS_ret_val exec_itoc(GYS* script) // int into char (ASCII)
{
	script->exam("(");

	int i=script->get_val(gys_int_type);
	
	script->exam(")");

	return (char)i;
}


void add_convert_funcs(GYS* script)
{
	script->add_gys_func("stoi", exec_stoi);	
	script->add_gys_func("stod", exec_stod);	
	
	script->add_gys_func("ctoi", exec_ctoi);
	script->add_gys_func("itoc", exec_itoc);
}
