#ifndef _PROT_H
#define _PROT_H
typedef uint16_t WORD;
typedef uint32_t DWORD;

/* 4 bytes IP address */
typedef struct ip_address {
	u_char byte1;
	u_char byte2;
	u_char byte3;
	u_char byte4;
} ip_address;

/* IPv4 header */
typedef struct ip_header {
	u_char ver_ihl;		// Version (4 bits) + Internet header length (4 bits)
	u_char tos;		// Type of service 
	u_short tlen;		// Total length 
	u_short identification;	// Identification
	u_short flags_fo;	// Flags (3 bits) + Fragment offset (13 bits)
	u_char ttl;		// Time to live
	u_char proto;		// Protocol
	u_short crc;		// Header checksum
	ip_address saddr;	// Source address
	ip_address daddr;	// Destination address
	u_int op_pad;		// Option + Padding
} ip_header;

/* UDP header*/
typedef struct udp_header {
	u_short sport;		// Source port
	u_short dport;		// Destination port
	u_short len;		// Datagram length
	u_short crc;		// Checksum
} udp_header;

// TCP首部
typedef struct tcp_header {
	WORD source_port;	// (16 bits)                         Winsock 内置函数 ntohs（），主要作用将大端转换为小端！
	WORD destination_port;	// (16 bits)                         Winsock 内置函数 ntohs（），主要作用将大端转换为小端！
	DWORD seq_number;	// Sequence Number (32 bits)         大小端原因，高低位4个8bit的存放顺序是反的，intel使用小端模式
	DWORD ack_number;	// Acknowledgment Number (32 bits)     大小端原因，高低位4个8bit的存放顺序是反的，intel使用小端模式
	WORD info_ctrl;		// Data Offset (4 bits), Reserved (6 bits), Control bits (6 bits)                intel使用小端模式
	WORD window;		// (16 bits)
	WORD checksum;		// (16 bits)
	WORD urgent_pointer;	// (16 bits)
} tcp_header;

#endif //_PROT_H
