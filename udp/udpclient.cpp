#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <cstring>

using namespace std;

int main()
{
    int udpSock = socket(AF_INET, SOCK_DGRAM, 0);
    if ( udpSock < 0 )
    {
        cerr << "Error in socket" << endl;
        return 1;
    }

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8080);
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    
    socklen_t serverAddrSize = sizeof(serverAddr);

    const char* msg = "Merhaba, ben client!";
    int sendStatus = sendto(udpSock, msg, strlen(msg), 0, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
    if( sendStatus < 0 )
    {
        cerr << "Error in send" << endl;
        return 1;
    }
    else
    {
        cout << "[+]: " << msg << endl;
    }

    char buffer[1024] = {0};
    int readStatus = recvfrom(udpSock, buffer, sizeof(buffer), 0, (struct sockaddr*)&serverAddr, &serverAddrSize);
    if( readStatus < 0 )
    {
        cerr << "Error in receive" << endl;
        return 1;
    }
    else
    {
        cout << "[Server -> ]: " << buffer << endl;
    }
    close(udpSock);
    return 0;
}