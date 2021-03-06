/*
 * viber.c 
 *
 * Copyright (C) 2013 Remy Mudingay <mudingay@ill.fr>
 * Copyright (C) 2013 - 2014 ntop.org
 *
 * This module is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This module is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public License.
 * If not, see <http://www.gnu.org/licenses/>.
 */


#include "ndpi_utils.h"
#include "ndpi_protocols.h"

#ifdef NDPI_PROTOCOL_VIBER

void ndpi_search_viber(struct ndpi_detection_module_struct *ndpi_struct, struct ndpi_flow_struct *flow)
{
  struct ndpi_packet_struct *packet = &flow->packet;
  u_int16_t dport = 0, sport = 0;
  
  NDPI_LOG(NDPI_PROTOCOL_VIBER, ndpi_struct, NDPI_LOG_DEBUG, "search for VIBER.\n");
  
  if(packet->udp != NULL) {
    sport = ntohs(packet->udp->source), dport = ntohs(packet->udp->dest);
    NDPI_LOG(NDPI_PROTOCOL_VIBER, ndpi_struct, NDPI_LOG_DEBUG, "calculating dport over udp.\n");

    if((packet->payload_packet_len == 12 && packet->payload[2] == 0x03 && packet->payload[3] == 0x00)
       || (packet->payload_packet_len == 20 && packet->payload[2] == 0x09 && packet->payload[3] == 0x00)
       || ((packet->payload_packet_len < 135) && (packet->payload[0] == 0x11))) {
      NDPI_LOG(NDPI_PROTOCOL_VIBER, ndpi_struct, NDPI_LOG_DEBUG, "found VIBER.\n");
      ndpi_int_add_connection(ndpi_struct, flow, NDPI_PROTOCOL_VIBER, NDPI_REAL_PROTOCOL);
      return;
    } 
  }

  NDPI_LOG(NDPI_PROTOCOL_VIBER, ndpi_struct, NDPI_LOG_DEBUG, "exclude VIBER.\n");
  NDPI_ADD_PROTOCOL_TO_BITMASK(flow->excluded_protocol_bitmask, NDPI_PROTOCOL_VIBER);
}

#endif
