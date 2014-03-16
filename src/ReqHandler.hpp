#ifndef FOX_RIVER_REQ_HANDLER_H
#define FOX_RIVER_REQ_HANDLER_H

#include "CppJieba/Husky/EpollServer.hpp"
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
                string sentence;
                httpReq.GET("sentence", sentence);
                return _wordAssembler.run(sentence, res);
            }
    };
}

#endif
