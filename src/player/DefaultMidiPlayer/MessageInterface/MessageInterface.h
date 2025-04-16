#ifndef MIDI_API_MESSAGE_INTERFACE_H
#define MIDI_API_MESSAGE_INTERFACE_H

#include <cstdint>
#include <string>
#include <vector>

namespace md{

    class MessageInterface {
    public:

        MessageInterface() = default;

        virtual ~MessageInterface() = default;

        MessageInterface(const MessageInterface &) = delete;

        MessageInterface &operator=(const MessageInterface &) = delete;

        virtual void openPort(uint32_t port_num) = 0;

        virtual void closePort() = 0;

        virtual void openVirtualPort(const std::string &port_name) = 0;

        virtual size_t getPortCount() = 0;

        virtual std::string getPortName(uint32_t port_num) = 0;

        virtual void sendMessage(const std::vector<uint8_t> *msg) = 0;

        void reset();

    protected:
        virtual void init() = 0;
        bool m_connected = false;
    };

}

#endif
