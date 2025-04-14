#include "kernel/types.h"
#include "user/user.h"
int main(){
    //exec 函数需要一个明确的标志来确定参数数组的结束位置
    char* argv[]= {"echo","this","is","echo",0};//必须要有这个空指针
    exec("echo", argv);
    printf("exec failed!\n");
    exit(0);
}
