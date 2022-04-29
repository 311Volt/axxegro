#define AXXEGRO_TRUSTED

#include <axxegro/io/Mouse.hpp>

al::mouse::Cursor::Cursor(const Bitmap& bmp, Coord<int> focus)
	: Resource(al_create_mouse_cursor(bmp.ptr(), focus.x, focus.y))
{
	
}


unsigned al::mouse::GetNumAxes()
{
	return al_get_mouse_num_axes();
}
unsigned al::mouse::GetNumButtons()
{
	return al_get_mouse_num_buttons();
}

al::mouse::State al::mouse::GetState()
{
	State ret;
	al_get_mouse_state(&ret);
	return ret;
}

bool al::mouse::IsButtonDown(Btn btn)
{
	return IsButtonDown(GetState(), btn);
}
bool al::mouse::IsButtonDown(State state, Btn btn)
{
	return al_mouse_button_down(&state, int(btn));
}

bool al::mouse::SetPos(Coord<int> p)
{
	return al_set_mouse_xy(al_get_current_display(), int(p.x), int(p.y));
}
al::Coord<int> al::mouse::GetPos()
{
	State st = GetState();
	return {st.x, st.y};
}
al::Coord<int> al::mouse::GetDesktopPos()
{
	int x,y;
	al_get_mouse_cursor_position(&x, &y);
	return {x, y};
}

al::mouse::MouseEventSource axxMouseEventSource;
const al::EventSource& al::mouse::GetEventSource()
{
	return axxMouseEventSource;
}