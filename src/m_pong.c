/************************************************************************
 *   IRC - Internet Relay Chat, src/m_pong.c
 *   Copyright (C) 1990 Jarkko Oikarinen and
 *                      University of Oulu, Computing Center
 *
 *   See file AUTHORS in IRC package for additional names of
 *   the programmers. 
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
 *   $Id: m_pong.c 33 2010-05-10 02:46:01Z openglx $
 */
#include "m_commands.h"
#include "client.h"
#include "ircd.h"
#include "numeric.h"
#include "send.h"
#include "channel.h"
#include "irc_string.h"

#include <stdlib.h>

/*
 * m_functions execute protocol messages on this server:
 *
 *      cptr    is always NON-NULL, pointing to a *LOCAL* client
 *              structure (with an open socket connected!). This
 *              identifies the physical socket where the message
 *              originated (or which caused the m_function to be
 *              executed--some m_functions may call others...).
 *
 *      sptr    is the source of the message, defined by the
 *              prefix part of the message if present. If not
 *              or prefix not found, then sptr==cptr.
 *
 *              (!IsServer(cptr)) => (cptr == sptr), because
 *              prefixes are taken *only* from servers...
 *
 *              (IsServer(cptr))
 *                      (sptr == cptr) => the message didn't
 *                      have the prefix.
 *
 *                      (sptr != cptr && IsServer(sptr) means
 *                      the prefix specified servername. (?)
 *
 *                      (sptr != cptr && !IsServer(sptr) means
 *                      that message originated from a remote
 *                      user (not local).
 *
 *              combining
 *
 *              (!IsServer(sptr)) means that, sptr can safely
 *              taken as defining the target structure of the
 *              message in this server.
 *
 *      *Always* true (if 'parse' and others are working correct):
 *
 *      1)      sptr->from == cptr  (note: cptr->from == cptr)
 *
 *      2)      MyConnect(sptr) <=> sptr == cptr (e.g. sptr
 *              *cannot* be a local connection, unless it's
 *              actually cptr!). [MyConnect(x) should probably
 *              be defined as (x == x->from) --msa ]
 *
 *      parc    number of variable parameter strings (if zero,
 *              parv is allowed to be NULL)
 *
 *      parv    a NULL terminated list of parameter pointers,
 *
 *                      parv[0], sender (prefix string), if not present
 *                              this points to an empty string.
 *                      parv[1]...parv[parc-1]
 *                              pointers to additional parameters
 *                      parv[parc] == NULL, *always*
 *
 *              note:   it is guaranteed that parv[0]..parv[parc-1] are all
 *                      non-NULL pointers.
 */

/*
** m_pong
**      parv[0] = sender prefix
**      parv[1] = origin
**      parv[2] = destination
*/
int     m_pong(struct Client *cptr,
               struct Client *sptr,
               int parc,
               char *parv[])
{
  struct Client *acptr = NULL;
  char  *origin, *destination;

  if (parc < 2 || *parv[1] == '\0')
    {
      sendto_one(sptr, form_str(ERR_NOORIGIN), me.name, parv[0]);
      return 0;
    }

  origin = parv[1];
  destination = parv[2];

#ifdef HLC
        /* for HLC --fabulous */
        if ((IsServer(cptr)) && (parv[2]) && (IsDigit(*parv[2]))) {
          sentping = 0;
          receivedat = atoi(parv[2]);
        }
#endif

  /* Now attempt to route the PONG, comstud pointed out routable PING
   * is used for SPING.  routable PING should also probably be left in
   *        -Dianora
   * That being the case, we will route, but only for registered clients (a
   * case can be made to allow them only from servers). -Shadowfax
   */
  if (!EmptyString(destination) && irccmp(destination, me.name) != 0
                && IsRegistered(sptr))
    {
      if ((acptr = find_client(destination, NULL)) ||
          (acptr = find_server(destination)))
        sendto_one(acptr,":%s PONG %s %s",
                   parv[0], origin, destination);
      else
        {
          sendto_one(sptr, form_str(ERR_NOSUCHSERVER),
                     me.name, parv[0], destination);
          return 0;
        }
    }
  else
    {
      if (MyConnect(sptr))
      {
        sptr->flags &= ~FLAGS_PINGSENT;
#ifdef NEED_SPLITCODE
#ifdef SPLIT_PONG
        if (IsServer(sptr))
          got_server_pong = 1;
#endif
#endif
        if(MyClient(sptr) && cptr->user)
          check_idle_actions(cptr);                
      }
    }
  return 0;
}

