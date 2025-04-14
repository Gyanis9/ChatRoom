#include "ChatServlet.h"
#include "base/Log.h"
#include "Protocol.h"

namespace Chat
{
    static auto g_logger = LOG_NAME("system");
    std::shared_mutex m_mutex;

    std::unordered_map<std::string, std::shared_ptr<Gyanis::net::http::WSSession>> m_sessions;

    bool session_exists(const std::string& id)
    {
        LOG_INFO(g_logger) << "session_exists id=" << id;
        std::shared_lock lock(m_mutex);
        const auto it = m_sessions.find(id);
        return it != m_sessions.end();
    }

    void session_add(const std::string& id, const std::shared_ptr<Gyanis::net::http::WSSession>& session)
    {
        LOG_INFO(g_logger) << "session_add id=" << id;
        std::unique_lock lock(m_mutex);
        m_sessions[id] = session;
    }

    void session_del(const std::string& id)
    {
        LOG_INFO(g_logger) << "session_add del=" << id;
        std::unique_lock lock(m_mutex);
        m_sessions.erase(id);
    }

    int32_t SendMessage(const std::shared_ptr<Gyanis::net::http::WSSession>& session
                        , const std::shared_ptr<ChatMessage>& msg)
    {
        LOG_INFO(g_logger) << msg->toString() << " - " << session;
        return session->sendMessage(msg->toString()) > 0 ? 0 : 1;
    }

    void session_notify(const std::shared_ptr<ChatMessage>& msg,
                        const std::shared_ptr<Gyanis::net::http::WSSession>& session = nullptr)
    {
        std::shared_lock lock(m_mutex);
        const auto sessions = m_sessions;
        lock.unlock();

        for (const auto& [fst, snd] : sessions)
        {
            if (snd == session)
            {
                continue;
            }
            SendMessage(snd, msg);
        }
    }

    ChatServlet::ChatServlet(): WSServlet("ChatServlet")
    {
    }

    int32_t ChatServlet::onConnect(const std::shared_ptr<Gyanis::net::http::HttpRequest>& header,
                                   const std::shared_ptr<Gyanis::net::http::WSSession>& session)
    {
        LOG_INFO(g_logger) << "onConnect " << session;
        return 0;
    }

    int32_t ChatServlet::onClose(const std::shared_ptr<Gyanis::net::http::HttpRequest>& header,
                                 const std::shared_ptr<Gyanis::net::http::WSSession>& session)
    {
        LOG_INFO(g_logger) << header->toString();
        const auto id = header->getHeader("$id");
        LOG_INFO(g_logger) << "onClose " << session << " id=" << id;
        if (!id.empty())
        {
            session_del(id);
            const auto nty = std::make_shared<ChatMessage>();
            nty->set("type", "user_leave");
            nty->set("time", Gyanis::base::Time2Str());
            nty->set("name", id);
            session_notify(nty);
        }
        return 0;
    }

    int32_t ChatServlet::handle(const std::shared_ptr<Gyanis::net::http::HttpRequest>& header,
                                const std::shared_ptr<Gyanis::net::http::WSFrameMessage>& msgx,
                                const std::shared_ptr<Gyanis::net::http::WSSession>& session)
    {
        LOG_INFO(g_logger) << "handle " << session
            << " opcode=" << msgx->getOpcode()
            << " data=" << msgx->getData();

        const auto msg = ChatMessage::Create(msgx->getData());
        auto id = header->getHeader("$id");
        if (!msg)
        {
            if (!id.empty())
            {
                std::unique_lock lock(m_mutex);
                m_sessions.erase(id);
            }
            return 1;
        }

        const auto message = std::make_shared<ChatMessage>();
        const auto type = msg->get("type");
        if (type == "login_request")
        {
            message->set("type", "login_response");
            const auto name = msg->get("name");
            if (name.empty())
            {
                message->set("result", "400");
                message->set("msg", "name is null");
                return SendMessage(session, message);
            }
            if (!id.empty())
            {
                message->set("result", "401");
                message->set("msg", "logined");
                return SendMessage(session, message);
            }
            if (session_exists(id))
            {
                message->set("result", "402");
                message->set("msg", "name exists");
                return SendMessage(session, message);
            }
            id = name;
            header->setHeader("$id", id);
            message->set("result", "200");
            message->set("msg", "ok");
            session_add(id, session);

            const auto nty = std::make_shared<ChatMessage>();
            nty->set("type", "user_enter");
            nty->set("time", Gyanis::base::Time2Str());
            nty->set("name", name);
            session_notify(nty, session);
            return SendMessage(session, message);
        }
        if (type == "send_request")
        {
            message->set("type", "send_response");
            const auto m = msg->get("msg");
            if (m.empty())
            {
                message->set("result", "500");
                message->set("msg", "msg is null");
                return SendMessage(session, message);
            }
            if (id.empty())
            {
                message->set("result", "501");
                message->set("msg", "not login");
                return SendMessage(session, message);
            }

            message->set("result", "200");
            message->set("msg", "ok");

            const auto nty = std::make_shared<ChatMessage>();
            nty->set("type", "msg");
            nty->set("time", Gyanis::base::Time2Str());
            nty->set("name", id);
            nty->set("msg", m);
            session_notify(nty, nullptr);
            return SendMessage(session, message);
            //TODO notify
        }
        return 0;
    }
}
