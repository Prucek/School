/**
 * @file ipk-sniffer.hpp
 * @author Peter Rucek, xrucek00
 * @date 18.4.2021
 * Header for packet sniffer
 */

#ifndef __IPK_SNIFFER_HPP__
#define __IPK_SNIFFER_HPP__

#include <iostream>
#include <getopt.h>
#include <cstring>
#include <pcap.h>
#include <arpa/inet.h>
#include <net/ethernet.h>
#include <netinet/ip_icmp.h>
#include <netinet/udp.h>
#include <netinet/tcp.h>
#include <netinet/ip.h>
#include <netinet/ip6.h>
#include <linux/if_arp.h>

#include "print.hpp"

// for getopt
#define no_argument 0
#define required_argument 1

#define OK 0        // exit code that evrything went OK
#define PCAP_ERR 1  // exit code that something from PCAP library failed
#define ARG_ERR  2  // exit code that arguments were wrong

#define IS_ALL ((tcp || udp || arp || icmp)? false : true)

/**
 * Checks program arguments and sets global variables if needed
 * @returns true if arguments are correct
 * @param argc is number of program arguments
 * @param argv are program arguments
 */
bool arg_parse(int argc, char* argv[]);

/**
 * Callback function for pcap_loop
 * Packets are print here
 * @param buffer is data to be printed
 */
void process_packet(u_char *args, const struct pcap_pkthdr *header, const u_char *buffer);

#endif