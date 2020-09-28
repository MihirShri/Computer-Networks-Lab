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
	char buff[255] = "\n";
	struct sockaddr_in client;
	socklen_t client_len;
	
	c_sock = socket(AF_INET, SOCK_DGRAM, 0);								//call socket() function with necessary parameters for UDP
	
	if (c_sock < 0)
		printf("Error opening socket");
		
	memset(&client, 0, sizeof(client));
	
	client_len = sizeof(client);
	client.sin_family = AF_INET;
	client.sin_port = htons(9009);
	client.sin_addr.s_addr = INADDR_ANY;

	do
	{
		int num1, num2, ans;
		
		sendto(c_sock, buff, sizeof(buff), 0, (struct sockaddr*)&client, sizeof(client));	
		
		bzero(buff, 255);
		n = recvfrom(c_sock, buff, 400, 0, (struct sockaddr*)&client, &client_len);			//receive the menu from server
		if (n < 0)
			error("Failed to receive message from server");
		printf("\nServer: %s\n", buff);
		scanf("%d", &c);										//enter choice
		sendto(c_sock, &c, sizeof(int), 0, (struct sockaddr*)&client, sizeof(client));		//send your choice to server
		
		if (c == 6)
			goto Q;
		
		bzero(buff, 255);
		n = recvfrom(c_sock, buff, 255, 0, (struct sockaddr*)&client, &client_len);						
		if (n < 0)
			error("Failed to receive message from server");
		printf("\nServer: %s\n", buff);
		scanf("%d", &num1);										//enter number1
		sendto(c_sock, &num1, sizeof(int), 0, (struct sockaddr*)&client, sizeof(client));		//send your choice to server
		
		bzero(buff, 255);
		n = recvfrom(c_sock, buff, 255, 0, (struct sockaddr*)&client, &client_len);
		if (n < 0)
			error("Failed to receive message from server");
		printf("\nServer: %s\n", buff);
		scanf("%d", &num2);										//enter number2
		sendto(c_sock, &num2, sizeof(int), 0, (struct sockaddr*)&client, sizeof(client));		//send your choice to server
		
		recvfrom(c_sock, &ans, sizeof(int), 0, (struct sockaddr*)&client, &client_len);		//receive answer from server
		printf("\nServer: Answer = %d\n", ans);
		
	}while(c != 6);
	
										
Q:	printf("Thank you for using my calculator :)\n");	
	close(c_sock);												//close the socket
	
	return 0;
}
