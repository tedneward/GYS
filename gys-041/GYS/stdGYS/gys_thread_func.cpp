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
    \brief Source file for stdGYS thread functions.
*/

#include "gys_thread_func.h"

#include "gys_int_type.h"

#define VC_EXTRALEAN // for VC only
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

GYS_ret_val exec_thread_end(GYS* s)
{
	s->exam("(");
	s->exam(")");
		
	ExitThread(1);
 
	return GYS_ret_val();
}

GYS_ret_val exec_thread_suspend(GYS* s)
{
	s->exam("(");
	
	SuspendThread( (HANDLE)((int)s->get_val(gys_int_type)) );

	s->exam(")");

	return GYS_ret_val();
}

// void thread_resume(<int>)
GYS_ret_val exec_thread_resume(GYS* s)
{
	s->exam("(");

	ResumeThread( (HANDLE)((int)s->get_val(gys_int_type)) );
		
	s->exam(")");

	return GYS_ret_val();
}

void add_thread_funcs(GYS* script)
{
	script->add_gys_func("thread_end",     exec_thread_end);
	script->add_gys_func("thread_suspend", exec_thread_suspend);
	script->add_gys_func("thread_resume",  exec_thread_resume);
}