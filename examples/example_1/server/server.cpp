#include "server.h"
#include <string>
//#include <netdb.h>

void Server::Initialization()
{           
    memset(&servaddr,0,sizeof servaddr);
    servaddr.ai_family = AF_UNSPEC;
    servaddr.ai_socktype = 0;
    servaddr.ai_flags = AI_PASSIVE;
    
    getaddrinfo(NULL,"5000",&servaddr,&res);

    listenfd = socket(res->ai_family,res->ai_socktype,0);
    udpfd = socket(res->ai_family,SOCK_DGRAM,0);

    bind(listenfd,res->ai_addr,res->ai_addrlen);  
    bind(udpfd,res->ai_addr,res->ai_addrlen); 
    //bind(udpfd,res->ai_addr,res->ai_addrlen); 
    //if(bind(udpfd, (struct sockaddr*)&servaddr, sizeof(servaddr))== -1){
    //    perror("server: bind");
    //}

    //слушать соединения на сокете 
    if(listen(listenfd, 10)==-1){
        perror("server: listen");
    }
    // очищает дескрипторы rset 
    FD_ZERO(&rset);

    // максимальное количество готовых дескрипторов.
    maxfdp1 = std::max(listenfd, udpfd) + 1;

}

void Server::TraceSockets(){  
    FD_SET(listenfd, &rset);
    FD_SET(udpfd, &rset);
    // select дает возможность следить за несколькими сокетами одновременно 
    /*nready = */select(maxfdp1, &rset, NULL, NULL, NULL);
}

void Server::Connection_TCP(){
        this->len = sizeof(this->cliaddr);
        connfd = accept(listenfd, (struct sockaddr*)&cliaddr, &len);
        if ((childpid = fork()) == 0) {
            close(listenfd);
            bzero(buffer, sizeof(buffer));
            std::cout<<"\nMessage From TCP client: ";
            //cчитывает данные файлового дескриптора
            if(read(connfd, buffer, sizeof(buffer))== -1){
                perror("TCP: read");
            }
            std::cout<<buffer<<std::endl;
            
            //*************************************
            // Здесь можно добавить отправку данных
            // Используя DataTransfer_TCP
            // Как в примере example_1
            //*************************************

            Termination(connfd);
            exit(0);
        }
    Termination(connfd);    
}

void Server::DataTransfer_TCP(){
    //********************************************
    // Пока что message = ""
    // Возможно этот метод будет virtual
    //********************************************

    if(write(connfd,message, sizeof(buffer))== -1){
                perror("TCP: write()");
            }
    
}

void Server::DataTransfer_UDP(){
        this->len = sizeof(cliaddr);
			bzero(buffer, sizeof(buffer));
            std::cout<<"\nMessage from UDP client: ";
            //получаем данные с udpfd и записываем их в buffer
			n = recvfrom(udpfd, buffer, sizeof(buffer), 0,
						(struct sockaddr*)&cliaddr, &len);
            if(n == -1){
                perror("UDP: recvfrom");
            }

            if(sendto(udpfd, buffer, sizeof(buffer), 0,
			  (struct sockaddr*)&cliaddr, sizeof(cliaddr))== -1){
                perror("UDP: sendto()");
            }
            std::cout<<buffer<<std::endl;
}

void Server::Termination(int fd){
    close(fd);
}


