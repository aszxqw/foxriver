#include <iostream>
#include "ReqHandler.hpp"
using namespace FoxRiver;

const char * const USAGE = "usage: %s -p <port>";

int main(int argc, char** argv)
{
    if(argc != 3)
    {
        fprintf(stderr, USAGE, argv[0]);
        return EXIT_FAILURE;
    }

    ReqHandler reqHandler;
    EpollServer server(stoi(argv[2]), &reqHandler);
    server.start();
    return EXIT_SUCCESS;
}
