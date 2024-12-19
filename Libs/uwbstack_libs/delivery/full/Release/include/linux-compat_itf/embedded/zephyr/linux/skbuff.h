/*
 * SPDX-FileCopyrightText: Copyright (c) 2024 Qorvo US, Inc.
 *
 * SPDX-License-Identifier: LicenseRef-QORVO-1
 */

#ifndef COMPAT_LINUX_SKBUFF_H
#define COMPAT_LINUX_SKBUFF_H

#include "qassert.h"
#include "uwbmac/uwbmac_buf.h"

#include <linux/slab.h>
#include <qerr.h>
#include <stdbool.h>
#include <stdint.h>

struct sk_buff_head {
	struct uwbmac_buf_queue queue;
};

struct sk_buff {
	union {
		struct uwbmac_buf buf;
		struct {
			struct sk_buff *frag;
			uint8_t __ref;
			uint8_t __flags;
			uint8_t __pool_id;
			uint8_t __user_data_size;
			uint8_t *data;
			uint16_t __len;
			uint16_t __size;
			uint8_t *__buf;
			/* All the fields above this line must match the layout
			 * of Zephyr's 'struct net_buf'.
			 */
			sys_snode_t node;
			uint16_t queue_mapping;
			uint16_t len;
			uint8_t *head;
			uint8_t cb[];
		};
	};
};
#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L
_Static_assert(offsetof(struct sk_buff, cb) % 8 == 0, "cb[] field must be 8 bytes aligned!");
#endif

static inline struct sk_buff *qalloc_skb(unsigned int size, int flags, enum mem_quota_id quota_id)
{
	struct sk_buff *skb = (struct sk_buff *)uwbmac_buf_alloc_quota(size, quota_id);

	if (skb)
		skb->head = skb->data;

	return skb;
}

static inline struct sk_buff *alloc_skb(unsigned int size, int flags)
{
	struct sk_buff *skb = (struct sk_buff *)uwbmac_buf_alloc(size);

	if (skb)
		skb->head = skb->data;

	return skb;
}

static inline void kfree_skb(struct sk_buff *skb)
{
	if (skb)
		uwbmac_buf_free(&skb->buf);
}

static inline void skb_reserve(struct sk_buff *skb, unsigned int len)
{
	uwbmac_buf_reserve(&skb->buf, len);
}

static inline unsigned int skb_headroom(const struct sk_buff *skb)
{
	return uwbmac_buf_headroom(&skb->buf);
}

static inline int skb_tailroom(const struct sk_buff *skb)
{
	return uwbmac_buf_tailroom(&skb->buf);
}

static inline unsigned char *skb_tail_pointer(struct sk_buff *skb)
{
	return net_buf_tail(&skb->buf.netbuf);
}

static inline struct sk_buff *get_next_skb(struct sk_buff *skb, struct sk_buff **skb_end)
{
	/* get_next_skb will browse sk_buff until it reaches the end of the skb
	 * sent as parameter It will returns the next skb/packet if it exists
	 * (i.e. if skb_zerocopy was used) the pointer will be NULL otherwise.
	 * It will also returns the previous fragment of the packet in `skb_end`
	 * if asked, so that we can split the packet to restore state before
	 * skb_zerocopy by setting skb_end's frag to NULL. */
	int rem_len = skb->len;
	int acc_len = 0;
	struct sk_buff *next_frag = skb;

	/* Loop through fragments */
	while ((acc_len + next_frag->__len) < rem_len) {
		acc_len += next_frag->__len;
		next_frag = next_frag->frag;
	}

	if (skb_end) {
		*skb_end = next_frag;
	}

	return next_frag->frag;
}

static inline void skb_trim(struct sk_buff *skb, unsigned int len)
{
	QASSERT(skb->len >= len);

	/* Some concepts:
	 *	At uwbmac layer, data might be put in several fragments within a
	 *`struct uwbmac_buf`. There is an initial struct skbuff (frame_skb)
	 *allocated in the MAC. When calling skb_zerocopy, incoming `uwbmac_buf`
	 *(casted into struct sk_buff) are linked as fragments to the initial
	 *skbuff, in order to form the frame to be transmitted. Only the first
	 *fragment of a uwbmac_buf is being referenced just after it's added as
	 *a fragment.
	 *
	 *	Before trimming, we need to check at which fragment we should
	 *trim, and free (unreference) buffers that come after the selected one.
	 *
	 *	These skbuff fragments must be reverted to they original state,
	 *before linking them in skb_zerocopy.
	 */

	unsigned int rem_len = len;
	struct sk_buff *next_skb = NULL;
	struct sk_buff *cur_skb = skb;

	/* First fragment `len` field will have the whole chain length. We use
	 * `__len` instead. */
	unsigned int buf_len = cur_skb->__len;

	if (len > buf_len) {
		rem_len -= buf_len;
		cur_skb = cur_skb->frag;

		/* From second buffer on, `len` field only has the buffer and
		 * its chained fragments length. */
		buf_len = cur_skb->len;

		/* Find the right buffer to trim from. */
		while (rem_len >= buf_len) {
			rem_len -= buf_len;

			cur_skb = get_next_skb(cur_skb, NULL);

			/* In `cur_frag` we have the first buffer from fragments
			 * chained list. */
			buf_len = cur_skb->len;
		}
	}

	skb->len = len;

	/* Look for buffers after the one we need to trim, to free them. */
	struct sk_buff *skb_end = NULL;
	next_skb = get_next_skb(cur_skb, &skb_end);

	struct sk_buff *ref = NULL;
	while (next_skb) {
		struct sk_buff *temp_end = NULL;
		ref = next_skb;
		next_skb = get_next_skb(next_skb, &temp_end);
		/* Set frag to NULL even if it is the last skb */
		temp_end->frag = NULL;
		net_buf_unref(&ref->buf.netbuf);
	}

	/* Trim uwbmac_buf fragment. */
	uwbmac_buf_trim(&cur_skb->buf, rem_len);

	/* Disconnect selected trimmed skbuffer from other fragments already
	 * freed. */
	if (!skb_end) {
		/* If skb_end == NULL, we are on on first skb */
		skb_end = cur_skb;
	}
	skb_end->frag = NULL;
}

static inline void *skb_put(struct sk_buff *skb, unsigned int len)
{
	return uwbmac_buf_put(&skb->buf, len);
}

static inline void *skb_put_data(struct sk_buff *skb, const void *data, unsigned int len)
{
	/* TODO: This function should do a simple memcpy to match linux'
	 * implementation. */
	uint8_t *tail = uwbmac_buf_get_data(&skb->buf) + uwbmac_buf_get_len(&skb->buf);
	uwbmac_buf_put_data(&skb->buf, data, len);
	return tail;
}

static inline void skb_put_u8(struct sk_buff *skb, uint8_t v)
{
	uwbmac_buf_put_u8(&skb->buf, v);
}

static inline void *skb_push(struct sk_buff *skb, unsigned int len)
{
	return uwbmac_buf_push(&skb->buf, len);
}

static inline void *skb_pull(struct sk_buff *skb, unsigned int len)
{
	struct net_buf *frag = &skb->buf.netbuf;
	void *data = NULL;

	uwbmac_buf_pull(&skb->buf, len);

	/* Return pointer to next data to read after call to skb_pull. */
	while (frag) {
		data = frag->data;
		if (frag->len)
			break;

		frag = frag->frags;
	}
	return data;
}

static inline void skb_tailroom_reserve(struct sk_buff *skb, unsigned int mtu,
					unsigned int needed_tailroom)
{
	uwbmac_buf_reserve_tailroom(&skb->buf, needed_tailroom);
}

static inline int skb_availroom(const struct sk_buff *skb)
{
	return skb_tailroom(skb);
}

static inline void skb_queue_head_init(struct sk_buff_head *list)
{
	uwbmac_buf_queue_init(&list->queue);
}

static inline int skb_queue_empty(const struct sk_buff_head *list)
{
	return uwbmac_buf_queue_empty(&list->queue);
}

static inline struct sk_buff *skb_peek(struct sk_buff_head *list)
{
	return (struct sk_buff *)uwbmac_buf_queue_peek(&list->queue);
}

static inline void skb_queue_head(struct sk_buff_head *list, struct sk_buff *newsk)
{
	uwbmac_buf_queue_push(&list->queue, &newsk->buf);
}

static inline void skb_queue_tail(struct sk_buff_head *list, struct sk_buff *newsk)
{
	uwbmac_buf_queue_put(&list->queue, &newsk->buf);
}

static inline struct sk_buff *skb_dequeue(struct sk_buff_head *list)
{
	return (struct sk_buff *)uwbmac_buf_queue_pop(&list->queue);
}

static inline void skb_queue_purge(struct sk_buff_head *list)
{
	uwbmac_buf_queue_purge(&list->queue);
}

static inline struct sk_buff *skb_queue_next(const struct sk_buff_head *list,
					     const struct sk_buff *skb)
{
	QASSERT(!uwbmac_buf_queue_is_last(&list->queue, &skb->buf));
	return (struct sk_buff *)uwbmac_buf_queue_next(&list->queue, &skb->buf);
}

static inline bool skb_queue_is_last(const struct sk_buff_head *list, const struct sk_buff *skb)
{
	return uwbmac_buf_queue_is_last(&list->queue, &skb->buf);
}

static inline int skb_zerocopy(struct sk_buff *to, struct sk_buff *from, int len, int hlen)
{
	if (to->buf.netbuf.len) {
		struct net_buf *ignore = NULL;
		ignore = net_buf_ref(&from->buf.netbuf);

		net_buf_frag_add(&to->buf.netbuf, &from->buf.netbuf);
		to->len += len;
	} else {
		/*
		 * Copy data instead of adding frags to the list because :
		 *  - fragments in 'from' buffer may be changed later (e.g. with
		 * skb_pull).
		 *  - fragments in 'from' buffer may not be copied entirely.
		 * TODO: This could be improved later. See UWB-6592.
		 */
		struct net_buf *frag = &from->buf.netbuf;
		int rem = len;

		while (frag && rem > 0) {
			if (frag->len) {
				int count = MIN(rem, frag->len);

				if (uwbmac_buf_put_data(&to->buf, frag->data, count))
					return QERR_ENOMEM;
				rem -= count;
			}

			frag = frag->frags;
		}
	}

	return QERR_SUCCESS;
}

static inline bool skb_is_nonlinear(const struct sk_buff *skb)
{
	return skb->__len < skb->len;
}

static inline unsigned int skb_headlen(const struct sk_buff *skb)
{
	return skb->__len;
}

#define skb_walk_frags(skb, iter) \
	for (iter = skb; iter; iter = (struct sk_buff *)iter->buf.netbuf.frags)

#endif /* COMPAT_LINUX_SKBUFF_H */
