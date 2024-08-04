/*
* Copyright (c) 2016, Spreadtrum Communications.
*
* The above copyright notice shall be
* included in all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#ifndef SPRD_PROXY_CRYPTO_H_
#define SPRD_PROXY_CRYPTO_H_

#include "../../../driver/src/trusty/core/util.h"
#include "../../../driver/src/trusty/core/ql_ipc.h"
#include "../../../driver/src/trusty/core/ql_ipc_dev.h"
#include <sprd_proxy_crypto_ipc.h>

/*
 * Initialize Crypto TIPC client. Returns one of trusty_err.
 *
 * @dev: initialized with trusty_ipc_dev_create
 */
int crypto_tipc_connect(struct trusty_ipc_dev *crypto_ipc_dev, const char *port);

/*
 * Send the CA Request to Crypto. Returns one of trusty_err.
 *
 * @cmd: CA Request cmd
 * @req: ca Request_buff
 * @req_len: size of ca Request_buff
 */
int crypto_send_request(uint32_t cmd, const void *req, size_t req_len);

/*
 * Reads a TA Response from Crypto. Returns one of trusty_err.
 *
 * @cmd: CA Request_cmd used to check cmd
 * @resp: CA Response_buff
 * @resp_len: size of CA Response_buff
 */
int crypto_read_response(uint32_t cmd, void *resp, size_t resp_len);

/*
 * Shutdown Crypto TIPC client.
 *
 * @dev: initialized with trusty_ipc_dev_create
 */
void crypto_tipc_disconnect(struct trusty_ipc_dev *crypto_ipc_dev);

/* Checks that the command opcode in |header| matches |ex-ected_cmd|. Checks
 * that |tipc_result| is a valid response size. Returns negative on error.
 */
int crypto_check_response_error(uint32_t expected_cmd,
                                proxy_crypto_message_t header,
                                int32_t tipc_result);

#endif /* SPRD_PROXY_CRYPTO_H_ */
