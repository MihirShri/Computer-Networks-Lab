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
	int c_sock, n, c;
	char buff[255];
	struct sockaddr_in client;
	socklen_t client_len;
	
	c_sock = socket(AF_INET, SOCK_STREAM, 0);					//call socket() function with necessary parameters for TCP
	
	if (c_sock < 0)
		printf("Error opening socket");
		
	memset(&client, 0, sizeof(client));
	
	client_len = sizeof(client);
	client.sin_family = AF_INET;
	client.sin_port = htons(9009);
	client.sin_addr.s_addr = INADDR_ANY;

	if (connect(c_sock, (struct sockaddr*)&client, sizeof(client)) == -1)	//establish connection
		error("Server is busy");
		
	do
	{
		int num1, num2, ans;
		
		bzero(buff, 255);
		n = recv(c_sock, buff, 255, 0);					//receive the menu from server
		if (n < 0)
			error("Failed to receive message from server");
		printf("\nServer: %s\n", buff);
		scanf("%d", &c);							//enter choice
		send(c_sock, &c, sizeof(int), 0);					//send your choice to server
		
		if (c == 6)
			goto Q;
		
		bzero(buff, 255);
		n = recv(c_sock, buff, 255, 0);						
		if (n < 0)
			error("Failed to receive message from server");
		printf("\nServer: %s\n", buff);
		scanf("%d", &num1);							//enter number1
		send(c_sock, &num1, sizeof(int), 0);					//send your choice to server
		
		bzero(buff, 255);
		n = recv(c_sock, buff, 255, 0);
		if (n < 0)
			error("Failed to receive message from server");
		printf("\nServer: %s\n", buff);
		scanf("%d", &num2);							//enter number2
		send(c_sock, &num2, sizeof(int), 0);					//send your choice to server
		
		recv(c_sock, &ans, sizeof(int), 0);					//receive answer from server
		printf("\nServer: Answer = %d\n", ans);
		
	}while(c != 6);
	
	//close the socket
Q:	printf("Thank you for using my calculator :)\n");	
	close(c_sock);
	
	return 0;
}
