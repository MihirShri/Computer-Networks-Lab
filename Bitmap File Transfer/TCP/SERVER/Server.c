 #include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<string.h>
#include<unistd.h>


void error(const char* msg)
{
	perror(msg);
	exit(1);
}


int main()
{
	int s_sock, s_sock_new;
	
	//call socket() function with necessary parameters for TCP
	s_sock = socket(AF_INET, SOCK_STREAM, 0);
	
	if (s_sock < 0)
		error("Error opening socket");

	struct sockaddr_in server, other;
	memset(&server, 0, sizeof(server));
	memset(&other, 0, sizeof(other));

	server.sin_family = AF_INET;
	server.sin_port = htons(9009);
	server.sin_addr.s_addr = INADDR_ANY;
		
	//bind the client and the server
	if(bind(s_sock, (struct sockaddr*)&server, sizeof(server)) != -1)
	{
		printf("Yo! This is SERVER.\n");
		socklen_t add;
		add = sizeof(other);
		
		//Specify the number of clients that can be connected at a time
		listen(s_sock, 5);
		
		//accept the connection request from the client
		s_sock_new = accept(s_sock, (struct sockaddr*)&other, &add);
	}
	else
	{
		error("\nError in Bind");
		return 1;
	}

	char fname[100], extension[100] = "", ext[10], buffer[2000], buf[1];

	//receive the fname and extension from the server
	recv(s_sock_new, fname, sizeof(fname), 0);
	recv(s_sock_new, ext, sizeof(ext), 0);

	//create a new file to which the contents of the file received will be dumped
	strcat(extension, fname);
	strcat(extension, "_received.");
	strcat(extension, ext);
	
	//open the file in write mode
	FILE *fp;
	fp = fopen(extension, "w");

	char eof[1] = "0";

	int flag = 0;
	while(flag == 0)
	{
		//receive the end of file signal from the client
		recv(s_sock_new, eof, sizeof(eof), 0);
		
		if(eof[0] == '1') 
			flag = 1;
			
		else 
		{
			//continue receiving data until end of file signal is received from the client
			recv(s_sock_new, buf, sizeof(buf), 0);
			fprintf(fp, "%c", buf[0]);
		}
	}
	
	printf("\n\nDone receiving file.\n\n");

	//close the file
	fclose(fp);
		
	printf("File %s Received Succesfully.\n", extension);

	//close server and connection
	close(s_sock_new);
	close(s_sock);
	
	return 0;
}

