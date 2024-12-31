#include <iostream>
#include <cstdlib>
#include <fstream> // For File Handling 
#include <unistd.h> // For sleep
#include <termios.h> // For password input handling on macOS

using namespace std;

// Enum for message status
enum MessageStatus { SENT, DELIVERED, READ };

// Message structure
struct Message {
    string sender;
    string content;
    MessageStatus status;

    Message(string send, string message, MessageStatus status) : sender(send), content(message), status(status) {}

    string getStatusString() const {
        switch (status) {
            case SENT: return "Sent";
            case DELIVERED: return "Delivered";
            case READ: return "Read";
            default: return "Unknown";
        }
    }

    void markAsRead() {
        status = READ;
    }
};

// Queue using linked list
class ChatQueue {
private:
    struct Node {
        Message message;
        Node* next;
        Node(const Message message) : message(message), next(nullptr) {}
    };

    Node* front;
    Node* rear;

public:
    ChatQueue() : front(nullptr), rear(nullptr) {}

    ~ChatQueue() {
        while(front != nullptr) {
            Node* temp = front;
            front = front->next;
            delete temp;
        }
    }

    void enqueue(string sender, string recipient, string content) {
        Node* newNode = new Node(Message(sender, content, SENT));
        if(rear == nullptr) {
            front = rear = newNode;
        } 
        else {
            rear->next = newNode;
            rear = newNode;
        }
        cout<<"Message sent successfully.\n";

        // Saving message to the recipient's file
        ofstream chatFile(recipient + "_chat_history.txt", ios::app); // Save messages in the recipient's file
        chatFile << sender << ": " << content << " | Status: " << newNode->message.getStatusString() << "\n";
        chatFile.close();
    }

    void displayChat(const string& user) const {
        ifstream chatFile(user + "_chat_history.txt");
        if (!chatFile.is_open()) {
            cout << "No messages or unable to open chat history for " << user << ".\n";
            return;
        }

        cout << "\n--- " << user << "'s Chat History ---\n";
        string line;
        while (getline(chatFile, line)) {
            cout << line << "\n";
        }
        chatFile.close();
    }

    void searchMessages(const string& user, const string& keyword) const {
        ifstream chatFile(user + "_chat_history.txt");
        if(!chatFile.is_open()) {
            cout << "No messages or unable to open chat history for " << user << ".\n";
            return;
        }

        cout << "\n--- Search Results for \"" << keyword << "\" ---\n";
        string line;
        bool found = false;
        while(getline(chatFile, line)) {
            if(line.find(keyword) != string::npos) {
                cout << line << "\n";
                found = true;
            }
        }
        if(!found) {
            cout<<"No messages found for the keyword \"" << keyword << "\".\n";
        }
        chatFile.close();
    }

void markMessagesAsRead(string user) {
    ifstream chatFile(user + "_chat_history.txt");
    if (!chatFile.is_open()) {
        cout << "No messages to mark as read for " << user << ".\n";
        return;
    }

    // Read and modify the messages
    string line;
    string updatedMessages = "";
    while (getline(chatFile, line)) {
        size_t pos = line.find(" | Status: ");
        if (pos != string::npos) {
            string messageStatus = line.substr(pos + 10); // Extract the current status (Sent/Delivered/Read)
            if (messageStatus == "Sent") {
                // Update status to "Read"
                size_t lastColon = line.rfind(":");
                line.replace(lastColon + 2, string::npos, "Read");
            }
        }
        updatedMessages += line + "\n";
    }

    chatFile.close();

    // Write back the updated messages
    ofstream outFile(user + "_chat_history.txt");
    outFile << updatedMessages;
    outFile.close();

    cout << "All messages marked as read.\n";
}

    //     chatFile.close();

    //     // Write back the updated messages
    //     ofstream outFile(user + "_chat_history.txt");
    //     outFile << updatedMessages;
    //     outFile.close();

    //     cout << "All messages marked as read.\n";
    // }
};

// Helper function for sending a message
void sendMessage(ChatQueue& chat, const string& sender) {
    string recipient, content;
    cout << "Enter the recipient's username: ";
    cin >> recipient;

    cout << "Enter your message content: ";
    cin.ignore(); // Clear input buffer
    getline(cin, content);

    chat.enqueue(sender, recipient, content);
}

// Login system
class signup {
protected:
    string username;
    string password;

public:
    signup() : username("no name"), password("") {}

    void print() {
        system("clear");
        cout << "\n\n\n\n\n\t\t\t\t\t   ********************************************\n"; 
        cout << "\t\t\t\t\t   *         Creating a new Account           *\n";
        cout << "\t\t\t\t\t   *      Enter Your Username And Password    *\n";
        cout << "\t\t\t\t\t   ********************************************\n";
        cout << "Enter Username: ";
        cin >> username;
        cout << "Enter Password: ";
        cin >> password;

        ofstream out("logins.txt", ios::app); // Consistent filename
        out << username << " " << password << endl;
        out.close();
        cout << "Account successfully created!\n";
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
    
        if(in.is_open()) {
            while (in >> storedname >> storedpass) {
                if (storedname == name && storedpass == pass) {
                    found = true;
                    break;
                }
            }
            in.close();
    
            if(found) {
                cout << "Login successful!\n";
            } else {
                cout << "ERROR! Try Again.\n";
                goto q;
            }
        }
    }
};

// Admin system 

class admin_signin : protected signup {
public:
    void enterry() {
        string name, pass;

        while (true) {
            system("clear");
            cout << "\n\n--- Administrator Login ---\n";
            cout << "Enter Username: ";
            cin >> name;

            cout << "Enter Password: ";
            cin >> pass;

            if (name == "Musa" && pass == "0000") {
                cout << "Administrator Login Successful!\n";
                system("clear");
                adminMenu();
                return;
            } else {
                cout << "Invalid admin credentials. Try again.\n";
            }
        }
    }

    void adminMenu() {
        int choice;
        while (true) {
            cout << "\n--- Administrator Menu ---\n";
            cout << "1. View All Users\n";
            cout << "2. Delete a User\n";
            cout << "3. View Chat History\n";
            cout << "4. Exit to Main Menu\n";
            cout << "Enter your choice: ";
            cin >> choice;

            switch (choice) {
                case 1:
                    viewAllUsers();
                    break;
                case 2:
                    deleteUser();
                    break;
                case 3:
                    viewChatHistory(); 
                    break;
                case 4:
                    return; 
                default:
                    cout << "Invalid choice. Try again.\n";
            }
        }
    }

    void viewAllUsers() {
        ifstream in("logins.txt");
        string name, pass;

        cout << "\n--- Registered Users ---\n";
        if (in.is_open()) {
            while (in >> name >> pass) {
                cout << "Username: " << name << "\n";
            }
            in.close();
        } else {
            cout << "Unable to open user file.\n";
        }
    }

    void deleteUser() {
        string usernameToDelete;
        cout << "Enter the username to delete: ";
        cin >> usernameToDelete;

        ifstream in("logins.txt");
        ofstream temp("temp.txt");
        string name, pass;
        bool deleted = false;

        if (in.is_open() && temp.is_open()) {
            while (in >> name >> pass) {
                if (name != usernameToDelete) {
                    temp << name << " " << pass << "\n";
                } else {
                    deleted = true;
                }
            }
            in.close();
            temp.close();

            remove("logins.txt");
            rename("temp.txt", "logins.txt");

            if (deleted) {
                cout << "User \"" << usernameToDelete << "\" deleted successfully.\n";
            } else {
                cout << "User not found.\n";
            }
        } else {
            cout << "Error opening file.\n";
        }
    }

void viewChatHistory() {
    string username;
    cout << "Enter the username whose chat history you want to view: ";
    cin >> username;

    ifstream chatFile(username + "_chat_history.txt");
    if (!chatFile.is_open()) {
        cout << "No chat history found or unable to open the file for " << username << ".\n";
        return;
    }

    cout << "\n--- Chat History for " << username << " ---\n";
    string line;
    int index = 1;
    while (getline(chatFile, line)) {
        cout << index++ << ". " << line << "\n";
    }
    chatFile.close();
}

};

int main() {
    ChatQueue chat;
    string sender;
    int choice;
    int options;

    cout << "\n\n\n\t\t\t\t ---------------------------------------------";
    cout << "\n\t\t\t\t *********************************************";
    cout << "\n\t\t\t\t                                             ";
    cout << "\n\t\t\t\t                WELCOME TO                   ";
    cout << "\n\t\t\t\t                                             ";
    cout << "\n\t\t\t\t                 MU CHAT                     ";
    cout << "\n\t\t\t\t                                             ";
    cout << "\n\t\t\t\t ********************************************";
    cout << "\n\t\t\t\t ---------------------------------------------\n\n";

    cout<<"Clearing the screen in 3 seconds...\n";
    sleep(3);
    system("clear");

    // Login System Object Declaration    
    signin sn;
    signup sp;
    admin_signin admin;

    int option;
    while (true) {
        cout << "\t\t\t______________________________________________________________________\n\n\n";
        cout << "\t\t\t                          Welcome To Login Page                        \n\n\n";
        cout << "\t\t\t _____________________________    MENU    ____________________________  \n\n";
        cout << "\t\t\t                                                                         \n\n";


        cout << "\t\t\t\t\t _____________________________________\n";
        cout << "\t\t\t\t\t|                                     |\n";
        cout << "\t\t\t\t\t|        Press 1 For Admin            |\n ";
        cout << "\t\t\t\t\t|        Press 2 For User             |\n";
        cout << "\t\t\t\t\t|_____________________________________|\n\n\n";
        cout << "\t\t\t\t\t Enter Your Choice :  ";
        cin >> options;

        switch (options) {
            case 1:
                cout << "Admin Login\n";
                admin.enterry();                system("clear");
                break;
            case 2:
                system("clear");
                cout << "\t\t\t\t\t _____________________________________\n";
                cout << "\t\t\t\t\t|                                     |\n";
                cout << "\t\t\t\t\t|        Press 1 For Sign In          |\n ";
                cout << "\t\t\t\t\t|        Press 2 For Sign Up          |\n";
                cout << "\t\t\t\t\t|        Press 3 For Exit             |\n";
                cout << "\t\t\t\t\t|_____________________________________|\n\n\n";
                cout << "\t\t\t\t\t Enter Your Choice :  ";
                cin >> option;

                switch(option) {
                    case 1:
                        sn.enterr();
                        system("clear");
                        cout << "Enter your username to start chatting: ";
                        cin >> sender;

                        system("clear");
                        while(true) {
                            cout << "\n--- Chat Application Menu ---\n";
                            cout << "1. Send Text Message\n";
                            cout << "2. View Chat History\n";
                            cout << "3. Search Messages\n";
                            cout << "4. Mark All Messages as Read\n";
                            cout << "5. Quit\n";
                            cout << "Enter your choice: ";
                            cin >> choice;

                            switch (choice) {
                                case 1:
                                    sendMessage(chat, sender);
                                    break;                       
                                case 2:
                                    chat.displayChat(sender);
                                    break;
                                case 3: {
                                    string keyword;
                                    cout << "Enter keyword to search: ";
                                    cin.ignore();
                                    getline(cin, keyword);
                                    chat.searchMessages(sender, keyword);
                                    break;
                                }
                                case 4:
                                    chat.markMessagesAsRead(sender);
                                    system("clear");
                                    break;
                                case 5: 
                                    system("clear");
                                    cout << "\n\t\t\t\t ---------------------------------------------";
                                    cout << "\n\t\t\t\t *********************************************";
                                    cout << "\n\t\t\t\t                                              ";
                                    cout << "\n\t\t\t\t             Thanks For Coming                ";
                                    cout << "\n\t\t\t\t                                              ";
                                    cout << "\n\t\t\t\t                 MU Chat                      ";
                                    cout << "\n\t\t\t\t                                              ";
                                    cout << "\n\t\t\t\t ******************************************** ";
                                    cout << "\n\t\t\t\t ---------------------------------------------";
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
                        cout << "Invalid Choice. Please try again.\n";
                }
        }
    }
}
