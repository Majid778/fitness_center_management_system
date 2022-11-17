
#include <string>
using std::string;
#include "DLinkedList.h"


class Session {
public:
    int sessionId;
    int classId;
    int maxCapacity;
    string sessionDate;
    string sessionTime;
    int roomNumber;
    Session();
    Session(int session_id, int class_id, int max_capacity, string session_date, string session_time, int room_number);
    // store member id's to identify all registered members in current session
    DLinkedList<int>* registeredMembers = new DLinkedList<int>();
};

