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
    \brief Header file for stdGYS general functions.
*/

#ifndef __GYS_FUNC_GENERAL_
#define __GYS_FUNC_GENERAL_

#include "..\gys.h"

void add_general_funcs(GYS* script);

GYS_ret_val exec_import(GYS* script);     // void import(<string(filename)>)
GYS_ret_val exec_run_script(GYS* script); // void run_script(<string(filename)>)
GYS_ret_val exec_run(GYS* script);        // void run(<string(code)>)

GYS_ret_val exec_defined(GYS* script);    // bool defined(NAME)

GYS_ret_val exec_wait(GYS* script);    // void wait(<int in ms>) =_sleep
GYS_ret_val exec_system(GYS* script);  // void system(<string>)

#endif