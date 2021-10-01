#include "server.h"

int main()
{
    Server server;
    server.Initialization();
    for(;;){
    server.TraceSockets();//
    if(FD_ISSET(server.GetListenfd(),server.GetSetfd())){
        server.Connection_TCP();
        }
    
    if(FD_ISSET(server.GetUdpfd(),server.GetSetfd())){
        server.Connection_TCP();
        }
    }
        return 0;
}
