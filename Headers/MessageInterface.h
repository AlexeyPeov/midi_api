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

        virtual void open_port(uint32_t port_num) = 0;

        virtual void close_port() = 0;

        virtual void open_virtual_port(const std::string &port_name) = 0;

        virtual size_t get_port_count() = 0;

        virtual std::string get_port_name(uint32_t port_num) = 0;

        virtual void send_message(const std::vector<uint8_t> *msg) = 0;

        void reset();

    protected:
        virtual void init() = 0;
        bool m_connected = false;
    };

}

#endif
