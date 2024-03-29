/************************************************************************
 *   IRC - Internet Relay Chat, include/common.h
 *   Copyright (C) 1990 Armin Gruner
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 1, or (at your option)
 *   any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 * $Id: common.h 33 2010-05-10 02:46:01Z openglx $
 */
#ifndef INCLUDED_common_h
#define INCLUDED_common_h

#ifdef __CYGWIN__
#include <sys/socket.h>
#endif

#ifndef NULL
#define NULL 0
#endif

#ifdef TRUE
#undef TRUE
#endif

#ifdef FALSE
#undef FALSE
#endif

#define FALSE  0
#define TRUE   1
#define HIDEME 2

/* Blah. I use these a lot. -Dianora */
#ifdef YES
#undef YES
#endif

#define YES 1

#ifdef NO
#undef NO
#endif

#define NO  0

/* Just blindly define our own MIN/MAX macro */

#define IRCD_MAX(a, b)  ((a) > (b) ? (a) : (b))
#define IRCD_MIN(a, b)  ((a) < (b) ? (a) : (b))

#if 0
extern struct SLink *find_user_link(/* struct SLink *, struct Client * */);
#endif

/* 
 * argh where should this go? who uses it? ;-)
 */

typedef struct
{
  int mode;
  char letter;
} FLAG_ITEM;


#ifdef __CYGWIN__
#define CLOSE(x) \
  shutdown((x),SHUT_RDWR);\
  close((x));
#else
#define CLOSE(x)   close((x));
#endif
#endif /* INCLUDED_common_h */
