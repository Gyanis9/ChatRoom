#include "my_module.h"
#include "base/Config.h"
#include "base/Log.h"
#include "core/Env.h"
#include "net/Application.h"
#include "net/http/HttpServer.h"
#include "net/web/TcpServer.h"
#include "ResourceServlet.h"
#include "net/http/WebSocketServer.h"
#include "ChatServlet.h"

namespace Chat
{
    static auto g_logger = LOG_NAME("system");

    MyModule::MyModule(): Module("ChatRoom", "1.0", "")
    {
    }

    bool MyModule::onLoad()
    {
        LOG_INFO(g_logger) << "onLoad";
        return true;
    }

    bool MyModule::onUnload()
    {
        LOG_INFO(g_logger) << "onUnload";
        return true;
    }

    bool MyModule::onServerReady()
    {
        LOG_INFO(g_logger) << "onServerReady";
        std::vector<std::shared_ptr<Gyanis::net::web::TcpServer>> servers;
        if (!Gyanis::net::Application::GetInstance()->getServer("http", servers))
        {
            LOG_INFO(g_logger) << "no httpserver alive";
            return false;
        }

        for (auto& i : servers)
        {
            const std::shared_ptr<Gyanis::net::http::HttpServer> http_server =
                std::dynamic_pointer_cast<Gyanis::net::http::HttpServer>(i);
            if (!i)
            {
                continue;
            }
            const auto slt_dispatch = http_server->getServletDispatch();

            auto slt = std::make_shared<ResourceServlet>(
                Gyanis::core::EnvMgr::GetInstance()->getCwd()
            );
            slt_dispatch->addGlobalServlet("/html/*", slt);
            LOG_INFO(g_logger) << "addServlet /html/*";
        }

        servers.clear();
        if (!Gyanis::net::Application::GetInstance()->getServer("ws", servers))
        {
            LOG_INFO(g_logger) << "no ws alive";
            return false;
        }

        for (auto& i : servers)
        {
            const auto ws_server = std::dynamic_pointer_cast<Gyanis::net::http::WSServer>(i);
            const auto slt_dispatch = ws_server->getWSServletDispatch();
            auto slt = std::make_shared<ChatServlet>();
            slt_dispatch->addServlet("/Gyanis/chat", slt);
            LOG_INFO(g_logger) << "addServlet /Gyanis/chat";
        }
        return true;
    }

    bool MyModule::onServerUp()
    {
        LOG_INFO(g_logger) << "onServerUp";
        return true;
    }
}

extern "C" {
Gyanis::net::Module* CreateModule()
{
    Gyanis::net::Module* module = new Chat::MyModule;
    LOG_INFO(Chat::g_logger) << "CreateModule " << module;
    return module;
}

void DestoryModule(const Gyanis::net::Module* module)
{
    LOG_INFO(Chat::g_logger) << "CreateModule " << module;
    delete module;
}
}
