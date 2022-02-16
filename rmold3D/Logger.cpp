/*
    rmmold3D
    Copyright (C) 2022  Moldovan Alexandru

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include "mold.h"

using namespace mold::log;

#define NOW std::chrono::high_resolution_clock::now()
#define DIFFERENCE (double)(std::chrono::duration_cast<std::chrono::milliseconds>(NOW - engineStartedSince).count() / 1000.0f)

auto engineStartedSince = NOW;

void mold::log::Info(std::string str)
{
    printf("\033[37;1m[\033[34;1m%.2f\033[37;1m/\033[32;1mInfo\033[37;1m] %s\n", DIFFERENCE, str.c_str());
}

void mold::log::Warn(std::string str)
{
    printf("\033[37;1m[\033[34;1m%.2f\033[37;1m/\033[33;1mWarn\033[37;1m] %s\n", DIFFERENCE, str.c_str());
}

void mold::log::Error(std::string str)
{
    printf("\033[37;1m[\033[34;1m%.2f\033[37;1m/\033[31;1mError\033[37;1m] %s\n", DIFFERENCE, str.c_str());
}

void mold::log::Debug(std::string str)
{
    if(!settings::Debug) return; // return if debug isn't enabled
    printf("\033[37;1m[\033[34;1m%.2f\033[37;1m/\033[31;1mDebug\033[37;1m] %s\n", DIFFERENCE, str.c_str());
}

void mold::log::Fatal(std::string str)
{
    printf("\033[37;1m[\033[34;1m%.2f\033[37;1m/\033[31;1mFatal\033[37;1m] %s\n", DIFFERENCE, str.c_str());
    Destroy();
}
