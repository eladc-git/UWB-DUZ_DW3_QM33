/*
 * SPDX-FileCopyrightText: Copyright (c) 2024 Qorvo US, Inc.
 *
 * SPDX-License-Identifier: LicenseRef-QORVO-1
 */

#pragma once

#include <linux/skbuff.h>
#include <stdint.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

#define NLMSG_DEFAULT_SIZE 1024

struct nlattr {
	uint16_t nla_len;
	uint16_t nla_type;
};

struct netlink_ext_ack {
	uint8_t dummy;
};

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
	uint16_t len;
	uint8_t validation_type;
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
	NLA_MSECS,
	NLA_NESTED,
	NLA_NESTED_ARRAY,
	NLA_NUL_STRING,
	NLA_BINARY,
	NLA_S8,
	NLA_S16,
	NLA_S32,
	NLA_S64,
	NLA_BITFIELD32,
	NLA_REJECT,
	__NLA_TYPE_MAX,
};

#define NLA_TYPE_MAX (__NLA_TYPE_MAX - 1)

#define NLA_ALIGNTO 4
#define NLA_ALIGN(len) (((len) + NLA_ALIGNTO - 1) & ~(NLA_ALIGNTO - 1))
#define NLA_HDRLEN ((int)NLA_ALIGN(sizeof(struct nlattr)))

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
	for (pos = (struct nlattr *)head, rem = len; nla_ok(pos, rem); pos = nla_next(pos, &(rem)))

#define nla_for_each_nested(pos, nla, rem) nla_for_each_attr (pos, nla_data(nla), nla_len(nla), rem)

struct nlattr *nla_next(const struct nlattr *nla, int *remaining);
void *nla_data(const struct nlattr *nla);
int nla_len(const struct nlattr *nla);
int nla_ok(const struct nlattr *nla, int remaining);
int nla_type(const struct nlattr *nla);
int64_t nla_get_s64(const struct nlattr *nla);
int nla_put_s64(struct sk_buff *skb, int attrtype, int64_t value, int padattr);
int32_t nla_get_s32(const struct nlattr *nla);
int nla_put_s32(struct sk_buff *skb, int attrtype, int32_t value);
int16_t nla_get_s16(const struct nlattr *nla);
int nla_put_s16(struct sk_buff *skb, int attrtype, int16_t value);
int8_t nla_get_s8(const struct nlattr *nla);
int nla_put_s8(struct sk_buff *skb, int attrtype, int8_t value);
uint64_t nla_get_u64(const struct nlattr *nla);
int nla_put_u64_64bit(struct sk_buff *skb, int attrtype, uint64_t value, int padattr);
uint32_t nla_get_u32(const struct nlattr *nla);
int nla_put_u32(struct sk_buff *skb, int attrtype, uint32_t value);
uint16_t nla_get_u16(const struct nlattr *nla);
uint16_t nla_get_le16(const struct nlattr *nla);
int nla_put_u16(struct sk_buff *skb, int attrtype, uint16_t value);
int nla_put_le16(struct sk_buff *skb, int attrtype, uint16_t value);
uint8_t nla_get_u8(const struct nlattr *nla);
int nla_put_u8(struct sk_buff *skb, int attrtype, uint8_t value);
int nla_get_flag(const struct nlattr *nla);
int nla_put_flag(struct sk_buff *skb, int attrtype);
struct nlattr *nla_nest_start(struct sk_buff *skb, int attrtype);
int nla_nest_end(struct sk_buff *skb, struct nlattr *start);
int nla_put(struct sk_buff *skb, int attrtype, int attrlen, const void *data);
int __nla_parse_nested(const struct nlattr *head, int len, int maxtype,
		       const struct nla_policy *policy, struct nlattr **tb);
int nla_parse_nested(struct nlattr *tb[], int maxtype, const struct nlattr *nla,
		     const struct nla_policy *policy, struct netlink_ext_ack *extack);

#ifdef __cplusplus
}
#endif
