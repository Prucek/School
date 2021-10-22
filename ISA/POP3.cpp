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
    this->ctx = NULL;
    SSL_library_init();
    SSL_load_error_strings();
    ERR_load_BIO_strings();
    OpenSSL_add_all_algorithms();
    this->options = options;
}


POP3::~POP3()
{
    if (this->ctx != NULL) SSL_CTX_free(this->ctx);
    BIO_free_all(bio);

    // SOURCE:
    // https://stackoverflow.com/questions/29008145/valgrind-shows-memory-leak-in-ssl-after-closing-the-connection
    // CRYPTO_cleanup_all_ex_data();
    // ERR_free_strings();
    // ERR_remove_state(0);
    // EVP_cleanup();
}


int POP3::Execute(bool *onlyNew)
{
    bool result = Authenticate();
    if( ! result) return DOWNLOAD_FAILED;

    int downloaded = DOWNLOAD_FAILED;
    if(this->options.getNewFlag())
    {
        *onlyNew = true;
        downloaded = DownloadOnlyNew();
    }
    else if(this->options.getDeleteFlag())
    {
        // Delete()
    }
    else
    {
        downloaded = DownloadAllMails();
    }

    return downloaded;
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
    if(LOGGER) cout <<"C: "<< str;
    return true;
}


bool POP3::ReadMessage(string dot)
{
    bool loop = true;
    this->message.clear();
    while (loop)
    {
        char buf[BUF_SIZE]= {0};
        int ret_val = BIO_read(bio, buf, BUF_SIZE - 2);
        if(ret_val <= 0)
        {
            if(! BIO_should_retry(bio))
            {
                cerr << "ERROR: Could not send request" << endl;
                return false;
            }
        }
        else
        {
            string str(buf);
            this->message += str;
            if (this->message.find(dot) != string::npos)
            {
                loop = false;
            }
        }
    }
    if(LOGGER) cout <<"S: "<< this->message;
    return CheckOK();
}


bool POP3::Authenticate()
{
    bool result;
    if (this->options.getPop3S())
    {
        result = ConnectionSecure();
    }
    else 
    {
        result = ConnectionUnsecure();
    }
    if (! result) return false;

    result = ReadAuthFile(this->options.getAuthorizationFile());
    if (! result) return false;

    // firts response from server
    if (! ReadMessage("\r\n"))
    {
        cerr << "ERROR: POP3 not ready" << endl;
        return false;
    }

    result = SendMessage("USER " + this->pair.username + "\r\n");
    if (! result) return false;

    if (! ReadMessage("\r\n"))
    {
        cerr << "ERROR: Username not found" << endl;
        return false;
    }

    result = SendMessage("PASS " + this->pair.password + "\r\n");
    if (! result) return false;

    if (! ReadMessage("\r\n"))
    {
        cerr << "ERROR: Password wrong" << endl;
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
        this->pair.username = words[2];
        this->pair.password = words[5];
        file.close();
        return true;
    }

    file.close();
    cerr << "ERROR: Problem with reading authorization file" << endl;
    return false;
}


bool POP3::ConnectionUnsecure()
{
    MakeHostname();
    if(LOGGER) cout << hostname << endl;

    this->bio = BIO_new_connect(hostname);

    if(this->bio == NULL)
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


bool POP3::ConnectionSecure()
{
    MakeHostname();
    if(LOGGER) cout << hostname << endl;

    this->ctx = SSL_CTX_new(SSLv23_client_method());
    SSL *ssl;
    // TODO
    // if(! SSL_CTX_load_verify_locations(ctx, "/path/to/TrustStore.pem", NULL))
    // {
    //     // Handle failed load here
    // }
    // if(! SSL_CTX_load_verify_locations(ctx, NULL, "/path/to/certfolder"))
    // {
    //     // Handle error here
    // }
    if (! SSL_CTX_set_default_verify_paths(this->ctx))
    {
        cerr << "ERROR: Certificate not found" << endl;
        return false;
    }

    this->bio = BIO_new_ssl_connect(this->ctx);
    BIO_get_ssl(bio, &ssl);
    SSL_set_mode(ssl, SSL_MODE_AUTO_RETRY);

    // Attempt to connect
    BIO_set_conn_hostname(bio, hostname);

    // Verify the connection opened and perform the handshake

    if(BIO_do_connect(bio) <= 0)
    {
        cerr << "ERROR: Error connecting to server" << endl;
        return false;
    }
     if(BIO_do_handshake(bio) <= 0)
    {
        cerr << "ERROR: Error establishing SSL connection" << endl;
        return false;
    }

    // if(SSL_get_verify_result(ssl) != X509_V_OK)
    // {
    //     cerr << "ERROR: verify, Could not connect" << endl;
    //     return false;
    // }

    return true;
}


void POP3::MakeHostname()
{
    char str[MAX_PORT_LEN];
    sprintf(str, "%d", this->options.getPort());
    // strcpy (hostname, "["); // add ipv6
    strcpy(this->hostname, this->options.getServer());
    strcat(this->hostname, ":");
    strcat(this->hostname,str);
}


int POP3::DownloadAllMails()
{
    int emailNumber = GetNumberOfMails();
    int numberOfMails = emailNumber;
    
    for(; emailNumber > 0; emailNumber--)
    {
        bool result = DownloadMail(emailNumber);
        if (! result) return DOWNLOAD_FAILED;

        result = AddUIDLentry(emailNumber);
        if (! result) return DOWNLOAD_FAILED;
    }

    return numberOfMails;
}


int POP3::DownloadOnlyNew()
{
    int emailNumber = GetNumberOfMails();
    int count = 0;
    bool result;
    for(; emailNumber > 0; emailNumber--)
    {
        result = GetUIDLofMessage(emailNumber);
        if (! result) return DOWNLOAD_FAILED;

        result = IsMessageNew(this->message);
        if(result)
        {
            count++;
            result = DownloadMail(emailNumber);
            if (! result) return DOWNLOAD_FAILED;

            result = AddUIDLentry(emailNumber);
            if (! result) return DOWNLOAD_FAILED;
        }
    }

    return count;
}


int POP3::GetNumberOfMails()
{
    string whole;
    bool result = SendMessage("STAT\r\n");
    if (! result) return 0;

    if (! ReadMessage("\r\n"))
    {
        cerr << "ERROR: STAT problem" << endl;
        return 0;
    }
    string delimiter = " ";
    int count = 0;
    size_t pos = 0;
    while ((pos = this->message.find(delimiter)) != string::npos )
    {
        if(count == 0)
        {
            this->message.erase(0, pos + 1);
        }
        if (count == 1)
        {
            this->message.erase(pos, this->message.find("\r\n"));
        }
        count++;
    }
    return stoi(message);
}


bool POP3::DownloadMail(int messageNumber)
{
    bool result = SendMessage("RETR "+ to_string(messageNumber) + "\r\n");
    if (! result) return false;
    if ( !ReadMessage("\r\n.\r\n")) 
    {
        cerr << "ERROR: RETR problem" << endl;
        return false;
    }
    // delete first line from server +OK .... octets..
    this->message.erase(0, this->message.find("\n") + 1);
    // delete terminantion octet
    this->message = this->message.substr(0, this->message.size() - 3);
    // delete dots
    string replacement = "\r\n.";
    regex toReplace("\r\n\\.\\.");
    this->message = std::regex_replace(this->message, toReplace, replacement);

    // write to file
    ofstream myfile;
    string outDir(this->options.getOutputDirecotry());
    myfile.open( outDir + "/" + this->pair.username + "_mail_" + to_string(messageNumber) + ".eml");
    myfile << this->message;
    myfile.close();
    return true;
}

bool POP3::AddUIDLentry(int messageNumber)
{
    bool result = GetUIDLofMessage(messageNumber);
    if (! result) return false;
    ofstream dbfile;
    dbfile.open(DB_NAME, ios_base::app);
    dbfile << this->message;
    dbfile.close();
    return true;
}


bool POP3::GetUIDLofMessage(int messageNumber)
{
    bool result = SendMessage("UIDL " + to_string(messageNumber) + "\r\n");
    if (! result) return false;

    if (! ReadMessage("\r\n"))
    {
        cerr << "ERROR: UIDL problem" << endl;
        return false;
    }
    ParseUIDLofMessage();
    return true;
}

void POP3::ParseUIDLofMessage()
{
    this->message.erase(0, this->message.find(" ") + 1);
    this->message.erase(0, this->message.find(" ") + 1);
}


bool POP3::IsMessageNew(string uidlNew)
{
    ifstream loacalDb(DB_NAME);
    if (loacalDb.is_open())
    {
        string uidlLine;
        while(getline(loacalDb, uidlLine))
        {
            uidlLine = uidlLine + "\n";
            if(uidlNew == uidlLine)
            {
                return false;
            }
        }
   }
   loacalDb.close();
   return true;
}


bool POP3::CheckOK()
{
    if(this->message.find("+OK") != string::npos)
    {
        return true;
    }
    else
    {
        return false;
    }
}