#define   __LIBRARY__
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>


_syscall1(void*,shmat,int,shmid);
_syscall1(int,shmget,char*,name);


int main() {
    int  i, shmid;
    int *p;
    
	shmid = shmget("buffer");
    p = (int*) shmat(shmid);
	
    for(i = 0 ; i < 10; i++) {
        p[i] = i;
    }
    
	while(1);
    return 0;
}