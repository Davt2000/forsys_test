//
// Created by perturabo on 30.01.2021.
//

#ifndef FORSYS_TEST_COMMON_HPP

#include <string>
#include <filesystem>
#include <vector>

inline int VERBOSE = 0;

namespace fs = std::filesystem;

struct student{
    int id{};
    std::string fullName;
    std::string dateOfBirth;
};

typedef std::vector<student> student_list;
typedef std::vector<std::string> string_list;

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

#define FORSYS_TEST_COMMON_HPP

#endif //FORSYS_TEST_COMMON_HPP
