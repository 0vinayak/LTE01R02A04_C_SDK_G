#ifndef _IPSECCOM_H_
#define _IPSECCOM_H_

//#include <string.h> /* memcmp, memset, strlen */
//#include <stddef.h> /* ptrdiff_t */
//#include <stdlib.h> /* exit */
#include "arpa/inet.h"
#include <string.h> /* memcmp, memset, strlen */
#include <stddef.h>
#include <stdint.h>
/* IPSEC_htonl &c htonl()-family porting macros
 *
 * Map these to the InterNiche htonl() definitions from ipport.h
 */
#define IPSEC_htonl(l) htonl(l)
#define IPSEC_htons(s) htons(s)
#define IPSEC_ntohl(l) ntohl(l)
#define IPSEC_ntohs(s) ntohs(s)

/* IPSEC_HTONL() &c in-place htonl()-family porting macros
 *
 * Map these to uses of the IPSEC_htonl() &c macros
 */
#define IPSEC_HTONL(l) (l) = IPSEC_htonl(l)
#define IPSEC_HTONS(l) (s) = IPSEC_htons(s)
#define IPSEC_NTOHL(l) (l) = IPSEC_ntohl(l)
#define IPSEC_NTOHS(s) (s) = IPSEC_ntohs(s)

#define OS_INICHE /* use InterNiche NicheStack OS (timekeeping) */
#define USE_SHA2 1

typedef unsigned char Uchar;
typedef unsigned char Uint8;
typedef unsigned short Uint16;
typedef unsigned int Uint32;
typedef unsigned long Uint64;
typedef unsigned int Bits32; /* 32-bit Bit field */
typedef unsigned int Bits8;  /* 8-bit Bit field */
typedef unsigned char u_char;
typedef unsigned short u_short;
typedef unsigned int Uint;
typedef unsigned long u_long;
typedef unsigned char UINT8;
typedef unsigned short UINT16;
typedef unsigned int UINT32;
typedef unsigned long Uint64;
typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned int uint32;
typedef unsigned long uint64;
typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
//typedef unsigned int        uint32_t;
//typedef unsigned long uint64_t;
typedef signed int int32;
typedef int Bool;
typedef unsigned short unshort;
typedef unsigned long ulong;
typedef unsigned long ip_addr;
typedef unsigned char BOOLEAN;
typedef int bool_t;
typedef unsigned char u8;
typedef unsigned int u32;
typedef unsigned long u64;
typedef char int8;

#ifdef IPSEC_INLINES
static __inline Uint32 IPSEC_get32(Uchar *p)
{
    return ((p[0] << 24) | p[1] << 16 | (p[2] << 8) | p[3]);
}

static __inline void IPSEC_put32(Uchar *p, Uint32 v)
{
    p[0] = (Uchar)(v >> 24);
    p[1] = (Uchar)(v >> 16);
    p[2] = (Uchar)(v >> 8);
    p[3] = (Uchar)(v);
}
#else
#define IPSEC_get32(p) (Uint32)((((Uchar *)(p))[0] << 24) | \
                                (((Uchar *)(p))[1] << 16) | \
                                (((Uchar *)(p))[2] << 8) |  \
                                (((Uchar *)(p))[3]))
#define IPSEC_put32(p, v)                                    \
    {                                                        \
        ((Uchar *)(p))[0] = (Uchar)((v & 0xff000000) >> 24); \
        ((Uchar *)(p))[1] = (Uchar)((v & 0x00ff0000) >> 16); \
        ((Uchar *)(p))[2] = (Uchar)((v & 0x0000ff00) >> 8);  \
        ((Uchar *)(p))[3] = (Uchar)(v & 0x000000ff);         \
    }
#endif

#ifndef PUBLIC
#define PUBLIC
#endif /* no PUBLIC */

#ifndef LOCAL
#define LOCAL static
#endif /* no LOCAL */

#ifndef TRUE
#define TRUE 1
#endif /* no TRUE */

#ifndef FALSE
#define FALSE 0
#endif /* no FALSE */

#ifndef NULL
#define NULL 0
#endif

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#define ESUCCESS 0  /* whatever the call was, it worked */
#define EFAILURE -1 /* whatever the call was, it failed */

//#define IPSEC_stricmp(p,n)    stricmp(p,n)

#define MemAlloc(n) calloc(1, n)
#define MemFree(p) free((void *)p)
#define MemZero(p, n) memset(p, 0, n)
#define MemCopy(d, s, n) memcpy(d, s, n)

#define StructAlloc(t) (t *)calloc(1, sizeof(t))
#define StructFree(t, p) free((void *)p)

#define npalloc(A) calloc(1, A)
#define npfree(B) free((void *)B)

#define bzero(s, n) memset(s, 0, n)
#define bcopy(s, d, n) memcpy(d, s, n)

#define CRYPT_MALLOC(size) calloc(1, size)
#define CRYPT_FREE(p) free(p)

#ifdef NEED_ALIGNMENT
#define CopyWord32(d, s) MemCopy((Uchar *)&(d), (Uchar *)&(s), 4);
#else
#define CopyWord32(d, s) (d) = (s)
#endif

#ifndef IP6EQ
/* compare two IP addresses, return TRUE if the same; else FALSE */
#define IP6EQ(addrptr1, addrptr2) (!MEMCMP(addrptr1, addrptr2, sizeof(ip6_addr)))
#define IP6EQX(addrptr1, addrptr2) (!MEMCMP(addrptr1, addrptr2, sizeof(ip6_addr)) || (IN6_IS_ADDR_LOOPBACK(addrptr1) && IN6_IS_ADDR_LOOPBACK(addrptr2)))
#endif

#ifndef IP6CPY
/* Copy addrptr2 into addrptr1 */
#define IP6CPY(addrptr1, addrptr2) MEMCPY(addrptr1, addrptr2, sizeof(ip6_addr))
#endif

/*ipport.h  parameter define*/

#define VOWIFI_LOW_POWER 1
#define MINI_TCP 1 /* Use Nichelite mini-TCP layer */
//#define IP_MULTICAST       1  /* support IP multicast capability in stack */
#define IP_V4 1             /* TCP/IPv4 */
#define IP_V6 1             /* Include IP version 6 in build */
#define MAX_NAT_INSTANCE 22 /* max tethering  */
#define NPDEBUG 1           /* turn on debugging dprintf()s */
#define IPTRACE 0x200       /* Trace packet in internet layer  */

#ifdef NOT_USED
#define USE_CRYPTOENG 1 /* enable cipher engine */
#endif                  /* NOT_USED */
/* Length of the biggest local net header in the whole system */
#define MaxLnh 4

/* length of buffer required for printable representation of IPv6 address */
#define IP6_ADDRBUF_LEN 40

#define TPS 2000000 //8910 8850

Uint32 time_ss(void);

u_long IPSEC_ticks(void);

#define CTICKS IPSEC_ticks()

void TCPIP_TRACE(const char *format, ...);

#define USE_ARG(x) \
    {              \
        x = x;     \
    }
#define USE_VOID(x) \
    {               \
        x = x;      \
    }

/**** IPSec Options *********************************************************/
#ifdef MODULE_SUPPORT
#define IPSEC 1       /* IPSEC */
#define IPSEC_MENUS 1 /* IPSEC CLI commands */
#endif
#ifdef NOT_USED
#error "NOT_USED" should not be defined

#define USE_IPSEC_TESTMENU 1

#endif /* NOT_USED */
#define min(a, b) ((a) < (b) ? (a) : (b))
#define IPSEC 1       /* IPSEC */
#define IPSEC_MENUS 1 /* IPSEC CLI commands */
/*
 * map memory routines to standard library
 *
 * NOTE:#define MEMCPY SCI_MEMCPY
 * this definition is very danger,because if
 * #define SCI_MEMCPY SCI_ASSERT(PNULL != _DEST_PTR); \
        if (_SIZE>0) { memcpy(_DEST_PTR, _SRC_PTR, _SIZE);}
 * in tcpip lib there is many code as:
    if (mppp->states[IPCP_STATE] != ST_OPENED)
      MEMCPY(go, &tryfor, sizeof(tryfor) );
   so this will lead to error codes.
   this must be note when port the codes.
 *
 */
//#define MEMCPY(dst,src,size)  memcpy(dst,src,size)
//#define MEMMOVE(dst,src,size) memmove(dst,src,size)//memmove(dst,src,size)
#define MEMSET(ptr, val, size) memset(ptr, val, size)
#define MEMCMP(p1, p2, size) memcmp(p1, p2, size) //memcmp(p1,p2,size)

#ifndef ASSERT
#define ASSERT(x)                                                         \
    do                                                                    \
    {                                                                     \
        if (!(x))                                                         \
        {                                                                 \
            sys_arch_printf("Assertion \"%d\" failed at line %d in %s\n", \
                            x, __LINE__, __FILE__);                       \
            fflush(NULL);                                                 \
            abort();                                                      \
        }                                                                 \
    } while (0)
#endif

/* IPSEC_in_cksum() - Internet checksum function
 *
 * Map this to your Internet checksum function, or #define
 * IPSEC_PORTABLE_CKSUM to build the implementation in cksum.c,
 * and use the function prototype below.
 */
//#ifndef IPSEC_PORTABLE_CKSUM
//extern unsigned short cksum(void *data, unsigned len);
//#define IPSEC_in_cksum(d, len) ~cksum((d), ((len) + 1) >> 1) & 0xffff
//#else
unsigned short IPSEC_in_cksum(unsigned char *data, int len);
//#endif /* IPSEC_PORTABLE_CKSUM */

/* ethernet packet header */
struct ethhdr
{
    u_char e_dst[6];
    u_char e_src[6];
    unshort e_type;
};

/* ETHHDR_SIZE - size of packet header structure for allocation
 * purposes Note this is a default -- it should be overridden in
 * ipport.h if the need arises.
 */
#ifndef ETHHDR_SIZE
#define ETHHDR_SIZE (sizeof(struct ethhdr))
#endif /* ETHHDR_SIZE */

int RAND_bytes(unsigned char *buf, int num);

unsigned int RAND_get(void); /* crypt/rand.h */

#endif /* _IPSECCOM_H_ */
