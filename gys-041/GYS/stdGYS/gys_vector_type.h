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
    \brief Header file for stdGYS vector type.
*/

#ifndef __GYS_VECTOR_TYPE_
#define __GYS_VECTOR_TYPE_

#include "..\gys.h"

//! Vector value.
/*! 
	Defined for use with GYS_ret_val to store stdGYS vectors
	which can contain values of one type.
*/
class GYS_vector_val
{
private:
	string val_type;
	vector<GYS_ret_val> vec;

public:
	//! Constructor.
    /*!
      \param vtype type of the contained values.
    */
	GYS_vector_val(string vtype=gys_void_type);

	//! Adds the new element to the end of the vector if it can contain the value of this type.
    /*!
      \param val new element to be inserted.
      \return <b>true</b> if function succeeded, <b>false</b> otherwise.

	  \see insert(), pop_back()
    */
	bool push_back(GYS_ret_val val);

	//! Pops the last element if it exists.
    /*!
      \return <b>true</b> if function succeeded, <b>false</b> otherwise.

	  \see push_back(), erase(), clear()
    */
	bool pop_back();

	//! Inserts the new element if the vector can contain the value of this type.
    /*!
	  \param pos specifies the index of the element before which the insertion will take place.
	  \param val new element to be inserted.
      \return <b>true</b> if function succeeded, <b>false</b> otherwise.

	  \see push_back()
    */
	bool insert(unsigned int pos, GYS_ret_val val);

	//!  Erases the element at the specified position.
    /*!
	  \param pos specifies the index of the element to be erased.
	  
	  \return <b>true</b> if function succeeded, <b>false</b> otherwise.

	  \see clear()
    */
	bool erase(unsigned int pos);

	//!  Clears the vector.
    /*!
	  \see erase()
    */
	void clear();

	//! Changes vector's size and fills it with some default value.
    /*!
	  \param n new size.
	  \param def_val default element value.
      \return <b>true</b> if function succeeded, <b>false</b> otherwise.
    */
	bool resize(unsigned int n, GYS_ret_val def_val);

	//! Returns the pointer to the specified element.
    /*!
	  \param index element's position.
	  \return 0 - if failed, pointer to the element instead.
    */
	GYS_ret_val* operator [](unsigned int index);

	//! Returns the STL vector.
    /*!
	  \return The corresponding STL vector.
    */
	vector<GYS_ret_val> get_vector() const;

	//! Returns the typename of the contained values.
    /*!
	  \return The typename of the values.
    */
	string get_type_name() const;
};

static const char* gys_vector_type = "vector"; // vector stdGYS type

GYS_ret_val& store(GYS_ret_val& val, GYS_vector_val vector_user_val);

// #type                       - empty vector
// #type{v0, v1, v2, ... , vN} - initialized (with elements) vector
// #type[int]:default_val      - initialized vector with N elements (=default_val)
// #... = #vector...
GYS_ret_val get_vector_val(GYS* script);

void add_vector_type(GYS* script);

#endif