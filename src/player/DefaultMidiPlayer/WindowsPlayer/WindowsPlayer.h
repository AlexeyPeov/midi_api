#ifndef MIDI_API_WINDOWS_PLAYER_H
#define MIDI_API_WINDOWS_PLAYER_H

#include <cstdint>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include <windows.h>

#include "../MessageInterface/MessageInterface.h"

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

        WindowsPlayer(const WindowsPlayer &) = delete;

        WindowsPlayer &operator=(const WindowsPlayer &) = delete;

        void openPort(uint32_t portNumber = 0) override;

        void closePort() override;

        void openVirtualPort(const std::string &portName) override;

        size_t getPortCount() override;

        std::string getPortName(uint32_t portNumber_ = 0) override;

        void sendMessage(const std::vector<uint8_t> *msg_) override;

    protected:
        void init() override;

    private:
        void *m_api_data_ptr = nullptr;
    };

}
#endif
