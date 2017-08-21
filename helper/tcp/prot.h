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

//帧头部结构体，共14字节
typedef struct enet_header {
	u_char dest_mac[6];	//目的MAC地址 6字节
	u_char src_mac[6];	//源MAC地址 6字节
	u_short ethtype;	//上一层协议类型，如0x0800代表上一层是IP协议，0x0806为arp  2字节
} enet_header;

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
	WORD hdrlen;                         //首部长度4位，保留位6位 共10位
	WORD info_ctrl;		// Data Offset (4 bits), Reserved (6 bits), Control bits (6 bits)                intel使用小端模式
	WORD window;		// (16 bits)
	WORD checksum;		// (16 bits)
	WORD urgent_pointer;	// (16 bits)
} tcp_header;
/*
//TCP头部结构体，共20字节
struct TcpHeader
{
unsigned short SrcPort;                        //源端口号  2字节
unsigned short DstPort;                        //目的端口号 2字节
unsigned int SequenceNum;               //序号  4字节
unsigned int Acknowledgment;         //确认号  4字节
unsigned char HdrLen;                         //首部长度4位，保留位6位 共10位
unsigned char Flags;                              //标志位6位
unsigned short AdvertisedWindow;  //窗口大小16位 2字节
unsigned short Checksum;                  //校验和16位   2字节
unsigned short UrgPtr;						  //紧急指针16位   2字节
};
*/
#endif //_PROT_H
