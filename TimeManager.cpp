#include "TimeManager.h"

TimeManager::TimeManager() {
	time_manager.prev_elapsed_time = 0;
	QueryPerformanceFrequency(&(time_manager.qpcFreq));
	QueryPerformanceCounter(&(time_manager.qpcStart));
}

double TimeManager::Update() {
	double delta_t;
	QueryPerformanceCounter(&(time_manager.qpcCurrent));
	time_manager.elapsed_time = (time_manager.qpcCurrent.QuadPart - time_manager.qpcStart.QuadPart) / (float)time_manager.qpcFreq.QuadPart;
	delta_t = time_manager.elapsed_time - time_manager.prev_elapsed_time;
	delta_t = MAX_DELTA_T < delta_t ? MAX_DELTA_T : delta_t;
	time_manager.prev_elapsed_time = time_manager.elapsed_time;
	return delta_t;
}