/**
 * @file PopOptions.cpp
 * @author Peter Rucek, xrucek00
 * @date 12.10.2021
 * PopOptions class
 */

#include "PopOptions.hpp"

PopOptions::PopOptions()
{
    this->port = 110; //default POP3
    this->deleteMessages = false;
    this->only_new = false;
    this->pop3s = false; // -T
    this->stls = false;  // -S
    this->tlsCertificate = NULL;
    this->tlsDirectory = NULL;

}

bool PopOptions::Create(char *argv[], int argc)
{
    FindServer(argv, argc);
    bool returnValue = ArgumentParse(argc, argv);
    if (server == NULL || !returnValue)
    {
        cout << "Usage:" << endl;
        cout << " popcl <server> [-p <port>] [-T|-S [-c <certfile>] [-C <certaddr>]]"
                " [-d] [-n] -a <auth_file> -o <out_dir>" << endl;
        return false;
    }
    return true;
}

void PopOptions::FindServer(char *argv[], int argc)
{
    char without_param_options[][OPTION_LEN] = {"-T","-S","-d","-n"};
    char with_param_options   [][OPTION_LEN] = {"-c","-C","-p","-a","-o"};
    int wo_arr_len = sizeof(without_param_options)/sizeof(*without_param_options);
    int w_arr_len  = sizeof(with_param_options)   /sizeof(*with_param_options);

    bool pos_of_server[MAX_PARA_LEN] = { false };

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
    
    if (count != 1)
        this->server = NULL;
    else
        this-> server = argv[position];
}



bool PopOptions::ArgumentParse(int argc, char* argv[])
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

                this->port = (unsigned)strtoul(optarg, &endptr, 10);
                if (strcmp(endptr, "")) 
                {
                    cerr << "ERROR: port should be a number (0-65535)" << endl;
                    return false;
                }
                if (this->port > 65535 || optarg[0] == '-')
                {
                    cerr << "ERROR: port should be a number (0-65535)" << endl;
                    return false;
                }
                break;
            }
            case 'T':
            {
                this->pop3s = true;
                break;
            }
            case 'S':
            {
                this->stls = true;
                break;
            }
            case 'c':
            {
                if (!CheckFile(optarg)) return false;
                this->tlsCertificate = optarg;
                break;
            }
            case 'C':
            {
                if (!CheckDir(optarg)) return false;
                this->tlsDirectory = optarg;
                break;
            }
            case 'a':
            {
                if (!CheckFile(optarg)) return false;
                this->authorizationFile = optarg;
                break;
            }
            case 'o':
            {
                if (!CheckDir(optarg)) return false;
                this->outputDirecory = optarg;
                break;
            }
            case 'd':
            {
                this->deleteMessages = true;
                break;
            }
            case 'n':
            {
                this->only_new = true;
                break;
            }
            case '?':
                return false;

        } // end switch
    } // end while

    if (this->outputDirecory == NULL)
    {
        cerr << "ERROR: You need to specify output directory" << endl;
        return false;
    }
    if (this->authorizationFile == NULL)
    {
        cerr << "ERROR: You need to specify authorization file" << endl;
        return false;
    }
    if (this->stls && this->pop3s)
    {
        cerr << "ERROR: Cannot do both STLS and POP3s, choose only one" << endl;
        return false;
    }
    if (this->tlsCertificate != NULL || this->tlsDirectory != NULL )
    {
        if (!(this->stls || this->pop3s))
        {
            cerr << "ERROR: Certificate only with options -S or -T" << endl;
            return false;
        }
    }
    return true;
}

bool PopOptions::CheckFile(char *string)
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

bool PopOptions::CheckDir(char *string)
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