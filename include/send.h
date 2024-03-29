/*
 * send.h
 * Copyright (C) 1999 Patrick Alken
 *
 * $Id: send.h 33 2010-05-10 02:46:01Z openglx $
 */

#ifndef INCLUDED_send_h
#define INCLUDED_send_h
#ifndef INCLUDED_config_h
#include "config.h"       /* HAVE_STDARG_H */
#endif

/*
 * struct decls
 */
struct Client;
struct Channel;

/* send.c prototypes */

extern  void send_operwall(struct Client *,char *,char *);
extern  void sendto_channel_type_notice(struct Client *, 
                                        struct Channel *, int, char *);
extern void
send_knock(struct Client *sptr, struct Client *cptr, struct Channel *chptr, int type, 
           char *message, char *key);
                                        
extern  int sendto_slaves(struct Client *, char *, char *, int, char **);

extern  void sendto_one(struct Client *, const char *, ...);
extern  void sendto_channel_butone(struct Client *, struct Client *, 
                                   struct Channel *, const char *, ...);
extern 	void sendto_channelops_butone(struct Client *one, struct Client *from,
				  struct Channel *chptr, const char *pattern, ...);
					  
extern  void sendto_channel_type(struct Client *,
                                 struct Client *, 
                                 struct Channel *,
                                 int type,
                                 const char *nick,
                                 const char *cmd,
                                 const char *message);
extern  void sendto_serv_butone(struct Client *, const char *, ...);
extern  void sendto_common_channels(struct Client *, const char *, ...);

extern  void sendto_common_channels_quits(struct Client *user, const char *pattern, ...);
extern  void sendto_common_channels_part(struct Client *user, char* msg);

extern  void sendto_channel_butserv(struct Channel *, struct Client *, 
                                    const char *, ...);

extern  void sendto_channel_chanops_butserv(struct Channel *chptr,
					    struct Client *from, 
					    const char *pattern, ...);

extern  void sendto_channel_non_chanops_butserv(struct Channel *chptr,
						struct Client *from, 
						const char *pattern, ...);

extern  void sendto_match_servs(struct Channel *, struct Client *, 
                                const char *, ...);
extern  void sendto_match_cap_servs(struct Channel *, struct Client *, 
                                    int, const char *, ...);
extern  void sendto_nomatch_cap_servs(struct Channel *, struct Client *, 
                                    int, const char *, ...);									
extern  void sendto_match_butone(struct Client *, struct Client *, 
                                 char *, int, const char *, ...);

extern  void sendto_ops_flags(int, const char *, ...);
extern  void sendto_ops_imodes(int, const char *, ...);

extern  void sendto_realops(const char *, ...);
extern  void sendto_realops_flags(int, const char *, ...);

extern  void sendto_ops(const char *, ...);
extern  void sendto_ops_butone(struct Client *, struct Client *, 
                               const char *, ...);
extern  void sendto_wallops_butone(struct Client *, struct Client *, 
                                   const char *, ...);
extern  void sendto_globops(const char *, ...);								   
extern  void ts_warn(const char *, ...);

extern  void sendto_prefix_one(struct Client *, struct Client *, 
                               const char *, ...);
							   
extern  void sendto_news(struct Client *, unsigned int newsmask, const char *, ...);
extern  void send_syntax(struct Client *cptr, const char *command, const char *pattern, ...);
extern  void    flush_server_connections(void);
extern void flush_connections(struct Client* cptr);

/* used when sending to #mask or $mask */

#define MATCH_SERVER  1
#define MATCH_HOST    2

#endif /* INCLUDED_send_h */
