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

#include <zmq.hpp>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

int main (void)
{
    zmq::context_t context(1);
    const char * protocol =
            "tcp://localhost:5555";
    printf ("Connecting to hello world server...");
    zmq::socket_t sock (context, ZMQ_SUB);
    sock.connect(protocol);
    sock.setsockopt (ZMQ_SUBSCRIBE, "", 0);
    printf ("done. \n");

    while(true){

        zmq::message_t reply;
        sock.recv (&reply, 0);
        printf ("Received Word %d bytes: \"%s\"\n", reply.size(), reply.data());
    }
    sock.close();
    return 0;
}