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
  \brief Source file for stdGYS vector type.

*/

#include "gys_vector_type.h"
#include "gys_type_stuff.cpp"

#include "gys_int_type.h"


class GYS_vector_user_val: public GYS_user_val
{
private:
	GYS_vector_val val;

public:
	GYS_vector_user_val(GYS_vector_val v) : val(v) { }

	string get_type_name() const {
		return gys_vector_type;
	}		

	void* get_user_val() {
		return &val;
	}
};

GYS_ret_val& store(GYS_ret_val& val, GYS_vector_val vector_user_val)
{
	val.set_user_val( new GYS_vector_user_val(vector_user_val) );
	return val;
}

// #type                       - empty vector
// #type{v0, v1, v2, ... , vN} - initialized (with elements) vector
// #type[int]:default_val      - initialized vector with N elements (=default_val)
// #... = #vector...
GYS_ret_val get_vector_val(GYS* s)
{
	if(!s->exam("#"))
		return GYS_ret_val();

	// read type of elements
	s->get_token();

	string vtype=gys_void_type;

	if(s->get_tok_type()==GYS_NAME && s->is_typename(s->get_tok_str()) )
		vtype=s->get_tok_str();
	else // '#' means '#vector'
	{
		s->putback_tok();
		vtype=gys_vector_type;
	}

	GYS_vector_val vec(vtype);

	s->get_token();

	if(s->get_tok_type()==GYS_DELIMETER && s->get_tok_str()=="{") // {v1, v2, ... , vN}
	{
		s->get_token();

		// read elements
		if( !(s->get_tok_type()==GYS_DELIMETER && s->get_tok_str()=="}") ) // {} - possible
		{
			s->putback_tok();

			for(;;)
			{
				if( !vec.push_back(s->get_val(vtype)) )
					s->error("'"+(string)gys_vector_type+"' getting: can't add this element.");

				s->get_token();
				if(s->get_tok_type()==GYS_DELIMETER && s->get_tok_str()=="}")
					break;
				else // elements are separated by commas
				{
					s->putback_tok();
					s->exam(",");
				}	
			}
		}
	}
	else if(s->get_tok_type()==GYS_DELIMETER && s->get_tok_str()=="[")  // [N]:default_val
	{
		int n=s->get_val(gys_int_type);

		if(!s->exam("]"))
			return GYS_ret_val();

		if(!s->exam(":"))
			return GYS_ret_val();

		GYS_ret_val def_val=s->get_val(vtype);

		if(!vec.resize(n, def_val))// resize and check
		{
			s->error("'"+(string)gys_vector_type+"' getting: can't resize in this way.");
			return GYS_ret_val();
		}
	}
	else // empty vector
		s->putback_tok();

	return vec;
}

// vector[int] - returns lvalue if vector is lvalue
GYS_ret_val op_index_vector(GYS* script, GYS_ret_val l, GYS_ret_val r)
{
	GYS_ret_val* v=(*(GYS_vector_val*)l )[ (int)r ];
	
	if(!v) // error
	{
		script->error((string)gys_vector_type+"[]: invalid index");
		return GYS_ret_val();
	}

	return l.is_lvalue() ? v : *v;
}

// vector+vector
GYS_ret_val op_add_vector(GYS* script, GYS_ret_val l, GYS_ret_val r)
{
	GYS_ret_val t=(GYS_vector_val)l; // temp

	// push all elements from r into t with conversion
	if((GYS_vector_val*)r)
	{
		unsigned r_size=((GYS_vector_val)r).get_vector().size();
		for(unsigned i=0; i<r_size; i++)
			if(! (*(GYS_vector_val*)t ).
push_back( script->exec_converter(*((GYS_vector_val)r)[i], ((GYS_vector_val)l).get_type_name()) ) )
				script->error("'"+(string)gys_vector_type+"' + '"+gys_vector_type+"' : can't add new element.");
	}

	return t;
}

// +=
GYS_ret_val op_add_assign_vector(GYS* script, GYS_ret_val l, GYS_ret_val r)
{
	if(!l.is_lvalue())
	{
		script->error("'"+(string)gys_vector_type+"' += '"+gys_vector_type+"' : left operand must be lvalue.");
		return l;
	}

	// push all elements from r into l with conversion
	if((GYS_vector_val*)r)
	{
		unsigned r_size=((GYS_vector_val)r).get_vector().size(); // avoids endless loop (consider this example: vec_var+=vec_var)
		for(unsigned i=0; i<r_size; i++)
			if(! (*(GYS_vector_val*)l ).
push_back( script->exec_converter(((GYS_vector_val)r)[i]->get_value(), ((GYS_vector_val)l).get_type_name()) ) )
				script->error("'"+(string)gys_vector_type+"' += '"+gys_vector_type+"' : can't add new element.");
	}

	return l;
}

// range op vector(low~up) - returns elements as new non-lvalue vector [low; up]
GYS_ret_val op_post_range_vector(GYS* script, GYS_ret_val l)
{
	int low=script->get_val(gys_int_type);

	// '~' check
	script->exam("~");
	
	int up=script->get_val(gys_int_type);

    script->exam(")");

	GYS_vector_val vec=l;

	if(low>up)
	{
		script->error("'"+(string)gys_vector_type+"' (int~int) : lower index > upper index.");
		return GYS_ret_val();
	}

	if(!vec[low])
	{
		script->error("'"+(string)gys_vector_type+"' (int~int) : invalid lower index.");
		return GYS_ret_val();
	}

	if(!vec[up])
	{
		script->error("'"+(string)gys_vector_type+"' (int~int) : invalid upper index.");
		return GYS_ret_val();
	}

	GYS_vector_val out_vec(vec.get_type_name());

	for(int i=low;i<=up;i++)
		out_vec.push_back(*vec[i]);

	return out_vec;
}

void add_vector_type(GYS* s)
{
	s->add_gys_type(gys_vector_type, get_vector_val);
	s->add_gys_func(gys_vector_type, exec_type_name);

	s->add_binary_op("=", gys_vector_type, gys_vector_type, op_assign);

	s->add_binary_op("[", gys_vector_type, gys_int_type, op_index_vector);
	s->add_binary_op("+", gys_vector_type, gys_vector_type, op_add_vector);
	s->add_binary_op("+=", gys_vector_type, gys_vector_type, op_add_assign_vector);

	s->add_post_unary_op("(", gys_vector_type, op_post_range_vector);
}

//--------------------------GYS vector value-------------------------------
GYS_vector_val::GYS_vector_val(string vtype) : val_type(vtype) {}

bool GYS_vector_val::push_back(GYS_ret_val v)	
{
	if(v.get_type_name()!=val_type ||  vec.size()>=vec.max_size())  // error
		return false;
	
	vec.push_back(v);
	return true;		
}

bool GYS_vector_val::pop_back()	
{
	if(vec.size()<1)  // error
		return false;
		
	vec.pop_back();

	return true;		
}

// inserts v before element vec[pos]
bool GYS_vector_val::insert(unsigned int pos, GYS_ret_val v)	
{
	if(pos<0 || pos>vec.size() || v.get_type_name()!=val_type ||  vec.size()>=vec.max_size())  // error
		return false;
	
	vec.insert(&vec[pos], v);
	return true;		
}

bool GYS_vector_val::erase(unsigned int pos)	
{
	if(pos<0 || pos>=vec.size())  // error
		return false;
	
	vec.erase(&vec[pos]);
	return true;		
}

void GYS_vector_val::clear()
{
	vec.clear();
}

bool GYS_vector_val::resize(unsigned int n, GYS_ret_val def_val)
{
	if(n<0 || n>vec.max_size() || def_val.get_type_name()!=val_type) // error
		return false;

	vec.resize(n, def_val);
	
	return true;
}

GYS_ret_val* GYS_vector_val::operator [](unsigned int index)
{
	if(index<0 || index>=vec.size()) // error
		return 0;
	
	return &vec[index];
}

vector<GYS_ret_val> GYS_vector_val::get_vector() const
{
	return vec;
}

string GYS_vector_val::get_type_name() const
{
	return val_type;
}