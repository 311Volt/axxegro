#include <axxegro/axxegro.hpp>

#include <allegro5/allegro.h>

int main()
{
	al::FullInit();
	al::Display disp(800, 600);
	disp.resize(1024, 768);
	al_rest(1.0);
	return 0;
}