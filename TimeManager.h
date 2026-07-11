#pragma once
#include <stdlib.h>
#include <profileapi.h>
#include "configs.h"

//-------------------------------------------------------------------------------------------------------
/// Estrutura básica para o time manager
typedef struct {
    LARGE_INTEGER qpcFreq, qpcStart, qpcCurrent;
    double elapsed_time;
    double prev_elapsed_time;
} time_manager_t;
//-------------------------------------------------------------------------------------------------------
/// Classe do time manager, para poder reservar as informações ao longo do tempo de vida da variável instanciada.
class TimeManager {
public:
    TimeManager();
    double Update();
private:
    time_manager_t time_manager;
};