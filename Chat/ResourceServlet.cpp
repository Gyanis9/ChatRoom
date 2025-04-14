#include "ResourceServlet.h"
#include "base/Log.h"

namespace Chat
{
    static auto g_logger = LOG_NAME("system");


    ResourceServlet::ResourceServlet(std::string path): Servlet("ResourceServlet"), m_path(std::move(path))
    {
    }

    int32_t ResourceServlet::handle(const std::shared_ptr<Gyanis::net::http::HttpRequest>& request,
                                    const std::shared_ptr<Gyanis::net::http::HttpResponse>& response,
                                    const std::shared_ptr<Gyanis::net::http::HttpSession>& session)
    {
        auto path = m_path + "/" + request->getPath();
        LOG_INFO(g_logger) << "handle path=" << path;
        if (path.find("..") != std::string::npos)
        {
            response->setBody("invalid path");
            response->setStatus(Gyanis::net::http::HttpStatus::NOT_FOUND);
            return 0;
        }
        std::ifstream ifs(path);
        if (!ifs)
        {
            response->setBody("invalid file");
            response->setStatus(Gyanis::net::http::HttpStatus::NOT_FOUND);
            return 0;
        }

        std::stringstream ss;
        std::string line;
        while (std::getline(ifs, line))
        {
            ss << line << std::endl;
        }

        response->setBody(ss.str());
        response->setHeader("content-type", "text/html;charset=utf-8");
        return 0;
    }
}
