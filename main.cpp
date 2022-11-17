#include <iostream>
#include <fstream>
#include <sstream>
#include <time.h>
#include <string>
#include <stdlib.h>
#include <iomanip>

#include "Admin.h"
#include "Member.h"
#include "FitnessClass.h"
#include "Session.h"

using namespace std;
using std::string;
using std::cout;

// The main class that runs the application

// All admins,members,classes and sessions on our system
DLinkedList<Admin> systemAdmins;
DLinkedList<Member> systemMembers;
DLinkedList<FitnessClass>systemFitnessClasses;
DLinkedList<Session> systemTotalSessions;

// Admin loggedInAdmin;
// the currently logged in Member.
Member* loggedInMember;


void displayMainMenu();
bool loginAdmin();
bool loginMember();
void showAdminMenu();
void registerNewMember(string basicString, string basicString1);
int generateUniqueID();
string convertToLowerCase(string basicString);
void showMemberMenu();
void addFitnessClass();
Node<FitnessClass> *getClassRefByID(int id);
void updateFitnessClassName();
void deleteFitnessClass();
void addSession();
Node<Session> *getSessionRefByID(int id);
void updateSessionDetails();
void deleteSessionFromFitnessClass(int id, int classId);
void deleteSessionFromMembersList(int id);
void deleteSession();
void viewSessionDetails();
void viewListOfMembers();
Node<Member> *getMemberRefByID(int id);
void viewListOfBookings();
void viewSessionsWithVacancies();
void bookASession();
void cancelBooking();
void quitApp();
void saveFitnessClassInfo();
void saveMembersInfo();
void saveSessionsInfo();
void saveAdminInfo();
string parseString(string basicString);
DLinkedList<string> *getListOfMembersInSession();
void viewListOfMembersSorted();
//*************************************************

Member::Member(int id, string firstN, string lastN, string user, string pass): User(firstN, lastN) {
    username = user;
    password = pass;
    this->id = id;
}
Member::Member():User() {
}

Admin::Admin(int id, string firstN, string lastN, string user, string pass) :User(firstN, lastN) {
    username = user;
    password = pass;
    this->id = id;
}
Admin::Admin() :User(){
}

Session::Session(int session_id, int class_id, int max_capacity, string session_date, string session_time,
                 int room_number) :sessionId(session_id),classId(class_id),maxCapacity(max_capacity),
                                   sessionDate(session_date),sessionTime(session_time),roomNumber(room_number){
}
Session::Session() {}

FitnessClass::FitnessClass() :classId(-1),className(""){}
FitnessClass::FitnessClass(int id, string name) :classId(id),className(name){}




//*************************************************

// load all the Admins from the Admins text file;
void loadAdmins() {
    ifstream inFile;
    inFile.open("Admins.txt");
    if (!inFile.is_open()) {
        cout << "Admins File could not be opened, exiting app";
        exit(1);
    }
    // discard first line with titles.
    string lineContents;
    getline(inFile, lineContents);
    // repeatedly read a single line, create an admin object and store into the admins list
    while (getline(inFile, lineContents)) {
        // break the line read into data members of the Admin
        string admin_ID, first_name, last_name, username, password;
        stringstream strStream(lineContents);
        getline(strStream, admin_ID, ',');
        getline(strStream, first_name, ',');
        getline(strStream, last_name, ',');
        getline(strStream, username, ',');
        getline(strStream, password, ',');
        // create an admin object
        Admin currentAdmin (stoi(admin_ID), first_name, last_name, username, password);
        // add to the systems admins list
        systemAdmins.addBack(currentAdmin);
    }
}

// load all the members from the Members text file;
void loadMembers() {
    ifstream inFile;
    inFile.open("Members.txt");
    if (!inFile.is_open()) {
        cout << "Members File could not be opened, exiting app";
        exit(1);
    }
    // discard first line with titles.
    string lineContents;
    getline(inFile, lineContents);
    // repeatedly read a single line, create a member object and store into the members list
    while (getline(inFile, lineContents)) {
        // break the line read into data members of the Member
        string member_ID, first_name, last_name, username, password, list_of_bookings;
        stringstream strStream(lineContents);
        getline(strStream, member_ID, ',');
        getline(strStream, first_name, ',');
        getline(strStream, last_name, ',');
        getline(strStream, username, ',');
        getline(strStream, password, ',');
        getline(strStream, list_of_bookings);
        // create a Member object
        Member currentMember (stoi(member_ID), first_name, last_name, username, password);
        // add his bookings
        // get the substring ignoring the [ ] characters on the start and end from the list_of_bookings
        string bookings = parseString(list_of_bookings);
        // do we have any booking info
        if(bookings.length()==1){
            currentMember.bookings->addBack(stoi(bookings));
        }
        else if(bookings.length()>1){
            stringstream strStream2(bookings);
            string bookingInfo;
            while(getline(strStream2,bookingInfo,',')){
                currentMember.bookings->addBack(stoi(bookingInfo));
            }
        }
        // add member to the system's member list
        systemMembers.addBack(currentMember);

    }
    cout<<"Done loading members"<<endl;
}

//ignoring the [ ] characters on the start and end from the given string
string parseString(string stringValue) {
    string result ="";
    for(int i=0;stringValue[i]!=']';i++)
    {
        if (stringValue[i] != '['){
            result+=stringValue[i];
        }
    }
    return result;
}

// load all the fitness classes from the FitnessClasses text file
void loadFitnessClasses(){
    ifstream inFile;
    inFile.open("FitnessClasses.txt");
    if (!inFile.is_open()) {
        cout << "Fitness Classes File could not be opened, exiting app";
        exit(1);
    }
    // discard first line with titles.
    string lineContents;
    getline(inFile, lineContents);
    // repeatedly read a single line, create a fitness class object and store into the list
    while (getline(inFile, lineContents)) {
        // break the line read into data members of the Member
        string class_ID,class_name,list_of_sessions;
        stringstream strStream(lineContents);
        getline(strStream, class_ID, ',');
        getline(strStream, class_name, ',');
        getline(strStream, list_of_sessions);
        // create a FitnessClass object
        FitnessClass currentFitnessClass (stoi(class_ID), class_name);
        // add sessions
        //ignoring the [ ] characters on the start and end from the list_of_bookings
        string sessions = parseString(list_of_sessions);
        // do we have any booking info
        if(sessions.length()==1){
            currentFitnessClass.sessions->addBack(stoi(sessions));
        }
        else if(sessions.length()>1){
            stringstream strStream2(sessions);
            string sessionInfo;
            while(getline(strStream2,sessionInfo,',')){
                currentFitnessClass.sessions->addBack(stoi(sessionInfo));
            }
        }
        // add session to the system's sessions list
        systemFitnessClasses.addBack(currentFitnessClass);
    }
    cout<<"Done loading Fitness Classes"<<endl;
    inFile.close();
}

// load all the members from the Members text file;
void loadSessions() {
    ifstream inFile;
    inFile.open("Sessions.txt");
    if (!inFile.is_open()) {
        cout << "Sessions File could not be opened, exiting app";
        exit(1);
    }
    // discard first line with titles.
    string lineContents;
    getline(inFile, lineContents);
    // repeatedly read a single line, create a member object and store into the members list
    while (getline(inFile, lineContents)) {
        // break the line read into data members of the Member
        string session_ID,class_ID,max_cap,session_date,session_time,room_number,list_of_registered_members;
        stringstream strStream(lineContents);
        getline(strStream, session_ID, ',');
        getline(strStream, class_ID, ',');
        getline(strStream, max_cap, ',');
        getline(strStream, session_date, ',');
        getline(strStream, session_time, ',');
        getline(strStream, room_number, ',');
        // read the remaining string
        getline(strStream, list_of_registered_members);
        // create a Session object
        Session currentSession (stoi(session_ID), stoi(class_ID), stoi(max_cap), session_date, session_time,stoi(room_number));
        // add registered members in the current session

        //ignoring the [ ] characters on the start and end from the list_of_bookings
        string registrants = parseString(list_of_registered_members);
        if(registrants.length()==1){
            currentSession.registeredMembers->addBack(stoi(registrants));
        }
        else if(registrants.length()>1){
            stringstream strStream2(registrants);
            string registrationInfo;
            while(getline(strStream2,registrationInfo,',')){
                currentSession.registeredMembers->addBack(stoi(registrationInfo));
            }
        }
        // add current session to the system's sessions list
        systemTotalSessions.addBack(currentSession);
    }
    cout<<"Done loading sessions"<<endl;
}

// shows the welcome screen and gives an option to login as admin or member
void displayMainMenu() {

    int selection, memberSelection;
    string user, pass;
    cout<<"Welcome to the Fitness Centre Management System"<<endl;
    do
    {
        cout<<"Enter 1 if you are a Member."<<endl;
        cout<<"Enter 2 if you are an Admin."<<endl;
        cout<<"Enter -1 to exit the System."<<endl;
        cin>>selection;
        // remove the error flag set by cin if user provides an invalid input
        cin.clear();
        // ignore/discard the next 1000 chars until newline char
        cin.ignore(1000,'\n');
        if(selection==1)
        {
            do{
                cout<<"Enter 1 to register, if you are a new member"<<endl;
                cout<<"Enter 2 to login"<<endl;
                cin>>memberSelection;
                cin.clear();
                cin.ignore(1000,'\n');
                if(memberSelection == 1){
                    string fname,lname;
                    cout<<"Please provide your first name"<<endl;
                    cin>>fname;
                    cin.clear();
                    cin.ignore(1000,'\n');
                    cout<<"Please provide your last name"<<endl;
                    cin>>lname;
                    cin.clear();
                    cin.ignore(1000,'\n');
                    registerNewMember(fname,lname);
                    // show member options
                    showMemberMenu();
                }
                else if(memberSelection == 2){
                    while(!(loginMember())){
                        cout<<"Invalid Username or Password, please try again..."<<endl;
                    }
                    showMemberMenu();
                }else{
                    cout<<"Invalid entry, please try again..."<<endl;
                }
            }while(memberSelection!=1 && memberSelection!=2);
        }
        else if(selection==2){
            while(!(loginAdmin())){
                cout<<"Invalid Username or Password, please try again..."<<endl;
            }
            showAdminMenu();
        }else if(selection == -1){
            cout<<"Exiting program... Goodbye."<<endl;
        }else{
            cout<<"Invalid input... Please try again."<<endl;
        }
    }while(selection != -1);
}

// shows the options available to a Member
void showMemberMenu() {
    int choice;
    do{
        cout<<"Dear Member, Please select from the given options..."<<endl;
        cout<<"Enter 1 to book a Session in a Fitness Class"<<endl;
        cout<<"Enter 2 to cancel a booking"<<endl;
        cout<<"Enter 3 to view Session with Vacancies"<<endl;
        cout<<"OR Enter -1 to Quit Application"<<endl;

        cin>>choice;
        cin.clear();
        cin.ignore(1000,'\n');
        if(choice == 1){
            bookASession();
        }
        else if(choice == 2){
            cancelBooking();
        }else if(choice==3){
            viewSessionsWithVacancies();
        }else if(choice==-1){
            quitApp();
        }else{
            cout<<"Invalid Entry, please try again..."<<endl;
        }
    }while(choice!=-1);
}

// saves all the Information and Exits application
void quitApp() {
    saveFitnessClassInfo();
    saveMembersInfo();
    saveSessionsInfo();
    saveAdminInfo();
    exit(0);
}

void saveAdminInfo() {
    ofstream outFile;
    outFile.open ("Admins.txt");
    if (!outFile.is_open()) {
        cout << "Admins File could not be opened, exiting app";
        exit(1);
    }
    //write out headings
    outFile<<"admin_ID,first_name,last_name,username,password"<<endl;
    // go through the global Admins list
    Node<Admin>* currentAdmin = systemAdmins.header->next;
    while(currentAdmin!=systemAdmins.trailer){
        outFile<<currentAdmin->data.id<<","<<currentAdmin->data.firstName<<","<<currentAdmin->data.lastName<<","<<currentAdmin->data.username<<","<<currentAdmin->data.password<<endl;
        currentAdmin = currentAdmin->next;
    }
    outFile.close();
}

void saveSessionsInfo() {
    ofstream outFile;
    outFile.open ("Sessions.txt");
    if (!outFile.is_open()) {
        cout << "Sessions File could not be opened, exiting app";
        exit(1);
    }
    //write out headings
    outFile<<"session_ID,class_ID,max_cap,session_date,session_time,room_number,list_of_registered_members"<<endl;
    // go through the sessions list
    Node<Session>* currentSession = systemTotalSessions.header->next;
    while(currentSession!=systemTotalSessions.trailer){
        outFile<<currentSession->data.sessionId<<","<<currentSession->data.classId<<","<<currentSession->data.maxCapacity<<","<<currentSession->data.sessionDate<<","<<currentSession->data.sessionTime<<","<<currentSession->data.roomNumber<<",[";
        //write the list of registered members
        Node<int>* registeredMemberInfo = currentSession->data.registeredMembers->header->next;
        int totalRegistrants = 0;
        while(registeredMemberInfo!=currentSession->data.registeredMembers->trailer){
            // write out register info
            outFile<<registeredMemberInfo->data;
            totalRegistrants++;
            // skip the comma if it is the last item in the list
            if(totalRegistrants!=currentSession->data.registeredMembers->totalItems){
                outFile<<",";
            }
            registeredMemberInfo=registeredMemberInfo->next;
        }
        outFile<<"]"<<endl;
        currentSession = currentSession->next;
    }
    outFile.close();

}

void saveMembersInfo() {
    ofstream outFile;
    outFile.open ("Members.txt");
    if (!outFile.is_open()) {
        cout << "Members File could not be opened, exiting app";
        exit(1);
    }
    //write out headings
    outFile<<"member_ID,first_name,last_name,username,password,list_of_bookings"<<endl;
    // go through the global members list
    Node<Member>* currentMember = systemMembers.header->next;
    while(currentMember!=systemMembers.trailer){
        outFile<<currentMember->data.id<<","<<currentMember->data.firstName<<","<<currentMember->data.lastName<<","<<currentMember->data.username<<","<<currentMember->data.password<<",[";
        //write the list of bookings
        Node<int>* bookingInfo = currentMember->data.bookings->header->next;
        int totalBookings = 0;
        while(bookingInfo!=currentMember->data.bookings->trailer){
            // write out booking info
            outFile<<bookingInfo->data;
            totalBookings++;
            // skip the comma if it is the last item in the list
            if(totalBookings!=currentMember->data.bookings->totalItems){
                outFile<<",";
            }
            bookingInfo=bookingInfo->next;
        }
        outFile<<"]"<<endl;
        currentMember = currentMember->next;
    }
    outFile.close();
}

void saveFitnessClassInfo() {
    ofstream outFile;
    outFile.open ("FitnessClasses.txt");
    if (!outFile.is_open()) {
        cout << "Fitness Classes File could not be opened, exiting app";
        exit(1);
    }
    //write out headings
    outFile<<"class_ID,class_name,list_of_sessions"<<endl;
    // go through the fitness class list
    Node<FitnessClass>* currentFitnessClass = systemFitnessClasses.header->next;
    while(currentFitnessClass!=systemFitnessClasses.trailer){
        outFile<<currentFitnessClass->data.classId<<","<<currentFitnessClass->data.className<<",[";
        //write the sessions list
        Node<int>* sessionInfo = currentFitnessClass->data.sessions->header->next;
        int totalSessions = 0;
        while(sessionInfo!=currentFitnessClass->data.sessions->trailer){
            // write out session info
            outFile<<sessionInfo->data;
            totalSessions++;
            // skip the comma if it is the last item in the list
            if(totalSessions!=currentFitnessClass->data.sessions->totalItems){
                outFile<<",";
            }
            sessionInfo=sessionInfo->next;
        }
        outFile<<"]"<<endl;
        currentFitnessClass = currentFitnessClass->next;
    }
    outFile.close();
}

// member cancels a booking for some fitness class's session
void cancelBooking() {

    // provide a valid session id
    // search for a session matching the given session id from the session list
    bool sessionFound = false;
    int sessionId;
    Node<Session>* sessionWithBooking;
    do {
        sessionFound = false;
        cout << "Provide the ID of the session whose booking is to be cancelled" << endl;
        cin >> sessionId;
        cin.clear();
        cin.ignore(1000,'\n');
        sessionWithBooking = getSessionRefByID(sessionId);
        if(sessionWithBooking == NULL){
            cout<<"Invalid ID, Session could not be found. try again."<<endl;
        } else{
            sessionFound = true;
        }
    }while(!sessionFound);
    // go through session's registered members list
    DLinkedList<int> *registeredMembers = sessionWithBooking->data.registeredMembers;
    Node<int> *memberInfo = registeredMembers->header->next;
    while (memberInfo!=registeredMembers->trailer){
        if(memberInfo->data == loggedInMember->id){
            // found the member information in the session's member list
            break;
        }
    }
    // if we did find a matching member
    if(memberInfo!=registeredMembers->trailer){
        // remove member info from session's list of registered members
        sessionWithBooking->data.registeredMembers->remove(memberInfo);
        // remove booking info from member's list of bookings
        Node<int>* memberBookingInfo = loggedInMember->bookings->header->next;
        while(memberBookingInfo!= loggedInMember->bookings->trailer){
            if(memberBookingInfo->data == sessionId){
                break;
            }
        }
        loggedInMember->bookings->remove(memberBookingInfo);
        cout<<"Booking cancelled successfully"<<endl;
    }else{
        cout<<"Member is not registered in the Session..."<<endl;
    }
}

// a member books a session in a fitness class if it has a vacancy
void bookASession() {

    // get valid fitness class id from the member
    bool classFound = false;
    int classId;
    Node<FitnessClass>* classToRegisterIn;
    do {
        classFound = false;
        cout << "Provide the ID of the fitness class in which you would like to book a session" << endl;
        cin >> classId;
        cin.clear();
        cin.ignore(1000,'\n');
        classToRegisterIn = getClassRefByID(classId);
        if(classToRegisterIn == NULL){
            cout<<"Invalid ID, fitness class could not be found. try again."<<endl;
        } else{
            classFound = true;
        }
    }while(!classFound);

    // check if the class has a vacancy in its list of available sessions
    DLinkedList<int>* sessions = classToRegisterIn->data.sessions;
    Node<int>* currentSession = sessions->header->next;
    while(currentSession!=sessions->trailer){
        Session sessionObject = getSessionRefByID(currentSession->data)->data;
        // check vacancy
        if(sessionObject.registeredMembers->totalItems<sessionObject.maxCapacity){
            // add member to session
            sessionObject.registeredMembers->addBack(loggedInMember->id);
            // add session to member's bookings
            loggedInMember->bookings->addBack(sessionObject.sessionId);
            cout<<"Session Booked Successfully"<<endl;
            return;
        }
        currentSession = currentSession->next;
    }
    // no available session could be found
    cout<<"Sorry, Selected Fitness Class has no vacant Session."<<endl;

}

// creates a new Member object and adds it to the system's Members list
void registerNewMember(string firstName, string lastName) {
    srand( (unsigned)time(NULL) );
    int memberId = generateUniqueID();
    // converting to lowercase
    string userName = convertToLowerCase(firstName)+ to_string(memberId);
    string password = convertToLowerCase(lastName)+ to_string(rand() % 9000 + 1000);
    // update the currently logged in Member and add to members list
    loggedInMember = new Member(memberId,firstName,lastName,userName,password);
    systemMembers.addBack(*loggedInMember);
}

//convert the given value to a lower case and return it.
string convertToLowerCase(string stringValue) {
    for(int i=0;stringValue[i]!='\0';i++)
    {
        if (stringValue[i] >= 'A' && stringValue[i] <= 'Z'){
            stringValue[i] = stringValue[i] + 32;}
    }
    return stringValue;
}

// generates a random 4 digit number and ensures it is unique
int generateUniqueID() {
    int numberGenerated;
    bool numberIsUnique;
    do{
        // generates a number between 1000 and 9999
        numberGenerated = rand() % 9000 + 1000;
        // assume it is unique
        numberIsUnique = true;
        // go through list of registered members and inspect their id's
        Node<Member>* currentNode = systemMembers.header->next;
        while(currentNode!=systemMembers.trailer){
            // is the number a duplicate
            if(currentNode->data.id==numberGenerated){
                // don't check the rest of the list, repeat generation process
                numberIsUnique = false;
                break;
            }
            currentNode = currentNode->next;
        }
    }while(!numberIsUnique);
    return numberGenerated;
}

void showAdminMenu() {
    int choice;
    do{
        cout << "Dear Admin, Please select from the given options..." << endl;
        cout << "Enter 1 to add a new Fitness Class" << endl;
        cout << "Enter 2 to delete a Fitness Class" << endl;
        cout << "Enter 3 to update a Fitness Class Name" << endl;
        cout << "Enter 4 to add a Session" << endl;
        cout << "Enter 5 to update a Session's Details" << endl;
        cout << "Enter 6 to delete a Session" << endl;
        cout << "Enter 7 to view a Session's Details" << endl;
        cout << "Enter 8 to view List of Members" << endl;
        cout << "Enter 9 to view List of Members Sorted" << endl;
        cout << "Enter 10 to view List of Bookings" << endl;
        cout << "Enter 11 to view Sessions with vacancies" << endl;
        cout << "OR Enter -1 to Quit the Application" << endl;

        cin >> choice;
        if (choice == 1) {
            addFitnessClass();
        } else if (choice == 2) {
            deleteFitnessClass();
        } else if (choice == 3) {
            updateFitnessClassName();
        } else if (choice == 4) {
            addSession();
        } else if (choice == 5) {
            updateSessionDetails();
        } else if (choice == 6) {
            deleteSession();
        } else if (choice == 7) {
            viewSessionDetails();
        } else if (choice == 8) {
            viewListOfMembers();
        } else if (choice == 9) {
            viewListOfMembersSorted();
        } else if (choice == 10) {
            viewListOfBookings();
        } else if (choice == 11) {
            viewSessionsWithVacancies();
        } else if (choice == 12) {
            // viewListOfMembersWithCommonClasses();
        } else if(choice == -1) {
            quitApp();
        }else {
            cout << "Invalid Entry, please try again..." << endl;
        }
    }while(choice!=-1);
}
void viewListOfMemberswithCommonClasses() {





};
void viewListOfMembersSorted() {
    DLinkedList<string>* membersGot = getListOfMembersInSession();
    if(membersGot == NULL){cout<<"No registered members found in the session"<<endl; return;}
    if(membersGot->totalItems == 1){cout<<membersGot->header->next->data<<endl; return;}
    Node<string>* currentPos = membersGot->header->next;
    // stop at second last name
    while(currentPos!=membersGot->trailer->prev){
        Node<string>* p = currentPos;
        Node<string>* q = p->next;
        while(q!=membersGot->trailer){
            // compare strings pointed to by p and q
            string pf,qf, pSeconName, qSecondName;
            // break the full name into firstName and secondName of both strings being compared
            stringstream strStream(p->data);
            getline(strStream, pf, ' ');
            getline(strStream, pSeconName);
            stringstream strStream2(q->data);
            getline(strStream2, qf, ' ');
            getline(strStream2, qSecondName);
            // out of order, swap
            if(qSecondName.compare(pSeconName)<0){
                string temp = p->data;
                p->data = q->data;
                q->data = temp;
            }
            // swap the strings
            p = p->next;
            q = q->next;
        }
        currentPos = currentPos->next;
    }

    // go through list and print member info
    Node<string> *memberName= membersGot->header->next;
    while(memberName!=membersGot->trailer){
        cout<<memberName->data<<endl;
        memberName = memberName->next;
    }

}

// print for a given fitness class all the sessions that have vacancies
void viewSessionsWithVacancies() {
    // ask for fitness class id
    bool classFound = false;
    int classId;
    Node<FitnessClass>* classToBeInspected = NULL;
    do {
        classFound = false;
        cout << "Provide the ID of the class whose vacancies you would like to see" << endl;
        cin >> classId;
        classToBeInspected = getClassRefByID(classId);
        if(classToBeInspected == NULL){
            cout<<"Invalid ID, class could not be found. try again."<<endl;
        } else{
            classFound = true;
        }
    }while(!classFound);
    // go through all sessions in this class and look for sessions with vacancies
    DLinkedList<int>* sessions = classToBeInspected->data.sessions;
    Node<int>* currentSession = sessions->header->next;
    bool vacancyAvailable = false;
    while(currentSession!=sessions->trailer){
        // get the corresponding session object
        Node<Session>* currentSessionObj = getSessionRefByID(currentSession->data);
        // inspect for available vacancy
        if(currentSessionObj->data.registeredMembers->totalItems<currentSessionObj->data.maxCapacity){
            // print session info
            vacancyAvailable = true;
            cout<<"Vacancy available in the following Session"<<endl;
            cout<<"Session ID: "<<currentSessionObj->data.sessionId<<endl;
            cout<<"Max Capacity : "<<currentSessionObj->data.maxCapacity<<endl;
            cout<<"Session Date: "<<currentSessionObj->data.sessionDate<<endl;
            cout<<"Session Time: "<<currentSessionObj->data.sessionTime<<endl;
            cout<<"Room Number: "<<currentSessionObj->data.roomNumber<<endl;
        }
        currentSession = currentSession->next;
    }
    if(!vacancyAvailable){
        cout<<"No vacancies available in any Session of the selected Fitness Class"<<endl;
    }
}

void viewListOfBookings() {
    // search for a member matching the given member id
    bool memberFound = false;
    int memberId;
    Node<Member>* memberToView;
    do {
        memberFound = false;
        cout << "Provide the ID of the Member whose Bookings are to be viewed" << endl;
        cin >> memberId;
        memberToView = getMemberRefByID(memberId);
        if(memberToView == NULL){
            cout<<"Invalid ID, Member could not be found. try again."<<endl;
        } else{
            memberFound = true;
        }
    }while(!memberFound);
    DLinkedList<int>* bookings = memberToView->data.bookings;
    if(memberToView->data.bookings->isEmpty()){
        cout<<"No Booking Information Found..."<<endl;
        return;
    }
    // for each booking info, find the corresponding session id and print out its Session Date and Time
    Node<int> *bookingInfo = bookings->header->next;
    while(bookingInfo!=bookings->trailer){
        // search for the session with a matching id as the bookingInfo
        Node<Session>* currentSession = systemTotalSessions.header->next;
        while(currentSession!=systemTotalSessions.trailer){
            // compare session Id with booking info id
            if(currentSession->data.sessionId == bookingInfo->data){
                // print Session Class Time and Date
                // fetch class by its id and get its name to print
                cout<<"Class Name "<<getClassRefByID(currentSession->data.classId)->data.className<<endl;
                cout<<"Session Time "<<currentSession->data.sessionTime<<endl;
                cout<<"Session Date "<<currentSession->data.sessionDate<<endl;
            }
            // look at next session
            currentSession = currentSession->next;
        }
        bookingInfo = bookingInfo->next;
    }
}

// returns a reference to the member in members list whose id matches the given id
Node<Member> *getMemberRefByID(int id) {
    // search for session, and return reference to where it is stored in the list
    Node<Member>* currentNode = systemMembers.header->next;
    while(currentNode!=systemMembers.trailer){
        if(currentNode->data.id==id){
            return currentNode;
        }
        currentNode = currentNode->next;
    }
    return NULL;
}

// prints information of members in a given session
void viewListOfMembers() {
    DLinkedList<string>* listOfMembers = getListOfMembersInSession();
    if(listOfMembers == NULL){cout<<"No registered members found in the session"<<endl;}
    // go through list and print member info
    Node<string> *memberName= listOfMembers->header->next;
    while(memberName!=listOfMembers->trailer){
        cout<<memberName->data<<endl;
        memberName = memberName->next;
    }
}

// return a list of members in a session
DLinkedList<string> *getListOfMembersInSession() {
    // search for a session matching the given session id
    bool sessionFound = false;
    int sessionId;
    Node<Session>* sessionToView;
    do {
        sessionFound = false;
        cout << "Provide the ID of the session to be viewed" << endl;
        cin >> sessionId;
        cin.clear();
        cin.ignore(1000,'\n');
        sessionToView = getSessionRefByID(sessionId);
        if(sessionToView == NULL){
            cout<<"Invalid ID, Session could not be found. try again."<<endl;
        } else{
            sessionFound = true;
        }
    }while(!sessionFound);
    // no registered members in session
    if(sessionToView->data.registeredMembers->isEmpty()){
        return NULL;
    }
    DLinkedList<string>* memberNames = new DLinkedList<string>();
    // get the list of registered members in the current session
    DLinkedList<int> *registrants = sessionToView->data.registeredMembers;
    // for each registrant, find the corresponding member id and print out its info
    Node<int> *regId = registrants->header->next;
    while(regId!=registrants->trailer){
        // search for member from member list with the same id
        Node<Member>* currentMember = systemMembers.header->next;
        while(currentMember!=systemMembers.trailer){
            // compare registrant id with current member id
            if(currentMember->data.id == regId->data){
                // store member name
                memberNames->addBack(currentMember->data.firstName+" "+currentMember->data.lastName);
            }
            // look at next member
            currentMember = currentMember->next;
        }
        regId = regId->next;
    }
    return memberNames;
}

// Admin prints the details of a Session using session id
void viewSessionDetails() {
    // search for a session matching the given session id
    bool sessionFound = false;
    int sessionId;
    Node<Session>* sessionToView;
    do {
        sessionFound = false;
        cout << "Provide the ID of the session to be viewed" << endl;
        cin >> sessionId;
        sessionToView = getSessionRefByID(sessionId);
        if(sessionToView == NULL){
            cout<<"Invalid ID, Session could not be found. try again."<<endl;
        } else{
            sessionFound = true;
        }
    }while(!sessionFound);
    cout<<"Session ID: "<<sessionToView->data.sessionId<<endl;
    cout<<"Class ID: "<<sessionToView->data.classId<<endl;
    cout<<"Max Capacity : "<<sessionToView->data.maxCapacity<<endl;
    cout<<"Session Date: "<<sessionToView->data.sessionDate<<endl;
    cout<<"Session Time: "<<sessionToView->data.sessionTime<<endl;
    cout<<"Room Number: "<<sessionToView->data.roomNumber<<endl;
}

// Admin deletes a session
void deleteSession() {
    // search for a session matching the given session id
    bool sessionFound = false;
    int sessionId;
    Node<Session>* sessionToDelete;
    do {
        sessionFound = false;
        cout << "Provide the ID of the session to be Deleted" << endl;
        cin >> sessionId;
        sessionToDelete = getSessionRefByID(sessionId);
        if(sessionToDelete == NULL){
            cout<<"Invalid ID, Session could not be found. try again."<<endl;
        } else{
            sessionFound = true;
        }
    }while(!sessionFound);
    // delete session from fitness class list
    deleteSessionFromFitnessClass(sessionId, sessionToDelete->data.classId);
    // delete session from members list stored as a booking
    deleteSessionFromMembersList(sessionId);
    // delete the session from the session list
    systemTotalSessions.remove(sessionToDelete);
    cout<<"Session deleted successfully"<<endl;
}

// go through all member list, find if anyone has this session as a booking, delete it
void deleteSessionFromMembersList(int id) {
    Node<Member>* memberInfo = systemMembers.header;
    // look at all members
    while(memberInfo!=systemMembers.trailer){
        Node<int>* bookingInfo = memberInfo->data.bookings->header->next;
        while(bookingInfo!=memberInfo->data.bookings->trailer){
            if(bookingInfo->data==id){
                break;
            }
            bookingInfo = bookingInfo->next;
        }
        //delete it from the bookings list
        if(bookingInfo!=memberInfo->data.bookings->trailer){
            memberInfo->data.bookings->remove(bookingInfo);
        }
        memberInfo = memberInfo->next;
    }
}

// delete a session from a fitness class
void deleteSessionFromFitnessClass(int sessionId, int classId) {
    Node<FitnessClass> * fitnessClass = getClassRefByID(classId);
    // go through fitness class list of sessions, delete occurrence of sessionId
    DLinkedList<int>* sessionList =  fitnessClass->data.sessions;
    Node<int>* current = sessionList->header->next;
    // find the specific session to delete
    while(current!=sessionList->trailer){
        if(current->data == sessionId){break;}
        current = current->next;
    }
    // what if no session was found
    if(current!=sessionList->trailer) {
        sessionList->remove(current);
    }
}

// update a session's details.
void updateSessionDetails() {
    // search for a session matching the given session id
    bool sessionFound = false;
    int sessionId;
    Node<Session>* sessionToUpdate;
    do {
        sessionFound = false;
        cout << "Provide the ID of the session to be updated" << endl;
        cin >> sessionId;
        sessionToUpdate = getSessionRefByID(sessionId);
        if(sessionToUpdate == NULL){
            cout<<"Invalid ID, Session could not be found. try again."<<endl;
        } else{
            sessionFound = true;
        }
    }while(!sessionFound);

    //provide user options that he could update in the session
    cout<<"Enter 1 to update Session's Capacity"<<endl;
    cout<<"Enter 2 to update Session's Room"<<endl;
    cout<<"Enter 3 to update Session's Date"<<endl;
    cout<<"Enter 4 to update Session's Time"<<endl;
    int chosenOption;
    cin>>chosenOption;
    cin.clear();
    cin.ignore(1000,'\n');
    do{
        if(chosenOption == 1){
            int cap;
            cout<<"Enter a new value for the Session's Capacity"<<endl;
            cin>>cap;
            cin.clear();
            cin.ignore(1000,'\n');
            sessionToUpdate->data.maxCapacity = cap;
        }else if(chosenOption == 2){
            int room;
            cout<<"Enter a new value for the Session's Room"<<endl;
            cin>>room;
            cin.clear();
            cin.ignore(1000,'\n');
            sessionToUpdate->data.roomNumber = room;
        }else if(chosenOption == 3){
            string date;
            cout<<"Enter a new value for the Session's Date"<<endl;
            cin>>date;
            cin.clear();
            cin.ignore(1000,'\n');
            sessionToUpdate->data.sessionDate = date;
        }else if(chosenOption == 4){
            string time;
            cout<<"Enter a new value for the Session's Time"<<endl;
            cin>>time;
            cin.clear();
            cin.ignore(1000,'\n');
            sessionToUpdate->data.sessionTime = time;
        }else{
            cout<<"Invalid Entry, please try again..."<<endl;
        }
    }while(chosenOption<0||chosenOption>4);

    cout<<"Session Details updated successfully"<<endl;
}

// creates a new session, adds it to the session list and also to a fitness class
void addSession() {
    // prompt user for a valid class id and search the class list for the respective fitness class
    bool classFound = false;
    int classId;
    Node<FitnessClass>* classToBeUpdated = NULL;
    do {
        classFound = false;
        cout << "Provide the ID of the class in which you would like to add a session" << endl;
        cin >> classId;
        cin.clear();
        cin.ignore(1000,'\n');
        // get the reference of the matching fitness class with the provided id
        classToBeUpdated = getClassRefByID(classId);
        if(classToBeUpdated == NULL){
            cout<<"Invalid ID, fitness class could not be found. try again."<<endl;
        } else{
            classFound = true;
        }
    }while(!classFound);

    // repeatedly ask for session id until a valid id is provided
    Node<Session>* sessionGot = NULL;
    bool sessionFound;
    int sessionId;
    do {
        sessionFound = false;
        cout << "Provide the session Id" << endl;
        cin >> sessionId;
        cin.clear();
        cin.ignore(1000,'\n');
        // is this id already allocated to some session?
        sessionGot = getSessionRefByID(sessionId);
        if(sessionGot != NULL){
            cout<<"Invalid Session Id OR Id is already Allocated. please try again."<<endl;
        } else{
            sessionFound = true;
        }
    }while(!sessionFound);

    // Get remaining data required to create a session
    int roomNo;

    string sessionDate,sessionTime;
    cout << "Provide the session Date" << endl;
    cin >> sessionDate;
    cin.clear();
    cin.ignore(1000,'\n');
    cout << "Provide the session Room No" << endl;
    cin >> roomNo;
    cin.clear();
    cin.ignore(1000,'\n');
    cout << "Provide the session Time" << endl;
    cin >> sessionTime;
    cin.clear();
    cin.ignore(1000,'\n');
    // create a new session object
    Session* session = new Session(sessionId,classId,20,sessionDate,sessionTime,roomNo);
    // add it to the session list
    systemTotalSessions.addBack(*session);
    // add session to fitness class
    // get the list of sessions stored in the current fitness class and add the newly created session
    classToBeUpdated->data.sessions->addBack(sessionId);
    cout<<"Session added successfully"<<endl;
}

void deleteFitnessClass() {
    bool classFound = false;
    int classId;
    Node<FitnessClass>* classToBeDeleted;
    do {
        classFound = false;
        cout << "Provide the ID of the class to be deleted" << endl;
        cin >> classId;
        classToBeDeleted = getClassRefByID(classId);
        if(classToBeDeleted == NULL){
            cout<<"Invalid ID, class could not be found. try again."<<endl;
        } else{
            classFound = true;
        }
    }while(!classFound);

    // delete all sessions with this class id
    DLinkedList<int>* sessions = classToBeDeleted->data.sessions;
    Node<int>* currentSession = sessions->header->next;
    // for each session in the current class to be deleted, remove it from the sessions list
    while(currentSession!=sessions->trailer){
        systemTotalSessions.remove(getSessionRefByID(currentSession->data));
        currentSession = currentSession->next;
    }
    // delete class from list of fitness classes
    systemFitnessClasses.remove(classToBeDeleted);
    cout<<"Fitness Class deleted successfully"<<endl;
}

void updateFitnessClassName() {
    string updatedClassName;
    bool classFound = false;
    int classId;
    Node<FitnessClass>* classToBeUpdated = NULL;
    do {
        classFound = false;
        cout << "Provide the ID of the class to be Updated" << endl;
        cin >> classId;
        classToBeUpdated = getClassRefByID(classId);
        if(classToBeUpdated == NULL){
            cout<<"Invalid ID, class could not be found. try again."<<endl;
        } else{
            classFound = true;
        }
    }while(!classFound);
    cout << "Provide the updated Name of the class" << endl;
    cin >> updatedClassName;
    classToBeUpdated->data.className = updatedClassName;
    cout<<"Class Name updated successfully"<<endl;
}

// searches the global list of FitnessClasses for a class with the given id
Node<FitnessClass> *getClassRefByID(int id) {
    // search for class, and return reference to where it is stored in the list
    Node<FitnessClass>* currentNode = systemFitnessClasses.header->next;
    while(currentNode!=systemFitnessClasses.trailer){
        if(currentNode->data.classId==id){
            return currentNode;
        }
        currentNode = currentNode->next;
    }
    return NULL;
}

// searches the global list of sessions for a session with the given id
// returns a reference to the node storing the matching session
Node<Session> *getSessionRefByID(int id) {
    // search for session, and return reference to where it is stored in the list
    Node<Session>* currentNode = systemTotalSessions.header->next;
    while(currentNode!=systemTotalSessions.trailer){
        //found the session being searched for
        if(currentNode->data.sessionId==id){
            return currentNode;
        }
        currentNode = currentNode->next;
    }
    return NULL;
}

void addFitnessClass() {
    bool classFound = false;
    int classId;
    Node<FitnessClass>* classToBeUpdated = NULL;
    do {
        classFound = false;
        cout << "Provide the ID of the class to be added" << endl;
        cin >> classId;
        classToBeUpdated = getClassRefByID(classId);
        if(classToBeUpdated != NULL){
            cout<<"Invalid ID or ID is already allocated. please try again."<<endl;
        } else{
            classFound = true;
        }
    }while(!classFound);

    string className;
    cout << "Provide the Name of the class to be added" << endl;
    cin >> className;
    cin.clear();
    cin.ignore(1000,'\n');
    systemFitnessClasses.addBack(*new FitnessClass(classId,className));
    cout<<"Class added successfully"<<endl;
}

bool loginAdmin() {
    string username,pass;
    cout<<"Please provide your username ";
    cin>>username;
    cin.clear();
    cin.ignore(1000,'\n');
    cout<<"Please provide your password ";
    cin>>pass;
    cin.clear();
    cin.ignore(1000,'\n');
    // manually go through the admin list nodes and find a match for username and password
    Node<Admin>* currentNode = systemAdmins.header->next;
    while(currentNode!=systemAdmins.trailer){
        if((currentNode->data.username==username) && (currentNode->data.password == pass)){
            cout<<"Login Successful"<<endl;
            return true;
        }
        currentNode=currentNode->next;
    }
    return false;
}


bool loginUser(string username, string pass, DLinkedList<User>){
    return false;
}

bool loginMember() {
    string username,pass;
    cout<<"Please provide your username ";
    cin>>username;
    cin.clear();
    cin.ignore(1000,'\n');
    cout<<"Please provide your password ";
    cin>>pass;
    cin.clear();
    cin.ignore(1000,'\n');

    // manually go through the list nodes and find a match for username and password
    Node<Member>* currentNode = systemMembers.header->next;
    while(currentNode!=systemMembers.trailer){
        if((currentNode->data.username==username) && (currentNode->data.password == pass)){
            cout<<"Login Successful"<<endl;
            loggedInMember = &currentNode->data;
            return true;
        }
        currentNode = currentNode->next;
    }
    return false;
}

int main() {
    loadAdmins();
    loadMembers();
    loadFitnessClasses();
    loadSessions();
    displayMainMenu();

    return 0;
}

