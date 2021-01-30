//
// Created by perturabo on 28.01.2021.
//

#include <zmq.hpp>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include "common.hpp"

bool student_cmp(const student& a, const student& b){
    return a.fullName < b.fullName;
}

int main (void)
{
    zmq::context_t context(1);
    const char * protocol =
            "tcp://localhost:5555";
    printf ("Connecting to server...");
    zmq::socket_t sock (context, ZMQ_SUB);
    sock.connect(protocol);
    sock.setsockopt (ZMQ_SUBSCRIBE, "", 0);
    printf ("done. \n");
    std::string recvdData;
    std::string line;
    student_list finalStudentList;
    student buf;

    while(true){
        zmq::message_t reply;
        sock.recv (&reply, 0);
        if(VERBOSE){
            printf("Received Word %d bytes: \"%s\"\n", reply.size(), reply.data());
        }        recvdData = reply.to_string();

        if(recvdData[0] == '^'){
            printf("Session ended, disconnecting from server...");
            break;
        }
        if(recvdData[0] == '%'){
           if(!line.empty()){

               parse_string(buf, line);
               finalStudentList.push_back(buf);
           }
            line.clear();
        } else {
            recvdData.erase(std::find(recvdData.begin(), recvdData.end(), '\0'), recvdData.end());
            line.erase(std::find(line.begin(), line.end(), '\0'), line.end());

            line += recvdData;
        }
    }
    sock.close();
    printf ("done. \n\n");
    sort(finalStudentList.begin(), finalStudentList.end(), student_cmp);

    for(auto item : finalStudentList)
        printf("%2i %20s   %10s\n", item.id, item.fullName.c_str(), item.dateOfBirth.c_str());

    return 0;
}
