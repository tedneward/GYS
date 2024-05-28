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
    \brief Header file for stdGYS math functions.
*/

#ifndef __GYS_FUNC_MATH_
#define __GYS_FUNC_MATH_

#include "..\gys.h"

void add_math_funcs(GYS* script);

//main
GYS_ret_val exec_abs(GYS* script);   // double abs(<double>) = |x|
GYS_ret_val exec_exp(GYS* script);   // double exp(<double>)
GYS_ret_val exec_log(GYS* script);   // double log(<double>)
GYS_ret_val exec_pow(GYS* script);   // double pow(<double>, <double>)
GYS_ret_val exec_sqrt(GYS* script);  // double sqrt(<double>)
GYS_ret_val exec_max(GYS* script);   // double max(<double>, <double>)
GYS_ret_val exec_min(GYS* script);   // double min(<double>, <double>)
GYS_ret_val exec_mod(GYS* script);   // int    mod(<int>, <int>) - divides and returns the remainder
GYS_ret_val exec_floor(GYS* script); // double floor(<double>)
GYS_ret_val exec_ceil(GYS* script);  // double ceil(<double>)

// pseudo-random numbers
GYS_ret_val exec_srand(GYS* script);  // void srand() = srand( (unsigned)time( NULL ) )
GYS_ret_val exec_rand(GYS* script);   // int  rand()

//trigonometry
GYS_ret_val exec_sin(GYS* script);  // double sin(<double>)
GYS_ret_val exec_asin(GYS* script); // double asin(<double>)
GYS_ret_val exec_sinh(GYS* script); // double sinh(<double>)
GYS_ret_val exec_cos(GYS* script);  // double cos(<double>)
GYS_ret_val exec_acos(GYS* script); // double acos(<double>)
GYS_ret_val exec_cosh(GYS* script); // double cosh(<double>)
GYS_ret_val exec_tan(GYS* script);  // double tan(<double>)
GYS_ret_val exec_atan(GYS* script); // double atan(<double>)
GYS_ret_val exec_tanh(GYS* script); // double tanh(<double>)

#endif