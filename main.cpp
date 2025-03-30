#include <iostream>
#include <mysql/mysql.h>
#include <string>

using namespace std;

class Database {
private:
    MYSQL* conn;
    MYSQL_ROW row;
    MYSQL_RES* res;

public:
    Database() {
        conn = mysql_init(0);
        if (!conn) {
            cout<<"init failed"<<endl;
        }
        conn = mysql_real_connect(conn, "localhost", "root", "1234", "user_db", 33060, NULL, 0);
        if (conn) {
            cout << "Database Connected Successfully!" << endl;
        } else {
            cout << "Database Connection Failed!" << endl;
            cout << "Connection failed: " << mysql_error(conn) << endl;
            exit(1);
        }
    }

    MYSQL* getConnection() {
        return conn;
    }

    ~Database() {
        mysql_close(conn);
    }
};

class User {
private:
    string username, password;
    MYSQL* conn;

public:
    User(MYSQL* dbConn) : conn(dbConn) {}

    void registerUser() {
        cout << "Enter username: ";
        cin >> username;
        cout << "Enter password: ";
        cin >> password;

        string query = "INSERT INTO users (username, password) VALUES ('" + username + "', '" + password + "')";
        if (mysql_query(conn, query.c_str()) == 0) {
            cout << "Registration successful!" << endl;
        } else {
            cout << "Registration failed! Username might be taken." << endl;
        }
    }

    bool loginUser() {
        cout << "Enter username: ";
        cin >> username;
        cout << "Enter password: ";
        cin >> password;

        string query = "SELECT * FROM users WHERE username='" + username + "' AND password='" + password + "'";
        if (mysql_query(conn, query.c_str()) == 0) {
            MYSQL_RES* res = mysql_store_result(conn);
            if (mysql_num_rows(res) > 0) {
                cout << "Login successful!" << endl;
                mysql_free_result(res);
                return true;
            } else {
                cout << "Invalid username or password!" << endl;
            }
            mysql_free_result(res);
        } else {
            cout << "Query failed!" << endl;
        }
        return false;
    }
};

int main() {
    Database db;
    User user(db.getConnection());

    int choice;
    do {
        cout << "\n1. Register\n2. Login\n3. Exit\nEnter choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                user.registerUser();
                break;
            case 2:
                user.loginUser();
                break;
            case 3:
                cout << "Exiting..." << endl;
                break;
            default:
                cout << "Invalid choice!" << endl;
        }
    } while (choice != 3);

    return 0;
}
