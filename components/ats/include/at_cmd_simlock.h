#ifndef __AT_CMD_SIMLOCK_H__
#define __AT_CMD_SIMLOCK_H__

#ifdef CONFIG_SRV_SIMLOCK_ENABLE

#define S_ATC_DISCARD 2
#define S_ATC_SUCCESS 1
#define S_ATC_FAIL 0
#define ATC_STATUS uint32_t

#ifdef CONFIG_SOC_8850

#define SIMLOCK_RPC_MAX_SIZE 512
#define SIMLOCK_DATA_MAX_SIZE 12 * 1024

typedef struct
{
    uint8_t *data;
    uint32_t length;
} simlock_data_async_t;

#else

typedef struct
{
    uint32_t pos;
    uint32_t size;
    char *mem;
} simlockAsyncCtx_t;

#endif

#endif

#endif