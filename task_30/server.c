#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/poll.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <stdbool.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/un.h>
#include <signal.h>

#define PORT 1234
int pipes[2];

static void handler(int signum) {
	printf("\n");
	// printf("\nGot a signal: %d\n", signum);
	write(pipes[1], " ", 1);
}

int main(int argc, char *argv[]) {

	if(pipe(pipes) == -1) {
		perror("pipe err");
		return -1;
	}

	// Mask !
	// struct sigaction sa;

	// sa.sa_handler = handler;
	// sa.sa_flags = SA_RESTART;
	// sigaction(SIGINT, &sa, NULL);

	signal(SIGINT, handler);
	// siginterrupt(SIGINT, 0);

	int server_fd = socket(AF_INET6, SOCK_STREAM, 0);
	if (server_fd == -1) {
		perror("socket err");
		return -1;
	}


	int on = 1;
	if (ioctl(server_fd, FIONBIO, (char *)&on)  == -1) {
		perror("ioctl err");
		close(server_fd);
		return -1;
	}

	struct sockaddr_in6 addr;
	memset(&addr, 0, sizeof(addr));
	addr.sin6_family = AF_INET6;
	memcpy(&addr.sin6_addr, &in6addr_any, sizeof(in6addr_any));
	addr.sin6_port = htons(PORT);
	
	if (bind(server_fd, (struct sockaddr *)&addr, sizeof(addr))  == -1) {
		perror("bind err");
		close(server_fd);
		return -1;
	}

	if (listen(server_fd, 32)  == -1) {
		perror("listen err");
		close(server_fd);
		return -1;
	}

	struct pollfd fds[200];
	memset(fds, 0 , sizeof(fds));

	fds[0].fd = pipes[0];
	fds[0].events = POLLIN;

	fds[1].fd = server_fd;
	fds[1].events = POLLIN;

	int nfds = 2;

	bool stop;
	bool need_to_shift;

	do {
		//printf("[Server] Waiting on poll()...\n");
		int changed = poll(fds, nfds, -1);

		if (changed == -1) {
			if(errno == EINTR) {
				continue;
			}

			perror("poll err");
			break;
		}

		if(fds[0].revents) {
			printf("[Server] stopped by SIGINT\n");
			break;
		}

		int curr = nfds;
		for (int i = 1; i < curr; ++i) {	// For every socket..
			if(fds[i].revents == 0) {
				continue;
			}

			// if(fds[i].revents != POLLIN) { // Unexpected fd behaviour
			// 	stop = true;
			// 	break;
			// }

			if (fds[i].fd == server_fd) {	// There are new connections to accept
				printf("[Server] Accepting new connection\n");

				do {	// Accepting..
					int new_fd = accept(server_fd, NULL, NULL);
					if (new_fd  == -1) {	
						if (errno != EWOULDBLOCK) {	// EWOULDBLOCK == no sockets to accept
							perror("accept err");
							stop = true;
						}
						break;
					}

					printf("[Server] New incoming connection fd - %d\n", new_fd);
					fds[nfds].fd = new_fd;
					fds[nfds].events = POLLIN;
					nfds++;
					
					if(ioctl(new_fd, FIONBIO, (char *)&on) == -1) {
						perror("New fd ioctl err");
						stop = true;
						break;
					}
				} while (true);
			}

			else {	// There are socket to read
				printf("[Server] Connection %d is readable\n", fds[i].fd);
				stop = false;
				bool close_conn = false;
				
				do {
					char buff[100];
					int len = read(fds[i].fd, buff, sizeof(buff));
					if (len  == -1) {
						if (errno != EWOULDBLOCK) {	// EWOULDBLOCK == nothing else to read
							perror("recv err");
							close_conn = true;
						}

						break;
					}

					if (len == 0) { // Connection closed
						printf("[Server] Connection %d closed\n", fds[i].fd);
						close_conn = true;
						
						break;
					}
					printf("[Server] %d bytes received:\n", len);

					for(int j = 0; j < len; ++j) {
						buff[j] = toupper(buff[j]);
					}
					write(STDOUT_FILENO, buff, len);
				} while(true);
				
				if (close_conn) {
					close(fds[i].fd);
					fds[i].fd = -1;
					need_to_shift = true;
				}
			}
		}

		if (need_to_shift) {
			need_to_shift = false;
			for (int i = 1; i < nfds; i++) {
				if (fds[i].fd == -1) {
					for(int j = i; j < nfds; j++) {
						fds[j].fd = fds[j+1].fd;
					}
					i--;
					nfds--;
				}
			}
		}

	} while (!stop);

	for (int i = 1; i < nfds; i++) {
		if(fds[i].fd >= 0)
			close(fds[i].fd);
	}
	close(pipes[0]);
	close(pipes[1]);

	return 0;
}