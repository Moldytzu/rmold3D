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

using namespace mold;

std::vector<std::thread*> threads; // threads vector

void mold::ThreadManager::Add(std::thread *t)
{
    threads.push_back(t); // push thread
}

void mold::ThreadManager::Reset()
{
    for(std::thread *thread : threads)
        delete thread;
    threads.clear(); // clear threads
}

void mold::ThreadManager::Wait()
{
    for(std::thread *thread : threads)
    {
        while(!thread->joinable()); // wait for the thread to be joinable
        thread->join();
    }
}