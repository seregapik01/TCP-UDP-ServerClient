#include "server.h"

int main()
{
    Server server;
    server.Initialization();
    for(;;){
        server.TraceSockets();
        //TCP
        if(FD_ISSET(server.GetListenfd(),server.GetSetfd())){
            server.Connection_TCP();
        }
        //UDP
        if(FD_ISSET(server.GetUdpfd(),server.GetSetfd())){
            server.DataTransfer_UDP();
        }
    }
        return 0;
}
