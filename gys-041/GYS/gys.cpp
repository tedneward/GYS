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
  \page notes Release notes
  \version 0.4.1 - Alpha
  \author Govoruschenko Yuri Y.
*/

/*!
  \file
  \brief The main source file.

  \todo
	- fix "@VECTOR-from-Host access" error
	- finish tutorial
	- ...
	- try to change vector and function definition syntax
	- add default user val (GYS_user_val)?
	- add hash table instead of map (STLport)
	- putback linenum too for better error tracing?
	- don't force user to write type-funcs for every type
	- write wrapper for classes and functions
	- optimize GRV using in GYS
	- allow user to set operators for almost any token (e.g. "mod", "is", "is_equal_to")
	- add unary postfix [ like ( , ~ binary operator like ==?
	- add multilingual support
	- remove line counter, instead calculate it using current pos
	- add another error management

  \bug
	- if user defines vector constant from host program it can't be accessed correctly in script
	(that's why ARGV in this version is NOT a constant)
*/

/*!
  \page news What's new in this release

  	- fixed: project settings
	- fixed: std interpreter code
	- fixed: main.gys code

<hr>
*/

/*!
  \page interp_notes Interpreter notes

  \section delimeters Delimeters supported by lexical analyzer:
  	- <tt> == += -= *= /= %= != <= >= <<= >>=
	-  := === !==  << >>
	-  , ; : ( ) { } [ ] < > + - * / % ! =
	-  ++ --
	-  && || 
	-  ^ # ~ 
	-  . -></tt>

  \section op_prec Precedence of operations:
	-# <i>LOWEST:</i><tt> := = += -= *= /= \%= <<= >>=
	-# ||  
	-# && 
	-# === == != !==
	-# < > <= >= 
	-# << >>
	-# + - 
	-# * / % 
	-# <i>HIGHEST:</i></tt>
		- <tt> (expr) - parentheses
		- built-in function call
		- var var^
		- binary []
		- prefix unary
		- postfix unary</tt>
*/

/*! \mainpage

  <A HREF="http:\\gys.sourceforge.net">Homepage at gys.sourceforge.net</a>

  \section intro Introduction

%GYS is the free scripting engine which helps to create the embedded interpreter of your own language.

The interpreter can be called within application
to execute the script according to the designed language definition and functionality.
Developer can define next language elements: 
 - types
 - operators (binary, prefix and postfix unary)
 - conversions between values of different types
 - built-in functions (commands)

Mainly this library can be used to create:
 - embedded scripting language for your application
 - standalone interpreter of your own language

 \section interp Interpreter

%GYS has built-in lexical and expression analyzer, but during interpretation it calls different user code
for executing commands, operators, conversions and value getting.
The interpreter is fully runtime - it reads code and interprets it at once. 
It doesn't generate any intermediate code.

  \section restricts Restrictions

Some language definition elements are built-in and can't be changed by user yet:
 - expression evaluator
 - operators' token set and precedence in expressions
 - lexical analyzer
*/

#include <cctype>   // for isdigit(), etc
#include <cmath>    // for HUGE_VAL

#include "gys.h"

GYS::GYS_var_map GYS::global_var_map; // static member must be defined

GYS::GYS(string name_str, string return_type, istream* in) : name(name_str)
{
	set_input(in);

	ret_type=return_type;
}

void GYS::init()
{
	token="";
	tok_type=GYS_EOF;
	tok_val=0;

	line_num=1;
	error_num=0;
	warning_num=0;

	is_putbacked=false;

	//clearing all the maps
	func_map.clear();
	type_func_map.clear();
	var_map.clear();
	binary_op_map.clear();
	post_unary_op_map.clear();
	pre_unary_op_map.clear();

	ret_type=gys_void_type;
	ret_val=GYS_ret_val();

	was_finished=false;

	warning_func=0;
	error_func=0;
	fatal_error_func=0;

	super_script=0;

	flags.set(); // enable everything
}

void GYS::set_name(string name_str)
{
	name=name_str;
}

void GYS::set_input(istream* in)
{
	init(); // reinit!

	file=in;
}

//interp next ONE
void GYS::interp_next()
{
	get_token();
	
	if(!was_finished)
	if(tok_type==GYS_NAME)
	{	
		if(is_func(token))     // built-in func
		{
			putback_tok();
			get_expr_1(gys_void_type);
		}
		else                        // local or global var...
		{
			GYS_ret_val* val=get_var(token);
			if(val) // check for var existence
			{
				putback_tok();
				get_expr_1(val->get_type_name());
			}
		}
	}
	else if(tok_type==GYS_DELIMETER && token=="(")
	{
		putback_tok();
		get_expr_1(gys_void_type);
	}
	else if( !(tok_type==GYS_DELIMETER && token==";") && tok_type!=GYS_EOF)
	{
		error("Can't interpret token: ", token);
	}
}

//interp ALL
void GYS::interp()
{
	do
	{
		interp_next();
	} while(tok_type!=GYS_EOF);

	finish_script();
}

void GYS::finish_script()
{
	tok_type=GYS_EOF;
	was_finished=true;

	if(ret_val.get_type_name()!=ret_type)	
		error("Script must return the value of type ", ret_type);
}

void GYS::set_ret_type(string return_type)
{
	ret_type=return_type;
}

string GYS::get_ret_type() const
{
	return ret_type;
}

void GYS::set_ret_val(GYS_ret_val val)
{
	ret_val=val;
}

GYS_ret_val GYS::get_ret_val() const
{
	return ret_val;
}

bool GYS::enable(GYS_flag flag)
{
	try	{
		flags.set(flag);
	}
	catch(out_of_range) { // error
		return false;
	}

	return true;
}

bool GYS::disable(GYS_flag flag)
{
	try	{
		flags.set(flag, false);
	}
	catch(out_of_range) { // error
		return false;
	}

	return true;
}

bool GYS::is_enabled(GYS_flag flag) const
{
	bool t;

	try	{
		t=flags.test(flag);;
	}
	catch(out_of_range) { // error
		return false;
	}

	return t;
}

void GYS::set_warning_func(GYS_WARNING_FUNC_PTR pf)
{
	warning_func=pf;
}

void GYS::set_error_func(GYS_ERROR_FUNC_PTR pf)
{
	error_func=pf;
}

void GYS::set_fatal_error_func(GYS_FATAL_ERROR_FUNC_PTR pf)
{
	fatal_error_func=pf;
}

void GYS::warning(string str1, string str2)
{
	warning_num++;

	if(warning_func)
		warning_func(this, str1, str2);
}

void GYS::error(string str1, string str2)
{
	error_num++;

	if(error_func)
		error_func(this, str1, str2);
}

void GYS::fatal_error(string str1, string str2)
{
	error_num++; 

	if(fatal_error_func)
		fatal_error_func(this, str1, str2);

	finish_script();
}

bool GYS::add_gys_func(string ext_repr, GYS_FUNC_PTR funcPtr)
{
	if(is_var(ext_repr))
	{
		error("Can't add new built-in GYS-function (name already exists): ", ext_repr);
		return false;
	}

	if(func_map.size()<func_map.max_size())
	{
		func_map[ext_repr]=funcPtr;
	}
	else
	{
		error("Can't add new built-in GYS-function (not enough memory): ", ext_repr);
		return false;
	}

	return true;
}

bool GYS::add_gys_type(string type_str, GYS_TYPE_FUNC_PTR tfuncPtr)
{
	if(is_var(type_str))
	{
		error("Can't add new GYS-type: ", type_str);
		return false;
	}

	if(type_func_map.size()<type_func_map.max_size())
	{
		type_func_map[type_str]=tfuncPtr;	
	}
	else
	{
		error("Can't add new GYS-type (not enough memory): ", type_str);
		return false;
	}

	return true;
}

bool GYS::add_gys_converter(string from_type, string into_type, GYS_CONVERTER_PTR pf)
{
	if(converter_map.size()<converter_map.max_size())
	{
		converter_map[GYS_converter(from_type, into_type)]=pf;	
	}
	else
	{
		error("Can't add new GYS-converter (not enough memory): from '"+from_type+"' into ", into_type);
		return false;
	}

	return true;
}

bool GYS::add_binary_op(string opname, string l_type, string r_type, GYS_BINARY_OP_FUNC_PTR pf)
{
	if(opname=="[]") // fix index operator 
		opname="[";

	if(!is_binary_op(opname))
	{
		error("Can't add new binary operator (wrong token): ", opname);
		return false;
	}

	if(binary_op_map.size()<binary_op_map.max_size()) {
		binary_op_map[GYS_binary_op(opname, l_type)]=make_pair(r_type, pf);
	}
	else
	{
		error("Can't add new binary operator (not enough memory): ", opname);
		return false;
	}

	return true;
}

bool GYS::add_pre_unary_op(string opname, string ret_type, GYS_PRE_UNARY_OP_FUNC_PTR pf)
{
	if(!is_pre_unary_op(opname))
	{
		error("Can't add new prefix unary operator (wrong token): ", opname);
		return false;
	}

	if(pre_unary_op_map.size()<pre_unary_op_map.max_size()) {		
		pre_unary_op_map[GYS_pre_unary_op(opname, ret_type)]=pf;
	}
	else
	{
		error("Can't add new prefix unary operator (not enough memory): ", opname);
		return false;
	}
	return true;
}

bool GYS::add_post_unary_op(string opname, string l_type, GYS_POST_UNARY_OP_FUNC_PTR pf)
{
	if(!is_post_unary_op(opname))
	{
		error("Can't add new postfix unary operator (wrong token): ", opname);
		return false;
	}

	if(post_unary_op_map.size()<post_unary_op_map.max_size()) {
		post_unary_op_map[GYS_post_unary_op(opname, l_type)]=pf;
	}
	else
	{
		error("Can't add new postfix unary operator (not enough memory): ", opname);
		return false;
	}
	return true;
}

bool GYS::add_local_var(string vname, GYS_ret_val val)
{
	bool is_const;

	// change '@'->''
	if(vname.at(0)=='@')
	{
		vname.erase(0, 1);
		is_const=true;
	}
	else
		is_const=false;

	if(is_func(vname) || is_typename(vname) || is_local_var(vname))
	{
		error("Can't add new local variable(name already exists): ", vname);
		return false;
	}
	
	if(var_map.size()<var_map.max_size())
		var_map[vname]=make_pair(val, is_const);
	else
	{
		error("Can't add new local variable(not enough memory): ", vname);
		return false;
	}

	return true;
}

bool GYS::add_global_var(string vname, GYS_ret_val val)
{
	bool is_const;

	// change '@'->''
	if(vname.at(0)=='@')
	{
		vname.erase(0, 1);
		is_const=true;
	}
	else
		is_const=false;

	if(is_func(vname) || is_typename(vname) || is_global_var(vname))
	{
		error("Can't add new global variable(name already exists): ", vname);
		return false;
	}
	
	if(global_var_map.size()<global_var_map.max_size())
		global_var_map[vname]=make_pair(val, is_const);
	else
	{
		error("Can't add new global variable(not enough memory): ", vname);
		return false;
	}

	return true;
}

void GYS::remove_gys_func(string ext_repr)
{
	func_map.erase(ext_repr);
}

void GYS::remove_gys_type(string type_str)
{
	type_func_map.erase(type_str);
}

void GYS::remove_gys_converter(string from_type, string into_type)
{
	converter_map.erase( GYS_converter(from_type, into_type) );
}

void GYS::remove_binary_op(string opname, string l_type)
{
	binary_op_map.erase( GYS_binary_op(opname, l_type) );
}

void GYS::remove_pre_unary_op(string opname, string ret_type)
{
	pre_unary_op_map.erase( GYS_pre_unary_op(opname, ret_type) );
}

void GYS::remove_post_unary_op(string opname, string l_type)
{
	post_unary_op_map.erase( GYS_post_unary_op(opname, l_type) );
}

void GYS::remove_local_var(string vname)
{
	var_map.erase(vname);
}

void GYS::remove_global_var(string vname)
{
	global_var_map.erase(vname);
}

GYS_tok_type GYS::get_token()
{
	// if putbacked - skip this get_token
	if(is_putbacked)
	{
		is_putbacked=false;
		return tok_type;
	}

	string str="";
	char ch;

	if(!file->get(ch)) // next char or EOF
	{
		token="";
		tok_val=0;
		return (tok_type=GYS_EOF);
	}

	if(ch=='\n' || ch=='\r') line_num++; // line number

	while(isspace(ch)) // skip spaces
	{
		if(!file->get(ch)) return (tok_type=GYS_EOF);

		if(ch=='\n' || ch=='\r') line_num++;  // line number
	}

	/* delimeters:
	== += -= *= /= %= != <= >=
	, ; : ( ) { } [ ] < > + - * / % ! =
	++ --
	&& ||
	^ # ~
	. ->
	:= ===
	<< >>
	<<= >>=
	*/
	if(strchr(";:,(){}[]<>=+-*&|!%^#~.", ch)) // delimeter?
	{
		tok_val=0;
		if(strchr("=+-*%!<>:", ch) && (file->peek()=='=')) // == += -= *= %= != <= >= := === !==
		{
			token=ch;
			file->get(ch);
			token+=ch;

			if( (token=="==" || token=="!=") && file->peek()=='=') // === !==
			{
				file->get(ch);
				token+=ch;
			}

			return (tok_type=GYS_DELIMETER);
		}
		else if(ch=='+' && (file->peek()=='+') ) // ++
		{
			file->get(ch);
			token="++";
			return (tok_type=GYS_DELIMETER);
		}
		else if(ch=='-' && (file->peek()=='-') ) // --
		{
			file->get(ch);
			token="--";
			return (tok_type=GYS_DELIMETER);
		}
		else if(ch=='-' && (file->peek()=='>') ) // ->
		{
			file->get(ch);
			token="->";
			return (tok_type=GYS_DELIMETER);
		}
		else if(ch=='|' && (file->peek()=='|') ) // ||
		{
			file->get(ch);
			token="||";
			return (tok_type=GYS_DELIMETER);
		}
		else if(ch=='&' && (file->peek()=='&') ) // &&
		{
			file->get(ch);
			token="&&";
			return (tok_type=GYS_DELIMETER);
		}
		else if(ch=='<' && (file->peek()=='<') ) // <<
		{
			file->get(ch);
			token="<<";

			if(file->peek()=='=') // <<=
			{
				file->get(ch);
				token="<<=";
			}

			return (tok_type=GYS_DELIMETER);
		}
		else if(ch=='>' && (file->peek()=='>') ) // >>
		{
			file->get(ch);
			token=">>";

			if(file->peek()=='=') // >>=
			{
				file->get(ch);
				token=">>=";
			}

			return (tok_type=GYS_DELIMETER);
		}
		else									//, ; : ( ) { } [ ] < > + - * / % ! ^ # ~ .
		{
			token=ch;
			return (tok_type=GYS_DELIMETER);
		}
	}
	else if(ch=='\'') //char?
	{
		tok_val=0;
		if(!file->get(ch))
		{
			fatal_error("The EOF found while reading the char");
			return (tok_type=GYS_EOF);
		}

		if(ch=='\n' || ch=='\r') line_num++;  // line number

		char next=ch;

		if(flags.test(GYS_ESC)) // if enabled
		if(ch=='\\') //special char
		{
			if( !file->get(next) )
			{
				fatal_error("The EOF found while reading the character");
				return (tok_type=GYS_EOF);
			}

			if(next=='\n' || next=='\r') line_num++;  // line number

			switch(next)
			{
				case 'n': next='\n'; break; // newline
				case 't': next='\t'; break; // horizontal tab
				case 'v': next='\v'; break; // vertical tab
				case 'b': next='\b'; break; // backspace
				case 'r': next='\r'; break; // carriage return
				case 'f': next='\f'; break; // formfeed
				case 'a': next='\a'; break; // alert
				case '0': next='\0'; break; // null
			}

		}

		str+=next;
					
		if(!file->get(ch))
		{
			fatal_error("The EOF found while reading the char");
			return (tok_type=GYS_EOF);
		}
		else if(ch!='\'')
			error("' expected while reading the char: ", string()+ch);
		
		token=str;
		return (tok_type=GYS_CHAR);
	}
	else if(ch=='"') // string?
	{
		do
		{
			if(!file->get(ch))
			{
				fatal_error("The EOF found while reading the string");
				return (tok_type=GYS_EOF);
			}

			if(ch=='\n' || ch=='\r') line_num++;  // line number

			if(ch!='"')
			{
				char next=ch;

				if(flags.test(GYS_ESC)) // if enabled
				if(ch=='\\') //special char
				{
					if( !file->get(next) )
					{
						fatal_error("The EOF found while reading the string");
						return (tok_type=GYS_EOF);
					}

					if(next=='\n' || next=='\r') line_num++;  // line number

					switch(next)
					{
						case 'n': next='\n'; break; // newline
						case 't': next='\t'; break; // horizontal tab
						case 'v': next='\v'; break; // vertical tab
						case 'b': next='\b'; break; // backspace
						case 'r': next='\r'; break; // carriage return
						case 'f': next='\f'; break; // formfeed
						case 'a': next='\a'; break; // alert
						case '0': next='\0'; break; // null
					}
				}

				str+=next;
			}
		} while(ch!='"');

		token=str;
		tok_val=0;
		return (tok_type=GYS_STRING);
	}
	else if(ch=='@') // constant token (@name)?
	{
		if(!file->get(ch))
		{
			fatal_error("The EOF found while reading the constant");
			return (tok_type=GYS_EOF);
		}

		if(!isalnum(ch) && ch!='_') //must be letter or number or both or '_'
		{
			error("Unexpected char found while reading the constant: ", string()+ch);
			if(ch=='\n' || ch=='\r') line_num++;  // line number
		}

		str+='@'; //for correct putbacking

		while(isalnum(ch) || ch=='_')
		{
			str+=ch;			
			if(!file->get(ch))
				break;
		}
	
		token=str; 
		tok_val=0;
		file->putback(ch);
		return (tok_type=GYS_CONST);
	}
	else if(isalpha(ch) || ch=='_') // name?
	{
		while(isalnum(ch) || ch=='_')
		{
			str+=ch;
			if(!file->get(ch))
				break;
		}

		token=str;
		tok_val=0; 

		file->putback(ch);
		return (tok_type=GYS_NAME);
	}
	else if(isdigit(ch)) // double?
	{
		do // int part
		{
			str+=ch;
			if(!file->get(ch))
				break;
		}
		while(isdigit(ch));
		
		if(ch=='.') // float part
		{
			do
			{
				str+=ch;
				if(!file->get(ch))
					break;
			}
			while(isdigit(ch));
		}

		// calculating tok_val
		token=str; 
		tok_val=strtod(token.c_str(), 0);

		if(tok_val==(-HUGE_VAL) || tok_val==(HUGE_VAL)) // error check
			error("Too big number, can't represent it: ", token);

		file->putback(ch);
		return (tok_type=GYS_DOUBLE);
	}
	else if(ch=='/') // comment?
	{
		file->get(ch);

		if(ch=='/') // -> ......... //COMMENT
		{
			do
			{
				if(!file->get(ch)) return (tok_type=GYS_EOF); //what if the comment is at the last line?
			}
			while(ch!='\n');

			if(ch=='\n' || ch=='\r') line_num++;  // line number

			return get_token();
		}
		else if(ch=='*') // -> ......... /*COMMENT*/ .............
		{
			bool is_checking=true;
			while(is_checking)
			{
				do
				{
					if(!file->get(ch))
					{
						fatal_error("The EOF found while checking the comment '/**/'");
						return (tok_type=GYS_EOF);
					}
					if(ch=='\n' || ch=='\r') line_num++;  // line number
				}
				while(ch!='*');
	
				if(!file->get(ch))
				{
					fatal_error("The EOF found while checking the comment '/**/'");
					return (tok_type=GYS_EOF);
				}
				
				if(ch=='\n' || ch=='\r') line_num++;  // line number
	
				if(ch=='/') is_checking=false;
			}
			return get_token();
		}
		else // / /=
		{
			tok_val=0;

			token='/';

			if(ch=='=')
				token+=ch; // /=
			else
				file->putback(ch); // /

			return (tok_type=GYS_DELIMETER);
		}
	}
	else
	{
		token="";
		tok_val=0;
		error("Unexpected character: ", string()+ch);	
	}
}

bool GYS::exam(string tok)
{
	get_token();
	string cmp_to=token;

	if(tok_type==GYS_STRING)
	{
		cmp_to="\""+cmp_to+"\"";
	}
	else if(tok_type==GYS_CHAR)
	{
		cmp_to="\'"+cmp_to+"\'";
	}

	if(tok!=cmp_to)
	{
		if(tok_type==GYS_EOF) 
			cmp_to="*EOF*"; // for readability

		error("Expected '"+tok+"' before '"+cmp_to+"'");
		return false;
	}

	return true;
}

bool GYS::exam(GYS_tok_type type)
{
	if(get_token()!=type)
	{
		if(type==GYS_NAME)
			error("Name token expected: ", token);
		else if(type==GYS_DELIMETER)
			error("Delimeter token expected: ", token);
		else if(type==GYS_STRING)
			error("String token expected: ", token);
		else if(type==GYS_CHAR)
			error("Char token expected: ", token);
		else if(type==GYS_CONST)
			error("Constant token expected: ", token);
		else if(type==GYS_DOUBLE)
			error("Number token expected: ", token);
		else if(type==GYS_EOF)
			error("EOF token expected: ", token);
		else
			error("Syntax error: ", token);

		return false;
	}

	return true;
}

void GYS::putback_tok()
{
	if(tok_type==GYS_EOF)
			return;

	is_putbacked=true;
}

void GYS::set_pos(long new_pos)
{
	file->clear();
	file->seekg(new_pos);
	is_putbacked=false;
}

void GYS::set_line_num(long new_line_num)
{
	line_num=new_line_num;
}

void GYS::set_super_script(GYS* s)
{
	super_script=s;
}

void GYS::copy_params_to_sub(GYS* s)
{
	s->func_map=func_map;
	s->type_func_map=type_func_map;
	s->binary_op_map=binary_op_map;
	s->pre_unary_op_map=pre_unary_op_map;
	s->post_unary_op_map=post_unary_op_map;
	s->converter_map=converter_map;

	s->warning_func=warning_func;
	s->error_func=error_func;
	s->fatal_error_func=fatal_error_func;

	s->set_super_script(this);
}

void GYS::copy_vars_to_script(GYS* s)
{
	(*s).var_map.insert(var_map.begin(), var_map.end());
}

GYS_ret_val GYS::get_val(string type_name) 
{
	return exec_converter(get_expr_1(type_name), type_name);
}

GYS_ret_val GYS::get_var_val(string name) // local or global
{
	GYS_var_map::iterator var_iter; 
	var_iter = var_map.find(name); 
	if(var_iter!=var_map.end()) // local
	{
		return (*var_iter).second.first; // value
	}
	else // global
	{
		GYS_var_map::iterator global_var_iter; 

		global_var_iter = global_var_map.find(name); 
	
		if(global_var_iter!=global_var_map.end())
			return (*global_var_iter).second.first; // value
		else
			error("Unknown variable: ", name); // error
	}

	return GYS_ret_val();
}

GYS_ret_val GYS::get_local_var_val(string name) // only local
{	
	GYS_var_map::iterator var_iter; 
	var_iter = var_map.find(name); 
	if(var_iter!=var_map.end())
		return (*var_iter).second.first; 
	else 
		error("Unknown local variable: ", name);
	
	return GYS_ret_val();
}

GYS_ret_val GYS::get_global_var_val(string name) //only global
{
	GYS_var_map::iterator global_var_iter; 
	global_var_iter = global_var_map.find(name); 
	
	if(global_var_iter!=global_var_map.end())
		return (*global_var_iter).second.first;
	else
		error("Unknown global variable: ", name);

	return GYS_ret_val();
}

string GYS::get_tok_str() const
{
	return token;
}

GYS_tok_type GYS::get_tok_type() const
{
	return tok_type;
}

double GYS::get_tok_val() const
{
	return tok_val;
}

string GYS::get_name() const
{
	return name;
}

long GYS::get_line_num() const
{
	return line_num;
}

long GYS::get_error_num() const
{
	return error_num;
}

long GYS::get_warning_num() const
{
	return warning_num;
}


long GYS::get_pos()
{
	return file->tellg();
}

bool GYS::is_finished() const
{
	return was_finished;
}

GYS* GYS::get_super_script()const
{
	return super_script;
}

bool GYS::is_binary_op(string op) const
{
	if(op=="+"
	|| op=="-"
	|| op=="*"
	|| op=="/"
	|| op=="%"
	|| op=="="
	|| op=="+="
	|| op=="-="
	|| op=="*="
	|| op=="/="
	|| op=="%="
	|| op=="=="
	|| op=="!="
	|| op=="<"
	|| op==">"
	|| op=="<="
	|| op==">="
	|| op=="&&"
	|| op=="||"
	|| op=="["
	|| op==":="
	|| op=="==="
	|| op=="!=="
	|| op=="<<"
	|| op==">>"
	|| op=="<<="
	|| op==">>="
	)
		return true;
	else
		return false;
}

bool GYS::is_pre_unary_op(string op) const
{
	if(op=="+"
	|| op=="-"
	|| op=="!"
	|| op=="++"
	|| op=="--")
		return true;
	else
		return false;
}

bool GYS::is_post_unary_op(string op) const
{
	if(op=="++"
	|| op=="--"
	|| op=="("
	|| op=="."
	|| op=="->")	
		return true;
	else
		return false;
}

bool GYS::is_local_var(string var_name)
{
	// if @xxx change it to xxx
	if(var_name.at(0)=='@')
		var_name.erase(0, 1); // erase first char
	
	GYS_var_map::iterator var_iter;
	var_iter = var_map.find(var_name); 
	if(var_iter!=var_map.end())
		return true;
	else
		return false;
}

bool GYS::is_local_const(string var_name)
{
	GYS_var_map::iterator var_iter;
	var_iter = var_map.find(var_name); 
	if(var_iter!=var_map.end()) // find var
	{
		if(var_iter->second.second) // check const flag 
			return true;
		else
			return false;
	}
	else
		return false;
}

bool GYS::is_global_var(string var_name)
{
	// if @xxx change it to xxx
	if(var_name.at(0)=='@')
		var_name.erase(0, 1); // erase first char

	GYS_var_map::iterator global_var_iter;
	global_var_iter = global_var_map.find(var_name); 
	if(global_var_iter!=global_var_map.end())
		return true;
	else
		return false;
}

bool GYS::is_global_const(string var_name)
{
	GYS_var_map::iterator global_var_iter;
	global_var_iter = global_var_map.find(var_name); 
	if(global_var_iter!=global_var_map.end()) // find var
	{
		if(global_var_iter->second.second) // check const flag 
			return true;
		else
			return false;
	}
	else
		return false;
}

bool GYS::is_var(string var_name)
{
	return (is_local_var(var_name) || is_global_var(var_name));
}

bool GYS::is_const(string var_name)
{
	return (is_local_const(var_name) || is_global_const(var_name));
}

bool GYS::is_typename(string name)
{
	GYS_type_func_map::iterator type_func_iter;

	type_func_iter = type_func_map.find(name); 
	
	if(type_func_iter==type_func_map.end())
		return false; // not found
	
	return true;
}

bool GYS::is_func(string name)
{
	GYS_func_map::iterator func_iter;

	func_iter = func_map.find(name); 

	if(func_iter==func_map.end())
		return false; // not found
	
	return true;
}

GYS_ret_val GYS::exec_func()
{
	GYS_func_map::iterator func_iter;

	func_iter = func_map.find(token); 
	if(func_iter!=func_map.end())
		return (*func_iter).second(this);
	else
	{
		error("Undefined function: ", token); // not found
		return GYS_ret_val(); // void
	}
}

GYS_ret_val GYS::exec_converter(GYS_ret_val v, string into_type)
{
	if(v.get_type_name()==into_type) // nothing to convert
		return v;

	GYS_converter_map::iterator converter_iter;

	converter_iter = converter_map.find(GYS_converter(v.get_type_name(), into_type)); 
	if(converter_iter!=converter_map.end())
		return (*converter_iter).second(this, v); // call converter
	else // error
	{
		error("Undefined GYS-conversion: from '"+v.get_type_name()+"' into ", into_type);
		return GYS_ret_val();
	}	
}

GYS_ret_val GYS::exec_binary_op(GYS_ret_val lval, int cur_prec)
{	
	string opname=token;

	// find func for this operator
	GYS_binary_op_map::iterator b_op_iter = binary_op_map.find( GYS_binary_op(opname, lval.get_type_name()) ); 

	if(b_op_iter!=binary_op_map.end())
	{
		// get right operand
		GYS_ret_val rval=get_val( /*type-->*/ (*b_op_iter).second.first, cur_prec);

		return (*b_op_iter).second.second(this, lval, rval);// processing operator
	}
	else // error
	{
		error("Binary operator is not defined: ", lval.get_type_name()+opname+"..."); // error - not found
		return GYS_ret_val();
	}
}

GYS_ret_val GYS::exec_pre_unary_op(string ret_type)
{
	string opname=token;
	GYS_pre_unary_op_map::iterator pre_unary_iter;

	// find func for this operator
	pre_unary_iter = pre_unary_op_map.find( GYS_pre_unary_op(opname, ret_type) ); 

	if(pre_unary_iter!=pre_unary_op_map.end())
	{
		// rtype=ret_type!
		GYS_ret_val rval=get_val(ret_type);     // get right operand
		
		GYS_ret_val result=(*pre_unary_iter).second(this, rval); // processing operator
		if(result.get_type_name()==ret_type) // ok
			return result;
		else                                 // error
		{
			error("Prefix unary operator '"+opname+"' doesn't return '"+ret_type+"' : ", result.get_type_name());
			return GYS_ret_val();
		}
	}
	else // error
	{
		error("Prefix unary operator is not defined: ", opname+" (returns "+ret_type+")"); // not found
		return GYS_ret_val();
	}
}

GYS_ret_val GYS::exec_post_unary_op(GYS_ret_val lval)
{
	string opname=token;
	GYS_post_unary_op_map::iterator post_unar_iter;

	// find func for this operator
	post_unar_iter = post_unary_op_map.find( GYS_post_unary_op(opname, lval.get_type_name()) ); 

	if(post_unar_iter!=post_unary_op_map.end())
	{
		GYS_ret_val result=(*post_unar_iter).second(this, lval); // processing operator
		return result;		
	}
	else // error
	{
		error("Postfix unary operator is not defined: ", lval.get_type_name()+opname); // not found
		return GYS_ret_val();
	}
}

GYS_ret_val GYS::get_val(string type, int cur_prec) 
{
	GYS_ret_val result;
	switch(cur_prec)
	{
	case 0:
		result=get_expr_1(type);
		break;
	case 1:
		result=get_expr_2(type);
		break;
	case 2:
		result=get_expr_3(type);
		break;
	case 3:
		result=get_expr_4(type);
		break;
	case 4:
		result=get_expr_5(type);
		break;
	case 5:
		result=get_expr_6(type);
		break;
	case 6:
		result=get_expr_7(type);
		break;
	case 7:
		result=get_expr_8(type);
		break;
	default:
		result=get_prim(type);
	}

	return exec_converter(result, type);
}
// := = += -= *= /= %= <<= >>=
GYS_ret_val GYS::get_expr_1(string type)
{
	GYS_ret_val left=get_expr_2(type);
	GYS_ret_val result=left;
	
	for(;;)
	{
		get_token();
		
		if( tok_type==GYS_DELIMETER && (
			token==":=" ||
			token=="="  ||
			token=="+=" ||
			token=="-=" ||
			token=="*=" ||
			token=="/=" ||
			token=="%=" ||
			token=="<<=" ||
			token==">>="
			) ) 
		{
			result.clean(); // not lvalue
			result=exec_binary_op(left, 1);

			left.clean(); // not lvalue
			left=result;
		}
		else
		{
			putback_tok();

			return result;
		}
	}
}

// ||
GYS_ret_val GYS::get_expr_2(string type)
{
	GYS_ret_val left=get_expr_3(type);
	GYS_ret_val result=left;	
		
	for(;;)
	{
		get_token();
		
		if(tok_type==GYS_DELIMETER && token=="||") 
		{
			result.clean(); // not lvalue
			result=exec_binary_op(left, 2);

			left.clean(); // not lvalue
			left=result;
		}
		else
		{
			putback_tok();
			
			return result;
		}
	}
}

// &&
GYS_ret_val GYS::get_expr_3(string type)
{
	GYS_ret_val left=get_expr_4(type);
	GYS_ret_val result=left;
		
	for(;;)
	{
		get_token();
		
		if(tok_type==GYS_DELIMETER && token=="&&") 
		{
			result.clean(); // not lvalue
			result=exec_binary_op(left, 3);

			left.clean(); // not lvalue
			left=result;
		}
		else
		{
			putback_tok();
			
			return result;
		}
	}
}

// === == != !==
GYS_ret_val GYS::get_expr_4(string type)
{
	GYS_ret_val left=get_expr_5(type);
	GYS_ret_val result=left;
		
	for(;;)
	{
		get_token();
		
		if(tok_type==GYS_DELIMETER && (token=="===" || token=="==" || token=="!=" || token=="!==") ) 
		{
			result.clean(); // not lvalue
			result=exec_binary_op(left, 4);

			left.clean(); // not lvalue
			left=result;
		}
		else
		{
			putback_tok();
			
			return result;
		}
	}
}

// < > <= >=
GYS_ret_val GYS::get_expr_5(string type)
{
	GYS_ret_val left=get_expr_6(type);
	GYS_ret_val result=left;
		
	for(;;)
	{
		get_token();
		
		if(tok_type==GYS_DELIMETER && (
			token=="<" ||
			token==">" ||
			token=="<=" ||
			token==">="
			) ) 
		{
			result.clean(); // not lvalue
			result=exec_binary_op(left, 5);

			left.clean(); // not lvalue
			left=result;
		}
		else
		{
			putback_tok();
			
			return result;
		}
	}
}

// << >>
GYS_ret_val GYS::get_expr_6(string type)
{
	GYS_ret_val left=get_expr_7(type);
	GYS_ret_val result=left;

	for(;;)
	{
		get_token();
				
		if(tok_type==GYS_DELIMETER && (token=="<<" || token==">>") ) 
		{
			result.clean(); // not lvalue
			result=exec_binary_op(left, 6);

			left.clean(); // not lvalue
			left=result;
		}
		else
		{
			putback_tok();
			
			return result;
		}
	}
}

// + -
GYS_ret_val GYS::get_expr_7(string type)
{
	GYS_ret_val left=get_expr_8(type);
	GYS_ret_val result=left;

	for(;;)
	{
		get_token();
				
		if(tok_type==GYS_DELIMETER && (token=="+" || token=="-") ) 
		{
			result.clean(); // not lvalue
			result=exec_binary_op(left, 7);

			left.clean(); // not lvalue
			left=result;
		}
		else
		{
			putback_tok();
			
			return result;
		}
	}
}

// * / %
GYS_ret_val GYS::get_expr_8(string type)
{
	GYS_ret_val left=get_prim(type);
	GYS_ret_val result=left;
		
	for(;;)
	{
		get_token();
		
		if(tok_type==GYS_DELIMETER && (
			token=="*" ||
			token=="/" ||
			token=="%"
			) ) 
		{
			result.clean(); // not lvalue
			result=exec_binary_op(left, 8);

			left.clean(); // not lvalue
			left=result;
		}
		else
		{
			putback_tok();
			
			return result;
		}
	}
}

// (expr) func var var^
// unary prefix
// unary postfix
// binary []
GYS_ret_val GYS::get_prim(string type)
{
	get_token();

    GYS_ret_val left, result;

	if(tok_type==GYS_NAME && is_var(token)) // var var^
	{	
		string vname=token;
		get_token();

		if(flags.test(GYS_GLOBAL_VAR_OP) && tok_type==GYS_DELIMETER && token=="^") // only global var
		{
			if(is_global_const(vname)) // is const
			{
				left=get_global_var_val(vname); // not lvalue
			}
			else
			{
				if(GYS_ret_val* t=get_global_var(vname))
					left=t;
			}
		}
		else // var
		{
			putback_tok();

			if(is_const(vname)) // is const
			{
				left=get_var_val(vname); // not lvalue
			}
			else
			{
				if(GYS_ret_val* t=get_var(vname))
					left=t;
			}
		}
	}
	else if(tok_type==GYS_NAME) // func or conversion
	{
		left=exec_func();		
	}
	else if(tok_type==GYS_DELIMETER && is_pre_unary_op( token ) ) // prefix unary
	{
		left=exec_pre_unary_op(type);		
	}
	else if(tok_type==GYS_DELIMETER && token=="(" ) // (expr)
	{
		GYS_ret_val e=get_expr_1(type);
		get_token();
		if( !(tok_type==GYS_DELIMETER && token==")") )
		{
			error("Expression anal: ')' expected: ", token);
			return GYS_ret_val(); 
		}

		left=e;
	}
	else // "raw" data (user defines it by add_gys_type_func)
	{
		putback_tok();
		left=get_raw_val(type);
	}

	result=left;

	// postfix unary ops and binary []
	for(;;)
	{
		get_token();

		if(tok_type==GYS_DELIMETER && is_post_unary_op(token)) // postfix unary ops
		{
			result.clean(); // not lvalue
			result=exec_post_unary_op(left);

			left.clean(); // not lvalue
			left=result;
		}
		else if(tok_type==GYS_DELIMETER && token=="[") // binary []
		{
			result.clean(); // not lvalue
			result=exec_binary_op(left, 0);

			left.clean(); // not lvalue
			left=result;

			exam("]");
		}
		else
		{
			putback_tok();
			return result;
		}
	}
}

GYS_ret_val GYS::get_raw_val(string type_name) 
{
	GYS_ret_val v;
	
	GYS_type_func_map::iterator type_func_iter;

	type_func_iter = type_func_map.find(type_name); 
	
	if(type_func_iter!=type_func_map.end())
	{
		v=(*type_func_iter).second(this); // call type func
	}
	else if(type_name!=gys_void_type)
		error("Unknown type, can't process the expression of this type: ", type_name); // error
	
	return v;
}

GYS_ret_val* GYS::get_var(string name)
{
	GYS_var_map::iterator var_iter; 
	var_iter = var_map.find(name); 
	if(var_iter!=var_map.end()) //local
	{
		return &(*var_iter).second.first; // value
	}
	else // global
	{
		GYS_var_map::iterator global_var_iter; 

		global_var_iter = global_var_map.find(name); 
	
		if(global_var_iter!=global_var_map.end())
		{	
			return &(*global_var_iter).second.first; // value 
		}
		else
		{
			error("Unknown variable: ", name); // error
			return 0;
		}
	}
}

GYS_ret_val* GYS::get_local_var(string name)
{
	GYS_var_map::iterator local_var_iter; 

	local_var_iter = var_map.find(name); 
	
	if(local_var_iter!=var_map.end())
	{	
		return &(*local_var_iter).second.first; // value
	}
	else
	{
		error("Unknown local variable: ", name); // error
		return 0;
	}
}

GYS_ret_val* GYS::get_global_var(string name)
{
	GYS_var_map::iterator global_var_iter; 

	global_var_iter = global_var_map.find(name); 
	
	if(global_var_iter!=global_var_map.end())
	{	
		return &(*global_var_iter).second.first; // value
	}
	else
	{
		error("Unknown global variable: ", name); // error
		return 0;
	}
}

// for map
bool operator<(const GYS_binary_op l, const GYS_binary_op r)
{
	return l.get_comp_name()<r.get_comp_name();
}

bool operator<(const GYS_post_unary_op l, const GYS_post_unary_op r)
{
	return l.get_comp_name()<r.get_comp_name();
}

bool operator<(const GYS_pre_unary_op l, const GYS_pre_unary_op r)
{
	return l.get_comp_name()<r.get_comp_name();
}

bool operator<(const GYS_converter l, const GYS_converter r)
{
	return l.get_comp_name()<r.get_comp_name();
}

//--------------------------GYS return values-------------------------------
GYS_ret_val::GYS_ret_val()
{
	clean();
}

GYS_ret_val::GYS_ret_val(const GYS_ret_val& v)
{
	if(v.is_lvalue())
		set_lvalue(v.get_lvalue());
	else if(v.get_type_name()==gys_void_type)
		clean();
	else
		set_user_val(v.get_ptr_to_user_val());
}

void GYS_ret_val::clean()
{
	lvalue=0;
	user_val=0;
	type_name=gys_void_type;
}

GYS_ret_val& GYS_ret_val::operator=(const GYS_ret_val v)
{
	string new_type=v.get_type_name();
	
	if(type_name==gys_lvalue_type) // set lvalue
	{
		// void
		if(new_type==gys_void_type)
			type_name=gys_void_type;

		// user type
		else
			lvalue->set_user_val(v.get_ptr_to_user_val());
	}
	else // set new value
	{
		// void
		if(new_type==gys_void_type)
			type_name=gys_void_type;	

		// lvalue
		else if(v.is_lvalue())
			set_lvalue(v.get_lvalue());
	
		// user type
		else
			set_user_val(v.get_ptr_to_user_val());
	}

	return *this;
}

string GYS_ret_val::get_type_name() const
{
	if(type_name==gys_user_type)
		return user_val->get_type_name();
	else if(type_name==gys_lvalue_type)
		return lvalue->get_type_name();
	else
		return type_name;
}

bool GYS_ret_val::is_lvalue() const
{
	return type_name==gys_lvalue_type ? true : false;
}

GYS_ret_val::GYS_ret_val(GYS_ret_val* p)
{
	set_lvalue(p);
}

GYS_ret_val& GYS_ret_val::set_lvalue(GYS_ret_val* p)
{
	user_val=0;

	lvalue=p;
	type_name=gys_lvalue_type;

	return *this;
}

GYS_ret_val& GYS_ret_val::operator=(GYS_ret_val* p)
{
	if(type_name==gys_lvalue_type)
		lvalue->set_lvalue(p);
	else
		set_lvalue(p);

	return *this;
}

GYS_ret_val* GYS_ret_val::get_lvalue() const
{
	if(type_name==gys_lvalue_type)
		return lvalue;
	else
		return 0;
}

void GYS_ret_val::set_user_val(GYS_user_val* user_v)
{
	if(type_name==gys_lvalue_type)
		lvalue->set_user_val(user_v);
	else
	{
		type_name=gys_user_type;
		user_val=user_v;
	}
}

void* GYS_ret_val::get_user_val()
{
	if(type_name==gys_user_type)
		return user_val->get_user_val();
	else if(type_name==gys_lvalue_type)
		return lvalue->get_user_val();
	else
		return 0;
}

GYS_user_val* GYS_ret_val::get_ptr_to_user_val() const
{
	if(type_name==gys_user_type)
		return user_val;
	else if(type_name==gys_lvalue_type)
		return lvalue->get_ptr_to_user_val();
	else
		return 0;
}

GYS_ret_val GYS_ret_val::get_value() const
{
	if(type_name==gys_lvalue_type)
		return lvalue->get_value();
	else
		return *this;
}

//-------------------------------------------

void std_gys_warning_callback(GYS* s, string str1, string str2)
{
	GYS* ps=s;

	// writing full path through all the scripts
	while(ps)
	{
		cerr<<ps->get_name()<<"("<<ps->get_line_num()<<") <= ";
		ps=ps->get_super_script();		
	}

	if(str2!="")
		cerr<<"WARNING : "<<str1<<"'"<<str2<<"'"<<endl;
	else
		cerr<<"WARNING : "<<str1<<endl;
}

void std_gys_error_callback(GYS* s, string str1, string str2)
{
	GYS* ps=s;

	// writing full path through all the scripts
	while(ps)
	{
		cerr<<ps->get_name()<<"("<<ps->get_line_num()<<") <= ";
		ps=ps->get_super_script();		
	}

	if(str2!="")
		cerr<<"ERROR : "<<str1<<"'"<<str2<<"'"<<endl;
	else
		cerr<<"ERROR : "<<str1<<endl;
}

void std_gys_fatal_error_callback(GYS* s, string str1, string str2)
{
	GYS* ps=s;

	// writing full path through all the scripts
	while(ps)
	{
		cerr<<ps->get_name()<<"("<<ps->get_line_num()<<") <= ";
		ps=ps->get_super_script();		
	}

	if(str2!="")
		cerr<<"FATAL ERROR : "<<str1<<"'"<<str2<<"'"<<endl;
	else
		cerr<<"FATAL ERROR : "<<str1<<endl;
}