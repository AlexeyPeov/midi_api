#ifndef MIDI_API_CONSTANTS_H
#define MIDI_API_CONSTANTS_H

namespace md{
#if defined(_WIN32) || defined(_WIN64)
    // Windows is always little endian
    constexpr const bool is_little_endian = true;
#elif defined(__APPLE__)
    #include <machine/endian.h>
    #if __DARWIN_BYTE_ORDER == __DARWIN_LITTLE_ENDIAN
        constexpr const bool is_little_endian = true;
    #else
        constexpr const bool is_little_endian = false;
    #endif
#elif defined(__linux__)
    #include <endian.h>
    #if __BYTE_ORDER == __LITTLE_ENDIAN
        constexpr const bool is_little_endian = true;
    #else
        constexpr const bool is_little_endian = false;
    #endif
#else
    #error "Unknown platform"
#endif
    enum class InstrumentName {
        kAcousticGrandPiano = 0,
        kBrightAcousticPiano = 1,
        kElectricGrandPiano = 2,
        kHonkytonkPiano = 3,
        kElectricPiano1 = 4,
        kElectricPiano2 = 5,
        kHarpsichord = 6,
        kClavi = 7,

        kCelesta = 8,
        kGlockenspiel = 9,
        kMusicBox = 10,
        kVibraphone = 11,
        kMarimba = 12,
        kXylophone = 13,
        kTubularBells = 14,
        kDulcimer = 15,

        kDrawbarOrgan = 16,
        kPercussiveOrgan = 17,
        kRockOrgan = 18,
        kChurchOrgan = 19,
        kReedOrgan = 20,
        kAccordion = 21,
        kHarmonica = 22,
        kTangoAccordion = 23,

        kAcousticGuitarNylon = 24,
        kAcousticGuitarSteel = 25,
        kElectricGuitarJazz = 26,
        kElectricGuitarClean = 27,
        kElectricGuitarMuted = 28,
        kOverdrivenGuitar = 29,
        kDistortionGuitar = 30,
        kGuitarHarmonics = 31,
        kAcousticBass = 32,
        kElectricBassFinger = 33,
        kElectricBassPick = 34,
        kFretlessBass = 35,
        kSlapBass1 = 36,
        kSlapBass2 = 37,
        kSynthBass1 = 38,
        kSynthBass2 = 39,

        kViolin = 40,
        kViola = 41,
        kCello = 42,
        kContrabass = 43,
        kTremoloStrings = 44,
        kPizzacatoStrings = 45,
        kOrchestralHarp = 46,
        kTimpani = 47,

        kStringEnsemble1 = 48,
        kStringEnsemble2 = 49,
        kSynthstrings1 = 50,
        kSynthstrings2 = 51,
        kChoirAahs = 52,
        kVoiceOohs = 53,
        kSynthVoice = 54,
        kOrchestraHit = 55,

        kTrumpet = 56,
        kTrombone = 57,
        kTuba = 58,
        kMutedTrumped = 59,
        kFrenchHorn = 60,
        kBrassSection = 61,
        kSynthbrass1 = 62,
        kSynthbrass2 = 63,
        kSopranoSax = 64,
        kAltoSax = 65,
        kTenorSax = 66,
        kBaritoneSax = 67,
        kOboe = 68,
        kEnglishHorn = 69,
        kBassoon = 70,
        kClarinet = 71,

        kPiccolo = 72,
        kFlute = 73,
        kRecorder = 74,
        kPanFlute = 75,
        kBlownBottle = 76,
        kShakuhachi = 77,
        kWhistle = 78,
        kOcarina = 79,

        kLeadSquare = 80,
        kLeadSawtooth = 81,
        kLeadCalliope = 82,
        kLeadChiff = 83,
        kLeadCharang = 84,
        kLeadVoice = 85,
        kLeadFifths = 86,
        kLeadBass = 87,

        kPadNewAge = 88,
        kPadWarm = 89,
        kPadPolysynth = 90,
        kPadChoir = 91,
        kPadBowed = 92,
        kPadMetallic = 93,
        kPadHalo = 94,
        kPadSweep = 95,

        kFxTrain = 96,
        kFxSoundtrack = 97,
        kFxCrystal = 98,
        kFxAtmosphere = 99,
        kFxBrightness = 100,
        kFxGoblins = 101,
        kFxEchoes = 102,
        kFxScifi = 103,

        kSitar = 104,
        kBanjo = 105,
        kShamisen = 106,
        kKoto = 107,
        kKalimba = 108,
        kBagpipe = 109,
        kFiddle = 110,
        kShanai = 111,

        kTinkleBell = 112,
        kAgogo = 113,
        kSteelDrums = 114,
        kWoodblocks = 115,
        kTaikoDrum = 116,
        kMelodicDrum = 117,
        kSynthDrum = 118,
        kReverseCymbal = 119,

        kGuitarFretNoise = 120,
        kBreathNoise = 121,
        kSeashore = 122,
        kBirdTweet = 123,
        kTelephoneRing = 124,
        kHelicopter = 125,
        kApplause = 126,
        kGunshot = 127,

        kAcousticBassDrum = 35,  // percussion (notes played on channel 10)
        kBassDrum1 = 36,
        kSideStick = 37,
        kAcousticSnare = 38,
        kHandClap = 39,
        kElectricSnare = 40,
        kLowFloorTom = 41,
        kClosedHiHat = 42,
        kHighFloorTom = 43,
        kPedalHiHat = 44,
        kLowTom = 45,
        kOpenHiHat = 46,
        kLowMidTom = 47,
        kHighMidTom = 48,
        kCrashCymbal1 = 49,
        kHighTom = 50,
        kRideCymbal1 = 51,
        kChineseCymbal = 52,
        kRideBell = 53,
        kTambourine = 54,
        kSplashCymbal = 55,
        kCowbell = 56,
        kCrashCymbal2 = 57,
        kVibraslap = 58,
        kRideCymbal2 = 59,
        kHiBongo = 60,
        kLowBongo = 61,
        kMuteHiConga = 62,
        kOpenHiConga = 63,
        kLowConga = 64,
        kHighTimbale = 65,
        kLowTimbale = 66,
        kHighAgogo = 67,
        kLowAgogo = 68,
        kCabasa = 69,
        kMaracas = 70,
        kShortWhistle = 71,
        kLongWhistle = 72,
        kShortGuiro = 73,
        kLongGuiro = 74,
        kClaves = 75,
        kHiWoodBlock = 76,
        kLowWoodBlock = 77,
        kMuteCuica = 78,
        kOpenCuica = 79,
        kMuteTriangle = 80,
        kOpenTriangle = 81,

        kUndefined = 128
    };

    enum msg_t : uint8_t {
        UNDEFINED_MSG_T = 0x00,
        // voice
        NoteOff = 0x80,
        NoteOn = 0x90,
        NoteAftertouch = 0xa0,
        ControlChange = 0xb0,
        ProgramChange = 0xc0,      // size 1
        ChannelAftertouch = 0xd0,  // size 1
        PitchWheel = 0xe0,

        // system common
        SysExBegin = 0xf0,
        MtcQuarterFrame = 0xf1,
        SongPositionPointer = 0xf2,
        SongSelect = 0xf3,
        TuneRequest = 0xf6,
        SysExEnd = 0xf7,

        // realtime
        Clock = 0xf8,
        Tick = 0xf9,
        Start = 0xfa,
        Continue = 0xfb,
        Stop = 0xfc,
        ActiveSense = 0xfe,
        Reset = 0xff,

        // Meta events
        Meta = 0xff
    };

    enum meta_t : uint8_t {
        SequenceNumber = 0x00,  // size 2
        Text = 0x01,
        Copyright = 0x02,
        TrackName = 0x03,
        InstrumentName = 0x04,
        Lyrics = 0x05,
        Marker = 0x06,
        CuePoint = 0x07,
        ChannelPrefix = 0x20,  // size 1
        OutputCable = 0x21,    // size 1
        EndOfTrack = 0x2f,     // size 0
        Tempo = 0x51,          // size 3
        SmpteOffset = 0x54,    // size 5
        TimeSignature = 0x58,
        KeySignature = 0x59,
        NO = 0x66
    };

    enum note : uint8_t {
        UNDEFINED_NOTE = 128,

        C0 = 0,
        C0s = 1,
        D0b = 1,
        D0 = 2,
        D0s = 3,
        E0b = 3,
        E0 = 4,
        F0 = 5,
        F0s = 6,
        G0b = 6,
        G0 = 7,
        G0s = 8,
        A0b = 8,
        A0 = 9,
        A0s = 10,
        B0b = 10,
        B0 = 11,
        C1 = 12,
        C1s = 13,
        D1b = 13,
        D1 = 14,
        D1s = 15,
        E1b = 15,
        E1 = 16,
        F1 = 17,
        F1s = 18,
        G1b = 18,
        G1 = 19,
        G1s = 20,
        A1b = 20,
        A1 = 21,
        A1s = 22,
        B1b = 22,
        B1 = 23,
        C2 = 24,
        C2s = 25,
        D2b = 25,
        D2 = 26,
        D2s = 27,
        E2b = 27,
        E2 = 28,
        F2 = 29,
        F2s = 30,
        G2b = 30,
        G2 = 31,
        G2s = 32,
        A2b = 32,
        A2 = 33,
        A2s = 34,
        B2b = 34,
        B2 = 35,
        C3 = 36,
        C3s = 37,
        D3b = 37,
        D3 = 38,
        D3s = 39,
        E3b = 39,
        E3 = 40,
        F3 = 41,
        F3s = 42,
        G3b = 42,
        G3 = 43,
        G3s = 44,
        A3b = 44,
        A3 = 45,
        A3s = 46,
        B3b = 46,
        B3 = 47,
        C4 = 48,
        C4s = 49,
        D4b = 49,
        D4 = 50,
        D4s = 51,
        E4b = 51,
        E4 = 52,
        F4 = 53,
        F4s = 54,
        G4b = 54,
        G4 = 55,
        G4s = 56,
        A4b = 56,
        A4 = 57,
        A4s = 58,
        B4b = 58,
        B4 = 59,
        C5 = 60,
        C5s = 61,
        D5b = 61,
        D5 = 62,
        D5s = 63,
        E5b = 63,
        E5 = 64,
        F5 = 65,
        F5s = 66,
        G5b = 66,
        G5 = 67,
        G5s = 68,
        A5b = 68,
        A5 = 69,
        A5s = 70,
        B5b = 70,
        B5 = 71,
        C6 = 72,
        C6s = 73,
        D6b = 73,
        D6 = 74,
        D6s = 75,
        E6b = 75,
        E6 = 76,
        F6 = 77,
        F6s = 78,
        G6b = 78,
        G6 = 79,
        G6s = 80,
        A6b = 80,
        A6 = 81,
        A6s = 82,
        B6b = 82,
        B6 = 83,
        C7 = 84,
        C7s = 85,
        D7b = 85,
        D7 = 86,
        D7s = 87,
        E7b = 87,
        E7 = 88,
        F7 = 89,
        F7s = 90,
        G7b = 90,
        G7 = 91,
        G7s = 92,
        A7b = 92,
        A7 = 93,
        A7s = 94,
        B7b = 94,
        B7 = 95,
        C8 = 96,
        C8s = 97,
        D8b = 97,
        D8 = 98,
        D8s = 99,
        E8b = 99,
        E8 = 100,
        F8 = 101,
        F8s = 102,
        G8b = 102,
        G8 = 103,
        G8s = 104,
        A8b = 104,
        A8 = 105,
        A8s = 106,
        B8b = 106,
        B8 = 107,
        C9 = 108,
        C9s = 109,
        D9b = 109,
        D9 = 110,
        D9s = 111,
        E9b = 111,
        E9 = 112,
        F9 = 113,
        F9s = 114,
        G9b = 114,
        G9 = 115,
        G9s = 116,
        A9b = 116,
        A9 = 117,
        A9s = 118,
        B9b = 118,
        B9 = 119,
        C10 = 120,
        C10s = 121,
        D10b = 121,
        D10 = 122,
        D10s = 123,
        E10b = 123,
        E10 = 124,
        F10 = 125,
        F10s = 126,
        G10b = 126,
        G10 = 127
    };

}

#endif //MIDI_API_CONSTANTS_H
