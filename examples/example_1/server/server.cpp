#include "server.h"

// method

void Server::Initialization()
{           
        //создаем TCP сокет
    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    if(listenfd == -1){
        perror("server: socket");
    }

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(PORT);

    // привязка к сокету listenfd структуры addr
    if(bind(listenfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) == -1){
        perror("server: bind");
    }
    //слушать соединения на сокете 
    if(listen(listenfd, 10)==-1){
        perror("server: listen");
    }

    //создаем UDP сокет
    udpfd = socket(AF_INET, SOCK_DGRAM, 0);
    if(udpfd == -1){
        perror("server: socket");
    }

    // привязка к сокету udpfd структуры addr
    if(bind(udpfd, (struct sockaddr*)&servaddr, sizeof(servaddr))== -1){
        perror("server: bind");
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
        Numbers storage_digits;
        this->len = sizeof(this->cliaddr);
        connfd = accept(listenfd, (struct sockaddr*)&cliaddr, &len);
        if ((childpid = fork()) == 0) {
            close(listenfd);
            bzero(buffer, sizeof(buffer));
            printf("Message From TCP client: ");
            //cчитывает данные файлового дескриптора
            if(read(connfd, buffer, sizeof(buffer))== -1){
                perror("TCP: read");
            }
            std::cout<<buffer<<std::endl;

            // парсит данные buffer
            // заполняет структуру storage_digits
            NumbersFromString(this->buffer,storage_digits);

            // отправляем числа и их сумму  
            if(write(connfd,storage_digits.digits.c_str(), sizeof(buffer))== -1){
                perror("TCP: write()");
            }
            if(write(connfd,storage_digits.digitsSum.c_str(), sizeof(buffer))== -1){
                perror("TCP: write()");
            }
            //закрываем сокет 
            Termination(connfd);//close(connfd);
            exit(0);
        }
    Termination(connfd);//close(connfd);    
}

void Server::DataTransfer_UDP(){
        Numbers storage_digits;	
        len = sizeof(cliaddr);
			bzero(buffer, sizeof(buffer));
			printf("\nMessage from UDP client: ");
            //получаем данные с udpfd и записываем их в buffer
			n = recvfrom(udpfd, buffer, sizeof(buffer), 0,
						(struct sockaddr*)&cliaddr, &len);
            if(n == -1){
                perror("UDP: recvfrom");
            }
            std::cout<<buffer<<std::endl;//puts(buffer);
            // парсит данные buffer
            // заполняет структуру storage_digits
            NumbersFromString(buffer,storage_digits);
            
            //отправляем числа и их сумму
            if(sendto(udpfd, storage_digits.digits.c_str(), sizeof(buffer), 0,
				(struct sockaddr*)&cliaddr, sizeof(cliaddr))== -1){
                perror("UDP: sendto()");
            }
            if(sendto(udpfd, storage_digits.digitsSum.c_str(), sizeof(buffer), 0,
				(struct sockaddr*)&cliaddr, sizeof(cliaddr))== -1){
                perror("UDP: sendto()");
            }
}

void Server::Termination(int fd){
    close(fd);
}


