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

#ifndef SPRD_QUEUE_H
#define SPRD_QUEUE_H

#include <sprd_crypto.h>

typedef struct sprd_queue_item
{
    struct sprd_queue_item *prev;
    struct sprd_queue_item *next;
    void *data;
} sprd_queue_item_t;

typedef struct
{
    sprd_queue_item_t *head;
    sprd_queue_item_t *tail;
    sprd_queue_item_t *current;
    void *mutex;
} sprd_queue_t;

uint32_t sprdQueueCreate(sprd_queue_t *queue);
uint32_t sprdQueueAddItemTop(sprd_queue_t *queue, sprd_queue_item_t *item);
sprd_queue_item_t *sprdQueueGetItemCurrent(sprd_queue_t *queue);
uint32_t sprdQueueUpdateCurrent(sprd_queue_t *queue);
sprd_queue_item_t *sprdQueueDeleteItemBottom(sprd_queue_t *queue);
uint32_t sprdQueueDestroy(sprd_queue_t *queue);

#endif /* SPRD_QUEUE_H */
