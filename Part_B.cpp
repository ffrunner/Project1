#include <iostream>
#include <unistd.h>  
#include <stdlib.h> 
#include <sys/types.h>
#include <sys/wait.h>

void producer(int writeEnd) {
	for (int i = 0; i < 10; ++i){
	    std::cout << "Producer produced: " << i << std::endl;
	    write(writeEnd, &i, sizeof(i));
	    sleep(1);
	}
	close(writeEnd);
}

void consumer(int rearEnd) {
     int item;
     while (read(rearEnd, &item, sizeof(item)) > 0) {
         std::cout << "Consumer consumed: " << item << std::endl;
     }
     close(rearEnd);
}

int main() {
	int pipefd[2];
	if (pipe(pipefd) == -1) {
	   std::cerr << "Pipe failed" << std::endl;
	   exit(1);
	}

	pid_t pid = fork();
	if (pid < 0) {
	   std::cerr << "Fork failed" << std::endl;
	   exit(1);
	}

	if (pid == 0) {
	    close(pipefd[1]);
	    consumer(pipefd[0]);
	} else {
	    close(pipefd[0]);
	    producer(pipefd[1]);
	    wait(NULL);
	}

	return 0;
}
