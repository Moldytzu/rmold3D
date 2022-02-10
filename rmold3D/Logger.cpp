#include <rmold3D/mold.h>

using namespace mold::log;

#define NOW std::chrono::high_resolution_clock::now()
#define DIFFERENCE (double)(std::chrono::duration_cast<std::chrono::milliseconds>(NOW - engineStartedSince).count() / 1000.0f)

auto engineStartedSince = NOW;

void mold::log::Info(std::string str)
{
    printf("[%.2f/Info] %s\n", DIFFERENCE, str.c_str());
}

void mold::log::Warn(std::string str)
{
    printf("[%.2f/Warn] %s\n", DIFFERENCE, str.c_str());
}

void mold::log::Error(std::string str)
{
    printf("[%.2f/Error] %s\n", DIFFERENCE, str.c_str());
}

void mold::log::Fatal(std::string str)
{
    printf("[%.2f/Fatal] %s\n", DIFFERENCE, str.c_str());
    Destroy();
}