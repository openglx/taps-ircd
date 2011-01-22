/*****************************************************************
 * PTlink IRCd is (C) CopyRight PTlink IRC Software 1999-2002    *
 * http://www.ptlink.net/Coders/ - coders@PTlink.net             *
 * This program is distributed under GNU Public License          *
 * Please read the file COPYING for copyright information.       *
 *****************************************************************
 * $Id: svline.c 33 2010-05-10 02:46:01Z openglx $         *
 
 Desc: services v-line (forbidden filenames on DCC SEND)
 Author: Lamego@PTlink.net
  
*/

#include "m_commands.h"
#include "client.h"
#include "s_user.h"
#include "s_misc.h"
#include "common.h"
#include "ircd.h"
#include "numeric.h"
#include "s_serv.h"
#include "irc_string.h"
#include "send.h"
#include "s_conf.h"
#include "struct.h"
#include "svline.h"
#include "dconf_vars.h"


#include <stdlib.h> 
#include <string.h>

/* svlines storage */
aConfItem *svlines = (aConfItem *)NULL;

/*
 * m_svline() - add filename mask DCC SEND fordbid line
 *
 *	parv[0] = sender prefix
 *	parv[1] = filename mask
 *      parv[2] = reason
 */
int	m_svline(struct Client *cptr, struct Client *sptr, int parc, char *parv[])
  {
    aConfItem *aconf;
    char *reason;

	if (!IsService(sptr) && !IsServer(cptr))
	  {
		sendto_one(sptr, form_str(ERR_NOPRIVILEGES), me.name, parv[0]);
		return 0;
	  }	  
		
	if(parc<2)
	  {
		sendto_one(sptr, form_str(ERR_NEEDMOREPARAMS),
      	  me.name, parv[0], "SVLINE");
        return 0;
	  }
      
    reason = (parc>2) ? parv[2] : "";
    
    parv[1] = strip_control_codes_lower(parv[1]);

    if (!find_svline(parv[1])) /* svline does not exist */
	  {

		aconf = make_conf();        
        
		DupString(aconf->name, parv[1]);

	        aconf->next = svlines;
		svlines = aconf;		

		/* check if first word is ":gline:", ":kill:" or ":block" */
		if (strstr(reason, ":gline: ") == reason) {
			SetSVLineAction(aconf, GLINE_ON_MATCH);
			reason += strlen(":gline: ");
		}
		else if (strstr(reason, ":kill: ") == reason) {
			SetSVLineAction(aconf, KILL_ON_MATCH);
			reason += strlen(":kill: ");
		}
		else if (strstr(reason, ":block: ") == reason) {
			SetSVLineAction(aconf, BLOCK_ON_MATCH);
			reason += strlen(":block: ");
		}
        
		if (reason && *reason != '\0')
			DupString(aconf->passwd, reason);

                sendto_serv_butone(cptr, ":%s SVLINE %s :%s",
			  parv[0], parv[1], reason);			
	  }

	return 0;
  }

/*
 * m_unsvline() - remove v-line
 *
 *	parv[0] = sender prefix
 *	parv[1] = svlined mask
 */
int	m_unsvline(struct Client *cptr, struct Client *sptr, int parc, char *parv[])
  {
	aConfItem *svl = svlines;
	aConfItem *last_svl = NULL;
		
	if (!IsService(sptr) && !IsServer(cptr))
	  {
		sendto_one(sptr, form_str(ERR_NOPRIVILEGES), me.name, parv[0]);
		return 0;
	  }	  
		
	if(parc<2)
	  {
		sendto_one(sptr, form_str(ERR_NEEDMOREPARAMS),
      	  me.name, parv[0], "UNSVLINE");
        return 0;
	  }		  
	
	while(svl && irccmp(svl->name, parv[1]))
	  {
		last_svl = svl;
		svl = svl->next;
	  }
	  
    if (svl) /* if svline does exist */
	  {
		if(last_svl) /* this is not the first svline */
			last_svl->next=svl->next; /* adjust list link -> */
		  else
			svlines = svl->next;
			
		free_conf(svl);
		
	    sendto_serv_butone(cptr, ":%s UNSVLINE %s",
			parv[0], parv[1]);			
	  }
	  
	return 0;
  }
  

/*
 * find_svline  - checks if the host matchs any entry on the sqline list
 * inputs       - lookup host
 * output       - return svline entry if found or NULL if not found;
 * side effects - heading svline will be checked and deleted if expired
 */
struct ConfItem* find_svline(char *name)
  {
	aConfItem *svl;

	/* First lets check if we have svlines */
	if(!svlines)
	  return NULL;
	  	  
	svl = svlines;

    /* search for host */	
	while(svl && irccmp(svl->name, name))
	  svl=svl->next;
	  
    return svl;	
		
  }


/*
 * is_supervlined - check if the message contains a supersvlined word
 * inputs       - lookup name
 * output       - returns 0 = not found, 1 = found
 */
int is_supersvlined(char *message, char *vline)
{
  char *cm = message; /* current char pointer*/
  char *cv; /* current char on vline */
  int i;
  return match(vline, message);
}
    
/*
 * is_svlined   - checks if the file matchs any entry on the sqline list
 * inputs       - lookup name
 * output       - return svline entry if found or NULL if not found;
 */
struct ConfItem* is_svlined(char *name)
{
  aConfItem *svl = svlines;
  
  /* search for host */	
  while(svl && (
    (svl->name[0] == '!') ||
    (svl->name[0] == '/') ||
    !match(svl->name, name)))
      svl=svl->next;
	  
  return svl;	
		
}

/*
 * is_msgvlined  - checks if any msgsvline is on the message
 * inputs       - lookup name
 * output       - return svline entry if found or NULL if not found;
 */
aConfItem* is_msgvlined(char *msg)
{
  aConfItem *svl = svlines;   
  
  /* search for host */	
  while(svl)
  {
    if((svl->name[0] == '!') && strstr(msg, &svl->name[1]))
      break;
    else if((svl->name[0] == '/') && is_supersvlined(msg, &svl->name[1]))
      break;
    else
      svl=svl->next;
  }
  return svl;		
}
  
/* send_all_svlines
 *
 * inputs       - pointer to aClient
 * output       - none
 * Side effects - sends all svlines to the specified server
 *
 */
void send_all_svlines(struct Client *acptr)
  {
	struct ConfItem *svl = svlines;
	
	while(svl)
	  {	  
	    sendto_one(acptr, ":%s SVLINE %s :%s",
			  me.name, svl->name, svl->passwd);
			  			  		  
		svl = svl->next;		
	  }
  }
  
/*
 * report_svlines
 *
 * inputs       - pointer to client to report to
 * output       - none
 * side effects - all SV lines are listed to client 
 */
void report_svlines(struct Client *sptr)
{
  struct ConfItem *aconf;
  char *host;
  char *user;
  char *pass;
  char *name;
  int port;
  
  aconf = svlines;
  
  while(aconf)
	{
  	  get_printable_conf(aconf, &name, &host, &pass, &user, &port);
          
      sendto_one(sptr, form_str(RPL_STATSQLINE),
                   me.name, sptr->name, name, pass, "", "");
	  aconf=aconf->next;	  
    }
}

/*
 * clear_svlines
 *
 * inputs       - none
 * output       - none
 * side effects - clear the svlines list
 */
void clear_svlines(void)
  {
  
    aConfItem *aconf;
	aConfItem *svl=svlines;
  
	while(svl)
	  {
		aconf = svl->next;
		free_conf(svl);
		svl = aconf;
	  }
			  
	svlines = NULL;
	
  }

/**
 * \brief	Act on svline match (warn, block, gline)
 * \param	user		aClient* of offending user
 * \param	msg		char* of offense
 * \param	svline		aConfItem* of matched svline
 * \return	0 if no futher action is needed, 1 is should act (block msg)
 */
int act_on_svline(struct Client* sptr, char* msg, aConfItem* svline)
{
	char *mename = me.name;
	aConfItem* aconf;
	char* kill_parv[3];
	
	/* if not user, ignore */
	if (! sptr->user)
		return 0;

	if (sptr->user->vlink)
		mename = sptr->user->vlink->name;

	/* warn user */
	sendto_one(sptr, ":%s NOTICE %s :%s", mename, sptr->name, svline->passwd);

	switch (svline->flags) {
		case GLINE_ON_MATCH:
		/* If GLineOnSVline is a number higher than 0 and an user
		 * has triggered a SVline, add a temporary GLine (with time
		 * being equal GLineOnSVline time) to *@(user host).
		 *
		 * Use wisely, as this can lead to big problems!
		 * -- openglx
		 */
		if(GLineOnSVline > 0) {
			aconf = make_conf();
			aconf->status = CONF_KILL;

			DupString(aconf->host, sptr->realhost);
			DupString(aconf->passwd, "Auto GLined for triggering a SVline");
			DupString(aconf->user, "*");
			DupString(aconf->name, me.name);
			aconf->hold = CurrentTime + GLineOnSVline;
			add_gline(aconf);

			sendto_serv_butone(NULL, ":%s GLINE %s@%s %lu %s :%s",
				 me.name,
				 aconf->user, aconf->host,
				 aconf->hold - CurrentTime,
				 aconf->name, aconf->passwd);
	
			apply_gline(aconf->host, aconf->user,
				"Auto GLined for triggering a SVline");
		}

		sendto_ops_imodes(IMODE_VLINES,"GLined for %d seconds after svlined message"
				" (from %s): %s", GLineOnSVline, sptr->name, msg);
		return 1;
		/******************/

		case KILL_ON_MATCH:
			kill_parv[0] = me.name;
			kill_parv[1] = sptr->name;
			kill_parv[2] = "SVLINE Killed after svline message";
			m_kill(&me, &me, 3, kill_parv);
			sendto_ops_imodes(IMODE_VLINES,"Killed after svlined message"
				" (from %s): %s", sptr->name, msg);
			return 1;
		/******************/

		case BLOCK_ON_MATCH:
			sendto_ops_imodes(IMODE_VLINES,"Blocked svlined message"
				" (from %s): %s", sptr->name, msg);
			return 1;
		/******************/

		case WARN_ON_MATCH:
		default:
			sendto_ops_imodes(IMODE_VLINES,"Warning on svlined message"
				" (from %s): %s", sptr->name, msg);
			return 0;
	}

	return 0;
}

