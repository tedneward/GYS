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
  \brief Source file for stdGYS conversions.
*/

#include "gys_converters.h"

#include "gys_bool_type.h"
#include "gys_int_type.h"
#include "gys_double_type.h"
#include "gys_char_type.h"
#include "gys_string_type.h"
#include "gys_vector_type.h"
#include "gys_function_type.h"

#include <sstream>  // ostreamstring

template<class From, class Into> GYS_ret_val convert(GYS* script, GYS_ret_val from_val)
{
	return from_val.set_val((Into) ((From)from_val));
}

template<class From, class Into> GYS_ret_val convert_and_warn(GYS* script, GYS_ret_val from_val)
{
	string from_type_name=from_val.get_type_name();

	GYS_ret_val new_val=from_val.set_val((Into) ((From)from_val));
	script->warning("Unsafe conversion from '"+from_type_name+"' into '"+new_val.get_type_name()+"'.");

	return new_val;
}

template<class From> GYS_ret_val convert_into_string(GYS* script, GYS_ret_val from_val)
{
	ostringstream s;
	s<<(From)from_val;

	return s.str(); // string
}

// char -> string
GYS_ret_val convert_ctos(GYS* script, GYS_ret_val from_val)
{
	return string("")+(char)from_val;
}

// string -> char
GYS_ret_val convert_stoc(GYS* script, GYS_ret_val from_val)
{
	string str=from_val;

	if(str.size()<=0)
	{
		script->error("Conversion from '"+(string)gys_string_type+"' into '"+gys_char_type+"' : string is empty.");
		return GYS_ret_val();
	}

	script->warning("Unsafe conversion from '"+(string)gys_string_type+"' into '"+gys_char_type+"'.");

	return (char)str[0];
}

// vector -> vector[0] with warning (aux func)
GYS_ret_val convert_from_vector(GYS* script, GYS_ret_val from_val, string Into_type_name)
{
	if(((GYS_vector_val)from_val).get_vector().size()<1)
	{
		script->error("Conversion from '"+from_val.get_type_name()+"' into '"+Into_type_name+"' : vector is empty");
		return GYS_ret_val();
	}
	
	script->warning("Unsafe conversion from '"+from_val.get_type_name()+"' into '"+Into_type_name+"'.");
	return script->exec_converter(*((GYS_vector_val)from_val)[0], Into_type_name);
}

// vector -> bool!
GYS_ret_val convert_vtob(GYS* script, GYS_ret_val from_val)
{
	return convert_from_vector(script, from_val, gys_bool_type);
}

// vector -> int!
GYS_ret_val convert_vtoi(GYS* script, GYS_ret_val from_val)
{
	return convert_from_vector(script, from_val, gys_int_type);
}

// vector -> double!
GYS_ret_val convert_vtod(GYS* script, GYS_ret_val from_val)
{
	return convert_from_vector(script, from_val, gys_double_type);
}

// vector -> char!
GYS_ret_val convert_vtoc(GYS* script, GYS_ret_val from_val)
{
	return convert_from_vector(script, from_val, gys_char_type);
}

// vector -> function!
GYS_ret_val convert_vtof(GYS* script, GYS_ret_val from_val)
{
	return convert_from_vector(script, from_val, gys_function_type);
}

// universal into vector conversion
GYS_ret_val convert_into_vector(GYS* script, GYS_ret_val from_val)
{
	GYS_vector_val vec(from_val.get_type_name());
	vec.push_back(from_val.get_value()); // not lvalue

	return vec;
}

// vector -> string! (converts all the elements into strings and concats them)
GYS_ret_val convert_vtos(GYS* script, GYS_ret_val from_val)
{
	script->warning("Unsafe conversion from '"+from_val.get_type_name()+"' into '"+gys_string_type+"'.");

	string str="";
		
	for(unsigned i=0;i<((GYS_vector_val)from_val).get_vector().size();i++)
		str+=(string)script->exec_converter( * ( ( (GYS_vector_val)from_val ) [i] ), gys_string_type );

	return str;
}

// string -> vector of char
GYS_ret_val convert_stov(GYS* script, GYS_ret_val from_val)
{
	string str=from_val;

	GYS_vector_val vec(gys_char_type);
	for(unsigned i=0;i<str.size();i++)
		vec.push_back(str[i]);
	
	return vec;
}

void add_converters(GYS* s)
{
	s->add_gys_converter(gys_double_type, gys_int_type,  convert_and_warn<double, int>);
	s->add_gys_converter(gys_double_type, gys_bool_type, convert_and_warn<double, bool>);

	s->add_gys_converter(gys_int_type, gys_double_type, convert<int, double>);
	s->add_gys_converter(gys_int_type, gys_bool_type,   convert_and_warn<int, bool>);

	s->add_gys_converter(gys_bool_type, gys_int_type,     convert<bool, int>);
	s->add_gys_converter(gys_bool_type, gys_double_type,  convert<bool, double>);

	s->add_gys_converter(gys_bool_type,   gys_string_type, convert_into_string<bool>);
	s->add_gys_converter(gys_int_type,    gys_string_type, convert_into_string<int>);
	s->add_gys_converter(gys_double_type, gys_string_type, convert_into_string<double>);

	s->add_gys_converter(gys_char_type,   gys_string_type, convert_ctos);
	s->add_gys_converter(gys_string_type, gys_char_type,   convert_stoc);

	s->add_gys_converter(gys_vector_type, gys_bool_type,     convert_vtob);
	s->add_gys_converter(gys_vector_type, gys_int_type,      convert_vtoi);
	s->add_gys_converter(gys_vector_type, gys_double_type,   convert_vtod);
	s->add_gys_converter(gys_vector_type, gys_char_type,     convert_vtoc);
	s->add_gys_converter(gys_vector_type, gys_function_type, convert_vtof);

	s->add_gys_converter(gys_bool_type,     gys_vector_type, convert_into_vector);
	s->add_gys_converter(gys_int_type,      gys_vector_type, convert_into_vector);
	s->add_gys_converter(gys_double_type,   gys_vector_type, convert_into_vector);
	s->add_gys_converter(gys_char_type,     gys_vector_type, convert_into_vector);
	s->add_gys_converter(gys_function_type, gys_vector_type, convert_into_vector);
	//s->add_gys_converter(gys_string_type, gys_vector_type, convert_into_vector);

	s->add_gys_converter(gys_vector_type, gys_string_type, convert_vtos);

	s->add_gys_converter(gys_string_type, gys_vector_type, convert_stov);
}