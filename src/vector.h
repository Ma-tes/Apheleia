#pragma once
#include <math.h>

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