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
    if (!options.Create(argc, argv))
    {
        return ARG_ERR;
    }

    POP3 pop = POP3(options);
    bool onlyNew = false;
    bool deleteFlag = false;
    int numberOfMails = pop.Execute(&onlyNew, &deleteFlag);
    if (numberOfMails == FAILED)
    {
        return POP_ERR;
    }
    else if (deleteFlag)
    {
        cout << "Deleted " << numberOfMails << " emails." << endl;
    }
    else
    {
        if (onlyNew)
        {
            cout << "Downloaded " << numberOfMails << " new emails." << endl;
        }
        else
        {
            cout << "Downloaded " << numberOfMails << " emails." << endl;
        }
    }

    return OK;
}