//
// Created by perturabo on 28.01.2021.
//

//Действия клиента (sub):
// - Принять все полученные данные от сервера по протоколу ZeroMQ sub
// - Отсортировать студентов по ФИО
// - Отобразить отсортированный список студентов на экране
//
//* (задание под звездочкой)
// - Запустить сервер в докер контейнере
// - Запустить клиент в докер контейнере
// - Результат работы сервера сохранить на жесткий диск хостовой машины в виде файла
//
//Дополнительное
//

#include <string>
#include <iostream>
#include <zmq.hpp>
#include <zmq.hpp>
#include <unistd.h>

int main (void)
{
    zmq::context_t context(1);
    const char * protocol =
            "tcp://localhost:5555";
    //  Socket to talk to server
    printf ("Connecting to hello world server...");
    zmq::socket_t sock (context, ZMQ_SUB);
    sock.connect(protocol);
    sock.setsockopt (ZMQ_SUBSCRIBE, "", 0);
    printf ("done. \n");

    zmq::message_t reply;
    sock.recv (&reply, 0);
    std::string receivedData;
    receivedData = reply.to_string();
    printf ("Received Word %d bytes: \"%s\"\n", reply.size(), receivedData.c_str());
    printf ("Proceeding to main loop\n");
    while(true){

        zmq::message_t reply;
        sock.recv (&reply, 0);
        printf ("Received Word %d bytes: \"%s\"\n", reply.size(), receivedData.c_str());
    }
    sock.close();
    return 0;
}
