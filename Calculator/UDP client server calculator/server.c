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
	int s_sock, n, c;
	char buff[255];
	struct sockaddr_in server, client;
	socklen_t client_len;
	
	s_sock = socket(AF_INET, SOCK_DGRAM, 0);												//call socket() function with necessary parameters for UDP
	if (s_sock < 0)
		error("Error opening socket");
	
	memset(&server, 0, sizeof(server));
	memset(&client, 0, sizeof(client));
	
	server.sin_family = AF_INET;
	server.sin_port = htons(9009);
	server.sin_addr.s_addr = INADDR_ANY;
	
	if(bind(s_sock, (struct sockaddr*)&server, sizeof(server)) < 0)									//bind the client and the server
		error("Bind failed");
									
	client_len = sizeof(client);
	
	do
	{
	
		int num1, num2, ans;
		
		recvfrom(s_sock, buff, sizeof(buff), 0, (struct sockaddr*)&client, &client_len);
		
		n = sendto(s_sock, "\nEnter your choice: \n\n1. Addition \n2. Subtraction \n3. Multiplication \n4. Division \n5. Modulus \n6. Exit\n", strlen("\nEnter your choice: \n\n1. Addition \n2. Subtraction \n3. Multiplication \n4. Division \n5. Modulus \n6. Exit\n"), 0, (struct sockaddr*)&client, sizeof(client));		//send a menu displaying choices to the client
		
		if (n < 0)
			error("Error sending message to client1");
		recvfrom(s_sock, &c, sizeof(int), 0, (struct sockaddr*)&client, &client_len);						//receive the choice of the client
		printf("Client: My choice is %d\n", c);
		
		if (c == 6)
			goto Q;
		
		n = sendto(s_sock, "\nEnter number 1: ", strlen("\nEnter number 1: "), 0, (struct sockaddr*)&client, sizeof(client));
		if (n < 0)
			error("Error sending message to client2");
		recvfrom(s_sock, &num1, sizeof(int), 0, (struct sockaddr*)&client, &client_len);						//receive 1st number from client
		printf("Client: Number 1 is %d\n", num1);
		
		n = sendto(s_sock, "\nEnter number 2: ", strlen("\nEnter number 2: "), 0, (struct sockaddr*)&client, sizeof(client));
		if (n < 0)
			error("Error sending message to client3");
		recvfrom(s_sock, &num2, sizeof(int), 0, (struct sockaddr*)&client, &client_len);						//receive 2nd number from client
		printf("Client: Number 2 is %d\n", num2);
		
		switch(c)
		{
			case 1: ans = num1 + num2;
				break;
			
			case 2: ans = num1 - num2;
				break;
				
			case 3: ans = num1 * num2;
				break;
				
			case 4: ans = num1 / num2;
				break;
				
			case 5: ans = num1 % num2;
				break;
				
			case 6: goto Q;
				break;
		}
		
		sendto(s_sock, &ans, sizeof(int), 0, (struct sockaddr*)&client, sizeof(client));						//send answer to client
		
	}while(c != 6);
	
	
Q:	close(s_sock);																//close server
										

	return 0;
}
