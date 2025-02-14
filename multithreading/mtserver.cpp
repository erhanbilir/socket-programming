#include <iostream>
#include <thread>
#include <vector>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <cstring>

using namespace std;

vector<thread> clientThreads;

void handleClient(int clientSock)
{
    char buffer[1024] = {0};

    while(true)
    {
        memset(buffer, 0, sizeof(buffer));
        int readStatus = recv(clientSock, buffer, sizeof(buffer), 0);
        if(readStatus <= 0)
        {
            cerr << "Client disconnected" << endl;
            break;
        }
        else
        {
            cout << "[Client -> ]: " << buffer << endl;
            const char* msg = "Hello, I'm server. I got your message!";
            int sendStatus = send(clientSock, msg, strlen(msg), 0);
            if(sendStatus < 0)
            {
                cerr << "Error in send" << endl;
            }
        }
    }
    close(clientSock);
}

int main()
{
    int serverSock = socket(AF_INET, SOCK_STREAM, 0);
    if(serverSock < 0)
    {
        cerr << "Error in socket" << endl;
        return 1;
    }

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8080);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    int bindStatus = bind(serverSock, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
    if(bindStatus < 0)
    {
        cerr << "Error in bind" << endl;
        return 1;
    }

    int listenStatus = listen(serverSock, 5);
    if(listenStatus < 0)
    {
        cerr << "Error in listen" << endl;
        return 1;
    }

    cout << "Server is listening..." << endl;

    while(true)
    {
        sockaddr_in clientAddr;
        socklen_t clientAddrSize = sizeof(clientAddr);

        int clientSock = accept(serverSock, (struct sockaddr*)&clientAddr, &clientAddrSize);
        if(clientSock < 0)
        {
            cerr << "Error in accept" << endl;
            continue;
        }

        cout << "Client connected!" << endl;

        clientThreads.push_back(thread(handleClient, clientSock));
    }
    close(serverSock);

    return 0;
}