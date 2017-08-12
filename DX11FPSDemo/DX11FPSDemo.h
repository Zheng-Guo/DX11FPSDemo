#pragma once
#include <windows.h>
#include <D2D1.h>
#include <DWrite.h>
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

class DX11Application
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
	ID2D1Factory* m_pDirect2dFactory;
	ID2D1HwndRenderTarget* m_pRenderTarget;
	ID2D1Factory* m_pD2DFactory;
	IDWriteFactory* m_pDWriteFactory;
	IDWriteTextFormat* m_pTextFormat;
	ID2D1SolidColorBrush* m_pBlackBrush;
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
	int killCount;
	wstring killCountDisplay;
	bool mouseLeftClicked;
	bool quitGame;
	void processInput();
	void update(float deltaTime);
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
public:
	DX11Application(int w = 100, int h = 100, HINSTANCE hi = nullptr, int sc = 0, LPCWSTR title = L"RenderingWindow");
	~DX11Application();
	void setWndProc(WNDPROC w) { wndProcFunc = w; }
	bool initWindow();
	bool initRendering();
	int run();
	void reset();
};