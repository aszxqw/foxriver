#include <iostream>
#include "ReqHandler.hpp"
using namespace FoxRiver;

int main(int argc, char** argv)
{
    int port = 14311;
    ReqHandler reqHandler;
    EpollServer server(port, &reqHandler);
    server.start();
    return EXIT_SUCCESS;
}
