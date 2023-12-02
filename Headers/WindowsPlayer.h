#ifndef MIDI_API_WINDOWS_PLAYER_H
#define MIDI_API_WINDOWS_PLAYER_H

#include <windows.h>
#include <mmsystem.h>

#include <cstdint>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>


#include "MessageInterface.h"

namespace md {

    class WindowsPlayer : public MessageInterface {
    private:

        static const int RT_SYSEX_BUFFER_SIZE = 1024;
        static const int RT_SYSEX_BUFFER_COUNT = 4;

        // A structure to hold variables related to the CoreMIDI API
        // implementation.

        struct WinMidiData {
            HMIDIIN inHandle;    // Handle to Midi Input Device
            HMIDIOUT outHandle;  // Handle to Midi Output Device
            DWORD lastTime;
            LPMIDIHDR sysexBuffer[RT_SYSEX_BUFFER_COUNT];
        };

    public:

        WindowsPlayer();

        explicit WindowsPlayer(uint32_t initialPort);

        virtual ~WindowsPlayer();

        WindowsPlayer(const WindowsPlayer &);             // non-copyable

        WindowsPlayer &operator=(const WindowsPlayer &);  // non-copyable (assignment)

        void open_port(uint32_t portNumber = 0) override;

        void close_port() override;

        void open_virtual_port(const std::string &portName) override;

        size_t get_port_count() override;

        std::string get_port_name(unsigned int portNumber_ = 0) override;

        void send_message(const std::vector<uint8_t> *msg_) override;

    protected:
        void init() override;

    private:
        void *m_api_data_ptr = nullptr;
    };

}
#endif
