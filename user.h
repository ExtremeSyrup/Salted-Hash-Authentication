/*
    @author     : Daathwinaagh
    @Role       : Student at IIITDM Kancheepuram, Chennai
    @Degree     : Bachelor of Technology (BTech)
    @Dept       : Computer Science and Engineering
    @project    : Salted Hash Authentication Implementation with MySQL and C++
*/

#include <iostream>
#include <cstring>
#include <iomanip>
#include <random>
#include <mysql_driver.h>
#include <mysql_connection.h>
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cryptopp/cryptlib.h>
#include <cryptopp/sha.h>
#include <cryptopp/hex.h>

using namespace std;
using namespace CryptoPP;

class User {
    private:
        bool isLoggedIn;
        string userName;
        string password;

        sql::mysql::MySQL_Driver *driver;
        sql::Connection *con;
        sql::Statement *stmt;
        sql::ResultSet *res;

        string hashPassword(string pwd);
        bool validatePassword(string pwd);
        bool validateUsername(string uname);

        // salt generation algorithm
        string generateSalt(int len) {
            string charset;
            charset = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!@#$%^&*()";

            random_device rd;
            mt19937 gen(rd());
            uniform_int_distribution<int> dist(0, charset.length() - 1);

            string randomString;
            for (int i = 0; i < len; ++i) {
                randomString += charset[dist(gen)];
            }

            return randomString;
        }

    public:
        User() {
            isLoggedIn = false;
        }

        ~User() {
            cout << "Logged out successfully\n";
            isLoggedIn = false;
        }

        bool establish_connection(string SQL_HOSTNAME, string SQL_USERNAME, string SQL_PASSWORD, string SQL_DATABASE) {
            string sql;
            driver = sql::mysql::get_mysql_driver_instance();
            con = driver->connect(SQL_HOSTNAME, SQL_USERNAME, SQL_PASSWORD);
            con->setSchema(SQL_DATABASE);
            return con->isValid();
        }

        bool login_user(string uname, string pwd) {
            string sql_query = "SELECT salt FROM users WHERE username = '" + uname + "'";
            stmt = con->createStatement();
            res = stmt->executeQuery(sql_query);
            string retrieved_salt;
            if(res->next()) {
                retrieved_salt = res->getString("salt");
            }
            cout << retrieved_salt << "\n";
            string hashed = hashPassword(pwd+retrieved_salt);

            sql_query = "SELECT password FROM users WHERE username = '" + uname + "'";
            stmt = con->createStatement();
            res = stmt->executeQuery(sql_query);

            string pwd_hash;
            if(res->next()) {
                pwd_hash = res->getString("password");
            }
            isLoggedIn = (hashed == pwd_hash);
            return hashed == pwd_hash;
        }

        bool register_user(string uname, string pwd) {
            bool valid_uname = validateUsername(uname);
            bool valid_pwd = validatePassword(pwd);

            if(valid_uname && valid_pwd) {
                string salt = generateSalt(10);
                pwd = hashPassword(pwd+salt);
                string sql_query = "INSERT INTO users (username, password, salt) VALUES ('" + uname + "', '" + pwd + "', '" + salt + "')";
                stmt = con->createStatement();
                stmt->execute(sql_query);
                return true;
            }
            return false;
        }

        bool isLoggedin() {
            return isLoggedIn;
        }
};

bool User::validateUsername(string uname) {
    if((int)uname.size() == 0) return false;
    bool username_set = false;
    string sql = "SELECT COUNT(*) AS cnt FROM users WHERE username = '" + uname + "'";
    stmt = con->createStatement();
    res = stmt->executeQuery(sql);

    if(res->next()) {
        int cnt = res->getInt("cnt");
        if(cnt > 0) return false;
        username_set = true;
    }
    return username_set;
}

bool User::validatePassword(string pwd) {
    // Atleast one Small case, upper case, numerical, from [%, &, @, $]
    if((int)pwd.size() == 0) return false;
    bool caps = false, smalls = false, spchar = false;
    for(int i=0; i<(int)pwd.size(); i++) {
        if(65 <= (int)pwd[i] && (int)pwd[i] <= 90) caps = true;
        if(97 <= (int)pwd[i] && (int)pwd[i] <= 122) smalls = true;
        if(pwd[i] == '%' || pwd[i] == '&' || pwd[i] == '@' || pwd[i] == '$') spchar = true;
    }
    return caps && smalls & spchar;
}

string User::hashPassword(string pwd) {
    SHA256 hash;
    string digest;

    StringSource(pwd, true,
        new HashFilter( hash,
            new HexEncoder(
                new StringSink(digest)
            )
        )
    );
    return digest;
}
