#ifndef _XP_ERROR_H
#define _XP_ERROR_H

#define _E_ 400                                         /* Base error code */
#define XE_EXIST (_E_ + 1)                              /* Exists */
#define XE_NOMEM (_E_ + 2)                              /* No memory */
#define XE_MTU_EXCEEDED (_E_ + 3)                       /* Packet larger than MTU */
#define XE_UNKNOWN_IP_PROTOCOL (_E_ + 4)                /* Unknown IP protocol */
#define XE_EXPECTED_AH_PROTO (_E_ + 5)                  /* expected next_proto to be AH */
#define XE_EXPECTED_ESP_PROTO (_E_ + 6)                 /* expected next_proto to be ESP */
#define XE_AH_DIGEST_MISMATCH (_E_ + 7)                 /* AH Digest mismatch */
#define XE_ESP_DIGEST_MISMATCH (_E_ + 8)                /* ESP Digest mismatch */
#define XE_SRC_OUT_OF_DATA (_E_ + 9)                    /* Ran out of data on src packet */
#define XE_DST_OUT_OF_DATA (_E_ + 10)                   /* Ran out of data on dest packet */
#define XE_BAD_PAD_DATA (_E_ + 11)                      /* Bad Pad Data */
#define XE_BAD_PARAM (_E_ + 12)                         /* Bad parameter */
#define XE_BAD_ID_TYPE (_E_ + 13)                       /* Bad ID type */
#define XE_POLICY_NOT_FOUND (_E_ + 14)                  /* Policy Not Found */
#define XE_BAD_ADDR_RANGE (_E_ + 15)                    /* Bad IP Address Range */
#define XE_BAD_PORT_NUMBER (_E_ + 16)                   /* Bad TCP/UDP port number */
#define XE_OUT_OF_PACKETS (_E_ + 17)                    /* Unable to allocate a Packet */
#define XE_OUT_OF_REQS (_E_ + 18)                       /* Unable to allocate a Request */
#define XE_OUT_OF_SESSIONS (_E_ + 19)                   /* Unable to allocate a Session */
#define XE_OUT_OF_POLICIES (_E_ + 20)                   /* Unable to allocate a Policy */
#define XE_NO_DATA (_E_ + 21)                           /* Not enough data */
#define XE_UNKNOWN_ALGORITHM (_E_ + 22)                 /* Unknown Algorithm */
#define XE_BAD_ESP_SPI (_E_ + 23)                       /* Bad ESP SPI */
#define XE_BAD_AH_SPI (_E_ + 24)                        /* Bad AH SPI */
#define XE_INBOUND_POLICY_FAILURE (_E_ + 25)            /* Inbound Policy Mismatch */
#define XE_KEYNEG_INPROGRESS (_E_ + 26)                 /* Key negotiation is in progress */
#define XE_BAD_SEQUENCE_NUMBER (_E_ + 27)               /* Replay failure - Bad AH or ESP seq # */
#define XE_INBOUND_UNEXPECTED_SECURED_PACKET (_E_ + 28) /* expecting to drop or bypass, got secure packet */
#define XE_INBOUND_EXPECTED_SECURE_PACKET (_E_ + 29)    /* expected a secure packet, got a clear packet instead */
#define XE_SA_NOT_FOUND (_E_ + 30)                      /* Security Association Not Found */
#define XE_BAD_DOMAIN (_E_ + 31)                        /* Bad domain */
#define XE_PANIC_ERR (_E_ + 32)
#define XE_POST_CLASSIFY_ENCAP_FAILURE (_E_ + 33) /* failed to encapsulate packet */
                                                  /* (currently only used for dummy packets) */
#define XE_SESS_UNAVL_FOR_TX (_E_ + 34)           /* session not "available" to transmit dummy packets */
#define XE_DST_OUT_OF_SPACE (_E_ + 35)            /* Ran out of data on dest packet */

#endif /* _XP_ERROR_H */
