#include <sys/types.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <stdio.h>
#include <string.h>
#include <iostream>

#define PORT 5000

class Server
{
public:
    
    Server() = default;
    ~Server() = default;
    Server(const Server& s) = delete;// no copies

    void Initialization();
    void TraceSockets();
    void Connection_TCP();
    void DataTransfer_UDP();
    void Termination(int fd);
    void DataTransfer(); 

    int GetListenfd(){return listenfd;}
    int GetUdpfd(){return udpfd;} 
    fd_set* GetSetfd(){return &rset;}
    
private:
    
    int listenfd,connfd,udpfd,nread,maxfdp1;
    char buffer[1024];
    pid_t childpid;
    fd_set rset;
    ssize_t n;
    socklen_t len;
    struct sockaddr_in cliaddr,servaddr;
    char const *message = "";
    void sig_chld(int);
};



