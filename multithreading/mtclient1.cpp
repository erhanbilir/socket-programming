#include <iostream>
#include <thread>
#include <vector>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <cstring>

using namespace std;

int main()
{
    int clientSock = socket(AF_INET, SOCK_STREAM, 0);
    if(clientSock < 0)
    {
        cerr << "Error in socket" << endl;
        return 1;
    }

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8080);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    int connectStatus = connect(clientSock, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
    if(connectStatus < 0)
    {
        cerr << "Error in connect" << endl;
        return 1;
    }

    const char* msg = "Hello, I'm client - 1!";
    int sendStatus = send(clientSock, msg, strlen(msg), 0);
    if(sendStatus < 0)
    {
        cerr << "Error in send" << endl;
        return 1;
    }

    char buffer[1024] = {0};
    int readStatus = recv(clientSock, buffer, sizeof(buffer), 0);
    if(readStatus < 0)
    {
        cerr << "Error in receive" << endl;
        return 1;
    }
    else
    {
        cout << "[Server -> ]: " << buffer << endl;
    }
    close(clientSock);
    return 0;
}