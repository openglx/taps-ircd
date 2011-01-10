/*****************************************************************
 * PTlink IRCd is (C) CopyRight PTlink IRC Software 1999-2000     *
 *                http://software.pt-link.net                     *
 * This program is distributed under GNU Public License          *
 * Please read the file COPYING for copyright information.       *
 *****************************************************************
 $Id: m_silence.h 33 2010-05-10 02:46:01Z openglx $
 
 File: m_silence.h
 Desc: silence handling routines
 Author: Lamego@PTlink.net  
*/
#include "client.h"
#ifndef INCLUDE_m_silence_h
#define INCLUDE_m_silence_h
extern int      is_silenced( struct Client *,  struct Client *);
#endif
