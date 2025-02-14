#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <cstring>

using namespace std;

int main()
{
    int udpSock = socket(AF_INET, SOCK_DGRAM, 0);
    if (udpSock< 0)
    {
        cerr << "Error in socket" << endl;
        return 1;
    }

    sockaddr_in serverAddr, clientAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8080);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    int bindStatus = bind(udpSock, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
    if( bindStatus < 0)
    {
        cerr << "Error in bind" << endl;
        return 1;
    }

    cout << "[+]: UDP Server dinliyor..." << endl;

    while(true)
    {
        char buffer[1024] = {0};
        socklen_t clientAddrSize = sizeof(clientAddr);

        int readStatus = recvfrom(udpSock, buffer, sizeof(buffer), 0, (struct sockaddr*)&clientAddr, &clientAddrSize);
        if( readStatus < 0 )
        {
            cerr << "Error in receive" << endl;
            continue;
        }

        cout << "[Client -> ]: " << buffer << endl;
        const char* msg = "Merhaba, ben server. Mesajini aldim!";
        int sendStatus = sendto(udpSock, msg, strlen(msg), 0, (struct sockaddr*)&clientAddr, clientAddrSize);
        if( sendStatus < 0 )
        {
            cerr << "Error in send" << endl;
            return 1;
        }
        else
        {
            cout << "[+]: " << msg << endl;
        }

        
    }
    close(udpSock);
    return 0;
}