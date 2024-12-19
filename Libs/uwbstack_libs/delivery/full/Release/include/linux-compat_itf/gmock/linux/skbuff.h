/*
 * SPDX-FileCopyrightText: Copyright (c) 2024 Qorvo US, Inc.
 *
 * SPDX-License-Identifier: LicenseRef-QORVO-1
 */

#pragma once

#include <linux/slab.h>
#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

struct sk_buff_head {
	struct sk_buff *next;
	struct sk_buff *prev;
};

struct sk_buff {
	struct sk_buff *next;
	struct sk_buff *prev;
	char cb[48] __attribute__((__aligned__(8)));
	unsigned int len;
	uint16_t queue_mapping;
	uint32_t reserved_tailroom;
	uint8_t *head;
	uint8_t *data;
	uint8_t *tail;
	uint8_t *end;
};

#define qalloc_skb(__s, __f, __q) alloc_skb(__s, __f)
struct sk_buff *alloc_skb(unsigned int size, int flags);
void kfree_skb(struct sk_buff *skb);
void skb_reserve(struct sk_buff *skb, unsigned int len);
void skb_trim(struct sk_buff *skb, unsigned int len);
void *skb_put(struct sk_buff *skb, unsigned int len);
void *skb_put_data(struct sk_buff *skb, const void *data, unsigned int len);
void skb_put_u8(struct sk_buff *skb, uint8_t v);
void *skb_push(struct sk_buff *skb, unsigned int len);
void *skb_pull(struct sk_buff *skb, unsigned int len);
void skb_tailroom_reserve(struct sk_buff *skb, unsigned int mtu, unsigned int needed_tailroom);
int skb_availroom(const struct sk_buff *skb);
int skb_queue_empty(const struct sk_buff_head *list);
struct sk_buff *skb_peek(struct sk_buff_head *list);
void skb_queue_head_init(struct sk_buff_head *list);
void skb_queue_head(struct sk_buff_head *list, struct sk_buff *newsk);
void skb_queue_tail(struct sk_buff_head *list, struct sk_buff *newsk);
struct sk_buff *skb_dequeue(struct sk_buff_head *list);
void skb_queue_purge(struct sk_buff_head *list);
unsigned int skb_headroom(const struct sk_buff *skb);
int skb_tailroom(const struct sk_buff *skb);
unsigned char *skb_tail_pointer(const struct sk_buff *skb);
struct sk_buff *skb_queue_next(const struct sk_buff_head *list, const struct sk_buff *skb);
bool skb_queue_is_last(const struct sk_buff_head *list, const struct sk_buff *skb);
int skb_zerocopy(struct sk_buff *to, struct sk_buff *from, int len, int hlen);
#define skb_headlen(skb) (skb)->len
#define skb_walk_frags(skb, iter) for ((iter) = (skb); (iter); (iter) = NULL)

#ifdef __cplusplus
}
#endif
