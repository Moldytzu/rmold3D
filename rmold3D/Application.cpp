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

void mold::Application::OnDraw() {}
void mold::Application::OnMouseInput() {}
void mold::Application::OnResize() {}
void mold::Application::Tick() {}

std::string mold::Application::Name()
{
    return "Game";
}

mold::Application::Application() : mold::Application::Application(800, 600) {}

mold::Application::Application(uint width, uint height)
{
    Init(width, height);
}

int main()
{
    GlobalApplication = BuildApplication(); // ask the sandbox to build an application
    Run();                                  // run the engine
    Destroy();                              // clean everything
    return 0;
}