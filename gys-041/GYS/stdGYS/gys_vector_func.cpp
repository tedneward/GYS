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
    \brief Source file for stdGYS functions for vectors.
*/

#include "gys_vector_func.h"

#include "gys_vector_type.h"
#include "gys_string_type.h"
#include "gys_int_type.h"
#include "gys_bool_type.h"

GYS_ret_val exec_make_vector(GYS* script)
{
	script->exam("(");

	GYS_vector_val vec( (string) script->get_val(gys_string_type) );

	script->exam(")");	

	return vec;
}

GYS_ret_val exec_content_type(GYS* script)
{
	script->exam("(");

	GYS_vector_val vec=script->get_val(gys_vector_type);

	script->exam(")");	

	return vec.get_type_name();
}

GYS_ret_val exec_size(GYS* script)
{
	script->exam("(");

	GYS_vector_val vec=script->get_val(gys_vector_type);
			
	script->exam(")");	

	return (int) (vec.get_vector().size());
}

GYS_ret_val exec_empty(GYS* script)
{
	script->exam("(");

	GYS_vector_val vec=script->get_val(gys_vector_type);

	script->exam(")");	

	return (bool) vec.get_vector().empty();
}

GYS_ret_val exec_front(GYS* script)
{
	script->exam("(");

	GYS_vector_val vec=script->get_val(gys_vector_type);

	script->exam(")");	

	if( !(vec.get_vector().empty()) )
		return vec.get_vector().front();
	else
		script->error("front(): vector is empty");
			
	return GYS_ret_val(); // error
}

GYS_ret_val exec_back(GYS* script)
{
	script->exam("(");

	GYS_vector_val vec=script->get_val(gys_vector_type);

	script->exam(")");	

	if( !(vec.get_vector().empty()) )
		return vec.get_vector().back();
	else
		script->error("back(): vector is empty");
			
	return GYS_ret_val(); // error
}

GYS_ret_val exec_push_back(GYS* script)
{
	script->exam("(");

	GYS_ret_val vec=script->get_val(gys_vector_type);

	if(!vec.is_lvalue())
		script->error("push_back(): argument 1 must be lvalue");

	script->exam(",");
	
	GYS_ret_val new_val=script->get_val( ( (GYS_vector_val)vec ).get_type_name() );

	script->exam(")");	
	
	if((GYS_vector_val*)vec)
		if( !(*(GYS_vector_val*)vec).push_back(new_val.get_value()) ) // adding new val and check
			script->error("push_back(): can't add this value");
	
	return (GYS_vector_val)vec;
}

GYS_ret_val exec_pop_back(GYS* script)
{
	script->exam("(");

	GYS_ret_val vec=script->get_val(gys_vector_type);

	if(!vec.is_lvalue())
		script->error("pop_back(): argument 1 must be lvalue");

	script->exam(")");	
	
	if((GYS_vector_val*)vec)
		if( !(*(GYS_vector_val*)vec).pop_back() ) // pop and check
			script->error("pop_back(): vector is empty");
	
	return (GYS_vector_val)vec;
}

GYS_ret_val exec_insert(GYS* script)
{
	script->exam("(");

	GYS_ret_val vec=script->get_val(gys_vector_type);

	if(!vec.is_lvalue())
		script->error("insert(): argument 1 must be lvalue");

	script->exam(",");

	int pos=script->get_val(gys_int_type);

	script->exam(",");
	
	GYS_ret_val v=script->get_val(((GYS_vector_val)vec).get_type_name());
						
	script->exam(")");	
	
	if((GYS_vector_val*)vec)
		if( !(*(GYS_vector_val*)vec).insert(pos, v.get_value()) )  // inserting new val and check
			script->error("insert(): can't insert in this way");
	
	return (GYS_vector_val)vec;
}

GYS_ret_val exec_erase(GYS* script)
{
	script->exam("(");

	GYS_ret_val vec=script->get_val(gys_vector_type);

	if(!vec.is_lvalue())
		script->error("erase(): argument 1 must be lvalue");

	script->exam(",");

	int pos=script->get_val(gys_int_type);

	script->exam(")");	
	
	if((GYS_vector_val*)vec)
		if( !(*(GYS_vector_val*)vec).erase(pos) ) // erase and check
			script->error("erase(): invalid index");
	
	return (GYS_vector_val)vec;
}

GYS_ret_val exec_clear(GYS* script)
{	
	script->exam("(");

	GYS_ret_val vec=script->get_val(gys_vector_type);

	if(!vec.is_lvalue())
		script->error("clear(): argument 1 must be lvalue");

	script->exam(")");	
	
	if((GYS_vector_val*)vec)
		(*(GYS_vector_val*)vec).clear();
				
	return (GYS_vector_val)vec;
}

void add_vector_funcs(GYS* script)
{
	script->add_gys_func("make_vector", exec_make_vector);

	script->add_gys_func("content_type", exec_content_type);
	script->add_gys_func("size",  exec_size);
	script->add_gys_func("empty", exec_empty);
	script->add_gys_func("front", exec_front);
	script->add_gys_func("back",  exec_back);

	script->add_gys_func("push_back", exec_push_back);
	script->add_gys_func("pop_back",  exec_pop_back);
	script->add_gys_func("insert",    exec_insert);
	script->add_gys_func("erase",     exec_erase);
	script->add_gys_func("clear",     exec_clear);
}