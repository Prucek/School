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
    this->deleteFlag = false;
    this->newFlag = false;
    this->pop3s = false; // -T
    this->stls = false;  // -S
    this->tlsCertificate = NULL;
    this->tlsDirectory = NULL;
    this->authorizationFile = NULL;
    this->outputDirecory = NULL;
    this->server = NULL;

}

bool PopOptions::Create(int argc, char *argv[])
{
    FindServer(argc, argv);
    if (this->server == NULL)
    {
        cout << USAGE << endl;
        return false;
    }

    bool returnValue = ArgumentParse(argc, argv);
    if (!returnValue)
    {
        return false;
    }
    return true;
}

void PopOptions::FindServer(int argc, char *argv[])
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
    for(int i = 1; i < argc; i++)
    {
        string str(argv[i]);
        if (str == this->server)
        {
            continue;
        }
        else if (str == "-p")
        {
            if (i+1 >= argc) 
            {
                cout << USAGE << endl;
                return false;
            }
            char *endptr = nullptr;

            this->port = (unsigned)strtoul(argv[i+1], &endptr, 10);
            if (strcmp(endptr, "")) 
            {
                cerr << "ERROR: port should be a number (0-65535)." << endl;
                return false;
            }
            if (this->port > 65535 || argv[i+1][0] == '-')
            {
                cerr << "ERROR: port should be a number (0-65535)." << endl;
                return false;
            }
            i++; 
        }
        else if (str == "-T")
        {
            this->pop3s = true;
            this->port = 995; // default pop3s
        }
        else if (str == "-S")
        {
            this->stls = true;
        }
        else if (str == "-c")
        {
            if (i+1 >= argc) 
            {
                cout << USAGE << endl;
                return false;
            }
            if (!CheckFile(argv[i+1])) return false;
            this->tlsCertificate = argv[i+1];
            i++;
        }
        else if (str == "-C")
        {
            if (i+1 >= argc) 
            {
                cout << USAGE << endl;
                return false;
            }
            if (!CheckDir(argv[i+1])) return false;
            this->tlsDirectory = argv[i+1];
            i++;
        }
        else if (str == "-a")
        {
            if (i+1 >= argc) 
            {
                cout << USAGE << endl;
                return false;
            }
            if (!CheckFile(argv[i+1])) return false;
            this->authorizationFile = argv[i+1];
            i++;
        }
        else if (str == "-o")
        {
            if (i+1 >= argc) 
            {
                cout << USAGE << endl;
                return false;
            }
            if (!CheckDir(argv[i+1])) return false;
            this->outputDirecory = argv[i+1];
            i++;
        }
        else if (str == "-d")
        {
            this->deleteFlag = true;
        }
        else if (str == "-n")
        {
            this->newFlag = true;
        }
        else
        {
            cout << USAGE << endl;
            return false;
        }
    } // end for

    if (this->outputDirecory == NULL)
    {
        cerr << "ERROR: You need to specify output directory." << endl;
        return false;
    }
    if (this->authorizationFile == NULL)
    {
        cerr << "ERROR: You need to specify authorization file." << endl;
        return false;
    }
    if (this->stls && this->pop3s)
    {
        cerr << "ERROR: Cannot do both STLS and POP3s, choose only one." << endl;
        return false;
    }
    if (this->tlsCertificate != NULL || this->tlsDirectory != NULL )
    {
        if (!(this->stls || this->pop3s))
        {
            cerr << "ERROR: Certificate only with options -S or -T." << endl;
            return false;
        }
    }
    return true;
}

bool PopOptions::CheckFile(char *string)
{
    struct stat buffer;
    if( stat( string, &buffer ) != 0 )
        cerr << "ERROR: " << string << " is not a file." << endl;
    else if( buffer.st_mode & S_IFREG )
        return true;
    else
        cerr << "ERROR: " << string << " is not a file." << endl;
    return false;
}

bool PopOptions::CheckDir(char *string)
{
    struct stat buffer;
    if( stat( string, &buffer ) != 0 )
        cerr << "ERROR: " << string << " is not a directory." << endl;
    else if( buffer.st_mode & S_IFDIR )
        return true;
    else
        cerr << "ERROR: " << string << " is not a directory." << endl;
    return false;
}