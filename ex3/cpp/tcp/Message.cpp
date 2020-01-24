#include "Message.hpp"

#include <atomic>

MsgUID generateUniqueId() {
    static std::atomic<MsgUID> i(0);
    return ++i;
}

Message::Message(int msgId) : messageId(msgId), uniqueId(generateUniqueId()) {}

std::unique_ptr<Message> Message::move() {
    return std::unique_ptr<Message>(new Message(std::move(*this)));
}

int Message::getMessageId() const { return messageId; }

MsgUID Message::getUniqueId() const { return uniqueId; }

