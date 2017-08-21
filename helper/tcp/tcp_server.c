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
	int sd, newsd;
	struct sockaddr_in my_end, his_end;
	socklen_t his_len;
	int ret;
	char buf[BUFSIZE];
	time_t cur_time;
	struct tm *cur_tm;
	int val;

	sd = socket(AF_INET, SOCK_STREAM, 0);
	if (sd == -1) {
		perror("socket()");
		goto socket_err;
	}

	val = 1;
	setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val));

	my_end.sin_family = AF_INET;
	my_end.sin_port = htons(8899);
	inet_pton(AF_INET, "192.168.0.101", &my_end.sin_addr);
	ret = bind(sd, (struct sockaddr *)&my_end, sizeof(my_end));
	if (ret == -1) {
		perror("bind()");
		goto bind_err;
	}

	listen(sd, 20);

	his_len = sizeof(his_end);	/* must init */
	newsd = accept(sd, (struct sockaddr *)&his_end, &his_len);
	if (newsd == -1) {
		perror("accept()");
		goto accept_err;
	}

	while (1) {
		ret = read(newsd, buf, BUFSIZE);
		if (ret == 0) {
			break;
		}

		write(1, buf, ret);

		cur_time = time(NULL);
		cur_tm = localtime(&cur_time);
		ret = strftime(buf, BUFSIZE, "%F %T", cur_tm);
		write(newsd, buf, ret);
		/* if error */
	}

	close(newsd);
	close(sd);

	return 0;

	close(newsd);
      accept_err:
      bind_err:
	close(sd);
      socket_err:
	return 1;
}
