#ifndef MIDI_API_IOHELPER_H
#define MIDI_API_IOHELPER_H

#include <cstdint>
#include <fstream>
#include <iostream>

#include "constants.h"

namespace md {

    class IOHelper {
    public:
        template <typename type>
        static type swap(const type &u) {

            union {
                type u;
                unsigned char u8[sizeof(type)];
            } source, dest;

            source.u = u;

            for (size_t i = 0; i < sizeof(type); ++i){
                dest.u8[i] = source.u8[sizeof(type) - i - 1];
            }

            return dest.u;
        }

        template <typename type>
        static type readAs(std::ifstream &file) {
            type r;
            file.read(reinterpret_cast<char *>(&r), sizeof(type));
            if ((sizeof(type) > 1) && is_little_endian) r = swap<type>(r);
            return r;
        }

        template <typename type>
        static size_t writeAs(std::ofstream &file, type val) {
            std::streamsize size = sizeof(val);
            if ((sizeof(type) > 1) && is_little_endian) val = swap<type>(val);
            file.write(reinterpret_cast<char *>(&val), size);
            return size;
        }

        static uint32_t getVariableLenQuantity(std::ifstream &file);

        static size_t saveVariableLenQuantity(
                std::ofstream &output_file,
                unsigned int val
        );
    };

}

#endif //MIDI_API_IOHELPER_H
