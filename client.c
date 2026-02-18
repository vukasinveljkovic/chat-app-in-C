#include <sys/socket.h>
#include <arpa/inet.h>
#include <poll.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
	if(argc < 3)
	{
		printf("\n");
		printf("Usage:\n");
		printf("./client <IP_ADDRESS> <PORT>\n");
		printf("To use the loopback address, you can type 0 in the <IP_ADDRESS>\n");
		printf("\n");
	}

	int sockfd = socket(AF_INET, SOCK_STREAM, 0);

	if(sockfd < 0)
	{
		perror("SOCKET ERROR: ");
		return -1;
	}

	struct sockaddr_in address = {
		AF_INET,
		htons(atoi(argv[2]))
	};

	if(inet_aton(argv[1], &address.sin_addr) < 1)
	{
		printf("Incorrect address\n");
		return -1;
	}

	if(connect(sockfd, (struct sockaddr *) &address, sizeof(address)) < 0)
	{
		printf("Failed to connect\n");
		return -1;
	}

	struct pollfd pollfd[2] = {
		{
			0, // <-- stdin
			POLLIN,
			0
		}, 
		{
			sockfd,
			POLLIN,
			0
		}
	};
	
	while(1)
	{
		char buffer[256] = { 0 };

		poll(pollfd, 2, 5000);

		if(pollfd[0].revents & POLLIN)
		{
			read(0, buffer, 255);
			send(sockfd, buffer, 255, 0);
		}
		else if(pollfd[1].revents & POLLIN)
		{
			recv(sockfd, buffer, 255, 0);
			printf("%s", buffer);
		}
	}


}
