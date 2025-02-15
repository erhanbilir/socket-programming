#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <cstring>
#include <fstream>

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

    fstream file = fstream("file.txt", ios::in | ios::binary);
    if(!file.is_open())
    {
        cerr << "Error while opening file" << endl;
        return 1;
    }
    char buffer[1024] = {0};
    char recvBuffer[1024] = {0};
    long long int byteCount = 0;
    while(file.read(buffer, sizeof(buffer)))
    {
        int sendStatus = send(clientSock, buffer, sizeof(buffer), 0);
        if(sendStatus < 0)
        {
            cerr << "Error in send" << endl;
            return 1;
        }
        byteCount += sizeof(buffer);
        memset(buffer, 0, sizeof(buffer));
    }

    cout << "File transfer completed" << endl;

    file.close();
    close(clientSock);
    return 0;
}