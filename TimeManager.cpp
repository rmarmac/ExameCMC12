#include "TimeManager.h"
//-------------------------------------------------------------------------------------------------------
/// Construtor do time manager
TimeManager::TimeManager() {
	time_manager.prev_elapsed_time = 0;
	QueryPerformanceFrequency(&(time_manager.qpcFreq));
	QueryPerformanceCounter(&(time_manager.qpcStart));
}
//-------------------------------------------------------------------------------------------------------
/// Time manager atua como um gerenciador do tempo decorrido entre cada invocação de Update. Esse método é específico
/// do hardware, que lê o relógio TSC interno do processador. (Pode não funcionar em algumas máquinas, mas é muito bom pra cronometrar)
double TimeManager::Update() {
	double delta_t;
	QueryPerformanceCounter(&(time_manager.qpcCurrent));
	time_manager.elapsed_time = (time_manager.qpcCurrent.QuadPart - time_manager.qpcStart.QuadPart) / (double)time_manager.qpcFreq.QuadPart;
	delta_t = time_manager.elapsed_time - time_manager.prev_elapsed_time;
	delta_t = MAX_DELTA_T < delta_t ? MAX_DELTA_T : delta_t;
	time_manager.prev_elapsed_time = time_manager.elapsed_time;
	return delta_t;
}