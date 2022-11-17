
#include "User.h"
#include "DLinkedList.h"

class Member : public User {
public:
    Member();
    Member(int id, string firstN, string lastN, string user, string pass) ;
    // store session id's to identify all session bookings made
    DLinkedList<int>* bookings = new DLinkedList<int>();
};

