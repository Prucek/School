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
#include <cstring>
#include <sys/types.h>
#include <sys/stat.h>

#define MAX_PARA_LEN 12
#define OPTION_LEN 3
#define USAGE "Usage: "\
            " popcl <server> [-p <port>] [-T|-S [-c <certfile>] [-C <certaddr>]]"\
            " [-d] [-n] -a <auth_file> -o <out_dir>"


class PopOptions
{
    private:
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

        /**
         * @brief Defines which of the option is server name
         */
        void FindServer(int argc, char *argv[]);

        /**
         * @brief Parses CL options and arguments
         */
        bool ArgumentParse(int argc, char* argv[]);

        /**
         * @brief Checks if file exists
         * 
         * @param string name/path of file
         */
        bool CheckFile(char *string);

        /**
         * @brief Checks if directory exists
         * 
         * @param string name/path of directory
         */
        bool CheckDir(char *string);

    public:
        PopOptions();

        // getters
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

        /**
         * @brief Initializes all fields and checks whole syntax of CL arguments and options
         */
        bool Create(int argc, char *argv[]);
};

#endif