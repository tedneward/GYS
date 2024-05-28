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
    \brief The main header file.
*/

#ifndef __GYS_
#define __GYS_

#include <iostream> 
#include <string>   
#include <map>      
#include <vector>   
#include <bitset>

#include "ref_cnt_ptr.h" // smart pointers

using namespace std;

// for user
class  GYS;
class  GYS_ret_val;

// for inner using
class  GYS_binary_op;
class  GYS_pre_unary_op;
class  GYS_post_unary_op;

//:::::::::::::::::::::::::::::::::::::::::GYS return values:::::::::::::::::::::::::::::::::::::::::
/*!
  \defgroup std_types Standard types for GYS_ret_val.
*/
/*@{*/

static const char* gys_void_type      = "void"; //!< The only built-in typename for "void" type.

static const char* gys_lvalue_type    = "<lvalue>"; //!< Used for lvalue in GYS_ret_val (inner).
static const char* gys_user_type      = "<user>"; //!< Used for user_val in GYS_ret_val (inner).
/*@}*/


//! User value abstract class.
/*!
	Create derived class to store user values with specified typename.
	Use pointers to new class objects to pass user data into GYS_ret_val.
	GYS_ret_val will use redefined
	get_user_val() and get_type_name() member functions from the derived class.
	Also user has to define GYS_ret_val& store(GYS_ret_val& val, <data_type_name_in_cpp> user_data)
	function to store data in GYS_ret_val using this class... 
	More info: special docs and examples.
*/
class  GYS_user_val : public RefCntObject
{
public:
	//! Returns user data as <tt>void*</tt>.
	/*!
	  \return Pointer to user data.
	*/
	virtual void* get_user_val() =0;

	//! Returns typename of user value.
	/*!
	  \return User value typename.
	*/
	virtual string get_type_name() const =0;
};

//! Return value class.
/*!
	Objects of this class are used to store all the values used by the interpreter.
*/
class GYS_ret_val
{
private:
	GYS_ret_val* lvalue; 
	RefCntPointer<GYS_user_val> user_val;

	string type_name;

public:
	//! Creates value of void type.
	GYS_ret_val();

	//! Sets user data calling store() defined by user.
    /*!
	  \param user_val user data.
    */
	template<class T> GYS_ret_val(T user_val) {
		store(*this, user_val);
	}

	// ! Copy constructor
	GYS_ret_val(const GYS_ret_val& val);

	//! Creates lvalue.
    /*!
	  \param p pointer to source value.
    */
	GYS_ret_val(GYS_ret_val* p);

	//! Gets data from another value.
    /*!
	  \param val source value.
	  \return Reference to the new value.
    */
	GYS_ret_val& operator=(const GYS_ret_val val);

	//! Sets user data calling store() defined by user.
    /*!
	  \param user_val user data.
	  \return Reference to the new value.
    */
	template<class T> GYS_ret_val& operator=(T user_val) {
		store(*this, user_val);
		return *this;
	}

	//! Sets lvalue.
    /*!
	  \param p pointer to source value.
	  \return Reference to the new value.
    */
	GYS_ret_val& operator=(GYS_ret_val* p);

	//! "Cleans" value.
	void clean();

	//! Gets type of the current stored data.
    /*!
	  \return Current typename.
    */
	string get_type_name() const;

	//! Checks if current value is lvalue.
    /*!
	  \return <b>true</b> - if current value is lvalue, <b>false</b> - instead.
    */
	bool is_lvalue() const;

	//! Sets lvalue.
    /*!
	  \param p pointer to source value.
    */
	GYS_ret_val& set_lvalue(GYS_ret_val* p);

	//! Gets lvalue.
    /*!
	  \return "L-value" value or 0 - if there's no lvalue.
    */
	GYS_ret_val* get_lvalue() const;

	//! Sets user value, can be used with lvalue.
    /*!
	  \param user_val pointer to user value.
    */
	void set_user_val(GYS_user_val* user_val);

	//! Sets user data calling store() defined by user, cleans lvalue.
    /*!
	  \param user_val user data.
    */
	template<class T> GYS_ret_val& set_val(T user_val) {
		clean();
		store(*this, user_val);

		return *this;;
	}

	//! Gets user data itself (explicit conversion).
    /*!
	  \return User data.
    */
	template<class T> operator T() {
		void* v=get_user_val();
		return v ? *static_cast<T*>(v) : T(); // avoiding crash
	}

	//! Gets pointer to user data itself (explicit conversion).
    /*!
	  \return Pointer to user data.
    */
	template<class T> operator T*() {
		void* v=get_user_val();
		return v ? static_cast<T*>(v) : 0;
	}

	//! Gets undefined pointer (void*) to user data.
    /*!
	  \note If you need the user data (but not void*) consider using explicit conversion (operator T()).

	  \return Pointer to user data.
	  \see get_ptr_to_user_val()
    */
	void* get_user_val();

	//! Gets pointer to user value.
    /*!
	  \return Pointer to user value.
	  \see get_user_val()
    */
	GYS_user_val* get_ptr_to_user_val() const;

	//! Gets "final" value (not lvalue).
    /*!
	  \return "Final" value.
    */
	GYS_ret_val get_value() const;
};

//:::::::::::::::::::::::::::::::::::::::::Operators (not for user):::::::::::::::::::::::::::::::::::::::::
// @ - operator

// a@ b
/*!
  \internal
  \brief Binary operator specifier.
*/
class GYS_binary_op
{
private:
	string name;     // @
	string l_type;	 // l-operand's type

public:
	GYS_binary_op()
		: name(""), l_type(gys_void_type) {}

	GYS_binary_op(string opname, string ltype)
		 : name(opname), l_type(ltype) {}

	inline string get_comp_name() const // for <
	{
		return name+l_type;
	}
};

bool operator<(const GYS_binary_op l, const GYS_binary_op r); //!< For inner map implementation (<b>not for user</b>).

// @a
/*!
  \internal
  \brief Unary prefix operator specifier.
*/
class GYS_pre_unary_op
{
private:
	string name;     // @
	string ret_type; // return value type

public:
	GYS_pre_unary_op()
		: name(""), ret_type(gys_void_type) {}

	GYS_pre_unary_op(string opname, string rettype)
		: name(opname), ret_type(rettype) {}	

	inline string get_comp_name() const // for <
	{
		return name+ret_type;
	}
};

bool operator<(const GYS_pre_unary_op l, const GYS_pre_unary_op r); //!< For inner map implementation (<b>not for user</b>).

// a@
/*!
  \internal
  \brief Unary postfix operator specifier.
*/
class GYS_post_unary_op
{
private:
	string name;   // @
	string l_type; // operand's type

public:
	GYS_post_unary_op() : name(""), l_type(gys_void_type) {}

	GYS_post_unary_op(string opname, string ltype) : name(opname), l_type(ltype) {}	

	inline string get_comp_name() const  // for <
	{
		return name+l_type;
	}
};

bool operator<(const GYS_post_unary_op l, const GYS_post_unary_op r); //!< For inner map implementation (<b>not for user</b>).

//:::::::::::::::::::::::::::::::::::::::::Converter (not for user):::::::::::::::::::::::::::::::::::::::::
/*!
  \internal
  \brief Converter specifier.
*/

class GYS_converter
{
private:
	string from_type, into_type;

public:
	GYS_converter() : from_type(gys_void_type), into_type(gys_void_type) {}
	
	GYS_converter(string from, string into) : from_type(from), into_type(into) {}

	inline string get_comp_name() const // for <
	{
		return from_type+into_type;
	}
};

bool operator<(const GYS_converter l, const GYS_converter r); //!< For inner map implementation (<b>not for user</b>).

//:::::::::::::::::::::::::::::::::::::::::Functions for GYS-derived language definition:::::::::::::::::::::::::::::::::::::::::

/*!
  \defgroup proto Prototypes of user functions called by interpreter.
*/
/*@{*/
typedef GYS_ret_val(*GYS_FUNC_PTR)(GYS*); //!< Processes built-in function.

typedef GYS_ret_val(*GYS_TYPE_FUNC_PTR)(GYS*); //!< Gets raw data of some type and returns the result.

typedef GYS_ret_val(*GYS_BINARY_OP_FUNC_PTR)(GYS*, GYS_ret_val, GYS_ret_val); //!< Processes binary operator.
typedef GYS_ret_val(*GYS_PRE_UNARY_OP_FUNC_PTR)(GYS*, GYS_ret_val);           //!< Processes prefix unary operator.
typedef GYS_ret_val(*GYS_POST_UNARY_OP_FUNC_PTR)(GYS*, GYS_ret_val);          //!< Processes postfix unary operator.

typedef GYS_ret_val(*GYS_CONVERTER_PTR)(GYS*, GYS_ret_val); //!< Processes GYS-converter.

typedef void(*GYS_WARNING_FUNC_PTR)(GYS*, string, string);     //!< Warning handler.
typedef void(*GYS_ERROR_FUNC_PTR)(GYS*, string, string);       //!< Error handler.
typedef void(*GYS_FATAL_ERROR_FUNC_PTR)(GYS*, string, string); //!< Fatal error handler.
/*@}*/

//:::::::::::::::::::::::::::::::::::::::::standard error and warning handlers:::::::::::::::::::::::::::::::::::::::::

void std_gys_warning_callback(GYS* s, string str1, string str2); //!< Standard warning handler.

void std_gys_error_callback(GYS* s, string str1, string str2); //!< Standard error handler.

void std_gys_fatal_error_callback(GYS* s, string str1, string str2); //!< Standard fatal error handler.

//:::::::::::::::::::::::::::::::::::::::::Token types:::::::::::::::::::::::::::::::::::::::::
//! Token types.
enum GYS_tok_type{
	GYS_NAME,      /*!< Identifier (name). */
	GYS_CONST,     /*!< Constant name (\@name). */
	GYS_DOUBLE,    /*!< Number. */
	GYS_STRING,    /*!< String. */
	GYS_CHAR,      /*!< Character. */
	GYS_DELIMETER, /*!< Delimeter. */
	GYS_EOF        /*!< End of input. */
};

//::::::::::::::::::::::::::::::::::::::::::::Flags::::::::::::::::::::::::::::::::::::::::::::
//! State flags.
enum GYS_flag{
	GYS_ESC,                /*!< Check escape sequences in string- and char-tokens. */
	GYS_GLOBAL_VAR_OP,      /*!< Use built-in global var operator (var^.) */
};

//! GYS interpreter class.
/*!
  Main class in the libary, represents interpreter objects.
*/

class GYS
{
private:
	typedef map<string, GYS_FUNC_PTR>                                  GYS_func_map; 
	typedef map<string, GYS_TYPE_FUNC_PTR>                             GYS_type_func_map;
	typedef map< string, pair<GYS_ret_val, bool> >                     GYS_var_map;
	typedef map< GYS_binary_op, pair<string, GYS_BINARY_OP_FUNC_PTR> > GYS_binary_op_map;
	typedef map<GYS_pre_unary_op, GYS_PRE_UNARY_OP_FUNC_PTR>           GYS_pre_unary_op_map;
	typedef map<GYS_post_unary_op, GYS_POST_UNARY_OP_FUNC_PTR>         GYS_post_unary_op_map;
	typedef map<GYS_converter, GYS_CONVERTER_PTR>                      GYS_converter_map;

	string name;	
	istream* file; // input
	long line_num, error_num, warning_num;
	
	string token;          // ext. repr. of token
	GYS_tok_type tok_type; 
	double tok_val;        // token double value
	bool is_putbacked;     

	GYS_func_map          func_map; 
	GYS_type_func_map     type_func_map; 
	GYS_var_map           var_map; 
	GYS_binary_op_map     binary_op_map;
	GYS_pre_unary_op_map  pre_unary_op_map;
	GYS_post_unary_op_map post_unary_op_map;
	GYS_converter_map     converter_map;
	static GYS_var_map    global_var_map;

	string      ret_type;
	GYS_ret_val ret_val;

	bool was_finished;

	GYS_WARNING_FUNC_PTR     warning_func;
	GYS_ERROR_FUNC_PTR       error_func;
	GYS_FATAL_ERROR_FUNC_PTR fatal_error_func;

	GYS* super_script;

	bitset<8> flags;

public:
	//! Constructor.
    /*!
	  \param name_str interpreter's name.
	  \param return_type type of script's return value.
	  \param in pointer to script's input stream.

	  \see set_name(), set_input(), set_ret_type()
    */
	GYS(string name_str="unnamed", string return_type=gys_void_type, istream* in=&cin);
		
	//! Sets name.
    /*!
	  \param name_str new interpreter's name.

	  \see set_input(), set_ret_type()
    */
	void set_name(string name_str);

	//! Sets input.
    /*!
	  \attention This function totally reinitializes interpreter
	  (clears language definition, variables, handlers etc.).

	  \param in pointer to new interpreter's input.

	  \see set_name(), set_ret_type()
    */
	void set_input(istream* in);

	//! Interpretes next expression.
	/*!
	  \see interp(), finish_script()
	*/
    void interp_next();

	//! Interpretes the whole script.
	/*!
	  \see interp_next(), finish_script()
	*/
    void interp();

	//! Finishes interpretation.
	/*!
		After calling this function is_finished()==true, token type=GYS_EOF
		and interpreter checks the return value (can generate error).

	  \see interp_next(), interp()
	*/
    void finish_script();
	
	//! Sets desired type of return value.
    /*!
	  \param return_type typename of value to be returned by script.

	  \see get_ret_type()
    */
	void set_ret_type(string return_type);

	//! Gets type of return value.
    /*!
	  \return Typename of return value.

	  \see set_ret_type()
    */
	string get_ret_type() const;

	//! Sets script's return value.
    /*!
	  Interpreter will check this value's type at the end of interpretation.

	  \param val script's return value.

	  \see get_ret_val()
    */
	void set_ret_val(GYS_ret_val val);

	//! Gets script's return value.
    /*!
	  \return Script's return value.

	  \see set_ret_val()
    */
	GYS_ret_val get_ret_val() const;

	//! Enable interpreter's option.
    /*!
	  \param flag specifies the option to be set.
	  \return <b>true</b> if flag exists, <b>false</b> otherwise.

	  \see disable(), is_enabled()
    */
	bool enable(GYS_flag flag);

	//! Disables interpreter's option.
    /*!
	  \param flag specifies the option to be unset.
	  \return <b>true</b> if flag exists, <b>false</b> otherwise.

	  \see enable(), is_enabled()
    */
	bool disable(GYS_flag flag);

	//! Checks interpreter's option.
    /*!
	  \param flag specifies the option.
	  \return <b>true</b> if flag is set, <b>false</b> otherwise.

	  \see enable(), disable()
    */
	bool is_enabled(GYS_flag flag) const;

	//! Sets new warning handler.
    /*!
	  \param pf handler function.
	  
	  \see warning()
    */
	void set_warning_func(GYS_WARNING_FUNC_PTR pf);

	//! Sets new error handler.
    /*!
	  \param pf handler function.
	  
	  \see error()
    */
	void set_error_func(GYS_ERROR_FUNC_PTR pf);

	//! Sets new fatal error handler.
    /*!
	  \param pf handler function.
	  
	  \see fatal_error()
    */
	void set_fatal_error_func(GYS_FATAL_ERROR_FUNC_PTR pf);

	//! Generates warning and calls warning handler.
    /*!
	  \param str1 message string.
	  \param str2 another message string (e.g. contains some detailed information).
	  
	  \see set_warning_func()
    */
	void warning(string str1, string str2="");     // warning

	//! Generates error and calls error handler.
    /*!
	  \param str1 message string.
	  \param str2 another message string (e.g. contains some detailed information).
	  
	  \see set_error_func()
    */
	void error(string str1, string str2="");       // error

	//! Generates fatal error, calls fatal error handler, calls finish_script().
    /*!
	  \param str1 message string.
	  \param str2 another message string (e.g. contains some detailed information).
	  
	  \see set_fatal_error_func()
    */
	void fatal_error(string str1, string str2=""); // fatal error (finishes interpretation)

	//! Adds new built-in function (script command).
    /*!
	  \param ext_repr function's name (external represantation of script function).
	  \param pf user function which processes this built-in script function.

	  \return <b>true</b> if function succeeded, <b>false</b> otherwise.

	  \see remove_gys_func()
    */
	bool add_gys_func(string ext_repr, GYS_FUNC_PTR pf); 	                      

	//! Adds new type.
    /*!
	  \param type_str type name.
	  \param pf user function which gets "raw" values of this type (e.g. 123 for integer or "abc" for some string type).

	  \return <b>true</b> if function succeeded, <b>false</b> otherwise.

	  \see remove_gys_type()
    */
	bool add_gys_type(string type_str, GYS_TYPE_FUNC_PTR pf); 

	//! Adds new conversion.
    /*!
	  \param from_type source type.
	  \param into_type desired type.
	  \param pf converter (user function which performs the conversion).

	  \return <b>true</b> if function succeeded, <b>false</b> otherwise.

	  \see remove_gys_converter(), exec_converter()
    */
	bool add_gys_converter(string from_type, string into_type, GYS_CONVERTER_PTR pf); 

	//! Adds new binary operator (operand1\@operand2).
    /*!
	  Only these binary operators can be defined by user:
	  	- <tt> + -
		-  * / %
		-  := = += -= *= /= %=
		-  == != === !== < > <= >= && ||
		-  << >> <<= >>=
		-  [] </tt> 

	  \note
	    Binary operator is specified only by its name and type of its left operand,
	    so user can't define 2 different operators like type1\@type2 and type1\@type3.
		Use "[" or "[]" <tt>opname</tt> for operator [].

	  \param opname operator's name (e.g. "+" or "[]").
	  \param l_type type of the left operand.
	  \param r_type type of the right operand.
	  \param pf user function which processes this operator.
	  
	  \return <b>true</b> if function succeeded, <b>false</b> otherwise.

	  \see remove_binary_op()
    */
	bool add_binary_op(string opname, string l_type, string r_type, GYS_BINARY_OP_FUNC_PTR pf);

	//! Adds new prefix unary operator (\@operand).
    /*!
	  Only these prefix unary operators can be defined by user:
	   - <tt> !
	   - + - 
	   - ++ -- </tt>

	  \note
	    Unary operator is specified only by its name and type of its result value.
		Interpreter evaluates right operand as the value of operator's return type.

	  \param opname operator's name (e.g. "-" or "!").
	  \param ret_type type of the return value.
	  \param pf user function which processes this operator.
	  
	  \return <b>true</b> if function succeeded, <b>false</b> otherwise.

	  \see remove_pre_unary_op()
    */
	bool add_pre_unary_op(string opname, string ret_type, GYS_PRE_UNARY_OP_FUNC_PTR pf);

	//! Adds new postfix unary operator (operand\@).
    /*!
	  Only these postfix unary operators can be defined by user: 
	   - <tt> (
	   - ++ -- 
	   - . -> </tt>

	  \param opname operator's name (e.g. "++" or "(").
	  \param l_type type of the left (and the only) operand.
	  \param pf user function which processes this operator.
	  
	  \return <b>true</b> if function succeeded, <b>false</b> otherwise.

	  \see remove_post_unary_op()
    */
	bool add_post_unary_op(string opname, string l_type, GYS_POST_UNARY_OP_FUNC_PTR pf);

	//! Adds new local variable (constant).
    /*!
	  \note
	    Use "@var_name" syntax to define new constant "var_name".
	    In this case this variable's value can't be changed by interpreter during evaluations,
	    but can be changed "manually" using this function.

	  \param var_name variable's name.
	  \param val value assigned to this variable.
	  	  
	  \return <b>true</b> if function succeeded, <b>false</b> otherwise.

	  \see add_global_var(), remove_local_var()
    */
	bool add_local_var(string var_name, GYS_ret_val val);

	//! Adds new global variable (constant).
    /*!
	  All the global variables are stored in the static member table of this class
	  thus they will exist until program ends.

	  \note
	    Use "@var_name" syntax to define new constant "var_name".
	    In this case this variable's value can't be changed by interpreter during evaluations,
	    but can be changed "manually" using this function.

	  \param var_name variable's name.
	  \param val value assigned to this variable.
	  	  
	  \return <b>true</b> if function succeeded, <b>false</b> otherwise.

	  \see add_local_var(), remove_global_var()
    */
	bool add_global_var(string var_name, GYS_ret_val val);

	//! Removes built-in function (script command).
    /*!
	  \param ext_repr function's name (external represantation of script function).
	  
	  \see add_gys_func()
    */
	void remove_gys_func(string ext_repr);

	//! Removes type.
    /*!
	  \param type_str type name.
	  
	  \see add_gys_type()
    */
	void remove_gys_type(string type_str); 

	//! Removes conversion.
    /*!
	  \param from_type source type.
	  \param into_type desired type.
	  
	  \see add_gys_converter()
    */
	void remove_gys_converter(string from_type, string into_type); 

	//! Removes binary operator.
    /*!
	  \param opname operator's name (e.g. "+" or "[").
	  \param l_type type of the left operand.
	
	  \see add_binary_op()
    */
	void remove_binary_op(string opname, string l_type);

	//! Removes prefix unary operator.
    /*!
	  \param opname operator's name (e.g. "-" or "!").
	  \param ret_type type of the return value.
	
	  \see add_pre_unary_op()
    */
	void remove_pre_unary_op(string opname, string ret_type);

	//! Removes postfix unary operator.
    /*!
	  \param opname operator's name (e.g. "++" or "(").
	  \param l_type type of the left (and the only) operand.
	  
	  \see add_post_unary_op()
    */
	void remove_post_unary_op(string opname, string l_type);

	//! Removes local variable.
    /*!
	  \param var_name variable's name.

	  \see add_local_var()
    */
	void remove_local_var(string var_name);

	//! Removes global variable.
    /*!
	  \param var_name variable's name.

	  \see add_global_var()
    */
	static void remove_global_var(string var_name);

	//! Gets next token from the input.
    /*!
	  \return Token's type.
    */
	GYS_tok_type get_token();

	//! Gets next token and checks its <i>external</i> representation.
    /*!
	  \param tok desired token <i>external</i> representation.
	  
	  \return <b>true</b> if function succeeded, <b>false</b> otherwise.
    */
	bool exam(string tok);

	//! Gets next token and checks its type.
    /*!
	  \param tok_type desired token type.
	  
	  \return <b>true</b> if function succeeded, <b>false</b> otherwise.
    */
	bool exam(GYS_tok_type tok_type);

	//! Puts current token back (<i>once</i>).
    /*!
	  This function just forces lexer to skip next get_token() call.
	  It doesn't restore previous token's params.
    */
	void putback_tok();

	//! Sets cursor position in the input (<i>low level function</i>).
    /*!
	  \param new_pos new cursor position.

	  \see get_pos()
    */
	void set_pos(long new_pos);

	//! Sets linenum counter value.
    /*!
	  Doesn't change cursor position in the input.

	  \param new_linenum new line number.

	  \see get_line_num()
    */
	void set_line_num(long new_linenum);

	//! Sets superscript.
    /*!
	  For example this pointer can be used for debugging.

	  \param script pointer to the superscript.

	  \see get_super_script()
    */
	void set_super_script(GYS* script);

	//! Copies current language definition into other script and becomes its superscript.
    /*!
	  \param script pointer to the target script.
    */
	void copy_params_to_sub(GYS* script);

	//! Copies all the local variables into other script.
    /*!
	  \param script pointer to the target script.
    */
	void copy_vars_to_script(GYS* script);

	//! Reads value by evaluating the expression.
    /*!
	  \param type_name desired type of the return value.

	  \return Evaluation result.
    */
	GYS_ret_val get_val(string type_name);

	//! Gets variable value.
    /*!
	  Function looks through all the local variables and if found nothing looks through global variables.

	  \param var_name variable name.

	  \return Variable value.

	  \see get_local_var_val(), get_global_var_val() 
    */
	GYS_ret_val get_var_val(string var_name);        

	//! Gets local variable value.
    /*!
	  \param var_name local variable name.

	  \return Local variable value.

	  \see get_var_val(), get_global_var_val() 
    */
	GYS_ret_val get_local_var_val(string var_name);  

	//! Gets global variable value.
    /*!
	  \param var_name global variable name.

	  \return Global variable value.

	  \see get_var_val(), get_local_var_val() 
    */
	GYS_ret_val get_global_var_val(string var_name); 

	//! Calls specified converter.
    /*!
	  \param val value to be converted.
	  \param into_type desired type of new value.

	  \return Convertered value.

	  \see add_gys_converter()
    */
	GYS_ret_val exec_converter(GYS_ret_val val, string into_type);

	//! Gets token value as string.
    /*!
	  \return Token value.

	  \see get_tok_val()
    */
	string get_tok_str() const;

	//! Gets token type.
    /*!
	  \return Token type.
    */
	GYS_tok_type get_tok_type() const;

	//! Gets token value as double (only if token type is GYS_DOUBLE).
    /*!
	  \return Token double value.

	  \see get_tok_str()
    */
	double get_tok_val() const;

	//! Gets current name of interpreter.
    /*!
	  \return Interpreter name.

	  \see set_name()
    */
	string get_name() const;

	//! Gets current line number.
    /*!
	  \return Line number.

	  \see set_line_num()
    */
	long get_line_num() const;

	//! Gets total error+fatal_error count.
    /*!
	  \return Number of errors and fatal errors.

	  \see get_warning_num(), error(), fatal_error()
    */
	long get_error_num() const;

	//! Gets total warning count.
    /*!
	  \return Number of warnings.

	  \see get_error_num(), warning()
    */
	long get_warning_num() const;

	//! Gets current cursor position in the input (<i>low level function</i>).
    /*!
	  \return Cursor position.

	  \see set_pos()
    */
	long get_pos();

	//! Checks if the interpretation is finished.
    /*!
	  \return <b>true</b> - if interpretation is finished, <b>false</b> - otherwise.

	  \see finish_script()
    */
	bool is_finished() const;

	//! Gets pointer to superscript.
    /*!
	  \return Pointer to current superscript.

	  \see set_super_script()
    */
	GYS* get_super_script() const;

	bool is_binary_op(string op) const;
	bool is_pre_unary_op(string op) const;
	bool is_post_unary_op(string op) const;

	bool is_local_var(string var_name); 
	bool is_local_const(string var_name);
	bool is_global_var(string var_name); 
	bool is_global_const(string const_name);

	bool is_var(string var_name);
	bool is_const(string var_name);
	bool is_typename(string name);
	bool is_func(string name);
	
private:
	void init(); // total reinit

	GYS_ret_val exec_func();                                    // just try to process the function 
	GYS_ret_val exec_binary_op(GYS_ret_val lval, int cur_prec); // a@...
	GYS_ret_val exec_pre_unary_op(string ret_type);             // @...
	GYS_ret_val exec_post_unary_op(GYS_ret_val lval);           // a@
		
	// expr anal
	GYS_ret_val get_val(string type, int cur_prec); // with precedence for ops

	GYS_ret_val get_expr_1(string type);
	GYS_ret_val get_expr_2(string type);
	GYS_ret_val get_expr_3(string type);
	GYS_ret_val get_expr_4(string type);
	GYS_ret_val get_expr_5(string type);
	GYS_ret_val get_expr_6(string type);
	GYS_ret_val get_expr_7(string type);
	GYS_ret_val get_expr_8(string type);
	GYS_ret_val get_prim(string type);

	GYS_ret_val get_raw_val(string type_name);

	GYS_ret_val* get_var(string name);         
	GYS_ret_val* get_local_var(string name);   
	GYS_ret_val* get_global_var(string name);
};

#endif