#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

using namespace std;

int main()
{
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0)
    {
        cerr << "Error in socket" << endl;
        return 1;
    }

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8080);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    char buff[1024] = {0};

    int bindStatus = bind(sock, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
    if (bindStatus < 0)
    {
        cerr << "Error in bind" << endl;
        return 1;
    }

    int listenStatus = listen(sock, 5);
    if (listenStatus < 0)
    {
        cerr << "Error in listen" << endl;
        return 1;
    }

    while(true)
    {
        cout << "Client bekleniyor..." << endl;
        sockaddr_in clientAddr;
        socklen_t clientAddrSize = sizeof(clientAddr);
        int clientSock = accept(sock, (struct sockaddr*)&clientAddr, &clientAddrSize);
        if (clientSock < 0)
        {
            cerr << "Error in accept" << endl;
            continue;
        }
    
        int readStatus = recv(clientSock, buff, sizeof(buff), 0);
        if (readStatus < 0)
        {
            cerr << "Error in recv" << endl;
        }
        else
        {
            cout << "Client: " << buff << endl;
        
            const char* msg = "Merhaba, ben server. Mesajini aldim!";
            int sendStatus = send(clientSock, msg, strlen(msg), 0);
            if (sendStatus < 0)
            {
                cerr << "Error in send" << endl;
                return 1;
            }
        }
        close(clientSock);
    }

    close(sock);

    return 0;
}