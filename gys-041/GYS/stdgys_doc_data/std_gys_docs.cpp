/*!
  \file
  \brief This FAKE cpp-file is used to generate pretty stdgys documentation in Doxygen.
*/

/*! \mainpage

  <A HREF="http:\\gys.sourceforge.net">Homepage at gys.sourceforge.net</a>

  \section intro Introduction

  stdGYS is the example of procedural language based on GYS project.
  This documentation gives some brief description of this language.
*/

#ifdef _DO_NOT_DEFINE_THIS_BECAUSE_THIS_FILE_IS_NOT_FOR_COMPILATION_

//! Runs script and imports variables into current script.
/*!
  \param filename script filename.
*/
void import(string filename);

//! Runs the script from file.
/*!
  \param filename script filename.
*/
void run_script(string filename);

//! Runs the script from string.
/*!
  \param code code of the script to be executed.
*/
void run(string code);

//! Checks if the name exists.
/*!
  \param NAME identifier (name token) to be checked.

  \return <b>true</b> if specified name exists, <b>false</b> otherwise.
*/
bool defined(NAME);

//! Creates empty vector with specified type of its elements.
/*!
  \param content_type type of vector elements.

  \return New vector.
*/
vector make_vector(string content_type);

//! Gets type of the elements stored in the specified vector.
/*!
  \param vec specified vector.

  \return Type of the elements stored in the vector.
*/
string content_type(vector vec);

//! Gets vector size.
/*!
  \param vec specified vector.

  \return Vector size (elements number).

  \see empty()
*/
int size(vector vec);

//! Checks if the specified is empty.
/*!
  \param vec specified vector.

  \return <b>true</b> if specified vector is empty, <b>false</b> otherwise.

  \see size()
*/
bool empty(vector vec);

//! Gets first element of the vector.
/*!
  \param vec specified vector.

  \return First element of this vector.

  \see back()
*/
some_type front(vector vec);

//! Gets last element of the vector.
/*!
  \param vec specified vector.

  \return Last element of this vector.

  \see front()
*/
some_type back(vector vec);

//! Adds the new element to the end of the vector if it can contain the value of this type.
/*!
  \param vec specified vector.
  \param val new element to be inserted.

  \return New vector.

  \see insert(), pop_back()
*/
vector push_back(vector vec, some_type val);

//! Pops the last element if it exists.
/*!
  \param vec specified vector.

  \return New vector.

  \see push_back(), erase(), clear()
*/
vector pop_back(vector vec);

//! Inserts the new element if the vector can contain the value of this type.
/*!
  \param vec specified vector.
  \param pos specifies the index of the element before which the insertion will take place.
  \param val new element to be inserted.

  \return New vector.

  \see push_back()
*/
vector insert(vector vec, int pos, some_type val);

//!  Erases the element at the specified position.
/*!
  \param vec specified vector.
  \param pos specifies the index of the element to be erased.
	
  \return <b>true</b> if function succeeded, <b>false</b> otherwise.

  \see clear()
*/
vector erase(vector vec, int pos);

//!  Clears the vector.
/*!
  \param vec specified vector.
  \see erase()
*/
vector clear(vector vec);

//! Prints specified string.
/*!
  \param str string to be printed.
*/
void print(string str);

//! Prints specified string and puts new line character.
/*!
  \param str string to be printed.
*/
void println(string str);

//! Prints specified character.
/*!
  \param ch character to be printed.
*/
void putch(char ch);

//! Returns character in upper case.
/*!
  \param ch specified character to be made uppercase.

  \return Specified character in uppercase.

  \see tolower()
*/
char toupper(char ch);

//! Returns character in lower case.
/*!
  \param ch specified character to be made lowercase.

  \return Specified character in lowercase.

  \see toupper()
*/
char tolower(char ch);

//! Gets bool value from the input stream.
/*!
  \return Bool value.
*/
bool   readb();

//! Gets integer value from the input stream.
/*!
  \return Integer value.
*/
int    readi();

//! Gets double value from the input stream.
/*!
  \return Double value.
*/
double readd();

//! Gets character from the input stream.
/*!
  \return Character value.
*/
char   readc();

//! Gets string from the input stream.
/*!
  \return String value.
*/
string reads();

//! Gets character from the console.
/*!
  Also used with kbhit() function.

  \return Char value.

  \see kbhit()
*/
char getch();

//! Checks the console for keyboard input.
/*!
  \return A nonzero value if a key has been pressed, 0 - otherwise.

  \see getch()
*/
int kbhit();

double abs(double);
double exp(double);
double log(double);
double pow(double, double);
double sqrt(double);
double max(double, double);
double min(double, double);
int    mod(int, int);
double floor(double);
double ceil(double);

void srand(int seed = current_time);
int  rand();

double sin(double);
double asin(double);
double sinh(double);
double cos(double);
double acos(double);
double cosh(double);
double tan(double);
double atan(double);
double tanh(double);

//! Converts string into integer.
/*!
  \param str string to be converted.

  \return Integer value.

  \see stod()
*/
int    stoi(string str);

//! Converts string into double.
/*!
  \param str string to be converted.

  \return Double value.

  \see stoi()
*/
double stod(string str);

//! Converts character into integer (ASCII code).
/*!
  \param ch character to be converted.

  \return ASCII code of the specified character.

  \see itoc()
*/
int ctoi(char ch);

//! Converts integer (ASCII code) into char.
/*!
  \param n ASCII code.

  \return Character with specified ASCII code.

  \see ctoi()
*/
char itoc(int n);

//! Suspends the execution of the current thread for a specified interval.
/*!
  \param ms interval in milliseconds.

  \see thread_suspend()
*/
void wait(int ms);

//! Executes command line.
/*!
  \param str code for system interpreter.

  \return 0 - if function succeeded.
*/
int system(string str);

//! Destroys current thread.
void thread_end();

//! Suspends the execution of the specified thread.
/*!
  \param handle thread handle.

  \see thread_resume()
*/
void thread_suspend(int handle);

//! Resumes the execution of the specified thread.
/*!
  \param handle thread handle.

  \see thread_suspend()
*/
void thread_resume(int handle);

//--------------------------------------------------------------------------------------------

/*!
  \page stdgys_statements stdGYS statements

  These statements are available in stdGYS:
	- <tt> \ref statement_return
	- \ref statement_if
	- \ref statement_for
	- \ref statement_while
	- \ref statement_do</tt>

  <hr> \section statement_return return
  \code
return some_return_value
  \endcode
  This statement finishes current script and sets its return value. 
  It also can be used to return values from functions.

  <hr> \section statement_if if
  \code
if(bool)
{
    [CODE]
}
else if(bool)
{
    [CODE]
}
else if(bool)
{
    [CODE]
} //...
else
{
    [CODE]
}
  \endcode
This control statement allows to execute a code if a condition is true
and execute a different code otherwise.

  \attention
This statement doesn't have its own local scope.

  <hr> \section statement_for for
  \code
for([init-CODE]; [bool cond]; [loop-CODE])
{
	[CODE]
}
  \endcode

The for loop executes a code and loop-code repeatedly until
cond-expression becomes false.
Use the for statement to construct loops that must execute a specified number of times.

\attention
This statement doesn't have its own local scope.

  <hr> \section statement_while while
  \code
while(bool cond)
{
    [CODE]
}
  \endcode

The while loop executes code repeatedly until cond-expression evaluates to zero.

  \attention
This statement doesn't have its own local scope.

  <hr> \section statement_do do
  \code
do
{
    [CODE]
} 
while(bool cond)
  \endcode

The do-while statement executes a code repeatedly
until the specified termination cond-expression evaluates to zero.
The test of the termination condition is made after each execution of the loop;
therefore, a do-while loop executes one or more times,
depending on the value of the termination expression.

  \attention
This statement doesn't have its own local scope.

*/

//--------------------------------------------------------------------------------------------

/*!
  \page stdgys_types stdGYS types, operators and conversions

  \section stdgys_types Types and operators:

	- <b>bool</b>:
		- binary:
			- <tt> =
			- == != < > <= >= && || </tt>
		- prefix:
			- <tt> ! + </tt>

	- <b>double, int</b>: 
		- binary:
			- <tt> = += -= *= /=
			- \%= for int
			- + - * / 
			- \% for int
			- == != < > <= >= && || </tt>
		- postfix:
			- <tt> ++ -- </tt>
		- prefix:
			- <tt> + - ! ++ -- </tt>

	- <b>char</b>:
		- binary:
			- <tt> =
			- +
			- * (e.g. 'x'*3=="xxx")
			- == != < > <= >= </tt>

	- <b>string</b>:
		- binary:
			- <tt> =
			- +
			- * (e.g. "xyz"*3=="xyzxyzxyz")
			- *=
			- +=
			- == != < > <= >=
			- "const" [] </tt>

	- <b>vector</b>:
		- binary:
			- <tt> =
			- []
			- +
			- += </tt>
		- postfix range operator: <tt> vector(low~up) </tt>

	- <b>function</b>:
		- binary:
			- <tt> = </tt>
		- postfix:
			- <tt> ( [args] )[:thread] </tt>
\note
	This operator executes function's code in the context (gets language definition from the script),
	creates <tt>self</tt> constant "in the body" during function call,
	with <tt>thread</tt> specifier function is called in a separate thread and operator returns its handle (int value).

		- and some useful built-in functions:
			- <tt> defunc [global] ret_type [@]funcname(...) { ... } </tt>
\note
	<b> IT IS THE SAME AS </b>: <tt> [global] function [@]fname=~ret_type(...) { ... } </tt>

			- <tt> fn ret_type(...) { ... } </tt>
\note
	<b> IT IS THE SAME AS </b>: <tt> function( ~ret_type (...) { ... } ) </tt>


  \section stdgys_converters Conversions:
  \note ! - conversion with script-warning.

	- <tt> double -> int!
	- double -> bool! </tt>

	- <tt> int    -> double
	- int    -> bool! </tt>

	- <tt> bool   -> int
	- bool   -> double </tt>

	- <tt> bool   -> string
	- int    -> string
	- double -> string </tt>

	- <tt> char   -> string
	- string -> char! </tt>
	
	- <tt> vector -> bool!
	- vector -> int!
	- vector -> double!
	- vector -> char! 
	- vector -> function! </tt>

	- <tt> bool   -> vector of bool
	- int    -> vector of int
	- double -> vector of double
	- char   -> vector of char 
	- function -> vector of function </tt>

	- <tt> vector -> string! (using conversions) </tt>
			
	- <tt> string -> vector of char </tt>
*/

#endif