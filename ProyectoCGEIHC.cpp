/*
Elaborado por:
Tapia Ledesma Angel Hazel 
Ramirez Arroyo Carlos Alberto 
Fecha entrega: 08/11/2024
*/

#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>
#include <math.h>

#include <glew.h>
#include <glfw3.h>

#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>
#include<assimp/Importer.hpp>

#include "Window.h"
#include "Mesh.h"
#include "Shader_light.h"
#include "Camera.h"
#include "Texture.h"
#include "Sphere.h"
#include"Model.h"
#include "Skybox.h"

//para iluminación
#include "CommonValues.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Material.h"
const float toRadians = 3.14159265f / 180.0f;

//variables para animación
/*
bool avanza;
float angulovaria = 0.0f;

//variables para keyframes
float reproduciranimacion, habilitaranimacion, guardoFrame, reinicioFrame, ciclo, ciclo2, contador = 0;
*/

Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

// Variables para el caballero 
glm::vec3 posCaballero = glm::vec3(-40.0f, 0.0f, -2.0f); 
float rotacionCaballero = 0.0f;                         
float velocidadCaballero = 0.5f;

// Movimiento de caminata del caballero
bool estaCaminando = false;
float anguloExtremidad = 0.0f;
bool animHaciaAdelante = true; 
float velocidadAnimManual = 2.0f; 
float amplitudAnimCaminata = 40.0f;

// Variables de velocidad del sol para la skybox 
float sunSpeed = 0.1f;
float sunAngle;

//Variables para la iluminacion
float dayFactor;
float ambientBase = 0.2f; // Valor base porque si no se oscurece mucho
float diffuseBase = 0.2f;

float ambientVariable = 0.3f; // Maximo de luz que se le agrega durante el dia
float diffuseVariable = 0.3f;

float newAmbient; // Para almacenar los nuevos valores de la luz
float newDiffuse;
bool gPointLightsOn = true;

// Camara
Camera camera;

// Variables para el cambio de cámara
int camaraModo = 1;             

// Variables para la cámara en Tercera Persona
float camaraYaw = 0.0f;        
float camaraPitch = 15.0f;     
float camaraDistancia = 40.0f; 
float mouseSensitivity = 0.4f; 

// Variables para la Cámara Aérea 
glm::vec3 posCamaraAerea = glm::vec3(0.0f, 200.0f, 0.0f); // Posición 
float velocidadCamaraAerea = 1.0f;


// Variables para la cámara de zonas imporantes 
int tourCamIndex = 0; // Índice del modelo actual que queremos cver 
const int TOTAL_TOUR_STOPS = 6;
float cicloB = 0;

// Coordenadas donde apunta la cam 
glm::vec3 tourCamTargets[TOTAL_TOUR_STOPS] = {
	glm::vec3(-87.0f, 10.0f, -121.0f), // 1. Centro Pokemon
	glm::vec3(-100.0f, 10.0f, 117.0f), // 2. Columpio
	glm::vec3(123.0f, 10.0f, -121.0f), // 3. Ring
	glm::vec3(158.0f, 10.0f, 80.0f),   // 4. Tianguis
	glm::vec3(0.0f, 10.0f, 138.0f),    // 5. Piramide
	glm::vec3(0.0f, 10.0f, 0.0f)       // 6. Kiosko
}; 
// si quieres agregar otro punto de interes ponlo aqui charly

// Coordenadas desde conde mira la camara
glm::vec3 tourCamPositions[TOTAL_TOUR_STOPS] = {
	glm::vec3(-87.0f, 80.0f, 70.0f),  // Centro pokemon
	glm::vec3(-250.0f, 50.0f, 150.0f), // columpio
	glm::vec3(20.0f, 100.0f, -20.0f),  // Ring
	glm::vec3(30.0f, 30.0f, 160.0f),   // Tianguis
	glm::vec3(0.0f, 80.0f, 10.0f),    // Piramide
	glm::vec3(-100.0f, 50.0f, 0.0f)     // Kiosko
};

// Variables para animaciones 
bool animColumpioActiva = false;
bool animPekkaActiva = false;
bool animCharizardActiva = false;
bool animPickachuActiva = false;


float cicloE = 0;
float cicloR = 0;
float cicloT = 0;
float cicloY = 0;

float anguloColumpio = 0.0f;
bool animColumpioAdelante = true;

float anguloPekka = 0.0f;
bool animPekkaAdelante = true;

float anguloCharizard = 0.0f;
bool animCharizardAdelante = true;


bool animPickachuAdelante = false;
float anguloPickachu = 0.0f;


// Texturas
Texture plainTexture;
Texture pisoTexture;
Texture concretoTexture;

Model Camino_M;

// Elementos generales
Model PosteLampara_M;
Model LamparaModerna_M;
Model Kiosko_M;
Model KioskoRing_M;
Model Banca_M;

// Macetero
Model Maceta_M;
Model Arbusto_M;
Model Valla_M;

// Arboles 
Model Arboles_M;

// Zona mercado
Model CentroPokemon_M;
Model CabezaOlmeca_M;
Model BancasMercado_M;
Model Mesa_M;
Model Bancos_M;

// Zona ring 
Model Ring_M;
Model RingEsquinero_M;
Model Gradas_M;
Model EstatuaDuende_M;

// Zona parque
Model Columpio_M;
Model ColumpioAsientos_M;
Model Resbaladilla_M;
Model Chacmool_M;

// Piramide
Model Piramide_M;

// Zona tianguis
Model Tianguis_M;

// Pokemons
Model Charizard_M;
Model CharizardAlaDer_M;
Model CharizardAlaIzq_M;
Model Pickachu_M;
Model PickachuCola_M;

// Clash royale	
Model Caballero_M;
Model CaballeroBrazoDer_M;
Model CaballeroBrazoIzq_M;
Model CaballeroPiernaDer_M;
Model CaballeroPiernaIzq_M;

Model Pekka_M;
Model PekkaBrazo_M;

//Model Plano_M;
Skybox skybox_day;
Skybox skybox_night;

//materiales
Material Material_brillante;
Material Material_opaco;

//Sphere cabeza = Sphere(0.5, 20, 20);
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;

// luz direccional
DirectionalLight mainLight;

//para declarar varias luces de tipo pointlight
PointLight pointLights[MAX_POINT_LIGHTS];
SpotLight spotLights[MAX_SPOT_LIGHTS];

// Vertex Shader
static const char* vShader = "shaders/shader_light.vert";

// Fragment Shader
static const char* fShader = "shaders/shader_light.frag";

//función para teclado de keyframes 
void inputKeyframes(bool* keys);

//cálculo del promedio de las normales para sombreado de Phong
void calcAverageNormals(unsigned int* indices, unsigned int indiceCount, GLfloat* vertices, unsigned int verticeCount, unsigned int vLength, unsigned int normalOffset)
{
	for (size_t i = 0; i < indiceCount; i += 3)
	{
		unsigned int in0 = indices[i] * vLength;
		unsigned int in1 = indices[i + 1] * vLength;
		unsigned int in2 = indices[i + 2] * vLength;
		glm::vec3 v1(vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1], vertices[in1 + 2] - vertices[in0 + 2]);
		glm::vec3 v2(vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 2]);
		glm::vec3 normal = glm::cross(v1, v2);
		normal = glm::normalize(normal);

		in0 += normalOffset; in1 += normalOffset; in2 += normalOffset;
		vertices[in0] += normal.x; vertices[in0 + 1] += normal.y; vertices[in0 + 2] += normal.z;
		vertices[in1] += normal.x; vertices[in1 + 1] += normal.y; vertices[in1 + 2] += normal.z;
		vertices[in2] += normal.x; vertices[in2 + 1] += normal.y; vertices[in2 + 2] += normal.z;
	}

	for (size_t i = 0; i < verticeCount / vLength; i++)
	{
		unsigned int nOffset = i * vLength + normalOffset;
		glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
		vec = glm::normalize(vec);
		vertices[nOffset] = vec.x; vertices[nOffset + 1] = vec.y; vertices[nOffset + 2] = vec.z;
	}
}

void CreateObjects()
{
	unsigned int indices[] = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};

	GLfloat vertices[] = {
		//	x      y      z			u	  v			nx	  ny    nz
			-1.0f, -1.0f, -0.6f,	0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, -1.0f, 1.0f,		0.5f, 0.0f,		0.0f, 0.0f, 0.0f,
			1.0f, -1.0f, -0.6f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,		0.5f, 1.0f,		0.0f, 0.0f, 0.0f
	};

	unsigned int floorIndices[] = {
		0, 2, 1,
		1, 2, 3
	};

	GLfloat floorVertices[] = {
		-10.0f, 0.0f, -10.0f,	0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, -10.0f,	10.0f, 0.0f,	0.0f, -1.0f, 0.0f,
		-10.0f, 0.0f, 10.0f,	0.0f, 10.0f,	0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, 10.0f,		10.0f, 10.0f,	0.0f, -1.0f, 0.0f
	};
	unsigned int vegetacionIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	   4,5,6,
	   4,6,7
	};

	GLfloat vegetacionVertices[] = {
		-0.5f, -0.5f, 0.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.0f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.0f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.0f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,

		0.0f, -0.5f, -0.5f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, -0.5f, 0.5f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.5f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, -0.5f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,


	};
	

	unsigned int flechaIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	};

	GLfloat flechaVertices[] = {
		-0.5f, 0.0f, 0.5f,		0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, 0.5f,		1.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, -0.5f,		1.0f, 1.0f,		0.0f, -1.0f, 0.0f,
		-0.5f, 0.0f, -0.5f,		0.0f, 1.0f,		0.0f, -1.0f, 0.0f,

	};

	unsigned int scoreIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	};

	GLfloat scoreVertices[] = {
		-0.5f, 0.0f, 0.5f,		0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, 0.5f,		1.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, -0.5f,		1.0f, 1.0f,		0.0f, -1.0f, 0.0f,
		-0.5f, 0.0f, -0.5f,		0.0f, 1.0f,		0.0f, -1.0f, 0.0f,

	};

	unsigned int numeroIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	};

	GLfloat numeroVertices[] = {
		-0.5f, 0.0f, 0.5f,		0.0f, 0.67f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, 0.5f,		0.25f, 0.67f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, -0.5f,		0.25f, 1.0f,		0.0f, -1.0f, 0.0f,
		-0.5f, 0.0f, -0.5f,		0.0f, 1.0f,		0.0f, -1.0f, 0.0f,

	};

	Mesh *obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj1);

	Mesh *obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj2);

	Mesh *obj3 = new Mesh();
	obj3->CreateMesh(floorVertices, floorIndices, 32, 6);
	meshList.push_back(obj3);

	Mesh* obj4 = new Mesh();
	obj4->CreateMesh(vegetacionVertices, vegetacionIndices, 64, 12);
	meshList.push_back(obj4);

	Mesh* obj5 = new Mesh();
	obj5->CreateMesh(flechaVertices, flechaIndices, 32, 6);
	meshList.push_back(obj5);

	Mesh* obj6 = new Mesh();
	obj6->CreateMesh(scoreVertices, scoreIndices, 32, 6);
	meshList.push_back(obj6);

	Mesh* obj7 = new Mesh();
	obj7->CreateMesh(numeroVertices, numeroIndices, 32, 6);
	meshList.push_back(obj7);

}

void CreateShaders()
{
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}

int main()
{
	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();

	CreateObjects();
	CreateShaders();

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.5f, 0.5f);

	plainTexture = Texture("Textures/plain.png");
	plainTexture.LoadTextureA();
	pisoTexture = Texture("Textures/pastoTexture.jpg");
	pisoTexture.LoadTextureA();
	concretoTexture = Texture("Textures/concretoTexture.jpg");
	concretoTexture.LoadTextureA();


	// Elementos generales 
	PosteLampara_M = Model();
	PosteLampara_M.LoadModel("Models/PosteLampara.obj");

	LamparaModerna_M = Model(); 
	LamparaModerna_M.LoadModel("Models/lamparaModerna.obj");

	Kiosko_M = Model();
	Kiosko_M.LoadModel("Models/Kiosko.obj");

	KioskoRing_M = Model();
	KioskoRing_M.LoadModel("Models/KioskoRing.obj");

	Banca_M = Model();
	Banca_M.LoadModel("Models/Banca.obj");
	// Macetero
	Maceta_M = Model();
	Maceta_M.LoadModel("Models/Maceta.obj");
	Arbusto_M = Model();
	Arbusto_M.LoadModel("Models/Arbusto.obj");
	Valla_M = Model();
	Valla_M.LoadModel("Models/Valla.obj");

	// Modelos 
	Camino_M = Model();
	Camino_M.LoadModel("Models/Camino.obj");

	LamparaModerna_M = Model();
	LamparaModerna_M.LoadModel("Models/lamparaModerna.obj");

	
	// Clash Royale
	Caballero_M = Model();
	Caballero_M.LoadModel("Models/caballero.obj");
	CaballeroBrazoDer_M = Model();
	CaballeroBrazoDer_M.LoadModel("Models/caballeroBrazoDer.obj");
	CaballeroBrazoIzq_M = Model();
	CaballeroBrazoIzq_M.LoadModel("Models/caballeroBrazoIzq.obj");
	CaballeroPiernaDer_M = Model();
	CaballeroPiernaDer_M.LoadModel("Models/caballeroPiernaDer.obj");
	CaballeroPiernaIzq_M = Model();
	CaballeroPiernaIzq_M.LoadModel("Models/caballeroPiernaIzq.obj");
	 
	Pekka_M = Model();
	Pekka_M.LoadModel("Models/pekka.obj");

	PekkaBrazo_M = Model();
	PekkaBrazo_M.LoadModel("Models/pekkaBrazo.obj");

	
	//Model Plano_M = Model();
	//Plano_M.LoadModel("Models/Plano.obj");
	
	// Pokemons
	Charizard_M = Model();
	Charizard_M.LoadModel("Models/Charizard.obj");
	CharizardAlaDer_M = Model();
	CharizardAlaDer_M.LoadModel("Models/CharizardAlaDer.obj");
	CharizardAlaIzq_M = Model();
	CharizardAlaIzq_M.LoadModel("Models/CharizardAlaIzq.obj");

	Pickachu_M = Model();
	Pickachu_M.LoadModel("Models/Pickachu.obj");
	PickachuCola_M = Model();
	PickachuCola_M.LoadModel("Models/PickachuCola.obj");

	// Zona mercado
	CentroPokemon_M = Model();
	CentroPokemon_M.LoadModel("Models/CentroPokemon.obj");

	CabezaOlmeca_M = Model();
	CabezaOlmeca_M.LoadModel("Models/CabezaOlmeca.obj");

	BancasMercado_M = Model();
	BancasMercado_M.LoadModel("Models/BancasMercado.obj");

	Mesa_M = Model();
	Mesa_M.LoadModel("Models/Mesa.obj");

	Bancos_M = Model();
	Bancos_M.LoadModel("Models/Bancos.obj");

	// Zona ring
	Ring_M = Model();
	Ring_M.LoadModel("Models/Ring.obj");

	RingEsquinero_M = Model();
	RingEsquinero_M.LoadModel("Models/RingEsquineros.obj");

	Gradas_M = Model();
	Gradas_M.LoadModel("Models/Gradas.obj");

	EstatuaDuende_M = Model();
	EstatuaDuende_M.LoadModel("Models/EstatuaDuende.obj");
	
	// Zona parque
	Columpio_M = Model();
	Columpio_M.LoadModel("Models/Columpio.obj");
	
	ColumpioAsientos_M = Model();
	ColumpioAsientos_M.LoadModel("Models/ColumpioAsientos.obj");
	
	Resbaladilla_M = Model();
	Resbaladilla_M.LoadModel("Models/Resbaladilla.obj");
	
	Chacmool_M = Model();
	Chacmool_M.LoadModel("Models/Chacmool.obj");

	// Arboles
	Arboles_M = Model();
	Arboles_M.LoadModel("Models/Arboles.obj");

	// Piramide
	Piramide_M = Model();
	Piramide_M.LoadModel("Models/Piramide.obj");

	// Zona tianguis
	Tianguis_M = Model();
	Tianguis_M.LoadModel("Models/Tianguis.obj");


	//skybox
	Skybox skyboxDia;
	Skybox skyboxNoche;

	// Skybox de noche
	std::vector<std::string> skyboxFacesNoche;
	skyboxFacesNoche.push_back("Textures/Skybox/skybox_night_lf.png");
	skyboxFacesNoche.push_back("Textures/Skybox/skybox_night_rt.png");
	skyboxFacesNoche.push_back("Textures/Skybox/skybox_night_dn.png");
	skyboxFacesNoche.push_back("Textures/Skybox/skybox_night_up.png");
	skyboxFacesNoche.push_back("Textures/Skybox/skybox_night_bk.png");
	skyboxFacesNoche.push_back("Textures/Skybox/skybox_night_ft.png");
	skyboxNoche = Skybox(skyboxFacesNoche);

	// Skybox de dia
	std::vector<std::string> skyboxFacesDia;
	skyboxFacesDia.push_back("Textures/Skybox/skybox_day_lf.png");
	skyboxFacesDia.push_back("Textures/Skybox/skybox_day_rt.png"); 
	skyboxFacesDia.push_back("Textures/Skybox/skybox_day_dn.png"); 
	skyboxFacesDia.push_back("Textures/Skybox/skybox_day_up.png"); 
	skyboxFacesDia.push_back("Textures/Skybox/skybox_day_bk.png"); 
	skyboxFacesDia.push_back("Textures/Skybox/skybox_day_ft.png"); 
	skyboxDia = Skybox(skyboxFacesDia);

	Material_brillante = Material(4.0f, 256);
	Material_opaco = Material(0.3f, 4);

	// Iluminación

	//contador de luces puntuales
	unsigned int pointLightCount = 0;

	std::vector<glm::vec3> posFrias = {
	{  -76.3f,  34.0f,  -26.0f },
	{  -148.6f,  34.0f,  -26.0f },
	{  -76.3f,  34.0f,  26.0f },
	{  -148.6f,  34.0f,  26.0f }

	};
	std::vector<glm::vec3> posCalidas = {
		{76.3f, 24.5f, -26.0f},
		{148.6f, 24.5f, -26.0f},
		{76.3f, 24.5f, 26.0f},
		{148.6f, 24.5f, 26.0f},
		{76.3f, 24.5f, -85.1f},
		{76.3f, 24.5f, -157.4f},
		{148.6f, 24.5f, -85.1f},
		{148.6f, 24.5f, -157.4f},
		{-28.5f, 24.5f, 76.0f},
		{13.3f, 24.5f, 76.0f}

	};

	//luz del sol
	mainLight = DirectionalLight(1.0f, 1.0f, 1.0f,
		0.4f, 0.4f,
		0.0f, 0.0f, -1.0f);

	//Declaración de primer luz puntual
	pointLights[0] = PointLight(1.0f, 0.0f, 0.0f,
		0.0f, 1.0f,
		0.0f, 2.5f, 1.5f,
		0.3f, 0.2f, 0.001f);
	pointLightCount++;

	unsigned int spotLightCount = 0;
	//linterna
	spotLights[0] = SpotLight(1.0f, 1.0f, 1.0f,
		0.0f, 2.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		5.0f);
	spotLightCount++;

	//luz fija
	spotLights[1] = SpotLight(0.0f, 0.0f, 1.0f,
		1.0f, 2.0f,
		5.0f, 10.0f, 0.0f,
		0.0f, -5.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		15.0f);
	spotLightCount++;

	// Luz fria 
	for (auto p : posFrias) {
		if (pointLightCount >= MAX_POINT_LIGHTS) break;
		pointLights[pointLightCount++] = PointLight(
			0.80f, 0.90f, 1.00f,
			0.02f, 1.0f,
			p.x, p.y, p.z,
			1.0f, 0.09f, 0.032f
		);
	}

	// Luz calida 
	for (auto p : posCalidas) {
		if (pointLightCount >= MAX_POINT_LIGHTS) break;
		pointLights[pointLightCount++] = PointLight(
			1.00f, 0.95f, 0.85f,
			0.03f, 1.0f,
			p.x, p.y, p.z,
			1.0f, 0.09f, 0.032f
		);
	}


	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0, uniformSpecularIntensity = 0, uniformShininess = 0, uniformTextureOffset=0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);

	glm::mat4 model(1.0);
	glm::mat4 modelaux(1.0);
	glm::mat4 modelaux2(1.0);
	glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec2 toffset = glm::vec2(0.0f, 0.0f);
	glm::vec3 lowerLight = glm::vec3(0.0f,0.0f,0.0f);

	// Impresion para el usuario
	printf("\n========== CONTROLES DEL PROYECTO ==========\n");
	printf("--- Movimiento ---\n");
	printf("[W, A, S, D] : Mover al Caballero\n");
	printf("\n--- Camaras ---\n");
	printf("[C] : Camara 3ra Persona\n");
	printf("[V] : Camara Aerea\n");
	printf("[I, J, K, L]: Mover camara aerea con [I, J, K, L]\n");
	printf("[B] : Camara Tour (Fijo) - Presiona [B] para cambiar de escena\n");
	printf("\n--- Animaciones (Activar/Desactivar) ---\n");
	printf("[E] : Animar Columpio\n");
	printf("[R] : Animar Brazo de Pekka\n");
	printf("[T] : Animar Alas de Charizard\n");
	printf("[Y] : Animar Cola de Pikachu\n");
	printf("\n--- Iluminacion ---\n");
	printf("[1/2] : Encender / Apagar\n");
	printf("===========================================\n");

	////Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;

		//Recibir eventos del usuario
		glfwPollEvents();
		bool* keys = mainWindow.getsKeys(); // Obtenemos el array de teclas
		
		// Para las luces
		if (keys[GLFW_KEY_1]) gPointLightsOn = true;   // prender
		if (keys[GLFW_KEY_2]) gPointLightsOn = false;  // apagar
		// Se puede usar tambien el delta time 
		sunAngle = glfwGetTime() * sunSpeed;

		// Rotacion del sol 
		glm::vec3 sunDirection;
		sunDirection.x = 0.0f;
		sunDirection.y = -sin(sunAngle); // Mov en y
		sunDirection.z = -cos(sunAngle); // Mov en z

		mainLight.SetDirection(sunDirection);

		// Es un valor entre 0 y 1 que indica si es de dia o noche
		float dayFactor = glm::max(0.0f, -sunDirection.y);
		newAmbient = ambientBase + (ambientVariable * dayFactor);
		newDiffuse = diffuseBase + (diffuseVariable * dayFactor);

		// Esta linea actualiza la luz direccional es la que le agregamos al objeto 
		mainLight.SetIntensity(newAmbient, newDiffuse);
		
		// Movimeinto del caballero 
		if (keys[GLFW_KEY_W] || keys[GLFW_KEY_S] || keys[GLFW_KEY_A] || keys[GLFW_KEY_D])
		{
			estaCaminando = true;
		}
		else
		{
			estaCaminando = false;
		}

		// Caminata con el angulo de las piernas y brazos 
		if (estaCaminando)
		{
			if (animHaciaAdelante)
			{
				anguloExtremidad += velocidadAnimManual * deltaTime;
				if (anguloExtremidad >= amplitudAnimCaminata)
				{
					anguloExtremidad = amplitudAnimCaminata; 
					animHaciaAdelante = false;
				}
			}
			else
			{
				anguloExtremidad -= velocidadAnimManual * deltaTime;
				if (anguloExtremidad <= -amplitudAnimCaminata)
				{
					anguloExtremidad = -amplitudAnimCaminata; 
					animHaciaAdelante = true; 
				}
			}
		}
		else
		{
			if (anguloExtremidad > 0.1f)
			{
				anguloExtremidad -= velocidadAnimManual * deltaTime;
			}
			else if (anguloExtremidad < -0.1f)
			{
				anguloExtremidad += velocidadAnimManual * deltaTime;
			}
			else
			{
				anguloExtremidad = 0.0f;
			}
		}

		// Animaciones basicas de personajes y el columpio
		if (animColumpioActiva)
		{
			if (animColumpioAdelante) {
				anguloColumpio += (velocidadAnimManual * 0.5f) * deltaTime; 
				if (anguloColumpio >= 30.0f) { 
					anguloColumpio = 30.0f;
					animColumpioAdelante = false;
				}
			}
			else {
				anguloColumpio -= (velocidadAnimManual * 0.5f) * deltaTime;
				if (anguloColumpio <= -30.0f) {
					anguloColumpio = -30.0f;
					animColumpioAdelante = true;
				}
			}
		}
		else {
			if (anguloColumpio > 0.1f) { anguloColumpio -= (velocidadAnimManual * 0.5f) * deltaTime; }
			else if (anguloColumpio < -0.1f) { anguloColumpio += (velocidadAnimManual * 0.5f) * deltaTime; }
			else { anguloColumpio = 0.0f; }
		}

		if (animPekkaActiva)
		{
			if (animPekkaAdelante) {
				anguloPekka += (velocidadAnimManual * 0.8f) * deltaTime;
				if (anguloPekka >= 25.0f) {
					anguloPekka = 25.0f;
					animPekkaAdelante = false;
				}
			}
			else {
				anguloPekka -= (velocidadAnimManual * 0.8f) * deltaTime;
				if (anguloPekka <= -25.0f) {
					anguloPekka = -25.0f;
					animPekkaAdelante = true;
				}
			}
		}
		else {
			if (anguloPekka > 0.1f) { anguloPekka -= (velocidadAnimManual * 0.8f) * deltaTime; }
			else if (anguloPekka < -0.1f) { anguloPekka += (velocidadAnimManual * 0.8f) * deltaTime; }
			else { anguloPekka = 0.0f; }
		}

		if (animCharizardActiva)
		{
			if (animCharizardAdelante) {
				anguloCharizard += (velocidadAnimManual * 1.0f) * deltaTime; 
				if (anguloCharizard >= 20.0f) { 
					anguloCharizard = 20.0f;
					animCharizardAdelante = false;
				}
			}
			else {
				anguloCharizard -= (velocidadAnimManual * 1.0f) * deltaTime;
				if (anguloCharizard <= 0.0f) { 
					anguloCharizard = 0.0f;
					animCharizardAdelante = true;
				}
			}
		}
		else {
			if (anguloCharizard > 0.1f) { anguloCharizard -= (velocidadAnimManual * 2.0f) * deltaTime; }
			else { anguloCharizard = 0.0f; }
		}
		if (animPickachuActiva)
		{
			if (animPickachuAdelante) {
				anguloPickachu += (velocidadAnimManual * 0.8f) * deltaTime;
				if (anguloPickachu >= 20.0f) {
					anguloPickachu = 20.0f;
					animPickachuAdelante = false;
				}
			}
			else {
				anguloPickachu -= (velocidadAnimManual * 0.8f) * deltaTime;
				if (anguloPickachu <= -20.0f) { 
					anguloPickachu = -20.0f;
					animPickachuAdelante = true;
				}
			}
		}
		else {
			if (anguloPickachu > 0.1f) { anguloPickachu -= (velocidadAnimManual * 1.5f) * deltaTime; }
			else if (anguloPickachu < -0.1f) { anguloPickachu += (velocidadAnimManual * 1.5f) * deltaTime; }
			else { anguloPickachu = 0.0f; }
		}


		// Teclas Para el movimiento del personaje en 3ra persona

		if (keys[GLFW_KEY_W])
		{
			posCaballero.x += velocidadCaballero * deltaTime;
			rotacionCaballero = 0.0f;
		}
		if (keys[GLFW_KEY_S])
		{
			posCaballero.x -= velocidadCaballero * deltaTime;
			rotacionCaballero = 180.0f;
		}
		if (keys[GLFW_KEY_A])
		{
			posCaballero.z -= velocidadCaballero * deltaTime;
			rotacionCaballero = 90.0f;
		}
		if (keys[GLFW_KEY_D])
		{
			posCaballero.z += velocidadCaballero * deltaTime;
			rotacionCaballero = 270.0f;
		}

		// Cambio de camaras 
		if (keys[GLFW_KEY_C])
		{
			camaraModo = 1;
			printf("Modo Camara: 1 (3ra Persona Orbital)\n");
		}

		if (keys[GLFW_KEY_V])
		{
			camaraModo = 2;
			printf("Modo Camara: 2 (Aerea Libre)\n");
		}

		if (keys[GLFW_KEY_B])
		{
			if (cicloB < 1)
			{
				if (camaraModo != 3)
				{
					camaraModo = 3;
					tourCamIndex = 0;
					printf("Modo Camara: 3 (Tour Fijo) - Parada 1: Centro Pokemon\n");
				}
				else
				{
					tourCamIndex = (tourCamIndex + 1) % TOTAL_TOUR_STOPS;
					printf("  > Siguiente parada del Tour (%d / %d)\n", tourCamIndex + 1, TOTAL_TOUR_STOPS);
				}
				cicloB++;
			}
		}
		if (!keys[GLFW_KEY_B]) { cicloB = 0; }
		// Para animacion
		// Columpio
		if (keys[GLFW_KEY_E])
		{
			if (cicloE < 1)
			{
				animColumpioActiva = !animColumpioActiva; // Invierte el estado
				printf("Animacion Columpio: %s\n", animColumpioActiva ? "ON" : "OFF");
				cicloE++;
			}
		}
		if (!keys[GLFW_KEY_E]) { cicloE = 0; }

		// Pekka
		if (keys[GLFW_KEY_R])
		{
			if (cicloR < 1)
			{
				animPekkaActiva = !animPekkaActiva; // Invierte el estado
				printf("Animacion Pekka: %s\n", animPekkaActiva ? "ON" : "OFF");
				cicloR++;
			}
		}
		if (!keys[GLFW_KEY_R]) { cicloR = 0; }

		// Alas Charizard
		if (keys[GLFW_KEY_T])
		{
			if (cicloT < 1)
			{
				animCharizardActiva = !animCharizardActiva; // Invierte el estado
				printf("Animacion Charizard: %s\n", animCharizardActiva ? "ON" : "OFF");
				cicloT++;
			}
		}
		if (!keys[GLFW_KEY_T]) { cicloT = 0; }
		if (keys[GLFW_KEY_Y])
		{
			if (cicloY < 1) 
			{
				animPickachuActiva = !animPickachuActiva; 
				printf("Animacion Pikachu: %s\n", animPickachuActiva ? "ON" : "OFF");
				cicloY++;
			}
		}
		if (!keys[GLFW_KEY_Y]) { cicloY = 0; }

		GLfloat xChange = mainWindow.getXChange();
		GLfloat yChange = mainWindow.getYChange();
		glm::mat4 viewMatrix;
		glm::vec3 cameraPos;
		glm::vec3 target;
	
		// Switch de modos de camaras 
		switch (camaraModo)
		{
		// 3ra persona
		case 1:
		{
			camaraYaw += xChange * mouseSensitivity;
			target = posCaballero;
			float horizontalDist = camaraDistancia * cos(camaraPitch * toRadians);
			float verticalDist = camaraDistancia * sin(camaraPitch * toRadians);
			cameraPos.x = target.x - horizontalDist * sin(camaraYaw * toRadians);
			cameraPos.y = target.y + verticalDist;
			cameraPos.z = target.z - horizontalDist * cos(camaraYaw * toRadians);
			viewMatrix = glm::lookAt(cameraPos, target, glm::vec3(0.0f, 1.0f, 0.0f));
			break;
		}

		// Aerea
		case 2:
		{
			if (keys[GLFW_KEY_I]) posCamaraAerea.z -= velocidadCamaraAerea * deltaTime;
			if (keys[GLFW_KEY_K]) posCamaraAerea.z += velocidadCamaraAerea * deltaTime;
			if (keys[GLFW_KEY_J]) posCamaraAerea.x -= velocidadCamaraAerea * deltaTime;
			if (keys[GLFW_KEY_L]) posCamaraAerea.x += velocidadCamaraAerea * deltaTime;
			cameraPos = posCamaraAerea;
			target = posCamaraAerea + glm::vec3(0.0f, -1.0f, 0.0f);
			viewMatrix = glm::lookAt(cameraPos, target, glm::vec3(0.0f, 0.0f, -1.0f));
			break;
		}
		// Fija con vistas a cosas importantes
		case 3:
		{
			cameraPos = tourCamPositions[tourCamIndex];
			target = tourCamTargets[tourCamIndex];
			viewMatrix = glm::lookAt(cameraPos, target, glm::vec3(0.0f, 1.0f, 0.0f));
			break;
		}

		}

		// Limpia la ventana
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Se cargan las texturas dependiendo del dia o noche 
		if (sunDirection.y < 0.0f)
		{
			skyboxDia.DrawSkybox(viewMatrix, projection);
		}
		else
		{
			skyboxNoche.DrawSkybox(viewMatrix, projection);
		}

		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();
		uniformEyePosition = shaderList[0].GetEyePositionLocation();
		uniformColor = shaderList[0].getColorLocation();
		uniformTextureOffset = shaderList[0].getOffsetLocation();

		//información en el shader de intensidad especular y brillo
		uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
		uniformShininess = shaderList[0].GetShininessLocation();
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(viewMatrix));
		glUniform3f(uniformEyePosition, cameraPos.x, cameraPos.y, cameraPos.z);

		//información al shader de fuentes de iluminación
		shaderList[0].SetDirectionalLight(&mainLight);
		shaderList[0].SetPointLights(pointLights, gPointLightsOn ? pointLightCount : 0);
		shaderList[0].SetSpotLights(spotLights, spotLightCount);

		/*
		Para Keyframes
		inputKeyframes(mainWindow.getsKeys());
		animate();
		*/


		// MODELOS 
		model = glm::mat4(1.0);
		modelaux = glm::mat4(1.0);
		modelaux2 = glm::mat4(1.0);
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		toffset = glm::vec2(0.0f, 0.0f);
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));

		// Piso
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -1.0f, -8.0f));
		model = glm::scale(model, glm::vec3(22.0f, 1.0f, 22.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		pisoTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[2]->RenderMesh();
		
		// Pasillo camino
		// Camino
		concretoTexture.UseTexture();
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		Camino_M.RenderModel();
		
		// Lamparas camino
		// Lamparas lado izquierdo
		// Lamparas modernas
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-148.0f, 0.0f, -26.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		LamparaModerna_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-76.0f, 0.0f, -26.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		LamparaModerna_M.RenderModel();
		// Postes luz 
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(76.0f, 0.0f, -26.0f));
		model = glm::rotate(model, -180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		PosteLampara_M.RenderModel();
		
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(148.0f, 0.0f, -26.0f));
		model = glm::rotate(model, -180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		PosteLampara_M.RenderModel();

		// Lamparas lado derecho
		// Lamparas modernas
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-148.0f, 0.0f, 26.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		LamparaModerna_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-76.0f, 0.0f, 26.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		LamparaModerna_M.RenderModel();
		// Postes luz 
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(76.0f, 0.0f, 26.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		PosteLampara_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(148.0f, 0.0f, 26.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		PosteLampara_M.RenderModel();
		
		// Maceteros lado izquierdo
		// Macetero 
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-136.0f, 0.0f, -17.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		Maceta_M.RenderModel();
		// Abrusto (Jerarquia de Macetero)
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, 1.65f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		Arbusto_M.RenderModel();
		// Vallas (Jerarquia de Macetero)
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		Valla_M.RenderModel();

		// Macetero 
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-88.0f, 0.0f, -17.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		Maceta_M.RenderModel();
		// Abrusto (Jerarquia de Macetero)
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, 1.65f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		Arbusto_M.RenderModel();
		// Vallas (Jerarquia de Macetero)
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		Valla_M.RenderModel();

		// Macetero 
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(136.0f, 0.0f, -17.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		Maceta_M.RenderModel();
		// Abrusto (Jerarquia de Macetero)
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, 1.65f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		Arbusto_M.RenderModel();
		// Vallas (Jerarquia de Macetero)
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		Valla_M.RenderModel();

		// Macetero 
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(88.0f, 0.0f, -17.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		Maceta_M.RenderModel();
		// Abrusto (Jerarquia de Macetero)
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, 1.65f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		Arbusto_M.RenderModel();
		// Vallas (Jerarquia de Macetero)
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		Valla_M.RenderModel();

		// Macetero lado derecho
		// Macetero 
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-136.0f, 0.0f, 17.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		Maceta_M.RenderModel();
		// Abrusto (Jerarquia de Macetero)
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, 1.65f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		Arbusto_M.RenderModel();
		// Vallas (Jerarquia de Macetero)
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		Valla_M.RenderModel();

		// Macetero 
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-88.0f, 0.0f, 17.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		Maceta_M.RenderModel();
		// Abrusto (Jerarquia de Macetero)
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, 1.65f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		Arbusto_M.RenderModel();
		// Vallas (Jerarquia de Macetero)
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		Valla_M.RenderModel();

		// Macetero 
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(136.0f, 0.0f, 17.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		Maceta_M.RenderModel();
		// Abrusto (Jerarquia de Macetero)
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, 1.65f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		Arbusto_M.RenderModel();
		// Vallas (Jerarquia de Macetero)
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		Valla_M.RenderModel();

		// Macetero 
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(88.0f, 0.0f, 17.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		Maceta_M.RenderModel();
		// Abrusto (Jerarquia de Macetero)
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, 1.65f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		Arbusto_M.RenderModel();
		// Vallas (Jerarquia de Macetero)
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		Valla_M.RenderModel();

		// Bancas lado izquierdo 
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-112.0f, 0.0f, -21.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Banca_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(112.0f, 0.0f, -21.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Banca_M.RenderModel();
		// Bancas lado derecho 
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-112.0f, 0.0f, 21.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Banca_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(112.0f, 0.0f, 21.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Banca_M.RenderModel();
		

		// Zona mercado
		// Centro Pokemon
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-87.0f, 0.0f, -121.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		CentroPokemon_M.RenderModel();

		// Cabeza Olmeca (Jerarquia de Centro Pokemon)
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, 35.0f, 83.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		CabezaOlmeca_M.RenderModel();

		// Bancas Mercado (Jerarquia de Centro Pokemon)
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		BancasMercado_M.RenderModel();

		// Mesa Centro(Jerarquia de Centro Pokemon)
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 56.0f));
		modelaux2 = model;
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		Mesa_M.RenderModel();

		// Bancos Mesa Centro (Jerarquia de Mesa Centro)
		model = modelaux2;
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		Bancos_M.RenderModel();

		// Mesa izq (Jerarquia de Centro Pokemon)
		model = modelaux;
		model = glm::translate(model, glm::vec3(-30.0f, 0.0f, 56.0f));
		modelaux2 = model;
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		Mesa_M.RenderModel();

		// Bancos Mesa izq(Jerarquia de Mesa Centro)
		model = modelaux2;
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		Bancos_M.RenderModel();
		
		// Mesa der (Jerarquia de Centro Pokemon)
		model = modelaux;
		model = glm::translate(model, glm::vec3(30.0f, 0.0f, 56.0f));
		modelaux2 = model;
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		Mesa_M.RenderModel();

		// Bancos Mesa der (Jerarquia de Mesa Centro)
		model = modelaux2;
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		Bancos_M.RenderModel();

		// Arboles 
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(10.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		Arboles_M.RenderModel();

		// Zona Ring 
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(123.0f, 0.0f, -121.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		Ring_M.RenderModel();

		// Esquineros Ring (Jerarquia de Ring)
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		RingEsquinero_M.RenderModel();
		
		// Poste Zona Ring (Arriba izq)
		model = modelaux;
		model = glm::translate(model, glm::vec3(-36.0f, 0.0f,-36.0f));
		model = glm::rotate(model, -135 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		PosteLampara_M.RenderModel();

		// Poste Zona Ring (Arriba der)
		model = modelaux;
		model = glm::translate(model, glm::vec3(36.0f, 0.0f, -36.0f));
		model = glm::rotate(model, 135 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		PosteLampara_M.RenderModel();

		// Poste Zona Ring (Abajo izq)
		model = modelaux;
		model = glm::translate(model, glm::vec3(-36.0f, 0.0f, 36.0f));
		model = glm::rotate(model, -45 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		PosteLampara_M.RenderModel();

		// Poste Zona Ring (Abajo der)
		model = modelaux;
		model = glm::translate(model, glm::vec3(36.0f, 0.0f, 36.0f));
		model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		PosteLampara_M.RenderModel();

		// Gradas (izquierda)
		model = modelaux;
		model = glm::translate(model, glm::vec3(-65.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		Gradas_M.RenderModel();

		// Gradas (Arriba)
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -65.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		Gradas_M.RenderModel();

		// Gradas (Abajo)
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 65.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		Gradas_M.RenderModel();

		// Estatua duende (Derecha)
		model = modelaux;
		model = glm::translate(model, glm::vec3(50.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		EstatuaDuende_M.RenderModel();

		// Kiosko
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		Kiosko_M.RenderModel();

		// Kiosko Ring
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		KioskoRing_M.RenderModel();

		// Zona Parque 
		// Columpio
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-100.0f, 0.0f, 117.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		Columpio_M.RenderModel();

		// Columpio Asientos (Jerarquia de Columpio)
		model = modelaux;
		model = glm::translate(model, glm::vec3(-0.02f, 23.3f, -0.11f));
		model = glm::rotate(model, anguloColumpio * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		ColumpioAsientos_M.RenderModel();

		// Cabeza Olmeca 
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-178.0f,0.0f, 160.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		CabezaOlmeca_M.RenderModel();

		// Resbaladilla
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-156.0f, 0.0f, 128.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		Resbaladilla_M.RenderModel();

		// Chacmool
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-55.0f, 0.0f, 91.0f));
		model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		Chacmool_M.RenderModel();

		// Piramide
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.0f, 0.0f, 138.0f));
		model = glm::rotate(model, -45 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		Piramide_M.RenderModel();

		// Postes luz piramide
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(10.0f, 0.0f, 84.0f));
		model = glm::rotate(model, -45 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		PosteLampara_M.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-10.0f, 0.0f, 84.0f));
		model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		PosteLampara_M.RenderModel();


		// Zona tianguis 
		// Puestos
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(158.0f, 0.0f, 80.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		Tianguis_M.RenderModel();

		// Personajes 
		// Charizard
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-76.0f, 0.0f, -94.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Charizard_M.RenderModel();

		// Charizard ala derecha (anim)
		model = modelaux;
		model = glm::translate(model, glm::vec3(-1.41f, 8.6f, -0.98f));
		model = glm::rotate(model, anguloCharizard * toRadians, glm::vec3(0.0f, -1.0f, 1.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		CharizardAlaDer_M.RenderModel();

		// Charizard ala izquierda
		model = modelaux;
		model = glm::translate(model, glm::vec3(-1.41f, 8.6f, 0.98f));
		model = glm::rotate(model,anguloCharizard * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		CharizardAlaIzq_M.RenderModel();

		// Pickachu
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-94.0f, 0.0f, -92.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		modelaux = model;	
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Pickachu_M.RenderModel();

		// Pickachu cola (jerarquia de Pickachu)
		model = modelaux;
		model = glm::translate(model, glm::vec3(1.47f, 1.97f, 0.0f));
		model = glm::rotate(model, anguloPickachu * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		PickachuCola_M.RenderModel();

		// Caballero
		model = glm::mat4(1.0);
		model = glm::translate(model, posCaballero);
		model = glm::rotate(model, 180* toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, rotacionCaballero * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Caballero_M.RenderModel();

		// caballero brazo izq (jerarquia de caballero)
		model = modelaux;
		model = glm::translate(model, glm::vec3(-0.06f, 7.10f, 1.7f));
		model = glm::rotate(model, -anguloExtremidad * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		CaballeroBrazoIzq_M.RenderModel();
		
		// Caballero brazo der (jerarquia de caballero)
		model = modelaux;
		model = glm::translate(model, glm::vec3(-0.06f, 7.10f, -1.7f));
		model = glm::rotate(model, anguloExtremidad * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		CaballeroBrazoDer_M.RenderModel();
		
		// Cabalero pierna izq (jerarquia de caballero)
		model = modelaux;
		model = glm::translate(model, glm::vec3(-0.40f, 4.23f, 0.47f));
		model = glm::rotate(model, anguloExtremidad * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		CaballeroPiernaIzq_M.RenderModel();
		
		// Caballero pierna der (jerarquia de caballero)
		model = modelaux;
		model = glm::translate(model, glm::vec3(-0.40f, 4.23f, -0.47f));
		model = glm::rotate(model, -anguloExtremidad * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		CaballeroPiernaDer_M.RenderModel();
		
		// Minipekka 
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(108.0f, 7.0f, -125.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Pekka_M.RenderModel();

		// Minipekka espada (jerarquia de Minipekka)
		model = modelaux;	
		model = glm::translate(model, glm::vec3(1.85f, 5.07f, -1.76f));
		model = glm::rotate(model, anguloPekka * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		PekkaBrazo_M.RenderModel();

		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}

