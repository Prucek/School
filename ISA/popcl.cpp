/**
 * @file popcl.cpp
 * @author Peter Rucek, xrucek00
 * @date 1.10.2021
 * POP3 client
 */

#include "popcl.hpp"

int main(int argc, char *argv[])
{
    PopOptions options = PopOptions();
    if (!options.Create(argv, argc))
    {
        return ARG_ERR;
    }

    POP3 pop = POP3(options);
    if(!pop.Authenticate())
    {
        return POP_ERR;
    }
    pop.DownloadAllMails();

    return OK;
}