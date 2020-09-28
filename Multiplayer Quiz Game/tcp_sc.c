#include<stdio.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include <sys/time.h>
#include<netinet/in.h>
#include<stdlib.h>
#include<unistd.h>
#include<time.h>
#include <fcntl.h>
#include<sys/wait.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <sys/stat.h>
int timings(struct timeval * start, struct timeval * stop, struct timeval * delta)
{
        suseconds_t microstart, microstop, microdelta;

        microstart = (suseconds_t)(100000 * (start -> tv_sec)) + start -> tv_usec;
        microstop = (suseconds_t)(100000 * (stop -> tv_sec)) + stop -> tv_usec;
        microdelta = microstop - microstart;

        delta -> tv_usec = microdelta % 100000;
        delta -> tv_sec = (time_t)(microdelta / 100000);

        if (( * delta).tv_sec < 0 || ( * delta).tv_usec < 0)
                return -1;
        else
                return 0;
}

int questions(int c_sock, socklen_t add) 
{
        char msg[250];

        struct timeval tv;
        tv.tv_sec = 5;
        printf("\n");
        int count, timestamp;
        float sum = 0; //for choice ,to display result
        //Command Display
        struct timeval start, tss, tse, stop, delta, delta1;
        char s[5][100000];
        strcpy(s[4], "Enter the option character. You don't have to type the entire answer.\n\n\nWhat is the capital of Austria?\nChoices:\n  a:New Delhi\n  b:London\n  c:Buenos Aries\n  d:Sikkim ");
        strcpy(s[3], "what cricket team is called the Proteas?\nChoices:\n  a:India\n  b:South Africa\n  c:New Zealand\n  d:Australia ");
        strcpy(s[2], "Who is the \"Father of India\"?\nChoices:\n  a:Bal Gangadhar\n  b:Vallabhai Patel\n  c:Mahatma Gandhi\n  d:Gordon Ramsay");
        strcpy(s[1], "what is the National Sport of India?\nChoices:\n  a:Volleyball\n  b:Kabaddi\n  c:Cricket\n  d:Hockey ");
        strcpy(s[0], "what is 8/2*(2+2)?\nChoices:\n  a:1\n  b:8\n  c:16\n  d:4 ");

        char ans[5];
        ans[4] = 'c';
        ans[3] = 'b';
        ans[2] = 'c';
        ans[1] = 'd';
        ans[0] = 'c';

        strcpy(msg, "Enter \"ok\" to start the game");
        send(c_sock, msg, sizeof(msg), 0);
        bzero(msg, 250);

        //recv yes
        recv(c_sock, msg, sizeof(msg), 0);

        if (strcmp(msg, "ok") == 0) {

                strcpy(msg, "ack");
                send(c_sock, msg, sizeof(msg), 0);
                bzero(msg, 250);

                count = 5;
                sum = 0; //total time taken to store
                while (count--) {
                        strcpy(msg, s[count]);

                        send(c_sock, msg, sizeof(msg), 0);
                        gettimeofday( & start, NULL);

                        recv(c_sock, msg, sizeof(msg), 0);

                        gettimeofday( & stop, NULL);

                        timings(& start, & stop, & delta);
                        long int sec = delta.tv_sec;
                        if (sec < 0) sec *= -1;
                        //printf("\nTotal Time taken to receive: %ld \n", sec);
                        sum += sec;


                        if (msg[0] != ans[count]||sec>=10) {
                                bzero(msg, 250);
                                strcpy(msg, "Better luck next time");
                                send(c_sock, msg, sizeof(msg), 0);
                                sum=0;
                                break;
                        } else {
                                bzero(msg, 250);
                                strcpy(msg, "Continue");
                                send(c_sock, msg, sizeof(msg), 0);
                        }
                }
                bzero(msg, 250);
                strcpy(msg, "Game Over");
                send(c_sock, msg, sizeof(msg), 0);

        } 
        else 
        {
                strcpy(msg, "Invalid Choice");
                send(c_sock, msg, sizeof(msg), 0);
                exit(0);
        }
        return sum;
}

int main() 
{

        int s_sock, c_sock, len;
        s_sock = socket(AF_INET, SOCK_STREAM, 0);

        if (s_sock > 0) 
        {

                struct sockaddr_in server_addr, client_addr;

                memset( & server_addr, 0, sizeof(server_addr));
                memset( & client_addr, 0, sizeof(client_addr));

                //server
                server_addr.sin_family = AF_INET;
                server_addr.sin_port = htons(9025);
                server_addr.sin_addr.s_addr = INADDR_ANY;

                socklen_t add;
                add = sizeof(client_addr);

                if (bind(s_sock, (struct sockaddr * ) & server_addr, sizeof(server_addr)) >= 0) 
                {
                       
                        if ((listen(s_sock, 10)) != -1) {
                                printf("Server Listening.\n");
                                len = sizeof(client_addr);



                                while (1) 
                                {
                                        c_sock = accept(s_sock, (struct sockaddr * ) & client_addr, & len);

                                        if (c_sock != -1) {
                                                
                                                printf("[/////]Connection accepted from IP: %s PORT: %d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
                                                pid_t pid;
                                                
                                                //Clients Multithreaded
                                                if ((pid = fork()) == 0) 
                                                {

                                                        close(s_sock);
                                                        double t = (double) questions(c_sock, add);
                                                        if(t==0)
                                                        {
                                                        	printf("User at Port: %d was disqualified :( because either wrong answer or time exceeded 10 seconds. \n",ntohs(client_addr.sin_port));
                                                        }
                                                        else
                                                        {
                                                        	double avg = t / 5;
                                                        	printf("Avg Time taken by WINNER USER on PORT: %d is %f. All the other users can disconnect. We have a winner.\n", ntohs(client_addr.sin_port), avg);
                                                        	kill(pid,SIGKILL);
                                                    	}

                                                }


                                                if (pid == 0)
                                                    break;
                                                
                                        }
                                        else 
                                        {
                                                printf("ERROR AT ACCEPT:\n");
                                        }
                                }

                        } //listen
                        else 
                        {
                                printf("ERROR AT LISTEN:\n");
                        }
                }
                else 
                {
                        printf("ERROR AT BIND:\n");
                }
        }
        else 
        {

                printf("ERROR AT SOCKET:\n");
        }

        close(c_sock);
        return 0;

}