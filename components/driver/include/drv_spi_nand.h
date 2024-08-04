/******************************************************************************
 ** File Name:      drv_spi_nand.h                                         *
 ** Author:         robert.luo1                                                *
 ** DATE:           07/05/2022                                                *
 ** Copyright:      2022 Spreatrum, Incoporated. All Rights Reserved.         *
 ** Description:                                                              *
 ******************************************************************************/

#ifndef _SPI_NAND_DRV_H_
#define _SPI_NAND_DRV_H_

#include "osi_compiler.h"

/******************************************************************************
                            Bit define
******************************************************************************/
#define BIT_0 0x00000001
#define BIT_1 0x00000002
#define BIT_2 0x00000004
#define BIT_3 0x00000008
#define BIT_4 0x00000010
#define BIT_5 0x00000020
#define BIT_6 0x00000040
#define BIT_7 0x00000080
#define BIT_8 0x00000100
#define BIT_9 0x00000200
#define BIT_10 0x00000400
#define BIT_11 0x00000800
#define BIT_12 0x00001000
#define BIT_13 0x00002000
#define BIT_14 0x00004000
#define BIT_15 0x00008000
#define BIT_16 0x00010000
#define BIT_17 0x00020000
#define BIT_18 0x00040000
#define BIT_19 0x00080000
#define BIT_20 0x00100000
#define BIT_21 0x00200000
#define BIT_22 0x00400000
#define BIT_23 0x00800000
#define BIT_24 0x01000000
#define BIT_25 0x02000000
#define BIT_26 0x04000000
#define BIT_27 0x08000000
#define BIT_28 0x10000000
#define BIT_29 0x20000000
#define BIT_30 0x40000000
#define BIT_31 0x80000000

#if 0
//config spi nand block counts
#define SPI_NAND_BLOCK_COUNTS (4096)
//config spi nand page counts in one block
#define SPI_NAND_PAGE_COUNTS_ONE_BLOCK (64)
//config spi nand sector counts in one block
#define SPI_NAND_SECTOR_COUNTS_ONE_PAGE (4)
//config spi nand  sector counts in nand device
#define SPI_NAND_SECTOR_COUNTS_MAX (SPI_NAND_BLOCK_COUNTS * SPI_NAND_PAGE_COUNTS_ONE_BLOCK * SPI_NAND_SECTORS_COUNTS_ONE_PAGE)
//config spi nand page counts in nand device
#define SPI_NAND_PAGE_COUNTS_MAX (SPI_NAND_BLOCK_COUNTS * SPI_NAND_PAGE_COUNTS_ONE_BLOCK)

//config spi nand page size
#define SPI_NAND_PAGE_SIZE (2048)
//config spi nand oob size
#define SPI_NAND_PAGE_OOB_SIZE (128)
//config spi nand sector size
#define SPI_NAND_SECTOR_SIZE (512)

//max bytes size in one page
#define SPI_NAND_PAGE_WITH_OOB_SIZE (SPI_NAND_PAGE_SIZE + SPI_NAND_PAGE_OOB_SIZE)
//max bytes size in nand device
#define SPI_NAND_BYTES_MAX (SPI_NAND_PAGE_COUNTS_MAX * SPI_NAND_PAGE_SIZE)

//BBT
static uint8_t gSpiNandBadBlockTable[SPI_NAND_BLOCK_COUNTS];
//buffer for read write test
static uint8_t gNandReadBuffer[SPI_NAND_PAGE_WITH_OOB_SIZE];
static uint8_t gNandWriteBuffer[SPI_NAND_PAGE_WITH_OOB_SIZE];
//define test block number min,reserve block0
#define BLOCKNUMMIN (1)
//define test block number max
#define BLOCKNUMMAX (SPI_NAND_BLOCK_COUNTS)
#endif

/*****************SPI NAND FLASH_FEATURE***********************/
#define FEATURE_PROT_ADDR (0xA0)
#define FEATURE_FEATURE_ADDR (0xB0)
#define FEATURE_STATUS_ADDR (0xC0)

//Block lock
#define F_CMP (BIT_1)
#define F_INV (BIT_2)
#define F_BP0 (BIT_3)
#define F_BP1 (BIT_4)
#define F_BP2 (BIT_5)
#define F_BRWD (BIT_7)

//OTP
#define F_QE (BIT_0)
#define F_ECC_EN (BIT_4)
#define F_OTP_EN (BIT_6)
#define F_OTP_PRT (BIT_6)

//Status
#define F_OIP (BIT_0)
#define F_WEL (BIT_1)
#define F_E_FAIL (BIT_2)
#define F_P_FAIL (BIT_3)
#define F_ECCS0 (BIT_4)
#define F_ECCS1 (BIT_5)
#define F_ECCS2 (BIT_6)

#define PAGE_SIZE_512 (512)
#define PAGE_SIZE_1K (1024)
#define PAGE_SIZE_2K (2048)
#define PAGE_SIZE_4K (4096)
#define PAGE_SIZE_8K (8192)

#define MAX_NAND_CHIPS (1)        //don't support multiple nand chips till now
#define MAX_SECTOR_PER_PAGE (8)   //By default all nand flash have a maxinum of 32 sectors per page
#define NAND_SECTOR_SIZE (1024)   //Default nand flash one sector main spare have 1024 byte
#define NAND_MAX_SPARE_SIZE (128) //Default nand flash spare area have maxnum 128 byte
#define NAND_MAX_STATUS_SIZE (56) //Default nand flash status area have maxnum 56 byte
/******************************************************************************
                         Macro define
******************************************************************************/
typedef enum
{
    ERR_NF_SUCCESS = 0,   // Success,no error
    ERR_NF_TIMEOUT = 1,   // Oprate nand flash timeout;
    ERR_NF_ECC_RISK = 2,  // the error bit number is larger than the eccRiskVal
    ERR_NF_ECC_ERROR = 3, // the error number is out of the capacity of the ECC decoder
    ERR_NF_PARAM = 4,     //the paramter error, the top layer need check the paramter
    ERR_NF_FAIL = 5,      // Program or erase nand failed
    ERR_NF_BUSY = 6,      // write error;
    ERR_NF_PROTECTED = 7, // Current device is protected;
    ERR_NF_ECC_RAW = 8
} ERR_NF_E;

#define FLASH_INTFTYPE_MASK 7
#define FLASH_CELLTYPE_MASK 8
typedef enum
{
    NFC_WORK_AUTO_MODE = 0,
    NFC_WORK_ONLY_MASTER_MODE = 1,
    NFC_WORK_ONLY_NAND_MODE = 2,
    NFC_WORK_ONLY_ECC_MODE = 3,
    NFC_WORK_MAX_MODE = 4,
} NFC_MODE;
typedef enum
{
    NANDCTL_ABCD_ABCD = 0,
    NANDCTL_ABCD_DCBA = 1,
    NANDCTL_ABCD_CDAB = 2,
    NANDCTL_ABCD_BADC = 3,
    NANDCTL_ENDIAN_MAX
} NFC_DATA_ENDIAN;

/******************************************************************************
                          Struct define
******************************************************************************/
typedef struct
{
    //#if defined(_NANDC_V2_)
    uint32_t inftype;
    //#endif
    uint16_t acyCle;   //address cycle, which can be set 3,4,5
    uint16_t sctSize;  //sector size
    uint16_t pageSize; //page size, which can be set 512 or 2048, 4096, 8192, 16384
    uint16_t nPgPerBlk;
    uint16_t sctPerPage;
    uint16_t busWidth; //bus width, which can be set 0 or 1
    uint16_t advance;  //advance property, which can be set 0 or 1

    uint16_t spareSize; //spare size,
    uint16_t badPage;   // the page index of bad mark
    uint16_t badSct;
    uint16_t badPos; // the postion of bad mark
    //uint16_t spareSize;        //the spare size of one sector
    uint16_t eccMode;    // 1bit ECC, 4bit ECC,  8bit ECC, 12bit ECC,  16bit ECC, 24bit ECC,
    uint16_t eccPos;     //ECC position
    uint16_t eccRiskVal; //ECC risk value
                         //#if defined(_NANDC_V2_)
    uint16_t sInfoPos;   //spare info postion, when ecc enable this member tell nfc controller the postion of data is protected in spare part
    uint16_t sInfoSize;  //spare info postion, when ecc enable this member tell nfc controller the size of data is protected in spare part
    //#endif
} NAND_PARA_T;

typedef struct
{
    uint16_t nTAcsTime; //Setup time for both ALE and CLE
    uint16_t nTAchTime; //Hold Time for both ALE and CLE
    uint16_t nTRwlTime; //Active low pulse width for both RE and WE
    uint16_t nTRwhTime; //Hold time for both RE and WE
    uint16_t nTRrTime;  //Ready to RE low
} NAND_TIMING_T, *NAND_TIMING_PTR;
typedef struct
{
    uint8_t mode;
    uint8_t eccNum;       //max is 8
    uint16_t sSize;       // spare part size;
    uint16_t eccPos;      //ecc postion
    uint32_t mSize;       //main part size
    uint32_t mainAddr;    // ecc at main data address
    uint8_t *pMbuf;       //main buf pointer
    uint8_t *pSbuf;       // spare buf pointer
    uint8_t *sts;         // save the decoder status
    uint16_t sInfoSize;   //byte length
    uint16_t sInfoPos;    //the offset in spare data
    uint8_t mEccStartIdx; //ecc start index
    uint8_t eccLoc;       // ecc location, 1 is main part, else spare part

} NAND_ECC_PARAM;

typedef struct
{
    uint8_t mCol0;
    uint8_t mCol1;
    uint8_t mRow0;
    uint8_t mRow1;
    uint8_t mRow2;
} NAND_ADDR_MAP;
typedef struct
{
    uint8_t eccMode;
    uint8_t mEccBit;
    uint16_t sctSize;
} NAND_ECC_CONFIG;

typedef struct
{
    uint16_t mEccLen; //bytes length
    uint8_t eccMode;
    bool apart;
    bool wpEn;              //write protect enable, if set 1, write is enable, else write is not enable
    NFC_DATA_ENDIAN endian; //data endian
} NAND_DEV_INFO;

typedef struct nandInfo
{
    uint32_t ID;

    uint32_t nBlkNum;
    uint32_t nNumOfPlane;  //not support yet
    uint32_t nBlkNumInRsv; // NVB

    uint16_t nSctSize;   //sector size
    uint32_t nPageSize;  //page size
    uint32_t nBlkSize;   //block size
    uint16_t nSpareSize; // relate with nSctSize
    uint8_t nSInfoSize;  //  spare info size
    uint16_t nPgPerBlk;
    uint8_t nSctPerPg;
    uint8_t nBlkShift;     //blk size shift
    uint8_t nPageShift;    //page size shift
    uint8_t nSctShift;     //sector size shift
    uint8_t nApart;        //if the main data and sprare data is apart
    uint8_t nOnlyInfoData; //only spare info data move to dst spare buffer
    bool mainMove;         //
    bool spareMove;        //
    uint8_t nWCntPerPg;    // partial program times per page
    uint8_t badPgId;
    uint8_t badSctId;
    uint8_t badPos;      // unit is bytes, lenth is 1 bytes default
    uint8_t badLen;      // bad flag byes lenth
    int8_t safeDataPos;  // data that be protected by ecc(-1 is all reserved area)
    uint8_t safeDataLen; // data that be protected by ecc(0 is none)

    uint8_t mEccPos;        // unit is bytes
    uint8_t mEccBitLen;     // unit is bits
    uint8_t mEccRiskBitNum; // unit is bits,This value must small to ecc capability
    uint8_t sEccPos;        // unit is bytes
    uint8_t sEccBitLen;     // unit is bits
    uint8_t sEccRiskBitNum; // unit is bits,This value must small to ecc capability

    //    host driver need next infomation
    uint8_t nBWidth;   // Nand Organization X8 or X16
    uint8_t nCycleDev; // 3 Cycle, 4 Cycle, 5 Cycle device
    uint8_t nAdvance;  // the property of Advance

    uint16_t nTrTime; // read-time of NAND device (unit: usec)
    uint16_t nTwTime; //write-time of NAND device (unit: usec)
    uint16_t nTeTime; // erase-time of NAND device (unit : usec)
    uint16_t nTfTime; // transfer-time from NAND device to host
} NandInfo_t;

typedef struct
{
    ERR_NF_E(*nandInPageWrite)
    (uintptr_t hwp, uint32_t blkId, uint32_t pageId, uint32_t sctId, uint32_t nScts, uint8_t *pMBuf, uint8_t *pSBuf, uint8_t *pStatus, uint8_t eccEn);
    ERR_NF_E(*nandInPageRead)
    (uintptr_t hwp, uint32_t blkId, uint32_t pageId, uint32_t sctId, uint32_t nScts, uint8_t *pMBuf, uint8_t *pSBuf, uint8_t *pStatus, uint8_t eccEn);
    ERR_NF_E(*nandBlockErase)
    (uintptr_t hwp, uint32_t blkId);
    uint32_t (*nandReadID)(uintptr_t hwp);
    ERR_NF_E(*nandReset)
    (uintptr_t hwp);
    ERR_NF_E(*nandSetFeature)
    (uintptr_t hwp, uint8_t addr, uint8_t data);
    uint32_t (*nandGetFeature)(uintptr_t hwp, uint8_t addr);
    NandInfo_t *(*nandGetNandInfo)(void);
} NandOps_t;

typedef struct nand_device
{
    NandInfo_t *pNandInfo;
    NandOps_t *pNandOpsApi;
} NAND_DEV_T;

#ifdef __cplusplus
extern "C" {
#endif

typedef uint32_t NANDCTL_HANDLE;
/******************************************************************************/
//  Description:   Init nand controller
//  Parameter:
//      nfcClk:   the frequency of nfc module clock
//  Return:
//      ERR_NF_SUCCESS    Init nand controller successfully
/******************************************************************************/
void NANDCTL_Init();

/******************************************************************************/
//  Description:   Open nand controller
//  Parameter:
//      csIndex    the nand flash cs index
//  Return:
//      0:failed
//  other:sucessful, the nand flash handler
/******************************************************************************/
NANDCTL_HANDLE NANDCTL_Open(uint8_t csIndex);

/******************************************************************************/
//  Description:   Set NF parameter according to pNandPara and pNandTiming
//  Parameter:
//            handler                IN  a nandctl handler, return from NANDCTL_Open
//            pNandPara     IN    the parameter of nandflash
//            pNandTiming        IN the nand  timing paramter
//  Return:
//      ERR_NF_SUCCESS    Set NF parameter successfully
//            ERR_NF_PARAM            Error input paramter
/******************************************************************************/
ERR_NF_E NANDCTL_SetParam(NANDCTL_HANDLE handler, NAND_PARA_T *pNandPara, NAND_TIMING_PTR pNandTiming, uint8_t chipidx);

/******************************************************************************/
//  Description:   Get NF parameter according to pNandPara and pNandTiming
//  Parameter:
//            handler                IN  a nandctl handler, return from NANDCTL_Open
//            pNandPara     IN    the parameter of nandflash
//            pNandTiming        IN the nand  timing paramter
//  Return:
//      ERR_NF_SUCCESS    Set NF parameter successfully
//            ERR_NF_PARAM            Error input paramter
/******************************************************************************/
ERR_NF_E NANDCTL_GetParam(NANDCTL_HANDLE handler, NAND_DEV_INFO *info);

/******************************************************************************/
//  Description:   GetNandInfo
//  Parameter:
//      handler        IN  a nandctl handler, return from NANDCTL_Open
//  Return: NandInfo_t pointer

/******************************************************************************/
//ERR_NF_E NANDCTL_GetNandInfo(NANDCTL_HANDLE handler, NandInfo_t *pNandInfo);
NandInfo_t *NANDCTL_GetNandInfo(NANDCTL_HANDLE handler);

/**
 * This function.is an operation to setfeature for nand
 * Parameter:
 * IN handler: nandctl handler, return from NANDCTL_Open
 * IN faddr The address required for the Setfeature operation
 * IN fdata  The data required for the Setfeature operation
 * Return:
 * ERR_NF_SUCCESS: Set feature success
 * ERR_NF_PARAM: Error handler or The Open operation was not perfomed
 * ERR_NF_FAIL: Oprate nand flash fail
 * ERR_NF_TIMEOUT: Oprate nand flash timeout;
**/
ERR_NF_E NANDCTL_Setfeature(NANDCTL_HANDLE handler, uint8_t faddr, uint32_t fdata);

/**
 * This function.is an operation to getfeature for nand
 * Parameter:
 * IN handler: nandctl handler, return from NANDCTL_Open
 * IN faddr The address required for the getfeature operation
 * OUT fdata return the feature data
 * Return:
 * ERR_NF_SUCCESS: Get feature success
 * ERR_NF_PARAM: Error handler or The Open operation was not perfomed
 * ERR_NF_FAIL: Oprate nand flash fail
 * ERR_NF_TIMEOUT: Oprate nand flash timeout;
**/
ERR_NF_E NANDCTL_Getfeature(NANDCTL_HANDLE handler, uint8_t faddr, uint32_t *fdata);

/******************************************************************************/
//  Description:   Close nand controller
//  Parameter:
//      handler        IN  a nandctl handler, return from NANDCTL_Open
//  Return:
//      ERR_NF_SUCCESS    Close nand controller successfully
//            ERR_NF_PARAM            error handler
/******************************************************************************/
ERR_NF_E NANDCTL_Close(NANDCTL_HANDLE handler);

/******************************************************************************/
//  Description:   Read nandflash ID
//  Parameter:
//            handler     IN     a nandctl handler, return from NANDCTL_Open
//      pID      OUT   the address of nandflash id
//  Return:
//      ERR_NF_SUCCESS     Get the ID successfully
//            ERR_NF_PARAM             Error handler or pID is 0
/******************************************************************************/
ERR_NF_E NANDCTL_ReadID(NANDCTL_HANDLE handler, uint8_t *pID);

/******************************************************************************/
//  Description:   Reset NandFlash
//  Parameter:
//      handler     IN     a nandctl handler, return from NANDCTL_Open
//  Return:
//      ERR_NF_SUCCESS     Reset nandflash successfully
//            ERR_NF_PARAM             Error handler
/******************************************************************************/
ERR_NF_E NANDCTL_Reset(NANDCTL_HANDLE handler);

/******************************************************************************/
//  Description:   Read main part and spare part of nand, ECC value can be get
//                 if necessary.
//  Parameter:
//            handler     IN    a nandctl handler, return from NANDCTL_Open
//            blkId            IN the block id
//      pageId    IN   the nand page id.
//            sctId:            IN    the sector index of one page
//            nScts                IN    the sector number
//      pMBuf:       OUT  the address of Main part
//      pSBuf:       OUT  the address of Spare part
//       pStatus         OUT    the sector read status
//      eccEn:       IN  the ecc on or off flag
//  Return:
//    ERR_NF_SUCCESS
//    ERR_NF_TIMEOUT
//    ERR_NF_PARAM
/******************************************************************************/
ERR_NF_E NANDCTL_Read(
    NANDCTL_HANDLE handler,
    uint32_t blkId,
    uint32_t pageId,
    uint32_t sctId,
    uint32_t nScts,
    uint8_t *pMBuf,
    uint8_t *pSBuf,
    uint8_t *pStatus,
    uint8_t eccEn);
/******************************************************************************/
//  Description:   Write main part and spare part of nand
//  Parameter:
//            handler     IN    a nandctl handler, return from NANDCTL_Open
//            blkId            IN the block id
//      pageId    IN   the nand page id.
//            sctId:            IN   the sector index of one page
//            nScts                IN sector number
//      pMBuf:      IN   the address of main part
//      pSBuf:      IN   the address of spare part
//      eccEn:      IN   the flag to enable ECC
//  Return:
//    ERR_NF_SUCCESS
//    ERR_NF_TIMEOUT
//    ERR_NF_FAIL
//    ERR_NF_PARAM
/******************************************************************************/
ERR_NF_E NANDCTL_Write(
    NANDCTL_HANDLE handler,
    uint32_t blkId,
    uint32_t pageId,
    uint32_t sctId,
    uint32_t nScts,
    uint8_t *pMBuf,
    uint8_t *pSBuf,
    uint8_t eccEn,
    uint8_t *pStatus);
/******************************************************************************/
//  Description:   Erase a block of nandflash
//  Parameter:
//            handler     IN    a nandctl handler, return from NANDCTL_Open
//      blkId:  IN   the nand block ID.
//  Return:
//      ERR_NF_SUCCESS    Control nand successfully
//      ERR_NF_TIMEOUT    nand is busy
//      ERR_NF_FAIL       Program or erase nand failed
//            ERR_NF_PARAM            error input paramter
/******************************************************************************/
ERR_NF_E NANDCTL_EraseBlock(NANDCTL_HANDLE handler, uint32_t blkId);

/******************************************************************************/
//  Description:   check, if the block is bad
//  Parameter:
//            handler     IN    a nandctl handler, return from NANDCTL_Open
//      blkId:  IN   the nand block ID.
//            ifGood   OUT    the good or bad flag address
//  Return:
//      ERR_NF_SUCCESS
//      ERR_NF_TIMEOUT
//            ERR_NF_PARAM            error input paramter
/******************************************************************************/
ERR_NF_E NANDCTL_VerifyBlk(NANDCTL_HANDLE handler, uint32_t blkId,
                           uint32_t *ifGood);
/******************************************************************************/
//  Description:   mark the physical block as bad block
//  Parameter:
//            handler     IN    a nandctl handler, return from NANDCTL_Open
//      blkId:  IN   the nand block ID.
//  Return:
//            ERR_NF_SUCCESS
//            ERR_NF_TIMEOUT
//            ERR_NF_FAIL
//            ERR_NF_PARAM            error input paramter
/******************************************************************************/
ERR_NF_E NANDCTL_SetBadBlk(NANDCTL_HANDLE handler, uint32_t blkId);

#endif //DRV_SPI_H_
