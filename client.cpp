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



int main(){
    char * data;
    zmq::context_t zmq_context(1);
    zmq::socket_t zmq_socket(zmq_context, ZMQ_SUB);
    zmq_socket.connect("tcp://127.0.0.1:5555");
    zmq_msg_t msg;
    int rc = zmq_msg_init (&msg);
    assert (rc == 0);
/* Block until a message is available to be received from socket */
    rc = zmq_socket.recv(&msg, 255);
    assert (rc == 0);
/* Release message */ zmq_msg_close (&msg);
    data = (char *) zmq_msg_data(&msg);
    std::cout << data;

    return 0;
}
