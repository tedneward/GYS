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
  \brief Source file for stdGYS types (some common functions).
*/

#include "..\gys.h"


//! For implementation of explicit conversions and var/const definition.
static GYS_ret_val exec_type_name(GYS* s)
{
	string type=s->get_tok_str();

	s->get_token();
	if(s->get_tok_type()==GYS_DELIMETER && s->get_tok_str()=="(")
	{
		GYS_ret_val val=s->get_val(type);
		s->exam(")");
		return val;
	}
	else
		s->putback_tok();

	GYS_ret_val val; // value for new var
	string name;     // var name

new_var:
	s->get_token();
	if(s->get_tok_type()!=GYS_NAME && s->get_tok_type()!=GYS_CONST)
	{
		s->error("Variable or constant token expected: ", s->get_tok_str());
		return GYS_ret_val();
	}

	name=s->get_tok_str();
	
	if(!s->exam("="))
		return GYS_ret_val();

	if( ( val=s->get_val( type ) ).get_type_name()!=gys_void_type ) // not VOID
		s->add_local_var(name, val.get_value());
	
	s->get_token();
	if(s->get_tok_type()==GYS_DELIMETER && s->get_tok_str()==",")
		goto new_var;
	else
		s->putback_tok();

	return GYS_ret_val();
}

//--------------------------------------------BINARY OPERATORS--------------------------------------------
//! universal =
static GYS_ret_val op_assign(GYS* script, GYS_ret_val l, GYS_ret_val r)
{
	if(!l.is_lvalue())
	{
		script->error("'"+l.get_type_name()+"' = '"+r.get_type_name()+"' : left operand must be lvalue.");
		return l;
	}
	
	return l=r.get_value();	
}

//! universal +
template <class Left, class Right, class Return> GYS_ret_val op_add(GYS* script, GYS_ret_val l, GYS_ret_val r)
{
	return (Return)((Left)l + (Right)(r));
}

//! universal -
template<class Left, class Right, class Return> GYS_ret_val op_sub(GYS* script, GYS_ret_val l, GYS_ret_val r)
{
	return (Return)((Left)l - (Right)r);
}

//! universal *
template<class Left, class Right, class Return> GYS_ret_val op_mul(GYS* script, GYS_ret_val l, GYS_ret_val r)
{
	return (Return)((Left)l * (Right)r);
}

//! universal % (with %0 check)
template<class Left, class Right, class Return> GYS_ret_val op_mod(GYS* script, GYS_ret_val l, GYS_ret_val r)
{
	if((Right)r==0)
	{
		script->error("'"+l.get_type_name()+"' % '"+r.get_type_name()+"' : right operand is 0, return 0.");
		return (Return)0;
	}

	return (Return)((Left)l % (Right)r);
}

//! universal / (with /0 check)
template<class Left, class Right, class Return> GYS_ret_val op_div(GYS* script, GYS_ret_val l, GYS_ret_val r)
{
	if((Right)r==0)
	{
		script->error("'"+l.get_type_name()+"' / '"+r.get_type_name()+"' : right operand is 0, return 0.");
		return (Return)0;
	}

	return (Return)((Left)l / (Right)r);
}

//! universal += (with left lvalue check)
template<class Left, class Right, class Return> GYS_ret_val op_add_assign(GYS* script, GYS_ret_val l, GYS_ret_val r)
{
	if(!l.is_lvalue())
	{
		script->error("'"+l.get_type_name()+"' += '"+r.get_type_name()+"' : left operand must be lvalue.");
		return (Return)l;
	}

	return l=(Return)((Left)l + (Right)r);
}

//! universal -= (with left lvalue check)
template<class Left, class Right, class Return> GYS_ret_val op_sub_assign(GYS* script, GYS_ret_val l, GYS_ret_val r)
{
	if(!l.is_lvalue())
	{
		script->error("'"+l.get_type_name()+"' -= '"+r.get_type_name()+"' : left operand must be lvalue.");
		return (Return)l;
	}

	return l=(Return)((Left)l - (Right)r);
}

//! universal *= (with left lvalue check)
template<class Left, class Right, class Return> GYS_ret_val op_mul_assign(GYS* script, GYS_ret_val l, GYS_ret_val r)
{
	if(!l.is_lvalue())
	{
		script->error("'"+l.get_type_name()+"' *= '"+r.get_type_name()+"' : left operand must be lvalue.");
		return (Return)l;
	}

	return l=(Return)((Left)l * (Right)r);
}

//! universal /= (with left lvalue and /0 check )
template<class Left, class Right, class Return> GYS_ret_val op_div_assign(GYS* script, GYS_ret_val l, GYS_ret_val r)
{
	if(!l.is_lvalue())
	{
		script->error("'"+l.get_type_name()+"' /= '"+r.get_type_name()+"' : left operand must be lvalue.");
		return (Return)l;
	}

	if((Right)r==0)
	{
		script->error("'"+l.get_type_name()+"' /= '"+r.get_type_name()+"' : right operand is 0, return 0.");
		return (Return)0;
	}


	return l=(Return)((Left)l / (Right)r);
}

//! universal %= (with left lvalue and /0 check )
template<class Left, class Right, class Return> GYS_ret_val op_mod_assign(GYS* script, GYS_ret_val l, GYS_ret_val r)
{
	if(!l.is_lvalue())
	{
		script->error("'"+l.get_type_name()+"' %= '"+r.get_type_name()+"' : left operand must be lvalue.");
		return (Return)l;
	}

	if((Right)r==0)
	{
		script->error("'"+l.get_type_name()+"' %= '"+r.get_type_name()+"' : right operand is 0, return 0.");
		return (Return)0;
	}


	return l=(Return)((Left)l % (Right)r);
}

//! universal ==
template<class Left, class Right, class Return> GYS_ret_val op_equal(GYS* script, GYS_ret_val l, GYS_ret_val r)
{
	return (Return)((Left)l == (Right)r);
}

//! universal !=
template<class Left, class Right, class Return> GYS_ret_val op_not_equal(GYS* script, GYS_ret_val l, GYS_ret_val r)
{
	return (Return)((Left)l != (Right)r);
}

//! universal >
template<class Left, class Right, class Return> GYS_ret_val op_greater(GYS* script, GYS_ret_val l, GYS_ret_val r)
{
	return (Return)((Left)l > (Right)r);
}

//! universal <
template<class Left, class Right, class Return> GYS_ret_val op_less(GYS* script, GYS_ret_val l, GYS_ret_val r)
{
	return (Return)((Left)l < (Right)r);
}

//! universal >=
template<class Left, class Right, class Return> GYS_ret_val op_greater_or_equal(GYS* script, GYS_ret_val l, GYS_ret_val r)
{
	return (Return)((Left)l >= (Right)r);
}

//! universal <=
template<class Left, class Right, class Return> GYS_ret_val op_less_or_equal(GYS* script, GYS_ret_val l, GYS_ret_val r)
{
	return (Return)((Left)l <= (Right)r);
}

//! universal &&
template<class Left, class Right, class Return> GYS_ret_val op_and(GYS* script, GYS_ret_val l, GYS_ret_val r)
{
	return (Return)((Left)l && (Right)r);
}

//! universal ||
template<class Left, class Right, class Return> GYS_ret_val op_or(GYS* script, GYS_ret_val l, GYS_ret_val r)
{
	return (Return)((Left)l || (Right)r);
}

//--------------------------------------------PREFIX OPERATORS--------------------------------------------
//! universal unary +
template<class Right> GYS_ret_val op_plus_unar(GYS* script, GYS_ret_val r)
{
	return (Right)r;
}

//! universal unary -
template<class Right> GYS_ret_val op_minus_unar(GYS* script, GYS_ret_val r)
{
	return (-(Right)r);
}

//! universal unary !
template<class Right> GYS_ret_val op_not(GYS* script, GYS_ret_val r)
{
	return (Right)(!(Right)r);
}

//! universal prefix ++ (with lvalue check)
template<class Right> GYS_ret_val op_pre_inc_unar(GYS* script, GYS_ret_val r)
{
	if(!r.is_lvalue())
	{
		script->error("++"+r.get_type_name()+" : right operand must be lvalue.");
		return r;
	}

	return r=((Right)r+1);
}

//! universal prefix -- (with lvalue check)
template<class Right> GYS_ret_val op_pre_dec_unar(GYS* script, GYS_ret_val r)
{
	if(!r.is_lvalue())
	{
		script->error("--"+r.get_type_name()+" : right operand must be lvalue.");
		return r;
	}

	return r=((Right)r-1);
}

//--------------------------------------------POSTFIX OPERATORS--------------------------------------------
//! universal post ++
template<class Left> GYS_ret_val op_post_inc(GYS* script, GYS_ret_val l)
{
	if(!l.is_lvalue())
	{
		script->error(l.get_type_name()+"++ : left operand must be lvalue.");
		return l;
	}

	GYS_ret_val l_original=l;
	l=(Left)l+1;

	return l_original;
}

//! universal post --
template<class Left> GYS_ret_val op_post_dec(GYS* script, GYS_ret_val l)
{
	if(!l.is_lvalue())
	{
		script->error(l.get_type_name()+"-- : left operand must be lvalue.");
		return l;
	}

	GYS_ret_val l_original=l;
	l=(Left)l-1;

	return l_original;
}