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
    \brief Header file for stdGYS conversion functions.
*/

#ifndef __GYS_FUNC_CONVERT_
#define __GYS_FUNC_CONVERT_

#include "..\gys.h"

void add_convert_funcs(GYS* script);

GYS_ret_val exec_stoi(GYS* script); // int    stoi(<string>)
GYS_ret_val exec_stod(GYS* script); // double stod(<string>)

GYS_ret_val exec_ctoi(GYS* script); // int ctoi(<char>) - char into int (ASCII)
GYS_ret_val exec_itoc(GYS* script); // char itoc(<int>) - int into char (ASCII)

#endif