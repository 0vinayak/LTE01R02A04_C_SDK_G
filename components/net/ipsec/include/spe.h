#ifndef _SPE_H
#define _SPE_H

/****************************************************************************
 * Packet Classification
 * Security Policy Entries (SPE) and Security Policy Search (SPS)
 ****************************************************************************/
/*
 * You need upto 5 entries for each IPSEC policy. (One each for Src Address,
 * Dst Address, Protocol, Src Port and Dst Port).
 */
#define MAX_SP_ENTRY_COUNT (MAX_SP_COUNT * 5)
/*
 * Define this if you want to maintain statistics on each entry
 */
#define SPE_STATISTICS

/*
 * Define this if you want to maintain statistics on policy searches
 */
#define SPS_STATISTICS

/*
 * SP Search Statistics (Global Statistics)
 */
#define SPS_STATS_SEARCHES 0     /* # of searches */
#define SPS_STATS_ENT_SEARCHED 1 /* # of entries searched */
#define SPS_STATS_MATCHCOUNT 2   /* Match Count */
#define SPS_STATS_NOMATCHCOUNT 3 /* No Match Count */
#define SPS_STATS_NUM_POLICIES 4 /* total # of policies */
#define SPS_STATS_NUM_ENTRIES 5  /* total # of entries */
#define SPS_STATS_MAX 6

#ifdef SPS_STATISTICS
#define SPSStatsSearches() sps_stats[SPS_STATS_SEARCHES]++
#define SPSStatsEntriesSearched() sps_stats[SPS_STATS_ENT_SEARCHED]++
#define SPSStatsMatch() sps_stats[SPS_STATS_MATCHCOUNT]++
#define SPSStatsNoMatch() sps_stats[SPS_STATS_NOMATCHCOUNT]++
#define SPSStatsEntryAdded() sps_stats[SPS_STATS_NUM_ENTRIES]++
#define SPSStatsPolicyAdded() sps_stats[SPS_STATS_NUM_POLICIES]++
#define SPSStatsEntryDeleted() sps_stats[SPS_STATS_NUM_ENTRIES]--
#define SPSStatsPolicyDeleted() sps_stats[SPS_STATS_NUM_POLICIES]--
#else
#define SPSStatsSearches()
#define SPSStatsEntries()
#define SPSStatsMatch()
#define SPSStatsNoMatch()
#define SPSStatsEntryAdded()
#define SPSStatsPolicyAdded()
#define SPSStatsEntryDeleted()
#define SPSStatsPolicyDeleted()
#endif /* SPS_STATISTICS */

/*
 * SP Entry Statistics (Per SP Entry statistics)
 */
#define SPE_STATS_MATCHCOUNT 0   /* Match Count */
#define SPE_STATS_NOMATCHCOUNT 1 /* No Match Count */
#define SPE_STATS_MAX 2

#ifdef SPE_STATISTICS
#define SPEStatsMatch(e) (e)->spe_stats[SPE_STATS_MATCHCOUNT]++
#define SPEStatsNoMatch(e) (e)->spe_stats[SPE_STATS_NOMATCHCOUNT]++
#else
#define SPEStatsMatch(e)
#define SPEStatsNoMatch(e)
#endif /* SPE_STATISTICS */

/*
 * Action Types
 */
#define SPE_ACTION_POLICY 1  /* action data is a SP */
#define SPE_ACTION_SESSION 2 /* action data is a IPSec Session */
#define SPE_ACTION_MGMT 3    /* Packet is a management packet */

typedef struct SPEntry_s SPEntry;

struct SPEntry_s
{
    SPEntry *spe_prevTable; /* Prev entry that matched */
    SPEntry *spe_nextTable; /* Next entry if match */
    SPEntry *spe_next;      /* Next entry if no match */
    void *spe_data;         /* actiontype specific data */
    Uint8 spe_type;         /* Type of entry (see below) */
    Uint8 spe_action;       /* Action Type */
    Uint8 spe_priority;     /* priority */
    Uint16 spe_ref;         /* Reference Count */
    union {
        Uint32 val32; /* 32-bit value */
        Uint16 val16; /* 16-bit value */
        Uint8 val8;   /* 8-bit value */
    } spe_u1;
    Uint32 spe_val32_2; /* subnet mask or upper limit*/
#ifdef SPE_STATISTICS
    Uint32 spe_stats[SPE_STATS_MAX];
#endif
};

#define spe_subnet_mask spe_val32_2
#define spe_upper_bound spe_val32_2
#define spe_lower_bound spe_u1.val32
#define spe_ipv4address spe_u1.val32
#define spe_port_num spe_u1.val16
#define spe_protocol spe_u1.val8

/*
 * SP Entry Types
 * (Source and Destination are for Outbound packets. Inbound packets
 * will have source and destination switched)
 */
#define SPE_UNUSED 0              /* Entry is not in use */
#define SPE_IPV4SRC_ADDR 1        /* IP v4 Src Address */
#define SPE_IPV4SRC_SUBNET_ADDR 2 /* IP v4 Src Subnet Address */
#define SPE_IPV4SRC_ADDR_RANGE 3  /* IP v4 Src Address Range */
#define SPE_IPV4SRC_PORT 4        /* IP v4 Src Port */

#define SPE_IPV4PROTOCOL 5 /* IP v4 protocol */
#define SPE_WILD 6         /* Wildcard (matches anything) */

#define SPE_IPV4DST_ADDR 7        /* IP v4 Dst Address */
#define SPE_IPV4DST_SUBNET_ADDR 8 /* IP v4 Dst Subnet Address */
#define SPE_IPV4DST_ADDR_RANGE 9  /* IP v4 Dst Address Range */
#define SPE_IPV4DST_PORT 10       /* IP v4 Dst Port */
/*
 * SP Entry Lock/Unlock
 */
#define SPELock(lock) IPSEC_Lock(lock, SPEMutex)
#define SPEUnlock(lock) IPSEC_Unlock(lock, SPEMutex)

#endif /* _SPE_H */
