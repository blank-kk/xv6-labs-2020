#include "kernel/types.h"
#include "user/user.h"

// 筛选质数的函数，接收一个管道作为参数
void sieve(int pleft[2]){
    // 从左邻居读取整数
    int p;
    read(pleft[0],&p,sizeof(p));
    if(p==-1){
        exit(0);
    }
    printf("prime %d\n",p);

    // 创建一个新的管道
    int pright[2];
    pipe(pright);

    if(fork()==0){          // 右邻居/新的子进程
        close(pright[1]);   // 右邻居用不到新管道的写端，关闭
        close(pleft[0]);    // 右邻居用不到旧管道的读端，关闭
        sieve(pright);      // 递归调用筛选函数
    }else{
        close(pright[0]);   // 旧的子进程进程用不到新管道的读端，关闭
        // 从左邻居接收数字
        int buf;
        while(read(pleft[0],&buf,sizeof(buf)) && buf!=-1){
            if(buf%p!=0){                           // 如果接收到的数字不是第一次接收到的数字的倍数
                write(pright[1],&buf,sizeof(buf));  // 才往往管道中给右邻居写入这个数字
            }
        }
        // 左邻居的数据接收完毕，右邻居的数据发送完毕，发送结束标志
        buf=-1;
        write(pright[1],&buf,sizeof(buf));
        wait(0);
        exit(0);
    }
}

int main(int argc,char** argv)
{
    // 创建初始管道
    int input_pipe[2];
    pipe(input_pipe);

    if(fork()==0){              // 右邻居/子进程
        close(input_pipe[1]);   // 右邻居用不到这个管道的写端，关闭写端
        sieve(input_pipe);      // 调用筛选函数
        exit(0);
    }else{
        close(input_pipe[0]);   // 父进程只会向管道中给右邻居写数据，关闭读端
        int i;
        for(i=2;i<=35;i++){
            write(input_pipe[1],&i,sizeof(i));  //向管道写入2~35的整数
        }
        // 写入结束标志
        i=-1;
        write(input_pipe[1],&i,sizeof(i));
    }
    wait(0);    // 等待子进程结束
    // 注意：这里无法等待子进程的子进程，只能等待子进程，无法等待间接子进程
    // 在sieve()中再执行wait(0),形成等待链
    exit(0);
}