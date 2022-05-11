#define AXXEGRO_TRUSTED

#include <axxegro/io/Mouse.hpp>

al::MouseCursor::MouseCursor(const Bitmap& bmp, Coord<int> focus)
	: Resource(al_create_mouse_cursor(bmp.ptr(), focus.x, focus.y))
{
	
}


unsigned al::GetNumAxes()
{
	return al_get_mouse_num_axes();
}
unsigned al::GetNumButtons()
{
	return al_get_mouse_num_buttons();
}

al::MouseState al::GetMouseState()
{
	MouseState ret;
	al_get_mouse_state(&ret);
	return ret;
}

bool al::IsButtonDown(al::MouseButton btn)
{
	return GetMouseState().isButtonDown(btn);
}

bool al::SetMousePos(Coord<int> p)
{
	return al_set_mouse_xy(al_get_current_display(), int(p.x), int(p.y));
}
al::Coord<int> al::GetMousePos()
{
	MouseState st = GetMouseState();
	return {st.x, st.y};
}
al::Coord<int> al::GetMouseDesktopPos()
{
	int x,y;
	al_get_mouse_cursor_position(&x, &y);
	return {x, y};
}

al::MouseEventSource axxMouseEventSource;
const al::EventSource& al::GetMouseEventSource()
{
	return axxMouseEventSource;
}