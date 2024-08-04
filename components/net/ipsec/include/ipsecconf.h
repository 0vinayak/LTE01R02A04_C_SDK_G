#ifndef _IPSECCONF_H
#define _IPSECCONF_H

/* MAX_SP_COUNT - maximum number of IPSEC security policies;
 *                each entry specifies policy for both directions
 */
#ifndef MAX_SP_COUNT
#define MAX_SP_COUNT 16
#endif

/* SP_COUNT_PRI_... - Security Policy Priority counts
 * Policy priorities determine the order in which the policies
 * are added into the Security Policy Database (SPD) and hence
 * the order in which they are searched for packet classification.
 * Define number of SPD entries for each priority level
 * For policies that are at the same priority level, the policy
 * that was added first will be at the top of the list.
 * The total of these MUST equal to MAX_SP_COUNT.
 * Usually, the bulk of the policies will be at MEDIUM priority,
 * so the macros below let you tune the other counts and set
 * SP_COUNT_PRI_MEDIUM as whatever's left over from MAX_SP_COUNT.
 */
#ifndef SP_COUNT_PRI_HIGHEST
#define SP_COUNT_PRI_HIGHEST 0
#endif
#ifndef SP_COUNT_PRI_HIGH
#define SP_COUNT_PRI_HIGH 6
#endif
#ifndef SP_COUNT_PRI_MED_HIGH
#define SP_COUNT_PRI_MED_HIGH 0
#endif
/* MEDIUM priority goes here, but is defined below */
#ifndef SP_COUNT_PRI_MED_LOW
#define SP_COUNT_PRI_MED_LOW 0
#endif
#ifndef SP_COUNT_PRI_LOW
#define SP_COUNT_PRI_LOW 0
#endif
#ifndef SP_COUNT_PRI_LOWEST
#define SP_COUNT_PRI_LOWEST 2
#endif
#ifndef SP_COUNT_PRI_MEDIUM
#define SP_COUNT_PRI_MEDIUM (MAX_SP_COUNT - SP_COUNT_PRI_HIGHEST - SP_COUNT_PRI_HIGH - SP_COUNT_PRI_MED_HIGH - SP_COUNT_PRI_MED_LOW - SP_COUNT_PRI_LOW - SP_COUNT_PRI_LOWEST)
#endif

#if SP_COUNT_PRI_MEDIUM <= 0
#define FOR_DISPLAY SP_COUNT_PRI_MEDIUM
#undef SP_COUNT_PRI_MEDIUM
#error SP_COUNT_PRI_MEDIUM (FOR_DISPLAY) is less than 0
#endif

/* MAX_IPSEC_SESSIONS - maximum number of concurrent IPSEC sessions;
 *                      must be a power of two
 */
#ifndef MAX_IPSEC_SESSIONS
#define MAX_IPSEC_SESSIONS 8
#endif

/* MAX_IPSEC_REQUESTS - maximum number of IPSEC requests that can be
 *                      pending simultaneously
 */
#ifndef MAX_IPSEC_REQUESTS
#define MAX_IPSEC_REQUESTS 4
#endif

/* MAX_POLICY_DOMAINS - maximum number of policy domains;
 *                      each domain has it's own policy classification tree
 *
 * You can either have each network interface as a separate domain or
 * bundle up several related interfaces into a single domain or even have
 * all interfaces on a single domain
 */
#ifndef MAX_POLICY_DOMAINS
#define MAX_POLICY_DOMAINS 1
#endif

#endif /* _IPSECCONF_H */
