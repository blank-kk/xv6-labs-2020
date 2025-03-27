#include "kernel/types.h"
#include "user.h"
 
int main(int argc,char* argv[]){
    //创建两个管道，分别实现ping、pong的读写
    int p_p2c[2];   // 0读 1写
    int p_c2p[2];
    pipe(p_p2c);
    pipe(p_c2p);

    int pid = fork();
    if(pid!=0){   //父进程
        write(p_p2c[1],"*",1);
        close(p_p2c[1]);
        char buf;
        read(p_c2p[0],&buf,1);
        printf("%d:recieved pong\n",getpid());
        wait(0);
    }else{ //子进程
        char buf;
        read(p_p2c[0],&buf,1);
        printf("%d:received ping\n",getpid());
        write(p_c2p[1],&buf,1);
        close(p_c2p[1]);
    }
    close(p_c2p[0]);
    close(p_p2c[0]);
    exit(0);
}