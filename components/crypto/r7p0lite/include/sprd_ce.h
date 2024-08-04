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

#ifndef SPRD_CE_H
#define SPRD_CE_H

#include <sprd_ce_reg.h>
#include <sprd_crypto.h>

#define SPRD_CE_STD 0x00
#define SPRD_CE_LINK 0x01

/* offset 0x44 */
typedef struct
{
    uint32_t link_mode : 1;
    uint32_t dont_rcv_ddr : 1;
    uint32_t dont_dump_ddr : 1;
    uint32_t cmd_ioc : 1;
    uint32_t std_end : 1;
    uint32_t std_aad_end : 1;
    uint32_t std_aad : 1;
    uint32_t dma_bypass : 1;
    uint32_t key_in_ddr : 1;
    uint32_t key_in_efuse : 1;
    uint32_t key_in_session : 1;
    uint32_t key_in_iram : 1;
    uint32_t do_wait_bdone : 1;
    uint32_t list_aad_end : 1;
    uint32_t list_aad : 1;
    uint32_t list_end : 1;
    uint32_t list_data_end : 1;
    uint32_t list_up_iv_sec_cnt : 1;
    uint32_t key_hdcp_en : 1;
    uint32_t : 1;
    uint32_t dst_byte_switch : 1;
    uint32_t src_byte_switch : 1;
    uint32_t dst_word_switch : 1;
    uint32_t src_word_switch : 1;
    uint32_t dst_outstanding_num : 4;
    uint32_t src_outstanding_num : 4;
} sprd_ce_cfg_t;

typedef struct
{
    /* offset 0x48 */
    uint32_t src_frag_len : 24;
    uint32_t src_addr_hi : 4;
    uint32_t : 4;

    /* offset 0x4C */
    uint32_t dst_frag_len : 24;
    uint32_t dst_addr_hi : 4;
    uint32_t : 4;

    /* offset 0x50 */
    uint32_t src_addr_lo;

    /* offset 0x54*/
    uint32_t dst_addr_lo;
} sprd_ce_data_t;

typedef struct
{
    /* offset 0x48 */
    uint32_t pka_instr_len : 24;
    uint32_t pka_instr_addr_hi : 4;
    uint32_t : 4;

    /* offset 0x4C */
    uint32_t : 32;

    /* offset 0x50 */
    uint32_t pka_instr_addr_lo;

    /* offset 0x54 */
    uint32_t pka_store_addr_hi;

    /* offset 0x64 */
    uint32_t pka_load_addr_hi;
} sprd_ce_pka_data_t;

typedef struct
{
    uint32_t len_0 : 10;
    uint32_t : 6;
    uint32_t len_1 : 10;
    uint32_t : 6;
} sprd_ce_pka_reg_len01_t;

typedef struct
{
    /* offset 0x60 */
    uint32_t atr_key_len : 24;
    uint32_t atr_key_addr_hi : 4;
    uint32_t : 4;

    /* offset 0x64 */
    uint32_t atr_key_addr_lo;
} sprd_ce_atr_key_t;

typedef struct
{
    /* offset 0x68 */
    uint32_t aes_tag_len : 8;
    uint32_t aes_tag_addr_hi : 4;
    uint32_t : 20;

    /* offset 0x6c */
    uint32_t aes_tag_addr_lo;
} sprd_ce_aes_tag_t;

/* offset 0x70 ~ 0x7c*/
typedef struct
{
    uint32_t iv_sec_cnt0; //iv[127:96]
    uint32_t iv_sec_cnt1; //iv[95:64]
    uint32_t iv_sec_cnt2; //iv[63:32]
    uint32_t iv_sec_cnt3; //iv[31:0]
} sprd_ce_iv_t;

/* offset 0x30 */
typedef struct
{
    uint32_t en : 1;
    uint32_t : 3;
    uint32_t enc_dec_sel : 1;
    uint32_t : 3;
    uint32_t mode : 3;
    uint32_t : 1;
    uint32_t key_len_sel : 2;
    uint32_t xts_iv_rotate : 1;
    uint32_t : 17;
} sprd_ce_algo_aes_t;

/* offset 0x34 */
typedef struct
{
    uint32_t en : 1;
    uint32_t : 3;
    uint32_t enc_dec_sel : 1;
    uint32_t : 3;
    uint32_t work_mode : 2;
    uint32_t : 2;
    uint32_t key_even_sel : 1;
    uint32_t key_evenod_check_on : 1;
    uint32_t : 18;
} sprd_ce_algo_tdes_t;

/* offset 0x38 */
typedef struct
{
    uint32_t en : 1;
    uint32_t : 3;
    uint32_t mode : 5;
    uint32_t : 3;
    uint32_t hamc_pad_sel : 2;
    uint32_t : 18;
} sprd_ce_algo_hash_t;

/* sm4 work mode cfg offset 0xc0 */
typedef struct
{
    uint32_t en : 1;
    uint32_t : 3;
    uint32_t enc_dec_sel : 1;
    uint32_t : 3;
    uint32_t work_mode : 3;
    uint32_t xts_inv_rotation : 1;
    uint32_t : 20;
} sprd_ce_algo_sm4_t;

/* offset 0xC4 */
typedef struct
{
    uint32_t en : 1;
    uint32_t : 31;
} sprd_ce_algo_rc4_t;

/* offset 0xCC */
#ifdef R3P0
typedef struct
{
    uint32_t en : 1;
    uint32_t reg_num_sel : 1;
    uint32_t : 14;
    uint32_t find_prime_num : 8;
    uint32_t : 8;
} sprd_ce_algo_pka_t;
#else
typedef struct
{
    uint32_t en : 1;
    uint32_t reg_num_sel : 1;
    uint32_t : 14;
    uint32_t find_prime_num : 8;
    uint32_t dst_byte_switch : 1;
    uint32_t src_byte_switch : 1;
    uint32_t dst_word_switch : 1;
    uint32_t src_word_switch : 1;
    uint32_t pka_hi_addr : 4; //pka high 4bits
} sprd_ce_algo_pka_t;
#endif

/* offset 0x228 */
typedef struct
{
    uint32_t rng_mode : 2;
    uint32_t : 6;
    uint32_t prng_mode : 1;
    uint32_t : 23;
} sprd_ce_algo_rng_t;

/* offset 0x340 */
#ifdef R3P0
typedef struct
{
    uint32_t key2_en : 1;
    uint32_t : 3;
    uint32_t key1_start_addr : 8;
    uint32_t key2_start_addr : 8;
    uint32_t key_len : 9;
    uint32_t key_data_sync_en : 1;
    uint32_t key_cpu_access : 1;
    uint32_t key_trng_write : 1;
} sprd_ce_secure_key_t;
#else
typedef struct
{
    uint32_t key2_en : 1;
    uint32_t key1_start_addr : 10;
    uint32_t key2_start_addr : 10;
    uint32_t key_len : 9;
    uint32_t key_cpu_access : 1;
    uint32_t key_trng_write : 1;
} sprd_ce_secure_key_t;
#endif

/*typedef struct {
	uint32_t		:8;
	uint32_t rcv_addr_hi	:4;
	uint32_t dump_addr_hi	:4;
	uint32_t 		:4;
	uint32_t algo_sel	:3;
	uint32_t		:5;
	uint32_t op		:4;
} sprd_ce_dump_addr_hi_t;*/

typedef struct
{
    uint32_t algo;

    union {
        sprd_ce_algo_aes_t aes;
        sprd_ce_algo_sm4_t sm4;
        sprd_ce_algo_tdes_t des;
        sprd_ce_algo_hash_t hash;
        sprd_ce_algo_pka_t pka;
    } algo_cfg;

    /* 0x44 */
    sprd_ce_cfg_t ce_cfg;

    /* offset 0x508 */
    uint32_t rcv_addr_lo;

    /* offset 0x50C */
    uint32_t dump_addr_lo;

    /* offset 0x510 */
    struct
    {
        uint32_t rcv_addr_hi : 4;
        uint32_t dump_addr_hi : 4;
        uint32_t : 24;
    } rcv_dump_addr_hi;
} sprd_ce_start_param_t;

typedef uint32_t (*sprd_ce_irq_handler_f)(void *);

void sprd_ce_reset(void);
void sprdCeInit(void);
void sprdCeTerminal(void);

void sprdCeOpen(void);
void sprdCeClose(void);

uint32_t sprdCeCmdFifoRemainder(void);

uint32_t sprdCeGetCmd_len(sprd_crypto_context_t *context);
void sprdCeWriteCmd(sprd_crypto_context_t *context);
void sprdCeWriteCmdCfg(uint32_t reg, uint32_t reg_num, uint32_t *reg_val);

/* register interrupt function */
void sprdCeRegisterIrqHandle(sprd_ce_irq_handler_f handler);

#ifndef R3P0
uint32_t sprdCePkaCmdFifoRemainder(void);
void sprdCeWritePkaCmdCfg(uint32_t reg, uint32_t reg_num, uint32_t *reg_val);
/* register interrupt function */
void sprdCeRegisterIrqHandlePka(sprd_ce_irq_handler_f handler);
#endif

#endif /* SPRD_CE_H */
