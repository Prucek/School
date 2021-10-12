/**
 * @file POP3.cpp
 * @author Peter Rucek, xrucek00
 * @date 12.10.2021
 * POP3 class
 */

# include "POP3.hpp"

#define LOGGER 1

POP3::POP3(PopOptions options)
{
    SSL_load_error_strings();
    ERR_load_BIO_strings();
    OpenSSL_add_all_algorithms();
    this->options = options;
}


POP3::~POP3()
{
    BIO_reset(bio);
    BIO_free_all(bio);
}


bool POP3::SendMessage(string str)
{
    char buf[BUF_SIZE];
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


string POP3::ReadMessage()
{
    char buf[BUF_SIZE] = {0};
    bool loop = true;
    
    while (loop)
    {
        int ret_val = BIO_read(bio, buf, BUF_SIZE);
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
    if(LOGGER) cout << str;
    return str;
}


bool POP3::Authenticate()
{
    bool returnValue = ConnectionUnsecure();
    if (returnValue == false)
        return false;

    returnValue = ReadAuthFile(options.getAuthorizationFile());
    if (returnValue == false)
        return false;

    string msg = ReadMessage();
    if (msg.empty())
    {
        return false;
    }

    returnValue = SendMessage("USER " + this->pair.username + "\r\n");
    if (returnValue == false)
        return false;

    msg = ReadMessage();
    if (msg.empty())
    {
        return false;
    }

    returnValue = SendMessage("PASS " + this->pair.password + "\r\n");
    if (returnValue == false)
        return false;
    msg = ReadMessage();
    if (msg.empty())
    {
        return false;
    }

    return true;
}


bool POP3::ReadAuthFile(char *file_name)
{
    this->pair = AuthorizationPair();
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
    // exact order in the file
    // username = user
    // password = pswd
    if ((words[0] == "username") && (words[1] == "=") && (words[3] == "password") && (words[4] == "="))
    {
        pair.username = words[2];
        pair.password = words[5];
        file.close();
        return true;
    }

    file.close();
    cerr << "ERROR: Problem with reading authorization file" << endl;
    return false;
}


bool POP3::ConnectionUnsecure()
{
    char str[MAX_PORT_LEN];
    sprintf(str, "%d", options.getPort());

    char hostname[MAX_NAME_LEN]; 
    strcpy(hostname, options.getServer());
    strcat(hostname, ":");
    strcat(hostname,str);

    if(LOGGER) cout << hostname << endl;

    this->bio = BIO_new_connect(hostname);

    if(bio == NULL)
    {
        cerr << "ERROR: Problem with creating socket" << endl;
        return false;
    }

    if(BIO_do_connect(bio) <= 0)
    {
        cerr << "ERROR: Could not connect" << endl;
        return false;
    }

    return true;
}