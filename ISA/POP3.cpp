/**
 * @file POP3.cpp
 * @author Peter Rucek, xrucek00
 * @date 12.10.2021
 * POP3 class
 */

# include "POP3.hpp"

#define LOGGER 0


POP3::POP3(PopOptions options)
{
    this->ctx = NULL;
    SSL_library_init();
    SSL_load_error_strings();
    ERR_load_BIO_strings();
    OpenSSL_add_all_algorithms();
    this->options = options;
    this->stlsStarted = false;
    this->ssl = NULL;
    this->bio = NULL;
    this->cert = NULL;
}


POP3::~POP3()
{
    if (this->ctx != NULL)
    {
        SSL_CTX_free(this->ctx);
    }
    if(this->cert != NULL)
    {
        X509_free(this->cert);
    }
    if(this->options.getStls())
    {
        if (this->ssl != NULL)
        {
            SSL_shutdown(this->ssl );
            SSL_free    (this->ssl );
        }
    }
    else
    {
        BIO_free_all(this->bio);
    }
    ERR_free_strings();
    EVP_cleanup();

}


int POP3::Execute(bool *onlyNew, bool *deleteFlag)
{
    bool result = Authenticate();
    if( ! result) return FAILED;

    int downloaded = FAILED;
    // if -n -d at the same time - ignoring -d
    if(this->options.getNewFlag())
    {
        *onlyNew = true;
        downloaded = DownloadOnlyNew();
    }
    else if(this->options.getDeleteFlag())
    {
        *deleteFlag = true;
        downloaded = Delete();
    }
    else
    {
        downloaded = DownloadAllMails();
    }

    result = Quit();
    if( ! result) return FAILED;

    return downloaded;
}

bool POP3::SendMessage(string str)
{
    char buf[BUF_SIZE];
    sprintf(buf, "%s", str.c_str());
    bool loop = true;
    
    while (loop)
    {
        int ret_val = 0;
        if (this->stlsStarted)
        {
            ret_val = SSL_write(this->ssl, buf, (int)str.size());
        }
        else
        {
            ret_val = BIO_write(this->bio, buf, (int)str.size());
        }
        if (ret_val <= 0)
        {
            if (this->stlsStarted)
            {
                return false;
            }
            else
            {
                if (!BIO_should_retry(this->bio))
                {
                    cerr << "ERROR: Could not send request." << endl;
                    return false;
                }
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
        int ret_val = 0;
        if (this->stlsStarted)
        {
            ret_val = SSL_read(this->ssl, buf, BUF_SIZE - 2);
        }
        else
        {
            ret_val = BIO_read(this->bio, buf, BUF_SIZE - 2);
        }
        
        if (ret_val <= 0)
        {
            if (this->stlsStarted)
            {
                return false;
            }
            else
            {
                if (! BIO_should_retry(this->bio))
                {
                    cerr << "ERROR: Could not send request." << endl;
                    return false;
                }
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
    MakeHostname();
    if(LOGGER) cout << this->hostname << endl;

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

    if (! this->stlsStarted) // if stls already read first message
    {
        // first response from server
        if (! ReadMessage("\r\n"))
        {
            cerr << "ERROR: POP3 server error." << endl;
            return false;
        }
    }

    result = SendMessage("USER " + this->pair.username + "\r\n");
    if (! result) return false;

    if (! ReadMessage("\r\n"))
    {
        cerr << "ERROR: Username not found." << endl;
        return false;
    }

    result = SendMessage("PASS " + this->pair.password + "\r\n");
    if (! result) return false;

    if (! ReadMessage("\r\n"))
    {
        cerr << "ERROR: Password wrong." << endl;
        return false;
    }

    return true;
}


bool POP3::ReadAuthFile(char *fileName)
{
    this->pair = AuthorizationPair();
    string myText,buf;
    ifstream file(fileName);

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
    cerr << "ERROR: Problem with reading authorization file." << endl;
    return false;
}


bool POP3::ConnectionUnsecure()
{
    if(LOGGER) cout << "Unsecure connection" << endl;

    this->bio = BIO_new_connect(this->hostname);

    if (this->bio == NULL)
    {
        cerr << "ERROR: Problem with creating socket." << endl;
        return false;
    }

    if (BIO_do_connect(bio) <= 0)
    {
        cerr << "ERROR: Could not connect." << endl;
        return false;
    }

    if (this->options.getStls())
    {
        bool result =  ConnectionSTLS();
        return result;
    }

    return true;
}


bool POP3::ConnectionSecure()
{
    if(LOGGER) cout << "POP3s connection" << endl;
    
    bool result = CheckCertificate();
    if(! result) return false;

    this->bio = BIO_new_ssl_connect(this->ctx);
    BIO_get_ssl(this->bio, &this->ssl);
    SSL_set_mode(this->ssl, SSL_MODE_AUTO_RETRY);

    // Attempt to connect
    BIO_set_conn_hostname(this->bio, this->hostname);

    // Verify the connection opened and perform the handshake

    if (BIO_do_connect(bio) <= 0)
    {
        cerr << "ERROR: Could not connect." << endl;
        return false;
    }

    //Check if server send a certificate
    if ((this->cert = SSL_get_peer_certificate(this->ssl)) == NULL)
    {
        cerr << "ERROR: Server did not provide certificate." << endl;
        return false;
    }
    // Verify the certificate
    if (SSL_get_verify_result(this->ssl) != X509_V_OK)
    {
        cerr << "ERROR: Error verifying, Could not connect." << endl;
        return false;
    }
    return true;
}

bool POP3::ConnectionSTLS()
{
    if (! ReadMessage("\r\n"))
    {
        cerr << "ERROR: POP3 server error." << endl;
        return false;
    }

    bool result = SendMessage("STLS\r\n");
    if (! result) return false;

    if (! ReadMessage("\r\n"))
    {
        cerr << "ERROR: STLS error." << endl;
        return false;
    }

    result = CheckCertificate();
    if(! result) return false;

    this->stlsStarted = true;
    if(LOGGER) cout << "STLS connection" << endl;

    this->ssl = SSL_new(ctx);
    SSL_set_mode(this->ssl, SSL_MODE_AUTO_RETRY);
    // Attempt to connect
    SSL_set_bio(this->ssl, bio, bio);

    // Verify the connection opened and perform the handshake
    if (SSL_connect(this->ssl) != 1)
    {
        cerr << "ERROR: SSL connect failed." << endl;
        return false;
    }
    if (!SSL_do_handshake(this->ssl))
    {
        cerr << "ERROR: SSL handshake failed." << endl;
        return false;
    }
    //Check if server really send a certificate
    if ((this->cert = SSL_get_peer_certificate(this->ssl)) == NULL)
    {
        cerr << "ERROR: Server did not provide certificate." << endl;
        return false;
    }
    //Check certificate
    if (SSL_get_verify_result(this->ssl) != X509_V_OK)
    {
        cerr << "ERROR: Error verifying, Could not connect." << endl;
        return false;
    }
    return true;
}


bool POP3::CheckCertificate()
{
    this->ctx = SSL_CTX_new(SSLv23_client_method());
    if (this->ctx == NULL)
    {
        cerr << "ERROR: Could not create SSL struct." << endl;
        return false;
    }

    char *certificate = this->options.getTlsCertificate();
    char *directory = this->options.getTlsDirectory();

    if (certificate != NULL && directory == NULL)
    {
        if (! SSL_CTX_load_verify_locations(this->ctx, certificate, NULL))
        {
            cerr << "ERROR: Certificate not found." << endl;
            return false;
        }
    }
    else if (certificate == NULL && directory != NULL)
    {
        if (! SSL_CTX_load_verify_locations(this->ctx, NULL, directory))
        {
            cerr << "ERROR: Certificate folder not found." << endl;
            return false;
        }
    }
    else if (certificate != NULL && directory != NULL)
    {
        if (! SSL_CTX_load_verify_locations(this->ctx, certificate, directory))
        {
            cerr << "ERROR: Certificate folder or file not found." << endl;
            return false;
        }
    }
    else if ( certificate == NULL && directory == NULL)
    {
        if (! SSL_CTX_set_default_verify_paths(this->ctx))
        {
            cerr << "ERROR: Certificate not found." << endl;
            return false;
        }
    }
    return true;
}


void POP3::MakeHostname()
{
    char* server = this->options.getServer();
    char port[MAX_PORT_LEN];
    sprintf(port, "%d", this->options.getPort());

    if (strstr(server, ":") != NULL) //is IPv6
    {
        strcpy(this->hostname, "[");
        strcat(this->hostname, server);
        strcat(this->hostname, "]:");
        strcat(this->hostname, port);
    }
    else
    {
        strcpy(this->hostname, server);
        strcat(this->hostname, ":");
        strcat(this->hostname, port);
    }
}


int POP3::DownloadAllMails()
{
    if(LOGGER) cout << "Downloading all mails" << endl;

    int emailNumber = GetNumberOfMails();
    int numberOfMails = emailNumber;
    
    for(; emailNumber > 0; emailNumber--)
    {
        bool result = DownloadMail(emailNumber);
        if (! result) return FAILED;

        result = GetUIDLofMessage(emailNumber);
        if (! result) return FAILED;

        if (IsMessageNew(this->message))
        {
            result = AddUIDLentry(emailNumber);
            if (! result) return FAILED;
        }
    }

    return numberOfMails;
}


int POP3::DownloadOnlyNew()
{
    if(LOGGER) cout << "Downloading only new mails" << endl;

    int emailNumber = GetNumberOfMails();
    int count = 0;
    bool result;
    for(; emailNumber > 0; emailNumber--)
    {
        result = GetUIDLofMessage(emailNumber);
        if (! result) return FAILED;

        result = IsMessageNew(this->message);
        if (result)
        {
            count++;
            result = DownloadMail(emailNumber);
            if (! result) return FAILED;

            result = AddUIDLentry(emailNumber);
            if (! result) return FAILED;
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
        cerr << "ERROR: STAT problem." << endl;
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
        cerr << "ERROR: RETR problem." << endl;
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
        cerr << "ERROR: UIDL problem." << endl;
        return false;
    }

    this->message.erase(0, this->message.find(" ") + 1);
    this->message.erase(0, this->message.find(" ") + 1);
    return true;
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


int POP3::Delete()
{
    if(LOGGER) cout << "Deleting all mails" << endl;
    int count = 0;
    int num = GetNumberOfMails();
    for (int i = 1; i <= num; i++)
    {
        bool result = SendMessage("DELE " + to_string(i) + "\r\n");
        if (! result) return FAILED;
        count++ ;
    }
    
    return count;
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

bool POP3::Quit()
{
    bool result = SendMessage("QUIT\r\n");
    if (! result) return false;
    if ( !ReadMessage("\r\n")) 
    {
        cerr << "ERROR: Quit problem." << endl;
        return false;
    }
    return true;
}