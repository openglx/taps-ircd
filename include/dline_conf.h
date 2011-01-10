/* dline_conf.h  -- lets muse over dlines, shall we?
 *
 * $Id: dline_conf.h 34 2010-07-31 18:35:02Z openglx $ 
 */

#ifndef INCLUDED_dline_conf_h
#define INCLUDED_dline_conf_h

#include <stdint.h>

struct Client;
struct ConfItem;

extern void clear_Dline_table();
extern void zap_Dlines();
extern void add_Dline(struct ConfItem *conf_ptr);
extern void add_ip_Kline(struct ConfItem *conf_ptr);

extern void add_dline(struct ConfItem *conf_ptr);

extern struct ConfItem *match_Dline(uint32_t ip);
extern struct ConfItem* match_ip_Kline(unsigned long ip, const char* name);

extern void report_dlines(struct Client *sptr);
extern void report_ip_Klines(struct Client *sptr);

#endif /* INCLUDED_dline_conf_h */


