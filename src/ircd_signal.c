/************************************************************************
 *   IRC - Internet Relay Chat, src/ircd_signal.c
 *   Copyright (C) 1990 Jarkko Oikarinen and
 *                      University of Oulu, Computing Center
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
 * $Id: ircd_signal.c 33 2010-05-10 02:46:01Z openglx $
 */
#include "ircd_signal.h"
#include "ircd.h"         /* dorehash */
#include "restart.h"      /* server_reboot */
#include "s_log.h"
#include "send.h"         /* flush_connections */

#include <signal.h>
#include <stdlib.h>

/*
 * dummy_handler - don't know if this is really needed but if alarm is still
 * being used we probably will
 */ 
static void dummy_handler(int sig)
{
  /* Empty */
}

#if 0
/*
 * sigsegv_handler - segfault handling
 */
static void sigsegv_handler(int sig)  
{
  flush_connections(0);
  irclog(L_CRIT, "Server terminating on SIGSEGV");
  abort();
}
#endif

/*
 * sigterm_handler - exit the server
 */
static void sigterm_handler(int sig)  
{
  flush_connections(0);
  irclog(L_CRIT, "Server killed By SIGTERM");
  exit(-1);
}
  
/* 
 * sighup_handler - reread the server configuration
 */
static void sighup_handler(int sig)
{
  dorehash = 1;
}

/*
 * sigint_handler - restart the server
 */
static void sigint_handler(int sig)
{
  static int restarting = 0;

  irclog(L_WARN, "Server Restarting on SIGINT");
  if (restarting == 0) {
    restarting = 1;
    server_reboot();
  }
}

/*
 * setup_signals - initialize signal handlers for server
 */
void setup_signals()
{
  struct sigaction act;

  act.sa_flags = 0;
  act.sa_handler = SIG_IGN;
  sigemptyset(&act.sa_mask);
  sigaddset(&act.sa_mask, SIGPIPE);
  sigaddset(&act.sa_mask, SIGALRM);
  sigaddset(&act.sa_mask, SIGTRAP);
  
# ifdef SIGWINCH
  sigaddset(&act.sa_mask, SIGWINCH);
  sigaction(SIGWINCH, &act, 0);
# endif
  sigaction(SIGTRAP, &act, 0);
  sigaction(SIGPIPE, &act, 0);

  act.sa_handler = dummy_handler;
  sigaction(SIGALRM, &act, 0);

  act.sa_handler = sighup_handler;
  sigemptyset(&act.sa_mask);
  sigaddset(&act.sa_mask, SIGHUP);
  sigaction(SIGHUP, &act, 0);

  act.sa_handler = sigint_handler;
  sigaddset(&act.sa_mask, SIGINT);
  sigaction(SIGINT, &act, 0);

  act.sa_handler = sigterm_handler;
  sigaddset(&act.sa_mask, SIGTERM);
  sigaction(SIGTERM, &act, 0);
  
#if 0
  act.sa_handler = sigsegv_handler;
  sigaddset(&act.sa_mask, SIGSEGV);
  sigaction(SIGSEGV, &act, 0);
#endif
}


