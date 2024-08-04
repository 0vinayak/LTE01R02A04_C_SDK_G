
/*================================================================
  Copyright (c) 2021, Quectel Wireless Solutions Co., Ltd. All rights reserved.
  Quectel Wireless Solutions Proprietary and Confidential.
=================================================================*/
    
#ifndef QL_API_AES_H
#define QL_API_AES_H

/*****************************************************************
* Function: ql_aes_string_encryption
*
* Description:
* AES encryption algorithm
* This function is to hash and encrypt the user input data to get an encrypted result.
* 
* Parameters:
* key            [in]  secret key
* key_length     [in]  secret key length
* inputdata      [in]  use inut data
* outputdata     [in]  hash result

* Return:
* void
*
*****************************************************************/
void ql_aes_string_encryption(unsigned char key[16], int key_length, unsigned char inputdata[16], unsigned char outputdata[16]);


/*****************************************************************
* Function: ql_aes_string_decryption
*
* Description:
* AES decryption algorithm
* This function decrypts the hash encrypted data to get the original data.
* 
* Parameters:
* key            [in]  secret key
* key_length     [in]  secret key length
* inputdata      [in]  use inut data
* outputdata     [in]  hash result

* Return:
* void
*
*****************************************************************/
void ql_aes_string_decryption(unsigned char key[16], int key_length, unsigned char inputdata[16], unsigned char outputdata[16]);
#endif 
