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
  \brief Source file for stdGYS function type.

*/

#include "gys_function_type.h"
#include "gys_type_stuff.cpp"

#include "gys_int_type.h"

#include <set>
#include <algorithm> // count
#include <sstream>

#define VC_EXTRALEAN // for VC only
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

class GYS_function_user_val: public GYS_user_val
{
private:
	GYS_function_val val;

public:
	GYS_function_user_val(GYS_function_val f) : val(f) { }

	string get_type_name() const {
		return gys_function_type;
	}		

	void* get_user_val() {
		return &val;
	}
};

GYS_ret_val& store(GYS_ret_val& val, GYS_function_val function_user_val)
{
	val.set_user_val( new GYS_function_user_val(function_user_val) );
	return val;
}

GYS_ret_val get_function_val(GYS* s)
{
	if(!s->exam("~"))
		return GYS_ret_val();

	// return type
	string ftype;

	s->get_token();
	if(s->get_tok_type()==GYS_NAME &&  !s->is_typename(s->get_tok_str()) )
	{
		s->error("'"+(string)gys_function_type+"' getting: return typename expected: ", s->get_tok_str());
		return GYS_ret_val();
	}

	ftype=s->get_tok_str();

	// read params
	if(!s->exam("("))
		return GYS_ret_val();

	GYS_func_params fparams=GYS_func_params();
	set<string> pnames_set; // for simple checking
	pnames_set.insert("SELF"); // script will create SELF at func-call

	// (t1 [@]v1, t2 [@]v2, ... , tN [@]vN)
	s->get_token();
	if(!(s->get_tok_type()==GYS_DELIMETER && s->get_tok_str()==")") ) // () - possible
	{
		s->putback_tok();

		string ptype, pname;
		for(;;)
		{
			s->get_token();		
			if(s->get_tok_type()==GYS_NAME && !s->is_typename(s->get_tok_str()) )
			{
				s->error("'"+(string)gys_function_type+"' getting: param typename expected: ", s->get_tok_str());
				return GYS_ret_val();
			}
			ptype=s->get_tok_str();

			s->get_token();
			if(s->get_tok_type()!=GYS_NAME && s->get_tok_type()!=GYS_CONST)
			{
				s->error("'"+(string)gys_function_type+"' getting: variable or constant token expected: ", s->get_tok_str());
				return GYS_ret_val();
			}

			pname=s->get_tok_str();

			// check and add
			if( pnames_set.find(pname)==pnames_set.end() && fparams.size()<fparams.max_size()
				&& pnames_set.size()<pnames_set.max_size())
			{
				fparams.push_back(make_pair(ptype, pname)); // add param
				pnames_set.insert(pname); // add param to the set
			}
			else
			{
				s->error("'"+(string)gys_function_type+"' getting: can't add new param to function: ", pname);
				return GYS_ret_val();
			}

			s->get_token();
			if(s->get_tok_type()==GYS_DELIMETER && s->get_tok_str()==")")
				break;
			else // params are separated by commas
			{
				s->putback_tok();
				s->exam(",");
			}	
		}
	}

	// read body
	string fbody="";
	if(!s->exam("{"))
		return GYS_ret_val();

	// move to } and read code
	int more=1;

	// for correct endl putting in body reading
	long old_line=s->get_line_num();
	long new_line=old_line;
	long endl_count=0;

	while(s->get_tok_type()!=GYS_EOF && more)
	{
		s->get_token();

		new_line=s->get_line_num();
		endl_count=new_line-old_line;

		string str=s->get_tok_str();
		endl_count-=count(str.begin(), str.end(), '\n'); // don't put endls from the tokens

		old_line=new_line;

		// putting endl for correct linenum
		for(int i=0;i<endl_count;i++)
			fbody+='\n';

		if( !(s->get_tok_type()==GYS_DELIMETER && s->get_tok_str()=="}" && more==1) )
		{
			if(s->get_tok_type()==GYS_STRING)
				fbody+='\"'+s->get_tok_str()+'\"';
			else if(s->get_tok_type()==GYS_CHAR)
				fbody+='\''+s->get_tok_str()+'\'';
			else
				fbody+=s->get_tok_str()+' ';
		}

		if(s->get_tok_type()==GYS_DELIMETER && s->get_tok_str()=="{") 
			more++; //we'll miss one }
		else if(s->get_tok_type()==GYS_DELIMETER && s->get_tok_str()=="}")
			more--;
	}

	if(s->get_tok_type()==GYS_EOF)
	{
		s->fatal_error("'"+(string)gys_function_type+"' getting: EOF found while reading the body");
		return GYS_ret_val();
	}
	
	return GYS_function_val(ftype, fparams, fbody);
}

// defunc [global] ret_type [@]fname(...) { ... }
GYS_ret_val exec_defunc(GYS* s)
{
	// return type
	string ftype;
	bool is_global=false;

	s->get_token();

	if( s->get_tok_type()==GYS_NAME && s->get_tok_str()=="global" )
	{
		is_global=true;
		s->get_token();
	}

	if( s->get_tok_type()==GYS_NAME && !s->is_typename(s->get_tok_str()) )
	{
		s->error("defunc: return typename expected: ", s->get_tok_str());
		return GYS_ret_val();
	}

	ftype=s->get_tok_str();

	// read name
	string fname;

	s->get_token();
	if(s->get_tok_type()!=GYS_NAME && s->get_tok_type()!=GYS_CONST)
	{
		s->error("defunc function name getting: variable or constant token expected: ", s->get_tok_str());
		return GYS_ret_val();
	}

	fname=s->get_tok_str();

	// read params
	if(!s->exam("("))
		return GYS_ret_val();

	GYS_func_params fparams=GYS_func_params();
	set<string> pnames_set; // for simple checking
	pnames_set.insert("SELF"); // script will create SELF at func-call

	// (t1 [@]v1, t2 [@]v2, ... , tN [@]vN)
	s->get_token();
	if( !(s->get_tok_type()==GYS_DELIMETER && s->get_tok_str()==")") ) // () - possible
	{
		s->putback_tok();

		string ptype, pname;
		for(;;)
		{
			s->get_token();		
			if( !(s->get_tok_type()==GYS_NAME && s->is_typename(s->get_tok_str())) )
			{
				s->error("defunc function param getting: param typename expected: ", s->get_tok_str());
				return GYS_ret_val();
			}
			ptype=s->get_tok_str();

			s->get_token();
			if(s->get_tok_type()!=GYS_NAME && s->get_tok_type()!=GYS_CONST)
			{
				s->error("defunc function param getting: variable or constant token expected: ", s->get_tok_str());
				return GYS_ret_val();
			}

			pname=s->get_tok_str();

			// check and add
			if( pnames_set.find(pname)==pnames_set.end() && fparams.size()<fparams.max_size()
				&& pnames_set.size()<pnames_set.max_size())
			{
				fparams.push_back(make_pair(ptype, pname)); // add param
				pnames_set.insert(pname); // add param to the set
			}
			else
			{
				s->error("defunc: can't add new param to function: ", pname);
				return GYS_ret_val();
			}

			s->get_token();
			if(s->get_tok_type()==GYS_DELIMETER && s->get_tok_str()==")")
				break;
			else // params are separated by commas
			{
				s->putback_tok();
				s->exam(",");
			}	
		}
	}

	// read body
	string fbody="";
	if(!s->exam("{"))
		return GYS_ret_val();

	// move to } and read code
	int more=1;

	// for correct endl putting in body reading
	long old_line=s->get_line_num();
	long new_line=old_line;
	long endl_count=0;

	while(s->get_tok_type()!=GYS_EOF && more)
	{
		s->get_token();

		new_line=s->get_line_num();
		endl_count=new_line-old_line;

		string str=s->get_tok_str();
		endl_count-=count(str.begin(), str.end(), '\n'); // don't put endls from the tokens

		old_line=new_line;

		// putting endl for correct linenum
		for(int i=0;i<endl_count;i++)
			fbody+='\n';

		if( !(s->get_tok_type()==GYS_DELIMETER && s->get_tok_str()=="}" && more==1) )
		{
			if(s->get_tok_type()==GYS_STRING)
				fbody+='\"'+s->get_tok_str()+'\"';
			else if(s->get_tok_type()==GYS_CHAR)
				fbody+='\''+s->get_tok_str()+'\'';
			else
				fbody+=s->get_tok_str()+' ';
		}

		if(s->get_tok_type()==GYS_DELIMETER && s->get_tok_str()=="{") 
			more++; //we'll miss one }
		else if(s->get_tok_type()==GYS_DELIMETER && s->get_tok_str()=="}")
			more--;
	}
	if(s->get_tok_type()==GYS_EOF)
	{
		s->fatal_error("defunc: EOF found while reading the body");
		return GYS_ret_val();
	}

	if(is_global)
		s->add_global_var(fname, GYS_function_val(ftype, fparams, fbody) );
	else
		s->add_local_var(fname, GYS_function_val(ftype, fparams, fbody) );

	return GYS_function_val(ftype, fparams, fbody);
}

// fn ret_type (...) { ... }
GYS_ret_val exec_func(GYS* s)
{
	// return type
	string ftype;

	s->get_token();
	if( !s->is_typename(s->get_tok_str()) )
	{
		s->error("func: return typename expected: ", s->get_tok_str());
		return GYS_ret_val();
	}

	ftype=s->get_tok_str();

	// read params
	if(!s->exam("("))
		return GYS_ret_val();

	GYS_func_params fparams=GYS_func_params();
	set<string> pnames_set; // for simple checking
	pnames_set.insert("SELF"); // script will create SELF at func-call

	// (t1 [@]v1, t2 [@]v2, ... , tN [@]vN)
	s->get_token();
	if(s->get_tok_str()!=")") // () - possible
	{
		s->putback_tok();

		string ptype, pname;
		for(;;)
		{
			s->get_token();		
			if( !s->is_typename(s->get_tok_str()) )
			{
				s->error("func function param getting: param typename expected: ", s->get_tok_str());
				return GYS_ret_val();
			}
			ptype=s->get_tok_str();

			s->get_token();
			if(s->get_tok_type()!=GYS_NAME && s->get_tok_type()!=GYS_CONST)
			{
				s->error("func function name getting: variable or constant token expected: ", s->get_tok_str());
				return GYS_ret_val();
			}

			pname=s->get_tok_str();

			// check and add
			if( pnames_set.find(pname)==pnames_set.end() && fparams.size()<fparams.max_size()
				&& pnames_set.size()<pnames_set.max_size())
			{
				fparams.push_back(make_pair(ptype, pname)); // add param
				pnames_set.insert(pname); // add param to the set
			}
			else
			{
				s->error("func: can't add new param to function: ", pname);
				return GYS_ret_val();
			}

			s->get_token();
			if(s->get_tok_str()==")")
				break;
			else // params are separated by commas
			{
				s->putback_tok();
				s->exam(",");
			}	
		}
	}

	// read body
	string fbody="";
	if(!s->exam("{"))
		return GYS_ret_val();

	// move to } and read code
	int more=1;

	// for correct endl putting in body reading
	long old_line=s->get_line_num();
	long new_line=old_line;
	long endl_count=0;

	while(s->get_tok_type()!=GYS_EOF && more)
	{
		s->get_token();

		new_line=s->get_line_num();
		endl_count=new_line-old_line;

		string str=s->get_tok_str();
		endl_count-=count(str.begin(), str.end(), '\n'); // don't put endls from the tokens

		old_line=new_line;

		// putting endl for correct linenum
		for(int i=0;i<endl_count;i++)
			fbody+='\n';

		if( !(s->get_tok_type()==GYS_DELIMETER && s->get_tok_str()=="}" && more==1) )
		{
			if(s->get_tok_type()==GYS_STRING)
				fbody+='\"'+s->get_tok_str()+'\"';
			else if(s->get_tok_type()==GYS_CHAR)
				fbody+='\''+s->get_tok_str()+'\'';
			else
				fbody+=s->get_tok_str()+' ';
		}

		if(s->get_tok_type()==GYS_DELIMETER && s->get_tok_str()=="{") 
			more++; //we'll miss one }
		else if(s->get_tok_type()==GYS_DELIMETER && s->get_tok_str()=="}")
			more--;
	}

	if(s->get_tok_type()==GYS_EOF)
	{
		s->fatal_error("func: EOF found while reading the body");
		return GYS_ret_val();
	}

	return GYS_function_val(ftype, fparams, fbody);
}

// postfix operator function(  [args] )[:thread]
DWORD WINAPI thread_func(LPVOID lpParam) 
{ 
	pair<GYS, istringstream*>* arg=static_cast< pair<GYS, istringstream*>* >(lpParam);

	// interpret out function script
	arg->first.disable(GYS_ESC); // don't process esc-chars again
	arg->first.interp();

	delete arg->second; // destroy  input stream
	delete arg; // destroy argument

	return 0; 
} 

GYS_ret_val op_post_func(GYS* script, GYS_ret_val l)
{
	// read func
	GYS_function_val func=l;

	istringstream* in=new istringstream(func.body);
	GYS fscript("function", func.ret_type, in);
		
	// read arguments
	fscript.add_local_var("@SELF", l); // add itself for easy recursion

	string pname;
	GYS_ret_val pval;
	for(unsigned i=0;i<func.params.size();i++)
	{
		pname=func.params[i].second; // param name
		pval=script->get_val(func.params[i].first).get_value(); //argument value
		
		fscript.add_local_var(pname, pval.get_value()); // add argument as a new var to func-script

		if( i<(func.params.size()-1) ) // check for comma
			script->exam(",");
	}

	script->exam(")");

	script->copy_params_to_sub(&fscript);

	// check for thread spec
	script->get_token();
	if(script->get_tok_str()==":" && script->get_tok_type()==GYS_DELIMETER)
	{
		if(!script->exam("thread")) // thread spec
			return GYS_ret_val(); // error

		if(func.ret_type!=gys_void_type)
			script->warning((string)gys_function_type+"():thread:  function has not void type of return value");
		
		pair<GYS, istringstream*>* param=new pair<GYS, istringstream*>(fscript, in); // will be destroyed in thread_func
	    
		DWORD dwThreadId; 
				
		return (int)CreateThread(NULL, 0, thread_func, param, 0, &dwThreadId);; // return handle
	}
	else
		script->putback_tok();

	fscript.disable(GYS_ESC); // don't process esc-chars again
	fscript.interp(); // interp func
	delete in;

	return fscript.get_ret_val();
}

void add_function_type(GYS* script)
{
	script->add_gys_type(gys_function_type, get_function_val);
	script->add_gys_func(gys_function_type, exec_type_name);

	script->add_binary_op("=", gys_function_type, gys_function_type, op_assign);
	
	script->add_post_unary_op("(", gys_function_type, op_post_func);

	script->add_gys_func("defunc", exec_defunc);
	script->add_gys_func("fn",     exec_func);
}