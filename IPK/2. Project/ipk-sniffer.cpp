/**
 * @file ipk-sniffer.cpp
 * @author Peter Rucek, xrucek00
 * @date 16.4.2021
 * Packet sniffer
 */

#include "ipk-sniffer.hpp"

// Global variables
unsigned port = 0;
unsigned num  = 0;
char *interface = nullptr;
bool tcp = false;
bool udp = false;
bool arp = false;
bool icmp = false;

int main(int argc, char * argv[])
{
    if (!arg_parse(argc, argv))
    {
        cerr << "ERROR: Wrong arguments" << endl;
        return ARG_ERR;
    }

    if (num == 0) 
    {
        num = 1; // if num not set print just 1 packet
    }

    pcap_if_t* devices = nullptr;
    char errbuf[PCAP_ERRBUF_SIZE]; // for printing errors
    if(pcap_findalldevs(&devices, errbuf) != 0) 
    {
        cerr << "ERROR: pcap failed: " << errbuf << endl;
        return PCAP_ERR;
    }

    // Loop thru available devices/interafces
    pcap_if_t* device = nullptr;
    for (pcap_if_t* curr_device = devices ; curr_device; curr_device = curr_device->next)
    {
        if (!interface) 
        {
            cout << curr_device->name << endl;
        }
        else if (strcmp(interface, curr_device->name) == 0)
        {
            device = curr_device;
            break;
        }
    }
    if (!interface)
    {
        pcap_freealldevs(devices);
        if (interface) 
        {
            free(interface);
        }
        return OK;
    }
    if (!device)
    {
        pcap_freealldevs(devices);
        if(interface) free(interface);
        cerr << "ERROR: Device not found" << endl;
        return ARG_ERR;
    }
    if (interface) 
    {
        free(interface);
    }

    // Open device for packet capturing
    pcap_t* pcap = pcap_open_live(device->name, BUFSIZ, 1, 0, errbuf);
    if (!pcap) 
    {
        pcap_freealldevs(devices);
        cerr << "ERROR: pcap failed: " << errbuf << endl;
        return PCAP_ERR;
    }
    pcap_freealldevs(devices);

    // Filter port
    if (port != 0 && (IS_ALL || tcp || udp) ) // Port filtering only if upot tcp ( would not work on arp and icmp)
    {
        std::string s = "port " + std::to_string(port);
        struct bpf_program pgm;
        if (pcap_compile(pcap, &pgm, s.c_str(), 1, PCAP_NETMASK_UNKNOWN) == PCAP_ERROR)
        {
            pcap_close(pcap);
            cerr << "ERROR: pcap failed: " << pcap_geterr(pcap) << endl;
            return PCAP_ERR;
        }
        if (pcap_setfilter(pcap, &pgm) == PCAP_ERROR) 
        {
            pcap_close(pcap);
            cerr << "ERROR: pcap failed: " << pcap_geterr(pcap) << endl;
            return PCAP_ERR;
        }
    }


    // struct pcap_pkthdr header;	// The header that pcap gives us

	// const u_char *packet;		// The actual packet
    // const struct sniff_ethernet *ethernet; // The ethernet header
    // TODO IPv6 MATBE ?
    // packet = pcap_next(pcap, &header);

    // Capture num packets
    while(num != 0)
    {
        if (pcap_loop(pcap, 1, process_packet, nullptr) == PCAP_ERROR)
        {
            pcap_close(pcap);
            cerr << "ERROR: pcap failed: " << pcap_geterr(pcap) << endl;
            return PCAP_ERR;
        }
    }

    pcap_close(pcap);

    return OK;
}

void process_packet(u_char *args, const struct pcap_pkthdr *header, const u_char *buffer)
{
    (void) args;
    int size = header->len;

    // Get the IP Header part of this packet , excluding the ethernet header
    struct ether_header *eth = (struct ether_header*) (buffer);

    if (ntohs(eth->ether_type) == ETHERTYPE_ARP)
    {
        struct arphdr *arph = (struct arphdr*) (buffer + sizeof(struct ethhdr));
        if (IS_ALL || arp)
        {
            int add_size = (int)arph->ar_pln * __CHAR_BIT__;
            char *address = new char[add_size];

            char *p = (char *) (buffer + sizeof(struct ethhdr) + sizeof(struct arphdr) + (int)arph->ar_hln); // Shift to first address
            for(int i = 0; i < add_size; i++)
                address[i] = p[i];

            cout << endl;
            cout << "ARP" << endl;
            print_time_rfc3339();
            printf ("%s > ",inet_ntoa(*((struct in_addr*)address))); // Print as IP adress

            p +=  arph->ar_hln + arph->ar_pln; // Shift to second address
            for(int i = 0; i < add_size; i++)
                address[i] = p[i];

            printf ("%s",inet_ntoa(*((struct in_addr*)address))); // Print as IP adress
            cout << ", length "<< size << " bytes"<< endl;
            cout << endl;
            print_data(buffer, size);

            num--;
            delete[] address; 
        }
        return;
    }
     // TODO ETHERTYPE_IPV6		0x86dd	
    if (ntohs(eth->ether_type) == ETHERTYPE_IP)
    {
        struct iphdr *iph = (struct iphdr*)(buffer + sizeof(struct ethhdr));

        // The IHL field contains the size of the IPv4 header, 
        // it has 4 bits that specify the number of 32-bit words in the header (5 min - 15 max)
        u_short ip_header_len = (iph->ihl) * 4;

        struct sockaddr_in source,dest;
        memset(&source, 0, sizeof(source));
        source.sin_addr.s_addr = iph->saddr; // source address

        memset(&dest, 0, sizeof(dest));
        dest.sin_addr.s_addr = iph->daddr; // destination address

        switch (iph->protocol)
        {
            case 1: // ICMP Protocol
            {
                if (IS_ALL || icmp)
                {
                    cout << endl;
                    cout << "ICMP" << endl;
                    print_time_rfc3339();
                    cout << inet_ntoa(source.sin_addr) << " > " << inet_ntoa(dest.sin_addr);
                    cout << ", length "<< size << " bytes"<< endl;
                    cout << endl;
                    print_data(buffer, size);
                    num--;
                }
                break;
            }
            case 6: // TCP Protocol
            {
                if (IS_ALL || tcp)
                {
                    const struct tcphdr* tcp_header = (struct tcphdr*)(buffer + sizeof(struct ethhdr) + ip_header_len);
                    cout << endl;
                    cout << "TCP" << endl;
                    print_time_rfc3339();
                    cout << inet_ntoa(source.sin_addr);
                    cout << " : " << ntohs(tcp_header->source);
                    cout << " > " << inet_ntoa(dest.sin_addr);
                    cout << " : " << ntohs(tcp_header->dest);
                    cout << ", length "<< size << " bytes"<< endl;
                    cout << endl;
                    print_data(buffer, size);
                    num--;
                }
                break;
            }
            case 17: // UDP Protocol
            {
                if (IS_ALL || udp)
                {
                    const struct udphdr* udp_header = (struct udphdr*)(buffer + sizeof(struct ethhdr) + ip_header_len);
                    cout << endl;
                    cout << "UDP" << endl;
                    print_time_rfc3339();
                    cout << inet_ntoa(source.sin_addr);
                    cout << " : " << ntohs(udp_header->source);
                    cout << " > " << inet_ntoa(dest.sin_addr);
                    cout << " : " << ntohs(udp_header->dest);
                    cout << ", length "<< size << " bytes"<< endl;
                    cout << endl;
                    print_data(buffer, size);
                    num--;
                }
                break;
            }
            default: // Other Protocols
            {
                if (IS_ALL)
                {
                    cout << endl;
                    cout << "ELSE" << endl;
                    print_time_rfc3339();
                    cout << inet_ntoa(source.sin_addr);
                    cout <<  " > " << inet_ntoa(dest.sin_addr);
                    cout << ", length "<< size << " bytes"<< endl;
                    cout << endl;
                    print_data(buffer, size);
                    num--;
                }
                break;
            }
        }
    }
}

bool arg_parse(int argc, char* argv[])
{
    const struct option longopts[] =
    {
        {"interface", required_argument, 0, 'i'},
        {"tcp",             no_argument, 0, 't'},
        {"udp",             no_argument, 0, 'u'},
        {"arp",             no_argument, 0, 'a'},
        {"icmp",            no_argument, 0, 'c'},
        {0,0,0,0},
    };

    int index;
    int iarg = 0;
    bool is_i = false;

    while(iarg != -1)
    {                                    // :D
        iarg = getopt_long(argc, argv, "p:uti::n:", longopts, &index);

        switch (iarg)
        {
            case 'i':
            {
                is_i = true;
                char *value = nullptr;
                for (int i = 0; i < argc - 1 ; i++)
                {
                    if (strcmp(argv[i], "-i")  == 0|| strcmp(argv[i], "--interface") == 0)
                        if (argv[i+1][0] != '-')
                        {
                            value = argv[i+1];
                        }
                }
                if (value)
                {
                    interface = (char *)malloc(strlen(value) + 1);
                    if (!interface) 
                    {
                        return false;
                    }

                    strcpy(interface, value);
                }
                break;
            }
            case 'p':
            {
                char *endptr = nullptr;

                port = (unsigned)strtoul(optarg, &endptr, 10);
                if (strcmp(endptr, "")) 
                {
                    return false;
                }
                if (port > 65535 || optarg[0] == '-') 
                {
                    return false;
                }
                break;
            }
            case 'n':
            {
                char *endptr = nullptr;

                num = (unsigned)strtoul(optarg, &endptr, 10);
                if (strcmp(endptr, "")) 
                {
                    return false;
                }
                if (optarg[0] == '-') 
                {
                    return false;
                }
                break;
            }
            case 't':
                tcp = true;
                break;
            case 'u':
                udp = true;
                break;
            case 'a':
                arp = true;
                break;
            case 'c':
                icmp = true;
                break;
            case '?':
                cout << "Usage:" << endl;
                cout << "  ipk-sniffer [-i rozhraní | --interface rozhraní]"
                    " {-p ­­port} {[--tcp|-t] [--udp|-u] [--arp] [--icmp] } {-n num}" << endl;
                return false;

        } // end switch
    } // end while
    return is_i;
}
