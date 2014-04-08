/*
 * proto_rtmp.c
 *
 * Copyright (C) 2014 Tomasz Bujlow <tomasz@skatnet.dk>
 *
 * This file is part of nDPI, an open source deep packet inspection
 * library based on the OpenDPI and PACE technology by ipoque GmbH
 *
 * nDPI is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * nDPI is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with nDPI.  If not, see <http://www.gnu.org/licenses/>.
 * 
 */


#include "ndpi_protocols.h"

void ndpi_search_rtmp(struct ndpi_detection_module_struct *ndpi_struct, struct ndpi_flow_struct *flow)
{
	struct ndpi_packet_struct *packet = &flow->packet;

	if (flow->rtmp_stage == 0 && packet->payload_packet_len > 0
		&& (packet->payload[0] == 0x03 || packet->payload[0] == 0x06)) {
		flow->rtmp_bytes = packet->payload_packet_len;
		if (packet->tcp->psh == 0) {
			NDPI_LOG(0, ndpi_struct, NDPI_LOG_DEBUG, "RTMP pass 1: \n");
			flow->rtmp_stage = packet->packet_direction + 1;

			NDPI_LOG(0, ndpi_struct, NDPI_LOG_DEBUG,
					"RTMP pass 1: rtmp_stage: %u, rtmp_bytes: %u\n", flow->rtmp_stage,
					flow->rtmp_bytes);
			return;
		} else if (packet->tcp->psh != 0 && flow->rtmp_bytes == 1537) {
			NDPI_LOG(0, ndpi_struct, NDPI_LOG_DEBUG,
					"RTMP hit: rtmp_stage: %u, rtmp_bytes: %u\n", flow->rtmp_stage,
					flow->rtmp_bytes);
			flow->rtmp_stage = 3;
			flow->ndpi_result_app = NDPI_RESULT_APP_RTMP;
			flow->ndpi_excluded_app[NDPI_RESULT_APP_RTMP] = 1;
			return;
		}
	} else if (flow->rtmp_stage == 1 + packet->packet_direction) {
		flow->rtmp_bytes += packet->payload_packet_len;
		if (packet->tcp->psh != 0 && flow->rtmp_bytes == 1537) {
			NDPI_LOG(0, ndpi_struct, NDPI_LOG_DEBUG,
					"RTMP hit: rtmp_stage: %u, rtmp_bytes: %u\n", flow->rtmp_stage,
					flow->rtmp_bytes);
			flow->rtmp_stage = 3;
			flow->ndpi_result_app = NDPI_RESULT_APP_RTMP;
			flow->ndpi_excluded_app[NDPI_RESULT_APP_RTMP] = 1;
			return;
		} else if (packet->tcp->psh == 0 && flow->rtmp_bytes < 1537) {
			NDPI_LOG(0, ndpi_struct, NDPI_LOG_DEBUG,
					"RTMP pass 2: rtmp_stage: %u, rtmp_bytes: %u\n", flow->rtmp_stage,
					flow->rtmp_bytes);
			return;
		}
	}

	/* Break after 20 packets. */
	if (flow->packet_counter > 20) {
	  NDPI_LOG(0, ndpi_struct, NDPI_LOG_DEBUG, "Exclude RTMP.\n");
	  flow->ndpi_excluded_app[NDPI_RESULT_APP_RTMP] = 1;
	  return;
	}
}

void ndpi_register_proto_rtmp (struct ndpi_detection_module_struct *ndpi_mod) {

  int tcp_ports[5] = {1935, 0, 0, 0, 0};
  int udp_ports[5] = {0, 0, 0, 0, 0};

  ndpi_initialize_scanner_app (ndpi_mod, NDPI_RESULT_APP_RTMP, "RTMP", NDPI_SELECTION_BITMASK_PROTOCOL_V4_V6_TCP_WITH_PAYLOAD, tcp_ports, udp_ports, ndpi_search_rtmp);
}
