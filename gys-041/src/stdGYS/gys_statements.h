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
    \brief Header file for stdGYS statements.
*/

#ifndef __GYS_STATEMENTS_
#define __GYS_STATEMENTS_

#include "..\gys.h"

void add_statements(GYS* script);

// return <some_value of script return type>
GYS_ret_val exec_return(GYS* script);

// void if(<bool>) {...} [else if(<bool>) {...} [else if(<bool>) {...}...]] [else{...}]
GYS_ret_val exec_if(GYS* script);

// void while(<bool>) {...} - loop
GYS_ret_val exec_while(GYS* script);

// void do {...} while(<bool>) - loop
GYS_ret_val exec_do_while(GYS* script);

// void for(<code>; <bool-condition>; <code>) {...} - loop
GYS_ret_val exec_for(GYS* script);

#endif