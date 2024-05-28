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
    \brief Source file for stdGYS (functions and statements).
*/

#include "gys_func.h"

#include "gys_statements.h"
#include "gys_io_func.h"
#include "gys_math_func.h"
#include "gys_convert_func.h"
#include "gys_general_func.h"
#include "gys_thread_func.h"
#include "gys_vector_func.h"

void add_stdgys_funcs(GYS* s)
{
	add_statements(s);

	add_io_funcs(s);
	add_math_funcs(s);
	add_convert_funcs(s);
	add_general_funcs(s);
	add_thread_funcs(s);
	add_vector_funcs(s);
}