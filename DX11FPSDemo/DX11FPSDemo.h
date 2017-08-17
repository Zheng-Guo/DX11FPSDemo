#pragma once
#include <windows.h>
#include <d3d11.h>
#include <dinput.h>
#include <xnamath.h>
#include "Clock.h"
#include "FreeLookCamera.h"
#include "RenderingWindow.h"
#include "SkyBox.h"
#include "MaterialManager.h"
#include "PlayerCharacter.h"
#include "ProjectileManager.h"
#include "LevelLoader.h"
#include "AIManager.h"
#include "ParticleSystemManager.h"
#include "ProgressBar.h"
#include "EnemyCharacterManager.h"
#include "AnimationManager.h"

class DX11FPSDemo
{
private:
	int width, height;
	float backgroundColor[4];
	HINSTANCE instanceHandle;
	int showCmd;
	LPCWSTR windowTitle;
	HWND windowHandle;
	WNDCLASS wc;
	WNDPROC wndProcFunc;
	RenderingWindow *window;
	ID3D11Device *device;
	IDirectInputDevice8* DIKeyboard;
	IDirectInputDevice8* DIMouse;
	DIMOUSESTATE mouseLastState;
	LPDIRECTINPUT8 DirectInput;
	HDC deviceContextHandle;
	Clock clock;
	FreeLookCamera freeLookCamera;
	SkyBox *skyBox;
	MaterialManager *materialManager;
	PlayerCharacter *playerCharacter;
	ProjectileManager *projectileManager;
	LevelLoader *levelLoader;
	Level *currentLevel;
	NavigationManager *navigationManager;
	AIManager *aiManager;
	ParticleSystemManager *particleSystemManager;
	ProgressBar *progressBar;
	ProgressBar *mana;
	EnemyCharacterManager *enemyCharacterManager;
	AnimationManager *animationManager;
	int killCount;
	wstring killCountDisplay;
	bool mouseLeftClicked;
	bool quitGame;
	void processInput();
	void update(float deltaTime);
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
public:
	DX11FPSDemo(int w = 100, int h = 100, HINSTANCE hi = nullptr, int sc = 0, LPCWSTR title = L"RenderingWindow");
	~DX11FPSDemo();
	void setWndProc(WNDPROC w) { wndProcFunc = w; }
	bool initWindow();
	bool initRendering();
	int run();
	void reset();
};