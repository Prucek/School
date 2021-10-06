/**
 * @file popcl.cpp
 * @author Peter Rucek, xrucek00
 * @date 1.10.2021
 * POP3 client
 */

#include "popcl.hpp"

int main(int argc, char *argv[])
{
    PopOptions options;
    if (!find_server(&(options.server), argv, argc) ||
        !arg_parse(argc, argv, &options))
    {
        cout << "Usage:" << endl;
        cout << " popcl <server> [-p <port>] [-T|-S [-c <certfile>] [-C <certaddr>]]"
                " [-d] [-n] -a <auth_file> -o <out_dir>" << endl;
        return ARG_ERR;
    }

    return OK;
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



bool arg_parse(int argc, char* argv[], PopOptions *options)
{
    int iarg = 0;

    while(iarg != -1)
    {
        iarg = getopt(argc, argv, "p:TSc:C:dna:o:");
        
        switch (iarg)
        {
            case 'p':
            {
                char *endptr = nullptr;

                options->port = (unsigned)strtoul(optarg, &endptr, 10);
                if (strcmp(endptr, "")) 
                {
                    cerr << "ERROR: port should be a number (0-65535)" << endl;
                    return false;
                }
                if (options->port > 65535 || optarg[0] == '-')
                {
                    cerr << "ERROR: port should be a number (0-65535)" << endl;
                    return false;
                }
                break;
            }
            case 'T':
            {
                options->pop3s = true;
                break;
            }
            case 'S':
            {
                options->stls = true;
                break;
            }
            case 'c':
            {
                if (!check_file(optarg)) return false;
                options->tls_certificate = optarg;
                break;
            }
            case 'C':
            {
                if (!check_dir(optarg)) return false;
                options->tls_directory = optarg;
                break;
            }
            case 'a':
            {
                if (!check_file(optarg)) return false;
                options->authorization_file = optarg;
                break;
            }
            case 'o':
            {
                if (!check_dir(optarg)) return false;
                options->output_direcory = optarg;
                break;
            }
            case 'd':
            {
                options->delete_messages = true;
                break;
            }
            case 'n':
            {
                options->only_new = true;
                break;
            }
            case '?':
                return false;

        } // end switch
    } // end while

    if (options->output_direcory == NULL)
    {
        cerr << "ERROR: You need to specify output directory" << endl;
        return false;
    }
    if (options->authorization_file == NULL)
    {
        cerr << "ERROR: You need to specify authorization file" << endl;
        return false;
    }
    if (options->stls && options->pop3s)
    {
        cerr << "ERROR: Cannot do both STLS and POP3s, choose only one" << endl;
        return false;
    }
    if (options->tls_certificate != NULL || options->tls_directory != NULL )
    {
        if (!(options->stls || options->pop3s))
        {
            cerr << "ERROR: Certificate only with options -S or -T" << endl;
            return false;
        }
    }
    return true;
}

bool check_file(char *string)
{
    struct stat buffer;
    if( stat( optarg, &buffer ) != 0 )
        cerr << "ERROR: " << string << " is not a file" << endl;
    else if( buffer.st_mode & S_IFREG )
        return true;
    else
        cerr << "ERROR: " << string << " is not a file" << endl;
    return false;
}

bool check_dir(char *string)
{
    struct stat buffer;
    if( stat( optarg, &buffer ) != 0 )
        cerr << "ERROR: " << string << " is not a directory" << endl;
    else if( buffer.st_mode & S_IFDIR )
        return true;
    else
        cerr << "ERROR: " << string << " is not a directory" << endl;
    return false;
}
