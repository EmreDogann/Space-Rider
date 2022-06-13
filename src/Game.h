#pragma once

#include "Common.h"
#include "GameWindow.h"
#include <unordered_map>

// Classes used in game.  For a new class, declare it here and provide a pointer to an object of this class below.  Then, in Game.cpp, 
// include the header.  In the Game constructor, set the pointer to NULL and in Game::Initialise, create a new object.  Don't forget to 
// delete the object in the destructor.   
class CCamera;
class CSkybox;
class CShader;
class CShaderProgram;
class CPlane;
class CFreeTypeFont;
class CHighResolutionTimer;
class CSphere;
class COctaSphere;
class COpenAssetImportMesh;
class CAudio;
class CCatmullRom;
class CRock;
class CStreetLight;
class CPlayer;
class CHexagon;
class CHealth;
class CEnemy;

class Game {
private:
	// Three main methods used in the game.  Initialise runs once, while Update and Render run repeatedly in the game loop.
	void Initialise();
	void Reset();
	void Update();
	void Render();

	// Pointers to game objects. They will get allocated in Game::Initialise()
	CSkybox* m_pSkybox;
	CCamera* m_pCamera;
	vector <CShaderProgram*>* m_pShaderPrograms;
	CFreeTypeFont* m_pFtFont;
	CPlayer* m_pPlayer;
	CRock* m_pAsteroid;
	CStreetLight* m_pStreetLight;
	CHighResolutionTimer* m_pHighResolutionTimer;
	CAudio* m_pAudio;
	CCatmullRom* m_pCatmullRom;

	std::vector<CEnemy*>* m_pEnemy;

	std::vector<COctaSphere*>* m_pOctaSphere;

	std::vector<CHexagon*>* m_pHexPool{};
	std::vector<CHexagon*>* m_pBulletPool{};
	std::vector<CHealth*>* m_pHealthPool{};

	// Some other member variables
	double m_dt;
	int m_framesPerSecond;
	bool m_appActive;
public:
	Game();
	~Game();
	static Game& GetInstance();
	LRESULT ProcessEvents(HWND window, UINT message, WPARAM w_param, LPARAM l_param);
	void SetHinstance(HINSTANCE hinstance);
	WPARAM Execute();

private:
	int width;
	int height;

	std::random_device rd;
	std::mt19937 mt = std::mt19937(rd());

	std::uniform_real_distribution<float> m_dist_combo_text = std::uniform_real_distribution<float>(5.0f, 5.0f);
	std::uniform_real_distribution<float> m_dist_pickups_side = std::uniform_real_distribution<float>(-125.0f, 125.0f);
	std::uniform_real_distribution<float> m_dist_pickups_front = std::uniform_real_distribution<float>(1500.0f, 2000.0f);
	std::uniform_real_distribution<float> m_dist_front_enemy = std::uniform_real_distribution<float>(2500.0f, 4000.0f);
	std::uniform_real_distribution<float> m_dist_pickups_front_health = std::uniform_real_distribution<float>(4000.0f, 9000.0f);
	std::uniform_int_distribution<int>    m_dist_time = std::uniform_int_distribution<int>(0, 100000);

	static const int FPS = 60;
	void DrawUI();
	void GameLoop();
	GameWindow m_gameWindow;
	HINSTANCE m_hInstance;
	int m_frameCount;
	double m_elapsedTime;
	double m_totalTime;
	float m_currentDistance;
	float m_cameraSpeed;
	float nearPlane = 0.1f;
	float farPlane = 10000.0f;
	bool godMode = false;

	float m_score = 0.0f;
	int m_currentBullet = 0;
	float m_shootCooldown = 0.12f;
	float m_currentShootCooldown = m_shootCooldown;

	bool m_fadeToBlack = false;
	float m_blackScreenCrossfade = 0.0f;

	glm::vec3 pointLightPosition = glm::vec3(-2000, 400, 1500);
	unsigned int m_pointShadowMapFBO;
	unsigned int m_depthCubeMap;
	unsigned int shadowMapWidth = 2048;
	unsigned int shadowMapHeight = 2048;
	glm::mat4 shadowProjection{};
	glm::mat4 shadowViews[6];

	unsigned int hdrFBO;
	unsigned int hdrColorBuffer;
	unsigned int hdrDepthRenderBuffer;

	static constexpr int bloomBufferCount = 5;
	unsigned int bloomBuffers[bloomBufferCount];
	std::vector<glm::vec2> bloomBufferResolutions{};
	unsigned int linearFiltering;
};
