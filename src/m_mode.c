/************************************************************************
 *   IRC - Internet Relay Chat, src/m_mode.c
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
 *   $Id: m_mode.c 33 2010-05-10 02:46:01Z openglx $
 */
#include "m_commands.h"
#include "channel.h"
#include "client.h"
#include "hash.h"
#include "irc_string.h"
#include "ircd.h"
#include "numeric.h"
#include "s_user.h"
#include "send.h"
#include "struct.h"
#include "list.h"
#include "s_conf.h"

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
 * m_mode - MODE command handler
 * parv[0] - sender
 * parv[1] - channel
 */
int m_mode(struct Client *cptr, struct Client *sptr, int parc, char *parv[])
{
  struct Channel* chptr;
  static char     modebuf[MODEBUFLEN];
  static char     parabuf[MODEBUFLEN];
  char *mename = me.name;

  if(sptr->user && sptr->user->vlink)
    mename = sptr->user->vlink->name;
     
  /* Now, try to find the channel in question */
  if (parc > 1)
    {
      if( IsChanPrefix(parv[1][0]) )
        {
          /* Don't do any of this stuff at all
           * unless it looks like a channel name 
           */

          if (!check_channel_name(parv[1]))
            { 
              sendto_one(sptr, form_str(ERR_BADCHANNAME),
                         mename, parv[0], (unsigned char *)parv[1]);
              return 0;
            }

          chptr = hash_find_channel(parv[1], NullChn);
          if (!chptr)
            return user_mode(cptr, sptr, parc, parv);
        }
      else
        {
          /* if here, it has to be a non-channel name */
          return user_mode(cptr, sptr, parc, parv);
        }
    }
  else
    {
      sendto_one(sptr, form_str(ERR_NEEDMOREPARAMS),
                 mename, parv[0], "MODE");
      return 0;
    }

  if (parc < 3)
    {
      *modebuf = *parabuf = '\0';
      modebuf[1] = '\0';
      channel_modes(sptr, modebuf, parabuf, chptr, 0);
      sendto_one(sptr, form_str(RPL_CHANNELMODEIS), mename, parv[0],
                 chptr->chname, modebuf, parabuf);
      sendto_one(sptr, form_str(RPL_CREATIONTIME), mename, parv[0],
                 chptr->chname, chptr->channelts);
      return 0;
    }

  set_channel_mode(cptr, sptr, chptr, parc - 2, parv + 2);

  return 0;
}


