#include <iostream>
#include <iomanip>

typedef enum {
    DOWN = 0x10,
    UP = 0x11,
    LEFT = 0x12,
    RIGHT = 0x13,
} State;

int main() {
    uint16_t state = 0xF1;
    std::cout << !((state ^ UP) & 0x3) << std::endl;
    std::cout << !((state ^ DOWN) & 0x3) << std::endl;
    std::cout << !((state ^ LEFT) & 0x3) << std::endl;
    std::cout << !((state ^ RIGHT) & 0x3) << std::endl;
    state = ((state & ~(1 << 0)) | (1 << 1) | (0 << 1));
    std::cout << !((state ^ UP) & 0x3) << std::endl;
    std::cout << !((state ^ DOWN) & 0x3) << std::endl;
    std::cout << !((state ^ LEFT) & 0x3) << std::endl;
    std::cout << !((state ^ RIGHT) & 0x3) << std::endl;
    std::cout << std::hex << state << std::endl;
    return 0;
}