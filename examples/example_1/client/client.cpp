// UDP client program
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <iostream>
#include <algorithm>
#include <cctype>

#define PORT 5000
#define MAXLINE 1024


int main(int argc,char*argv[])
{
    std::string setting_server = "udp";
    if(argc>=2){    
        setting_server = argv[1];
        std::transform(setting_server.begin(), setting_server.end(), setting_server.begin(),
            [](unsigned char c){ return std::tolower(c); });
        
        if(setting_server=="tcp"){
            setting_server="tcp";
            std::cout<<"client connection over TCP"<<std::endl;
        }
        else if(setting_server=="udp"){
            setting_server="udp";
            std::cout<<"client connection over UDP"<<std::endl;
            }
        else{
            setting_server = "udp";
            std::cout<<"client connection over UDP(Default).No valid parameter: "<<argv[1]<<std::endl;}
            
    }
    else {
        std::cout<<"client connection over UDP(Default)"<<std::endl;
    }

    int sockfd;
    char buffer[MAXLINE];
    std::string tmp_message = "";

    //-----------------UDP-------------------
    if(setting_server=="udp"){ 
        std::cout<<"Write message: ";
    
        getline(std::cin,tmp_message);
	    const char* message = tmp_message.c_str();
	    struct sockaddr_in servaddr;

	    int n, len;
	    // создаем сокет UDP
        if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		    printf("socket creation failed");
		    exit(0);
	    }

	    memset(&servaddr, 0, sizeof(servaddr));

	    // заполняем информации о сервере
        servaddr.sin_family = AF_INET;
	    servaddr.sin_port = htons(PORT);
	    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	    // отправляем введенную пользователем строку
	    sendto(sockfd, (const char*)message, strlen(message),
		    0, (const struct sockaddr*)&servaddr,sizeof(servaddr));

	    /* получаем ответ от сервера и выводим его */
        socklen_t x = sizeof(servaddr);
	    memset(buffer,0,sizeof buffer);
        n = recvfrom(sockfd, (char*)buffer, 1024,0,(struct sockaddr*)&servaddr,&x);
        std::cout<<buffer<<std::endl;
        memset(buffer,0,sizeof buffer);
        n = recvfrom(sockfd, (char*)buffer, 1024,0,(struct sockaddr*)&servaddr,&x);
        std::cout<<buffer<<std::endl;
        close(sockfd);
    }

    //-------------------TCP------------------
    if (setting_server=="tcp") {
        std::cout<<"Write message: ";
    
        getline(std::cin,tmp_message);

        const char* message = tmp_message.c_str();
	    struct sockaddr_in servaddr;

	    int n, len;
	    // создаем TCP сокет
	    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		    printf("socket creation failed");
		    exit(0);
	    }

	    memset(&servaddr, 0, sizeof(servaddr));

        // заполняем информацию о сервер
        servaddr.sin_family = AF_INET;
	    servaddr.sin_port = htons(PORT);
	    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");

        // подключаем сокет на который указывает 
        // файловый дескриптор sockfd
        // c адресом указанным в servaddr
	    if (connect(sockfd, (struct sockaddr*)&servaddr,sizeof(servaddr)) < 0) {
		    printf("\n Error : Connect Failed \n");
	    }
        /* получаем ответ от сервера и выводим его */
	    memset(buffer, 0, sizeof(buffer));
	    strcpy(buffer, message);
	    write(sockfd, buffer, sizeof(buffer));
	    read(sockfd, buffer, 1024/*sizeof(buffer)*/);
        std::cout<<buffer<<std::endl;
        memset(buffer,0,sizeof(buffer));
        read(sockfd, buffer, sizeof(buffer));
        std::cout<<buffer<<std::endl;
        close(sockfd);
    }
	return 0;
}

