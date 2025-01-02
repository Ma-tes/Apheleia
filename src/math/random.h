#pragma once

#include <stdlib.h> 

int random_range(int min, int max) { 
    return min + rand() / (RAND_MAX / (max - min + 1) + 1);
}