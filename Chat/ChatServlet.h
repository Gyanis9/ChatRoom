#ifndef CHATSERVLET_H
#define CHATSERVLET_H
#include "net/http/WebSocketServlet.h"

namespace Chat
{
    class ChatServlet final : public Gyanis::net::http::WSServlet
    {
    public:
        using ptr = std::shared_ptr<ChatServlet>;

        ChatServlet();

        int32_t onConnect(const std::shared_ptr<Gyanis::net::http::HttpRequest>& header,
                          const std::shared_ptr<Gyanis::net::http::WSSession>& session) override;

        int32_t onClose(const std::shared_ptr<Gyanis::net::http::HttpRequest>& header,
                        const std::shared_ptr<Gyanis::net::http::WSSession>& session) override;

        int32_t handle(const std::shared_ptr<Gyanis::net::http::HttpRequest>& header,
                       const std::shared_ptr<Gyanis::net::http::WSFrameMessage>& msgx,
                       const std::shared_ptr<Gyanis::net::http::WSSession>& session) override;
    };
}

#endif
