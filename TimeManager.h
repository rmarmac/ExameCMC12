#pragma once
#include <stdlib.h>
#include <profileapi.h>
#include "configs.h"


typedef struct {
    LARGE_INTEGER qpcFreq, qpcStart, qpcCurrent;
    double elapsed_time;
    double prev_elapsed_time;
} time_manager_t;

class TimeManager {
public:
    TimeManager();
    double Update();
private:
    time_manager_t time_manager;
};