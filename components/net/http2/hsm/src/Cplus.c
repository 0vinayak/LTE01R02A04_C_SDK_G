/********************************************************************
 * "C+" -- Portable Object-Oriented Extension to C
 * Copyright (c) 2002 Miro Samek, Palo Alto, CA.
 * All Rights Reserved.
 *******************************************************************/
#include "hsm_api.h"

ObjectVTABLE theObjectVTABLE = {
    0,                                 /* no superclass */
    (void (*)(Object *))ObjectAbstract /* purely virtual xtor */
};

int ObjectIsKindOf__(Object *me, void *vtable)
{
    register ObjectVTABLE *vt = NULL;
    for (vt = me->vptr__; vt; vt = vt->super__)
    {
        if (vt == vtable)
        {
            return !0; /*lint !e506*/
        }
    }
    return 0;
}

void ObjectAbstract(void)
{
    OSI_LOGE(0x1000a56e, "qhsm ObjectAbstract error, abstract method should never be called");
}
