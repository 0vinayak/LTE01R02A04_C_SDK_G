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

#ifndef _SIMAGE_H_
#define _SIMAGE_H_

#include "osi_compiler.h"
#include "calclib/simage_types.h"
#include "calclib/simage_config.h"

OSI_EXTERN_C_BEGIN

/**
 * \brief whether simage header is valid
 *
 * It will perform sanity check of header, and the checksum of header.
 * \p max_size is the maximum permitted size, to avoid a huge wrong size
 * in header to cause trouble.
 *
 * \param header simage header
 * \param max_size maximum permitted size
 * \return
 *      - true if the header is valid
 *      - false if the header is invalid
 */
bool simageHeaderValid(const simageHeader_t *header, unsigned max_size);

/**
 * \brief whether simage payload is valid
 *
 * It will just verify the payload checksum. It must be called after
 * \p simageHeaderValid returns true.
 *
 * \param header simage header
 * \return
 *      - true if the payload checksum is valid
 *      - false if the payload checksum is invalid
 */
bool simagePayloadValid(const simageHeader_t *header);

/**
 * \brief whether simage header and payload is valid
 *
 * It is the same to call \p simageHeaderValid and \p simagePayloadValid.
 *
 * \param header simage header
 * \param max_size maximum permitted size
 * \return
 *      - true if the header and payload are valid
 *      - false if the header or payload is invalid
 */
bool simageValid(const simageHeader_t *header, unsigned max_size);

/**
 * \brief get simage security version
 *
 * \return security version, 0 when there are no keycert.
 */
unsigned simageSecurityVersion(const simageHeader_t *header);

/**
 * \brief calculate RotPK, hash of public key in key cert
 *
 * \param header        simage header
 * \param rotpk         output of RotPK
 * \return
 *      - true on success
 *      - false on fail, no key cert inside
 */
bool simageCalcRotPK(const simageHeader_t *header, osiBits256_t *rotpk);

/**
 * \brief jump to the data entry address
 *
 * \param header        simage header
 * \param param         parameter for the entry
 */
OSI_NO_RETURN void simageJumpEntry(const simageHeader_t *header, unsigned param);

/**
 * \brief check whether hash match by hash method specified in cert
 *
 * \param cert cert header
 * \param expected expected hash value
 * \param data data for hash calculation
 * \param size data size
 * \return
 *      - true on match
 *      - false on mismatch, or invalid parameter
 */
bool simageCertHashMatch(const simageCertHeader_t *cert, const osiBits256_t *expected, const void *data, unsigned size);

/**
 * \brief check whether cert signature is valid
 *
 * It will only verify the message in cert with the signature. The message
 * itself isn't checked.
 *
 * \p pubkey_hash is optional. Only when it is not NULL, the public key in
 * cert will be checked. The hash method will follow cert header.
 *
 * \param cert cert header
 * \param pubkey_hash optional public key hash
 * \return
 *      - true on match
 *      - false on mismatch, or invalid parameter
 */
bool simageCertVerify(const simageCertHeader_t *cert, const osiBits256_t *pubkey_hash);

/**
 * \brief whether certs inside are valid
 *
 * chaincert is optional. When chaincert exists, it will be verified with
 * \p rotpk and \p soc_raw_id.
 *
 * keycert is mandatary. It will be verified with \p rotpk if chaincert not
 * exists, or verified with chaincert if exists.
 *
 * debugcert and develcert are optional. When existed, the signature must be
 * valid.
 *
 * Due to the public key of develcert will be verified in debugcert,
 * debugcert must exist when develcert exists.
 *
 * \param header simage header
 * \param rotpk trusted public key hash
 * \param soc_raw_id raw SoC ID
 * \param min_secure_version minimal valid security version
 * \return
 *      - true if all security checks are passed
 *      - false if there are security checks failed
 */
bool simageSignVerify(const simageHeader_t *header,
                      const osiBits256_t *rotpk,
                      const osiBits64_t *soc_raw_id,
                      unsigned min_secure_version);

OSI_EXTERN_C_END
#endif
