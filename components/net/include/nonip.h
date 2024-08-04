#ifndef _NONIP_H_
#define _NONIP_H_

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "drv_ps_path.h"

#ifdef __cplusplus
extern "C" {
#endif
typedef struct NonIPSession NonIPSession_t;

typedef void (*nonIPDataArriveCB_t)(void *ctx, NonIPSession_t *nonIPSession);

struct NonIPSession
{
    uint8_t simID;
    uint8_t cID;
    uint8_t used;
    drvPsIntf_t *dataPsPath;
    void *cb_ctx;
    nonIPDataArriveCB_t cb;
};

NonIPSession_t *getNonIPSession(uint8_t nSim, uint8_t nCid);
int8_t NonIPDataSend(uint8_t nSimId, uint8_t nCid, const uint8_t *data, uint32_t len, uint8_t rai, int seqno);
int32_t NonIPDataRecv(uint8_t nSimId, uint8_t nCid, uint8_t *data, uint32_t len);
void NonIPSessionCreate(uint8_t nSimId, uint8_t nCid, nonIPDataArriveCB_t cb, void *cb_ctx);
void NonIPSessionDestory(uint8_t nSimId, uint8_t nCid);
bool NonIPSessionSetDataArriveCB(uint8_t nSimId, uint8_t nCid, nonIPDataArriveCB_t cb, void *cb_ctx);

#ifdef __cplusplus
}
#endif
#endif
