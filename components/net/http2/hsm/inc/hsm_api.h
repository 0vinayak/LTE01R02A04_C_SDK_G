/****************************************************************************
** File Name:      hsm_api.h                                                *
** Author:         bruce.chi                                                *
** Date:           2009.1.7                                                 *
** Copyright:      2022 Spreadtrum, Incorporated. All Rights Reserved.      *
** Description:    This file is used to describe the hsm module api         *
*****************************************************************************
**                         Important Edit History                           *
** -------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                              *
** 2009.1        bruce.chi         Create
** 2022.11       chenchen.wang     Modified
**                                                                          *
****************************************************************************/

#ifndef HSM_API_H
#define HSM_API_H

/**-------------------------------------------------------------------------*
**                         Include Files                                    *
**--------------------------------------------------------------------------*/
//#include "sci_types.h"
#include "sci_api.h"
/**-------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/
#ifdef HSM_ASSERT_TRACE_MODE

#define HSM_ASSERT_TRACE SCI_TRACE_HIGH /*assert verified*/
#else

#define HSM_ASSERT_TRACE SCI_TRACE_LOW
#endif

#define HSM_TRACE SCI_TRACE_LOW

#define HSM_XTOR_STATE (HSM_STATE_T)0xFFFFFFFF /* return value when you destroy HSM in HSM_Dispatch() */

//Macros for declaring simple classes (without polymorphism)
#define CLASS(name_)            \
    typedef struct name_ name_; \
    struct name_                \
    {
#define METHODS \
    }           \
    ;
#define END_CLASS

//Abstract base class 'Object', root of all classes with polymorphism
CLASS(Object)
struct ObjectVTABLE *vptr__; /* private virtual pointer */
METHODS
#define ObjectCtor_(me_) ((me_)->vptr__ = &theObjectVTABLE) //, (me_)) /* protected constructor 'inline'... */
#define ObjectXtor_(me_) ((void)0)                          /* protected destructor 'inline'... */
void ObjectAbstract(void);                                  /* dummy implementation for abstract methods RTTI*/
#define ObjectIS_KIND_OF(me_, class_) \
    ObjectIsKindOf__((Object *)(me_), &the##class_##VTABLE)
int ObjectIsKindOf__(Object *me, void *class);
END_CLASS

CLASS(ObjectVTABLE)
ObjectVTABLE *super__;  /* pointer to superclass' VTBL */
void (*xtor)(Object *); /* public virtual destructor */
METHODS
END_CLASS

extern ObjectVTABLE theObjectVTABLE; /* Object class descriptor */

//Macros for declaring subclasses
#define SUBCLASS(class_, superclass_) \
    CLASS(class_)                     \
    superclass_ super;

//Macros for defining VTABLEs and binding virtual functions
#define VTABLE(class_, superclass_)               \
    }                                             \
    ;                                             \
    typedef struct class_##VTABLE class_##VTABLE; \
    extern class_##VTABLE the##class_##VTABLE;    \
    struct class_##VTABLE                         \
    {                                             \
        superclass_##VTABLE super_;

#define BEGIN_VTABLE(class_, superclass_)                   \
    class_##VTABLE the##class_##VTABLE;                     \
    static ObjectVTABLE *class_##VTABLECtor(class_ *t)      \
    {                                                       \
        register class_##VTABLE *me = &the##class_##VTABLE; \
        *(superclass_##VTABLE *)me =                        \
            *(superclass_##VTABLE *)((Object *)t)->vptr__;

#define VMETHOD(class_, meth_) ((class_##VTABLE *)me)->meth_

#define END_VTABLE                                         \
    ((ObjectVTABLE *)me)->super__ = ((Object *)t)->vptr__; \
    return (ObjectVTABLE *)me;                             \
    }

//Macro to hook virtual pointer used in constructors
#define VHOOK(class_)                                         \
    if (((ObjectVTABLE *)&the##class_##VTABLE)->super__ == 0) \
        ((Object *)me)->vptr__ = class_##VTABLECtor(me);      \
    else                                                      \
        ((Object *)me)->vptr__ =                              \
            (ObjectVTABLE *)&the##class_##VTABLE

//Macro to access v-pointer
#define VPTR(class_, obj_) \
    ((class_##VTABLE *)(((Object *)(obj_))->vptr__))

//Macro for invoking virtual class methods and macro for invoking interface methods.
#define VCALL(class_, meth_, obj_) \
   (*VPTR(class_, obj_)->meth_)((class_*)(obj_)
#define END_CALL )

//Find an offset of a member into its class
#define OFFSET_OF(class_, attr_) \
    ((unsigned)&((class_ *)0)->attr_)

/**-------------------------------------------------------------------------*
**                         ENUM DEFINITION                                  *
**--------------------------------------------------------------------------*/
//standard signals
enum
{
    HSM_EMPTY_SIG = 0,
    HSM_STATE_INIT_SIG = 1,
    HSM_STATE_ENTRY_SIG,
    HSM_STATE_EXIT_SIG,
    HSM_USER_SIG
};

/**-------------------------------------------------------------------------*
**                         TYPE DEFINITION                                  *
**--------------------------------------------------------------------------*/
CLASS(HSMEvent_T)
_SIGNAL_VARS
METHODS
END_CLASS

typedef struct
{
    _SIGNAL_VARS
} SignalHeader;

typedef void (*HSM_PseudoState)(void *, HSMEvent_T const *);
typedef HSM_PseudoState (*HSM_State)(void *, HSMEvent_T const *);
typedef HSM_PseudoState HSM_STATE_T; /* return value from a state-handler */
extern HSMEvent_T const pkgStdEvt[]; /* preallocated standard events */

/**-------------------------------------------------------------------------*
**                         STRUCTURE DEFINITION                             *
**--------------------------------------------------------------------------*/

/**-------------------------------------------------------------------------*
**                         PUBLIC FUNCTION DECLARE                          *
**--------------------------------------------------------------------------*/
/* Hierarchical State Machine base class */
SUBCLASS(HSM_C, Object)
HSM_State state__;  /* the active state */
HSM_State source__; /* source state during a transiton */
VTABLE(HSM_C, Object)
METHODS
/* public members */
void HSM_Init(HSM_C *me, HSMEvent_T const *e);            /* initial transition */
HSM_STATE_T HSM_Dispatch(HSM_C *me, HSMEvent_T const *e); /* take RTC step */
int HSM_IsIn(HSM_C const *me, HSM_State state);           /* "is-in" query */
                                                          /* static method (no "me" pointer) */
char const *HSM_GetVersion(void);

/* protected members */
CLASS(Tran_) /* protected inner class Tran_ */
HSM_State chain[8];
unsigned short actions; /* action mask (2-bits for action) */
METHODS
END_CLASS

HSM_C *HSM_Constructor(HSM_C *me, HSM_PseudoState initial); /* Ctor */
void HSM_Destructor(HSM_C *me);                             /* Xtor */

HSM_STATE_T HSM_Top(HSM_C *me, HSMEvent_T const *); /* "top" state */
#define HSM_GetState(me_) ((me_)->state__)
void HSM_TranDyn(HSM_C *me, HSM_State target); /* dynamic transition */
void HSM_TranStatic(HSM_C *me, Tran_ *t, HSM_State target);
#define HSM_STATE_INIT(target_) \
    (((HSM_C *)this_ptr)->state__ = (HSM_State)(target_))

#define HSM_TRAN_STATIC(target_)                                      \
    if (1)                                                            \
    {                                                                 \
        static Tran_ t_;                                              \
        HSM_TranStatic((HSM_C *)this_ptr, &t_, (HSM_State)(target_)); \
    }                                                                 \
    else

#define HSM_TRAN_DYN(target_) \
    HSM_TranDyn((HSM_C *)this_ptr, (HSM_State)(target_))
void HsmTranSetup__(HSM_C *me, Tran_ *t, HSM_State target);
END_CLASS

#endif
