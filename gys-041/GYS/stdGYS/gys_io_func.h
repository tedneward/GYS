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
    \brief Header file for stdGYS IO functions.
*/

#ifndef __GYS_FUNC_IO_
#define __GYS_FUNC_IO_

#include "..\gys.h"

void add_io_funcs(GYS* script);

//out
GYS_ret_val exec_print(GYS* script);   // void print(<string>)
GYS_ret_val exec_println(GYS* script); // void println(<string>)

GYS_ret_val exec_putch(GYS* script);   // void putch(<char>) =_putch

GYS_ret_val exec_toupper(GYS* script); // char toupper(<char>)
GYS_ret_val exec_tolower(GYS* script); // char tolower(<char>)

//in
GYS_ret_val exec_readb(GYS* script); // bool   readb() - read bool 
GYS_ret_val exec_readd(GYS* script); // double readd() - read double 
GYS_ret_val exec_readi(GYS* script); // int    readi() - read int 
GYS_ret_val exec_reads(GYS* script); // string reads() - read string
GYS_ret_val exec_readc(GYS* script); // char   readc() - read char

GYS_ret_val exec_getch(GYS* script); // char getch()
GYS_ret_val exec_kbhit(GYS* script); // int  kbhit()

// GYS_ret_val exec_match(GYS* s); // bool match(<string>, <pattern string>)

#endif