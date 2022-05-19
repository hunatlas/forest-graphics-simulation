#pragma once

// C++ includes
#include <memory>

// GLEW
#include <GL/glew.h>

// SDL
#include <SDL.h>
#include <SDL_opengl.h>

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform2.hpp>

#include "includes/gCamera.h"

#include "includes/ProgramObject.h"
#include "includes/BufferObject.h"
#include "includes/VertexArrayObject.h"
#include "includes/TextureObject.h"

// mesh
#include "includes/ObjParser_OGL3.h"

class CMyApp
{
public:
	CMyApp();
	~CMyApp();

	bool Init();
	void Clean();

	void Update();
	void Render();

	void KeyboardDown(SDL_KeyboardEvent&);
	void KeyboardUp(SDL_KeyboardEvent&);
	void MouseMove(SDL_MouseMotionEvent&);
	void MouseDown(SDL_MouseButtonEvent&);
	void MouseUp(SDL_MouseButtonEvent&);
	void MouseWheel(SDL_MouseWheelEvent&);
	void Resize(int, int);

protected:
	// shaderekhez szükséges változók
	ProgramObject		m_program;			// mesh shader
	ProgramObject		m_programSkybox;	// skybox shader

	VertexArrayObject	m_SkyboxVao;
	IndexBuffer			m_SkyboxIndices;	
	ArrayBuffer			m_SkyboxPos;

	VertexArrayObject	m_GroundVao;
	IndexBuffer			m_GroundIndices;
	ArrayBuffer			m_GroundVertexBuffer;

	VertexArrayObject	m_ForestVao;
	IndexBuffer			m_ForestIndices;
	ArrayBuffer			m_ForestVertexBuffer;

	VertexArrayObject	m_SpheretVao;
	IndexBuffer			m_SphereIndices;
	ArrayBuffer			m_SphereVertexBuffer;

	gCamera				m_camera;

	Texture2D			m_grassTexture;
	Texture2D			m_forestTexture;
	Texture2D			m_barkTexture;
	Texture2D			m_leavesTexture;
	TextureCubeMap		m_skyboxTexture;

	struct Vertex
	{
		glm::vec3 p;
		glm::vec3 n;
		glm::vec2 t;
	};

	// mesh adatok
	std::unique_ptr<Mesh> m_cylinderMesh;

	// a jobb olvashatóság kedvéért
	void InitShaders();
	void InitGround();
	void InitForest();
	void InitSphere();
	void InitSkyBox();

	void RenderCylinder(glm::mat4 world);
	void RenderSphere(glm::mat4 world);
	void RenderTree(glm::vec3 position, float scale);

	glm::vec3 GetGroundPos(float u, float v);
	glm::vec3 GetGroundNorm(float u, float v);

	glm::vec3 GetSpherePos(float u, float v);
	glm::vec3 GetSphereNorm(float u, float v);

	glm::vec2 GetTex(float u, float v);

	const static int GROUND_N = 19;
	float m_trees[GROUND_N][GROUND_N];
	std::vector<std::string> rows{"Can grow anywhere", "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13", "14", "15", "16", "17", "18"};

	float starting_time;
	void populateForest();
	void growTrees();
	void cutTrees();

	glm::vec3 light_dir_vec = glm::vec3(-1.f, -1.f, -1.f);
	std::vector<glm::vec3> ambient_season;
	int seasonNumber = 0;
	glm::vec3 ambient_current;
	glm::vec3 diffuse_current = glm::vec3(0.6f, 0.6f, 0.6f);

	float populationTime = 0.f;
	float treeGrowUpTime = 0.f;
	float seasonTime = 0.f;

	int destroyedTrees = 0;
	int treeGrowTime = 10;
	int yearLength = 20;
	float treeHightToCut = 1.f;
	int noGrowRow = 0;
};