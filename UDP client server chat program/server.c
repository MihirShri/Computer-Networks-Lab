//Import necessary libraries
#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<string.h>

void error(const char* msg)
{
	perror(msg);
	exit(1);
}

int main()
{
	int s_sock, n;
	char buff[255];
	struct sockaddr_in server, client;
	socklen_t client_len;
	
	//call socket() function with necessary parameters for UDP
	s_sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (s_sock < 0)
		error("Error opening socket");
	
	server.sin_family = AF_INET;
	server.sin_port = htons(9009);
	server.sin_addr.s_addr = INADDR_ANY;
	if(bind(s_sock, (struct sockaddr*)&server, sizeof(server)) < 0)
		error("Bind failed");
	client_len = sizeof(client);
	
	//loop untill either the server or the client says "bye"
	while(1)
	{
		bzero(buff, 255);
		
		//receive message from server
		n = recvfrom(s_sock, buff, sizeof(buff), 0, (struct sockaddr*)&client, &client_len);
		if (n < 0)
			error("Error in receiving message from client");
		printf("Client: %s\n", buff);
		
		bzero(buff, 255);
		
		//get the message from server
		fgets(buff, 255, stdin);
		
		//send message to client
		n = sendto(s_sock, buff, sizeof(buff), 0, (struct sockaddr*)&client, sizeof(client));
		if (n < 0)
			error("Error in sending message to client");
		
		int i = strncmp("bye", buff, 3);
		if(i==0)
			break;
		
	}
	
	//close the socket
	close(s_sock);
	
	return 0;
}
