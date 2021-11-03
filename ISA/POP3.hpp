/**
 * @file POP3.hpp
 * @author Peter Rucek, xrucek00
 * @date 12.10.2021
 * POP3 class
 */

#ifndef __POP3_HPP__
#define __POP3_HPP__

using namespace std;
#include <fstream>
#include <vector>
#include <string>
#include <regex>
#include "openssl/bio.h"
#include "openssl/ssl.h"
#include "openssl/err.h"

#include "PopOptions.hpp"

#define MAX_PORT_LEN 16
#define MAX_NAME_LEN 256
#define BUF_SIZE 1024
#define DB_NAME ".UIDL_db" // file where UIDLs of mails are sored
#define FAILED -1 // retrun code for error

/**
 * @brief Strucutre thas represents authentication file
 */
struct AuthorizationPair
{
    string username;
    string password; 
};


/**
 * @class Provides all POP3 actions needed
 */
class POP3
{
    private:
        BIO *bio;
        AuthorizationPair pair;
        PopOptions options;
        SSL_CTX *ctx;
        char hostname[MAX_NAME_LEN];
        string message; // curent message
        SSL *ssl;
        bool stlsStarted;
        X509* cert; // provided certificate


        /**
         * @brief Provides authentication to POP server by USER and PASS
         */
        bool Authenticate();

        /**
         * @brief Downloads all mails from POP3 server innbox
         */
        int DownloadAllMails();

        /**
         * @brief Reads and parses authentication file into this->pair
         * 
         * @param fileName name/path of authentication file
         */
        bool ReadAuthFile(char *fileName);

        /**
         * @brief Creates usecured connection with POP3 server
         */
        bool ConnectionUnsecure();

        /**
         * @brief Creates encrypted POP3s connection with POP3 server
         */
        bool ConnectionSecure();

        /**
         * @brief Provides connection enctyption from unsecured connection by sending STLS 
         */
        bool ConnectionSTLS();

        /**
         * @brief Prepares this->hostname for BIO_new_connect and BIO_set_conn_hostname
         */
        void MakeHostname();

        /**
         * @brief Checks if in this->message is +OK present
         */
        bool CheckOK();

        /**
         * @brief Reads responses from POP3 server to this->message
         * 
         * @param dot delimeter which defines end of message
         */
        bool ReadMessage(string dot);

        /**
         * @brief Sends messages to POP3 server
         * 
         * @param str message to send
         */
        bool SendMessage(string str);

        /**
         * @brief Sends STAT command to POP3 server
         * 
         * @return Number of messages in inbox
         */
        int GetNumberOfMails();

        /**
         * @brief Downloads single mail from inbox
         * 
         * @param messageNumber is number of mail in inbox
         */
        bool DownloadMail(int messageNumber);

        /**
         * @brief Gets UIDL of certian mail
         * 
         * @param messageNumber is number of mail in inbox
         */
        bool GetUIDLofMessage(int messageNumber);

        /**
         * @brief Adds UIDL of messageNumber into UIDL database if new
         * 
         * @param messageNumber is number of mail in inbox
         */
        bool AddUIDLentry(int messageNumber);

        /**
         * @brief Checks whether message to be downloaded is new (not in database)
         * 
         * @param uidl new mail UIDL
         */
        bool IsMessageNew(string uidl);

        /**
         * @brief Downloads only new (not in database) messages from inbox
         */
        int DownloadOnlyNew();

        /**
         * @brief Checks if we provided correct SSL certificate
         */
        bool CheckCertificate();

        /**
         * @brief Deletes all messages in inbox
         * 
         * @return Number of deleted messages
         */
        int Delete();

        /**
         * @brief Properly ends communication with POP3 server
         */
        bool Quit();

    public:

        POP3(PopOptions options);
        ~POP3();

        /**
         * @brief Executes either downloading messages or downloading only new messages or 
         *        deleting all messages from inbox - specified by the program options
         * @return number of downloaded/deleted messages
         * 
         * @param onlyNew will be set to true if -n flag is present
         * 
         * @param deleteFlag will be set to true if -d flag is present
         * 
         */
        int Execute(bool *onlyNew, bool *deleteFlag);
};

#endif