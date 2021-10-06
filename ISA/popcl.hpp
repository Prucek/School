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

using std::cout;
using std::cerr;
using std::endl;


#define OK 0        // exit code that evrything went OK
// #define PCAP_ERR 1  // exit code that something from PCAP library failed
#define ARG_ERR  2  // exit code that arguments were wrong


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

bool find_server(char **server, char *argv[], int argc);
bool arg_parse(int argc, char* argv[], PopOptions *options);
bool check_dir(char *string);
bool check_file(char *string);

#endif