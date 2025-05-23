#include "MessageInterface.h"

namespace md {

    void MessageInterface::reset() {
        std::vector<uint8_t> message(3);
        message[2] = 0;

        for (int i = 176; i <= 191; i++) {
            message[0] = i;
            message[1] = 120;  // all sound off
            sendMessage(&message);

            message[1] = 121;  // reset all controllers
            sendMessage(&message);

            message[1] = 122;  // local control off
            sendMessage(&message);

            message[1] = 123;  // all notes off
            sendMessage(&message);
        }
    }
}
