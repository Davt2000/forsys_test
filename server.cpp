//
// Created by perturabo on 28.01.2021.
//

#include <fstream>
#include <iostream>
#include <zmq.hpp>
#include <unistd.h>
#include "common.hpp"

int scan_data_folder(const std::string& dataFolder, string_list &output){
    for (const auto & entry : fs::directory_iterator(dataFolder))
        if(entry.is_regular_file())
            output.push_back(entry.path());
}



int prepare_to_send(string_list &pcs, std::string &s){
    std::string s_copy = s;
    int pivot = 14;
    pcs.push_back("%");     // delimiter, which will be useful in combining string back by client
                            // entries are divided by '%', sessions by '^'
    while(!s_copy.empty()){
        if(s_copy.size() >= pivot) {
            pcs.push_back(s_copy.substr(0, pivot));
            s_copy = s_copy.substr(pivot);
        } else{
            pcs.push_back(s_copy);
            s_copy = "";
        }
    }
    return 0;
}

int read_data(const std::string& filename, student_list& studentList){
    std::string line;
    std::fstream file (filename);
    student buf;

    if (file.is_open()){
        while (getline (file,line)){
            parse_string(buf, line);
            studentList.push_back(buf);
        }
        file.close();
    }
    else
        exit(-2);
}

int collapse_similar(student_list& output, student_list& input){
    int f;
    for(const auto& i : input){
        f = 0;
        for(const auto& j : output){
            if((i.dateOfBirth == j.dateOfBirth) && (i.fullName == j.fullName)){
                f = 1;
                break;
            }
        }
        if(!f){
            output.push_back(i);
        }

    }
}

void concat_back(char * str, student& st){
    std::string s = std::to_string(st.id);
    char const *pchar = s.c_str();
    memset(str, 0, 255);
    strcat(str, pchar);
    strcat(str, " ");
    strcat(str, st.fullName.c_str());
    strcat(str, " ");
    strcat(str, st.dateOfBirth.c_str());
}

[[noreturn]] void zmq_server_start_broadcast(student_list& output) {
    char buf[255];
    zmq::context_t context(1);
    zmq::socket_t socket(context, ZMQ_PUB);
    socket.bind("tcp://*:5555");
    string_list dataToBreak;

    for (auto item : output){
        concat_back(buf, item);
        std::string s = buf;
        dataToBreak.push_back(s);
    }


    while (true){
        std::cout << "Starting broadcast\n";
        usleep(150000);
        for (auto item0 : dataToBreak) {
            if(VERBOSE){
                std::cout << "Atomic broadcast for single item initiated\n";
            }
            string_list dataToSend;
            prepare_to_send(dataToSend, item0);
            for(auto item1 : dataToSend){
                usleep(150000);
                std::stringstream ss;
                ss << item1;
                zmq::message_t request((void *) ss.str().c_str(), ss.str().size() + 1, nullptr);

                int rc = socket.send(request);
                if(VERBOSE){
                    std::cout << "Data \"" << item1 << '\"' << " sent." << std::endl;
                }            }
        }
        usleep(200000);
        for(int i = 0; i < 25; i++)
        {
            usleep(10000);
            std::stringstream ss;
            ss << "^"; // improvised EndOfTranslation
            zmq::message_t request((void *) ss.str().c_str(), ss.str().size() + 1, nullptr);
            socket.send(request);
        }
            // somehow sometimes client does not hear this ending and somehow sending message
            // stops working being wrapped in function. So, this ridiculous cycle exists for the reliability

        if(VERBOSE){
            std::cout << "All data sent.\n";
            std::cout << "Job is done.\n";
        }
        socket.close();
        exit(0);
    }
}

int main(int argc, char * argv[]) {
    fs::path dataFolder = fs::current_path();
    if(argc == 2){
        if(argv[1][0] == '/')
            dataFolder = fs::path(argv[1]);
        dataFolder += (std::string) argv[1];
    }
    else{
        dataFolder += "/data";
    }
    string_list dataFiles;

    scan_data_folder(dataFolder, dataFiles);

    if(dataFiles.empty()){
        exit(-1);
    }

    student_list listToSend;

    read_data(dataFiles[0], listToSend);
    dataFiles.erase(dataFiles.begin());

    if(!dataFiles.empty()){
        student_list buff;
        for(const auto& filename : dataFiles){
            read_data(filename, buff);
            collapse_similar(listToSend, buff);
            buff.clear();
        }
    }

    zmq_server_start_broadcast(listToSend);
}


