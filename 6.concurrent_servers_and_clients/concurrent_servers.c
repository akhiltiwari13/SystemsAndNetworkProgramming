#include <arpa/inet.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

void set_socket_address_for_tcpserver(struct sockaddr_in* server_socket_address)
{
    server_socket_address->sin_family = AF_INET;
    server_socket_address->sin_port = htons(1234);
    (server_socket_address->sin_addr).s_addr = INADDR_ANY;
}

extern time_t time();

char* word[] = {
#include "words"
};

#define NUM_OF_WORDS (sizeof(word) / sizeof(word[0]))
#define MAXLEN 80 /* Maximum size of any string in the world */
#define HANGMAN_TCP_PORT 1068

void play_hangman(int in, int out);

int main()
{
    int listner_socket, accept_socket, reuse = 1 /*for reusing the  sticky ports*/;
    struct sockaddr_in* server_socket_address;
    struct sockaddr_storage* client_socket_address;

    /*initialize a socket descriptor with error check*/
    if((listner_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
	perror("can't open a socket");
    }

    /* set up the socket address */
    server_socket_address = (struct sockaddr_in*)malloc(sizeof(struct sockaddr_in));
    set_socket_address_for_tcpserver(server_socket_address);

    /*setting the reuse option on the port*/
    if(setsockopt(listner_socket, SOL_SOCKET, SO_REUSEADDR, (char*)&reuse, sizeof(int)) < 0) {
	perror("can't set the reuse option on the socket");
    }

    /* Binding server socket to a port with an error check */
    if(bind(listner_socket, (struct sockaddr*)server_socket_address, sizeof(struct sockaddr_in)) < 0) {
	perror("can't bind to a port");
    }

    /* listen on the bound port with error check*/
    if(listen(listner_socket, 10) < 0) {
	perror("can't listen");
    }

    /* main server logic for accepting client requests */
    client_socket_address = (struct sockaddr_storage*)malloc(sizeof(struct sockaddr_storage));
    unsigned int client_address_size = sizeof(client_socket_address);
    while(1) {
	if((accept_socket = accept(listner_socket, (struct sockaddr*)client_socket_address, &client_address_size)) <
	    0) {
	    perror("can't open the acceptance socket");
	} else {
	    /*parent code*/
	    if(fork()) {
		close(accept_socket);     // closing the established socket descriptor for the parent.
		signal(SIGCHLD, SIG_IGN); /*for ignoring SIGCHILD*/

	    }
	    /*child's code */
	    else {
		close(listner_socket); // child does not need the listner socket.
		char* msg = "This is a test message from the child process";
		send(accept_socket, msg, strlen(msg), 0);
		printf("new child (pid %d) using descriptor %d\n", getpid(), accept_socket);
		srand((int)time((long*)0)); /* Randomise the seed */
		play_hangman(accept_socket, accept_socket);
		printf("child (pid %d) exiting\n", getpid());
		close(accept_socket); // once the child is done sending the message it should close.
		exit(0);              // exit the child process after sending the response.
	    }
	}
    }

    return 0;
}

/* ------------------ play_hangman() ------------------------ */
/* Plays one game of hangman,  returning when the  word has been
   guessed or all the player's "lives" have been used.  For each
   "turn"  of the game,  a line is read  from  stream "in".  The
   first character of this line is  taken as the player's guess.
   After each guess and prior to the first guess, a line is sent
   to stream "out". This consists of the word as guessed so far,
   with - to show unguessed letters,   followed by the number of
   lives remaining.
   Note that this function neither knows nor cares  whether its
   input and output streams refer to sockets, devices, or files.
*/
void play_hangman(int in, int out)
{
    char *whole_word, part_word[MAXLEN], guess[MAXLEN], outbuf[MAXLEN];
    int lives = 12;       /* Number of lives left */
    int game_state = 'I'; /* I ==> Incomplete     */
    int i, good_guess, word_length;

    /* Pick a word at random from the list */
    whole_word = word[rand() % NUM_OF_WORDS];
    word_length = strlen(whole_word);

    /* No letters are guessed initially */
    for(i = 0; i < word_length; i++)
	part_word[i] = '-';
    part_word[i] = '\0';

    sprintf(outbuf, " %s   %d\n", part_word, lives);
    write(out, outbuf, strlen(outbuf));

    while(game_state == 'I') {
	read(in, guess, MAXLEN); /* Get guess letter from player */
	good_guess = 0;
	for(i = 0; i < word_length; i++) {
	    if(guess[0] == whole_word[i]) {
		good_guess = 1;
		part_word[i] = whole_word[i];
	    }
	}
	if(!good_guess)
	    lives--;
	if(strcmp(whole_word, part_word) == 0)
	    game_state = 'W'; /* W ==> User Won */
	else if(lives == 0) {
	    game_state = 'L';              /* L ==> User Lost */
	    strcpy(part_word, whole_word); /* Show User the word */
	}
	sprintf(outbuf, " %s   %d\n", part_word, lives);
	write(out, outbuf, strlen(outbuf));
    }
}