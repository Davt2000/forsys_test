#include <string>
#include <filesystem>
#include <fstream>
#include <vector>
#include <iostream>
#include <zmq.hpp>

namespace fs = std::filesystem;

struct student{
    int id{};
    std::string fullName;
    std::string dateOfBirth;
};

typedef std::vector<student> student_list;
typedef std::vector<std::string> string_list;


int scan_data_folder(const std::string& dataFolder, string_list &output){
    for (const auto & entry : fs::directory_iterator(dataFolder))
        if(entry.is_regular_file())
            output.push_back(entry.path());
}

int parse_string(student& student_out,const std::string& input){
    std::string reference = input;
    student_out.id = stoi(reference.substr(0, reference.find(' ')));
    reference = reference.substr(reference.find(' ') + 1);

    student_out.fullName = "";
    student_out.fullName += reference.substr(0, reference.find(' '));
    reference = reference.substr(reference.find(' ') + 1);
    student_out.fullName += " " + reference.substr(0, reference.find(' '));
    reference = reference.substr(reference.find(' ') + 1);
    if(isalpha(reference[0]))      // case there is 3 part full name
        student_out.fullName += " " + reference.substr(0, reference.find(' '));

    student_out.dateOfBirth = reference;
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

void concat_back(char * string, student& st){
    std::string s = std::to_string(st.id);
    char const *pchar = s.c_str();
    memset(string, 0, 255);
    strcat(string, pchar);
    strcat(string, " ");
    strcat(string, st.fullName.c_str());
    strcat(string, " ");
    strcat(string, st.dateOfBirth.c_str());
}

bool send(zmq::socket_t& socket, const char* data) {
    size_t size = strlen(data); // Assuming your char* is NULL-terminated
    zmq::message_t message(size);
    std::memcpy (message.data(), data, size);
    bool rc = socket.send (message);
    return (rc);
}

void zmq_server_send(student_list& output) {
    char *buf = nullptr;
    zmq::context_t context(1);
    zmq::socket_t socket(context, ZMQ_PUB);
    socket.bind("tcp://*:5555");
    zmq::message_t request;
    for (auto item : output) {
        concat_back(buf, item);
        send(socket, buf);

    }
}

int send_html(student_list& output){

}

int print_list(student_list& output){
    for(const auto& i : output)
        std::cout << i.id << ' ' << i.fullName << ' ' << i.dateOfBirth << std::endl;
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

    zmq_server_send(listToSend);

    return 0;
}

// read data from 'data' into a vector of lists
// create a common list
// send


//  == ЗАДАЧА 1 (С++ ZMQ) ==
//
//Программа обработки и передачи информации о студентах клиентам от сервера по протоколу ZeroMQ.
//
//Действия сервера:
// - Сформировать обобщенный список студентов. В качестве исходных данных использовать 2 файла
// 	student_file_1.txt и student_file_2.txt в следующем формате:
//		 student_file_1.txt
//		----
//		1 Ivan Ivanovich 01.01.1988
//		2 Petr Petrov 04.07.1988
//		3 Denis Denisov 43.01.1988
//		4 Vladimir Jukov 04.5.1987
//		5 Vladimir Kochkin 11.10.1989
//		----
//
//		student_file_2.txt
//		----
//		34 Ivan Ivanovich 01.01.1988
//		54 Petr Kazakov 05.07.1986
//		23 Denis Denisov 04.03.1988
//		12 Vladimir Jukov 04.5.1987
//		43 Vladimir Kochkin 11.11.1989
//
// - Объединить одинаковых студентов по ФИО и дате рождения
// - Отправить полученный список на клиент по протоколу ZeroMQ pub






















// == ЗАДАЧА 2 (C++ HTTP) ==
//
//Реализовать аналогичный обмен (с задачей 1) по протоколам HTTP\REST.
//
// == ДОПОЛНИТЕЛЬНЫЕ ТРЕБОВАНИЯ К ЗАДАЧА 1 и ЗАДАЧА 2 (ОПЦИОНАЛЬНО):
// - решение задачи 2 должно являться расширением функционала задачи 1
// - обеспечить одновременную работу задачи 1 и задачи 2 в рамках разных потоков одного адресного пространства
// - многопоточность
//  -- прием и обработка данных должны выполняться в отдельных потоках (для http свой, для zmq свой)
//  -- обработка данных должна всегда проходить в отдельном потоке (всегда в одном)
//  -- Gui поток не должен выполнять задачи по приему, разбору и обработке данных
//
// == ЗАДАЧА 3 (SQL) ==
//
//Имеется table1(id, value) и table2(id, value) с однородными, местами дублирующими данными.
//Необходимо:
// - Написать SQL скрипт создания таблицы table3(id, value)
// - Написать SQL скрипт наполняющий созданную таблицу данными по следующим правилам:
//	 - все записи из table1, которых нет в table2 поместить в table3
//	 - все записи из table2, которых нет в table1 поместить в table3


