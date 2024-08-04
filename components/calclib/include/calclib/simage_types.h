/* Copyright (C) 2017 RDA Technologies Limited and/or its affiliates("RDA").
 * All rights reserved.
 *
 * This software is supplied "AS IS" without any warranties.
 * RDA assumes no responsibility or liability for the use of the software,
 * conveys no license or title under any patent, copyright, or mask work
 * right to the product. RDA reserves the right to make changes in the
 * software without notification.  RDA also make no representation or
 * warranty that such application will be suitable for the specified use
 * without further testing or modification.
 */

#ifndef _SIMAGE_TYPES_H_
#define _SIMAGE_TYPES_H_

#include "osi_compiler.h"

OSI_EXTERN_C_BEGIN

#define SIMAGE_HEADER_MAGIC (0x52484953) // SIHR in little endian
#define SIMAGE_FORMAT_VERSION_V1 (1)     // vesion 1
#define SIMAGE_FORMAT_VERSION_V2 (2)     // vesion 2
#define SIMAGE_HEADER_CHECKSUM_OFFSET (sizeof(uint32_t) + sizeof(osiBytes32_t))
#define SIMAGE_HEADER_SIZE (256)
#define SIMAGE_HEADER_NAME_LEN (64)
#define SIMAGE_HEADER_CHECK_CRC (0)
#define SIMAGE_HEADER_CHECK_SHA256 (1)
#define SIMAGE_HEADER_CHECK_SM3 (2)
#define SIMAGE_ALIGNMENT (64)
#define SIMAGE_CERTTYPE_KEY (1)
#define SIMAGE_CERTTYPE_DEBUG (2)
#define SIMAGE_CERTTYPE_DEVELOP (3)
#define SIMAGE_CERTTYPE_CHAIN (4)
#define SIMAGE_CERTTYPE_CHAIN_SOC (5)
#define SIMAGE_CERTTYPE_RMA (6)
#define SIMAGE_CERTTYPE_FILELIST (7)
#define SIMAGE_PKATYPE_ECC (0)
#define SIMAGE_PKATYPE_RSA (1)
#define SIMAGE_PKATYPE_RSA_PSS (2)
#define SIMAGE_PKATYPE_SM2 (3)
#define SIMAGE_HASHTYPE_SHA256 (0)
#define SIMAGE_HASHTYPE_SM3 (1)
#define SIMAGE_DATA_CRYPT_NONE (0)
#define SIMAGE_DATA_CRYPT_SM4_KCE (1)
#define SIMAGE_SIGN_DATA_OFFSET (sizeof(simagePubkey_t) + sizeof(simageSignature_t))

#define SIMAGE_HEADER_CHECK_TYPE(type) ((type)&0xf)       // header->header_flags
#define SIMAGE_DATA_CRYPT_TYPE(type) ((type)&0xf)         // header->data_flags
#define SIMAGE_CERT_PKA_TYPE(type) ((type)&0xf)           // cert->pk_type
#define SIMAGE_CERT_HASH_TYPE(type) (((type) >> 8) & 0xf) // cert->pk_type
#define SIMAGE_CERT_ENABLE_DOWNLOAD_MASK (1U << 31)

#define SIMAGE_ECC_CURVE_secp160k1 (0)
#define SIMAGE_ECC_CURVE_secp160r1 (1)
#define SIMAGE_ECC_CURVE_secp160r2 (2)
#define SIMAGE_ECC_CURVE_secp192k1 (3)
#define SIMAGE_ECC_CURVE_secp192r1 (4)
#define SIMAGE_ECC_CURVE_secp224k1 (5)
#define SIMAGE_ECC_CURVE_secp224r1 (6)
#define SIMAGE_ECC_CURVE_secp256k1 (7)
#define SIMAGE_ECC_CURVE_secp256r1 (8)
#define SIMAGE_ECC_CURVE_secp384r1 (9)
#define SIMAGE_ECC_CURVE_secp521r1 (10)

/**
 * \brief function prototype of signed image data entry
 */
typedef void (*simageEntry_t)(uint32_t param);

/**
 * \brief ecc public key
 */
typedef struct
{
    osiBytes80_t x; ///< ecp x, real bits depends on curve
    osiBytes80_t y; ///< ecp y, real bits depends on curve
    uint32_t curve; ///< curve type, \p SIMAGE_ECC_CURVE_.
} simageEccPubkey_t;

/**
 * \brief ecc signature
 */
typedef struct
{
    osiBytes80_t r; ///< signature r, real bits depends on curve
    osiBytes80_t s; ///< signature s, real bits depends on curve
} simageEccSignature_t;

/**
 * \brief sm2 public key
 */
typedef struct
{
    osiBytes80_t x; ///< ecp x, real bits depends on curve
    osiBytes80_t y; ///< ecp y, real bits depends on curve
} simageSm2Pubkey_t;

/**
 * \brief sm2 signature
 */
typedef struct
{
    osiBytes80_t r; ///< signature r, real bits depends on curve
    osiBytes80_t s; ///< signature s, real bits depends on curve
} simageSm2Signature_t;

/**
 * \brief RSA public key
 */
typedef struct
{
    uint32_t n_bytes; ///< bytes of N
    uint32_t e;       ///< E, 32bits at most
    osiBits4096_t n;  ///< N, 4096 bits at most
} simageRsaPubkey_t;

/**
 * \brief RSA signature
 */
typedef struct
{
    uint32_t bytes;    ///< number of bytes, should be the same of N
    osiBits4096_t sig; ///< signature data
} simageRsaSignature_t;

/**
 * \brief union of RSA and ECC public key
 */
typedef union {
    simageRsaPubkey_t rsa; ///< RSA public key
    simageEccPubkey_t ecc; ///< ECC public key
    simageSm2Pubkey_t sm2; ///< SM2 public key
} simagePubkey_t;

/**
 * \brief union of RSA and ECC signature
 */
typedef union {
    simageRsaSignature_t rsa; ///< RSA signature
    simageEccSignature_t ecc; ///< ECC signature
    simageSm2Signature_t sm2; ///< SM2 signature
} simageSignature_t;

/**
 * \brief Cert common header
 *
 * The header of certs is the public key and signature. The signature
 * is calculated for the hash (SHA256) of all data followed:
 * <tt>header->cert_size - SIMAGE_SIGN_DATA_OFFSET</tt>.
 */
typedef struct
{
    simagePubkey_t pubkey; ///< Public key.
    simageSignature_t sig; ///< Signature.
    uint32_t cert_type;    ///< Cert type, refer to \p SIMAGE_CERTTYPE_.
    uint32_t pk_type;      ///< [3:0] PKA type, [11:8] hash type
    uint32_t cert_size;    ///< Total cert type.
} simageCertHeader_t;

/**
 * \brief key cert
 */
typedef struct
{
    simageCertHeader_t header;  ///< Cert header
    osiBits256_t hash_data;     ///< hash of data, to verify data.
    osiBits256_t hash_next_key; ///< hash of next public key in trust chain. Hash method follows chained cert.
    uint32_t security_version;  ///< Security version, for anti-rollback.
    uint32_t data_size;         ///< Data size. It should match \p data_size in image header.
    uint32_t data_flags;        ///< Data flags. It should match \p data_flags in image header.
    uint32_t data_load;         ///< Data load address. It should match \p data_load in image header.
    uint32_t data_load_hi;      ///< Data load address [63:32]. It should match \p data_load_hi in image header.
    uint32_t data_entry;        ///< Data entry address. It should match \p data_entry in image header.
    uint32_t data_entry_hi;     ///< Data entry address [63:32]. It should match \p data_entry_hi in image header.
    uint32_t zero[3];           ///< Padding to make cert 64B aligned.
} simageKeyCert_t;

/**
 * \brief debug cert
 */
typedef struct
{
    simageCertHeader_t header;   ///< Cert header
    osiBits256_t hash_devel_key; ///< hash of next public key in trust chain. Hash method follows chained cert.
    uint32_t debug_mask;         ///< Debug enable mask.
    uint32_t zero[17];           ///< Padding to make cert 64B aligned.
} simageDebugCert_t;

/**
 * \brief developer cert
 */
typedef struct
{
    simageCertHeader_t header; ///< Cert header
    osiBits256_t soc_id;       ///< SOC id. Hash method follows this cert.
    uint32_t debug_mask;       ///< Debug enable mask.
    uint32_t zero[17];         ///< Padding to make cert 64B aligned.
} simageDevelCert_t;

/**
 * \brief chain cert
 */
typedef struct
{
    simageCertHeader_t header;  ///< Cert header
    osiBits256_t hash_next_key; ///< hash of next public key in trust chain. Hash method follows chained cert.
    osiBits256_t soc_id;        ///< Optional SOC id. Hash method follows this cert.
    uint32_t zero[10];          ///< Padding to make cert 64B aligned.
} simageChainCert_t;

/**
 * \brief cert for rma bits write enable
 */
typedef struct
{
    simageCertHeader_t header; ///< Cert header
    osiBits256_t soc_id;       ///< SOC id. Hash method follows this cert.
    uint32_t rma_mask;         ///< RMA bits write enable mask.
    uint32_t zero[17];         ///< Padding to make cert 64B aligned.
} simageRmaCert_t;

/**
 * \brief file list cert
 *
 * File list cert is not fixed size. More name and hash combination can
 * be followed.
 */
typedef struct
{
    simageCertHeader_t header; ///< Cert header
    uint8_t info[40];          ///< Arbitrary string information.
    uint8_t name[32];          ///< File name.
    osiBits256_t hash;         ///< File hash. Hash method follows this cert.
} simageFilelistCert_t;

/**
 * \brief signed image header
 *
 * The structure of signed image is:
 * - header
 * - data
 * - private data
 * - key cert
 * - private data cert
 * - debug primary cert
 * - debug devel cert
 * - chain cert
 *
 * Only header is mendatory, all others are optional. The cert names are for
 * bootloader. For other images, most likely, debug certs won't appear.
 */
typedef struct simageHeader
{
    uint32_t magic;                ///< Magic number: SIHR
    osiBytes32_t header_checksum;  ///< Image header checksum. Checksum calculation offset is \p SIMAGE_HEADER_CHECKSUM_OFFSET.
    uint32_t image_size;           ///< Total image size
    uint8_t image_name[64];        ///< Image name. The terminate NUL should be included
    uint16_t format_version;       ///< Image format version.
    uint16_t ver_patch;            ///< Version patch level.
    uint16_t ver_minor;            ///< Minor version
    uint16_t ver_major;            ///< Major version
    uint32_t revision;             ///< Revision information. Usually, it is SVN revision number or the first 4 bytes of git hash.
    uint32_t platform;             ///< Platform ID, 0 for unknown
    uint32_t timestamp;            ///< Creation timestamp, seconds since epoch
    uint32_t header_size;          ///< Header size. Currently, 256B is used, other sizes are reserved for future.
    uint32_t header_flags;         ///< Header flags. Currently, [3:0] is used to indicate checksum method
    osiBytes32_t payload_checksum; ///< Payload (data after header) checksum. Fill zero when there are no payload
    uint32_t data_offset;          ///< Data offset in the image, from the beginning of image.
    uint32_t data_size;            ///< Data size, not including padding. 0 for not exist.
    uint32_t data_flags;           ///< Data flags. [3:0] for encryption type
    uint32_t data_load;            ///< Data load address.
    uint32_t data_load_hi;         ///< Data load address [63:32]
    uint32_t data_entry;           ///< Data entry address.
    uint32_t data_entry_hi;        ///< Data entry address [63:32]
    uint32_t priv_data_offset;     ///< Private data offset in the image, from the beginning of image.
    uint32_t priv_data_size;       ///< Private data size, not including padding. 0 for not exist.
    uint32_t priv_data_flags;      ///< Private data flags, reserved for future.
    uint32_t keycert_offset;       ///< Key cert offset in the image, from the beginning of image.
    uint32_t keycert_size;         ///< Key cert size, 0 for not exist.
    uint32_t privcert_offset;      ///< Private data cert offset in the image, from the beginning of image.
    uint32_t privcert_size;        ///< Private data cert size, 0 for not exist.
    uint32_t debugcert_offset;     ///< Debug cert offset in the image, from the beginning of image
    uint32_t debugcert_size;       ///< Debug cert size, 0 for not exist.
    uint32_t develcert_offset;     ///< Developer debug cert offset in the image, from the beginning of image
    uint32_t develcert_size;       ///< Developer debug cert size, 0 for not exist.
    uint32_t chaincert_offset;     ///< Chain cert offset in the image, from the beginning of image.
    uint32_t chaincert_size;       ///< Chain cert size, 0 for not exist.
    uint32_t zero[3];              ///< Zero padding to 256 bytes.
} simageHeader_t;

/**
 * \brief helper to check whether magic matches simage.
 *
 * \param ptr pointer to be checked
 * \return
 *      - true if magic matches
 *      - false if magic mismatches
 */
static inline bool simageMagicMatch(const void *ptr)
{
    const simageHeader_t *header = (const simageHeader_t *)ptr;
    return header->magic == SIMAGE_HEADER_MAGIC;
}

/**
 * \brief helper to get keycert by simage header
 *
 * \param header simage header
 * \return keycert, NULL for not exist
 */
static inline const simageKeyCert_t *simageGetKeyCert(const simageHeader_t *header)
{
    if (header->keycert_offset == 0)
        return NULL;

    uintptr_t ptr = (uintptr_t)header + header->keycert_offset;
    return (const simageKeyCert_t *)ptr;
}

/**
 * \brief helper to get chaincert by simage header
 *
 * \param header simage header
 * \return chaincert, NULL for not exist
 */
static inline const simageChainCert_t *simageGetChainCert(const simageHeader_t *header)
{
    if (header->chaincert_offset == 0)
        return NULL;

    uintptr_t ptr = (uintptr_t)header + header->chaincert_offset;
    return (const simageChainCert_t *)ptr;
}

/**
 * \brief helper to get debugcert by simage header
 *
 * \param header simage header
 * \return debugcert, NULL for not exist
 */
static inline const simageDebugCert_t *simageGetDebugCert(const simageHeader_t *header)
{
    if (header->debugcert_offset == 0)
        return NULL;

    uintptr_t ptr = (uintptr_t)header + header->debugcert_offset;
    return (const simageDebugCert_t *)ptr;
}

/**
 * \brief helper to get develcert by simage header
 *
 * \param header simage header
 * \return develcert, NULL for not exist
 */
static inline const simageDevelCert_t *simageGetDevelCert(const simageHeader_t *header)
{
    if (header->develcert_offset == 0)
        return NULL;

    uintptr_t ptr = (uintptr_t)header + header->develcert_offset;
    return (const simageDevelCert_t *)ptr;
}

/**
 * \brief helper to get entry by simage header
 *
 * \param header simage header
 * \return entry
 */
static inline simageEntry_t simageGetEntry(const simageHeader_t *header)
{
    return (simageEntry_t)header->data_entry;
}

OSI_EXTERN_C_END
#endif
