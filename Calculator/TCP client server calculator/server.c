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
	int s_sock, c_sock, n, c;
	char buff[255];
	struct sockaddr_in server, client;
	socklen_t client_len;
	
	//call socket() function with necessary parameters for UDP
	s_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (s_sock < 0)
		error("Error opening socket");
	
	memset(&server, 0, sizeof(server));
	memset(&client, 0, sizeof(client));
	
	server.sin_family = AF_INET;
	server.sin_port = htons(9009);
	server.sin_addr.s_addr = INADDR_ANY;
	
	if(bind(s_sock, (struct sockaddr*)&server, sizeof(server)) < 0)				//bind the client and the server
		error("Bind failed");
		
	listen(s_sock, 10);										//Specify the number of clients that can be connected at a time
	client_len = sizeof(client);
	
	c_sock = accept(s_sock, (struct sockaddr*)&client, &client_len);				//accept the connection request from the client
	
	if(c_sock < 0)
		error("Accept failed");
	
	do
	{
	
		int num1, num2, ans;
		
		n = send(c_sock, "\nEnter your choice: \n\n1. Addition \n2. Subtraction \n3. Multiplication \n4. Division \n5. Modulus \n6. Exit\n", strlen("Enter your choice: \n1. Addition 			\n2. Subtraction \n3.Multiplication \n4. Division \n5. Modulus \n6. Exit\n"), 0);	//send a menu displaying choices to the client
		
		if (n < 0)
			error("Error sending message to client");
		recv(c_sock, &c, sizeof(int), 0);							//receive the choice of the client
		printf("Client: My choice is %d\n", c);
		
		if (c == 6)
			goto Q;
		
		n = send(c_sock, "\nEnter number 1: ", strlen("\nEnter number 1: "), 0);
		if (n < 0)
			error("Error sending message to client");
		recv(c_sock, &num1, sizeof(int), 0);							//receive 1st number from client
		printf("Client: Number 1 is %d\n", num1);
		
		n = send(c_sock, "\nEnter number 2: ", strlen("\nEnter number 2: "), 0);
		if (n < 0)
			error("Error sending message to client");
		recv(c_sock, &num2, sizeof(int), 0);							//receive 2nd number from client
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
		
		send(c_sock, &ans, sizeof(int), 0);							//send answer to client
		
	}while(c != 6);
	
	
Q:	close(s_sock);											//close server
	close(c_sock);											//close client

	return 0;
}
