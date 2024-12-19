#pragma once

#include "net/mcps802154.h"
#include "qerr.h"

#include <stdint.h>

struct fira_session;
struct fira_report_info;
struct fira_workspace;
struct sk_buff;

/**
 * fira_diagnostics_set_tx() - Set Tx related diagnostics.
 * @session: FiRa session.
 * @slot_idx: slot index.
 * @tx_info: tx frame info.
 */
void fira_diagnostics_set_tx(struct fira_session *session, int slot_idx,
			     const struct mcps802154_tx_frame_info *tx_info);

/**
 * fira_diagnostics_set_rx() - Set Rx related diagnostics.
 * @session: FiRa session.
 * @index: index.
 * @src_short_addr: short address of a device which sent this message.
 * @rx_ctx: rx context.
 * @rx_info: rx frame info.
 * @error: error type.
 */
void fira_diagnostics_set_rx(struct fira_session *session, int16_t index, uint16_t src_short_addr,
			     void *rx_ctx, const struct mcps802154_rx_frame_info *rx_info,
			     enum mcps802154_rx_error_type error);

/**
 * fira_diagnostics_report_ranging_diagnostics() - Reports ranging diagnostics.
 * @session: FiRa session.
 * @llhw: llhw data.
 * @report_info: Report info.
 * @msg: message buffer.
 *
 * Return: QERR_SUCCESS or error.
 */
enum qerr fira_diagnostics_report_ranging_diagnostics(const struct fira_session *session,
						      const struct mcps802154_llhw *llhw,
						      const struct fira_report_info *report_info,
						      struct sk_buff *msg);

/**
 * fira_diagnostics_alloc() - Allocate diagnostics.
 * @ws: FiRa workspace context.
 * @next_index: Index in the array of diagnostics to allocate first element.
 * @n_diags: Number of elements to allocate.
 *
 * Return: QERR_SUCCESS or error.
 */
enum qerr fira_diagnostics_alloc(struct fira_workspace *ws, int next_index, int n_diags);

/**
 * fira_diagnostics_destroy() - Destroy diagnostics elements from array.
 * @ws: FiRa workspace context.
 */
void fira_diagnostics_destroy(struct fira_workspace *ws);
