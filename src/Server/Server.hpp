#pragma once

#include <string>
#include <iostream>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <utility>
#include <algorithm>
#include <sstream>
#include <sys/un.h>
#include <thread>
#include <vector>
#include "../LongBufUnion/Union.hpp"

#define BUF_SIZE 1024
#define byte unsigned char

template <class Handler>
class Server{
private:
    int                 port;
    int                 connection_limit;
    int                 listener;
    bool                isActive;
public:
    Server(int port, int connection_limit){
        std::cout<<"Creating server instance"<<std::endl;
        this->port=port;
        this->connection_limit=connection_limit;
    }
    ~Server(){
        std::cout << "Destructing server instance" << std::endl;
        if (this->listener != 0)
            this->Stop();
        }
        void Start()
        {
            std::cout << "Starting server" << std::endl;

            this->listener = socket(AF_INET, SOCK_STREAM, 0);
            if (listener < 0)
            {
                std::cerr << "Error while socket initialization" << std::endl;
                exit(1);
            }
            std::cout << "Socket successfully initialized" << std::endl;

            struct sockaddr_in addr;
            std::memset(&addr, 0, sizeof(addr));
            addr.sin_family = AF_INET;
            addr.sin_addr.s_addr = INADDR_ANY;
            addr.sin_port = htons(this->port);

            if (bind(this->listener, (struct sockaddr *)&addr, sizeof(struct sockaddr_in)))
            {
                std::cerr << "Error on binding stream socket" << std::endl;
                exit(1);
            }

            std::cout << "Socket was successfully binded with port " << this->port << std::endl;

            listen(this->listener, this->connection_limit);
            isActive = true;

            for (;;){
                auto msgsock=accept(this->listener,0,0);
                if (msgsock<0){
                    std::cerr<<"Error on accept"<<std::endl;
                    if (!isActive) break;  
                }
                else
                {
                    std::thread T([](int client) {
                        unsigned long size;
                        int len = recv(client, &size, sizeof(size), MSG_NOSIGNAL);
                        if (len > 0)
                        {
                            byte buf[size];
                            len = recv(client, buf, size, MSG_NOSIGNAL);
                            if (len > 0)
                            {
                                std::vector<byte> Request;
                                for (unsigned int i = 0; i < size; ++i)
                                    Request.push_back(buf[i]);

                                Handler H;
                                std::vector<byte> response = H.handle(Request);
                                if (response.size() > 0)
                                {
                                    LongBuf S;
                                    S.Id = response.size();
                                    send(client, S.Buf, 8, MSG_NOSIGNAL);
                                    send(client, response.data(), response.size(), MSG_NOSIGNAL);
                                }
                            }
                        }

                        close(client);
                        
                    },
                                  msgsock);
                    T.detach();
                }
            }
        }
    void Stop()
    {
        std::cout << "Closing socket" << std::endl;
        shutdown(this->listener, SHUT_RDWR);
        close(this->listener);
    }
};