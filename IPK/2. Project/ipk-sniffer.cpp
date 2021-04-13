// /ipk-sniffer [-i rozhraní | --interface rozhraní] {-p ­­port} {[--tcp|-t] [--udp|-u] [--arp] [--icmp] } {-n num}

// kde
// -i eth0 (právě jedno rozhraní, na kterém se bude poslouchat. Nebude-li tento parametr uveden, či bude-li uvedené jen -i bez hodnoty, vypíše se seznam aktivních rozhraní)
// -p 23 (bude filtrování paketů na daném rozhraní podle portu; nebude-li tento parametr uveden, uvažují se všechny porty; pokud je parametr uveden, může se daný port vyskytnout jak v source, tak v destination části)
// -t nebo --tcp (bude zobrazovat pouze TCP pakety)
// -u nebo --udp (bude zobrazovat pouze UDP pakety)
// --icmp (bude zobrazovat pouze ICMPv4 a ICMPv6 pakety)
// --arp (bude zobrazovat pouze ARP rámce)
// Pokud nebudou konkrétní protokoly specifikovány, uvažují se k tisknutí všechny (tj. veškerý obsah, nehledě na protokol)
// -n 10 (určuje počet paketů, které se mají zobrazit; pokud není uvedeno, uvažujte zobrazení pouze jednoho paketu)
// argumenty mohou být v libovolném pořadí

// Formát výstupu:
// čas IP : port > IP : port, length délka

// offset_vypsaných_bajtů:  výpis_bajtů_hexa výpis_bajtů_ASCII

// přičemž:

// čas je ve formátu dle RFC3339
// délka je v bytech
// (takto vypíšete úplně celý paket)

// Příklady volání:

// ./ipk-sniffer -i eth0 -p 23 --tcp -n 2
// ./ipk-sniffer -i eth0 --udp
// ./ipk-sniffer -i eth0 -n 10      
// ./ipk-sniffer -i eth0 -p 22 --tcp --udp --icmp --arp   .... stejné jako:
// ./ipk-sniffer -i eth0 -p 22
// ./ipk-sniffer -i eth0

// Příklady výstupu:

// ./ipk-sniffer -i

// lo0
// eth0

// ./ipk-sniffer -i eth0

// 2021-03-19T18:42:52.362+01:00 147.229.13.223 : 4093 > 10.10.10.56 : 80, length 112 bytes

// 0x0000:  00 19 d1 f7 be e5 00 04  96 1d 34 20 08 00 45 00  ........ ..4 ..
// 0x0010:  05 a0 52 5b 40 00 36 06  5b db d9 43 16 8c 93 e5  ..R[@.6. [..C....
// 0x0020:  0d 6d 00 50 0d fb 3d cd  0a ed 41 d1 a4 ff 50 18  .m.P..=. ..A...P.

// 0x0030:  19 20 c7 cd 00 00 99 17  f1 60 7a bc 1f 97 2e b7  . ...... .`z.....
// 0x0040:  a1 18 f4 0b 5a ff 5f ac 07 71 a8 ac 54 67 3b 39  ....Z._. .q..Tg;9
// 0x0050:  4e 31 c5 5c 5f b5 37 ed  bd 66 ee ea b1 2b 0c 26  N1.\_.7. .f...+.&
// 0x0060:  98 9d b8 c8 00 80 0c 57  61 87 b0 cd 08 80 00 a1  .......W a.......

#include <iostream>
#include <getopt.h>
#include <cstring>
#include <pcap.h>

#include<sys/socket.h>
#include<arpa/inet.h> // for inet_ntoa()
#include<net/ethernet.h>
#include<netinet/ip_icmp.h>	//Provides declarations for icmp header
#include<netinet/udp.h>	//Provides declarations for udp header
#include<netinet/tcp.h>	//Provides declarations for tcp header
#include<netinet/ip.h>	//Provides declarations for ip header

using std::cout;
using std::endl;

#define no_argument 0
#define required_argument 1 

bool arg_parse(int argc, char* argv[], unsigned *port, unsigned *num, char **interface, bool *tcp, bool *udp, bool *arp, bool *icmp);
void process_packet(u_char *args, const struct pcap_pkthdr *header, const u_char *buffer);
void PrintData (const u_char * data , int Size);

int tcp=0,udp=0,icmp=0,others=0,igmp=0,total=0;	

int main(int argc, char * argv[])
{
    unsigned *port = nullptr;
    unsigned *num  = nullptr;
    char *interface = nullptr;
    bool tcp = false;
    bool udp = false;
    bool arp = false;
    bool icmp = false;
    if (!arg_parse(argc,argv, port, num, &interface, &tcp, &udp, &arp, &icmp))
    {
        cout << "ERROR: Wrong arguments" << endl;
        return 1;
    }


    pcap_if_t* devices = nullptr;
    char errbuf[PCAP_ERRBUF_SIZE];
    if(pcap_findalldevs(&devices, errbuf) != 0) 
    {
        cout << "ERROR: pcap failed: " << errbuf << endl;
        return 1;
    }

    pcap_if_t* device = nullptr;
    for (pcap_if_t* curr_device = devices ; curr_device; curr_device = curr_device->next)
    {
        if (!interface) cout << curr_device->name << endl;
        else if (strcmp(interface, curr_device->name) == 0)
        {
            device = curr_device;
            break;
        }
    }
    if (!interface)
    {
        pcap_freealldevs(devices);
        if (port) free(port);
        if(num) free(num);
        if(interface) free(interface);
        return 0;
    }
    if (!device)
    {
        pcap_freealldevs(devices);
        if (port) free(port);
        if(num) free(num);
        if(interface) free(interface);
        cout << "ERROR: Device not found" << endl;
        return 1;
    }
    if(interface) free(interface);

    pcap_t* pcap = pcap_open_live(device->name, BUFSIZ, 1, 0, errbuf);
    if (!pcap) 
    {
        pcap_freealldevs(devices);
        if (port) free(port);
        if(num) free(num);
        cout << "ERROR: pcap failed: "<< errbuf << endl;
        return 1;
    }
    pcap_freealldevs(devices);

    // cout << pcap << endl;

    // if (port) {
    //     bpf_program  filter;
    //     if (pcap_compile(pcap, &filter, port.c_str(), 1, 0) == -1)
    //     {
    //         pcap_close(pcap);
    //         cout << "pcap_compile() failed: " << pcap_geterr(pcap);
    //         return 1;
    //     }
    //     if (pcap_setfilter(handle, &filter) == -1) {
    //         pcap_close(handle);
    //         cout << "pcap_setfilter() failed: " << pcap_geterr(handle);
    //         return 1;
    //     }
    // }
    
    if (pcap_loop(pcap, 10, process_packet, nullptr) == PCAP_ERROR) {
        pcap_close(pcap);
        cout << "ERROR: pcap failed: " << pcap_geterr(pcap);
        return 1;
    }
    pcap_close(pcap);
    if (port) free(port);
    if(num) free(num);

    return 0;
}

// Callback function
void process_packet(u_char *args, const struct pcap_pkthdr *header, const u_char *buffer)
{
	int size = header->len;
    
	
	//Get the IP Header part of this packet , excluding the ethernet header
	struct iphdr *iph = (struct iphdr*)(buffer + sizeof(struct ethhdr));

    u_short ip_header_len = (iph->ihl) * 4; // The IHL field contains the size of the IPv4 header, it has 4 bits that specify the number of 32-bit words in the header (5 min - 15 max)

    struct sockaddr_in source,dest;
    memset(&source, 0, sizeof(source));
	source.sin_addr.s_addr = iph->saddr;
	
	memset(&dest, 0, sizeof(dest));
	dest.sin_addr.s_addr = iph->daddr;

    

	++total;
	switch (iph->protocol) //Check the Protocol and do accordingly...
	{
		case 1:  //ICMP Protocol
        {
            ++icmp;
            const struct icmphdr* icmp_header = (struct icmphdr*)(buffer + sizeof(struct ethhdr) + ip_header_len);
            cout << endl << endl;
            cout << "ICMP" << endl;
            cout << inet_ntoa(source.sin_addr) << " : " << " > " << inet_ntoa(dest.sin_addr) << ":";
            cout << ", length "<< size << " bytes"<< endl;
            PrintData(buffer,size);
			break;
        }
		case 6:  //TCP Protocol
        {
            ++tcp;
            const struct tcphdr* tcp_header = (struct tcphdr*)(buffer + sizeof(struct ethhdr) + ip_header_len);
            cout << endl << endl;
            cout << "TCP" << endl;
            cout << inet_ntoa(source.sin_addr);
            cout << " : " << ntohs(tcp_header->source);
            cout << " > " << inet_ntoa(dest.sin_addr);
            cout << " : " << ntohs(tcp_header->dest);
            cout << ", length "<< size << " bytes"<< endl;
            PrintData(buffer,size);
			break;
        }
		case 17: //UDP Protocol
        {
            ++udp;
            const struct udphdr* udp_header = (struct udphdr*)(buffer + sizeof(struct ethhdr) + ip_header_len);
            cout << endl << endl;
            cout << "UDP" << endl;
            cout << inet_ntoa(source.sin_addr);
            cout << " : " << ntohs(udp_header->source);
            cout << " > " << inet_ntoa(dest.sin_addr);
            cout << " : " << ntohs(udp_header->dest);
            cout << ", length "<< size << " bytes"<< endl;
            PrintData(buffer,size);
			break;
        }
		default: //Some Other Protocol like ARP etc. 54 /91
        {
            ++others;
            cout << endl << endl;
            cout << "ELSE" << endl;
            cout << inet_ntoa(source.sin_addr);
            cout << " : " << " > " << inet_ntoa(dest.sin_addr)<< ":";
            cout << ", length "<< size << " bytes"<< endl;
            PrintData(buffer,size);
			break;
        }		
	}
}

bool arg_parse(int argc, char* argv[], unsigned *port, unsigned *num, char **interface, bool *tcp, bool *udp, bool *arp, bool *icmp)
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
                            value = argv[i+1];
                }
                if (value)
                {
                    *interface = (char *)malloc(strlen(value) + 1);
                    if (!*interface) return false;

                    strcpy(*interface, value);
                }
                break;
            }
            case 'p':
            {
                char *endptr = nullptr;
                port = (unsigned *)malloc(sizeof(unsigned));
                if (!port) return false;

                *port = (unsigned)strtoul(optarg, &endptr, 10);
                if (strcmp(endptr, "")) 
                {
                    return false;
                }
                if (*port > 65535 || optarg[0] == '-') 
                {
                    return false;
                }
                break;
            }
            case 'n':
            {
                char *endptr = nullptr;
                num = (unsigned *)malloc(sizeof(unsigned));
                if (!num) return false;

                *num = (unsigned)strtoul(optarg, &endptr, 10);
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
                *tcp = true;
                break;
            case 'u':
                *udp = true;
                break;
            case 'a':
                *arp = true;
                break;
            case 'c':
                *icmp = true;
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

void PrintData (const u_char * data , int Size)
{
	int i , j;
	for(i=0 ; i < Size ; i++)
	{
		if( i!=0 && i%16==0)   //if one line of hex printing is complete...
		{
			cout <<"         ";
			for(j=i-16 ; j<i ; j++)
			{
				if(data[j]>=32 && data[j]<=128)
				    cout << (unsigned char)data[j]; //if its a number or alphabet
				
				else cout <<"."; //otherwise print a dot
			}
			cout << endl;
		} 
		
		if(i%16==0) cout << "   ";
			printf(" %02X",(unsigned int)data[i]);
				
		if( i==Size-1)  //print the last spaces
		{
			for(j=0;j<15-i%16;j++) 
			{
			  printf("   "); //extra spaces
			}
			
			printf("         ");
			
			for(j=i-i%16 ; j<=i ; j++)
			{
				if(data[j]>=32 && data[j]<=128) 
				{
				  printf("%c",(unsigned char)data[j]);
				}
				else 
				{
				  printf(".");
				}
			}
			
			printf("\n" );
		}
	}
}

