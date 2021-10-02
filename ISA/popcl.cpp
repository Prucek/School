/**
 * @file popcl.cpp
 * @author Peter Rucek, xrucek00
 * @date 1.10.2021
 * POP3 client
 */

#include "popcl.hpp"

int main(int argc, char *argv[])
{
    char *auth_file;
    char *out_dir;
    int port = 0;
    char *server = new char[128]();
    if (!find_server(&server, argv, argc) ||
        !arg_parse(argc, argv, &auth_file, &out_dir, &port))
    {
        cerr << "ERROR: Wrong arguments" << endl;
        return 2;
    }
    else cerr << "Server je  "<< server << endl;
    return 0;
}


bool find_server(char **server, char *argv[], int argc)
{
    char without_param_options[][3] = {"-T","-S","-d","-n"};
    char with_param_options   [][3] = {"-c","-C","-p","-a","-o"};
    int wo_arr_len = sizeof(without_param_options)/sizeof(*without_param_options);
    int w_arr_len  = sizeof(with_param_options)   /sizeof(*with_param_options);

                    //MAX para len
    bool pos_of_server[12] = { false };

    for(int i = 1; i < argc; i++)
    {
        for(int j = 0; j < wo_arr_len; j++)
            if (strcmp(argv[i],without_param_options[j]) == 0)
            {
                pos_of_server[i] = true;
                break;
            }
        for(int j = 0; j < w_arr_len; j++)
            if (strcmp(argv[i],with_param_options[j]) == 0)
            {
                pos_of_server[i] = true;
                pos_of_server[i+1] = true;
                break;
            }
    }

    int position = 1;
    int count = 0;
    for(int i = 1; i < argc; i++)
    {
        if(pos_of_server[i] == false)
        {
            position = i;
            count++;
        }
    }
    
    *server = argv[position];
    if (count != 1)
        return false;
    else
        return true;
}



bool arg_parse(int argc, char* argv[], char **auth_file, char **out_dir, int *port)
{
    int iarg = 0;
    bool is_a = false;
    bool is_o = false;

    while(iarg != -1)
    {
        iarg = getopt(argc, argv, "p:TSc:C:dna:o:");
        
        switch (iarg)
        {
            case 'p':
            {
                char *endptr = nullptr;

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
            case 'T':
            {
                break;
            }
            case 'S':
            {
                break;
            }
            case 'a':
            {
                is_a = true;
                *auth_file = optarg;
                break;
            }
            case 'o':
            {
                is_o = true;
                *out_dir = optarg;
                break;
            }
            case '?':
                cout << "Usage:" << endl;
                cout << " popcl <server> [-p <port>] [-T|-S [-c <certfile>] [-C <certaddr>]]"
                        " [-d] [-n] -a <auth_file> -o <out_dir>" << endl;
                return false;

        } // end switch
    } // end while
    return is_o && is_a;
}
