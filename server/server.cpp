// Server program
#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include "utils.h"
#include <iostream>

#define PORT 5000
#define MAXLINE 1024


int main()
{
	int listenfd, connfd, udpfd, nready, maxfdp1;
	char buffer[MAXLINE];
	pid_t childpid;
	fd_set rset;
	ssize_t n;
	socklen_t len;
	const int on = 1;
	struct sockaddr_in cliaddr, servaddr;
	char const * message = "";
	void sig_chld(int);

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
    };
	//слушать соединения на сокете 
    if(listen(listenfd, 10)==-1){
        perror("server: listen");
    };

    //создаем UDP сокет
	udpfd = socket(AF_INET, SOCK_DGRAM, 0);
    if(udpfd == -1){
        perror("server: socket");
    }
    
    // привязка к сокету udpfd структуры addr
	if(bind(udpfd, (struct sockaddr*)&servaddr, sizeof(servaddr))== -1){
        perror("server: bind");
    };
	// очищает дескрипторы rset 
    FD_ZERO(&rset);

	// максимальное количество готовых дескрипторов.
	maxfdp1 = std::max(listenfd, udpfd) + 1;
	
    // структура для чисел и их суммы
    Numbers storage_digits;
   
    for (;;) {
        //очищает структуру 
        storage_digits.ClearStorage();
        // добавляет listenfd и udpfd в rset
		FD_SET(listenfd, &rset);
		FD_SET(udpfd, &rset);

		// select дает возможность следить за несколькими сокетами одновременно 
		/*nready = */select(maxfdp1, &rset, NULL, NULL, NULL);

        // если TCP сокет доступен для чтения
        // извлекаем первый запрос на соединение из очереди
        // ожидющих соединений,создаёт новый подключенный сокет
        // почти с такими же параметрами
		if (FD_ISSET(listenfd, &rset)) {
			len = sizeof(cliaddr);
			connfd = accept(listenfd, (struct sockaddr*)&cliaddr, &len);
			if ((childpid = fork()) == 0) {
				close(listenfd);
				bzero(buffer, sizeof(buffer));
				printf("Message From TCP client: ");
                //cчитывает данные файлового дескриптора
				if(read(connfd, buffer, sizeof(buffer))== -1){
                    perror("TCP: read");
                };
                std::cout<<buffer<<std::endl;

                // парсит данные buffer
                // заполняет структуру storage_digits
                NumbersFromString(buffer,storage_digits);

                // отправляем числа и их сумму  
                if(write(connfd,storage_digits.digits.c_str(), sizeof(buffer))== -1){
                    perror("TCP: write()");
                };
			    if(write(connfd,storage_digits.digitsSum.c_str(), sizeof(buffer))== -1){
                    perror("TCP: write()");
                };
                //закрываем сокет 
                close(connfd);
				exit(0);
			}
			close(connfd);
		}
        // если UDP сокет доступен для чтения,получаем сообщения
		if (FD_ISSET(udpfd, &rset)) {
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
            };
            if(sendto(udpfd, storage_digits.digitsSum.c_str(), sizeof(buffer), 0,
				(struct sockaddr*)&cliaddr, sizeof(cliaddr))== -1){
                perror("UDP: sendto()");
            };
        }
	}
}

