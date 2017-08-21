#include <pcap.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>

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

int main()
{
	char errbuf[PCAP_ERRBUF_SIZE], *devstr;
	bpf_u_int32 mask;
	bpf_u_int32 net;

	/* get a device */
	devstr = pcap_lookupdev(errbuf);
	if (devstr) {
		printf("success: device: %s\n", devstr);
	} else {
		printf("error: %s\n", errbuf);
		exit(1);
	}

	pcap_lookupnet(devstr, &net, &mask, errbuf);

	/* open a device, wait until a packet arrives */
	pcap_t *device = pcap_open_live(devstr, 65535, 1, 0, errbuf);
	if (!device) {
		printf("error: pcap_open_live(): %s\n", errbuf);
		exit(1);
	}

	/* construct a filter */
#if 1
	struct bpf_program filter;
	pcap_compile(device, &filter, "dst port 8899", 1, net);
	pcap_setfilter(device, &filter);
#endif
	/* wait loop forever */
	int id = 0;
	pcap_loop(device, -1, get_packet, (u_char *) & id);

	pcap_close(device);

	return 0;
}
