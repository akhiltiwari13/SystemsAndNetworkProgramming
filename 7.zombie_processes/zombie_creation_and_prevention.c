#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
    if(fork()) {
	signal(SIGCHLD, SIG_IGN); // this line explicitly ignores SIGCHLD signal thus saving child from becoming zombie.
	puts("parent");
	sleep(15);
    } else {
	puts("child");
	exit(0);
    }
    return 0;
}