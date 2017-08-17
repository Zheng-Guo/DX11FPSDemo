#include <sstream>
#include "DX11FPSDemo.h"
#include "CollisionDetection.h"
#include "ExplosionParticleSystem.h"

DX11FPSDemo::DX11FPSDemo(int w, int h, HINSTANCE hi, int sc, LPCWSTR title) :width(w),
height(h),
instanceHandle(hi),
showCmd(sc),
windowTitle(title),
windowHandle(nullptr),
wc({ 0 }),
window(nullptr),
device(nullptr),
freeLookCamera(XMFLOAT3(0.f, 0.f, 10.f), XMFLOAT3(0.f, 0.f, -1.f), XMFLOAT3(0.f, 1.f, 0.f), 15.f, 1.f),
mouseLeftClicked(false),
quitGame(false)
{

}

DX11FPSDemo::~DX11FPSDemo()
{
	delete skyBox;
	delete materialManager;
	delete playerCharacter;
	delete projectileManager;
	delete levelLoader;
	delete currentLevel;
	delete navigationManager;
	delete aiManager;
	delete particleSystemManager;
	delete progressBar;
	delete enemyCharacterManager;
	delete mana;
	delete animationManager;
	DIKeyboard->Release();
	DIMouse->Release();
	delete window;
	UnregisterClass(L"DX11ApplicationClass", wc.hInstance);
}

bool DX11FPSDemo::initWindow()
{
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = instanceHandle;
	wc.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName = nullptr;
	wc.lpszClassName = L"DX11ApplicationClass";

	if (!RegisterClass(&wc))
	{
		MessageBox(nullptr, L"RegisterClass FAILED", nullptr, 0);
		return false;
	}

	windowHandle = CreateWindow(
		L"DX11ApplicationClass",
		windowTitle,
		WS_OVERLAPPEDWINDOW,
		0,
		0,
		width,
		height,
		nullptr,
		nullptr,
		instanceHandle,
		nullptr
	);

	if (windowHandle == nullptr)
	{
		MessageBox(nullptr, L"CreateWindow FAILED", nullptr, 0);
		return false;
	}

	ShowWindow(windowHandle, showCmd);
	UpdateWindow(windowHandle);

	DirectInput8Create(instanceHandle, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&DirectInput, nullptr);
	DirectInput->CreateDevice(GUID_SysKeyboard, &DIKeyboard, nullptr);
	DirectInput->CreateDevice(GUID_SysMouse, &DIMouse, nullptr);
	DIKeyboard->SetDataFormat(&c_dfDIKeyboard);
	DIKeyboard->SetCooperativeLevel(windowHandle, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	DIMouse->SetDataFormat(&c_dfDIMouse);
	DIMouse->SetCooperativeLevel(windowHandle, DISCL_EXCLUSIVE | DISCL_NOWINKEY | DISCL_FOREGROUND);

	window = new RenderingWindow(width, height, windowHandle);
	if (!window->initD3D())
	{
		MessageBox(nullptr, L"D3D initialization FAILED", nullptr, 0);
		return false;
	}
	window->initRendering();
	device = window->getDevice();

	return true;
}

bool DX11FPSDemo::initRendering()
{
	MaterialManager::setDevice(device);
	materialManager = MaterialManager::getInstance();

	projectileManager = ProjectileManager::getInstance();
	projectileManager->setMaterial(materialManager->getRuby());

	particleSystemManager = ParticleSystemManager::getInstance();

	levelLoader = new LevelLoader(device);
	currentLevel = levelLoader->loadLevelFromFile("Level_map.txt");
	currentLevel->setWallMaterial(materialManager->getBrickWallMaterial());
	currentLevel->setFloorMaterial(materialManager->getStoneFloor());

	navigationManager = NavigationManager::getInstance();
	navigationManager->setMap(currentLevel);

	aiManager = AIManager::getInstance();

	animationManager = AnimationManager::getInstance();
	animationManager->loadAnimation("walk","walk.txt");

	enemyCharacterManager = EnemyCharacterManager::getInstance();
	enemyCharacterManager->setDevice(device);
	enemyCharacterManager->setAvailableGrids(currentLevel);
	enemyCharacterManager->setCharacterMaterial(materialManager->getBronze());
	enemyCharacterManager->setWeaponMaterial(materialManager->getEmerald());
	enemyCharacterManager->setSpawnInterval(5, 3);
	enemyCharacterManager->setSpawnRange(10.f);

	skyBox = new SkyBox(device);
	skyBox->setScale(200.f, 200.f, 200.f);

	playerCharacter = new PlayerCharacter(device);
	playerCharacter->setMaterial(materialManager->getSteel());
	playerCharacter->setPosition(25.f, 1.f, 26.f);
	playerCharacter->setLinearSpeed(15.f);
	playerCharacter->setProjectileOffset(XMFLOAT3(0.f, -0.3f, 0.5f));
	playerCharacter->setFireballSpeed(20.f);
	playerCharacter->setMana(100.f);
	playerCharacter->setManaRecoveryRate(2.f);
	playerCharacter->setFireballManaCost(10.f);

	enemyCharacterManager->setPlayerCharacter(playerCharacter);

	progressBar = new ProgressBar(device);
	progressBar->setMaterial(materialManager->getrectangleShape());
	progressBar->setBackgroundColor(1.f, 1.f, 1.f, 1.f);
	progressBar->setForegroundColor(0.f, 1.f, 0.f, 1.f);
	progressBar->setScaling(0.5f, 0.05f, 1.f);
	progressBar->setPosition(-0.4f, 0.85f, -1.f);

	mana = new ProgressBar(device);
	mana->setMaterial(materialManager->getrectangleShape());
	mana->setBackgroundColor(1.f, 1.f, 1.f, 1.f);
	mana->setForegroundColor(0.f, 0.f, 1.f, 1.f);
	mana->setScaling(0.5f, 0.05f, 1.f);
	mana->setPosition(-0.4f, 0.65f, -1.f);

	return true;
}

int DX11FPSDemo::run()
{
	MSG msg = { 0 };
	clock.startClock();
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		if (quitGame)
			break;
		clock.tick();
		processInput();
		update(clock.getDeltaTime());
		XMMATRIX camViewMatrix = playerCharacter->getCamViewMatrix(), camProjectionMatrix = playerCharacter->getCamProjectionMatrix();
		window->clear();
		window->draw(skyBox, camViewMatrix, camProjectionMatrix);
		for (auto w = currentLevel->getWallBlockListBegin(); w != currentLevel->getWallBlockListEnd(); ++w)
			window->draw(*w, camViewMatrix, camProjectionMatrix);
		for (auto f = currentLevel->getFloorListBegin(); f != currentLevel->getFloorListEnd(); ++f)
			window->draw(*f, camViewMatrix, camProjectionMatrix);
		window->draw(playerCharacter, camViewMatrix, camProjectionMatrix);
		for (auto f = projectileManager->getListBegin(); f != projectileManager->getListEnd(); ++f)
			window->draw(*f, camViewMatrix, camProjectionMatrix);
		for (auto p = particleSystemManager->getListBegin(); p != particleSystemManager->getListEnd(); ++p)
		{
			for (auto ps = (*p)->getListBegin(); ps != (*p)->getListEnd(); ++ps)
				window->draw(*ps, camViewMatrix, camProjectionMatrix);
		}
		for (auto c = enemyCharacterManager->getListBegin(); c != enemyCharacterManager->getListEnd(); ++c)
			window->draw(*c, camViewMatrix, camProjectionMatrix);
		window->draw(progressBar, camViewMatrix, camProjectionMatrix);
		window->draw(mana, camViewMatrix, camProjectionMatrix);

		window->display();

	}

	return (int)msg.wParam;
}

void DX11FPSDemo::reset()
{
	aiManager->reset();
	enemyCharacterManager->reset();
	projectileManager->reset();
	particleSystemManager->reset();

	playerCharacter->setPosition(25.f, 1.f, 26.f);
	playerCharacter->setLife(100.f);
	playerCharacter->setMana(100.f);
	playerCharacter->resetOrientation();
}

void DX11FPSDemo::processInput()
{
	DIMOUSESTATE mouseCurrState;

	BYTE keyboardState[256];
	double deltaTime = clock.getDeltaTime();
	DIKeyboard->Acquire();
	DIMouse->Acquire();

	DIMouse->GetDeviceState(sizeof(DIMOUSESTATE), &mouseCurrState);

	DIKeyboard->GetDeviceState(sizeof(keyboardState), (LPVOID)&keyboardState);

	if (keyboardState[DIK_ESCAPE] & 0x80)
		PostMessage(windowHandle, WM_DESTROY, 0, 0);

	if (keyboardState[DIK_A] & 0x80)
	{
		//freeLookCamera.moveLeftward(deltaTime);
		playerCharacter->moveLeftward();
	}
	if (keyboardState[DIK_D] & 0x80)
	{
		//freeLookCamera.moveRightward(deltaTime);
		playerCharacter->moveRightward();
	}
	if (keyboardState[DIK_W] & 0x80)
	{
		//freeLookCamera.moveForward(deltaTime);
		playerCharacter->moveForward();
	}
	if (keyboardState[DIK_S] & 0x80)
	{
		//freeLookCamera.moveBackward(deltaTime);
		playerCharacter->moveBackward();
	}
	if ((mouseCurrState.lX != mouseLastState.lX) || (mouseCurrState.lY != mouseLastState.lY))
	{
		//freeLookCamera.rotate(-mouseCurrState.lY*deltaTime, mouseCurrState.lX*deltaTime);
		//mouseLastState = mouseCurrState;
		playerCharacter->rotateSight(mouseCurrState.lX*0.001f, mouseCurrState.lY *0.001f);
	}
	if (mouseCurrState.rgbButtons[0] && !mouseLeftClicked)
	{
		playerCharacter->fireFireball();
		mouseLeftClicked = true;
	}
	if (!mouseCurrState.rgbButtons[0])
	{
		mouseLeftClicked = false;
	}
}

void DX11FPSDemo::update(float deltaTime)
{
	playerCharacter->update(deltaTime);
	projectileManager->update(deltaTime);
	aiManager->update(deltaTime);
	particleSystemManager->update(deltaTime);
	progressBar->setPercentage(playerCharacter->getLifePercentage());
	enemyCharacterManager->update(deltaTime);
	mana->setPercentage(playerCharacter->getManaPercentage());

	if (playerCharacter->getLifePercentage() <= 0.f)
	{
		int returnOption = MessageBox(nullptr, L"Try again?", L"Game Over", MB_OKCANCEL | MB_ICONQUESTION);
		switch (returnOption)
		{
		case IDOK:
			reset();
			return;
		case IDCANCEL:
			PostMessage(windowHandle, WM_DESTROY, 0, 0);
			quitGame = true;
			return;
		}
	}

	CylindricalCollider *c = (CylindricalCollider*)(playerCharacter->getCollider());

	int wallBlockNumber = currentLevel->getWallBlockListEnd() - currentLevel->getWallBlockListBegin();
	XMFLOAT3 *offsets = new XMFLOAT3[wallBlockNumber];
	for (auto w = currentLevel->getWallBlockListBegin(); w != currentLevel->getWallBlockListEnd(); ++w)
	{
		CubicCollider *cubicCollider = (CubicCollider*)((*w)->getCollider());
		offsets[w - currentLevel->getWallBlockListBegin()] = CollisionDetection::isOverlappedBetweenCubicAndCylindrical(cubicCollider, c);
	}
	XMFLOAT3 overallOffset(0.f, 0.f, 0.f);
	for (int i = 0; i<wallBlockNumber; ++i)
	{
		if (abs(offsets[i].x) > abs(overallOffset.x))
			overallOffset.x = offsets[i].x;
		if (abs(offsets[i].y) > abs(overallOffset.y))
			overallOffset.y = offsets[i].y;
		if (abs(offsets[i].z) > abs(overallOffset.z))
			overallOffset.z = offsets[i].z;
	}
	playerCharacter->move(overallOffset);
	delete[] offsets;

	int numberOfEnemy = enemyCharacterManager->getListEnd() - enemyCharacterManager->getListBegin();
	XMFLOAT3 *eOffsets = new XMFLOAT3[numberOfEnemy];
	for (auto enemyCharacter = enemyCharacterManager->getListBegin(); enemyCharacter != enemyCharacterManager->getListEnd(); ++enemyCharacter)
	{
		CylindricalCollider *ec = (CylindricalCollider*)((*enemyCharacter)->getCollider());
		eOffsets[enemyCharacter - enemyCharacterManager->getListBegin()] = CollisionDetection::isOverlappedBetweenTwoCylindrical(ec, c);
	}
	XMFLOAT3 eOverallOffset(0.f, 0.f, 0.f);
	for (int i = 0; i<numberOfEnemy; ++i)
	{
		if (abs(eOffsets[i].x) > abs(eOverallOffset.x))
			eOverallOffset.x = eOffsets[i].x;
		if (abs(eOffsets[i].y) > abs(eOverallOffset.y))
			eOverallOffset.y = eOffsets[i].y;
		if (abs(eOffsets[i].z) > abs(eOverallOffset.z))
			eOverallOffset.z = eOffsets[i].z;
	}
	playerCharacter->move(eOverallOffset);

	for (auto f = projectileManager->getListBegin(); f != projectileManager->getListEnd(); ++f)
	{
		SphericalCollider *sphereCollider = (SphericalCollider*)((*f)->getCollider());
		int numberofWallBlock = currentLevel->getWallBlockListEnd() - currentLevel->getWallBlockListBegin();
		XMFLOAT3 *pOffsets = new XMFLOAT3[numberofWallBlock];
		for (auto w = currentLevel->getWallBlockListBegin(); w != currentLevel->getWallBlockListEnd(); ++w)
		{
			CubicCollider *cubicCollider = (CubicCollider*)((*w)->getCollider());
			pOffsets[w - currentLevel->getWallBlockListBegin()] = CollisionDetection::isOverlappedBetweenCubicAndSphere(cubicCollider, sphereCollider);
		}
		XMFLOAT3 pOverallOffset(0.f, 0.f, 0.f);
		for (int i = 0; i<numberofWallBlock; ++i)
		{
			if (abs(pOffsets[i].x) > abs(pOverallOffset.x))
				pOverallOffset.x = pOffsets[i].x;
			if (abs(pOffsets[i].y) > abs(pOverallOffset.y))
				pOverallOffset.y = pOffsets[i].y;
			if (abs(pOffsets[i].z) > abs(pOverallOffset.z))
				pOverallOffset.z = pOffsets[i].z;
		}
		if (pOverallOffset.x != 0 || pOverallOffset.y != 0 || pOverallOffset.z != 0)
		{
			(*f)->setActive(false);
			ExplosionParticleSystem* e = new ExplosionParticleSystem(device);
			e->setPosition((*f)->getPosition().x, (*f)->getPosition().y, (*f)->getPosition().z);
			e->setMaterial(materialManager->getRuby());
			e->setParticleDuration(0.2);
			e->setDuration(0.1);
			e->setParticleDensity(20);
			particleSystemManager->addParticleSystem(e);
		}
		delete[] pOffsets;
	}

	for (auto f = projectileManager->getListBegin(); f != projectileManager->getListEnd(); ++f)
	{
		SphericalCollider *sphereCollider = (SphericalCollider*)((*f)->getCollider());
		int numberOfFloor = currentLevel->getFloorListEnd() - currentLevel->getFloorListBegin();
		XMFLOAT3 *pOffsets = new XMFLOAT3[numberOfFloor];
		for (auto fl = currentLevel->getFloorListBegin(); fl != currentLevel->getFloorListEnd(); ++fl)
		{
			CubicCollider *cubicCollider = (CubicCollider*)((*fl)->getCollider());
			pOffsets[fl - currentLevel->getFloorListBegin()] = CollisionDetection::isOverlappedBetweenCubicAndSphere(cubicCollider, sphereCollider);
		}
		XMFLOAT3 pOverallOffset(0.f, 0.f, 0.f);
		for (int i = 0; i<numberOfFloor; ++i)
		{
			if (abs(pOffsets[i].x) > abs(pOverallOffset.x))
				pOverallOffset.x = pOffsets[i].x;
			if (abs(pOffsets[i].y) > abs(pOverallOffset.y))
				pOverallOffset.y = pOffsets[i].y;
			if (abs(pOffsets[i].z) > abs(pOverallOffset.z))
				pOverallOffset.z = pOffsets[i].z;
		}
		if (pOverallOffset.x != 0 || pOverallOffset.y != 0 || pOverallOffset.z != 0)
		{
			(*f)->setActive(false);
			ExplosionParticleSystem* e = new ExplosionParticleSystem(device);
			e->setPosition((*f)->getPosition().x, (*f)->getPosition().y, (*f)->getPosition().z);
			e->setMaterial(materialManager->getRuby());
			e->setParticleDuration(0.2);
			e->setDuration(0.1);
			e->setParticleDensity(20);
			particleSystemManager->addParticleSystem(e);
		}
		delete[] pOffsets;
	}

	for (auto f = projectileManager->getListBegin(); f != projectileManager->getListEnd(); ++f)
	{
		SphericalCollider *sphereCollider = (SphericalCollider*)((*f)->getCollider());

		for (auto enemyCharacter = enemyCharacterManager->getListBegin(); enemyCharacter != enemyCharacterManager->getListEnd(); ++enemyCharacter)
		{
			CylindricalCollider *ec = (CylindricalCollider*)((*enemyCharacter)->getCollider());
			eOffsets[enemyCharacter - enemyCharacterManager->getListBegin()] = CollisionDetection::isOverlappedBetweenCylindricalAndSphere(ec, sphereCollider);
		}
		int collidedEnemyIndex = -1;
		for (int i = 0; i<numberOfEnemy; ++i)
		{
			if (abs(eOffsets[i].x) > abs(eOverallOffset.x)) {
				eOverallOffset.x = eOffsets[i].x;

			}
			if (abs(eOffsets[i].y) > abs(eOverallOffset.y))
			{
				eOverallOffset.y = eOffsets[i].y;
				collidedEnemyIndex = i;
			}

			if (abs(eOffsets[i].z) > abs(eOverallOffset.z))
			{
				eOverallOffset.z = eOffsets[i].z;
				collidedEnemyIndex = i;
			}

		}

		if (eOverallOffset.x != 0 || eOverallOffset.y != 0 || eOverallOffset.z != 0)
		{
			(*f)->setActive(false);
			ExplosionParticleSystem* e = new ExplosionParticleSystem(device);
			e->setPosition((*f)->getPosition().x, (*f)->getPosition().y, (*f)->getPosition().z);
			e->setMaterial(materialManager->getRuby());
			e->setParticleDuration(0.2);
			e->setDuration(0.1);
			e->setParticleDensity(20);
			particleSystemManager->addParticleSystem(e);
			if (collidedEnemyIndex != -1)
			{
				auto ec = enemyCharacterManager->getListBegin();
				(*(ec + collidedEnemyIndex))->takeDamage((*f)->getDamage());
			}
		}
	}
	if (numberOfEnemy>0)
		delete[] eOffsets;

	XMFLOAT3 cameraPosition = playerCharacter->getPosition();
	skyBox->setPosition(cameraPosition.x, cameraPosition.y, cameraPosition.z);
}

LRESULT DX11FPSDemo::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}


