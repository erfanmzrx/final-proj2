#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <ctime>
#include <iomanip>
using namespace std;

class task {
public:
    string taskname;
    bool isStarred;
    int dateDay;
    int dateMonth;
    int dateYear;
    string description;
    bool isCompleted;

public:
    task()
    {
        isCompleted = false;
    }
    void task2()
    {
        cout << "Enter your task name: " << endl;
        cin >> taskname;
        cout << "Is this a starred task?" << endl;
        cout << "1. Yes " << endl << "2. No " << endl;
        int choice = 0;
        while (choice < 1 || choice > 2)
        {
            cin >> choice;
        }
        if (choice == 1)
            isStarred = true;
        else
            isStarred = false;
        cout << "Do you want to set an alert time for this task? " << endl;
        cout << "1. Yes " << endl << "2. No " << endl;
        choice = 0;
        while (choice < 1 || choice > 2)
        {
            cin >> choice;
        }
        if (choice == 1)
        {
            cout << "Enter alert date: " << endl;
            cout << "Day:  ";
            cin >> dateDay;
            cout << "Month:  ";
            cin >> dateMonth;
            cout << "Year:  ";
            cin >> dateYear;
        }
        if (choice == 2)
        {
            dateDay = 0;
            dateMonth = 0;
            dateYear = 0;
        }
        cout << "Do you want to write a description for your task? " << endl;
        cout << "1. Yes " << endl << "2. No " << endl;
        choice = 0;
        while (choice < 1 || choice > 2)
        {
            cin >> choice;
        }
        if (choice == 1)
        {
            cout << "Write your description below: " << endl;
            cin.ignore();
            getline(cin, description);
        }
    }
    // task(string name, bool starred, int day, int month, int year, string desc, bool completed)
    //     : taskname(name), isStarred(starred), dateDay(day), dateMonth(month), dateYear(year),
    //       description(desc), isCompleted(completed) {}

    void save(ofstream& outFile) const {
        outFile << taskname << endl;
        outFile << isStarred << endl;
        outFile << dateDay << endl;
        outFile << dateMonth << endl;
        outFile << dateYear << endl;
        outFile << description << endl;
        outFile << isCompleted << endl;
    }

    void load(ifstream& inFile) {
        inFile >> taskname;
        inFile >> isStarred;
        inFile >> dateDay;
        inFile >> dateMonth;
        inFile >> dateYear;
        inFile.ignore();
        getline(inFile, description);
        inFile >> isCompleted;
    }
};

class tasknode {
public:
    task data;
    tasknode* next;
    tasknode(const task& task) : data(task), next(nullptr) {}
};

class tasklist {
public:
    string name;
    string color;
    tasknode* head;
    int size;

public:
    tasklist()
    {
        size = 0;
        head = nullptr;
    }
    void tasklist2()
    {
        cout << "Enter a name for your list " << endl;
        cin >> this->name;
        cout << "Select a color for your list" << endl;
        cout << "1. Red " << endl << "2. Blue " << endl << "3. Green " << endl;
        int choice = 0;
        while (choice < 1 || choice > 3)
        {
            cin >> choice;
            if (choice == 1)
                this->color = "Red";
            else
            {
                if (choice == 2)
                    this->color = "Blue";
                else
                {
                    if (choice == 3)
                        this->color = "Green";;
                }
            }
        }
    }

    void addtask(const task& task) {
        tasknode* newNode = new tasknode(task);
        if (!head) {
            head = newNode;
            size++;
        }
        else {
            tasknode* current = head;
            while (current->next) {
                current = current->next;
            }
            current->next = newNode;
            size++;
        }
    }

    void save(ofstream& outFile) const {
        outFile << name << endl;
        outFile << color << endl;
        outFile << size << endl;
        tasknode* current = head;
        while (current) {
            current->data.save(outFile);
            current = current->next;
        }
    }

    void load(ifstream& inFile) {
        inFile >> name;
        inFile >> color;
        inFile >> size;
        int tmp=size;
        head = nullptr;
        size=0;
        for (int i = 0; i < tmp; ++i) {
            task t;
            t.load(inFile);
            addtask(t);
        }
    }

    void operator<<(const task& task) {
        this->addtask(task);
    }
};

class account {
public:
    string username;
    string password;
    vector<tasklist> lists;
    int listcount;

    account() {
        listcount = 0;
        lists.clear();
    }

    void save(ofstream& outFile) const {
        outFile << username << endl;
        outFile << password << endl;
        outFile << listcount << endl;
        for (const auto& list : lists) {
            list.save(outFile);
        }
    }

    void load(ifstream& inFile) {
        inFile >> username;
        inFile >> password;
        inFile >> listcount;
        int tmp=listcount;
        lists.clear();
        for (int i = 0; i < tmp; ++i) {
            tasklist list;
            list.load(inFile);
            lists.push_back(list);
        }
    }
};

vector<account> persons;
account selected_acc;

bool compareDates(int day, int month, int year) {
    time_t t = time(0);
    tm* now = localtime(&t);
    int currentDay = now->tm_mday;
    int currentMonth = now->tm_mon + 1; 
    int currentYear = now->tm_year + 1900;
    return (day == currentDay && month == currentMonth && year == currentYear);
}

void login() {
    while (true) {
        cout << "Enter your username: " << endl;
        string tmpname;
        cin >> tmpname;
        cout << "Enter your password" << endl;
        string tmppass;
        cin >> tmppass;
        for (auto itr = persons.begin(); itr < persons.end(); ++itr)
        {
            if (itr->username == tmpname && itr->password == tmppass)
            {
                selected_acc = *itr;
                return;
            }
        }
        cout << "Username or password wrong! Try again " << endl;
    }

}

void sign_up() {
    account tmp;
    cout << "Create your username: " << endl;
    cin.ignore();
    getline(cin, tmp.username);
    cout << "Create your password: " << endl;
    getline(cin, tmp.password);
    selected_acc = tmp;
    persons.push_back(selected_acc);
}

void maketasklist() {
    tasklist list;
    list.tasklist2();
    selected_acc.lists.push_back(list);
    selected_acc.listcount++;
    for (auto itr = persons.begin(); itr < persons.end(); ++itr)
    {
        if (itr->username == selected_acc.username && itr->password == selected_acc.password)
        {
            itr->lists.push_back(list);
            itr->listcount++;
            break;
        }
    }
}

void addtask() {
    if (selected_acc.listcount == 0)
    {
        cout << "You don't have any task list! Backing to menu." << endl;
        return;
    }
    cout << "Choose one of your lists" << endl;
    int i = 1;
    for (auto itr = selected_acc.lists.begin(); itr < selected_acc.lists.end(); ++itr)
    {
        cout << i << ". " << itr->name << endl;
        i++;
    }
    int choice = 0;
    while (choice < 1 || choice > i)
    {
        cin >> choice;
    }
    task task;
    task.task2();
    // selected_acc.lists.at(choice - 1).addtask(task);
    for (auto itr = persons.begin(); itr < persons.end(); ++itr)
    {
        if (itr->username == selected_acc.username && itr->password == selected_acc.password)
        {
            // itr->lists.at(choice - 1).addtask(task);
            itr->lists.at(choice - 1)<<task;
            break;
        }
    }
    cout << "Task added successfully." << endl;
    return;
}

void showstarredtasks() {
    int i = 1;
    if (selected_acc.listcount == 0)
    {
        cout << "Empty! Backing to menu." << endl;
        return;
    }
    for (auto itr = persons.begin(); itr < persons.end(); ++itr)
    {
        if (itr->username == selected_acc.username && itr->password == selected_acc.password)
        {
            for(auto itr2 = itr->lists.begin(); itr2 < itr->lists.end() ; ++itr2 )
            {
                if (itr2->size == 0)
                    continue;
                tasknode* tmp = itr2->head;
                while (tmp)
                {
                    if (tmp->data.isStarred)
                    {
                        cout << i << ". " << tmp->data.taskname << endl;
                        i++;
                    }
                    tmp = tmp->next;
                }     
            }
            break;
        }
    }
    // for (auto itr = selected_acc.lists.begin(); itr < selected_acc.lists.end(); ++itr)
    // {
    //     if (itr->size == 0)
    //         continue;
    //     tasknode* tmp = itr->head;
    //     while (tmp)
    //     {
    //         if (tmp->data.isStarred)
    //         {
    //             cout << i << ". " << tmp->data.taskname << endl;
    //             i++;
    //         }
    //         tmp = tmp->next;
    //     }
    // }
    return;
}

void showtasklist() {
    cout << "Choose one of your lists" << endl;
    int i = 1;
    for (auto itr = selected_acc.lists.begin(); itr < selected_acc.lists.end(); ++itr)
    {
        cout << i << ". " << itr->name << endl;
        i++;
    }
    int lchoice = 0;
    while (lchoice < 1 || lchoice > i)
    {
        cin >> lchoice;
    }
    cout << "Choose one of your tasks for more details: " << endl;
    int j = 0;

    tasknode* tmp ;
    for (auto itr = persons.begin(); itr < persons.end(); ++itr)
    {
        if (itr->username == selected_acc.username && itr->password == selected_acc.password)
        {
            tmp=itr->lists.at(lchoice-1).head;
            break;
        }
    }
    while (tmp)
    {
        j++;
        cout << j << ". " << tmp->data.taskname << endl;
        tmp = tmp->next;
    }
    int tchoice = 0;
    while (tchoice < 1 || tchoice > j)
    {
        cin >> tchoice;
    }
    tasknode* tmp2 ;//selected_acc.lists.at(lchoice - 1).head;
    for (auto itr = persons.begin(); itr < persons.end(); ++itr)
    {
        if (itr->username == selected_acc.username && itr->password == selected_acc.password)
        {
            tmp2=itr->lists.at(lchoice-1).head;
            break;
        }
    }
    for (int z = 0; z < tchoice - 1; z++)
    {
        tmp2 = tmp2->next;
    }
    cout << tmp2->data.taskname << endl << tmp2->data.description << endl;
    if (tmp2->data.isStarred)
        cout << "**starred task**" << endl;
    if (tmp2->data.isCompleted)
    {
        cout << "This task is done." << endl;
        return;
    }
    else
    {
        cout << "Have you done this task?" << endl;
        cout << "1. Yes " << endl << "2. No " << endl;
        int choice = 0;
        while (choice < 1 || choice > 2)
        {
            cin >> choice;
        }
        if (choice == 1)
        {
            tmp2->data.isCompleted = true;
            for (auto itr = persons.begin(); itr < persons.end(); ++itr)
            {
                if (itr->username == selected_acc.username && itr->password == selected_acc.password)
                {
                    tasknode* tmp2 = itr->lists.at(lchoice - 1).head;
                    for (int z = 0; z < tchoice - 1; z++)
                    {
                        tmp2 = tmp2->next;
                    }
                    tmp2->data.isCompleted = true;
                }
            }
            cout << "Task done successfully." << endl;
        }
        if (choice == 2)
        {
            cout << "Okk!" << endl;
        }
    }
    cout << "Task alert time: " << endl;
    cout << tmp2->data.dateDay << "/" << tmp2->data.dateMonth << "/" << tmp2->data.dateYear << endl;
    return;
}

void displaymenu() {
    int choice = 0;
    while (choice != 6)
    {
        cout << "Select an option below: " << endl;
        cout << "1. Create a new task list " << endl;
        cout << "2. Add a task to an existing list " << endl;
        cout << "3. Show starred tasks " << endl;
        cout << "4. Show task lists" << endl;
        cout << "5. Create a PDF file of an existing list " << endl;
        cout << "6. Exit" << endl;
        cin >> choice;
        switch (choice)
        {
        case 1: maketasklist(); break;
        case 2: addtask(); break;
        case 3: showstarredtasks(); break;
        case 4: showtasklist(); break;
        case 6: return;
        }
    }
}

void saveToFile() {
    ofstream outFile("tasks.txt");
    if (!outFile) {
        cout << "Error opening file for writing!" << endl;
        return;
    }
    outFile << persons.size() << endl;
    for (const auto& acc : persons) {
        acc.save(outFile);
    }
    outFile.close();
}

void loadFromFile() {
    ifstream inFile("tasks.txt");
    if (!inFile) {
        cout << "Error opening file for reading or file does not exist!" << endl;
        return;
    }
    size_t personCount;
    inFile >> personCount;
    inFile.ignore(); // Ignore the newline character
    persons.clear();
    for (size_t i = 0; i < personCount; ++i) {
        account acc;
        acc.load(inFile);
        persons.push_back(acc);
    }
    inFile.close();
}

int main() {
    loadFromFile();
    while (true) {
        cout << "Welcome\nPlease login or sign up\n1. Login\n2. Sign up\n3. Exit\n";
        int choice = 0;
        while (choice < 1 || choice > 3) {
            cin >> choice;
        }
        if (choice == 1) login();
        if (choice == 2) sign_up();
        if (choice == 3) {
            saveToFile();
            return 0;
        }
        for (auto itr = persons.begin(); itr < persons.end(); ++itr)
        {
            if (itr->username == selected_acc.username && itr->password == selected_acc.password)
            {
                for(auto itr2 = itr->lists.begin(); itr2 < itr->lists.end() ; ++itr2 )
                {
                    if (itr2->size == 0)
                        continue;
                    tasknode* tmp = itr2->head;
                    while (tmp)
                    {
                        if(compareDates(tmp->data.dateDay , tmp->data.dateMonth , tmp->data.dateYear))
                        {
                            cout << tmp->data.taskname << "  alert time is today! don't forget to do it." << endl;
                        }
                        tmp = tmp->next;
                    }     
                }
                break;
            }
        }
        displaymenu();
    }
}
