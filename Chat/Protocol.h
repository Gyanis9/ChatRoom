#ifndef PROTOCOL_H
#define PROTOCOL_H
#include <string>
#include <memory>
#include <unordered_map>

namespace Chat
{
    class ChatMessage
    {
    public:
        using ptr = std::shared_ptr<ChatMessage>;

        static ptr Create(const std::string& value);

        ChatMessage();

        std::string get(const std::string& name);

        void set(const std::string& name, const std::string& value);

        std::string toString() const;

    private:
        std::unordered_map<std::string, std::string> m_datas;
    };
}

#endif
