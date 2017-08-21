#include <pcap.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>		/* superset of previous */
#include <arpa/inet.h>
#include "prot.h"

void get_packet(u_char * arg, const struct pcap_pkthdr *pkthdr,
		const u_char * packet)
{
	int *id = (int *)arg;

	printf("id: %d\n", ++(*id));
	printf("Packet length: %d\n", pkthdr->len);
	printf("Number of bytes: %d\n", pkthdr->caplen);
	printf("Recieved time: %s", ctime((const time_t *)&pkthdr->ts.tv_sec));

	int i;
	for (i = 0; i < pkthdr->len; ++i) {
		printf(" %02x", packet[i]);
		if ((i + 1) % 16 == 0) {
			printf("\n");
		}
	}

	printf("\n\n");
}

void dispatcher_handler(u_char * temp1,
			const struct pcap_pkthdr *header,
			const u_char * pkt_data)
{
	struct tm *ltime;
	char timestr[16];
	enet_header *eh;
	ip_header *ih;
	tcp_header *th;
	u_int ip_len;
	u_short sport, dport;
	time_t local_tv_sec;
	int i = 0;
	u_char mac[6];

	/* convert the timestamp to readable format */
	local_tv_sec = header->ts.tv_sec;
	ltime = localtime(&local_tv_sec);
	strftime(timestr, sizeof timestr, "%H:%M:%S", ltime);

	/* print timestamp and length of the packet */
	printf("%s.%.6d len:%d ", timestr, (int)header->ts.tv_usec, header->len);
	
	/* 获取以太网帧的目的MAC */
	eh = (enet_header *)(pkt_data);
	for (i = 0; i < 6; i++) {
		mac[i] = *((u_char *)eh + 6 + i);
		printf("%02x:", mac[i]);
	}
	printf("\n");

	/* retireve the position of the ip header */
	ih = (ip_header *) (pkt_data + 14);	//length of ethernet header

	/* retireve the position of the tcp header */
	//从IPV4首部中取出"首部长度(4 bits)"
	ip_len = (ih->ver_ihl & 0xf) * 4;
	//强制类型转换，便于用自己的命名处理
	th = (tcp_header *) ((u_char *) ih + ip_len);

	/* convert from network byte order to host byte order */
	sport = ntohs(th->source_port);
	dport = ntohs(th->destination_port);

	/* print ip addresses and udp ports */
	printf("%d.%d.%d.%d.%d -> %d.%d.%d.%d.%d\n",
	       ih->saddr.byte1,
	       ih->saddr.byte2,
	       ih->saddr.byte3,
	       ih->saddr.byte4,
	       sport,
	       ih->daddr.byte1,
	       ih->daddr.byte2, ih->daddr.byte3, ih->daddr.byte4, dport);
}

int main()
{
	char errbuf[PCAP_ERRBUF_SIZE], *devstr;
	bpf_u_int32 maskp;
	bpf_u_int32 netp;
	struct in_addr addr;
	/* get the network address in a human readable form */
	char *net;		/* dot notation of the network address */
	char *mask;		/* dot notation of the network mask */
	int ret = 0;

	/* get a device */
	devstr = pcap_lookupdev(errbuf);
	if (devstr) {
		printf("success: device: %s\n", devstr);
	} else {
		printf("error: %s\n", errbuf);
		exit(1);
	}

	ret = pcap_lookupnet(devstr, &netp, &maskp, errbuf);
	if (ret == -1) {
		printf("pcap_lookupnet() error: %s\n", errbuf);
		exit(1);
	}

	addr.s_addr = netp;
	net = inet_ntoa(addr);
	if (!net) {
		perror("inet_ntoa() ip error: ");
		exit(1);
	}
	printf("ip: %s\n", net);

	/* do the same as above for the device's mask */
	addr.s_addr = maskp;
	mask = inet_ntoa(addr);
	if (!mask) {
		perror("inet_ntoa() sub mask error: ");
		exit(1);
	}
	printf("sub mask: %s\n", mask);

	/* open a device, wait until a packet arrives */
	pcap_t *device = pcap_open_live(devstr, 65535, 1, 0, errbuf);
	if (!device) {
		printf("error: pcap_open_live(): %s\n", errbuf);
		exit(1);
	}

	/* construct a filter */
#if 1
	struct bpf_program filter;
	pcap_compile(device, &filter, "dst port 8899", 1, netp);
	pcap_setfilter(device, &filter);
#endif
	/* wait loop forever */
	int id = 0;
#ifdef DEBUG
	pcap_loop(device, -1, get_packet, (u_char *) & id);
#else
	pcap_loop(device, -1, dispatcher_handler, (u_char *) & id);
#endif
	pcap_close(device);

	return 0;
}
