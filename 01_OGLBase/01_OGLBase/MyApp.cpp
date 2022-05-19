#include "MyApp.h"

#include <math.h>
#include <vector>

#include <array>
#include <list>
#include <tuple>
#include <random>
#include <imgui/imgui.h>
#include "includes/GLUtils.hpp"

CMyApp::CMyApp(void)
{
	m_camera.SetView(glm::vec3(5, 5, 5), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	m_cylinderMesh = nullptr;
}

CMyApp::~CMyApp(void)
{
}

glm::vec3 CMyApp::GetGroundPos(float u, float v)
{
	return glm::vec3(u * 20.f - 10.f, sinf(u * 10.f * v) * 0.5f, -v * 20.f + 10.f);
}

glm::vec3 CMyApp::GetGroundNorm(float u, float v)
{
	
	glm::vec3 du = GetGroundPos(u + 0.01, v) - GetGroundPos(u - 0.01, v);
	glm::vec3 dv = GetGroundPos(u, v + 0.01) - GetGroundPos(u, v - 0.01);

	return glm::normalize(glm::cross(du, dv));
}

glm::vec2 CMyApp::GetTex(float u, float v)
{
	return glm::vec2(u, 1 - v);
}

void CMyApp::InitGround()
{
	Vertex vert[(GROUND_N + 1) * (GROUND_N + 1)];
	for (int i = 0; i <= GROUND_N; ++i)
		for (int j = 0; j <= GROUND_N; ++j)
		{
			float u = i / (float)GROUND_N;
			float v = j / (float)GROUND_N;

			vert[i + j * (GROUND_N + 1)].p = GetGroundPos(u, v);
			vert[i + j * (GROUND_N + 1)].n = GetGroundNorm(u, v);
			vert[i + j * (GROUND_N + 1)].t = GetTex(u, v);
		}

	GLushort indices[3 * 2 * (GROUND_N) * (GROUND_N)];
	for (int i = 0; i < GROUND_N; ++i)
		for (int j = 0; j < GROUND_N; ++j)
		{
			indices[6 * i + j * 3 * 2 * (GROUND_N)+0] = (i)+(j) * (GROUND_N + 1);
			indices[6 * i + j * 3 * 2 * (GROUND_N)+1] = (i + 1) + (j) * (GROUND_N + 1);
			indices[6 * i + j * 3 * 2 * (GROUND_N)+2] = (i)+(j + 1) * (GROUND_N + 1);
			indices[6 * i + j * 3 * 2 * (GROUND_N)+3] = (i + 1) + (j) * (GROUND_N + 1);
			indices[6 * i + j * 3 * 2 * (GROUND_N)+4] = (i + 1) + (j + 1) * (GROUND_N + 1);
			indices[6 * i + j * 3 * 2 * (GROUND_N)+5] = (i)+(j + 1) * (GROUND_N + 1);
		}

	m_GroundVertexBuffer.BufferData(vert);
	m_GroundIndices.BufferData(indices);

	m_GroundVao.Init(
		{
			{ CreateAttribute<0, glm::vec3, 0, sizeof(Vertex)>, m_GroundVertexBuffer },
			{ CreateAttribute<1, glm::vec3, (sizeof(glm::vec3)), sizeof(Vertex)>, m_GroundVertexBuffer },
			{ CreateAttribute<2, glm::vec2, (2 * sizeof(glm::vec3)), sizeof(Vertex)>, m_GroundVertexBuffer }
		},
		m_GroundIndices
	);
}

void CMyApp::InitForest()
{
	std::vector<Vertex>vertices;

	//front									 
	vertices.push_back({ glm::vec3(-10, -1, 10), glm::vec3(0, 0, 1), glm::vec2(0, 0) });
	vertices.push_back({ glm::vec3(10, -1, 10), glm::vec3(0, 0, 1), glm::vec2(1, 0) });
	vertices.push_back({ glm::vec3(-10, 5, 10), glm::vec3(0, 0, 1), glm::vec2(0, 1) });
	vertices.push_back({ glm::vec3(10, 5, 10), glm::vec3(0, 0, 1), glm::vec2(1, 1) });
	//back
	vertices.push_back({ glm::vec3(10, -1, -10), glm::vec3(0, 0, -1), glm::vec2(0, 0) });
	vertices.push_back({ glm::vec3(-10, -1, -10), glm::vec3(0, 0, -1), glm::vec2(1, 0) });
	vertices.push_back({ glm::vec3(10, 5, -10), glm::vec3(0, 0, -1), glm::vec2(0, 1) });
	vertices.push_back({ glm::vec3(-10, 5, -10), glm::vec3(0, 0, -1), glm::vec2(1, 1) });
	//right									 
	vertices.push_back({ glm::vec3(10, -1, 10), glm::vec3(1, 0, 0), glm::vec2(0, 0) });
	vertices.push_back({ glm::vec3(10, -1, -10), glm::vec3(1, 0, 0), glm::vec2(1, 0) });
	vertices.push_back({ glm::vec3(10, 5, 10), glm::vec3(1, 0, 0), glm::vec2(0, 1) });
	vertices.push_back({ glm::vec3(10, 5, -10), glm::vec3(1, 0, 0), glm::vec2(1, 1) });
	//left									 
	vertices.push_back({ glm::vec3(-10, -1, -10), glm::vec3(-1, 0, 0), glm::vec2(0, 0) });
	vertices.push_back({ glm::vec3(-10, -1, 10), glm::vec3(-1, 0, 0), glm::vec2(1, 0) });
	vertices.push_back({ glm::vec3(-10, 5, -10), glm::vec3(-1, 0, 0), glm::vec2(0, 1) });
	vertices.push_back({ glm::vec3(-10, 5, 10), glm::vec3(-1, 0, 0), glm::vec2(1, 1) });

	int indices[24] = {0, 2, 1, 1, 2, 3, 4, 6, 5, 5, 6, 7, 8, 10, 9, 9, 10, 11, 12, 14, 13, 13, 14, 15};

	m_ForestVertexBuffer.BufferData(vertices);
	m_ForestIndices.BufferData(indices);

	m_ForestVao.Init(
		{
			{ CreateAttribute<0, glm::vec3, 0, sizeof(Vertex)>, m_ForestVertexBuffer },
			{ CreateAttribute<1, glm::vec3, (sizeof(glm::vec3)), sizeof(Vertex)>, m_ForestVertexBuffer },
			{ CreateAttribute<2, glm::vec2, (2 * sizeof(glm::vec3)), sizeof(Vertex)>, m_ForestVertexBuffer }
		},
		m_ForestIndices
	);
}

void CMyApp::InitSphere()
{
	Vertex vert[(GROUND_N + 1) * (GROUND_N + 1)];
	for (int i = 0; i <= GROUND_N; ++i)
		for (int j = 0; j <= GROUND_N; ++j)
		{
			float u = i / (float)GROUND_N;
			float v = j / (float)GROUND_N;

			vert[i + j * (GROUND_N + 1)].p = GetSpherePos(u, v);
			vert[i + j * (GROUND_N + 1)].n = GetSphereNorm(u, v);
			vert[i + j * (GROUND_N + 1)].t = GetTex(u, v);
		}

	GLushort indices[3 * 2 * (GROUND_N) * (GROUND_N)];
	for (int i = 0; i < GROUND_N; ++i)
		for (int j = 0; j < GROUND_N; ++j)
		{
			indices[6 * i + j * 3 * 2 * (GROUND_N)+0] = (i)+(j) * (GROUND_N + 1);
			indices[6 * i + j * 3 * 2 * (GROUND_N)+1] = (i + 1) + (j) * (GROUND_N + 1);
			indices[6 * i + j * 3 * 2 * (GROUND_N)+2] = (i)+(j + 1) * (GROUND_N + 1);
			indices[6 * i + j * 3 * 2 * (GROUND_N)+3] = (i + 1) + (j) * (GROUND_N + 1);
			indices[6 * i + j * 3 * 2 * (GROUND_N)+4] = (i + 1) + (j + 1) * (GROUND_N + 1);
			indices[6 * i + j * 3 * 2 * (GROUND_N)+5] = (i)+(j + 1) * (GROUND_N + 1);
		}

	m_SphereVertexBuffer.BufferData(vert);
	m_SphereIndices.BufferData(indices);

	m_SpheretVao.Init(
		{
			{ CreateAttribute<0, glm::vec3, 0, sizeof(Vertex)>, m_SphereVertexBuffer },
			{ CreateAttribute<1, glm::vec3, (sizeof(glm::vec3)), sizeof(Vertex)>, m_SphereVertexBuffer },
			{ CreateAttribute<2, glm::vec2, (2 * sizeof(glm::vec3)), sizeof(Vertex)>, m_SphereVertexBuffer }
		},
		m_SphereIndices
	);
}

glm::vec3 CMyApp::GetSpherePos(float u, float v)
{
	u *= float(2 * M_PI);
	v *= float(M_PI);
	float r = 1;

	return glm::vec3(r * sin(v) * cos(u),
					 r * cos(v),
					 r * sin(v) * sin(u));
}

glm::vec3 CMyApp::GetSphereNorm(float u, float v)
{
	u *= float(2 * M_PI);
	v *= float(M_PI);
	return glm::vec3(sin(v) * cos(u), cos(v), sin(v) * sin(u));
}

void CMyApp::InitSkyBox()
{
	m_SkyboxPos.BufferData(
		std::vector<glm::vec3>{
		// hátsó lap
		glm::vec3(-1, -1, -1),
		glm::vec3(1, -1, -1),
		glm::vec3(1, 1, -1),
		glm::vec3(-1, 1, -1),
		// elülső lap
		glm::vec3(-1, -1, 1),
		glm::vec3(1, -1, 1),
		glm::vec3(1, 1, 1),
		glm::vec3(-1, 1, 1),
	}
	);

	// és a primitíveket alkotó csúcspontok indexei (az előző tömbökből) - triangle list-el való kirajzolásra felkészülve
	m_SkyboxIndices.BufferData(
		std::vector<int>{
			// hátsó lap
			0, 1, 2,
			2, 3, 0,
			// elülső lap
			4, 6, 5,
			6, 4, 7,
			// bal
			0, 3, 4,
			4, 3, 7,
			// jobb
			1, 5, 2,
			5, 6, 2,
			// alsó
			1, 0, 4,
			1, 4, 5,
			// felső
			3, 2, 6,
			3, 6, 7,
	}
	);

	// geometria VAO-ban való regisztrálása
	m_SkyboxVao.Init(
		{
			{ CreateAttribute<0, glm::vec3, 0, sizeof(glm::vec3)>, m_SkyboxPos },
		}, m_SkyboxIndices
	);

	// skybox texture
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

	m_skyboxTexture.AttachFromFile("assets/xpos.png", false, GL_TEXTURE_CUBE_MAP_POSITIVE_X);
	m_skyboxTexture.AttachFromFile("assets/xneg.png", false, GL_TEXTURE_CUBE_MAP_NEGATIVE_X);
	m_skyboxTexture.AttachFromFile("assets/ypos.png", false, GL_TEXTURE_CUBE_MAP_POSITIVE_Y);
	m_skyboxTexture.AttachFromFile("assets/yneg.png", false, GL_TEXTURE_CUBE_MAP_NEGATIVE_Y);
	m_skyboxTexture.AttachFromFile("assets/zpos.png", false, GL_TEXTURE_CUBE_MAP_POSITIVE_Z);
	m_skyboxTexture.AttachFromFile("assets/zneg.png", true, GL_TEXTURE_CUBE_MAP_NEGATIVE_Z);

	// a GL_TEXTURE_MAG_FILTER-t és a GL_TEXTURE_MIN_FILTER-t beállítja az AttachFromFile
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

void CMyApp::InitShaders()
{
	m_program.Init(
		{
			{ GL_VERTEX_SHADER, "myVert.vert" },
			{ GL_FRAGMENT_SHADER, "myFrag.frag" }
		},
		{
			{ 0, "vs_in_pos" },
			{ 1, "vs_in_norm" },
			{ 2, "vs_in_tex" },
		}
	);

	m_programSkybox.Init(
		{
			{ GL_VERTEX_SHADER, "skybox.vert" },
			{ GL_FRAGMENT_SHADER, "skybox.frag" }
		},
		{
			{ 0, "vs_in_pos" },
		}
	);
}

void CMyApp::RenderCylinder(glm::mat4 world)
{
	glm::mat4 viewProj = m_camera.GetViewProj();
	m_program.Use();
	
	m_program.SetUniform("MVP", viewProj * world);
	m_program.SetUniform("world", world);
	m_program.SetUniform("worldIT", glm::inverse(glm::transpose(world)));
	m_program.SetUniform("eyePos", m_camera.GetEye());
	m_program.SetUniform("light_dir", light_dir_vec);
	m_program.SetUniform("La", ambient_current);
	m_program.SetUniform("Ld", diffuse_current);
	m_program.SetTexture("texImage", 0, m_barkTexture);

	m_cylinderMesh->draw();
	
	m_program.Unuse();
}

void CMyApp::RenderSphere(glm::mat4 world)
{
	glm::mat4 viewProj = m_camera.GetViewProj();
	m_program.Use();
	
	m_program.SetUniform("MVP", viewProj * world);
	m_program.SetUniform("world", world);
	m_program.SetUniform("worldIT", glm::inverse(glm::transpose(world)));
	m_program.SetUniform("eyePos", m_camera.GetEye());
	m_program.SetUniform("light_dir", light_dir_vec);
	m_program.SetUniform("La", ambient_current);
	m_program.SetUniform("Ld", diffuse_current);
	m_program.SetTexture("texImage", 0, m_leavesTexture);

	m_SpheretVao.Bind();
	glDrawElements(GL_TRIANGLES, 3 * 2 * (GROUND_N) * (GROUND_N), GL_UNSIGNED_SHORT, 0);
	m_SpheretVao.Unbind();

	m_program.Unuse();
}

void CMyApp::RenderTree(glm::vec3 position, float scale)
{
	glm::mat4 world = glm::translate(position) * glm::translate(glm::vec3(0.f, 2.f * scale, 0.f)) * glm::scale(glm::vec3(scale, scale, scale));

	RenderCylinder(world);
	RenderSphere(world * glm::translate(glm::vec3(0.f, 2.f, 0.f)));
}

void CMyApp::populateForest()
{
	
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<> rnd(0, 1);
	

	for (int i = 1; i < GROUND_N; ++i)
		for (int j = 1; j < GROUND_N; ++j)
			if (i != noGrowRow && m_trees[i][j] == 0.f)
				if ((((int)(rnd(gen) * 19)) + 1) == 5)
					m_trees[i][j] = 0.05f;
}

void CMyApp::growTrees()
{
	for (int i = 1; i < GROUND_N; ++i)
		for (int j = 1; j < GROUND_N; ++j)
			if (m_trees[i][j] > 0.f)
				m_trees[i][j] += 0.1f / (treeGrowTime / 10.f);
}

void CMyApp::cutTrees()
{
	for (int i = 0; i < GROUND_N; ++i)
		for (int j = 0; j < GROUND_N; ++j)
			if (m_trees[i][j] >= treeHightToCut) {
				m_trees[i][j] = 0.f;
				destroyedTrees++;
			}
					
}

bool CMyApp::Init()
{
	// törlési szín legyen kékes
	glClearColor(0.125f, 0.25f, 0.5f, 1.0f);

	glEnable(GL_CULL_FACE); // kapcsoljuk be a hatrafele nezo lapok eldobasat
	glEnable(GL_DEPTH_TEST); // mélységi teszt bekapcsolása (takarás)

	InitShaders();
	InitGround();
	InitForest();
	InitSphere();
	InitSkyBox();

	// egyéb textúrák betöltése
	m_grassTexture.FromFile("assets/grass.jpg");
	m_forestTexture.FromFile("assets/forest.jpg");
	m_barkTexture.FromFile("assets/bark.jpg");
	m_leavesTexture.FromFile("assets/leaves.jpg");

	// mesh betöltése
	m_cylinderMesh = std::unique_ptr<Mesh>(ObjParser::parse("assets/henger.obj"));
	m_cylinderMesh->initBuffers();
	
	// kamera
	m_camera.SetProj(glm::radians(60.0f), 640.0f / 480.0f, 0.01f, 1000.0f);

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<> rnd(0.05, 1.00);
	std::uniform_real_distribution<> rndTrees(50, 100);
	std::uniform_real_distribution<> rndGround(1, 18);

	int trees = (int)rndTrees(gen);
	int plantedTrees = 0;

	for (int i = 0; i < GROUND_N; ++i)
		for (int j = 0; j < GROUND_N; ++j)
			m_trees[i][j] = 0.f;

	while (plantedTrees < trees)
	{
		int groundX = (int)rndGround(gen);
		int groundY = (int)rndGround(gen);
		if (m_trees[groundX][groundY] == 0.f)
		{
			m_trees[groundX][groundY] = rnd(gen);
			plantedTrees++;
		}
	}

	ambient_season.push_back(glm::vec3(0.5, 0.7, 0.6));
	ambient_season.push_back(glm::vec3(0.7, 0.7, 0.6));
	ambient_season.push_back(glm::vec3(0.6, 0.4, 0.3));
	ambient_season.push_back(glm::vec3(0.3, 0.6, 0.7));
	ambient_current = ambient_season[seasonNumber];

	starting_time = SDL_GetTicks() / 1000.f;

	return true;
}

void CMyApp::Clean()
{
}

void CMyApp::Update()
{
	static Uint32 last_time = SDL_GetTicks();
	float delta_time = (SDL_GetTicks() - last_time) / 1000.0f;

	m_camera.Update(delta_time);

	populationTime += delta_time;
	if (populationTime >= 2.f)
	{
		populateForest();
		populationTime = 0.f;
	}

	treeGrowUpTime += delta_time;
	if (treeGrowUpTime >= 1.f)
	{
		growTrees();
		treeGrowUpTime = 0.f;
	}
	
	seasonTime += delta_time;
	if (seasonTime >= yearLength / 4)
	{
		seasonNumber++;
		if (seasonNumber == 4) seasonNumber = 0;
		ambient_current = ambient_season[seasonNumber];
		seasonTime = 0.f;
	}
	
	last_time = SDL_GetTicks();
	
}

void CMyApp::Render()
{
	// töröljük a frampuffert (GL_COLOR_BUFFER_BIT) és a mélységi Z puffert (GL_DEPTH_BUFFER_BIT)
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::mat4 viewProj = m_camera.GetViewProj();

	// Ground
	m_program.Use();
	m_program.SetUniform("MVP", viewProj);
	m_program.SetUniform("world", glm::mat4(1.f));
	m_program.SetUniform("worldIT", glm::inverse(glm::transpose(glm::mat4(1.f))));
	m_program.SetUniform("eyePos", m_camera.GetEye());
	m_program.SetUniform("light_dir", light_dir_vec);
	m_program.SetUniform("La", ambient_current);
	m_program.SetUniform("Ld", diffuse_current);
	m_program.SetTexture("texImage", 0, m_grassTexture);

	m_GroundVao.Bind();
	glDrawElements(GL_TRIANGLES, 3 * 2 * (GROUND_N) * (GROUND_N), GL_UNSIGNED_SHORT, 0);
	m_GroundVao.Unbind();

	m_ForestVao.Bind();
	m_program.SetTexture("texImage", 0, m_forestTexture);
	glDrawElements(GL_TRIANGLES, 24, GL_UNSIGNED_INT, 0);
	m_ForestVao.Unbind();

	m_program.Unuse();


	for (int i = 0; i < GROUND_N; ++i) {
		for (int j = 0; j < GROUND_N; ++j)
		{
			if (m_trees[i][j] > 0.f)
			{
				float u = i / (float)GROUND_N;
				float v = j / (float)GROUND_N;

				RenderTree(GetGroundPos(u, v), m_trees[i][j]);
			}
		}
	}

	cutTrees();

	// skybox
	// mentsük el az előző Z-test eredményt, azaz azt a relációt, ami alapján update-eljük a pixelt.
	GLint prevDepthFnc;
	glGetIntegerv(GL_DEPTH_FUNC, &prevDepthFnc);

	// most kisebb-egyenlőt használjunk, mert mindent kitolunk a távoli vágósíkokra
	glDepthFunc(GL_LEQUAL);

	m_SkyboxVao.Bind();
	m_programSkybox.Use();
	m_programSkybox.SetUniform("MVP", viewProj * glm::translate( m_camera.GetEye()) );
	
	// cube map textúra beállítása 0-ás mintavételezőre és annak a shaderre beállítása
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_skyboxTexture);
	glUniform1i(m_programSkybox.GetLocation("skyboxTexture"), 0);
	// az előző három sor <=> m_programSkybox.SetCubeTexture("skyboxTexture", 0, m_skyboxTexture);

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr);
	m_programSkybox.Unuse();

	// végül állítsuk vissza
	glDepthFunc(prevDepthFnc);

	//ImGui Testwindow
	ImGui::ShowTestWindow();
	if (ImGui::Begin("Interactive GUI"))
	{
		ImGui::Text("Cut down trees: %d", destroyedTrees);
		ImGui::Text("Use the slider to change the light direction");
		ImGui::SliderFloat3("Light Direction", &light_dir_vec.x, -10.f, 10.f);
		ImGui::Text("Use the slider to change the summer and winter shades of the light");
		ImGui::SliderFloat3("Light Shades", &diffuse_current.x, 0.3f, 0.7f);
		ImGui::Text("Set how tall trees can be cut down");
		ImGui::SliderFloat("Three Height", &treeHightToCut, 0.5f, 1.2f);
		ImGui::Text("Set how slow the trees grow");
		ImGui::SliderInt("The higher the value the slower the trees gorw", &treeGrowTime, 10, 30);
		ImGui::Text("Set how long a year lasts");
		ImGui::SliderInt("Year Length", &yearLength, 4, 32);
		ImGui::ListBoxHeader("Select a row where trees cannot gorw", 4);
		for (size_t i = 0; i < rows.size(); ++i)
		{
			if (ImGui::Selectable(rows[i].c_str(), (i == noGrowRow)))
				noGrowRow = i;
		}
		ImGui::ListBoxFooter();
		if (ImGui::Button("Cut down all trees"))
		{
			for (int i = 0; i < GROUND_N; ++i)
				for (int j = 0; j < GROUND_N; ++j)
					if (m_trees[i][j] > 0.f) {
						m_trees[i][j] = 0.f;
						destroyedTrees++;
					}
		}
	}
	ImGui::End();
}

void CMyApp::KeyboardDown(SDL_KeyboardEvent& key)
{
	m_camera.KeyboardDown(key);
}

void CMyApp::KeyboardUp(SDL_KeyboardEvent& key)
{
	m_camera.KeyboardUp(key);
}

void CMyApp::MouseMove(SDL_MouseMotionEvent& mouse)
{
	m_camera.MouseMove(mouse);
}

void CMyApp::MouseDown(SDL_MouseButtonEvent& mouse)
{
}

void CMyApp::MouseUp(SDL_MouseButtonEvent& mouse)
{
}

void CMyApp::MouseWheel(SDL_MouseWheelEvent& wheel)
{
}

// a két paraméterbe az új ablakméret szélessége (_w) és magassága (_h) található
void CMyApp::Resize(int _w, int _h)
{
	glViewport(0, 0, _w, _h );

	m_camera.Resize(_w, _h);
}
