
#include <stdint.h>
#include "kernel_config.h"
#include "quec_proj_config.h"
#include "FreeRTOS.h"
//#include <stddef.h>

#ifdef CONFIG_TRUSTZONE_SUPPORT
//  <o>Interrupt controller unique priorities
//  <i> The number of unique priorities that can be specified in the ARM Generic Interrupt Controller (GIC).
//  <i> Default: 32
#define configUNIQUE_INTERRUPT_PRIORITIES       16
//  <o>Maximum API call interrupt priority
//  <i> Interrupts assigned a priority at or below this priority can call interrupt safe FreeRTOS API function, and will nest.
//  <i> Interrupts assigned a priority above this priority will not be effected by RTOS critical sections, and will nest,
//  <i> but cannot call any FreeRTOS API functions.
//  <i> Default: 32
#define configMAX_API_CALL_INTERRUPT_PRIORITY   16
#else
#define configUNIQUE_INTERRUPT_PRIORITIES       32
#define configMAX_API_CALL_INTERRUPT_PRIORITY   32
#endif

/* The number of bits to shift for an interrupt priority is dependent on the
number of bits implemented by the interrupt controller. */
#if configUNIQUE_INTERRUPT_PRIORITIES == 16
	#define portPRIORITY_SHIFT 4
	#define portMAX_BINARY_POINT_VALUE	3
#elif configUNIQUE_INTERRUPT_PRIORITIES == 32
	#define portPRIORITY_SHIFT 3
	#define portMAX_BINARY_POINT_VALUE	2
#elif configUNIQUE_INTERRUPT_PRIORITIES == 64
	#define portPRIORITY_SHIFT 2
	#define portMAX_BINARY_POINT_VALUE	1
#elif configUNIQUE_INTERRUPT_PRIORITIES == 128
	#define portPRIORITY_SHIFT 1
	#define portMAX_BINARY_POINT_VALUE	0
#elif configUNIQUE_INTERRUPT_PRIORITIES == 256
	#define portPRIORITY_SHIFT 0
	#define portMAX_BINARY_POINT_VALUE	0
#else
	#error Invalid configUNIQUE_INTERRUPT_PRIORITIES setting.  configUNIQUE_INTERRUPT_PRIORITIES must be set to the number of unique priorities implemented by the target hardware
#endif

#ifndef configUNIQUE_INTERRUPT_PRIORITIES
	#error configUNIQUE_INTERRUPT_PRIORITIES must be defined.  See http://www.freertos.org/Using-FreeRTOS-on-Cortex-A-Embedded-Processors.html
#endif

#ifndef configMAX_API_CALL_INTERRUPT_PRIORITY
	#error configMAX_API_CALL_INTERRUPT_PRIORITY must be defined.  See http://www.freertos.org/Using-FreeRTOS-on-Cortex-A-Embedded-Processors.html
#endif

#if configMAX_API_CALL_INTERRUPT_PRIORITY == 0
	#error configMAX_API_CALL_INTERRUPT_PRIORITY must not be set to 0
#endif

#if configMAX_API_CALL_INTERRUPT_PRIORITY > configUNIQUE_INTERRUPT_PRIORITIES
	#error configMAX_API_CALL_INTERRUPT_PRIORITY must be less than or equal to configUNIQUE_INTERRUPT_PRIORITIES as the lower the numeric priority value the higher the logical interrupt priority
#endif


/* In case security extensions are implemented. */
#if configMAX_API_CALL_INTERRUPT_PRIORITY <= ( configUNIQUE_INTERRUPT_PRIORITIES / 2 )
	#error configMAX_API_CALL_INTERRUPT_PRIORITY must be greater than ( configUNIQUE_INTERRUPT_PRIORITIES / 2 )
#endif

unsigned char ql_port_priority_shift = portPRIORITY_SHIFT;
unsigned char ql_port_max_binary_point_value = portMAX_BINARY_POINT_VALUE;
unsigned char ql_unique_interrupt_priorities  = configUNIQUE_INTERRUPT_PRIORITIES;
unsigned char ql_max_api_call_interrupt_priority = configMAX_API_CALL_INTERRUPT_PRIORITY;
unsigned char ql_thread_local_userdata_id = (configNUM_THREAD_LOCAL_STORAGE_POINTERS - 1);


