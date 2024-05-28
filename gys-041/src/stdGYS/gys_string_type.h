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
    \brief Header file for stdGYS string type.
*/

#ifndef __GYS_STRING_TYPE_
#define __GYS_STRING_TYPE_

#include "..\gys.h"

static const char* gys_string_type = "string"; // bool stdGYS type

class GYS_string_user_val: public GYS_user_val
{
private:
	string val;

public:
	GYS_string_user_val(string s) : val(s) { }

	string get_type_name() const {
		return gys_string_type;
	}		

	void* get_user_val() {
		return &val;
	}
};
GYS_ret_val& store(GYS_ret_val& val, string string_user_val);

GYS_ret_val get_string_val(GYS* script);

void add_string_type(GYS* script);

#endif
