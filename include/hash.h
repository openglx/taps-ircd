/************************************************************************
 *   IRC - Internet Relay Chat, include/hash.h
 *   Copyright (C) 1991 Darren Reed
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
 *   $Id: hash.h 33 2010-05-10 02:46:01Z openglx $
 */
#ifndef INCLUDED_hash_h
#define INCLUDED_hash_h
#ifndef INCLUDED_sys_types_h
#include <sys/types.h>
#define INCLUDED_sys_types_h
#endif

/* 
 * Client hash table size
 *
 * used in hash.c, s_debug.c
 */
#define U_MAX 65536

/* 
 * Channel hash table size
 *
 * used in hash.c, s_debug.c
 */
#define CH_MAX 16384

/* 
 * Watch list hash table size
 *
 */

#define WATCHHASHSIZE   10007 


struct Client;
struct Channel;

struct HashEntry {
  int    hits;
  int    links;
  void*  list;
};


extern struct HashEntry hash_get_channel_block(int i);
extern size_t hash_get_client_table_size(void);
extern size_t hash_get_channel_table_size(void);
extern void   init_hash(void);
extern void   add_to_client_hash_table(const char* name, 
                                       struct Client* client);
extern void   del_from_client_hash_table(const char* name, 
                                         struct Client* client);
extern void   add_to_channel_hash_table(const char* name, 
                                        struct Channel* chan);
extern void   del_from_channel_hash_table(const char* name, 
                                          struct Channel* chan);
extern struct Channel* hash_find_channel(const char* name, 
                                         struct Channel* chan);
extern struct Client* hash_find_client(const char* name, 
                                       struct Client* client);
extern struct Client* hash_find_server(const char* name);
extern unsigned int hash_nick_name(const char* name);
extern unsigned int hash_channel_name(const char* name);

extern struct Watch  *hash_get_watch(char  *name);
extern int   add_to_watch_hash_table(char* nick, struct Client  *cptr);
extern int   del_from_watch_hash_table(char  *nick, struct Client  *cptr);
extern int   hash_del_watch_list(struct Client  *cptr);
extern int   hash_check_watch(struct Client *cptr, int reply);
extern void  count_watch_memory(int *count, u_long *memory);
extern void  clear_watch_hash_table();
extern void  clear_client_hash_table();
#endif  /* INCLUDED_hash_h */



