#include <iostream>
#include "ReqHandler.hpp"
using namespace FoxRiver;

const char * const USAGE = "usage: %s -p <port>";

const char * const DICT_PATH = "../dict/dict.utf8";
const char * const HMM_PATH = "../dict/hmm_model.utf8";
const char * const IDF_PATH = "../dict/idf.utf8";
const char * const STOPWORD_PATH = "../dict/stop_words.utf8";
const char * const TIME_DICT_PATH = "../dict/time.utf8";
const char * const CITY_PATH = "../dict/city.xml";
const char * const LOCATION_PATH = "../dict/location.xml";

const size_t THREAD_NUMBER = 4;
const size_t QUEUE_MAX_SIZE = 1024;

int main(int argc, char** argv)
{
    if(argc != 3)
    {
        fprintf(stderr, USAGE, argv[0]);
        return EXIT_FAILURE;
    }

    WordAssembler wordAssembler(DICT_PATH, HMM_PATH, IDF_PATH, STOPWORD_PATH, TIME_DICT_PATH, CITY_PATH, LOCATION_PATH);
    ReqHandler reqHandler(wordAssembler);
    ThreadPoolServer server(THREAD_NUMBER, QUEUE_MAX_SIZE, stoi(argv[2]), reqHandler);
    server.start();
    return EXIT_SUCCESS;
}
