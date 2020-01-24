#include <memory>
#include <utility>

using MsgUID = unsigned long long;

using namespace std;

class Message {
    public:
        Message(int msgId);
    
        virtual ~Message() = default;
        Message(const Message&) = delete;
        Message& operator=(const Message&) = delete;

        virtual unique_ptr<Message> move();

        int getMessageId() const;

        MsgUID getUniqueId() const;
    protected:
        Message(Message&&) = default;
        Message& operator=(Message&&) = default;
    private:
        int messageId;
        MsgUID uniqueId;
};

template <typename PayloadType>
class DataMessage : public Message {
    public:
        template <typename ...Args>
        DataMessage(int msgId, Args&& ... args) : Message(msgId),
            payload(new PayloadType(std::forward<Args>(args) ...)) {}
        virtual ~DataMessage() = default;

        DataMessage(const DataMessage&) = delete;

        DataMessage& operator=(const DataMessage&) = delete;

        virtual unique_ptr<Message> move() override {
            return std::unique_ptr<Message>(new DataMessage<PayloadType>(std::move(*this)));
        }
        PayloadType& getPayload() const { return *payload; }
    protected:
        DataMessage(DataMessage&&) = default;
        DataMessage& operator=(DataMessage&&) = default;
    private:
        std::unique_ptr<PayloadType> payload;
};



