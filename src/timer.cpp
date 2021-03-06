#ifdef _WIN32
#	define WIN32_LEAN_AND_MEAN 1
#	define NOMINMAX
#	define _WIN32_WINNT 0x0500
#	include <Windows.h>
#elif defined(__APPLE__)
#	include <chrono>
#else
#	include <time.h>
#endif

#include "crossdata.hpp"

#include "timer.hpp"

double time_micros() {
	double ms = 0.0f;
#if defined(_WIN32)
	LARGE_INTEGER frq;
	if (QueryPerformanceFrequency(&frq)) {
		LARGE_INTEGER ctr;
		QueryPerformanceCounter(&ctr);
		ms = ((double)ctr.QuadPart / (double)frq.QuadPart) * 1.0e6;
	}
#elif defined(__APPLE__)
	using namespace std::chrono;
	auto t = high_resolution_clock::now();
	ms = (double)duration_cast<nanoseconds>(t.time_since_epoch()).count() * 1.0e-3;
#else
	struct timespec t;
	if (clock_gettime(CLOCK_MONOTONIC, &t) != 0) {
		clock_gettime(CLOCK_REALTIME, &t);
	}
	ms = (double)t.tv_nsec*1.0e-3 + (double)t.tv_sec*1.0e6;
#endif
	return ms;
}

void cStopWatch::alloc(int nsmps) {
	mpSmps = (double*)nxCore::mem_alloc(nsmps * sizeof(double), XD_FOURCC('t', 'i', 'm', 'e'));
	if (mpSmps) {
		mSmpsNum = nsmps;
	}
}

void cStopWatch::free() {
	if (mpSmps) {
		nxCore::mem_free(mpSmps);
		mpSmps = nullptr;
	}
	mSmpsNum = 0;
	mSmpIdx = 0;
}

void cStopWatch::begin() {
	mT = time_micros();
}

bool cStopWatch::end() {
	if (mSmpIdx < mSmpsNum) {
		mpSmps[mSmpIdx] = time_micros() - mT;
		++mSmpIdx;
	}
	return (mSmpIdx >= mSmpsNum);
}

void cStopWatch::reset() {
	mSmpIdx = 0;
}

static int smp_cmp(const void* pA, const void* pB) {
	double* pSmp1 = (double*)pA;
	double* pSmp2 = (double*)pB;
	double s1 = *pSmp1;
	double s2 = *pSmp2;
	if (s1 > s2) return 1;
	if (s1 < s2) return -1;
	return 0;
}

double calc_time_smps_median(double* pSmps, const size_t nsmps) {
	double med = 0.0;
	if (pSmps && nsmps) {
		::qsort(pSmps, nsmps, sizeof(double), smp_cmp);
		med = (nsmps & 1) ? pSmps[(nsmps - 1) / 2] : (pSmps[(nsmps / 2) - 1] + pSmps[nsmps / 2]) * 0.5;
	}
	return med;
}

double cStopWatch::median() {
	double val = 0;
	int n = mSmpIdx;
	if (n > 0 && mpSmps) {
		val = calc_time_smps_median(mpSmps, n);
	}
	return val;
}
