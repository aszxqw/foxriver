#include "CppJieba/Husky/EpollServer.hpp"

namespace FoxRiver
{

    using namespace Husky;
    class ReqHandler: public IRequestHandler
    {
        public:
            ReqHandler(){}
            virtual ~ReqHandler(){}
        public:
            bool do_GET(const HttpReqInfo& httpReq, string& res) const
            {
                res.assign("hello foxriver.");
                return true;
            }
    };
}

