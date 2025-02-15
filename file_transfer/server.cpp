#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <cstring>
#include <string>
#include <fstream>

using namespace std;

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
    
    fstream file;


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
        fstream file = fstream("received.txt", ios::out | ios::binary | ios::app);
        if(!file.is_open())
        {
            cerr << "Error while opening file" << endl;
            return 1;
        }
        else
        {
            cout << "File opened successfully" << endl;
            char buffer[1024] = {0};
            int bytesRead = 0;
            while((bytesRead = recv(clientSock, buffer, sizeof(buffer), 0) > 0))
            {
                //file.write(buffer, bytesRead);
                file << buffer;
                //cout << "[+] Number of bytes received: " << bytesRead << endl;
                memset(buffer, 0, sizeof(buffer));
            }

            if(bytesRead == 0)
            {
                cout << "File transfer completed" << endl;
            }
            else if(bytesRead < 0)
            {
                cerr << "File transfer failed" << endl;
            }
            file.close();
        }
        close(clientSock);
    }
    close(serverSock);

    return 0;
}