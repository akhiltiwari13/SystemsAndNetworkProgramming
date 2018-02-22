/* author: akhil
 * description: a daemon that broadcasts a request to maintaing the same state throughout a network.
 * uses a simple BLAB methodology for server implementation.
 * created on: 17th Feb 2018
 */
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <unistd.h>
#include <ctype.h>

#define SERVER_PORT 1067 

void rot13(unsigned char *s, int n)
{
  unsigned char *p;
  for (p=s; p < s + n; p++)
  {
    if (islower(*p))
    {
      *p += 13;
      if (*p > 'z') *p -= 26;
    }
  }
} 

void rot13_service(int in, int out)
{
  unsigned char buf[1024];
  int count;
  while ((count = read(in, buf, 1024)) > 0)
  {
    rot13(buf, count);
    write(out, buf, count);
  }
}

int main(int argc, char **argv)
{
	
	sockaddr_in *server, *client;
	int sock, fd;	 
	socklen_t client_len;

	

	server= (sockaddr_in *)malloc(sizeof(sockaddr_in));
	server->sin_family= AF_INET;							   //using the address family of IPV4.
 	(server->sin_addr).s_addr= htonl(INADDR_ANY);					   //INADDR_ANY Macro lets the server listen to all available IPs of the server.
	server->sin_port= htons(SERVER_PORT);						   //Setting up service's port.
	sock= socket(AF_INET, SOCK_STREAM, 0);						   //Created a socket for IPV4 endpt addresss and using TCP protocol.
		
	bind(sock, (struct sockaddr *)server, sizeof(sockaddr_in));			   //bind the server to the socket
	
	listen(sock, 5);								   //an iterative server using a queue of length 5;
	printf("listening\n");
	
	
	client= (sockaddr_in *)malloc(sizeof(sockaddr_in));
	while(1)
	{
	client_len =sizeof(sockaddr_in);
	fd= accept(sock, (struct sockaddr *)client, &client_len);
	printf("got connection\n");
	printf("response begining\n");
    rot13_service(fd, fd);
	printf("response end\n");
	close(fd);
	}

	return 0;
}
