#include <axxegro/time/Time.hpp>

#include <allegro5/allegro.h>

double al::GetTime()
{
	return al_get_time();
}
void al::Sleep(double secs)
{
	al_rest(secs);
}