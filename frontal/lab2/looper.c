#include <stdio.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <signal.h>
#include <string.h>

char logMsg[256];
void logger(char *msg);

void handler(int sig)
{
	snprintf(logMsg, sizeof(logMsg), "Received Signal: %s", strsignal(sig));
	logger(logMsg);

    if (sig == SIGTSTP) {
        signal(SIGTSTP, SIG_DFL);  
        raise(SIGTSTP);            
    
    } else if (sig == SIGCONT) {
		signal(SIGCONT, SIG_DFL);
		raise(SIGCONT);

    } else if (sig == SIGINT) {
        signal(SIGINT, SIG_DFL);   
        raise(SIGINT);            
    }
}

int main(int argc, char **argv)
{
    logger("Starting the program\n");
	fflush(stdout);

    while (1) {
        sleep(1);
		signal(SIGINT, handler);
		signal(SIGTSTP, handler);
		signal(SIGCONT, handler);
    }

    return 0;
}
void logger(char *msg){
	printf("%s\n", msg);
	fflush(stdout);
}