/*
 * SPDX-FileCopyrightText: Copyright (c) 2024 Qorvo US, Inc.
 *
 * SPDX-License-Identifier: LicenseRef-QORVO-1
 */

#ifndef COMPAT_NET_NETLINK_H
#define COMPAT_NET_NETLINK_H

#include <asm/unaligned.h>
#include <linux/skbuff.h>
#include <qerr.h>
#include <stdint.h>
#include <string.h>

#define NLE_SUCCESS QERR_SUCCESS
/* Keep the negative sign as NLE_error are positive on Linux platforms. */
#define NLE_INVAL -QERR_EINVAL
#define NLE_RANGE -QERR_ERANGE
#define NLE_MSGSIZE -QERR_EMSGSIZE

#define NLMSG_DEFAULT_SIZE 1024

struct nlattr {
	uint16_t nla_len;
	uint16_t nla_type;
};

struct netlink_ext_ack {};

struct netlink_range_validation {
	uint64_t min, max;
};

struct netlink_range_validation_signed {
	int64_t min, max;
};

enum nla_policy_validation {
	NLA_VALIDATE_NONE,
	NLA_VALIDATE_RANGE,
	NLA_VALIDATE_MIN,
	NLA_VALIDATE_MAX,
	NLA_VALIDATE_RANGE_PTR,
};

struct nla_policy {
	uint8_t type;
	uint8_t validation_type;
	uint16_t len;
	const struct nla_policy *nested_policy;
	union {
		struct {
			int16_t min, max;
		};
		struct netlink_range_validation *range;
		struct netlink_range_validation_signed *range_signed;
	};
};

enum {
	NLA_UNSPEC,
	NLA_U8,
	NLA_U16,
	NLA_U32,
	NLA_U64,
	NLA_STRING,
	NLA_FLAG,
	NLA_NESTED,
	NLA_NESTED_ARRAY,
	NLA_BINARY,
	NLA_S8,
	NLA_S16,
	NLA_S32,
	NLA_S64,
	__NLA_TYPE_MAX,
};

#define NLA_TYPE_MAX (__NLA_TYPE_MAX - 1)

#ifndef NLA_ALIGNTO
#define NLA_ALIGNTO 4
#endif
#define NLA_ALIGN(len) (((len) + NLA_ALIGNTO - 1) & ~(NLA_ALIGNTO - 1))
#define NLA_HDRLEN ((int)NLA_ALIGN(sizeof(struct nlattr)))

#define NLA_COMPAT_PADDING (0xFFFF)

#define NLA_F_NESTED (1 << 15)
#define NLA_F_NET_BYTEORDER (1 << 14)
#define NLA_TYPE_MASK ~(NLA_F_NESTED | NLA_F_NET_BYTEORDER)

#define NLA_POLICY_EXACT_LEN(_len)                                                        \
	{                                                                                 \
		.type = NLA_BINARY, .validation_type = NLA_VALIDATE_RANGE, .min = (_len), \
		.max = (_len)                                                             \
	}

#define NLA_POLICY_RANGE(tp, _min, _max)                                                           \
	{                                                                                          \
		.type = (tp), .validation_type = NLA_VALIDATE_RANGE, .min = (_min), .max = (_max), \
	}

#define NLA_POLICY_FULL_RANGE(tp, _range)                                                   \
	{                                                                                   \
		.type = (tp), .validation_type = NLA_VALIDATE_RANGE_PTR, .range = (_range), \
	}

#define NLA_POLICY_FULL_RANGE_SIGNED(tp, _range)                                                   \
	{                                                                                          \
		.type = (tp), .validation_type = NLA_VALIDATE_RANGE_PTR, .range_signed = (_range), \
	}

#define NLA_POLICY_MIN(tp, _min)                                                  \
	{                                                                         \
		.type = (tp), .validation_type = NLA_VALIDATE_MIN, .min = (_min), \
	}

#define NLA_POLICY_MAX(tp, _max)                                                  \
	{                                                                         \
		.type = (tp), .validation_type = NLA_VALIDATE_MAX, .max = (_max), \
	}

#define nla_for_each_attr(pos, head, len, rem) \
	for (pos = head, rem = len; nla_ok(pos, rem); pos = nla_next(pos, &(rem)))

#define nla_for_each_nested(pos, nla, rem) nla_for_each_attr (pos, nla_data(nla), nla_len(nla), rem)

#define nlmsg_new(payload, flags) alloc_skb(NLA_ALIGN((payload) + NLA_HDRLEN), (flags))

#define qnlmsg_new(payload, flags, quota) \
	qalloc_skb(NLA_ALIGN((payload) + NLA_HDRLEN), (flags), (quota))

struct nlattr *nla_nest_start(struct sk_buff *skb, int attrtype);
int nla_nest_end(struct sk_buff *skb, struct nlattr *start);
int nla_put(struct sk_buff *skb, int attrtype, int attrlen, const void *data);
int nla_put_64bit(struct sk_buff *skb, int attrtype, int attrlen, const void *data, int padattr);
int __nla_parse_nested(const struct nlattr *head, int len, int maxtype,
		       const struct nla_policy *policy, struct nlattr **tb);
int nla_parse_nested(struct nlattr *tb[], int maxtype, const struct nlattr *nla,
		     const struct nla_policy *policy, struct netlink_ext_ack *extack);

static inline int nla_attr_size(int payload)
{
	return NLA_HDRLEN + payload;
}

static inline int nla_total_size(int payload)
{
	return NLA_ALIGN(nla_attr_size(payload));
}

static inline int nla_padlen(int payload)
{
	return nla_total_size(payload) - nla_attr_size(payload);
}

static inline struct nlattr *nla_next(const struct nlattr *nla, int *remaining)
{
	unsigned int totlen = NLA_ALIGN(nla->nla_len);

	*remaining -= totlen;
	return (struct nlattr *)((char *)nla + totlen);
}

static inline void *nla_data(const struct nlattr *nla)
{
	return (char *)nla + NLA_HDRLEN;
}

static inline int nla_len(const struct nlattr *nla)
{
	return nla->nla_len - NLA_HDRLEN;
}

static inline int nla_ok(const struct nlattr *nla, int remaining)
{
	return remaining >= (int)sizeof(*nla) && nla->nla_len >= sizeof(*nla) &&
	       nla->nla_len <= remaining;
}

static inline int nla_type(const struct nlattr *nla)
{
	return nla->nla_type & NLA_TYPE_MASK;
}

static inline const char *nla_get_string(const struct nlattr *nla)
{
	return (const char *)nla_data(nla);
}

static inline int nla_put_string(struct sk_buff *skb, int attrtype, const char *str)
{
	return nla_put(skb, attrtype, strlen(str) + 1, str);
}

static inline int nla_strcmp(const struct nlattr *nla, const char *str)
{
	char *data = (char *)nla_data(nla);

	return strncmp(str, data, strlen(str));
}

static inline int64_t nla_get_s64(const struct nlattr *nla)
{
#if NLA_ALIGNTO >= 8
	return *(int64_t *)nla_data(nla);
#else
	int64_t tmp;
	memcpy(&tmp, nla_data(nla), sizeof(int64_t));
	return tmp;
#endif
}

static inline int nla_put_s64(struct sk_buff *skb, int attrtype, int64_t value, int padattr)
{
	int64_t tmp = value;

	return nla_put_64bit(skb, attrtype, sizeof(int64_t), &tmp, padattr);
}

static inline int32_t nla_get_s32(const struct nlattr *nla)
{
#if NLA_ALIGNTO >= 4
	return *(int32_t *)nla_data(nla);
#else
	int32_t tmp;
	memcpy(&tmp, nla_data(nla), sizeof(int32_t));
	return tmp;
#endif
}

static inline int nla_put_s32(struct sk_buff *skb, int attrtype, int32_t value)
{
	return nla_put(skb, attrtype, sizeof(int32_t), &value);
}

static inline int16_t nla_get_s16(const struct nlattr *nla)
{
#if NLA_ALIGNTO >= 2
	return *(int16_t *)nla_data(nla);
#else
	int16_t tmp;
	memcpy(&tmp, nla_data(nla), sizeof(int16_t));
	return tmp;
#endif
}

static inline int nla_put_s16(struct sk_buff *skb, int attrtype, int16_t value)
{
	return nla_put(skb, attrtype, sizeof(int16_t), &value);
}

static inline int8_t nla_get_s8(const struct nlattr *nla)
{
	return *(int8_t *)nla_data(nla);
}

static inline int nla_put_s8(struct sk_buff *skb, int attrtype, int8_t value)
{
	return nla_put(skb, attrtype, sizeof(int8_t), &value);
}

static inline uint64_t nla_get_u64(const struct nlattr *nla)
{
#if NLA_ALIGNTO >= 8
	return *(uint64_t *)nla_data(nla);
#else
	int64_t tmp;
	memcpy(&tmp, nla_data(nla), sizeof(int64_t));
	return tmp;
#endif
}

static inline int nla_put_u64_64bit(struct sk_buff *skb, int attrtype, uint64_t value, int padattr)
{
	uint64_t tmp = value;

	return nla_put_64bit(skb, attrtype, sizeof(uint64_t), &tmp, padattr);
}

static inline uint32_t nla_get_u32(const struct nlattr *nla)
{
#if NLA_ALIGNTO >= 4
	return *(uint32_t *)nla_data(nla);
#else
	uint32_t tmp;
	memcpy(&tmp, nla_data(nla), sizeof(uint32_t));
	return tmp;
#endif
}

static inline int nla_put_u32(struct sk_buff *skb, int attrtype, uint32_t value)
{
	return nla_put(skb, attrtype, sizeof(uint32_t), &value);
}

static inline uint16_t nla_get_u16(const struct nlattr *nla)
{
#if NLA_ALIGNTO >= 2
	return *(uint16_t *)nla_data(nla);
#else
	uint16_t tmp;
	memcpy(&tmp, nla_data(nla), sizeof(uint16_t));
	return tmp;
#endif
}

static inline uint16_t nla_get_le16(const struct nlattr *nla)
{
	return (uint16_t)get_unaligned_le16(nla_data(nla));
}

static inline int nla_put_u16(struct sk_buff *skb, int attrtype, uint16_t value)
{
	return nla_put(skb, attrtype, sizeof(uint16_t), &value);
}

static inline int nla_put_le16(struct sk_buff *skb, int attrtype, uint16_t value)
{
	return nla_put(skb, attrtype, sizeof(uint16_t), &value);
}

static inline uint8_t nla_get_u8(const struct nlattr *nla)
{
	return *(uint8_t *)nla_data(nla);
}

static inline int nla_put_u8(struct sk_buff *skb, int attrtype, uint8_t value)
{
	return nla_put(skb, attrtype, sizeof(uint8_t), &value);
}

static inline int nla_get_flag(const struct nlattr *nla)
{
	return !!nla;
}

static inline int nla_put_flag(struct sk_buff *skb, int attrtype)
{
	return nla_put(skb, attrtype, 0, NULL);
}

#endif /* COMPAT_NET_NETLINK_H */
