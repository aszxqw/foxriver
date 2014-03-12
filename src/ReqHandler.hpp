#ifndef FOX_RIVER_REQ_HANDLER_H
#define FOX_RIVER_REQ_HANDLER_H

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
                string time = "20140311";
                string location = "上海";
                string_format(res, "{\"time\": \"%s\", \"location\": \"%s\"}", time.c_str(), location.c_str());
                return true;
            }
    };
}

#endif
