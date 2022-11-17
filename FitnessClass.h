
#include <string>
using std::string;
#include "DLinkedList.h"

class FitnessClass {
public:
    int classId;
    string className;
    FitnessClass();
    FitnessClass(int id, string name);

    // store session id's to identify all sessions available in a fitness class
    DLinkedList<int>* sessions=new DLinkedList<int>();
};



