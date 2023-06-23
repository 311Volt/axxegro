
#include <axxegro/axxegro.hpp>

#include <iostream>

int main()
{
	al::Config cfg("data/samplecfg.ini");

	for(auto& section: cfg.sections()) {
		std::cout << al::Format("processing section %s\n", section.name());
		for(auto& [key, value]: section) {
			std::string valStr = value;
			std::cout << al::Format("%s -> %s\n", key.path().c_str(), valStr.c_str());
		}
	}

	al::Vec2i dispSize {cfg.get<int>("Display.Width").value(), cfg.get<int>("Display.Height").value()};
	float refreshRate = cfg.get<float>("Display.RefreshRate").value();

	std::cout << al::Format("old mode: %d x %d, %.2f Hz\n", dispSize.x, dispSize.y, refreshRate);

	cfg.set("Display.Width", 3840);
	cfg.set("Display.Height", 2160);
	cfg.set("Display.RefreshRate", 144.0);

	dispSize = {cfg.get<int>("Display.Width").value(), cfg.get<int>("Display.Height").value()};
	refreshRate = cfg.get<float>("Display.RefreshRate").value();

	std::cout << al::Format("new mode: %d x %d, %.2f Hz", dispSize.x, dispSize.y, refreshRate);

}