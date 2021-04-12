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

#define no_argument 0
#define required_argument 1 

bool arg_parse(int argc, char* argv[], unsigned *port, unsigned *num, char *interface);
// [-i rozhraní | --interface rozhraní] {-p ­­port} {[--tcp|-t] [--udp|-u] [--arp] [--icmp] } {-n num}
int main(int argc, char * argv[])
{
    std::cout << "Hello" << std::endl;

    unsigned *port = nullptr;
    unsigned *num  = nullptr;
    char *interface = nullptr; 
    if (arg_parse(argc,argv, port, num, interface))
        std::cout << "All right!" << std::endl;

    std::cout << "GoodBye!" << std::endl;

    return 0; 
}

bool arg_parse(int argc, char* argv[], unsigned *port, unsigned *num, char *interface)
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
    int iarg=0;
    bool is_i = false;

    while(iarg != -1)
    {                                    // : )
        iarg = getopt_long(argc, argv, "p:uti::n:", longopts, &index);

        switch (iarg)
        {
            case 'i':
            {
                is_i = true;
                char *value = nullptr;
                std::cout << "You hit i" << std::endl;
                for (int i = 0; i < argc ; i++)
                {
                    if (strcmp(argv[i], "-i")  == 0|| strcmp(argv[i], "--interface") == 0)
                        value = argv[i+1];
                }
                if (value)
                {
                    std::cout << value << std::endl;
                    interface = (char *)malloc(strlen(value) + 1);
                    if (!interface) return false;
                    strcpy(interface, value);
                }
                    
                break;
            }
            case 'p':
            {
                std::cout << "You hit p" << std::endl;
                std::cout<<optarg<<std::endl;

                char *endptr = nullptr;
                port = (unsigned *)malloc(sizeof(unsigned));
                if (!port)
                {
                    return false;
                }
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
            case 't':
                std::cout << "You hit t" << std::endl;
                break;

            case 'u':
                std::cout << "You hit u" << std::endl;
                break;

            case 'n':
            {
                std::cout << "You hit n" << std::endl;
                std::cout<<optarg<<std::endl;

                char *endptr = nullptr;
                num = (unsigned *)malloc(sizeof(unsigned));
                if (!port)
                {
                    return false;
                }
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
            case 'a':
                std::cout << "You hit arp" << std::endl;
                break;

            case 'c':
                std::cout << "You hit icmp" << std::endl;
                break;

        } // end switch
    } // end while
    return is_i;
}