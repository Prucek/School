/**
 * @file popcl.cpp
 * @author Peter Rucek, xrucek00
 * @date 1.10.2021
 * POP3 client
 */

#include "popcl.hpp"

int main(int argc, char *argv[])
{
    cout <<"bstrd";
    PopOptions *options = new PopOptions();
    if (!find_server(&(options->server), argv, argc) ||
        !arg_parse(argc, argv, options))
    {
        cout << "Usage:" << endl;
        cout << " popcl <server> [-p <port>] [-T|-S [-c <certfile>] [-C <certaddr>]]"
                " [-d] [-n] -a <auth_file> -o <out_dir>" << endl;
        return ARG_ERR;
    }

    

    /* Initializing OpenSSL */
    SSL_load_error_strings();
    ERR_load_BIO_strings();
    OpenSSL_add_all_algorithms();
    
    // creating conection
    cout <<"bstrd";
    BIO* bio = connection_unsecure(options);
    
    if (bio == NULL)
    {
        return BIO_ERR;
    }
    cout << "connected"<< endl;
    string msg = read_message(bio);
    cout << msg;

    // authorization_file reading
    AuthorizationPair *pair = read_authfile(options->authorization_file);
    if (pair->username.empty() || pair->password.empty())
    {
        cerr << "ERROR: Wrong authentization file" << endl;
        delete pair;
        return AUT_ERR;
    }

    authenticate(bio,pair);

    BIO_reset(bio);
    BIO_free_all(bio);
    delete pair; 
    delete options;
    return OK;
}

bool send_message(BIO *bio, string str)
{
    char buf[1024];
    sprintf(buf, "%s", str.c_str());
    bool loop = true;
    
    while (loop)
    {
        int ret_val = BIO_write(bio, buf, (int)str.size());
        if(ret_val <= 0)
        {
            if (!BIO_should_retry(bio)) {
                cerr << "ERROR: Could not send request" << endl;
                return false;
            }
        }
        else
        {
            //ok
            loop = false;
        }
    }
    return true;
}
string read_message(BIO *bio)
{
    char buf[1024] = {0};
    bool loop = true;
    
    while (loop)
    {
        int ret_val = BIO_read(bio, buf, 1024);
        if(ret_val <= 0)
        {
            if(! BIO_should_retry(bio))
            {
                cerr << "ERROR: Could not send request" << endl;
                return {};
            }
        }
        else
        {
            //ok
            loop = false;
        }
    }
    string str(buf);
    return str;
}
bool authenticate(BIO *bio, AuthorizationPair* pair)
{
    bool ret_val = send_message(bio,"USER " + pair->username + "\r\n");
    if (ret_val == false)
        return false;
    string msg = read_message(bio);
    if (msg.empty())
    {
        return false;
    }
    cout<< msg;
    ret_val = send_message(bio,"PASS " + pair->password + "\r\n");
    if (ret_val == false)
        return false;
    msg = read_message(bio);
    if (msg.empty())
    {
        return false;
    }
    cout<< msg;
    return true;
}

AuthorizationPair* read_authfile(char *file_name)
{
    AuthorizationPair* pair = new AuthorizationPair();
    string myText,buf;
    ifstream file(file_name);

    while (getline (file, myText)) 
    {
        buf += myText;
        buf += ' ';
    }
    string space_delimiter = " ";
    vector<string> words{};

    size_t pos = 0;
    while ((pos = buf.find(space_delimiter)) != string::npos)
    {
        words.push_back(buf.substr(0, pos));
        buf.erase(0, pos + space_delimiter.length());
    }
    if ((words[0] == "username") && (words[1] == "=") && (words[3] == "password") && (words[4] == "="))
    {
        pair->username = words[2];
        pair->password = words[5];
    }

    file.close();
    return pair;
}

BIO* connection_unsecure(PopOptions *options)
{
    char str[16]; // TODO enough bc max port 65535
    sprintf(str, "%d", options->port);

    char hostname[300]; 
    strcpy(hostname, options->server);
    strcat(hostname, ":");
    strcat(hostname,str);

    cout<< "aaaa";
    BIO *bio = BIO_new_connect(hostname);
    cout << "bbbbb";
    if(bio == NULL)
    {
        cerr << "ERROR: Problem with creting socket" << endl;
        return NULL;
    }


    if(BIO_do_connect(bio) <= 0)
    {
        cerr << "ERROR: Could not connect" << endl;
        return NULL;
    }
    return bio;
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
