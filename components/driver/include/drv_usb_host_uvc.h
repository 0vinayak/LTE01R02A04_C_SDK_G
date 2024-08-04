/* Copyright (C) 2023 UNISOC Technologies Limited and/or its affiliates("UNISOC").
 * All rights reserved.
 *
 * This software is supplied "AS IS" without any warranties.
 * UNISOC assumes no responsibility or liability for the use of the software,
 * conveys no license or title under any patent, copyright, or mask work
 * right to the product. UNISOC reserves the right to make changes in the
 * software without notification.  UNISOC also make no representation or
 * warranty that such application will be suitable for the specified use
 * without further testing or modification.
 */

#ifndef _DRV_USB_HOST_H_
#define _DRV_USB_HOST_H_

/**
 * @brief read data from the uvc
 *
 * @param tem     the serial
 * @param data_length      the size of the buffer
 * @return
 *      - (-1)       fail
 *      - (positive) the bytes of data actually read
 */
int transfer_data(unsigned char *tem, int data_length);

#endif
