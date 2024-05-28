// test.cpp : example of GYS embedding

#include "gys.h"

#include <fstream>

class GYS_string_user_val: public GYS_user_val // objects of this class contain string data
{
private:
	string val;

public:
	GYS_string_user_val(string str) : val(str) { }

	// returns typename as we see it in scripts
	string get_type_name() const {
		return "string";
	}

	// retuns data (as void* value!)
	void* get_user_val() {
		return &val;
	}
};

GYS_ret_val& store(GYS_ret_val& val, string string_user_val)
{
	val.set_user_val( new GYS_string_user_val(string_user_val) );
	return val;
}

// gets raw string data
GYS_ret_val get_string_val(GYS* script)
{
	script->get_token(); // read next token

	if(script->get_tok_type()==GYS_STRING) // check its type
	{
		return (string)script->get_tok_str(); // return token - it is the string we need
	}
	else // error
	{
		script->error("'string' getting: string-token expected: ", script->get_tok_str()); // error
		return GYS_ret_val();// return void value
	}
}

// this function will be called to interpret 'print' name-token
GYS_ret_val exec_print(GYS* script)
{
	script->exam("("); // read next token and check it for "("

	cout << (string)script->get_val("string"); // evaluate expression to get 'string'-value

	script->exam(")"); // read next token and check it for ")"

	return GYS_ret_val(); // return "nothing" - value of type gys_void_type
}

GYS_ret_val exec_gets(GYS* script)
{
	script->exam("(");

	string str;
	cin >> str;

	script->exam(")");

	return str;
}

GYS_ret_val exec_string(GYS* s)
{
	string type="string";

	GYS_ret_val val; // value for new var
	string name; // var name

	new_var:
	s->get_token();
	if(s->get_tok_type()!=GYS_NAME)
	{
		s->error("Variable token expected: ", s->get_tok_str());
		return GYS_ret_val();
	}

	name=s->get_tok_str();

	if(!s->exam("="))
		return GYS_ret_val();

	if( ( val=s->get_val( type ) ).get_type_name()!=gys_void_type ) // not void
		s->add_local_var(name, val.get_value());

	s->get_token();
	if(s->get_tok_type()==GYS_DELIMETER && s->get_tok_str()==",")
		goto new_var;
	else
		s->putback_tok();

	return GYS_ret_val();
}

// string+string
GYS_ret_val op_add_string(GYS* script, GYS_ret_val left_operand, GYS_ret_val right_operand)
{
	return (string)left_operand + (string)right_operand;
}

int main(int argc, char* argv[])
{
	// create new script interpreter and set its name
	const char* filename="main.gys";
	GYS script(filename); // we set name for debug purposes
    	
	// load the file we want to interpret
	ifstream fin(filename);

	if(!fin) // error
	{
		cerr<<"Interpreter error: can't open the input file: "<<filename<<endl;
		return 1;
	}

	// set input for our interpreter
	script.set_input(&fin);           		

	// add error and warning handlers
	script.set_warning_func(std_gys_warning_callback);
	script.set_error_func(std_gys_error_callback);
	script.set_fatal_error_func(std_gys_fatal_error_callback);

	// --- Language definition:

	// types
	script.add_gys_type("string", get_string_val);

	// functions
	script.add_gys_func("print", exec_print);
	script.add_gys_func("gets",  exec_gets);

	// var definition statement
	script.add_gys_func("string", exec_string); 

	// binary operators
	script.add_binary_op("+", "string", "string", op_add_string);

	// interpretation
	script.interp();

	return 0;
}