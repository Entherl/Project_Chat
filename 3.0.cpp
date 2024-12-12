#include <iostream>
#include <string>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <unistd.h> // For sleep
#include <cstdlib>  // For system
#include <fstream>  // For File Handling 
#include <termios.h> // For password input handling on macOS

using namespace std;

// Enum for message status
enum MessageStatus { SENT, DELIVERED, READ };

// Message structure
struct Message {
    string sender;
    string content;
    MessageStatus status;
    string timestamp;

    Message(string snd, string msg, MessageStatus st) : sender(snd), content(msg), status(st) {
        timestamp = getCurrentTime();
    }

    static string getCurrentTime() {
        time_t now = time(0);
        tm* localTime = localtime(&now);
        stringstream ss;
        ss << put_time(localTime, "%Y-%m-%d %H:%M:%S");
        return ss.str();
    }

    string getStatusString() const {
        switch (status) {
            case SENT: return "Sent";
            case DELIVERED: return "Delivered";
            case READ: return "Read";
            default: return "Unknown";
        }
    }
};

// Node for linked list
struct Node {
    Message message;
    Node* next;

    Node(const Message& msg) : message(msg), next(nullptr) {}
};

// Queue using linked list
class ChatQueue {
private:
    Node* front;
    Node* rear;

public:
    ChatQueue() : front(nullptr), rear(nullptr) {}

    ~ChatQueue() {
        while (front != nullptr) {
            Node* temp = front;
            front = front->next;
            delete temp;
        }
    }

    void enqueue(string sender, string content) {
        Node* newNode = new Node(Message(sender, content, SENT));
        if (rear == nullptr) {
            front = rear = newNode;
        }
        else {
            rear->next = newNode;
            rear = newNode;
        }
        cout << "Message sent at " << newNode->message.timestamp << ".\n";

        // Saving message to file
        ofstream chatFile("chat_history.txt", ios::app);
        chatFile << "[" << newNode->message.timestamp << "] " << sender << ": " << content << "\n";
        chatFile.close();
    }

    void displayChat() const {
        if (front == nullptr) {
            cout << "No messages in chat.\n";
            return;
        }

        cout << "\n--- Chat History ---\n";
        Node* current = front;
        int index = 1;
        while (current != nullptr) {
            const Message& msg = current->message;
            cout << index++ << ". [" << msg.timestamp << "] ";
            cout << msg.sender << ": " << msg.content << " [" << msg.getStatusString() << "]\n";
            current = current->next;
        }
    }

    void markAllAsRead() {
        Node* current = front;
        while (current != nullptr) {
            current->message.status = READ;
            current = current->next;
        }
        cout << "All messages marked as read.\n";
    }

    void searchMessages(const string& keyword) const {
        cout << "\n--- Search Results for \"" << keyword << "\" ---\n";
        Node* current = front;
        bool found = false;
        while (current != nullptr) {
            const Message& msg = current->message;
            if (msg.content.find(keyword) != string::npos || msg.sender.find(keyword) != string::npos) {
                cout << "[" << msg.timestamp << "] " << msg.sender << ": " << msg.content << "\n";
                found = true;
            }
            current = current->next;
        }
        if (!found) {
            cout << "No messages found for the keyword \"" << keyword << "\".\n";
        }
    }
};


// Helper function for sending a message
void sendMessage(ChatQueue& chat, const string& sender) {
    string content;
    cout << "Enter your message content: ";
    cin.ignore();
    getline(cin, content);
    chat.enqueue(sender, content);
}

// Login system

class signup {
	
protected:
    string username;
    int password;
	
public:
    // constructor
    signup() {
        username = "no name";
        password = 0;
    }
    
    void setName(string n) {
        username = n;
    }
    
    string getName() {
        return username;
    }
    
    void setPass(int p) {
        password = p;
    }
    
    int getPass() {
        return password;
    }
    
    void print() {
        cout << "\n\n\n\n\n\t\t\t\t\t   ********************************************\n"; 
        cout << "\t\t\t\t\t   *         Creating a new Account           *\n";
        cout << "\t\t\t\t\t   *                                          *\n";
        cout << "\t\t\t\t\t   *      Enter Your Username And Password    *\n";
        cout << "\t\t\t\t\t   *                                          *\n";
        cout << "\t\t\t\t\t   ********************************************\n";
        cout << "Enter Username: ";
        cin >> username;
        cout << "Enter Password: ";
        cin >> password;
        
        // File Handling
        ofstream out("logins.txt", ios::app);
        out << username << " ";
        out << password << endl;
        out.close();
    }
};

class signin : protected signup {
public:
    void enterr() {
        string name, pass;

q:
        cout << "\n\n\n\n\n\t\t\t\t\t   ***************************************\n"; 
        cout << "\t\t\t\t\t   *  Enter Your Username And Password   *\n";
        cout << "\t\t\t\t\t   *                                     *\n";
        cout << "\t\t\t\t\t   ***************************************\n"; 
        cout << "\t\t\t\t\t\t------------------------------\n\n";    
        cout << "\t\t\t\t\t\tEnter Username: ";
        cin >> name;
    
        cout << "\n\t\t\t\t\t\tEnter Password: ";
        cin >> pass;

        ifstream in;
        in.open("logins.txt");
        string storedname, storedpass;
        bool found = false;
    
        if (in.is_open()) {
            while (in >> storedname >> storedpass) {
                if (storedname == name && storedpass == pass) {
                    found = true;
                    break;
                }
            }
            in.close();
    
            if (found) {
                cout << "Login successful!\n";
            } else {
                cout << "ERROR! Try Again.\n";
                system("pause");
                goto q;
            }
        }
    }
};

// Main driver
int main() {
    ChatQueue chat;
    string sender;
    int choice;

    cout << "\n\n\n\t\t\t\t ---------------------------------------------";
    cout << "\n\t\t\t\t *********************************************";
    cout << "\n\t\t\t\t                                             ";
    cout << "\n\t\t\t\t                WELCOME TO                   ";
    cout << "\n\t\t\t\t                                             ";
    cout << "\n\t\t\t\t                 MU CHAT                     ";
    cout << "\n\t\t\t\t                                             ";
    cout << "\n\t\t\t\t ********************************************";
    cout << "\n\t\t\t\t ---------------------------------------------\n\n";

    cout << "Clearing the screen in 3 seconds...\n";
    sleep(3);
    system("clear");

    // Login System Object Declaration    
    signin sn;
    signup sp;

    int option;
    while (true) {
        cout << "\t\t\t______________________________________________________________________\n\n\n";
        cout << "\t\t\t                          Welcome To Login Page                        \n\n\n";
        cout << "\t\t\t _____________________________    MENU    ____________________________  \n\n";
        cout << "\t\t\t                                                                         \n\n";
        cout << "\t\t\t\t\t _____________________________________\n";
        cout << "\t\t\t\t\t|                                     |\n";
        cout << "\t\t\t\t\t|        Press 1 For Sign In          |\n ";
        cout << "\t\t\t\t\t|        Press 2 For Sign Up          |\n";
        cout << "\t\t\t\t\t|        Press 3 For Exit             |\n";
        cout << "\t\t\t\t\t|_____________________________________|\n\n\n";
        cout << "\t\t\t\t\t Enter Your Choice :  ";
        cin >> option;
        
        switch (option) {
            case 1:
                sn.enterr();
                system("clear");
                cout << "Enter your username to start chatting: ";
                cin >> sender;

                while (true) {
                    cout << "\n--- Chat Application Menu ---\n";
                    cout << "1. Send Text Message\n";
                    cout << "2. View Chat History\n";
                    cout << "3. Mark All Messages as Read\n";
                    cout << "4. Search Messages\n";
                    cout << "5. Quit\n";
                    cout << "Enter your choice: ";
                    cin >> choice;

                    switch (choice) {
                        case 1:
                            sendMessage(chat, sender);
                            break;
                        case 2:
                            chat.displayChat();
                            break;
                        case 3:
                            chat.markAllAsRead();
                            break;
                        case 4: {
                            string keyword;
                            cout << "Enter keyword to search: ";
                            cin.ignore();
                            getline(cin, keyword);
                            chat.searchMessages(keyword);
                            break;
                        }
                        case 5:
                            cout << "Goodbye!\n";
                            return 0;
                        default:
                            cout << "Invalid choice! Try again.\n";
                    }
                }
                break;   
            case 2:
                sp.print();
                system("clear");
                break;
            case 3:
                cout << "\n\n\t\t Thank You !!\n\n";
                return 0;
            default:
                cout << "Invalid Choice\n";
        }
    }
    return 0;
}