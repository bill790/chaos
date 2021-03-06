/*
 * chaos.h
 *
 * original; Brad Parker <brad@heeltoe.com>
 * byte order cleanups; Joseph Oswald <josephoswald@gmail.com>
 *
 * $Id$
 */

#include "endian.h"

#define CHMAXDATA	488	/* Maximum data per packet */
#define CHSTATNAME	32	/* Length of node name in STATUS protocol */
#define CHSP	(040)
#define CHNL	(0200|015)
#define CHTAB	(0200|011)
#define CHFF	(0200|014)
#define CHBS	(0200|010)
#define CHLF	(0200|012) 

/*
 * These are the connection states
 */
#define CSCLOSED	0	/* Closed */
#define CSLISTEN	1	/* Listening */
#define CSRFCRCVD	2	/* RFC received (used?) */
#define CSRFCSENT	3	/* RFC sent */
#define CSOPEN		4	/* Open */
#define CSLOST		5	/* Broken by receipt of a LOS */
#define CSINCT		6	/* Broken by incomplete transmission */

#define CSUNKNOWN	-1

/*
 * These are the packet opcode types
 */
#define RFCOP	001		/* Request for connection */
#define OPNOP	002		/* Open connection */
#define CLSOP	003		/* Close connection */
#define FWDOP	004		/* Forward this packet */
#define ANSOP	005		/* Answer packet */
#define SNSOP	006		/* Sense packet */
#define STSOP	007		/* Status packet */
#define RUTOP	010		/* Routing information packet */
#define LOSOP	011		/* Losing connection packet */
#define LSNOP	012		/* Listen packet (never transmitted) */
#define MNTOP	013		/* Maintenance packet */
#define EOFOP	014		/* End of File packet  */
#define UNCOP	015		/* Uncontrolled data packet */
#define BRDOP	016		/* Broadcast RFC opcode */
#define DATOP	0200		/* Ordinary character data  */
#define DWDOP	0300		/* 16 bit word data */

/*
 * Modes available in CHIOCSMODE call.
 */
#define CHTTY		1
#define CHSTREAM	2
#define CHRECORD	3

/*
 * This file contains known contact names.
 */
#define CHAOS_FILE	"FILE"
#define CHAOS_SUPDUP	"SUPDUP"
#define CHAOS_TELNET	"TELNET"
#define CHAOS_STATUS	"STATUS"
#define CHAOS_TIME	"TIME"
#define CHAOS_ARPA	"ARPA"
#define CHAOS_SEND	"SEND"
#define CHAOS_RTAPE	"RTAPE"
#define CHAOS_MAIL	"MAIL"
#define CHAOS_ULOGIN	"ulogin"
#define CHAOS_UREAD	"uread"
#define CHAOS_UWRITE	"uwrite"
#define CHAOS_UCSH	"ucsh"
#define CHAOS_USEND	"usend"

#define CHMAXPKT	488		/* Maximum data length in packet */
#define CHMAXRFC	CHMAXPKT	/* Maximum length of a rfc string */
#define CHMAXARGS	50		/* Maximum number of words in a RFC */

#define CHSHORTTIME	(250)		/* Short time for retransmits - 250ms */

#define CHDRWSIZE	5		/* Default receive window size */

/*
 * A chaos network address.
 * JAO: By convention, the subnet is the MSB and the host the LSB
 * of the 16-bit (short) address. On the network, and in memory, 
 * LSB comes first.
 */
 
typedef	struct {
  unsigned char	host;	/* Host number on subnet */
  unsigned char	subnet;	/* Subnet number */
} chaddr;

#define CH_ADDR_SHORT(ch) ((ch).host | ((ch).subnet << 8))
#define SET_CH_ADDR(ch,short) do { \
ch.host = (short & 0xff); \
ch.subnet = (short & 0xff00) >> 8; \
} while (0)

/*
 * A chaos index - a hosts connection identifier
 * JAO: by convention, the LSB are used as an index into a table
 * (ci_Tidx), and the MSB is incremented to keep connection indices
 * unique to avoid collisions.
 */

typedef	struct	{
  unsigned char	tidx;	/* Connection table index */
  unsigned char	uniq;	/* Uniquizer for table slot */
} chindex;

#define CH_INDEX_SHORT(ci) ((ci).tidx | ((ci).uniq << 8))
#define SET_CH_INDEX(ci,short) do { ci.tidx = (short & 0xff); ci.uniq = (short & 0xff00) >> 8; } while (0)

/* The packet length is 16 bits, but only the lowest 12 bits denote
   an actual length; the MSB 4 bits are a forwarding count. We store them
   in CHAOS network order, LSB first. */
   
typedef struct {
  unsigned char lsb;
  unsigned char msb;
} chpklenfc;

#define LENFC_LEN(lenfc) ((lenfc).lsb | ((int)((lenfc).msb & 0x0f) << 8))
#define LENFC_FC(lenfc) (((lenfc).msb & 0xf0) >> 4)
#define SET_LENFC_LEN(lenfc,len) do {  (lenfc).lsb = (len) & 0xff; \
(lenfc).msb = ((lenfc).msb & 0xf0) | (((len) & 0x0f00) >> 8); } while(0)
#define SET_LENFC_FC(lenfc,fc) (lenfc).msb = (((lenfc).msb & 0x0f) | ((fc & 0xf) << 4))

#define PH_LEN(ph) (LENFC_LEN(ph.ph_lenfc))
#define PH_FC(ph)  (LENFC_FC(ph.ph_lenfc))
#define SET_PH_LEN(ph,len) SET_LENFC_LEN(ph.ph_lenfc,len)
#define SET_PH_FC(ph,fc) SET_LENFC_FC(ph.ph_lenfc,fc)

struct pkt_header {
  unsigned char		ph_type;	/* Protocol type */
  unsigned char		ph_op;		/* Opcode of the packet */
  chpklenfc             ph_lenfc;
  
	chaddr		ph_daddr;		/* Destination address */
	chindex		ph_didx;		/* Destination index */
	chaddr		ph_saddr;		/* Source address */
	chindex		ph_sidx;		/* Source index */
  unsigned short	LE_ph_pkn; 		/* Packet number */
  unsigned short	LE_ph_ackn; 	/* Acknowledged packet number */
};

/*
 * Record mode packet structure.
 */
struct chpacket	{
	unsigned char	cp_op;
	char		cp_data[CHMAXDATA];
};

/*
 * FILE server login record structure.
 */
struct chlogin {
	int	cl_pid;		/* Process id of server */
	short	cl_cnum;	/* Chaos channel number of server */
	short	cl_haddr;	/* Host address of other end */
	long	cl_ltime;	/* Login time */
	long	cl_atime;	/* Last time used. */
	char	cl_user[8];	/* User name */
};

/*
 * This structure returned by the CHIOCGSTAT ioctl to return
 * connection status information.
 */
struct	chstatus	{
	short	st_fhost;		/* remote host */
	short	st_cnum;		/* local channel number */
	short	st_rwsize;		/* receive window size */
	short	st_twsize;		/* transmit window size */
	short	st_state;		/* connection state */
	short	st_ptype;		/* Opcode of next packet to read */
	short	st_plength;		/* Length of next packet to read */
	short	st_cmode;		/* Mode of connection */
	short	st_oroom;		/* Output window space left */
	/* etc - anything else useful? */
};

/*
 * Structure for CHIOCOPEN
 */
struct chopen {
	char	*co_contact;	/* Contact string */
	char	*co_data;	/* RFC data if not NULL */
	short	co_host;	/* Host address to contact or zero for listen */
	short	co_async;	/* If non zero don't wait */
	short	co_clength;	/* Length of contact string */
	short	co_length;	/* Length of RFC data */
	short	co_rwsize;	/* Receive window size */
};

/*
 * Structure for CHIOCREJECT
 */
struct chreject {
	char	*cr_reason;
	int	cr_length;
};

#if 0
/*
 * Structure for CHIOCETHER
 */
#define CHIFNAMSIZ	16
struct chether {
	char	ce_name[CHIFNAMSIZ];
	short	ce_addr;
};

/*
 * Structure for CHIOCILADDR
 */
struct chiladdr {
	unsigned short	cil_device;
	unsigned short	cil_address;
};
struct chstatname {
	char	cn_name[CHSTATNAME];
};
#endif
