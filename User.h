#ifndef FITNESSMGMTSYSTEM_USER_H
#define FITNESSMGMTSYSTEM_USER_H

#include <string>

using std::string;

#include <stdlib.h>
#include <iostream>

using namespace std;

class User {
public:
    int id;
    string firstName;
    string lastName;
    string username;
    string password;

    User() : id(0), firstName(""), lastName(""), username(""), password("") {}

    User(std::string firstN, string lastN) : id(0), firstName(firstN), lastName(lastN), username(""),
                                             password("") {}

    // prints user name
    void printUserInfo(){std::cout<<firstName;}
};

#endif //FITNESSMGMTSYSTEM_USER_H
