/*
 * viber.c 
 *
 * Copyright (C) 2013 Remy Mudingay <mudingay@ill.fr>
 * Copyright (C) 2014 Tomasz Bujlow <tomasz@skatnet.dk>
 *
 * This file is part of nDPIng, an open source deep packet inspection
 * library based on nDPI, OpenDPI, and PACE technology by ipoque GmbH
 *
 * nDPIng is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * nDPIng is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with nDPIng.  If not, see <http://www.gnu.org/licenses/>.
 * 
 */

#include "ndpi_utils.h"
#include "ndpi_protocols.h"

#ifdef NDPI_OLD_RESULT_APP_TEAMSPEAK

static void ndpi_int_teamspeak_add_connection(struct ndpi_detection_module_struct
                                             *ndpi_struct, struct ndpi_flow_struct *flow)
{
    ndpi_int_add_connection(ndpi_struct, flow, NDPI_OLD_RESULT_APP_TEAMSPEAK, NDPI_REAL_PROTOCOL);
}
  u_int16_t tdport = 0, tsport = 0;
  u_int16_t udport = 0, usport = 0;


void ndpi_search_teamspeak(struct ndpi_detection_module_struct *ndpi_struct, struct ndpi_flow_struct *flow)
{
    struct ndpi_packet_struct *packet = &flow->packet;

if (packet->udp != NULL) {
  usport = ntohs(packet->udp->source), udport = ntohs(packet->udp->dest);
  /* http://www.imfirewall.com/en/protocols/teamSpeak.htm  */
  if (((usport == 9987 || udport == 9987) || (usport == 8767 || udport == 8767)) && packet->payload_packet_len >= 20) {
     NDPI_LOG(NDPI_OLD_RESULT_APP_TEAMSPEAK, ndpi_struct, NDPI_LOG_DEBUG, "found TEAMSPEAK udp.\n");
     ndpi_int_teamspeak_add_connection(ndpi_struct, flow);
  }
}
else if (packet->tcp != NULL) {
  tsport = ntohs(packet->tcp->source), tdport = ntohs(packet->tcp->dest);
  /* https://github.com/Youx/soliloque-server/wiki/Connection-packet */
  if(packet->payload_packet_len >= 20) {
    if (((memcmp(packet->payload, "\xf4\xbe\x03\x00", 4) == 0)) ||
          ((memcmp(packet->payload, "\xf4\xbe\x02\x00", 4) == 0)) ||
            ((memcmp(packet->payload, "\xf4\xbe\x01\x00", 4) == 0))) {
     NDPI_LOG(NDPI_OLD_RESULT_APP_TEAMSPEAK, ndpi_struct, NDPI_LOG_DEBUG, "found TEAMSPEAK tcp.\n");
     ndpi_int_teamspeak_add_connection(ndpi_struct, flow);
    }  /* http://www.imfirewall.com/en/protocols/teamSpeak.htm  */
  } else if ((tsport == 14534 || tdport == 14534) || (tsport == 51234 || tdport == 51234)) {
     NDPI_LOG(NDPI_OLD_RESULT_APP_TEAMSPEAK, ndpi_struct, NDPI_LOG_DEBUG, "found TEAMSPEAK.\n");
     ndpi_int_teamspeak_add_connection(ndpi_struct, flow);
   }
  }
  NDPI_LOG(NDPI_OLD_RESULT_APP_TEAMSPEAK, ndpi_struct, NDPI_LOG_DEBUG, "TEAMSPEAK excluded.\n");
  NDPI_ADD_PROTOCOL_TO_BITMASK(flow->excluded_protocol_bitmask, NDPI_OLD_RESULT_APP_TEAMSPEAK);
  return;
}
#endif
