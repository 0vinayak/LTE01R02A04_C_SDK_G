/********************************************************************
 * Quantum Framewrok implemenatation (C version)
 * Copyright (c) 2002 Miro Samek, Palo Alto, CA.
 * All Rights Reserved.
 *******************************************************************/
#include "hsm_api.h"
#include "stdio.h"

#if defined WINDOWS_PLATFORM
#include "process.h"
#endif

HSMEvent_T const pkgStdEvt[] = {
    {HSM_EMPTY_SIG, sizeof(HSMEvent_T), 0, 0, 0},
    {HSM_STATE_INIT_SIG, sizeof(HSMEvent_T), 0, 0, 0},
    {HSM_STATE_ENTRY_SIG, sizeof(HSMEvent_T), 0, 0, 0},
    {HSM_STATE_EXIT_SIG, sizeof(HSMEvent_T), 0, 0, 0}};

/*=================================================================*/
void onAssert__(char const *file, unsigned line)
{
    char str[160] = {0};
    snprintf(str, sizeof(str) - 1, "Assertion failed in %s, line %d", file, line);
#if defined WINDOWS_PLATFORM
    EXT_LOG_ID(0, EXT_TRA_SID("qhsm onAssert__ WINDOWS_PLATFORM", 0x27000665));
    exit(-1);
#else
#endif
}
