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

void Events::CallEvent(EventType type)
{
    for (auto const &[name, ptr] : GlobalGameObjects.Get())
    {
        if (ptr->Enabled) // don't handle events on disabled gameobjects
        {
            ptr->HandleComponents(type); // handle events
        }
    }

    switch (type) // call the event
    {
    case EventType::Tick:
        GlobalApplication->Tick();
        break;
    case EventType::Redraw:
        GlobalApplication->OnDraw();
        break;
    case EventType::Mouse:
        GlobalApplication->OnMouseInput();
        break;
    case EventType::Resize:
        GlobalApplication->OnResize();
        break;

    default:
        break;
    }
}