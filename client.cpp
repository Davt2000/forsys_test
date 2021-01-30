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
#include <filesystem>
namespace fs = std::filesystem;

int main()
{
    std::string path = fs::current_path();
    path += "/data";
    for (const auto & entry : fs::directory_iterator(path))
        std::cout << entry.path() << std::endl;
}
