/********************************************************************
 * Quantum Hierarchical State Machine declarations (C version)
 * Copyright (c) 2002 Miro Samek, Palo Alto, CA.
 * All Rights Reserved.
 *******************************************************************/
#include "hsm_api.h"
/* helper macro */
#define TRIGGER(state_, sig_) \
    (HSM_State) (*(state_))(me, &pkgStdEvt[sig_])

const char *HSM_GetVersion(void)
{
    return "HSM_C 2.2.2";
}

BEGIN_VTABLE(HSM_C, Object)
VMETHOD(Object, xtor) = (void (*)(Object *))HSM_Destructor;
END_VTABLE
//...................................................................
HSM_C *HSM_Constructor(HSM_C *me, HSM_PseudoState initial)
{
    ObjectCtor_(&me->super);
    VHOOK(HSM_C);
    me->state__ = (HSM_State)HSM_Top;
    me->source__ = (HSM_State)initial;
    return me;
}
//...................................................................
void HSM_Destructor(HSM_C *me)
{
    ObjectXtor_(&me->super);
}
//...................................................................
HSM_STATE_T HSM_Top(HSM_C *me, HSMEvent_T const *e)
{
    return 0;
}
//...................................................................
void HSM_Init(HSM_C *me, HSMEvent_T const *e)
{
    register HSM_State s = PNULL;
    if (!(me->state__ == (HSM_State)HSM_Top && /* HSM not executed */
          me->source__))                       /* we are about to dereference source__ */
    {
        OSI_LOGE(0x1000a56f, "qhsm HSM_Init HSM not executed");
        return;
    }
    s = me->state__;                         /* save me->state__ in a temporary */
    (*(HSM_PseudoState)me->source__)(me, e); /* top-most initial tran. */
                                             /* initial transition must go one level deep */
    if (s != TRIGGER(me->state__, HSM_EMPTY_SIG))
    {
        OSI_LOGE(0x1000a570, "qhsm HSM_Init error first");
    }
    s = me->state__;                           /* update the temporary */
    (*s)(me, &pkgStdEvt[HSM_STATE_ENTRY_SIG]); /* enter the state */
    while ((*s)(me, &pkgStdEvt[HSM_STATE_INIT_SIG]) == 0)
    { /* init handled? */
        /* initial transition must go one level deep */
        if (s != TRIGGER(me->state__, HSM_EMPTY_SIG))
        {
            OSI_LOGE(0x1000a571, "qhsm HSM_Init error second");
        }
        s = me->state__;
        (*s)(me, &pkgStdEvt[HSM_STATE_ENTRY_SIG]); /* enter the substate */
    }
}
//...................................................................
int HSM_IsIn(HSM_C const *me, HSM_State state)
{
    register HSM_State s = PNULL;
    for (s = me->state__; s;                                                 /* traverse the state hierarchy */
         (HSM_State)(*me->source__)((HSM_C *)me, &pkgStdEvt[HSM_EMPTY_SIG])) /*lint !e521*/
    {
        if (s == state)
        {                            /* do the states match? */
            return 1; /*lint !e506*/ /* match found, return true */
        }
    }
    return 0; /* no match found, return false */
}
//...................................................................
void HSM_TranStatic(HSM_C *me, Tran_ *tran, HSM_State target)
{
    register HSM_State s = PNULL;
    if (target == (HSM_State)HSM_Top) /* cannot target top state */
    {
        OSI_LOGE(0x1000a572, "qhsm HSM_TranStatic cannot target top state");
    }
    for (s = me->state__; s != me->source__;)
    {
        HSM_State t = PNULL;
        /* we are about to dereference s */
        if (0 == s)
        {
            OSI_LOGE(0x1000a573, "qhsm HSM_TranStatic error");
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
    if (tran->chain[0] == 0)
    {                                     /* is the tran object initialized? */
        HsmTranSetup__(me, tran, target); /* setup the transition */
    }
    else
    { /* transition initialized, execute transition chain */
        register HSM_State *c = &tran->chain[0];
        register unsigned short a;
        for (a = tran->actions; a; a >>= 2, ++c)
        {
            (*(*c))(me, &pkgStdEvt[a & 3]);
        }
        me->state__ = *c;
    }
}
//...................................................................
HSM_STATE_T HSM_Dispatch(HSM_C *me, HSMEvent_T const *e)
{

    HSM_State state = 0;

    for (me->source__ = me->state__; me->source__;)
    {
        state = (HSM_State)(*me->source__)(me, e);
        if (state == (HSM_State)HSM_XTOR_STATE)
        {
            return HSM_XTOR_STATE;
        }
        else
        {
            me->source__ = state;
        }
    }

    return (HSM_STATE_T)state;
}
//...................................................................
void HsmTranSetup__(HSM_C *me, Tran_ *tran, HSM_State target)
{
    HSM_State entry[8], p, q, s, *c, *e, *lca;
    unsigned short a = 0;

#define RECORD(state_, sig_)        \
    if (TRIGGER(state_, sig_) == 0) \
    {                               \
        a |= ((sig_) << 14);        \
        a >>= 2;                    \
        *c++ = (state_);            \
    }                               \
    else                            \
        ((void)0)

    c = &tran->chain[0];
    *(e = &entry[0]) = 0;
    *(++e) = target; /* assume entry to target */

    /* (a) check source == target (transition to self) */
    if (me->source__ == target)
    {
        RECORD(me->source__, HSM_STATE_EXIT_SIG); /* exit source */
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
        RECORD(me->source__, HSM_STATE_EXIT_SIG); /* exit source */
        goto inLCA;
    }
    /* (d) check me->source->super == target */
    if (q == target)
    {
        RECORD(me->source__, HSM_STATE_EXIT_SIG); /* exit source */
        --e;                                      /* do not enter the LCA */
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
    RECORD(me->source__, HSM_STATE_EXIT_SIG); /* exit source */
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
        RECORD(s, HSM_STATE_EXIT_SIG); /* exit s */
    }
    OSI_LOGE(0x1000a574, "qhsm HsmTranSetup__ error");
inLCA: /* now we are in the LCA of me->source__ and target */
    /* entry fits */
    if (e >= &entry[sizeof(entry) / sizeof(*entry)])
    {
        OSI_LOGE(0x1000a575, "qhsm HsmTranSetup__ inLCA error entry");
    }
    while ((s = *(e--)) != NULL)
    { /*lint !e720*/ /*lint !e661*/     /* retrace the entry path in reverse order */
        RECORD(s, HSM_STATE_ENTRY_SIG); /* enter s */
    }
    me->state__ = target; /* update current state */
    while (TRIGGER(target, HSM_STATE_INIT_SIG) == 0)
    {
        /* initial transition must go one level deep*/
        if (target != TRIGGER(me->state__, HSM_EMPTY_SIG))
        {
            OSI_LOGE(0x1000a576, "qhsm HsmTranSetup__ inLCA error");
        }
        a |= (HSM_STATE_INIT_SIG << 14);
        a >>= 2;
        *c++ = target;
        target = me->state__;
        RECORD(target, HSM_STATE_ENTRY_SIG); /* enter target */
    }
#undef RECORD
    *c = target;
    tran->actions = a >> (14 - (c - &tran->chain[0]) * 2); /*lint !e504*/
    if (!(tran->chain[0] != 0 &&                           /* transition initialized */
          c < &tran->chain[sizeof(tran->chain) /
                           sizeof(*tran->chain)])) /* chain fits in */
    {
        OSI_LOGE(0x1000a577, "qhsm HsmTranSetup__ inLCA transition initialized");
    }
}
