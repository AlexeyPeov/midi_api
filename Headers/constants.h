#ifndef MIDI_API_CONSTANTS_H
#define MIDI_API_CONSTANTS_H

namespace md{
#if defined(_WIN32) || defined(_WIN64)
    // Windows is always little endian
    constexpr bool is_little_endian = true;
#elif defined(__APPLE__)
    #include <machine/endian.h>
    #if __DARWIN_BYTE_ORDER == __DARWIN_LITTLE_ENDIAN
        constexpr bool is_little_endian = true;
    #else
        constexpr bool is_little_endian = false;
    #endif
#elif defined(__linux__)
    #include <endian.h>
    #if __BYTE_ORDER == __LITTLE_ENDIAN
        constexpr bool is_little_endian = true;
    #else
        constexpr bool is_little_endian = false;
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

    enum class MidiMessageType {
        kUndefined = 0x00,
        // voice
        kNoteOff = 0x80,
        kNoteOn = 0x90,
        kNoteAftertouch = 0xa0,
        kControlChange = 0xb0,
        kProgramChange = 0xc0,      // size 1
        kChannelAftertouch = 0xd0,  // size 1
        kPitchWheel = 0xe0,

        // system common
        kSysExBegin = 0xf0,
        kMtcQuarterFrame = 0xf1,
        kSongPositionPointer = 0xf2,
        kSongSelect = 0xf3,
        kTuneRequest = 0xf6,
        kSysExEnd = 0xf7,

        // realtime
        kClock = 0xf8,
        kTick = 0xf9,
        kStart = 0xfa,
        kContinue = 0xfb,
        kStop = 0xfc,
        kActiveSense = 0xfe,
        kReset = 0xff,

        // Meta events
        kMeta = 0xff
    };

    enum class MidiMetaType {
        kSequenceNumber = 0x00,  // size 2
        kText = 0x01,
        kCopyright = 0x02,
        kTrackName = 0x03,
        kInstrumentName = 0x04,
        kLyrics = 0x05,
        kMarker = 0x06,
        kCuePoint = 0x07,
        kChannelPrefix = 0x20,  // size 1
        kOutputCable = 0x21,    // size 1
        kEndOfTrack = 0x2f,     // size 0
        kTempo = 0x51,          // size 3
        kSmpteOffset = 0x54,    // size 5
        kTimeSignature = 0x58,
        kKeySignature = 0x59
    };

    enum class Style {
        POP,
        JAZZ,
        NONE,
    };

    enum class NoteLenBias {
        NO,
        SLOOOOW,
        SLOW,
        MEDIUM,
        FAST,
        FASTER,
        FFFAAASSTER,
    };

    enum class FileCreationStatus{
        FAILED_UNKNOWN = 0,
        FAILED_FILE_EMPTY,
        SUCCESS,
    };

    enum class ProgressionStyle {
        NOTES = 0,
        ROOTS,
        NOTES_OF_CHORD,

        CHORD_PLAIN,
        CHORD_ASC,
        CHORD_DESC,
        CHORD_ASC_DESC,
        CHORD_ALTERNATING,
        CHORD_RAND_SEQ,
    };

    enum class Keys {
        C = 0,
        C_SHARP = 1,
        D_FLAT = 1,
        D = 2,
        D_SHARP = 3,
        E_FLAT = 3,
        E = 4,
        F = 5,
        F_SHARP = 5,
        G_FLAT = 6,
        G = 7,
        G_SHARP = 8,
        A_FLAT = 8,
        A = 9,
        A_SHARP = 10,
        B_FLAT = 10,
        B = 11,


    };

    enum class RomanChord {
        I = 0,
        i = 0,
        II = 2,
        ii = 2,
        III = 4,
        iii = 3,
        IV = 5,
        iv = 5,
        V = 7,
        v = 7,
        vi = 8,
        VI = 9,
        vii = 10,
        VII = 11,
    };

    enum class Mood {
        MAJOR,
        MINOR,
    };
}

#endif //MIDI_API_CONSTANTS_H
