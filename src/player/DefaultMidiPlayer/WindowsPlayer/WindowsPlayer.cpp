#include "WindowsPlayer.h"

namespace md
{

WindowsPlayer::WindowsPlayer()
{
    init();
    openPort(0);
}

WindowsPlayer::WindowsPlayer(uint32_t initialPort_)
{
    init();
    openPort(initialPort_);
}

WindowsPlayer::~WindowsPlayer()
{
    // Close a connection if it exists.
    closePort();

    // Cleanup
    WinMidiData* data = static_cast<WinMidiData*>(m_api_data_ptr);
    delete data;
}

size_t WindowsPlayer::getPortCount()
{
    return midiOutGetNumDevs();
}

std::string WindowsPlayer::getPortName(uint32_t portNumber_)
{
    std::string stringName;
    uint32_t nDevices = midiOutGetNumDevs();
    if (portNumber_ >= nDevices)
    {
#ifndef CXXMIDI_QUIET
        std::cerr << "CxxMidi: invalid port number" << portNumber_ << std::endl;
#endif
        return stringName;
    }

    MIDIOUTCAPS deviceCaps;
    midiOutGetDevCaps(portNumber_, &deviceCaps, sizeof(MIDIOUTCAPS));

    // For some reason, we need to copy character by character with
    // UNICODE (thanks to Eduardo Coutinho!).
    // std::string stringName = std::string( deviceCaps.szPname );
    char nameString[MAXPNAMELEN];
    for (int i = 0; i < MAXPNAMELEN; ++i)
        nameString[i] = static_cast<char>(deviceCaps.szPname[i]);

    stringName = nameString;
    return stringName;
}

void WindowsPlayer::init()
{
    // We'll issue a warning here if no devices are available but not
    // throw an error since the user can plug something in later.
    size_t nDevices = getPortCount();

    if (!nDevices)
    {
        std::cerr << "WindowsPlayer::init getPortCount = 0\n";
    }
    // Save our api-specific connection information.
    auto* data = new WinMidiData;
    m_api_data_ptr = reinterpret_cast<void*>(data);
}

void WindowsPlayer::openPort(uint32_t portNumber)
{
    if (m_connected)
    {
        return;
    }

    size_t nDevices = getPortCount();

    if (!nDevices)
    {
        std::cerr << "WindowsPlayer::openPort: " << portNumber
                  << " getPortCount = 0\n";
    }

    auto* data = static_cast<WinMidiData*>(m_api_data_ptr);
    MMRESULT result = midiOutOpen(
        &data->outHandle,
        portNumber,
        (DWORD)NULL,
        (DWORD)NULL,
        CALLBACK_NULL
    );

    if (result)
    {
        std::cerr << "WindowsPlayer::openPort: " << portNumber
                  << " midiOutOpen error: " << result << '\n';
    }
    

    m_connected = true;
}

void WindowsPlayer::closePort()
{
    if (m_connected)
    {
        auto* data = static_cast<WinMidiData*>(m_api_data_ptr);
        midiOutReset(data->outHandle);
        midiOutClose(data->outHandle);
        m_connected = false;
    }
}

void WindowsPlayer::openVirtualPort(const std::string& /*portName_ */)
{
    // This function cannot be implemented for the WindowsPlayer MM MIDI API

#ifndef CXXMIDI_QUIET
    std::cerr
        << "CxxMidi: WindowsPlayer MM MIDI virtual ports can't be implemented"
        << std::endl;
#endif
}

void WindowsPlayer::sendMessage(const std::vector<uint8_t>* msg_)
{
    auto nBytes = static_cast<unsigned int>(msg_->size());
    if (nBytes == 0)
    {

#ifndef CXXMIDI_QUIET
        std::cerr << "CxxMidi: message argument is empty" << std::endl;
#endif
        return;
    }

    MMRESULT result;
    auto* data = static_cast<WinMidiData*>(m_api_data_ptr);
    if (msg_->at(0) == 0xF0)
    { // Sysex message
        // Allocate buffer for sysex data.
        char* buffer = reinterpret_cast<char*>(malloc(nBytes));
#ifndef CXXMIDI_QUIET
        if (buffer == nullptr)
            std::cerr << "CxxMidi: message malloc error " << std::endl;
#endif

        // Copy data to buffer
        for (unsigned int i = 0; i < nBytes; ++i)
            buffer[i] = msg_->at(i);

        // Create and prepare MIDIHDR structure
        MIDIHDR sysex;
        sysex.lpData = (LPSTR)buffer;
        sysex.dwBufferLength = nBytes;
        sysex.dwFlags = 0;
        result = midiOutPrepareHeader(data->outHandle, &sysex, sizeof(MIDIHDR));
        if (result != MMSYSERR_NOERROR)
        {
            free(buffer);
#ifndef CXXMIDI_QUIET
            std::cerr << "CxxMidi: error preparing sysex header" << std::endl;
#endif
        }

        // Send the message.
        result = midiOutLongMsg(data->outHandle, &sysex, sizeof(MIDIHDR));
        if (result != MMSYSERR_NOERROR)
        {
            free(buffer);
#ifndef CXXMIDI_QUIET
            std::cerr << "CxxMidi: error sending sysex message" << std::endl;
#endif
        }

        // Unprepare the buffer and MIDIHDR
        while (
            MIDIERR_STILLPLAYING
            == midiOutUnprepareHeader(data->outHandle, &sysex, sizeof(MIDIHDR))
        )
            Sleep(1);
        free(buffer);
    }
    else
    { // Channel or system message
        // Make sure the message size isn't too big
        if (nBytes > 3)
        {
#ifndef CXXMIDI_QUIET
            std::cerr << "CxxMidi: message size is greater than 3 bytes (and "
                         "not sysex)"
                      << std::endl;
#endif
            return;
        }

        // Pack MIDI bytes into double word
        DWORD packet;
        auto* ptr = (unsigned char*)&packet;
        for (unsigned int i = 0; i < nBytes; ++i)
        {
            *ptr = msg_->at(i);
            ++ptr;
        }

        // Send the message immediately
        result = midiOutShortMsg(data->outHandle, packet);
#ifndef CXXMIDI_QUIET
        if (result != MMSYSERR_NOERROR)
            std::cerr << "CxxMidi: error sending MIDI message" << std::endl;
#endif
    }
}

}