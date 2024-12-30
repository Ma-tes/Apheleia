#pragma once

#include <math.h>
#include <stdbool.h>

#define ZERO_VECTOR2        \
    (vector2) {             \
        .x = 0,             \
        .y = 0              \
    }                       \

#define IDENTITY_VECTOR2    \
    (vector2) {             \
        .x = 1,             \
        .y = 1              \
    }                       \

typedef struct vector2 {
    float x;
    float y;
} vector2;

vector2 add(const vector2 vector_one, const vector2 vector_two) {
    return (vector2){
        .x = vector_one.x + vector_two.x,
        .y = vector_one.y + vector_two.y
    };
}

vector2 sub(const vector2 vector_one, const vector2 vector_two) {
    return (vector2){
        .x = vector_one.x - vector_two.x,
        .y = vector_one.y - vector_two.y
    };
}

vector2 mul(const vector2 vector_one, const vector2 vector_two) {
    return (vector2){
        .x = vector_one.x * vector_two.x,
        .y = vector_one.y * vector_two.y
    };
}

vector2 mul_value(const vector2 vector, const float value) {
    return (vector2){
        .x = vector.x * value,
        .y = vector.y * value
    };
}

vector2 div_vector(const vector2 vector_one, const vector2 vector_two) {
    if(vector_two.x == 0 || vector_two.y == 0) {
        //TODO: Create relative log, with specific exception message.
        return ZERO_VECTOR2;
    }
    return (vector2){
        .x = vector_one.x / vector_two.x,
        .y = vector_one.y / vector_two.y
    };
}

double distance(const vector2 vector_one, const vector2 vector_two) {
    float difference_x = vector_one.x - vector_two.x;
    float difference_y = vector_one.y - vector_two.y;

    return sqrt((difference_y * difference_y) + (difference_x * difference_x));
}

bool equal(vector2 first_vector, vector2 second_vector) {
    return first_vector.x == second_vector.x &&
        first_vector.y == second_vector.y;
}

bool contains(vector2 vector, vector2 *vectors, const int count) {
    for (int i = 0; i < count; i++)
    {
        if(equal(vector, vectors[i])) {
            return true;
        }
    }
    return false;
}