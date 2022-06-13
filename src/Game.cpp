/*
OpenGL Template for INM376 / IN3005
City University London, School of Mathematics, Computer Science and Engineering
Source code drawn from a number of sources and examples, including contributions
from
 - Ben Humphrey (gametutorials.com), Michal Bubner (mbsoftworks.sk), Christophe
Riccio (glm.g-truc.net)
 - Christy Quinn, Sam Kellett and others

 For educational use by Department of Computer Science, City University London
UK.

 This template contains a skybox, simple terrain, camera, lighting, shaders,
texturing

 Potential ways to modify the code:  Add new geometry types, shaders, change the
terrain, load new meshes, change the lighting, different camera controls,
different shaders, etc.

 Template version 5.0a 29/01/2017
 Dr Greg Slabaugh (gregory.slabaugh.1@city.ac.uk)

 version 6.0a 29/01/2019
 Dr Eddie Edwards (Philip.Edwards@city.ac.uk)

 version 6.1a 13/02/2022 - Sorted out Release mode and a few small compiler
warnings Dr Eddie Edwards (Philip.Edwards@city.ac.uk)

*/

#include "game.h"

// Setup includes
#include "GameWindow.h"
#include "HighResolutionTimer.h"

// Game includes
#include "Audio.h"
#include "Camera.h"
#include "CatmullRom.h"
#include "FreeTypeFont.h"
#include "MatrixStack.h"
#include "OpenAssetImportMesh.h"
#include "Plane.h"
#include "Shaders.h"
#include "Skybox.h"
#include "Sphere.h"
#include "OctaSphere.h"
#include "Rock.h"
#include "StreetLight.h"
#include "Player.h"
#include "Hexagon.h"
#include "Health.h"
#include "Enemy.h"
#include <iostream>

// Constructor
Game::Game() {
	m_pSkybox = NULL;
	m_pCamera = NULL;
	m_pShaderPrograms = NULL;
	m_pFtFont = NULL;
	m_pHighResolutionTimer = NULL;
	m_pAudio = NULL;
	m_pCatmullRom = NULL;
	m_pOctaSphere = NULL;
	m_pHexPool = NULL;
	m_pBulletPool = NULL;
	m_pHealthPool = NULL;
	m_pEnemy = NULL;

	m_dt = 0.0;
	m_framesPerSecond = 0;
	m_frameCount = 0;
	m_elapsedTime = 0.0f;
	m_totalTime = 0.0f;
	m_currentDistance = 0.0f;
	m_cameraSpeed = 0.85f;
	m_score = 0.0f;
	m_blackScreenCrossfade = 0.0f;
	m_fadeToBlack = false;
	m_currentBullet = 0;
}

// Destructor
Game::~Game() {
	delete m_pCamera;
	delete m_pSkybox;
	delete m_pFtFont;
	delete m_pPlayer;
	delete m_pAsteroid;
	delete m_pStreetLight;
	delete m_pAudio;
	delete m_pCatmullRom;

	if (m_pShaderPrograms != NULL) {
		for (unsigned int i = 0; i < m_pShaderPrograms->size(); i++)
			delete (*m_pShaderPrograms)[i];
	}
	delete m_pShaderPrograms;

	if (m_pOctaSphere != NULL) {
		for (unsigned int i = 0; i < m_pOctaSphere->size(); i++)
			delete (*m_pOctaSphere)[i];
	}
	delete m_pOctaSphere;

	if (m_pHexPool != NULL) {
		for (unsigned int i = 0; i < m_pHexPool->size(); i++)
			delete (*m_pHexPool)[i];
	}
	delete m_pHexPool;

	if (m_pBulletPool != NULL) {
		for (unsigned int i = 0; i < m_pBulletPool->size(); i++)
			delete (*m_pBulletPool)[i];
	}
	delete m_pBulletPool;

	if (m_pHealthPool != NULL) {
		for (unsigned int i = 0; i < m_pHealthPool->size(); i++)
			delete (*m_pHealthPool)[i];
	}
	delete m_pHealthPool;

	if (m_pEnemy != NULL) {
		for (unsigned int i = 0; i < m_pEnemy->size(); i++)
			delete (*m_pEnemy)[i];
	}
	delete m_pEnemy;

	// Delete framebuffers
	glDeleteFramebuffers(1, &hdrFBO);
	glDeleteFramebuffers(1, &m_pointShadowMapFBO);

	// Delete Renderbuffers
	glDeleteRenderbuffers(1, &hdrDepthRenderBuffer);

	// Delete Textures
	glDeleteTextures(1, &hdrColorBuffer);
	glDeleteTextures(1, &m_depthCubeMap);
	glDeleteTextures(bloomBufferCount, &bloomBuffers[0]);

	// Delete Samplers
	glDeleteSamplers(1, &linearFiltering);

	// Setup objects
	delete m_pHighResolutionTimer;
}

// NOTE: It would be more efficient to reuse these instead of deleting and remaking them on game reset,
// but for the purpose of player death it is fine.
void Game::Reset() {
	delete m_pCamera;
	delete m_pSkybox;
	delete m_pFtFont;
	delete m_pPlayer;
	delete m_pAsteroid;
	delete m_pStreetLight;
	delete m_pAudio;
	delete m_pCatmullRom;

	if (m_pShaderPrograms != NULL) {
		for (unsigned int i = 0; i < m_pShaderPrograms->size(); i++)
			delete (*m_pShaderPrograms)[i];
	}
	delete m_pShaderPrograms;

	if (m_pOctaSphere != NULL) {
		for (unsigned int i = 0; i < m_pOctaSphere->size(); i++)
			delete (*m_pOctaSphere)[i];
	}
	delete m_pOctaSphere;

	if (m_pHexPool != NULL) {
		for (unsigned int i = 0; i < m_pHexPool->size(); i++)
			delete (*m_pHexPool)[i];
	}
	delete m_pHexPool;

	if (m_pBulletPool != NULL) {
		for (unsigned int i = 0; i < m_pBulletPool->size(); i++)
			delete (*m_pBulletPool)[i];
	}
	delete m_pBulletPool;

	if (m_pHealthPool != NULL) {
		for (unsigned int i = 0; i < m_pHealthPool->size(); i++)
			delete (*m_pHealthPool)[i];
	}
	delete m_pHealthPool;

	if (m_pEnemy != NULL) {
		for (unsigned int i = 0; i < m_pEnemy->size(); i++)
			delete (*m_pEnemy)[i];
	}
	delete m_pEnemy;

	// Delete framebuffers
	glDeleteFramebuffers(1, &hdrFBO);
	glDeleteFramebuffers(1, &m_pointShadowMapFBO);

	// Delete Renderbuffers
	glDeleteRenderbuffers(1, &hdrDepthRenderBuffer);

	// Delete Textures
	glDeleteTextures(1, &hdrColorBuffer);
	glDeleteTextures(1, &m_depthCubeMap);
	glDeleteTextures(bloomBufferCount, &bloomBuffers[0]);

	// Delete Samplers
	glDeleteSamplers(1, &linearFiltering);

	// Reset Game state
	m_dt = 0.0;
	m_framesPerSecond = 0;
	m_frameCount = 0;
	m_elapsedTime = 0.0f;
	m_totalTime = 0.0f;
	m_currentDistance = 0.0f;
	m_score = 0.0f;
	m_blackScreenCrossfade = 0.0f;
	m_fadeToBlack = false;
	m_currentBullet = 0;
	m_currentShootCooldown = m_shootCooldown;
	godMode = false;
}

void Game::Initialise() {
	// Create objects
	m_pCamera = new CCamera;
	m_pSkybox = new CSkybox;
	m_pShaderPrograms = new vector<CShaderProgram*>;
	m_pFtFont = new CFreeTypeFont;
	m_pPlayer = new CPlayer;
	m_pAsteroid = new CRock;
	m_pStreetLight = new CStreetLight;
	m_pAudio = new CAudio;
	m_pCatmullRom = new CCatmullRom;
	m_pOctaSphere = new std::vector<COctaSphere*>;
	m_pHexPool = new std::vector<CHexagon*>;
	m_pBulletPool = new std::vector<CHexagon*>;
	m_pHealthPool = new std::vector<CHealth*>;
	m_pEnemy = new std::vector<CEnemy*>;

	for (int i = 0; i < 13; ++i) {
		m_pOctaSphere->push_back(new COctaSphere);
	}

	for (int i = 0; i < 250; ++i) {
		m_pHexPool->push_back(new CHexagon);
	}

	for (int i = 0; i < 10; ++i) {
		m_pBulletPool->push_back(new CHexagon);
	}

	for (int i = 0; i < 3; ++i) {
		m_pHealthPool->push_back(new CHealth);
	}

	for (int i = 0; i < 13; ++i) {
		m_pEnemy->push_back(new CEnemy);
	}

	RECT dimensions = m_gameWindow.GetDimensions();

	width = GameWindow::SCREEN_WIDTH;
	height = GameWindow::SCREEN_HEIGHT;

	// Set the orthographic and perspective projection matrices based on the image size
	m_pCamera->SetOrthographicProjectionMatrix(width, height);
	m_pCamera->SetPerspectiveProjectionMatrix(glm::radians(90.0f), (float)width / (float)height, nearPlane, farPlane);

	std::string resourcesFilePath = "resources/";

	// Load shaders
	vector<CShader> shShaders;
	vector<string> sShaderFileNames;
	sShaderFileNames.push_back("mainShader.vert"); // 0
	sShaderFileNames.push_back("mainShader.frag"); // 1
	sShaderFileNames.push_back("textShader.vert"); // 2
	sShaderFileNames.push_back("textShader.frag"); // 3
	sShaderFileNames.push_back("gridShader.vert"); // 4
	sShaderFileNames.push_back("gridShader.frag"); // 5
	sShaderFileNames.push_back("bloomShader.frag"); // 6
	sShaderFileNames.push_back("postFXShader.frag"); // 7
	sShaderFileNames.push_back("fullscreenQuadShader.vert"); // 8
	sShaderFileNames.push_back("planetShader.vert"); // 9
	sShaderFileNames.push_back("planetShader.frag"); // 10
	sShaderFileNames.push_back("shadowCubeMapShader.vert"); // 11
	sShaderFileNames.push_back("shadowCubeMapShader.frag"); // 12
	sShaderFileNames.push_back("cubeMapShader.geom"); // 13
	sShaderFileNames.push_back("mainInstancedShader.vert"); // 14
	sShaderFileNames.push_back("mainInstancedShader.frag"); // 15
	sShaderFileNames.push_back("lineShader.vert"); // 16
	sShaderFileNames.push_back("lineShader.frag"); // 17

	for (int i = 0; i < (int)sShaderFileNames.size(); i++) {
		string sExt =
			sShaderFileNames[i].substr((int)sShaderFileNames[i].size() - 4, 4);
		int iShaderType;
		if (sExt == "vert")
			iShaderType = GL_VERTEX_SHADER;
		else if (sExt == "frag")
			iShaderType = GL_FRAGMENT_SHADER;
		else if (sExt == "geom")
			iShaderType = GL_GEOMETRY_SHADER;
		else if (sExt == "tcnl")
			iShaderType = GL_TESS_CONTROL_SHADER;
		else
			iShaderType = GL_TESS_EVALUATION_SHADER;
		CShader shader;
		shader.LoadShader(resourcesFilePath + "shaders/" + sShaderFileNames[i],
			iShaderType);
		shShaders.push_back(shader);
	}

	// Create & Link shader programs.
	{
		// 0: Create the main shader program
		{
			CShaderProgram* pProgram = new CShaderProgram;
			pProgram->CreateProgram();
			pProgram->AddShaderToProgram(&shShaders[0]);
			pProgram->AddShaderToProgram(&shShaders[1]);
			pProgram->LinkProgram();
			m_pShaderPrograms->push_back(pProgram);
		}

		// 1: Create a shader program for fonts
		{
			CShaderProgram* pProgram = new CShaderProgram;
			pProgram->CreateProgram();
			pProgram->AddShaderToProgram(&shShaders[2]);
			pProgram->AddShaderToProgram(&shShaders[3]);
			pProgram->LinkProgram();
			m_pShaderPrograms->push_back(pProgram);

			m_pFtFont->LoadSystemFont("arial.ttf", 32);
			m_pFtFont->SetShaderProgram(pProgram);
		}

		// 2: Create a shader program for track grid
		{
			CShaderProgram* pProgram = new CShaderProgram;
			pProgram->CreateProgram();
			pProgram->AddShaderToProgram(&shShaders[4]);
			pProgram->AddShaderToProgram(&shShaders[5]);
			pProgram->LinkProgram();
			m_pShaderPrograms->push_back(pProgram);
		}

		// 3: Create a shader program for track grid bloom
		{
			CShaderProgram* pProgram = new CShaderProgram;
			pProgram->CreateProgram();
			pProgram->AddShaderToProgram(&shShaders[6]);
			pProgram->AddShaderToProgram(&shShaders[8]);
			pProgram->LinkProgram();
			m_pShaderPrograms->push_back(pProgram);
		}

		// 4: Create a shader program for post processing composition
		{
			CShaderProgram* pProgram = new CShaderProgram;
			pProgram->CreateProgram();
			pProgram->AddShaderToProgram(&shShaders[7]);
			pProgram->AddShaderToProgram(&shShaders[8]);
			pProgram->LinkProgram();
			m_pShaderPrograms->push_back(pProgram);
		}

		// 5: Create a shader program for planet effects
		{
			CShaderProgram* pProgram = new CShaderProgram;
			pProgram->CreateProgram();
			pProgram->AddShaderToProgram(&shShaders[9]);
			pProgram->AddShaderToProgram(&shShaders[10]);
			pProgram->LinkProgram();
			m_pShaderPrograms->push_back(pProgram);
		}

		// 6: Create a shader program for point light cube mapped shadows
		{
			CShaderProgram* pProgram = new CShaderProgram;
			pProgram->CreateProgram();
			pProgram->AddShaderToProgram(&shShaders[11]);
			pProgram->AddShaderToProgram(&shShaders[12]);
			pProgram->AddShaderToProgram(&shShaders[13]);
			pProgram->LinkProgram();
			m_pShaderPrograms->push_back(pProgram);
		}

		// 7: Create a shader program for instanced objects
		{
			CShaderProgram* pProgram = new CShaderProgram;
			pProgram->CreateProgram();
			pProgram->AddShaderToProgram(&shShaders[14]);
			pProgram->AddShaderToProgram(&shShaders[15]);
			pProgram->LinkProgram();
			m_pShaderPrograms->push_back(pProgram);
		}

		// 8: Create a shader program for drawing lines (Currently not used)
		{
			CShaderProgram* pProgram = new CShaderProgram;
			pProgram->CreateProgram();
			pProgram->AddShaderToProgram(&shShaders[16]);
			pProgram->AddShaderToProgram(&shShaders[17]);
			pProgram->LinkProgram();
			m_pShaderPrograms->push_back(pProgram);
		}
	}

	// Create the skybox
	m_pSkybox->Create(2500.0f, resourcesFilePath);

	m_pPlayer->Create(resourcesFilePath, glm::vec3(40.0f));

	int subdivisions = 3;
	(*m_pOctaSphere)[0]->Create(subdivisions + 2, 1, m_dist_time(mt), true, "", glm::vec3(401.923f, -811.0f, -2572.42f), glm::vec3(500.0f));
	(*m_pOctaSphere)[1]->Create(subdivisions, 3, m_dist_time(mt), false, resourcesFilePath + "textures/" + "Moon.jpg", glm::vec3(46.0f, -975.0f, 0.0f), glm::vec3(359.0f));
	(*m_pOctaSphere)[2]->Create(subdivisions, 3, m_dist_time(mt), false, resourcesFilePath + "textures/" + "Mars.jpg", glm::vec3(-103.0f, -161.0f, 418.0f), glm::vec3(551.0f));
	(*m_pOctaSphere)[3]->Create(subdivisions, 2, m_dist_time(mt), true, "", glm::vec3(-1033.0f, -1169.0f, 829.0f), glm::vec3(254.0f));
	(*m_pOctaSphere)[4]->Create(subdivisions, 8, m_dist_time(mt), false, resourcesFilePath + "textures/" + "Earth.jpg", glm::vec3(-1680.0f, -238.0f, -783.0f), glm::vec3(553.0f));
	(*m_pOctaSphere)[5]->Create(subdivisions, 3, m_dist_time(mt), false, resourcesFilePath + "textures/" + "Venus.jpg", glm::vec3(-2975.0f, -655.0f, -142.0f), glm::vec3(380.0f));
	(*m_pOctaSphere)[6]->Create(subdivisions + 3, 5, m_dist_time(mt), true, resourcesFilePath + "textures/" + "Sun.jpg", glm::vec3(-2108.0f, 233.0f, 1544.0f), glm::vec3(927.0f));
	(*m_pOctaSphere)[7]->Create(subdivisions, 6, m_dist_time(mt), true, resourcesFilePath + "textures/" + "BloodStone.png", glm::vec3(-3600.0f, 82.0f, 1488.0f), glm::vec3(292.0f));
	(*m_pOctaSphere)[8]->Create(subdivisions, 6, m_dist_time(mt), true, resourcesFilePath + "textures/" + "BloodStone.png", glm::vec3(-1548.0f, 1006.0f, 2650.0f), glm::vec3(235.0f));
	(*m_pOctaSphere)[9]->Create(subdivisions + 2, 4, m_dist_time(mt), true, resourcesFilePath + "textures/" + "Moon.jpg", glm::vec3(-2147.0f, 243.0f, -2476.0f), glm::vec3(450.0f));
	(*m_pOctaSphere)[10]->Create(subdivisions, 2, m_dist_time(mt), true, "", glm::vec3(-3565.0f, 602.0f, -3349.0f), glm::vec3(262.0f));
	(*m_pOctaSphere)[11]->Create(subdivisions + 2, 7, m_dist_time(mt), true, resourcesFilePath + "textures/" + "Sun.jpg", glm::vec3(-3584.0f, 1557.0f, -1550.0f), glm::vec3(563.0f));
	(*m_pOctaSphere)[12]->Create(subdivisions, 3, m_dist_time(mt), false, resourcesFilePath + "textures/" + "Snow.png", glm::vec3(-3127.0f, 2703.0f, -3955.0f), glm::vec3(445.0f));

	// Initialise audio and play background music
	//m_pAudio->Initialise();

	m_pCatmullRom->CreateCentreline(resourcesFilePath);
	m_pCatmullRom->CreateOffsetCurves();
	m_pCatmullRom->CreateTrack();

	m_pAsteroid->Create(resourcesFilePath, 10000);

	m_pStreetLight->Create(resourcesFilePath, m_pCatmullRom, 100);

	// Setup Coins
	int distance = m_dist_pickups_front(mt);
	for (int i = 0; i < (*m_pHexPool).size(); i += 10) {
		float randomOffset = m_dist_pickups_side(mt);
		randomOffset = randomOffset >= 0 ? randomOffset + 15.0f : randomOffset - 15.0f;
		for (int j = 0; j < 10; ++j) {
			CHexagon* hexObj = (*m_pHexPool)[j + i];

			glm::vec3 p;
			m_pCatmullRom->Sample(distance + (100.0f * j), p);

			// Look ahead a bit.
			glm::vec3 pNext;
			m_pCatmullRom->Sample(distance + 1.0f + (100.0f * j), pNext);

			// Tangent Vector
			glm::vec3 vecT = glm::normalize(pNext - p);
			glm::vec3 vecN = glm::normalize(glm::cross(vecT, glm::vec3(0.0f, 1.0f, 0.0f))); // Normal Vector
			glm::vec3 vecB = glm::normalize(glm::cross(vecN, vecT)); // Binormal Vector
			p += 60.0f * vecB;
			p += randomOffset * vecN;

			hexObj->Create(resourcesFilePath + "textures/goldEnergy.jpg", distance + (100.0f * j), 10.0f, p, glm::vec3(20.0f, 5.0f, 20.0f));

			glm::mat4 rotationMatrix = glm::mat4_cast(glm::angleAxis(glm::radians(90.0f), vecN));
			rotationMatrix *= glm::inverse(glm::lookAt(glm::vec3(0.0f), glm::vec3(0.0f) + vecT, glm::vec3(0.0f, 1.0f, 0.0f)));
			hexObj->SetRotation(rotationMatrix);

			hexObj->SetUpVector(vecB);
		}

		distance += m_dist_pickups_front(mt);
	}

	// Setup Bullets
	for (auto bulletObj : (*m_pBulletPool)) {
		bulletObj->Create(resourcesFilePath + "textures/bulletEmission.jpg", 0, 35.0f, glm::vec3(0.0f), glm::vec3(2.0f, 40.0f, 2.0f));
		bulletObj->SetActive(false);
	}

	// Setup Health
	distance = m_dist_pickups_front(mt);
	for (auto healthObj : (*m_pHealthPool)) {
		float randomOffset = m_dist_pickups_side(mt);
		randomOffset = randomOffset >= 0 ? randomOffset + 25.0f : randomOffset - 25.0f;

		glm::vec3 p;
		m_pCatmullRom->Sample(distance, p);

		// Look ahead a bit.
		glm::vec3 pNext;
		m_pCatmullRom->Sample(distance + 50.0f, pNext);

		// Tangent Vector
		glm::vec3 vecT = glm::normalize(pNext - p);
		glm::vec3 vecN = glm::normalize(glm::cross(vecT, glm::vec3(0.0f, 1.0f, 0.0f))); // Normal Vector
		glm::vec3 vecB = glm::normalize(glm::cross(vecN, vecT)); // Binormal Vector
		p += 85.0f * vecB;
		p += randomOffset * vecN;

		healthObj->Create(resourcesFilePath + "textures/health.png", distance, p, glm::vec3(10.0f));

		glm::mat4 rotationMatrix = glm::mat4_cast(glm::angleAxis(glm::radians(90.0f), vecB));
		rotationMatrix *= glm::inverse(glm::lookAt(glm::vec3(0.0f), glm::vec3(0.0f) + vecT, glm::vec3(0.0f, 1.0f, 0.0f)));
		healthObj->SetRotation(rotationMatrix);

		healthObj->SetUpVector(vecB);

		distance += m_dist_pickups_front_health(mt);
	}

	// Setup Enemies
	distance = m_dist_pickups_front(mt);
	for (auto enemyObj : (*m_pEnemy)) {
		float randomOffset = m_dist_pickups_side(mt);
		randomOffset = randomOffset >= 0 ? randomOffset + 25.0f : randomOffset - 25.0f;

		glm::vec3 p;
		m_pCatmullRom->Sample(distance, p);

		// Look ahead a bit.
		glm::vec3 pNext;
		m_pCatmullRom->Sample(distance + 50.0f, pNext);

		// Tangent Vector
		glm::vec3 vecT = glm::normalize(pNext - p);
		glm::vec3 vecN = glm::normalize(glm::cross(vecT, glm::vec3(0.0f, 1.0f, 0.0f))); // Normal Vector
		glm::vec3 vecB = glm::normalize(glm::cross(vecN, vecT)); // Binormal Vector
		p += 85.0f * vecB;
		p += randomOffset * vecN;

		enemyObj->Create(resourcesFilePath, distance, p, glm::vec3(40.0f));

		glm::mat4 rotationMatrix = glm::mat4_cast(glm::angleAxis(glm::radians(0.0f), vecB));
		rotationMatrix *= glm::inverse(glm::lookAt(glm::vec3(0.0f), glm::vec3(0.0f) + vecT, glm::vec3(0.0f, 1.0f, 0.0f)));
		enemyObj->SetRotation(rotationMatrix);
		enemyObj->SetTNBFrame(vecT, vecN, vecB);
		enemyObj->SetActive(false);

		distance += m_dist_front_enemy(mt);
	}

	/* ---- HDR Framebuffer & Bloom Setup ---- */
	glGenFramebuffers(1, &hdrFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, hdrFBO);

	// Create a renderbuffer object for depthbuffer
	glGenRenderbuffers(1, &hdrDepthRenderBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, hdrDepthRenderBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, hdrDepthRenderBuffer);

	// Create chain of mip resolutions
	int currentWidth = width;
	int currentHeight = height;
	for (int i = 0; i < bloomBufferCount; ++i) {
		bloomBufferResolutions.push_back(glm::vec2(currentWidth /= 2, currentHeight /= 2));
	}

	glGenTextures(1, &hdrColorBuffer);
	glBindTexture(GL_TEXTURE_2D, hdrColorBuffer);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_FLOAT, NULL);

	// Create HDR mip chain
	glGenTextures(bloomBufferCount, bloomBuffers);
	for (int i = 0; i < bloomBufferCount; ++i) {
		glBindTexture(GL_TEXTURE_2D, bloomBuffers[i]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, bloomBufferResolutions[i].x, bloomBufferResolutions[i].y, 0, GL_RGBA, GL_FLOAT, NULL);
	}

	glGenSamplers(1, &linearFiltering);
	glSamplerParameteri(linearFiltering, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glSamplerParameteri(linearFiltering, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glSamplerParameteri(linearFiltering, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glSamplerParameteri(linearFiltering, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	/* ---- Point Light Shadows Setup ---- */
	glGenFramebuffers(1, &m_pointShadowMapFBO);
	glGenTextures(1, &m_depthCubeMap);

	glBindTexture(GL_TEXTURE_CUBE_MAP, m_depthCubeMap);
	for (int i = 0; i < 6; ++i) {
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, shadowMapWidth, shadowMapHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, m_pointShadowMapFBO);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, m_depthCubeMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	shadowProjection = glm::perspective(glm::radians(90.0f), 1.0f, nearPlane, farPlane);
	shadowViews[0] = shadowProjection * glm::lookAt(pointLightPosition, pointLightPosition + glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)); // Right
	shadowViews[1] = shadowProjection * glm::lookAt(pointLightPosition, pointLightPosition + glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)); // Left
	shadowViews[2] = shadowProjection * glm::lookAt(pointLightPosition, pointLightPosition + glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)); // Up
	shadowViews[3] = shadowProjection * glm::lookAt(pointLightPosition, pointLightPosition + glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f)); // Down
	shadowViews[4] = shadowProjection * glm::lookAt(pointLightPosition, pointLightPosition + glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f)); // Back
	shadowViews[5] = shadowProjection * glm::lookAt(pointLightPosition, pointLightPosition + glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f)); // Forward

	// Render scene to point light cube map.
	{
		// Set up a matrix stack
		glutil::MatrixStack modelViewMatrixStack;
		modelViewMatrixStack.SetIdentity();

		glViewport(0, 0, shadowMapWidth, shadowMapHeight);
		glBindFramebuffer(GL_FRAMEBUFFER, m_pointShadowMapFBO);
		// Clear the buffers and enable depth testing (z-buffering)
		glClear(GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT);

		// Use the point light shadow shader program
		CShaderProgram* pShadowProgram = (*m_pShaderPrograms)[6];
		pShadowProgram->UseProgram();
		pShadowProgram->SetUniform("matrix[0]", shadowViews[0]);
		pShadowProgram->SetUniform("matrix[1]", shadowViews[1]);
		pShadowProgram->SetUniform("matrix[2]", shadowViews[2]);
		pShadowProgram->SetUniform("matrix[3]", shadowViews[3]);
		pShadowProgram->SetUniform("matrix[4]", shadowViews[4]);
		pShadowProgram->SetUniform("matrix[5]", shadowViews[5]);
		pShadowProgram->SetUniform("lightPos", pointLightPosition);
		pShadowProgram->SetUniform("farPlane", farPlane);

		for (int i = 0; i < (*m_pOctaSphere).size(); ++i) {
			COctaSphere* octaSphere = (*m_pOctaSphere)[i];

			modelViewMatrixStack.Push();
			modelViewMatrixStack.Translate(octaSphere->GetPosition());
			modelViewMatrixStack.Scale(octaSphere->GetScale());
			pShadowProgram->SetUniform("model", modelViewMatrixStack.Top());
			octaSphere->Render();
			modelViewMatrixStack.Pop();
		}
		// Render track
		glDisable(GL_CULL_FACE);
		modelViewMatrixStack.Push();
		pShadowProgram->SetUniform("model", modelViewMatrixStack.Top());
		m_pCatmullRom->RenderTrack();
		modelViewMatrixStack.Pop();
		glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT);
	}
}

// Render method runs repeatedly in a loop
void Game::Render() {
	// Set up a matrix stack
	glutil::MatrixStack modelViewMatrixStack;
	modelViewMatrixStack.SetIdentity();

	glViewport(0, 0, width, height);
	glBindFramebuffer(GL_FRAMEBUFFER, hdrFBO);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	// Clear the buffers and enable depth testing (z-buffering)
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	// Call LookAt to create the view matrix and put this on the modelViewMatrix
	// stack. Store the view matrix and the normal matrix associated with the view
	// matrix for later (they're useful for lighting -- since lighting is done in
	// eye coordinates)
	modelViewMatrixStack.LookAt(m_pCamera->GetPosition(), m_pCamera->GetView(), m_pCamera->GetUpVector());
	glm::mat4 viewMatrix = modelViewMatrixStack.Top();
	glm::mat3 viewNormalMatrix = m_pCamera->ComputeNormalMatrix(viewMatrix);

	// Use the main shader program
	CShaderProgram* pMainProgram = (*m_pShaderPrograms)[0];
	pMainProgram->UseProgram();
	pMainProgram->SetUniform("bUseTexture", true);
	pMainProgram->SetUniform("sampler0", 0);
	pMainProgram->SetUniform("material1.emission", 1);
	// Note: cubemap and non-cubemap textures should not be mixed in the same
	// texture unit.  Setting unit 10 to be a cubemap texture.
	int cubeMapTextureUnit = 10;
	pMainProgram->SetUniform("CubeMapTex", cubeMapTextureUnit);

	// Set the projection matrix
	pMainProgram->SetUniform("matrices.projMatrix", m_pCamera->GetPerspectiveProjectionMatrix());

	// Set light and materials in main shader program
	pMainProgram->SetUniform("light1.position", viewMatrix * glm::vec4(pointLightPosition, 1.0f)); // Position of light source *in eye coordinates*
	pMainProgram->SetUniform("light1.La",
		glm::vec3(1.0f)); // Ambient colour of light
	pMainProgram->SetUniform("light1.Ld",
		glm::vec3(1.0f)); // Diffuse colour of light
	pMainProgram->SetUniform("light1.Ls",
		glm::vec3(1.0f)); // Specular colour of light
	pMainProgram->SetUniform("material1.Ma",
		glm::vec3(1.0f)); // Ambient material reflectance
	pMainProgram->SetUniform("material1.Md",
		glm::vec3(0.0f)); // Diffuse material reflectance
	pMainProgram->SetUniform("material1.Ms",
		glm::vec3(0.0f)); // Specular material reflectance
	pMainProgram->SetUniform("material1.shininess",
		15.0f); // Shininess material property

	// Render the skybox and terrain with full ambient reflectance
	{
		glCullFace(GL_BACK);
		modelViewMatrixStack.Push();
		pMainProgram->SetUniform("renderSkybox", true);
		// Translate the modelview matrix to the camera eye point so skybox stays centred around camera
		glm::vec3 vEye = m_pCamera->GetPosition();
		modelViewMatrixStack.Translate(vEye);
		pMainProgram->SetUniform("matrices.modelViewMatrix",
			modelViewMatrixStack.Top());
		pMainProgram->SetUniform(
			"matrices.normalMatrix",
			m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
		m_pSkybox->Render(cubeMapTextureUnit);
		pMainProgram->SetUniform("renderSkybox", false);
		modelViewMatrixStack.Pop();
	}

	// Turn on diffuse + specular materials
	pMainProgram->SetUniform("material1.Ma",
		glm::vec3(0.75f)); // Ambient material reflectance
	pMainProgram->SetUniform("material1.Md",
		glm::vec3(2.0f)); // Diffuse material reflectance
	pMainProgram->SetUniform("material1.Ms",
		glm::vec3(2.0f)); // Specular material reflectance

	// Render the player's car
	{
		glm::vec3 playerPosition = m_pPlayer->GetPosition();
		playerPosition += m_pPlayer->GetPositionOffset().x * m_pPlayer->GetStrafeDirection();
		glm::mat4 lookDirection = glm::inverse(glm::lookAt(playerPosition, playerPosition - m_pPlayer->GetForwardDirection(), glm::vec3(0.0f, 1.0f, 0.0f)));

		glm::mat4 modelMatrix(1.0f);
		modelMatrix *= lookDirection;
		modelMatrix = glm::scale(modelMatrix, m_pPlayer->GetScale());

		modelViewMatrixStack.Push();
		pMainProgram->SetUniform("matrices.modelViewMatrix",
			modelViewMatrixStack.Top() * modelMatrix);
		pMainProgram->SetUniform(
			"matrices.normalMatrix",
			m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
		m_pPlayer->Render();
		modelViewMatrixStack.Pop();

		glActiveTexture(GL_TEXTURE0 + 1);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	// Render coins
	{
		for (int i = 0; i < (*m_pHexPool).size(); ++i) {
			CHexagon* hexagon = (*m_pHexPool)[i];
			if (!hexagon->GetActive()) continue;

			if (glm::abs(hexagon->GetDistance() - (std::fmod(m_currentDistance, m_pCatmullRom->GetTrackLength()))) <= 5000.0f ||
				glm::abs(hexagon->GetDistance() - (std::fmod(m_currentDistance + 1000.0f, m_pCatmullRom->GetTrackLength()))) <= 5000.0f) {
				glm::mat4 rotationScaleMatrix = hexagon->GetRotationOffset() * hexagon->GetRotation();
				rotationScaleMatrix = glm::scale(rotationScaleMatrix, hexagon->GetScale());

				modelViewMatrixStack.Push();
				modelViewMatrixStack.Translate(hexagon->GetPosition());
				pMainProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top() * rotationScaleMatrix);
				pMainProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
				hexagon->Render();

				modelViewMatrixStack.Pop();
			}
		}
	}

	// Render enemies
	{
		for (int i = 0; i < (*m_pEnemy).size(); ++i) {
			CEnemy* enemy = (*m_pEnemy)[i];
			if (!enemy->GetActive()) continue;

			if (glm::abs(enemy->GetDistance() - (std::fmod(m_currentDistance, m_pCatmullRom->GetTrackLength()))) <= 5000.0f ||
				glm::abs(enemy->GetDistance() - (std::fmod(m_currentDistance + 1000.0f, m_pCatmullRom->GetTrackLength()))) <= 5000.0f) {
				glm::mat4 rotationScaleMatrix = enemy->GetRotationOffset() * enemy->GetRotation();
				rotationScaleMatrix = glm::scale(rotationScaleMatrix, enemy->GetScale());

				modelViewMatrixStack.Push();
				modelViewMatrixStack.Translate(enemy->GetPosition());
				pMainProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top() * rotationScaleMatrix);
				pMainProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
				enemy->Render();

				modelViewMatrixStack.Pop();
			}
		}
	}

	// Render bullets
	{
		pMainProgram->SetUniform("material1.Ma", glm::vec3(10.0f));
		pMainProgram->SetUniform("material1.Md", glm::vec3(0.0f));
		pMainProgram->SetUniform("material1.Ms", glm::vec3(0.0f));

		for (int i = 0; i < (*m_pBulletPool).size(); ++i) {
			CHexagon* bullet = (*m_pBulletPool)[i];
			if (!bullet->GetActive()) continue;

			glm::mat4 rotationMatrix = glm::mat4_cast(glm::angleAxis(glm::radians(90.0f), bullet->GetStrafeDirection()));
			rotationMatrix *= glm::inverse(glm::lookAt(glm::vec3(0.0f), glm::vec3(0.0f) + bullet->GetForwardDirection(), bullet->GetUpDirection()));
			bullet->SetRotation(rotationMatrix);

			glm::mat4 rotationScaleMatrix = bullet->GetRotationOffset() * bullet->GetRotation();
			rotationScaleMatrix = glm::scale(rotationScaleMatrix, bullet->GetScale());

			modelViewMatrixStack.Push();
			modelViewMatrixStack.Translate(bullet->GetPosition());
			pMainProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top() * rotationScaleMatrix);
			pMainProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
			bullet->Render();

			modelViewMatrixStack.Pop();
		}

		pMainProgram->SetUniform("material1.Ma", glm::vec3(0.75f));
		pMainProgram->SetUniform("material1.Md", glm::vec3(2.0f));
		pMainProgram->SetUniform("material1.Ms", glm::vec3(2.0f));
	}

	// Render health signs
	{
		for (int i = 0; i < (*m_pHealthPool).size(); ++i) {
			CHealth* health = (*m_pHealthPool)[i];
			if (!health->GetActive()) continue;

			if (glm::abs(health->GetDistance() - (std::fmod(m_currentDistance, m_pCatmullRom->GetTrackLength()))) <= 5000.0f ||
				glm::abs(health->GetDistance() - (std::fmod(m_currentDistance + 1000.0f, m_pCatmullRom->GetTrackLength()))) <= 5000.0f) {
				glm::mat4 rotationScaleMatrix = health->GetRotationOffset() * health->GetRotation();
				rotationScaleMatrix = glm::scale(rotationScaleMatrix, health->GetScale());

				modelViewMatrixStack.Push();
				modelViewMatrixStack.Translate(health->GetPosition());
				pMainProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top() * rotationScaleMatrix);
				pMainProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
				health->Render();

				modelViewMatrixStack.Pop();
			}
		}
	}

	// Use the instanced main shader program
	CShaderProgram* pMainInstancedProgram = (*m_pShaderPrograms)[7];
	pMainInstancedProgram->UseProgram();
	pMainInstancedProgram->SetUniform("bUseTexture", true);
	pMainInstancedProgram->SetUniform("sampler0", 0);

	// Set the projection matrix
	pMainInstancedProgram->SetUniform("matrices.projMatrix", m_pCamera->GetPerspectiveProjectionMatrix());

	// Set light and materials in main shader program
	pMainInstancedProgram->SetUniform("light1.position", viewMatrix * glm::vec4(pointLightPosition, 1.0f)); // Position of light source *in eye coordinates*
	pMainInstancedProgram->SetUniform("light1.La",
		glm::vec3(1.0f)); // Ambient colour of light
	pMainInstancedProgram->SetUniform("light1.Ld",
		glm::vec3(1.0f)); // Diffuse colour of light
	pMainInstancedProgram->SetUniform("light1.Ls",
		glm::vec3(1.0f)); // Specular colour of light
	pMainInstancedProgram->SetUniform("material1.Ma",
		glm::vec3(0.05f)); // Ambient material reflectance
	pMainInstancedProgram->SetUniform("material1.Md",
		glm::vec3(2.0f)); // Diffuse material reflectance
	pMainInstancedProgram->SetUniform("material1.Ms",
		glm::vec3(0.0f)); // Specular material reflectance
	pMainInstancedProgram->SetUniform("material1.shininess",
		15.0f); // Shininess material property

	glCullFace(GL_BACK);
	// Render asteroids
	{
		modelViewMatrixStack.Push();
		pMainInstancedProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
		pMainInstancedProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
		m_pAsteroid->Render();
		modelViewMatrixStack.Pop();
	}

	// Render Street Lights
	{
		modelViewMatrixStack.Push();
		pMainInstancedProgram->SetUniform("material1.emission", 1);
		pMainInstancedProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
		pMainInstancedProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
		m_pStreetLight->Render();
		modelViewMatrixStack.Pop();
	}
	glCullFace(GL_FRONT);

	// Render the octasphere
	{
		/* ---- Planet Noise Generation ---- */
		// Use the main shader program
		CShaderProgram* pPlanetProgram = (*m_pShaderPrograms)[5];
		pPlanetProgram->UseProgram();
		pPlanetProgram->SetUniform("sampler0", 0);
		pPlanetProgram->SetUniform("depthMap", 2);

		glActiveTexture(GL_TEXTURE0 + 2);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_depthCubeMap);

		// Set the projection matrix
		pPlanetProgram->SetUniform("matrices.projMatrix", m_pCamera->GetPerspectiveProjectionMatrix());

		// Set light and materials in main shader program
		pPlanetProgram->SetUniform("farPlane", farPlane);
		pPlanetProgram->SetUniform("light1.position", glm::vec4(pointLightPosition, 1.0f)); // Position of light source *in eye coordinates*
		pPlanetProgram->SetUniform("light1.La",
			glm::vec3(1.0f)); // Ambient colour of light
		pPlanetProgram->SetUniform("light1.Ld",
			glm::vec3(1.0f)); // Diffuse colour of light
		pPlanetProgram->SetUniform("light1.Ls",
			glm::vec3(0.0f)); // Specular colour of light
		pPlanetProgram->SetUniform("material1.Ma",
			glm::vec3(0.5f)); // Ambient material reflectance
		pPlanetProgram->SetUniform("material1.Md",
			glm::vec3(0.5f)); // Diffuse material reflectance
		pPlanetProgram->SetUniform("material1.Ms",
			glm::vec3(1.5f)); // Specular material reflectance
		pPlanetProgram->SetUniform("material1.shininess",
			15.0f); // Shininess material property

		for (int i = 0; i < (*m_pOctaSphere).size(); ++i) {
			COctaSphere* octaSphere = (*m_pOctaSphere)[i];
			pPlanetProgram->SetUniform("time", (float)m_totalTime + octaSphere->GetTimeOffset());
			pPlanetProgram->SetUniform("type", octaSphere->GetType());

			if (octaSphere->IsLightEmitter()) {
				pPlanetProgram->SetUniform("bLightEmitter", true);
			}
			else {
				pPlanetProgram->SetUniform("bLightEmitter", false);
			}

			if (octaSphere->UsingTexture()) {
				pPlanetProgram->SetUniform("bUseTexture", true);
			}
			else {
				pPlanetProgram->SetUniform("bUseTexture", false);
			}

			glm::mat4 transform{};
			transform = glm::translate(transform, octaSphere->GetPosition());
			transform = glm::scale(transform, octaSphere->GetScale());

			modelViewMatrixStack.Push();
			modelViewMatrixStack.Translate(octaSphere->GetPosition());
			modelViewMatrixStack.Scale(octaSphere->GetScale());
			pPlanetProgram->SetUniform("matrices.modelMatrix", transform);
			pPlanetProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
			pPlanetProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
			octaSphere->Render();

			modelViewMatrixStack.Pop();
		}
	}

	// Use the grid shader program
	CShaderProgram* pGridProgram = (*m_pShaderPrograms)[2];
	pGridProgram->UseProgram();
	pGridProgram->SetUniform("time", (float)m_totalTime);
	pGridProgram->SetUniform("trackDistance", m_currentDistance);

	pGridProgram->SetUniform("light1.position", viewMatrix * glm::vec4(pointLightPosition, 1.0f)); // Position of light source *in eye coordinates*
	pGridProgram->SetUniform("light1.La", glm::vec3(2.0f)); // Ambient colour of light
	pGridProgram->SetUniform("light1.Ld", glm::vec3(2.0f)); // Diffuse colour of light
	pGridProgram->SetUniform("light1.Ls", glm::vec3(2.0f)); // Specular colour of light

	pGridProgram->SetUniform("lightPlayer.position", viewMatrix * glm::vec4(m_pPlayer->GetPosition() + m_pPlayer->GetPositionOffset().x * m_pPlayer->GetStrafeDirection(), 1.0f)); // Position of light source *in eye coordinates*
	pGridProgram->SetUniform("lightPlayer.La", glm::vec3(500.0f, 200.0f, 50.0f)); // Ambient colour of light
	pGridProgram->SetUniform("lightPlayer.Ld", glm::vec3(500.0f, 200.0f, 50.0f)); // Diffuse colour of light
	pGridProgram->SetUniform("lightPlayer.Ls", glm::vec3(500.0f, 200.0f, 50.0f)); // Specular colour of light
	pGridProgram->SetUniform("lightPlayer.constant", 10.0f); // Constant Attenuation of Light
	pGridProgram->SetUniform("lightPlayer.linear", 0.09f); // Linear Attenuation of Light
	pGridProgram->SetUniform("lightPlayer.quadratic", 0.032f); // Quadratic Attenuation of Light

	pGridProgram->SetUniform("material1.shininess", 15.0f); // Shininess material property
	// Turn on diffuse + specular materials
	pGridProgram->SetUniform("material1.Ma", glm::vec3(2.0f)); // Ambient material reflectance
	pGridProgram->SetUniform("material1.Md", glm::vec3(0.5f)); // Diffuse material reflectance
	pGridProgram->SetUniform("material1.Ms", glm::vec3(1.5f)); // Specular material reflectance

	// Set the projection matrix
	pGridProgram->SetUniform("matrices.projMatrix", m_pCamera->GetPerspectiveProjectionMatrix());

	// Render track
	{
		glDisable(GL_CULL_FACE);

		glm::mat4 transform(1.0f);

		modelViewMatrixStack.Push();
		pGridProgram->SetUniform("skybox", cubeMapTextureUnit);
		pGridProgram->SetUniform("cameraPos", m_pCamera->GetPosition());
		pGridProgram->SetUniform("matrices.modelMatrix", transform);
		pGridProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
		pGridProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
		m_pCatmullRom->RenderTrack();
		modelViewMatrixStack.Pop();

		// Use the bloom shader program
		float knee = 0.5f;
		CShaderProgram* pBloomProgram = (*m_pShaderPrograms)[3];
		pBloomProgram->UseProgram();
		pBloomProgram->SetUniform("image", 0);
		pBloomProgram->SetUniform("bloomImage", 1);
		pBloomProgram->SetUniform("sampleScale", 1.0f);
		pBloomProgram->SetUniform("colorIntensity", 0.5f);
		pBloomProgram->SetUniform("threshold", glm::vec4(1.1f, knee, knee * 2, 0.25f / knee));
		pBloomProgram->SetUniform("params", 0.5f);
		pBloomProgram->SetUniform("pass", 0);

		glDisable(GL_DEPTH_TEST);
		glBindSampler(0, linearFiltering);

		// NOTE: Could these bloom passes be done more efficiently by utilizing texture mips instead of whole separate textures?

		/* ---- Prefilter pass ---- */
		glViewport(0, 0, bloomBufferResolutions[0].x, bloomBufferResolutions[0].y);
		glBindTexture(GL_TEXTURE_2D, hdrColorBuffer);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, bloomBuffers[0], 0);
		glDrawArrays(GL_TRIANGLES, 0, 3);


		/* ---- Downsample pass ---- */
		pBloomProgram->SetUniform("pass", 1);
		for (int i = 1; i < bloomBufferCount; ++i) {
			glViewport(0, 0, bloomBufferResolutions[i].x, bloomBufferResolutions[i].y);
			glBindTexture(GL_TEXTURE_2D, bloomBuffers[i - 1]);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, bloomBuffers[i], 0);
			glDrawArrays(GL_TRIANGLES, 0, 3);
		}

		/* ---- Upsample & Combine pass ---- */
		pBloomProgram->SetUniform("pass", 2);
		for (int i = bloomBufferCount - 1; i > 0; --i) {
			glViewport(0, 0, bloomBufferResolutions[i - 1].x, bloomBufferResolutions[i - 1].y);
			glActiveTexture(GL_TEXTURE0 + 0);
			glBindTexture(GL_TEXTURE_2D, bloomBuffers[i]);
			glActiveTexture(GL_TEXTURE0 + 1);
			glBindTexture(GL_TEXTURE_2D, bloomBuffers[i - 1]);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, bloomBuffers[i - 1], 0);
			glDrawArrays(GL_TRIANGLES, 0, 3);
		}

		/* ---- Final Composite, Tonemapping & Gamma Correction ---- */
		CShaderProgram* pPostFXProgram = (*m_pShaderPrograms)[4];
		pPostFXProgram->UseProgram();
		pPostFXProgram->SetUniform("scene", 0);
		pPostFXProgram->SetUniform("bloomImage", 1);
		pPostFXProgram->SetUniform("exposure", 0.5f);
		pPostFXProgram->SetUniform("gamma", 2.2f);

		if (m_fadeToBlack) {
			pPostFXProgram->SetUniform("blackCrossfade", m_blackScreenCrossfade);
		}

		glViewport(0, 0, width, height);
		glActiveTexture(GL_TEXTURE0 + 0);
		glBindTexture(GL_TEXTURE_2D, hdrColorBuffer);
		glActiveTexture(GL_TEXTURE0 + 1);
		glBindTexture(GL_TEXTURE_2D, bloomBuffers[0]);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, hdrColorBuffer, 0);
		glDrawArrays(GL_TRIANGLES, 0, 3);
	}
	glReadBuffer(GL_COLOR_ATTACHMENT0);
	glDrawBuffer(GL_COLOR_ATTACHMENT0);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_COLOR_BUFFER_BIT, GL_NEAREST);

	// Draw the 2D graphics after the 3D graphics
	DrawUI();

	// Swap buffers to show the rendered image
	SwapBuffers(m_gameWindow.Hdc());
}

// Update method runs repeatedly with the Render method
void Game::Update() {
	if (m_fadeToBlack) {
		m_blackScreenCrossfade += 0.01f;
		m_dt -= m_dt * m_blackScreenCrossfade; // Slow down the game timer until it is at 0.
	}

	// Update the camera using the amount of time that has elapsed to avoid
	// framerate dependent motion
	m_currentDistance += m_dt * m_cameraSpeed;
	m_currentShootCooldown -= m_dt / 1000.0f;

	// Update player position and direction
	{
		glm::vec3 p;
		m_pCatmullRom->Sample(m_currentDistance + 125.0f, p);

		// Look ahead a bit.
		glm::vec3 pNext;
		float amount = 25.0f;
		for (int i = 1; i < (int)amount + 1; ++i) {
			glm::vec3 tempPos;
			m_pCatmullRom->Sample((m_currentDistance + 125.0f) + (25.0f * i), tempPos);
			pNext += tempPos;
		}
		pNext /= amount;

		// Tangent Vector
		glm::vec3 vecT = glm::normalize(pNext - p);
		glm::vec3 vecN = glm::normalize(glm::cross(vecT, glm::vec3(0.0f, 1.0f, 0.0f))); // Normal Vector
		glm::vec3 vecB = glm::normalize(glm::cross(vecN, vecT)); // Binormal Vector
		p += 50.0f * vecB;
		p.y += glm::sin(1.0f / 20.0f + (m_totalTime * 0.0025f)) * 4.5f;

		m_pPlayer->SetPosition(p);
		m_pPlayer->SetTNBFrame(vecT, vecN, vecB);
		m_pPlayer->Update(m_dt, m_pCatmullRom->GetTrackWidth());
	}

	// Update Camera position and direction
	{
		m_pCamera->Update(m_dt);

		if (!(m_pCamera->GetViewMode() == CCamera::ViewMode::FREEVIEW)) {
			if (m_pCamera->GetViewMode() == CCamera::ViewMode::THIRD_P) {
				glm::vec3 p;
				m_pCatmullRom->Sample(m_currentDistance, p);

				// Look ahead a bit.
				glm::vec3 pNext;
				float amount = 50.0f;
				for (int i = 1; i < (int)amount + 1; ++i) {
					glm::vec3 tempPos;
					m_pCatmullRom->Sample(m_currentDistance + (10.0f * i), tempPos);
					pNext += tempPos;
				}
				pNext /= amount;

				// Tangent Vector
				glm::vec3 vecT = glm::normalize(pNext - p);
				glm::vec3 vecN = glm::normalize(glm::cross(vecT, glm::vec3(0.0f, 1.0f, 0.0f))); // Normal Vector
				glm::vec3 vecB = glm::normalize(glm::cross(vecN, vecT)); // Binormal Vector
				p += 110.0f * vecB;

				m_pCamera->Set(p, p + vecT, m_pCamera->GetUpVector());
			}
			else if (m_pCamera->GetViewMode() == CCamera::ViewMode::FIRST_P) {
				glm::vec3 p;
				m_pCatmullRom->Sample(m_currentDistance + 125.0f, p);

				// Look ahead a bit.
				glm::vec3 pNext;
				float amount = 25.0f;
				for (int i = 1; i < (int)amount + 1; ++i) {
					glm::vec3 tempPos;
					m_pCatmullRom->Sample((m_currentDistance + 125.0f) + (25.0f * i), tempPos);
					pNext += tempPos;
				}
				pNext /= amount;

				// Tangent Vector
				glm::vec3 vecT = glm::normalize(pNext - p);
				glm::vec3 vecN = glm::normalize(glm::cross(vecT, glm::vec3(0.0f, 1.0f, 0.0f))); // Normal Vector
				glm::vec3 vecB = glm::normalize(glm::cross(vecN, vecT)); // Binormal Vector
				p += 85.0f * vecB;
				p += 5.0f * vecT;
				p.y += glm::sin(1.0f / 20.0f + (m_totalTime * 0.0025f)) * 2.5f;

				p += m_pPlayer->GetPositionOffset().x * vecN;

				m_pCamera->Set(p, p + vecT, m_pCamera->GetUpVector());
			}
			else if (m_pCamera->GetViewMode() == CCamera::ViewMode::TOP) {
				glm::vec3 p;
				m_pCatmullRom->Sample(m_currentDistance + 125.0f, p);

				// Look ahead a bit.
				glm::vec3 pNext;
				float amount = 25.0f;
				for (int i = 1; i < (int)amount + 1; ++i) {
					glm::vec3 tempPos;
					m_pCatmullRom->Sample((m_currentDistance + 125.0f) + (25.0f * i), tempPos);
					pNext += tempPos;
				}
				pNext /= amount;

				// Tangent Vector
				glm::vec3 vecT = glm::normalize(pNext - p);
				glm::vec3 vecN = glm::normalize(glm::cross(vecT, glm::vec3(0.0f, 1.0f, 0.0f))); // Normal Vector
				glm::vec3 vecB = glm::normalize(glm::cross(vecN, vecT)); // Binormal Vector
				p += 1000.0f * vecB;
				p += 250.0f * vecT;

				m_pCamera->Set(p, p - vecB, vecT);
			}
		}
	}

	// Update bullet position and direction
	{
		for (int i = 0; i < (*m_pBulletPool).size(); ++i) {
			CHexagon* bulletObj = (*m_pBulletPool)[i];
			if (!bulletObj->GetActive()) continue;

			if (glm::abs(bulletObj->GetDistance() - (std::fmod(m_currentDistance, m_pCatmullRom->GetTrackLength()))) <= 200.0f) {
				if (bulletObj->GetTriggeredBy() != 0) { // If shot by enemy...
					glm::vec3 p;
					m_pCatmullRom->Sample(m_currentDistance + 150.0f, p);

					// Look ahead a bit.
					glm::vec3 pNext;
					float amount = 15.0f;
					for (int i = 1; i < (int)amount + 1; ++i) {
						glm::vec3 tempPos;
						m_pCatmullRom->Sample((m_currentDistance + 150.0f) + (50.0f * i), tempPos);
						pNext += tempPos;
					}
					pNext /= amount;

					// Tangent Vector
					glm::vec3 vecT = glm::normalize(pNext - p);
					glm::vec3 vecN = glm::normalize(glm::cross(vecT, glm::vec3(0.0f, 1.0f, 0.0f))); // Normal Vector
					glm::vec3 vecB = glm::normalize(glm::cross(vecN, vecT)); // Binormal Vector
					bulletObj->SetTNBFrame(-vecT, vecN, vecB); // Travel in opposite direction of the player.
				}
				else { // If shot by player...
					glm::vec3 p;
					m_pCatmullRom->Sample(m_currentDistance + 150.0f, p);

					// Look ahead a bit.
					glm::vec3 pNext;
					float amount = 15.0f;
					for (int i = 1; i < (int)amount + 1; ++i) {
						glm::vec3 tempPos;
						m_pCatmullRom->Sample((m_currentDistance + 150.0f) + (50.0f * i), tempPos);
						pNext += tempPos;
					}
					pNext /= amount;

					// Tangent Vector
					glm::vec3 vecT = glm::normalize(pNext - p);
					glm::vec3 vecN = glm::normalize(glm::cross(vecT, glm::vec3(0.0f, 1.0f, 0.0f))); // Normal Vector
					glm::vec3 vecB = glm::normalize(glm::cross(vecN, vecT)); // Binormal Vector
					bulletObj->SetTNBFrame(vecT + glm::vec3(0.0f, 0.15f, 0.0f), vecN, vecB);
				}
			}
			else if (glm::abs(bulletObj->GetDistance() - (std::fmod(m_currentDistance, m_pCatmullRom->GetTrackLength()))) > 3000.0f) {
				bulletObj->SetActive(false); // Disable the bullets after they travelled far enough.
			}
			bulletObj->SetPosition(bulletObj->GetPosition() + (50.0f * bulletObj->GetForwardDirection()));
		}
	}

	// Update enemy state
	{
		for (int i = 0; i < (*m_pEnemy).size(); ++i) {
			CEnemy* enemyObj = (*m_pEnemy)[i];

			if (enemyObj->GetHealth() > 0.0f && glm::abs(enemyObj->GetDistance() - (std::fmod(m_currentDistance, m_pCatmullRom->GetTrackLength()))) <= 5000.0f) {
				enemyObj->SetActive(true);
			}
			else {
				enemyObj->SetActive(false);
			}

			if (!enemyObj->GetActive()) continue;

			enemyObj->Update(m_dt, m_pCatmullRom->GetTrackWidth());

			// Shoot bullet if cooldown is done. Not used.
			//if (enemyObj->IsShootCooldownFinished()) {
			//	m_currentBullet = (m_currentBullet + 1) % m_pBulletPool->size();

			//	CHexagon* bulletObj = (*m_pBulletPool)[m_currentBullet];
			//	bulletObj->SetActive(true);
			//	bulletObj->SetTriggeredBy(1); // Triggered by enemy.
			//	bulletObj->SetDistance(m_currentDistance);

			//	bulletObj->SetPosition(enemyObj->GetPosition() + (enemyObj->GetPositionOffset().x * enemyObj->GetStrafeVector()));
			//	bulletObj->SetTNBFrame(enemyObj->GetForwardVector(), enemyObj->GetStrafeVector(), enemyObj->GetUpVector());

			//	enemyObj->ResetShootCooldown();
			//}

			// Animate Enemy
			glm::vec3 position = enemyObj->GetPosition();

			position.y += glm::sin(1.0f / 20.0f + (m_totalTime * 0.005f)) * 0.25f;

			enemyObj->SetPosition(position);
		}
	}

	// Animate coin pickup
	for (int i = 0; i < (*m_pHexPool).size(); ++i) {
		CHexagon* hexObj = (*m_pHexPool)[i];

		glm::vec3 position = hexObj->GetPosition();

		position.y += glm::sin(1.0f / 20.0f + (m_totalTime * 0.005f)) * 0.15f;

		hexObj->SetPosition(position);

		glm::mat4 rotationMatrix = glm::mat4_cast(glm::angleAxis(glm::radians(std::fmod((float)m_totalTime * 0.1f, 360.0f)), hexObj->GetUpDirection()));
		hexObj->SetRotationOffset(rotationMatrix);
	}

	// Animate health pickup
	for (int i = 0; i < (*m_pHealthPool).size(); ++i) {
		CHealth* healthObj = (*m_pHealthPool)[i];

		glm::vec3 position = healthObj->GetPosition();

		position.y += glm::sin(1.0f / 20.0f + (m_totalTime * 0.005f)) * 0.15f;

		healthObj->SetPosition(position);

		glm::mat4 rotationMatrix = glm::mat4_cast(glm::angleAxis(glm::radians(std::fmod((float)m_totalTime * 0.1f, 360.0f)), healthObj->GetUpVector()));
		healthObj->SetRotationOffset(rotationMatrix);
	}

	// Collision Detection - This could be optimized by utilizing a octree.
	{
		// Coins
		for (int i = 0; i < (*m_pHexPool).size(); ++i) {
			BoundingBox playerBB = m_pPlayer->GetBoundingBox();
			CHexagon* hexObj = (*m_pHexPool)[i];
			if (!hexObj->GetActive()) continue;

			if (glm::abs(hexObj->GetDistance() - (std::fmod(m_currentDistance, m_pCatmullRom->GetTrackLength()))) <= 500.0f) {
				if (hexObj->GetBoundingBox().intersect(playerBB)) {
					hexObj->SetActive(false);
					m_score += hexObj->GetValue();
				}
			}
		}

		// Bullets
		for (int i = 0; i < (*m_pBulletPool).size(); ++i) {
			CHexagon* bulletObj = (*m_pBulletPool)[i];
			if (!bulletObj->GetActive()) continue;

			if (bulletObj->GetTriggeredBy() != 0) {
				if (bulletObj->GetBoundingBox().intersect(m_pPlayer->GetBoundingBox())) {
					bulletObj->SetActive(false);
					m_pPlayer->SetHealth(m_pPlayer->GetHealth() - bulletObj->GetValue());

					if (m_pPlayer->GetHealth() <= 0.0f) {
						m_fadeToBlack = true;
					}
				}
			}
			else {
				for (int i = 0; i < (*m_pEnemy).size(); ++i) {
					CEnemy* enemyObj = (*m_pEnemy)[i];
					if (!enemyObj->GetActive()) continue;

					if (bulletObj->GetBoundingBox().intersect(enemyObj->GetBoundingBox())) {
						bulletObj->SetActive(false);
						enemyObj->SetHealth(enemyObj->GetHealth() - bulletObj->GetValue());

						if (enemyObj->GetHealth() <= 0.0f) {
							enemyObj->SetActive(false);
							m_score += 250.0f;
						}
					}
				}
			}
		}

		// Enemy
		if (!godMode) {
			for (int i = 0; i < (*m_pEnemy).size(); ++i) {
				CEnemy* enemyObj = (*m_pEnemy)[i];
				if (!enemyObj->GetActive()) continue;

				if (enemyObj->GetBoundingBox().intersect(m_pPlayer->GetBoundingBox())) {
					enemyObj->SetActive(false);
					enemyObj->SetHealth(0.0f);
					m_pPlayer->SetHealth(m_pPlayer->GetHealth() - enemyObj->GetDamage());

					if (m_pPlayer->GetHealth() <= 0.0f) {
						m_fadeToBlack = true;
					}
				}
			}
		}

		// Health
		if (m_pPlayer->GetHealth() != m_pPlayer->GetMaxHealth()) {
			for (int i = 0; i < (*m_pHealthPool).size(); ++i) {
				BoundingBox playerBB = m_pPlayer->GetBoundingBox();
				CHealth* healthObj = (*m_pHealthPool)[i];
				if (!healthObj->GetActive()) continue;

				if (glm::abs(healthObj->GetDistance() - (std::fmod(m_currentDistance, m_pCatmullRom->GetTrackLength()))) <= 500.0f) {
					if (healthObj->GetBoundingBox().intersect(playerBB)) {
						healthObj->SetActive(false);
						m_pPlayer->SetHealth(m_pPlayer->GetHealth() + healthObj->GetHealth());
					}
				}
			}
		}
	}

	// Spawn bullet at player location on shoot.
	if (GetKeyState(VK_LBUTTON) & 0x80 && m_currentShootCooldown <= 0.0f) {
		m_currentShootCooldown = m_shootCooldown;
		m_currentBullet = (m_currentBullet + 1) % m_pBulletPool->size();

		CHexagon* bulletObj = (*m_pBulletPool)[m_currentBullet];
		bulletObj->SetActive(true);
		bulletObj->SetTriggeredBy(0); // Triggered by player.
		bulletObj->SetDistance(std::fmod(m_currentDistance, m_pCatmullRom->GetTrackLength()));

		bulletObj->SetPosition(m_pPlayer->GetPosition() + (m_pPlayer->GetPositionOffset().x * m_pPlayer->GetStrafeDirection()) + (150.0f * m_pPlayer->GetForwardDirection()));
		bulletObj->SetTNBFrame(m_pPlayer->GetForwardDirection(), m_pPlayer->GetStrafeDirection(), m_pPlayer->GetUpDirection());
	}

	// Speed up/slow down camera.
	if (GetKeyState('Z') & 0x80) {
		m_cameraSpeed = 5.0f;
	}
	else {
		m_cameraSpeed = 0.85f;
	}

	// Enable/Disbale godmode
	if (GetKeyState('G') & 0x80) {
		godMode = true;
	}

	if (GetKeyState('H') & 0x80) {
		godMode = false;
	}
}

void Game::DrawUI() {

	CShaderProgram* fontProgram = (*m_pShaderPrograms)[1];
	RECT dimensions = m_gameWindow.GetDimensions();
	int height = dimensions.bottom - dimensions.top;

	// Draw Framerate
	{
		// Increase the elapsed time and frame counter
		m_totalTime += m_dt;
		m_elapsedTime += m_dt;
		m_frameCount++;

		// Now we want to subtract the current time by the last time that was stored
		// to see if the time elapsed has been over a second, which means we found our
		// FPS.
		if (m_elapsedTime > 1000) {
			m_elapsedTime = 0;
			m_framesPerSecond = m_frameCount;

			// Reset the frames per second
			m_frameCount = 0;
		}

		if (m_framesPerSecond > 0) {
			// Use the font shader program and render the text
			fontProgram->UseProgram();
			glDisable(GL_DEPTH_TEST);
			fontProgram->SetUniform("matrices.modelViewMatrix", glm::mat4(1.0f));
			fontProgram->SetUniform("matrices.projMatrix", m_pCamera->GetOrthographicProjectionMatrix());
			fontProgram->SetUniform("vColour", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
			m_pFtFont->Render(20, height - 20, 20, "FPS: %d", m_framesPerSecond);
		}
	}

	// Draw Score & Health
	{
		// Use the font shader program and render the text
		fontProgram->UseProgram();
		glDisable(GL_DEPTH_TEST);
		fontProgram->SetUniform("matrices.modelViewMatrix", glm::mat4(1.0f));
		fontProgram->SetUniform("matrices.projMatrix", m_pCamera->GetOrthographicProjectionMatrix());
		fontProgram->SetUniform("vColour", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
		m_pFtFont->Render(20, 20, 30, "Score: %.1f", m_score);
		m_pFtFont->Render(20, 60, 30, "Health: %.1f", m_pPlayer->GetHealth());
		if (godMode) {
			m_pFtFont->Render((width / 2.0f) - 120, 20, 30, "Godmode: Enabled");
		}
	}
}

// The game loop runs repeatedly until game over
void Game::GameLoop() {
	/*
	// Fixed timer
	dDt = pHighResolutionTimer->Elapsed();
	if (dDt > 1000.0 / (double) Game::FPS) {
			pHighResolutionTimer->Start();
			Update();
			Render();
	}
	*/

	// Variable timer
	m_pHighResolutionTimer->Start();
	Update();
	Render();
	m_dt = m_pHighResolutionTimer->Elapsed();
}

WPARAM Game::Execute() {
	m_pHighResolutionTimer = new CHighResolutionTimer;
	m_gameWindow.Init(m_hInstance);

	if (!m_gameWindow.Hdc()) {
		return 1;
	}

	MSG msg;
	while (1) {
		Initialise();
		m_pHighResolutionTimer->Start();

		while (1) {
			if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
				if (msg.message == WM_QUIT) {
					break;
				}

				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else if (m_fadeToBlack && m_blackScreenCrossfade >= 1.0f) { // Restart game if player died and fade to black is finished.
				Reset();
				break;
			}
			else if (m_appActive) {
				GameLoop();
			}
			else
				Sleep(200); // Do not consume processor power if application isn't active
		}

		if (msg.message == WM_QUIT) {
			break;
		}
		Sleep(500);
	}

	m_gameWindow.Deinit();

	return (msg.wParam);
}

LRESULT Game::ProcessEvents(HWND window, UINT message, WPARAM w_param,
	LPARAM l_param) {
	LRESULT result = 0;

	switch (message) {

	case WM_ACTIVATE: {
		switch (LOWORD(w_param)) {
		case WA_ACTIVE:
		case WA_CLICKACTIVE:
			m_appActive = true;
			m_pHighResolutionTimer->Start();
			break;
		case WA_INACTIVE:
			m_appActive = false;
			break;
		}
		break;
	}

	case WM_SIZE:
		RECT dimensions;
		GetClientRect(window, &dimensions);
		m_gameWindow.SetDimensions(dimensions);
		break;

	case WM_PAINT:
		PAINTSTRUCT ps;
		BeginPaint(window, &ps);
		EndPaint(window, &ps);
		break;

	case WM_KEYDOWN:
		switch (w_param) {
		case VK_ESCAPE:
			PostQuitMessage(0);
			break;
		}
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default:
		result = DefWindowProc(window, message, w_param, l_param);
		break;
	}

	return result;
}

Game& Game::GetInstance() {
	static Game instance;

	return instance;
}

void Game::SetHinstance(HINSTANCE hinstance) { m_hInstance = hinstance; }

LRESULT CALLBACK WinProc(HWND window, UINT message, WPARAM w_param,
	LPARAM l_param) {
	return Game::GetInstance().ProcessEvents(window, message, w_param, l_param);
}

int WINAPI wmain(HINSTANCE hinstance, HINSTANCE, PSTR, int) {
	Game& game = Game::GetInstance();
	game.SetHinstance(hinstance);

	return int(game.Execute());
}
