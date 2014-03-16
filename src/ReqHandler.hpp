#ifndef FOX_RIVER_REQ_HANDLER_H
#define FOX_RIVER_REQ_HANDLER_H

#include "CppJieba/Husky/EpollServer.hpp"
#include "CppJieba/KeywordExtractor.hpp"

namespace FoxRiver
{
    using namespace CppJieba;
    using namespace Husky;
    class ReqHandler: public IRequestHandler
    {
        private:
            KeywordExtractor _keywordExtractor;
        public:
            ReqHandler(const string& dictPath, const string& hmmPath, const string& idfPath, const string& stopwordPath): _keywordExtractor(dictPath, hmmPath, idfPath, stopwordPath){}
            virtual ~ReqHandler(){}
        public:
            bool do_GET(const HttpReqInfo& httpReq, string& res) const
            {
                //string time = "20140311";
                //string location = "上海";
                vector<string> words;
                string sentence;
                httpReq.GET("sentence", sentence);
                _keywordExtractor.extract(sentence, words, 5);
                print(words);
                //string_format(res, "{\"time\": \"%s\", \"location\": \"%s\"}", time.c_str(), location.c_str());
                return true;
            }
    };
}

#endif
