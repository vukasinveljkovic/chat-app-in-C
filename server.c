#include <sys/socket.h>
#include <arpa/inet.h>
#include <poll.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
	if(argc < 2 || argc > 2)
	{
		printf("\n");
		printf("Usage:\n");
		printf("./server <PORT>\n");
		printf("\n");
		return -1;
	}

	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	
	if(sockfd < 0)
	{
		perror("SOCKET ERROR: ");
		return -1;
	}
	
	struct sockaddr_in address = {
		AF_INET,
		htons(atoi(argv[1])),
		0
	};

	if(bind(sockfd, (struct sockaddr *) &address, sizeof(address)) < 0)
	{
		printf("Please wait few more minutes before starting the server again.\n");
		perror("BIND ERROR: ");
		return -1;
	}

	if(listen(sockfd, 10) < 0)
	{
		perror("LISTEN ERROR: ");
		return -1;
	}
	
	int clientfd = accept(sockfd, 0, 0);
	
	if(clientfd < 0)
	{
		printf("Connection failed\n");
	}

	struct pollfd pollfd[2] = {
		{
			0, // <-- stdin
			POLLIN,
			0
		}, 
		{
			clientfd,
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
			send(clientfd, buffer, 255, 0);
		}
		else if(pollfd[1].revents & POLLIN)
		{
			recv(clientfd, buffer, 255, 0);
			printf("%s", buffer);
		}
	}


}
