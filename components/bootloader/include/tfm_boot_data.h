#ifndef __TFM_BOOT_DATA_H__
#define __TFM_BOOT_DATA_H__

int bootStatusToSharedData(void);
int swComponentToSharedData(char *swComponent, simageHeader_t *header);
#endif
