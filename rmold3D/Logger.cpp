#include <rmold3D/mold.h>

#define NOW std::chrono::high_resolution_clock::now()
#define DIFFERENCE (double)(std::chrono::duration_cast<std::chrono::milliseconds>(NOW - engineStartedSince).count() / 1000.0f)

auto engineStartedSince = NOW;

void mold::log::Info(std::string fmt, ...)
{
    va_list list;
    va_start(list, fmt); //generate va_list from arguments
    printf("[%.2f/Info] ", DIFFERENCE);
    vprintf(fmt.c_str(), list); //use user's va_list so we can use printf style formating
    printf("\n");
    va_end(list); //end list
}

void mold::log::Warn(std::string fmt, ...)
{
    va_list list;
    va_start(list, fmt); //generate va_list from arguments
    printf("[%.2f/Warn] ", DIFFERENCE);
    vprintf(fmt.c_str(), list); //use user's va_list so we can use printf style formating
    printf("\n");
    va_end(list); //end list
}

void mold::log::Error(std::string fmt, ...)
{
    va_list list;
    va_start(list, fmt); //generate va_list from arguments
    printf("[%.2f/Error] ", DIFFERENCE);
    vprintf(fmt.c_str(), list); //use user's va_list so we can use printf style formating
    printf("\n");
    va_end(list); //end list
}