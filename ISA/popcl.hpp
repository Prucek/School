/**
 * @file popcl.hpp
 * @author Peter Rucek, xrucek00
 * @date 1.10.2021
 * POP3 client
 */

#ifndef __POPCL_HPP__
#define __POPCL_HPP__

#include <iostream>
#include <getopt.h>
#include <cstring>
#include <fstream>
#include <vector>
// #include <pcap.h>
// #include <arpa/inet.h>
// #include <net/ethernet.h>
// #include <netinet/ip_icmp.h>
// #include <netinet/udp.h>
// #include <netinet/tcp.h>
// #include <netinet/ip.h>
// #include <netinet/ip6.h>
// #include <linux/if_arp.h>

#include <sys/types.h>
#include <sys/stat.h>
/* OpenSSL headers */

# include  "openssl/bio.h"
# include  "openssl/ssl.h"
# include  "openssl/err.h"


using namespace std;


#define OK 0        // exit code for everything being OK
#define BIO_ERR 1   // exit code for connection being aborted
#define ARG_ERR  2  // exit code for arguments being wrong
#define AUT_ERR  3  // exit code for authorization being wrong


struct PopOptions
{
    char *authorization_file;
    char *output_direcory;
    int port = 110; //default POP3
    char *server;
    bool delete_messages = false;
    bool only_new = false;
    bool pop3s = false; // -T
    bool stls = false;  // -S
    char *tls_certificate = NULL;
    char *tls_directory = NULL;
};

struct AuthorizationPair
{
    string username;
    string password; 
};

bool find_server(char **server, char *argv[], int argc);
bool arg_parse(int argc, char* argv[], PopOptions *options);
bool check_dir(char *string);
bool check_file(char *string);

BIO* connection_unsecure(PopOptions *options);
AuthorizationPair* read_authfile(char *file_name);
bool authenticate(BIO *bio, AuthorizationPair* pair);
bool send_message(BIO *bio, string str);
string read_message(BIO *bio);

#endif