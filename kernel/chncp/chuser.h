#ifndef CHAOS_CHNCP_CHUSER_H
#define CHAOS_CHNCP_CHUSER_H

void ch_read(struct connection *);
int ch_sflush(struct connection *);
int ch_eof(struct connection *);
void ch_close(struct connection *, struct packet *, int);
void ch_accept(struct connection *);
int ch_sread(struct connection *, char *, unsigned, int, int *);
int ch_swrite(struct connection *, char *, unsigned, int, int *);
int ch_write(struct connection *, struct packet *);
void ch_free(char *);
void ch_sts(struct connection *conn);
void setpkt(struct connection *conn,struct packet *pkt);
void clsconn(struct connection *conn, int state, struct packet *pkt);
void rlsconn(struct connection *conn);
void senddata(struct packet *pkt);
int concmp(struct packet *rfcpkt,char *lsnstr,int lsnlen);
void lsnmatch(struct packet *rfcpkt,struct connection *conn);
void makests(struct connection *conn,struct packet *pkt);
void reflect(struct packet *pkt);
void rmlisten(struct connection *conn);
void sendsts(struct connection *conn);
void sendctl(struct packet *pkt);
struct packet *ch_rnext(void);
void ch_rskip(void);

#endif
