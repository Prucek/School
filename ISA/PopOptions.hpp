/**
 * @file PopOptions.hpp
 * @author Peter Rucek, xrucek00
 * @date 12.10.2021
 * PopOptions class
 */

#ifndef __POPOPTIONS_HPP__
#define __POPOPTIONS_HPP__

using namespace std;
#include <iostream>
#include <getopt.h>
#include <cstring>
#include <sys/types.h>
#include <sys/stat.h>

#define MAX_PARA_LEN 12
#define OPTION_LEN 3


class PopOptions
{
    private:
        //fields
        char *authorizationFile;
        char *outputDirecory;
        int port;
        char *server;
        bool deleteFlag;
        bool newFlag;
        bool pop3s; // -T
        bool stls;  // -S
        char *tlsCertificate;
        char *tlsDirectory;

        //methods
        void FindServer(char *argv[], int argc);
        bool ArgumentParse(int argc, char* argv[]);
        bool CheckFile(char *string);
        bool CheckDir(char *string);

    public:
        PopOptions();
        char* getAuthorizationFile(){return this->authorizationFile;}
        char* getOutputDirecotry(){return this->outputDirecory;}
        char* getServer(){return this->server;}
        int getPort(){return this->port;}
        bool getPop3S(){return this->pop3s;};
        bool getStls(){return this->stls;};
        bool getDeleteFlag(){return this->deleteFlag;}
        bool getNewFlag(){return this->newFlag;}
        char* getTlsCertificate(){return this->tlsCertificate;}
        char* getTlsDirectory(){return this->tlsDirectory;}
        bool Create(char *argv[], int argc);
        

};

#endif