#include "DX11FPSDemo.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPreInstance, PSTR pCmdLine, int nShowCmd)
{
	DX11FPSDemo app(800, 800, hInstance, nShowCmd, L"FPS Demo");
	if (!app.initWindow())
		return EXIT_FAILURE;
	if (!app.initRendering())
		return EXIT_FAILURE;
	return app.run();
}
