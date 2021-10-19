/**
 * @file POP3.hpp
 * @author Peter Rucek, xrucek00
 * @date 12.10.2021
 * POP3 class
 */

#ifndef __POP3_HPP__
#define __POP3_HPP__

using namespace std;
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "openssl/bio.h"
#include "openssl/ssl.h"
#include "openssl/err.h"

#include "PopOptions.hpp"

#define MAX_PORT_LEN 16
#define MAX_NAME_LEN 256
#define BUF_SIZE 1024

struct AuthorizationPair
{
    string username;
    string password; 
};

class POP3
{
    private:
        BIO *bio;
        AuthorizationPair pair;
        PopOptions options;
        SSL_CTX *ctx;
        char hostname[MAX_NAME_LEN];
        string message;

        bool ReadAuthFile(char *file_name);
        bool ConnectionUnsecure();
        bool ConnectionSecure();  // POP3s
        void MakeHostname();
        bool CheckOK();
        bool ReadMessage(string dot);
        bool SendMessage(string str);
        int GetNumberOfMails();
        bool DownloadMail();

    public:
        POP3(PopOptions options);
        ~POP3();
        bool Authenticate();
        bool DownloadAllMails();
};

#endif