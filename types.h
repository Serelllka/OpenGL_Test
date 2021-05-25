#pragma once
#include <cstdlib>

namespace rc_types {
    enum edges {
        UPP = 0,
        DWN,    // 1
        RGT,    // 2
        LFT,    // 3
        FRT,    // 4
        BCK     // 5
    };

    enum colors {
        WHITE = 0,
        YELLOW, // 1
        RED,    // 2
        ORANGE, // 3
        GREEN,  // 4
        BLUE,   // 5
        BLACK   // 6
    };

    enum dimensions {
        X = 0,
        Y = 1,
        Z = 2
    };
}