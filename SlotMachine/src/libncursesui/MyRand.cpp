#include <cstdlib>
#include <ctime>    
#include "MyRand.h"

namespace MyRand {

void seed() {
    srand(static_cast<unsigned int>(time(0)));
}

int randomInt(int min, int max) {
    return min + static_cast<int>(rand() % (max - min + 1));
}

float randomFloat01() {
    return static_cast<float>(rand()) / (RAND_MAX + 1.0);
}

}; // namespace MyRand
