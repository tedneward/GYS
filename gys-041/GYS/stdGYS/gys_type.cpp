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
  \brief Source file for stdGYS (types, operators, conversions).

  \todo
	- another function using syntax
	- another vector using syntax
*/

/*!
  \page news

  \section types stdGYS types, operators and conversions...
	- rewritten: everything
*/

#include "gys_type.h"

#include "gys_void_type.h"
#include "gys_bool_type.h"
#include "gys_int_type.h"
#include "gys_double_type.h"
#include "gys_char_type.h"
#include "gys_string_type.h"
#include "gys_vector_type.h"
#include "gys_function_type.h"

#include "gys_converters.h"

//! Universal 'global' function for var definition syntax
GYS_ret_val exec_global(GYS* s)
{
	GYS_ret_val val;
	string name;
	
	s->get_token();

	if( !(s->get_tok_type()==GYS_NAME && s->is_typename(s->get_tok_str())) )
	{
		s->error("Typename expected: ", s->get_tok_str());
		return GYS_ret_val();
	}

	string type=s->get_tok_str();
		
new_ext_var:
	s->get_token();
	if(s->get_tok_type()!=GYS_NAME && s->get_tok_type()!=GYS_CONST)
	{
		s->error("Variable or constant token expected: ", s->get_tok_str());
		return GYS_ret_val();
	}

	name=s->get_tok_str();
	
	if(!s->exam("="))
		return GYS_ret_val();

	if( ( val=s->get_val( type ) ).get_type_name()!=gys_void_type ) // not void
		s->add_global_var(name, val.get_value());
	
	s->get_token();
	if(s->get_tok_type()==GYS_DELIMETER && s->get_tok_str()==",")
		goto new_ext_var;
	else
		s->putback_tok();

	return GYS_ret_val();
}

void add_stdgys_types(GYS* s)
{
	s->add_gys_func("global", exec_global); // global var definition syntax - BAD (e.g. user needs his own syntax for globals)

	add_void_type(s);
	add_bool_type(s);
	add_int_type(s);
	add_double_type(s);
	add_char_type(s);
	add_string_type(s);
	add_vector_type(s);
	add_function_type(s);

	add_converters(s);
}