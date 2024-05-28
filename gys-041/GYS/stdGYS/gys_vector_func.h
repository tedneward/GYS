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
    \brief Header file for stdGYS functions for vectors.
*/

#ifndef __GYS_FUNC_VECTOR_
#define __GYS_FUNC_VECTOR_

#include "..\gys.h"

void add_vector_funcs(GYS* script);

GYS_ret_val exec_make_vector(GYS* script);  // vector make_vector(<string>)

GYS_ret_val exec_content_type(GYS* script);  // string content_type(<vector>)
GYS_ret_val exec_size(GYS* script);			 // int size(<vector>)
GYS_ret_val exec_empty(GYS* script);		 // bool empty(<vector>)
GYS_ret_val exec_front(GYS* script);		 // some_type front(<vector>)
GYS_ret_val exec_back(GYS* script);			 // some_type back(<vector>)

GYS_ret_val exec_push_back(GYS* script); // vector push_back(<vector>, <some_type>)
GYS_ret_val exec_pop_back(GYS* script);  // vector pop_back(<vector>)
GYS_ret_val exec_insert(GYS* script);    // vector insert(<vector>, <int>, <some_type>)
GYS_ret_val exec_erase(GYS* script);     // vector erase(<vector>, <int>)
GYS_ret_val exec_clear(GYS* script);     // vector clear(<vector>)

#endif