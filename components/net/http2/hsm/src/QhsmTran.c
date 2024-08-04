/********************************************************************
 * Quantum Hierarchical State Machine dynamic transition definition
 * Copyright (c) 2002 Miro Samek, Palo Alto, CA.
 * All Rights Reserved.
 *******************************************************************/
#include "hsm_api.h"
/* helper macro */
#define TRIGGER(state_, sig_) \
    (HSM_State) (*(state_))(me, &pkgStdEvt[sig_])

void HSM_TranDyn(HSM_C *me, HSM_State target)
{
    HSM_State entry[8], p, q, s, *e, *lca;

    if (PNULL == me || PNULL == me->source__)
    {
        OSI_LOGE(0x1000a568, "HSM_TranDyn input error!");
        return;
    }

    if (target == (HSM_State)HSM_Top) /* cannot target top state */
    {
        OSI_LOGI(0x1000a569, "qhsm HSM_TranDyn cannot target top state");
    }
    for (s = me->state__; s != me->source__;)
    {
        HSM_State t;
        /* we are about to dereference s */
        if (0 == s)
        {
            OSI_LOGE(0x1000a56a, "qhsm HSM_TranDyn error");
            return;
        }
        t = TRIGGER(s, HSM_STATE_EXIT_SIG); /*lint !e613*/
        if (t)
        { /* exit action unhandled, t points to superstate */
            s = t;
        }
        else
        {                                  /* exit action handled, elicit superstate */
            s = TRIGGER(s, HSM_EMPTY_SIG); /*lint !e613*/
        }
    }

    *(e = &entry[0]) = 0;
    *(++e) = target; /* assume entry to target */

    /* (a) check source == target (transition to self) */
    if (me->source__ == target)
    {
        TRIGGER(me->source__, HSM_STATE_EXIT_SIG); /* exit source */
        goto inLCA;
    }
    /* (b) check source == target->super */
    p = TRIGGER(target, HSM_EMPTY_SIG);
    if (me->source__ == p)
    {
        goto inLCA;
    }
    /* (c) check source->super == target->super (most common) */
    q = TRIGGER(me->source__, HSM_EMPTY_SIG);
    if (q == p)
    {
        TRIGGER(me->source__, HSM_STATE_EXIT_SIG); /* exit source */
        goto inLCA;
    }
    /* (d) check me->source->super == target */
    if (q == target)
    {
        TRIGGER(me->source__, HSM_STATE_EXIT_SIG); /* exit source */
        --e;                                       /* do not enter the LCA */
        goto inLCA;
    }
    /* (e) check rest of source == target->super->super... hierarchy*/
    *(++e) = p;
    for (s = TRIGGER(p, HSM_EMPTY_SIG); s;
         s = TRIGGER(s, HSM_EMPTY_SIG))
    {
        if (me->source__ == s)
        {
            goto inLCA;
        }
        *(++e) = s;
    }
    TRIGGER(me->source__, HSM_STATE_EXIT_SIG); /* exit source */
    /* (f) check rest of source->super == target->super->super... */
    for (lca = e; *lca; --lca)
    {
        if (q == *lca)
        {
            e = lca - 1; /* do not enter the LCA */
            goto inLCA;
        }
    }
    /* (g) check each me->source__->super->super..for each target...*/
    for (s = q; s; s = TRIGGER(s, HSM_EMPTY_SIG))
    {
        for (lca = e; *lca; --lca)
        {
            if (s == *lca)
            {
                e = lca - 1; /* do not enter the LCA */
                goto inLCA;
            }
        }
        TRIGGER(s, HSM_STATE_EXIT_SIG); /* exit s */
    }
    OSI_LOGE(0x1000a56b, "qhsm HSM_TranDyn error malformed HSM");
inLCA: /* now we are in the LCA of me->source__ and target */
    /* entry fits */
    if (e >= &entry[sizeof(entry) / sizeof(*entry)])
    {
        OSI_LOGE(0x1000a56c, "qhsm HSM_TranDyn error inLCA entry");
    }
    while ((s = *(e--)) != NULL)
    { /*lint !e720*/ /*lint !e661*/      /* retrace the entry path in reverse order */
        TRIGGER(s, HSM_STATE_ENTRY_SIG); /* enter s */
    }
    me->state__ = target; /* update current state */
    while (TRIGGER(target, HSM_STATE_INIT_SIG) == 0)
    {
        /* initial transition must go one level deep */
        if (target != TRIGGER(me->state__, HSM_EMPTY_SIG))
        {
            OSI_LOGE(0x1000a56d, "qhsm HSM_TranDyn error inLCA");
        }
        target = me->state__;
        TRIGGER(target, HSM_STATE_ENTRY_SIG); /* enter target */
    }
}
