#include "Protocol.h"
#include <nlohmann/json.hpp>

#include "base/Log.h"
namespace Chat {

    static auto g_logger = LOG_NAME("system");

    ChatMessage::ptr ChatMessage::Create(const std::string& value)
    {
        nlohmann::json jsons;
        try
        {
            jsons = nlohmann::json::parse(value);
        }
        catch (const nlohmann::json::parse_error& e)
        {
            LOG_ERROR(g_logger) << "json parse error: %s" << e.what();
            return nullptr;
        }

        auto result = std::make_shared<ChatMessage>();
        for (auto& [key, value] : jsons.items())
        {
            LOG_INFO(g_logger) << "key: " << key << " value: " << value;
            result->m_datas[key] = value.get<std::string>();
        }
        return result;
    }

    ChatMessage::ChatMessage() = default;

    std::string ChatMessage::get(const std::string& name)
    {
        const auto it = m_datas.find(name);
        return it == m_datas.end() ? "" : it->second;
    }

    void ChatMessage::set(const std::string& name, const std::string& value)
    {
        m_datas[name] = value;
    }

    std::string ChatMessage::toString() const
    {
        nlohmann::json jsons;
        for (const auto& [fst, snd] : m_datas)
        {
            jsons[fst] = snd;
        }
        return jsons.dump();
    }
}