#ifndef _PAM_WORKER_H_
#define _PAM_WORKER_H_

#include <stdint.h>
#include <stdbool.h>
#include "osi_api.h"
#include "osi_event_hub.h"
#include "osi_mem_recycler.h"
#include "pam_config.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef CONFIG_PAM_LTE_GNSS_WIFISCAN_SUPPORT

#define RETURN_IF_NULL(VarPointer) OSI_DO_WHILE0(if (pamVarIsNullPointer(VarPointer)) return; else break;)

typedef enum
{
    IGNORE_VALUE = 0,
    AUTO_GNSS_ON,
    USER_GNSS_ON,
    AUTO_GNSS_OFF,
    USER_GNSS_OFF,
    _PAM_END
} PAM_GNSS_SWITCH_CTRL;

typedef enum
{
    PAM_NO_WORK,
    PAM_GNSS_WORK,
    PAM_WIFISCAN_WORK,
    PAM_LTE_WORK,
} PAM_WORK_TYPE;

typedef struct
{
    uint32_t ev_id;
    bool auto_remove;
    osiThread_t *thread;
    osiEventCallback_t cb;
    void *cb_ctx;
} pamEvidRegistry_t;

#define CONFIG_PAM_PENDING_EVID_COUNT 64

typedef struct
{
    osiMutex_t *lock;
    osiThread_t *main_thread;
    pamEvidRegistry_t pending_evid_regs[CONFIG_PAM_PENDING_EVID_COUNT];
    uint16_t pending_evid_count;
} pamGnssContext_t;

typedef struct
{
    osiThread_t *pamThread;
    osiThread_t *pamDelayThread;
    osiEventDispatch_t *id_man; // register by event ID
    osiEventHub_t *event_hub;   // static dispatch
    osiMemRecycler_t *mem_recycler;
} pamEngine_t;

typedef struct
{
    uint8_t state;          //machine state
    uint8_t gnssCtrlSwitch; //user stop or sys stop
    PAM_WORK_TYPE workType; // 1:gnss, 2:wifiscan, 3:lte, others:reserve
} pamSetting_t;
extern pamSetting_t pamInfo;

/**
  * @brief create binding event callback handler.
  */
void pamEngineInit(void);

/**
  * @brief set user ctrl on/off or auto ctrl on/off gnss.
  * @param onType:AUTO_GNSS_ON, USER_GNSS_ON
  *               offType:AUTO_GNSS_OFF, USER_GNSS_OFF
  */
void pamSetGnssSwitchCtrl(PAM_GNSS_SWITCH_CTRL onType, PAM_GNSS_SWITCH_CTRL offType);

/**
  * @brief get gnss on/off switch ctrl value.
  * @return: gnssCtrlSwitch&0x0F:onType user or auto
  *                gnssCtrlSwitch&0xF0:offType user or auto
  * enum PAM_GNSS_SWITCH_CTRL
  */
uint8_t pamGetGnssSwitchCtrl(void);

/**
  * @brief set pam module work type.
  * @param type value from PAM_WORK_TYPE
  */
void pamSetModuleWorkType(uint8_t type);

/**
  * @brief get pam module work type.
  * @return workType
  *              PAM_NO_WORK,
  *              PAM_GNSS_WORK,
  *              PAM_WIFISCAN_WORK,
  *              PAM_LTE_WORK,
  */
uint8_t pamGetModuleWorkType(void);

/*****************************************
** pamGetGnssStatus
** Description: get gnss status
** return:
        typedef enum
        {
            GPS_STATUS_NOT_INIT = 0x00,
            GPS_STATUS_INIT,
            GPS_STATUS_OPEN,
            GPS_STATUS_SLEEP,
            GPS_STATUS_LOST,
        #ifdef CONFIG_SOC_8850
            GPS_STATUS_CLOSING,
        #endif
            GPS_STATUS_CLOSE,
            GPS_STATUS_IDLEOFF,
        #if defined(GPS_CHIP_VER_GREENEYE2)
            GPS_STATUS_POWEROFF,
        #endif
            GPS_STATUS_MAX
} GPS_STATUS_E;
********************************************/
uint8_t pamGetGnssStatus();

/**
  * @brief send event to others task,
  *    taskid from pamCreateAsyncReg func
  */
void pamWorkerSendEvent(const osiEvent_t *event);

/**
  * @brief reveive event is distributed to the 
  *    callback function correspondif to the event
  */
bool pamInvokeEvidCallback(const osiEvent_t *event);

/**
  * @brief release event id
  */
void pamReleaseEvid(uint32_t ev_id_cnf);

/**
  * @brief pam worker init config gPamGnssCtx.
  */
void pamWorkerInit(void);

/**
  * @brief get event name
  * @return event name
  */
const char *pamEventName(uint32_t id);

/**
  * @brief Register event handler.
  */
bool pamEventsRegister(uint32_t id, ...);

/**
  * @brief urc display text.
  */
void pamRespSimUrcText(uint8_t nSim, const char *text);

/**
  * @brief if VarPointer is null, return.
  */
bool pamVarIsNullPointer(void *VarPointer);

/**
  * @brief entry pam thread callback.
  */
void pamThreadCallback(osiCallback_t cb, void *ctx);

/**
  * @brief mapping the id of the reveived and sent event.
  * @param ID of the event to send.
  * @return ID of the event to be received.
  */
uint32_t pamMapEventId(uint32_t eventId);

/**
  * @brief get pam thread id.
  */
osiThread_t *pamGetTaskID(void);

/**
  * @brief get pam_delay thread id.
  */
osiThread_t *pamGetDelayTaskID(void);

/**
  * @brief Entry pam_delay task callback.
  */
void pamDelayThreadCallback(osiCallback_t cb, void *ctx);

/**
  * @brief create a callback function for asynchronous events.
  * @param cb: callback function
  * @param cb_ctx: config value
  * @param thread: task id
  * @param auto_remove: auto release  received id.
  * @param event_id: ID of the event to send
  */
uint32_t pamCreateAsyncReg(osiEventCallback_t cb, void *cb_ctx, osiThread_t *thread, bool auto_remove, uint32_t event_id);

/**
 * @brief gnss and cfw event distribution control.
 */
bool pamIsHandleCfwOrGnssCB(const osiEvent_t *event);

/**
 * @brief Determine whether there it's an indicator event .
 */
bool pamIsCfwOrGnssIndicate(uint32_t nEventId);

/**
 * @brief Determine whether there it's a pam event .
 */
bool pamIsPamEvent(uint32_t nEventId);

/**
 * @brief send event data to others task.
 * @param event: event data.
 * @param hsl_gnss_flag: gnss event or cfw event flage.
 */
void pamSendEvent(osiEvent_t *event, uint8_t hsl_gnss_flag);

/**
  * @brief init pam engine and worker 
  *            create pamThread and pamDelayThread task.
  */
void pamStart(void);

// ===========================from mal handle==========================================

#define PAM_THREAD_STACK_SIZE (2048)
#define PAM_THREAD_EVENT_COUNT (50)

typedef void (*pamContextDelete_t)(void *ctx);

typedef enum
{
    PAM_BINDER_UNKNOWN = 0,
    PAM_BINDER_UTI,
} pamBinderType_t;

typedef struct
{
    pamBinderType_t type;
    uint16_t uti;
    uint32_t event_id_cnf;
    void *ctx;
    pamContextDelete_t ctx_delete;
    osiSemaphore_t *wait_sema;
} pamBinderaction_t;

typedef struct
{
    pamGnssContext_t gPamGnssCtx;
    pamBinderaction_t gPamBinderCtx;
} pamContext_t;

/**
 * \brief transaction event callback function prototype
 *
 * The callback will be invoked in separated thread.
 */
typedef void (*pamBinderCallback_t)(pamBinderaction_t *binder, const osiEvent_t *event);

/**
 * \brief macro to terminate transaction and return error code
 */
#define PAM_BINDER_RETURN_ERR(binder, err) OSI_DO_WHILE0(pamTerminateBinder(binder); return (err);)

void pamSetBinderContext(pamBinderaction_t *binder, void *ctx, pamContextDelete_t destroy);

pamBinderaction_t *pamCreateBinder(void);

void pamStartAsyncBinder(pamBinderaction_t *binder, pamBinderCallback_t cb, uint32_t nEventId, bool auto_remove);

uint16_t pamStartNoWaitUtiBinder(void);

void pamAbortBinder(pamBinderaction_t *binder);

void pamTerminateBinder(pamBinderaction_t *binder);

uint16_t pamBinderUti(pamBinderaction_t *binder);

void pamBinderFinished(pamBinderaction_t *binder);

bool pamBinderWait(pamBinderaction_t *binder, unsigned timeout);

void pamContextFree(void *ctx);

void *pamBinderContext(pamBinderaction_t *binder);

#endif //CONFIG_PAM_LTE_GNSS_WIFISCAN_SUPPORT
#endif //_PAM_DISPATCH_H_
