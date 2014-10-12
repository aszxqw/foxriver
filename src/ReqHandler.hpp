#ifndef FOX_RIVER_REQ_HANDLER_H
#define FOX_RIVER_REQ_HANDLER_H

#include "CppJieba/Husky/ThreadPoolServer.hpp"
#include "WordAssembler.hpp"

namespace FoxRiver
{
    using namespace Husky;
    class ReqHandler: public IRequestHandler
    {
        private:
            const WordAssembler& _wordAssembler;
        public:
            ReqHandler(const WordAssembler& wordAssembler): _wordAssembler(wordAssembler){}
            virtual ~ReqHandler(){}
        public:
            bool do_GET(const HttpReqInfo& httpReq, string& res) const
            {
                string sentence, tmp;
                httpReq.GET("sentence", tmp);
                URLDecode(tmp, sentence);
                return _wordAssembler.run(sentence, res);
            }
            bool do_POST(const HttpReqInfo& httpReq, string& res) const 
            {
                //TODO
                return false;
            }

    };
}

#endif
