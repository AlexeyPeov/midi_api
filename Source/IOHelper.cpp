#include "IOHelper.h"

namespace md {

    uint32_t IOHelper::get_variable_len_quantity(std::ifstream &file) {
        uint32_t r = 0;
        uint8_t c;

        do {
            file.read(reinterpret_cast<char *>(&c), 1);
            r = (r << 7) + (c & 0x7f);
        } while (c & 0x80);

        return r;
    }

    size_t IOHelper::save_variable_len_quantity(
            std::ofstream &output_file,
            unsigned int val
    ) {
        size_t amount = 0;
        uint32_t vlq = val & 0x7f;

        // prepare
        while ((val >>= 7) > 0) {
            vlq <<= 8;
            vlq |= 0x80;
            vlq += (val & 0x7f);
        }

        // save
        while (true) {
            amount++;
            output_file.write(reinterpret_cast<char *>(&vlq), 1);
            if (vlq & 0x80) {
                vlq >>= 8;
            } else {
                break;
            }

        }

        return amount;
    }

    uint32_t IOHelper::extract_tempo(uint8_t v0, uint8_t v1, uint8_t v2) {

        union {
            uint32_t one_tempo;
            uint8_t tab_tempo[3];
        };
        one_tempo = 0;

        if (is_little_endian) {
            tab_tempo[0] = v2;
            tab_tempo[1] = v1;
            tab_tempo[2] = v0;
        } else {
            tab_tempo[0] = v0;
            tab_tempo[1] = v1;
            tab_tempo[2] = v2;
        }

        return one_tempo;

    }
} // mapi