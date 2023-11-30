#include "../Headers/IOHelper.h"

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
    )
    {
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
            if (vlq & 0x80){
                vlq >>= 8;
            }
            else{
                break;
            }

        }

        return amount;
    }
} // mapi