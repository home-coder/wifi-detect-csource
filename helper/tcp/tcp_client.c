#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <unistd.h>
#include <time.h>

#include <stdio.h>

#define BUFSIZE 64

int main(void)
{
	int sd;
	struct sockaddr_in his_end;
	socklen_t his_len;
	int ret;
	char buf[BUFSIZE];

	sd = socket(AF_INET, SOCK_STREAM, 0);
	if (sd == -1) {
		perror("socket()");
		goto socket_err;
	}

	his_end.sin_family = AF_INET;
	his_end.sin_port = htons(8899);
	inet_pton(AF_INET, "192.168.0.101", &his_end.sin_addr);
	his_len = sizeof(his_end);
	ret = connect(sd, (struct sockaddr *)&his_end, his_len);
	if (ret == -1) {
		perror("connect()");
		goto connect_err;
	}

	while (1) {
		ret = read(0, buf, BUFSIZE);
		if (ret == 0) {
			break;
		}

		write(sd, buf, ret);

		ret = read(sd, buf, BUFSIZE);
		write(1, buf, ret);
	}

	close(sd);

	return 0;

      connect_err:
	close(sd);
      socket_err:
	return 1;
}
