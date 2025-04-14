#ifndef RESOURCESERVLET_H
#define RESOURCESERVLET_H

#include "net/http/servlets/Servlet.h"

namespace Chat
{
    class ResourceServlet final : public Gyanis::net::http::Servlet
    {
    public:
        using ptr = std::shared_ptr<ResourceServlet>;

        explicit ResourceServlet(std::string path);

        int32_t handle(const std::shared_ptr<Gyanis::net::http::HttpRequest>& request,
                       const std::shared_ptr<Gyanis::net::http::HttpResponse>& response,
                       const std::shared_ptr<Gyanis::net::http::HttpSession>& session) override;

    private:
        std::string m_path;
    };
}

#endif
