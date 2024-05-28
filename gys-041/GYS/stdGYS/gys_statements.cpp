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
    \brief Source file for stdGYS statements.
*/

#include "gys_statements.h"

#include "gys_bool_type.h"

// interpretes till the first unpaired odd "}" is found
// returns false if GYS_EOF found
bool interp_to_odd_cbracket(GYS* s)
{
	int more=1;

	while(s->get_tok_type()!=GYS_EOF && more)
	{
		s->get_token();
		if(s->get_tok_type()==GYS_DELIMETER && s->get_tok_str()=="}") // {} is possible
			break;
		else
			s->putback_tok();

		s->interp_next();

		s->get_token();		
		if(s->get_tok_type()==GYS_DELIMETER && s->get_tok_str()=="{") 
			more++; // we'll miss one }
		else if(s->get_tok_type()==GYS_DELIMETER && s->get_tok_str()=="}")
			more--;
		else
			s->putback_tok();
	}

	if(s->get_tok_type()==GYS_EOF && !s->is_finished()) // unexpected EOF
		return false;
	
	return true;
}

// interpret to endstr-token(GYS::exam param syntax)
// returns false if GYS_EOF found
bool interp_to(GYS* s, string endstr)
{
	do
	{
		s->get_token();		

		string cmp_to=s->get_tok_str();

		if(s->get_tok_type()==GYS_STRING)
		{
			cmp_to="\""+cmp_to+"\"";
		}
		else if(s->get_tok_type()==GYS_CHAR)
		{
			cmp_to="\'"+cmp_to+"\'";
		}
		
		if(cmp_to==endstr)   // 'endstr'-token found
			break;
		else                 // continue interpreting
			s->putback_tok();

		s->interp_next();
	}	
	while(s->get_tok_type()!=GYS_EOF);

	if(s->get_tok_type()==GYS_EOF && !s->is_finished()) // unexpected EOF
		return false;
	
	return true;
}

// move to odd endstr-token(GYS::exam param syntax)
// returns false if GYS_EOF found
// EXAMPLE: endstr is ")", endstr_pair is "(", moving to odd endstr
bool move_to_odd(GYS* s, string endstr, string endstr_pair)
{
	int more=1;

	do
	{
		s->get_token();		

		string cmp_to=s->get_tok_str();

		if(s->get_tok_type()==GYS_STRING)
		{
			cmp_to="\""+cmp_to+"\"";
		}
		else if(s->get_tok_type()==GYS_CHAR)
		{
			cmp_to="\'"+cmp_to+"\'";
		}
		
		if(s->get_tok_type()==GYS_DELIMETER && cmp_to==endstr_pair) 
			more++; // we'll miss one endstr
		else if(s->get_tok_type()==GYS_DELIMETER && cmp_to==endstr)
			more--;
	}	
	while(s->get_tok_type()!=GYS_EOF && more);

	if(s->get_tok_type()==GYS_EOF) // unexpected EOF
		return false;
	
	return true;
}

// moves to the first unpaired odd "}" (curly)
// returns false if GYS_EOF found
bool move_to_odd_cbracket(GYS* s)
{
	return move_to_odd(s, "}", "{");
}

//::::::::::::::::::::::::::::::::::::::::MAIN::::::::::::::::::::::::::::::::::::::::
// return statement
GYS_ret_val exec_return(GYS* s)
{
	// read return value
	s->set_ret_val(s->get_val(s->get_ret_type()).get_value());

	s->finish_script();

	return GYS_ret_val();
}

// if execution
/*if(<bool>)
 {
	...
 }
 [ else if(<bool>) [else if(<bool>) [else if(<bool>) ...]] { ... } ] 
 [else { ... }] 
*/
GYS_ret_val exec_if(GYS* s)
{	
	bool yes=false;
	bool exec_else=true;

condition:
	if(!s->exam("("))
		return GYS_ret_val();

	bool new_condition=s->get_val(gys_bool_type);
	yes=(!yes && new_condition);

	// there was some true condition -> no else execution
	if(yes)
		exec_else=false;

	if(!s->exam(")"))
		return GYS_ret_val();

	// body
	if(!s->exam("{"))
		return GYS_ret_val();

	// if FALSE just miss everything inside IF-body
	if(!yes)
	{
		if(!move_to_odd_cbracket(s))
		{
			s->fatal_error("IF-statement: } wasn't found");
			return GYS_ret_val();
		}
	}
	else // do the body
	{
		if(!interp_to_odd_cbracket(s))
		{
			s->fatal_error("IF-statement: } wasn't found");
			return GYS_ret_val();
		}
		else if(s->is_finished()) // e.g. after return
			return GYS_ret_val();
	}
		
	// if there any 'elses'?
	s->get_token();
	
	if(s->get_tok_type()==GYS_NAME && s->get_tok_str()=="else" ) // yes
	{
		s->get_token(); // else if?

		if( s->get_tok_type()==GYS_NAME && s->get_tok_str()=="if" ) // yes		
			goto condition;
		else // no
			s->putback_tok(); 

		if(!s->exam("{"))
			return GYS_ret_val();

		if(!exec_else) // just miss everything inside ELSE-body
		{
			if(!move_to_odd_cbracket(s))
			{
				s->fatal_error("IF-statement: } wasn't found");
				return GYS_ret_val();
			}
		}
		else // exec the body
		{
			if(!interp_to_odd_cbracket(s))
			{
				s->fatal_error("IF-statement: } wasn't found");
				return GYS_ret_val();
			}
			else if(s->is_finished()) // script is finished (e.g. after return)
				return GYS_ret_val();
		}
	}
	else // no ELSES
		s->putback_tok();

	return GYS_ret_val();
}

// while(<bool>) {}
GYS_ret_val exec_while(GYS* s)
{
	long condition_pos=s->get_pos();
	long old_line_num=s->get_line_num();
	bool yes=false;

condition:
	// condition
	if(!s->exam("("))
		return GYS_ret_val(); 
		
	yes=s->get_val(gys_bool_type);		

	if(!s->exam(")"))
		return GYS_ret_val(); 

	// body
	if(!s->exam("{"))
		return GYS_ret_val(); 

	// if FALSE just miss everything inside WHILE-body
	if(!yes)
	{
		if(!move_to_odd_cbracket(s))
		{
			s->fatal_error("WHILE-statement: } wasn't found");
			return GYS_ret_val(); 
		}
	}
	else
	{
		// exec body code
		if(!interp_to_odd_cbracket(s))
		{
			s->fatal_error("WHILE-statement: } wasn't found");
			return GYS_ret_val(); 
		}
		else if(s->is_finished()) // e.g. after return
			return GYS_ret_val();

		// next iteration - return back to the condition pos in this script
		s->set_pos(condition_pos); // go back 
		s->set_line_num(old_line_num); // correct linenum
		goto condition; // process new iteration
	}
	
	return GYS_ret_val(); 
}

// do {...} while(<bool>)
GYS_ret_val exec_do_while(GYS* s)
{
	long body_pos=s->get_pos();
	long body_line_num=s->get_line_num();
	bool yes=true;

body:
	// body
	if(!s->exam("{"))
		return GYS_ret_val(); 

	// if FALSE just miss everything inside WHILE-body
	if(!yes)
	{
		if(!move_to_odd_cbracket(s))
		{
			s->fatal_error("DO-WHILE-statement: } wasn't found");
			return GYS_ret_val(); 
		}
	}
	else
	{
		// exec body code
		if(!interp_to_odd_cbracket(s))
		{
			s->fatal_error("DO-WHILE-statement: } wasn't found");
			return GYS_ret_val(); 
		}
		else if(s->is_finished()) // e.g. after return
			return GYS_ret_val();

	}
	
	// condition
	if(!s->exam("while"))
		return GYS_ret_val(); 

	if(!s->exam("("))
		return GYS_ret_val(); 
		
	yes=s->get_val(gys_bool_type);		

	if(!s->exam(")"))
		return GYS_ret_val(); 

	if(yes)
	{
		// next iteration - return back to the body pos in this script
		s->set_pos(body_pos); // go back 
		s->set_line_num(body_line_num); // correct linenum
		goto body; // process new iteration
	}

	return GYS_ret_val(); 
}

// for([<init-code>];[<bool-condition>];[<loop-code>]) {...}
GYS_ret_val exec_for(GYS* s)
{
	if(!s->exam("("))
		return GYS_ret_val(); 

	// interp init-code
	if(!interp_to(s, ";"))
	{
		s->fatal_error("FOR-statement(init-code): ';' wasn't found");
		return GYS_ret_val(); 
	}
	else if(s->is_finished()) // e.g. after return (though it's strange)
		return GYS_ret_val();

condition:
	// read condition
	long cond_pos=s->get_pos();
	long cond_line_num=s->get_line_num();
	bool yes=true;

	s->get_token();
	if( !(s->get_tok_type()==GYS_DELIMETER && s->get_tok_str()==";") ) // e.g. for(xxx;;yyy)
	{
		s->putback_tok();
		yes=s->get_val(gys_bool_type);

		if(!s->exam(";"))
			return GYS_ret_val(); 
	}
	
	// miss loop-code
	long loop_pos=s->get_pos();
	long loop_line_num=s->get_line_num();

	if(!move_to_odd(s, ")", "("))
	{
		s->fatal_error("FOR-statement(loop-code reading): ')' wasn't found");
		return GYS_ret_val(); 
	}
	
	// body
	if(!s->exam("{"))
		return GYS_ret_val(); 

	if(!yes) // miss body if FALSE
	{
		if(!move_to_odd_cbracket(s))
		{
			s->fatal_error("FOR-statement: } wasn't found");
			return GYS_ret_val(); 
		}
	}
	else // interp body and next iteration
	{
		// exec body code
		if(!interp_to_odd_cbracket(s))
		{
			s->fatal_error("FOR-statement: } wasn't found");
			return GYS_ret_val(); 
		}
		else if(s->is_finished()) // e.g. after return
			return GYS_ret_val();

		// process next iteration
		
		// process loop-code
		s->set_pos(loop_pos); // go back 
		s->set_line_num(loop_line_num); // correct linenum

		if(!interp_to(s, ")"))
		{	
			s->fatal_error("FOR-statement(loop-code interpreting): ')' wasn't found");
			return GYS_ret_val(); 
		}
		
		// return back to the condition pos in this script
		s->set_pos(cond_pos); // go back 
		s->set_line_num(cond_line_num); // correct linenum
		goto condition; // process new iteration
	}

	return GYS_ret_val(); 
}

void add_statements(GYS* script)
{
	script->add_gys_func("return", exec_return); 
	script->add_gys_func("if",     exec_if); 
	script->add_gys_func("while",  exec_while); 
	script->add_gys_func("do",     exec_do_while); 
	script->add_gys_func("for",    exec_for); 
}
