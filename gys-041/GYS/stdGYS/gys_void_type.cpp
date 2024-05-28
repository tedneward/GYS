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
  \brief Source file for stdGYS void type.

*/

#include "gys_void_type.h"

GYS_ret_val get_void_val(GYS* script)
{
	return GYS_ret_val(); // void value
}

// void(<expr>) is possible
// void var=<expr> - not
static GYS_ret_val exec_void(GYS* s)
{
	s->get_token();
	if(s->get_tok_type()==GYS_DELIMETER && s->get_tok_str()=="(")
	{
		s->putback_tok();
		return s->get_val(gys_void_type); // ...(expr)
	}
	else
		s->error("Variables of '"+(string)gys_void_type+"' type can't be defined.");

	return GYS_ret_val();
}

void add_void_type(GYS* script)
{
	script->add_gys_type(gys_void_type, get_void_val);
	script->add_gys_func(gys_void_type, exec_void);
}
