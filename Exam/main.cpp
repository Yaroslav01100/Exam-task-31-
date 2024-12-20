#include <iostream>
#include <Windows.h>
#include <process.h>

#define NUM_THREADS 5
#define NUM_ITERATIONS 10000

long long sharedCounter = 0;

CRITICAL_SECTION criticalSection;

// Функція потоку
unsigned __stdcall ThreadFunc(void* pArguments) {
    int threadNum = *(int*)pArguments;

    for (int i = 0; i < NUM_ITERATIONS; ++i) {
        EnterCriticalSection(&criticalSection);

        sharedCounter++;

        LeaveCriticalSection(&criticalSection);
    }

    std::cout << "Thread " << threadNum << ": Counter = " << sharedCounter << std::endl;

    return 0;
}


int main() {
    InitializeCriticalSection(&criticalSection);


    HANDLE hThreads[NUM_THREADS];

    int threadNumbers[NUM_THREADS];


    for (int i = 0; i < NUM_THREADS; ++i) {
        threadNumbers[i] = i + 1;
        hThreads[i] = (HANDLE)_beginthreadex(nullptr, 0, ThreadFunc, &threadNumbers[i], 0, nullptr);
        if (hThreads[i] == nullptr) {
            std::cerr << "Error creating thread " << i + 1 << std::endl;
            return 1;
        }
    }

    WaitForMultipleObjects(NUM_THREADS, hThreads, TRUE, INFINITE);

    DeleteCriticalSection(&criticalSection);

    for (int i = 0; i < NUM_THREADS; i++) {
        CloseHandle(hThreads[i]);
    }

    return 0;
}