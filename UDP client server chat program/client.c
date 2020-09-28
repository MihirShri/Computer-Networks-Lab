//Import necessary libraries
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<string.h>
#include<netdb.h>

void error(const char* msg)
{
	perror(msg);
	exit(1);
}


int main()
{
	int c_sock, n;
	char buff[255];
	struct sockaddr_in client;
	socklen_t client_len;
	
	//call socket() function with necessary parameters for UDP
	c_sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (c_sock < 0)
		printf("Error opening socket");
	
	client_len = sizeof(client);
	client.sin_family = AF_INET;
	client.sin_port = htons(9009);
	client.sin_addr.s_addr = INADDR_ANY;
	
	//loop until either the server or the client says "bye"
	while(1)
	{
		bzero(buff, 255);
		
		//get the message from client
		fgets(buff, 255, stdin);
		
		//send message to server
		n = sendto(c_sock, buff, sizeof(buff), 0, (struct sockaddr*)&client, sizeof(client));
		if (n < 0)
			error("Error in sending message to server");
		
		bzero(buff, 255);
		
		//receive message from server
		n = recvfrom(c_sock, buff, sizeof(buff), 0, (struct sockaddr*)&client, &client_len);
		if (n < 0)
			error("Error in receiving message from server");
		
		printf("Server: %s\n", buff);
		
		int i = strncmp("bye", buff, 3);
		if(i==0)
			break;
	}
	
	//close the socket
	close(c_sock);
	
	return 0;
}
