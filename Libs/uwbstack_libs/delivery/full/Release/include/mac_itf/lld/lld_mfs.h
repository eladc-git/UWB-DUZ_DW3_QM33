/*
 * SPDX-FileCopyrightText: Copyright (c) 2024 Qorvo US, Inc.
 *
 * SPDX-License-Identifier: LicenseRef-QORVO-1
 */

#pragma once

#include "linux/skbuff.h"
#include "lld/lld_common.h"
#include "qatomic.h"
#include "qerr.h"

#include <stdbool.h>
#include <stdint.h>

struct mcps802154_llhw;
struct mcps802154_region;

#define LLD_MFS_WINDOW_MSDU_MIN 1
#define LLD_MFS_WINDOW_MSDU_MAX 64
#define LLD_MFS_WINDOW_MSDU_DEFAULT 8
#define LLD_MFS_WINDOW_BYTES_MIN 127
#define LLD_MFS_TX_QUEUE_LEN_DEFAULT 32

/**
 * enum lld_mfs_id - MFS ids.
 * @LLD_MFS_ID_INTERACTIVE:
 * 	Id for interactive MFS.
 * @LLD_MFS_ID_BEST_EFFORT:
 * 	Id for best effort MFS.
 * @LLD_MFS_ID_NB:
 * 	Number of MFS id.
 */
enum lld_mfs_id {
	LLD_MFS_ID_INTERACTIVE,
	LLD_MFS_ID_BEST_EFFORT,
	LLD_MFS_ID_NB,
};

/**
 * struct lld_mfs_stats - LLD MFS statistics.
 */
struct lld_mfs_stats {
	/**
	 * @rx_packets: Number of good data packets received.
	 */
	uint32_t rx_packets;
	/**
	 * @rx_bytes: Number of good received bytes, corresponding to
	 * rx_packets.
	 */
	uint32_t rx_bytes;
	/**
	 * @rx_errors: Number of bad data packets received.
	 */
	uint32_t rx_errors;
	/**
	 * @rx_ignored: Number of good data packets ignored because
	 * they are out of the reception window.
	 */
	uint32_t rx_ignored;
	/**
	 * @tx_packets: Number of packets successfully transmitted.
	 */
	uint32_t tx_packets;
	/**
	 * @tx_bytes: Number of transmitted bytes, corresponding to tx_packet.
	 */
	uint32_t tx_bytes;
	/**
	 * @tx_errors: Number of data packets not transmitted due to problem
	 * with the packet.
	 */
	uint32_t tx_errors;
	/**
	 * @tx_dropped: Number of data packets dropped before successful
	 * transmission.
	 */
	uint32_t tx_dropped;
};

/**
 * struct lld_mfs_params - MFS parameters.
 */
struct lld_mfs_params {
	/**
	 * @mfs_expiration_ms: MSDU expiration delay in milliseconds. Expiration
	 * is disabled if 0.
	 */
	int mfs_expiration_ms;
	/**
	 * @mfs_window_msdu: Size of the MFS window in number of MSDUs.
	 */
	int mfs_window_msdu;
	/**
	 * @mfs_window_bytes: Size of the MFS window in bytes.
	 */
	int mfs_window_bytes;
};

/**
 * struct lld_mfs_rx - Rx MAC Frame Stream.
 */
struct lld_mfs_rx {
	/**
	 * @sync: True if the Rx MFS is synchronised.
	 */
	bool sync;
	/**
	 * @next_expected_seq: Next expected sequence number.
	 */
	uint8_t next_expected_seq;
};

#define lld_mfs_rx_init(mfs_rx)                               \
	do {                                                  \
		memset(mfs_rx, 0, sizeof(struct lld_mfs_rx)); \
	} while (0)

/**
 * struct lld_mfs_tx - Tx MAC Frame Stream.
 */
struct lld_mfs_tx {
	/**
	 * @params: MFS parameters.
	 */
	struct lld_mfs_params params;
	/**
	 * @queue: Queue of buffers to be transmitted.
	 */
	struct sk_buff_head queue;
	/**
	 * @n_queued: Number of queued buffers.
	 */
	qatomic_int n_queued;
	/**
	 * @n_selected: Number of selected buffers.
	 */
	int n_selected;
	/**
	 * @seq: Sequence number to use for the next enqueued buffer.
	 */
	uint8_t seq;
	/**
	 * @sync: True if the Tx MFS is synchronised.
	 */
	bool sync;
};

/**
 * struct lld_mfs_tx_peer - Tx remote peer information.
 */
struct lld_mfs_tx_peer {
	/**
	 * @next_expected_seq: Next expected sequence numbers from peer.
	 */
	uint8_t next_expected_seq;
	/**
	 * @sync: True if the peer is synchronised.
	 */
	bool sync;
};

/**
 * struct lld_msdu_info - Info about MSDU.
 */
struct lld_msdu_info {
	/**
	 * @seq: Sequence number.
	 */
	uint8_t seq;
	/**
	 * @queued_time_ms: Time at which the MSDU was queued in a Tx MFS.
	 */
	uint64_t queued_time_ms;
};

/**
 * lld_mfs_tx_init() - Initialize Tx MFS.
 * @tx: Tx MAC Frame Stream.
 */
void lld_mfs_tx_init(struct lld_mfs_tx *tx);

/**
 * lld_mfs_tx_reset() - Reset MFS.
 * @tx: Tx MAC Frame Stream.
 * @stats: LLD MFS statistics.
 * @mfs_id: MFS id.
 */
void lld_mfs_tx_reset(struct lld_mfs_tx *tx, struct lld_mfs_stats *stats, enum lld_mfs_id mfs_id);

/**
 * lld_mfs_tx_queue() - Add buffer.
 * @llhw: Low-level device pointer.
 * @region: Pointer to the open region.
 * @tx: Tx MAC Frame Stream.
 * @mfs_id: MFS id.
 * @skb: Buffer.
 */
void lld_mfs_tx_queue(struct mcps802154_llhw *llhw, struct mcps802154_region *region,
		      struct lld_mfs_tx *tx, enum lld_mfs_id mfs_id, struct sk_buff *skb);

/**
 * lld_mfs_tx_get_queue_len() - Get queue length.
 * @tx: Tx MAC Frame Stream.
 *
 * Return: The queue length.
 */
int lld_mfs_tx_get_queue_len(struct lld_mfs_tx *tx);

/**
 * lld_mfs_tx_dequeue_expired() - Remove expired buffers from MFS queue.
 * @llhw: Low-level device pointer.
 * @region: Pointer to the open region.
 * @tx: Tx MAC Frame Stream.
 * @gen_stats: LLD MFS general statistics.
 *
 * Return: True if any buffer expired.
 */
bool lld_mfs_tx_dequeue_expired(struct mcps802154_llhw *llhw, struct mcps802154_region *region,
				struct lld_mfs_tx *tx, struct lld_mfs_stats *gen_stats);

/**
 * lld_mfs_tx_frame_msdu_put() - Put buffers into frame.
 * @frame_mtu: Chosen frame MTU for TX.
 * @tx: Tx MAC Frame Stream.
 * @mfs_id: MFS id.
 * @agg_limit_bytes: Limit MPDU aggregated payload size after the first
 * sub-frame.
 * @skb: Frame.
 *
 * Return: The number of buffers put into the frame.
 */
int lld_mfs_tx_frame_msdu_put(unsigned int frame_mtu, struct lld_mfs_tx *tx, enum lld_mfs_id mfs_id,
			      int agg_limit_bytes, struct sk_buff *skb);
/**
 * struct lld_mfs_tx_mcast - Tx multicast MAC Frame Stream.
 */
struct lld_mfs_tx_mcast {
	/**
	 * @tx: Tx MAC Frame Stream.
	 */
	struct lld_mfs_tx tx;
	/**
	 * @tx_peers: Tx peers.
	 */
	struct lld_mfs_tx_peer tx_peers[LLD_DEVICES_MAX];
	/**
	 * @n_tx_peers: Number of Tx peers.
	 */
	int n_tx_peers;
};

#define lld_mfs_tx_mcast_init(mfs_tx_mcast)                               \
	do {                                                              \
		memset(mfs_tx_mcast, 0, sizeof(struct lld_mfs_tx_mcast)); \
		lld_mfs_tx_init(&(mfs_tx_mcast)->tx);                     \
	} while (0)
#define lld_mfs_tx_mcast_reset(mfs_tx_mcast, mfs_stats, mfs_id) \
	lld_mfs_tx_reset(&(mfs_tx_mcast)->tx, mfs_stats, mfs_id)

/**
 * lld_mfs_tx_mcast_update_tx_state() - Update Tx MFS state.
 * @tx_mcast: Tx multicast MAC Frame Stream.
 *
 */
void lld_mfs_tx_mcast_update_tx_state(struct lld_mfs_tx_mcast *tx_mcast);

/**
 * lld_mfs_tx_mcast_update_peer() - Update peer information.
 * @tx_mcast: Tx multicast MAC Frame Stream.
 * @peer_id: Peer id.
 * @sync: True if remote Rx MFS state is sync.
 * @next_expected_seq: Next expected sequence number from remote peer.
 *
 * Return: QERR_SUCCESS or error.
 */
enum qerr lld_mfs_tx_mcast_update_peer(struct lld_mfs_tx_mcast *tx_mcast, int peer_id, bool sync,
				       uint8_t next_expected_seq);

/**
 * lld_mfs_seq_num_ack() - Ack Multicast MFS transmission.
 * @llhw: Low-level device pointer.
 * @region: Pointer to the open region.
 * @tx_mcast: Tx multicast MAC Frame Stream.
 * @gen_stats: LLD MFS general statistics.
 * @dev_stats: LLD MFS device statistics.
 */
void lld_mfs_seq_num_ack(struct mcps802154_llhw *llhw, struct mcps802154_region *region,
			 struct lld_mfs_tx_mcast *tx_mcast, struct lld_mfs_stats *gen_stats,
			 struct lld_mfs_stats *dev_stats);

/**
 * lld_mfs_single_bit_ack() - Handle single bit acknowledgment.
 * @llhw: Low-level device pointer.
 * @region: Pointer to the open region.
 * @tx: Tx MAC Frame Stream.
 * @stats: LLD MFS statistics.
 * @ack: True if frame was acknowledged.
 */
void lld_mfs_single_bit_ack(struct mcps802154_llhw *llhw, struct mcps802154_region *region,
			    struct lld_mfs_tx *tx, struct lld_mfs_stats *stats, bool ack);

/**
 * lld_mfs_rx_msdu_process() - Process received buffer.
 * @llhw: Low-level device pointer.
 * @region: Pointer to the open region.
 * @rx: Rx MAC Frame Stream.
 * @mfs_id: MFS id.
 * @skb: Buffer.
 * @seq: Sequence number of buffer.
 * @lqi: Link quality indicator.
 * @is_opm: True if the buffer is the oldest pending MSDU.
 *
 * Return: Length of MSDU received or -1 value when out of sync.
 */
int lld_mfs_rx_msdu_process(struct mcps802154_llhw *llhw, struct mcps802154_region *region,
			    struct lld_mfs_rx *rx, enum lld_mfs_id mfs_id, struct sk_buff *skb,
			    uint8_t seq, uint8_t lqi, bool is_opm);
