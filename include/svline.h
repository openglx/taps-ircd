/*****************************************************************
 * PTlink IRCd is (C) CopyRight PTlink IRC Software 1999-2001     *
 *                http://software.pt-link.net                     *
 * This program is distributed under GNU Public License          *
 * Please read the file COPYING for copyright information.       *
 *****************************************************************
 *   $Id:
   
  File: svline.h
  Desc: services v-line (DCC block, filename based)
  Author: Lamego@PTlink.net
*/
#include "struct.h"

extern aConfItem *svlines;
struct ConfItem* find_svline(char *name);
struct ConfItem* is_svlined(char *name);
struct ConfItem* is_msgvlined(char *msg);
int is_supervlined(char *message, char *vline);
void send_all_svlines(struct Client *acptr);
void report_svlines(struct Client *sptr);
void clear_svlines(void);

int act_on_svline(aClient* user, char* msg, aConfItem* svline);

/* flags */
#define		WARN_ON_MATCH	0x0001
#define		BLOCK_ON_MATCH	0x0002
#define		KILL_ON_MATCH	0x0004
#define		GLINE_ON_MATCH	0x0008

#define		SetSVLineAction(x, y)	((x)->flags |= (y))
#define		SVLineWarnOnMatch(x)	((x)->flags & WARN_ON_MATCH)
#define		SVLineBlockOnMatch(x)	((x)->flags & BLOCK_ON_MATCH)
#define		SVLineKillOnMatch(x)	((x)->flags & KILL_ON_MATCH)
#define		SVLineGLineOnMatch(x)	((x)->flags & GLINE_ON_MATCH)

