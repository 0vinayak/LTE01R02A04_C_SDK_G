#ifndef QXWZ_SDK_OS_H__
#define QXWZ_SDK_OS_H__

#include "gnss_config.h"
//#include "qxwz_sdk_type.h"


/**
 * sleep
 * params:
        time       :    the milliseconds
 * returns:
        0 is ok
 */
unsigned int qx_os_msleep(unsigned int time);

/**
 * get current unix time
 * params:
        
 * returns:
        return current time, in units of millisecond
 */
long long qx_os_gettime();
/**
 * set current time for sdk use only
 * params:
        
 * returns:
        return current time, in units of millisecond
 */
void qx_os_settime(unsigned long long tm);
/**
 * memory alloc
 * params:
        size    :  size of the memory block to be allocated, in units of byte
 * returns:
        if success will return a void pointer to the allocated memory block. NULL if failed.
 */
void *qx_os_malloc(unsigned int size);
/**
 * free memory
 * params:
        
 * returns:
        return current time, in units of millisecond
 */
void qx_os_free(void *p);

void qx_os_memcpy(void *dst, const void *src, unsigned int size);

void qx_os_memset(void *dst, int value, unsigned int size);

/**
 * get memory malloc/free count for debug
 * params:
        
 * returns:
        return the difference between the count of malloc and free
 */
int qx_os_get_memory_log();

/**
 * print debug log
 * params:
        
 * returns:
        Upon successful return, these functions return the number of characters printed,
        If an output error is encountered, a negative value is returned.
 */
int qx_os_printf(const char *format, ...);


#endif

