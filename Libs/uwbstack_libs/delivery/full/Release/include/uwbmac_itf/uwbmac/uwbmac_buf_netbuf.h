/*
 * SPDX-FileCopyrightText: Copyright (c) 2024 Qorvo US, Inc.
 *
 * SPDX-License-Identifier: LicenseRef-QORVO-1
 */

#pragma once

#include <net/buf.h>
#include <uwbmac/uwbmac_types.h>

#ifdef __cplusplus
extern "C" {
#endif

#define __UWBMAC_MALLOC_NETBUF 0x55

#define UWBMAC_BUF_MAX_SIZE 128
#define USER_DATA(x) ((struct uwbmac_buf_userdata *)net_buf_user_data(x))

struct uwbmac_buf_queue {
	/**
	 * @slist: single-linked-list
	 */
	sys_slist_t slist;
};

/**
 * struct uwbmac_buf - Network buffer, used to exchange frames.
 */
struct uwbmac_buf {
	/**
	 * @netbuf: network buffer
	 */
	struct net_buf netbuf;
};

/**
 * struct uwbmac_buf_userdata - User data fields available in uwbmac_buf
 */
struct uwbmac_buf_userdata {
	/**
	 * @node: Allow placing the buffer into sys_slist_t while using net_buf
	 * frags.
	 */
	sys_snode_t node;
	/**
	 * @queue_mapping: Queue id, used by regions supporting multiple queues.
	 */
	uint16_t queue_mapping;
	/**
	 * @len: Total length of buffer, including fragments attached to this
	 * buffer.
	 */
	uint16_t len;
	/**
	 * @head: The head of the buffer.
	 */
	uint8_t *head;
	/**
	 * @cb: Control block, used to store related data, depending on the
	 * layer owning the buffer.
	 *
	 * WARNING: Field cb must be aligned on 8-bytes, which is only
	 * guaranteed on a 32-bit platform here.
	 */
	uint8_t cb[UWBMAC_BUF_CB_SIZE];
};
#ifdef __LP64__
#define ZEPHYR_NET_BUF_STRUCT_SIZE 40
#else
#define ZEPHYR_NET_BUF_STRUCT_SIZE 20
#endif
#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L
_Static_assert((ZEPHYR_NET_BUF_STRUCT_SIZE + offsetof(struct uwbmac_buf_userdata, cb)) % 8 == 0,
	       "cb[] field must be 8 bytes aligned!");
#endif

struct uwbmac_buf *uwbmac_buf_alloc_quota(unsigned int size, enum mem_quota_id quota_id);
struct uwbmac_buf *uwbmac_buf_alloc(unsigned int size);
void uwbmac_buf_free(struct uwbmac_buf *buf);

int uwbmac_buf_put_data(struct uwbmac_buf *buf, const void *data, unsigned int len);

static inline void uwbmac_buf_reserve(struct uwbmac_buf *buf, unsigned int len)
{
	net_buf_reserve(&buf->netbuf, len);
}

static inline unsigned int uwbmac_buf_headroom(const struct uwbmac_buf *buf)
{
	return net_buf_headroom(&((struct uwbmac_buf *)buf)->netbuf);
}

static inline unsigned int uwbmac_buf_tailroom(const struct uwbmac_buf *buf)
{
	return net_buf_tailroom(&((struct uwbmac_buf *)buf)->netbuf);
}

static inline void uwbmac_buf_reserve_tailroom(struct uwbmac_buf *buf, unsigned int len)
{
	buf->netbuf.size -= len;
}

static inline void uwbmac_buf_trim(struct uwbmac_buf *buf, unsigned int len)
{
	if (!buf || USER_DATA(&buf->netbuf)->len < len)
		return;

	struct net_buf *frag = &buf->netbuf;
	int rem_len = len;

	/* Find the right fragment to trim from. */
	while (rem_len > frag->len) {
		rem_len -= frag->len;
		frag = frag->frags;
	}

	/* Update netbuf len. */
	frag->len = rem_len;
	/* Update user data. */
	USER_DATA(&buf->netbuf)->len = len;
}

void *uwbmac_buf_put(struct uwbmac_buf *buf, unsigned int len);

static inline void uwbmac_buf_put_u8(struct uwbmac_buf *buf, uint8_t data)
{
	USER_DATA(&buf->netbuf)->len += sizeof(data);
	net_buf_add_u8(&buf->netbuf, data);
}

static inline void *uwbmac_buf_push(struct uwbmac_buf *buf, unsigned int len)
{
	USER_DATA(&buf->netbuf)->len += len;
	return net_buf_push(&buf->netbuf, len);
}

void uwbmac_buf_pull(struct uwbmac_buf *buf, unsigned int len);

static inline void uwbmac_buf_queue_init(struct uwbmac_buf_queue *queue)
{
	sys_slist_init(&queue->slist);
}

static inline bool uwbmac_buf_queue_empty(const struct uwbmac_buf_queue *queue)
{
	return sys_slist_is_empty((sys_slist_t *)&queue->slist);
}

static inline void uwbmac_buf_queue_push(struct uwbmac_buf_queue *queue, struct uwbmac_buf *buf)
{
	sys_slist_prepend(&queue->slist, &USER_DATA(&buf->netbuf)->node);
}

static inline void uwbmac_buf_queue_put(struct uwbmac_buf_queue *queue, struct uwbmac_buf *buf)
{
	sys_slist_append(&queue->slist, &USER_DATA(&buf->netbuf)->node);
}

static inline bool uwbmac_buf_queue_is_last(const struct uwbmac_buf_queue *queue,
					    const struct uwbmac_buf *buf)
{
	return &USER_DATA(&buf->netbuf)->node == queue->slist.tail;
}

static inline struct uwbmac_buf *uwbmac_buf_queue_next(const struct uwbmac_buf_queue *queue,
						       const struct uwbmac_buf *buf)
{
	sys_snode_t *node = USER_DATA(&buf->netbuf)->node.next;
	if (node) {
		struct uwbmac_buf_userdata *ud =
			CONTAINER_OF(node, struct uwbmac_buf_userdata, node);
		return (struct uwbmac_buf *)CONTAINER_OF(ud, struct net_buf, user_data);
	}
	return NULL;
}

static inline struct uwbmac_buf *uwbmac_buf_queue_peek(struct uwbmac_buf_queue *queue)
{
	sys_snode_t *node = sys_slist_peek_head(&queue->slist);
	if (node) {
		struct uwbmac_buf_userdata *ud =
			CONTAINER_OF(node, struct uwbmac_buf_userdata, node);
		return (struct uwbmac_buf *)CONTAINER_OF(ud, struct net_buf, user_data);
	}
	return NULL;
}

static inline struct uwbmac_buf *uwbmac_buf_queue_pop(struct uwbmac_buf_queue *queue)
{
	sys_snode_t *node = sys_slist_get(&queue->slist);
	if (node) {
		struct uwbmac_buf_userdata *ud = (struct uwbmac_buf_userdata *)CONTAINER_OF(
			node, struct uwbmac_buf_userdata, node);
		return (struct uwbmac_buf *)CONTAINER_OF(ud, struct net_buf, user_data);
	}
	return NULL;
}

static inline void uwbmac_buf_queue_purge(struct uwbmac_buf_queue *queue)
{
	struct uwbmac_buf *buf;
	while ((buf = uwbmac_buf_queue_pop(queue)))
		uwbmac_buf_free(buf);
}

static inline uint8_t *uwbmac_buf_get_data(struct uwbmac_buf *buf)
{
	return buf->netbuf.data;
}

static inline struct uwbmac_buf *uwbmac_buf_get_next_frag(struct uwbmac_buf *buf)
{
	return (struct uwbmac_buf *)buf->netbuf.frags;
}

static inline unsigned int uwbmac_buf_get_len(struct uwbmac_buf *buf)
{
	return USER_DATA(&buf->netbuf)->len;
}

static inline unsigned int uwbmac_buf_get_frag_len(struct uwbmac_buf *buf)
{
	return buf->netbuf.len;
}

static inline unsigned int uwbmac_buf_get_size(struct uwbmac_buf *buf)
{
	/* We need to use uwbmac_buf_get_len to stop browsing in the frags
	 * since skb_zerocopy will chain frags from different uwbmac_buf.
	 * Also we must check uwbmac_buf_headroom if uwbmac_buf_reserve was
	 * used.*/
	struct net_buf *cur = &buf->netbuf;
	unsigned int skb_len = uwbmac_buf_get_len(buf) + uwbmac_buf_headroom(buf);
	unsigned int acc_len = buf->netbuf.size;

	while (cur && acc_len <= skb_len) {
		cur = cur->frags;
		acc_len += buf->netbuf.size;
	}
	return acc_len;
}

static inline void uwbmac_buf_set_queue_mapping(struct uwbmac_buf *buf, uint16_t value)
{
	USER_DATA(&buf->netbuf)->queue_mapping = value;
}

#ifdef __cplusplus
}
#endif
