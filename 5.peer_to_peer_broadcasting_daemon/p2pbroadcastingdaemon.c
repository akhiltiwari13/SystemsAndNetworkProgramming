/* *
 * author akhil
 * a peer to peer broadcasting daemon that used udp datagrams to broadcast within the network. 
 * date 17/02/2018
 * */
 
 #include <stdlib.h>
 #include <stdio.h>
 #include <stdio.h>
 #include <sys/socket.h>
 #include <netinet/in.h>
 #include <string.h>
 #include <unistd.h>
 #include <ncurses.h>
 #include <netdb.h>
 
 #define BROADCAST_PORT 6666
 #define TRUE 1
 
 char text[64];
 

 void display_state(char *text); //funtion prototype
 
 int main ( int argc , char * argv[])
 {
	 int yes= TRUE;
	 struct sockaddr_in *server; //has the broadcast address
	 struct sockaddr_in *client;
	 socklen_t client_len;
	 
	  /* Initialise the curses package */
    initscr();
    cbreak();	/* Disable input buffering */
    noecho();	/* Disable echoing */
	 
	 int sock= socket(AF_INET, SOCK_DGRAM, 0); //created datagram socket.
	 setsockopt(sock, SOL_SOCKET, SO_BROADCAST, (char *)&yes, sizeof(yes)); //setting socket options for broadcasting.
	 
	 /* set up sockaddr_in */
	 unsigned int sockaddr_size_inbytes= sizeof(struct sockaddr_in);
	 client= (struct sockaddr_in *)malloc(sockaddr_size_inbytes);
	 server= (struct sockaddr_in *)malloc(sockaddr_size_inbytes);
	 server->sin_family= AF_INET;							   //using the address family of IPV4.
 	 (server->sin_addr).s_addr= htonl(INADDR_ANY);					   //INADDR_ANY Macro lets the server listen to all available IPs of the server.
	 server->sin_port= htons(BROADCAST_PORT);
	 
	 /*bind the socket with the port*/
	 if(bind(sock, (struct sockaddr *)server, sockaddr_size_inbytes)<0)
	 {
		 perror("can't bind to port");
		 exit(1);
	 }
	 
	/*verify the below three lines*/
	 server->sin_family= AF_INET;							   
 	 (server->sin_addr).s_addr= 0xffffffff;					   //setting the IP address to all 1's
	 /**/
	 
/* *
 * now fork() this program creating a child that acts as a server receiving the broadcasts and displaying it on the console.
 * The parent acts as the client that broadcasts the message to all programs on the network that happen to be listening on the 
 * on the port # BROADCAST_PORT.
 * */
if(fork())	// the code for the parent i.e the client segment of this peer.
{
	
	if(argc>1) // a command line argument has been passed to the daemon.
	{
		strcpy(text, argv[1]);
	}
	else
	{
		gethostname(text, 64);
	}
	while(1)
	{
	sendto(sock, text, sizeof(text), 0, (struct sockaddr *)server, sockaddr_size_inbytes);
		sleep(1);
	}
} 
/* End of Parent(Client) code */
/*----------------------------*/
else
{
	/* * 
	 * code for the child process i.e the server segment of this program
	 * this will receive the text and display it on the screen
	 * */
	 
	 while(1)
	 {
		 /* *
		  * receives a datagram
		  * */
		  client_len= sizeof(client);
	      recvfrom(sock, text, sizeof(text), 0, (struct sockaddr *)client, &client_len);
          display_state((char*)text);
	 }
	 /* End of Child(Server)code */
	 /* ------------------------ */
	 
	
}

	 return 0;
 }
 
 
 

 /**
  * @brief Definition of display text
  * @param text
  */
 void display_state(char *text)
 {
   #define TSIZE    50

   static char table[TSIZE][64];
   static int  entries = 0;
   int         i, found;

   /* Search the  table for an entry with a matching  text string.  If
      found, the ordinal position of the entry in the table is used to
      determing the position on the screen at which this entry will be
      displayed. The x,y values are only displayed;  they are NOT kept
      in the table. If no matching entry is found and the table is not
      full, a new entry is created.
   */

   for (i=0, found=0; i<entries; i++) {
      if (strcmp(text, table[i]) == 0) {
         found = 1;             /* Found a matching entry */
         break;
      }
   }
   if (!found) {
      if (entries == TSIZE)	/* Table is full */
         return;
      strcpy(table[entries], text);
      i = entries++;            /* Create new table entry */
   }
   /* Update the display of the i'th entry in the table */
   move(i, 1);
   printw("%16s", text);
   refresh();
}