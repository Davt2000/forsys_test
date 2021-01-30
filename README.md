# forsys_test
Data folder for server should be explicitly given as argument (default is just "data" in the same folder the server is).

Client build

`g++ -std=c++2a client.cpp common.hpp -lzmq -o client`

Server build

`g++ -std=c++2a server.cpp common.hpp -lzmq -o server`
