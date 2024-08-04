#ifndef NVITEM_H
#define NVITEM_H

#include "gnss_sci_types.h"
/******************************************************************************
 * Parameter Attribute
 ******************************************************************************/
/*
#define INPUT
#define OUTPUT
#define INOUT
*/
/******************************************************************************
 * Interface Prototype   
 ******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#ifdef CONFIG_SOC_8850
#ifndef NV_GPS_PARAM
#define NV_GPS_PARAM 0x193
#define NVID_CPTSX_NV 0x1b2
#define NVID_APTSX_NV 0x1b4
#if defined(GPS_SUPPORT) && defined(GPS_CHIP_VER_GREENEYE2)
#define DIAG_GPS_F 58
#else
#define DIAG_GPS_F 35
#endif
#endif
#endif
/******************************************************************************
 * Return values of NVITEM_XXX()   
 ******************************************************************************/
typedef enum _NVITEM_ERROR
{
    NVERR_NONE = 0, /* Success */
    NVERR_SYSTEM,   /* System error, e.g. hardware failure */
    NVERR_INVALID_PARAM,
    NVERR_NO_ENOUGH_RESOURCE,
    NVERR_NOT_EXIST,
    NVERR_ACCESS_DENY,
    NVERR_INCOMPATIBLE,
    NVERR_NOT_OPENED
} NVITEM_ERROR_E;

/**
	NVITEM_Read
	
    Description:
        Read an item from the medium.
       
    Paramters:
    	ItemID   - Identifier of the NV item to be read.
    	cchSize  - Size in bytes to be read.
    	pBuf     - Pointer to the buffer to hold the data read.
    
    Return values:
        NVERR_NONE          - The item is read successfully.
        NVERR_NOT_EXIST     - The nvitem does not exist.
        NVERR_INVALID_PARAM - Parameters are invalid, e.g. buf_ptr is NULL, count 
                              is larger than the size of this NV item or the specified
                              Identifier does not exist.
    
**/
//NVITEM_ERROR_E NVITEM_Read(INPUT uint16 ItemID, INPUT uint16 cchSize, OUTPUT void *pBuf);
NVITEM_ERROR_E EFS_NvitemRead(uint16 identifier, uint16 count, uint8 *buf_ptr);

/**
	NVITEM_Write
	    
    Description:
        Write an item to the medium. If the item does not exist, it will be created.
        
    Parameters:
    	ItemID  - Identifier of the item to be written.
    	cchSize - Size in byte of this item.
    	pBuf    - Pointer to the buffer holding the data of the item.
        
    Return values:
        NVERR_NONE               - The item is written successfully.
        NVERR_NO_ENOUGH_RESOURCE - There is no enough resource to complete this 
                                   operation, e.g. no enough space on the medium.
            
        NVERR_INVALID_PARAM      - Parameters are invalid, e.g. buf_ptr is NULL or 
        						   Identifier is invalid.
    
**/
//NVITEM_ERROR_E NVITEM_Write(INPUT uint16 ItemID, INPUT uint16 cchSize,  INPUT const void *pBuf);
NVITEM_ERROR_E EFS_NvitemWrite(uint16 identifier, uint16 count, const uint8 *buf_ptr, uint8 basync);
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* NVITEM_H */
