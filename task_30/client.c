#include <sys/socket.h>
#include <sys/un.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/ioctl.h>

#define PORT 1234

int main(int argc, char* argv[]) {
	int con_fd = socket(AF_INET6, SOCK_STREAM, 0);
	if (con_fd == -1) {
		perror("socket err");
		return -1;
	}

	struct sockaddr_in6 addr;
	memset(&addr, 0, sizeof(addr));
	addr.sin6_family = AF_INET6;
	memcpy(&addr.sin6_addr, &in6addr_any, sizeof(in6addr_any));
	addr.sin6_port = htons(PORT);

	if (connect(con_fd, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
			close(con_fd);
			perror("connect err");
			return -1;
	}


	char buff[100];	
	int size;

	while((size = read(STDIN_FILENO, buff, sizeof(buff))) > 0) {
		// write(STDOUT_FILENO, "!", 1);
		// write(STDOUT_FILENO, buff, size);
		// write(STDOUT_FILENO, "!\n", 2);
		
		if (write(con_fd, buff, size) < 0) {
			perror("Write err");
			close(con_fd);
			return -1;
		}
	}

	close(con_fd);
	return 0;
}