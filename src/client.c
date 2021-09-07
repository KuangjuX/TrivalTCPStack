#include "api.h"
#include "kernel.h"
#include <string.h>


int main(int argc, char **argv) {
    // 开启仿真环境 
    startSimulation();

    tju_tcp_t* my_socket = tcp_socket();
    // printf("my_tcp state %d\n", my_socket->state);
    
    tju_sock_addr target_addr;
    target_addr.ip = inet_network("10.0.0.3");
    target_addr.port = 1234;

    tcp_connect(my_socket, target_addr);

    // sleep(3);
    // printf("send packet.\n");
    // tcp_send(my_socket, "hello world", 12);

    // char buf[2021];
    // tcp_recv(my_socket, (void*)buf, 12);
    // printf("client recv1 %s\n", buf);

    // tcp_send(my_socket, "hello tju", 10);
    // tcp_recv(my_socket, (void*)buf, 10);
    // printf("client recv2 %s\n", buf);
    // tcp_close(my_socket);
    for(int i = 10; i < 99; i++) {
        char* msg = (char*)malloc(7);
        sprintf(msg, "echo %d", i);
        printf("msg: %s.\n", msg);
        tcp_send(my_socket, msg, 7);
        sleep(1);
    }
    while(TRUE){}
    return EXIT_SUCCESS;
}
