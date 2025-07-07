// sensor_data.h
#include <stdint.h>
#ifndef SENSOR_DATA_H
#define SENSOR_DATA_H

typedef struct {
    int16_t ax, ay, az;
    int16_t gx, gy, gz;
} sensor_data_t;

#endif
