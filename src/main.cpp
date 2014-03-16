#include <iostream>
#include "ReqHandler.hpp"
using namespace FoxRiver;

const char * const USAGE = "usage: %s -p <port>";

const char * const DICT_PATH = "../dict/dict.utf8";
const char * const HMM_PATH = "../dict/hmm_model.utf8";
const char * const IDF_PATH = "../dict/idf.utf8";
const char * const STOPWORD_PATH = "../dict/stop_words.utf8";

int main(int argc, char** argv)
{
    if(argc != 3)
    {
        fprintf(stderr, USAGE, argv[0]);
        return EXIT_FAILURE;
    }

    ReqHandler reqHandler(DICT_PATH, HMM_PATH, IDF_PATH, STOPWORD_PATH);
    EpollServer server(stoi(argv[2]), &reqHandler);
    server.start();
    return EXIT_SUCCESS;
}
