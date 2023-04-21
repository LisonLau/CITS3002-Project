#include <stdio.h>
#include <stdlib.h>
// src: https://barrgroup.com/embedded-systems/how-to/internet-protocol 

// IP header
// Data structures and constants for the IP layer
typedef struct { 
    uint8_t   ver_hlen;   /* Header version and length (dwords). */
    uint8_t   service;    /* Service type. */
    uint16_t  length;     /* Length of datagram (bytes). */
    uint16_t  ident;      /* Unique packet identification. */
    uint16_t  fragment;   /* Flags; Fragment offset. */
    uint8_t   timetolive; /* Packet time to live (in network). */
    uint8_t   protocol;   /* Upper level protocol (UDP, TCP). */
    uint16_t  checksum;   /* IP header checksum. */
    uint32_t  src_addr;   /* Source IP address. */
    uint32_t  dest_addr;  /* Destination IP address. */

} NetIpHeader; 

#define IP_VER_HLEN     0x45 
#define IP_HEADER_LEN   sizeof(NetIpHeader) 
#define IP_DATA_LEN     (MTU - IP_HEADER_LEN) 

// Actual IP Packet
typedef struct 
{ 
    NetIpHeader ipHeader; 
    uint8_t  data[IP_DATA_LEN]; 

} NetIpPkt;

// A function to send an IP packet
int NetIpSend(NetIpPkt * pIpPkt, uint16_t len) { 
    uint16_t ident; 

    // Assign a unique ID to the outgoing packet. 

    // Enter critical section here. 
    ident = gNextPacketID++; 
    // Leave critical section here. 

    /* 
     * Fill in the remaining IP header fields. 
     * (Some fields were pre-filled by the UDP layer.) 
     */ 
    pIpPkt->ipHeader.ver_hlen = IP_VER_HLEN; 
    pIpPkt->ipHeader.service = 0x00; 
    pIpPkt->ipHeader.length = htons(len); 
    pIpPkt->ipHeader.ident = htons(ident); 
    pIpPkt->ipHeader.fragment = 0x00; 
    pIpPkt->ipHeader.timetolive = 0x10; 

    /* 
     * Compute and fill in the IP header checksum. 
     */ 
    pIpPkt->ipHeader.checksum = NetIpChecksum((INT16U *) pIpPkt, IP_HEADER_LEN); 

    /* 
     * Forward the IP packet to the network interface driver. 
     */ 
    return (NetPhySnd(htons(PROTO_IP), (uint8_t *) pIpPkt, len)); 

}   /* NetIpSend() */

// Function to recieve an IP packet
int NetIpRcv(NetIpPkt * pIpPkt) { 
    uint16_t checksum; 


    /* 
     * Check IP header version and length. 
     */ 
    if (pIpPkt->ipHeader.ver_hlen != IP_VER_HLEN)
    { 
        /* 
         * Unsupported header version or length. 
         */ 
        return (NET_ERROR); 
    }

    /*
     * Move the IP header checksum out of the header. 
     */ 
    checksum = pIpPkt->ipHeader.checksum; 
    pIpPkt->ipHeader.checksum = 0; 

    /* 
     * Compute checksum and compare with received value. 
     */ 
    if (checksum != NetIpChecksum((uint16_t *) pIpPkt, IP_HEADER_LEN)) 
    {
        return (NET_ERROR); //Bad checksum 
    } 

    /* 
     * Route the packet to the appropriate Layer 3 protocol. 
     */ 
    switch (pIpPkt->ipHeader.protocol) 
    { 
      case PROTO_UDP: 
        return (NetUdpRcv((NetUdpPkt *) pIpPkt, ntohs(pIpPkt->ipHeader.length) 
			- IP_HEADER_LEN)); 

#if defined (NET_TCP_EN) 
      case PROTO_TCP: 
        return (NetTcpRcv((NetTcpPkt *) pIpPkt, ntohs(pIpPkt->ipHeader.length) 
			- IP_HEADER_LEN)); 
#endif 

      default: 
        return (NET_ERROR); // Unsupported protocol 
    } 

}   /* NetIpRcv() */