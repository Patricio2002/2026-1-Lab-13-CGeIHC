/*
Animación:
Simple o básica:Por banderas y condicionales (más de 1 transformación geométrica se ve modificada)
Compleja: Por medio de funciones y algoritmos.
Textura Animada
*/
//para cargar imagen
#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>
#include <math.h>
#include <stdlib.h>


#include <glew.h>
#include <glfw3.h>

#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>

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



//variables para animación
float movCoche;
float movOffset;
float rotllanta;
float rotllantaOffset;
bool avanza;
float toffsetflechau = 0.0f;
float toffsetflechav = 0.0f;
float toffsetnumerou = 0.0f;
float toffsetnumerov = 0.0f;
float toffsetnumerocambiau = 0.0;
float angulovaria = 0.0f;
float dragonavance = 0.0f;
float reproduciranimacion, habilitaranimacion, guardoFrame, reinicioFrame, 
ciclo, ciclo2, ciclo3, ciclo4,ciclo5,ciclo6,ciclo7,ciclo8, contador = 0;

Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

Camera camera;

Texture brickTexture;
Texture dirtTexture;
Texture plainTexture;
Texture pisoTexture;
Texture AgaveTexture;
Texture FlechaTexture;
Texture NumerosTexture;
Texture Numero1Texture;
Texture Numero2Texture;
Texture Degradado;
Texture race;
Texture bottom_Trunk;
Texture coconut;

Model Kitt_M;
Model Llanta_M;
Model Dragon_M;
Model Tiamat_M;
Skybox skybox;
Model castillo;
Model constMedio;
Model constMedioChiquito;
Model esqIzq;
Model casitaIzq;
Model ruinasIzq;
Model esqIzqFrente;
Model cajaFrente;
Model constFrente;
Model explanadaDer;
Model esqDer;
Model racecourse;
Model palmera;
Model coco;

//materiales
Material Material_brillante;
Material Material_opaco;

//archivos
FILE* archivo;
errno_t err;
char linea[100];

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


void inputKeyframes(bool* keys);

//cálculo del promedio de las normales para sombreado de Phong
void calcAverageNormals(unsigned int* indices, unsigned int indiceCount, GLfloat* vertices, unsigned int verticeCount,
    unsigned int vLength, unsigned int normalOffset)
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

    Mesh* obj1 = new Mesh();
    obj1->CreateMesh(vertices, indices, 32, 12);
    meshList.push_back(obj1);

    Mesh* obj2 = new Mesh();
    obj2->CreateMesh(vertices, indices, 32, 12);
    meshList.push_back(obj2);

    Mesh* obj3 = new Mesh();
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
    meshList.push_back(obj6); // todos los números

    Mesh* obj7 = new Mesh();
    obj7->CreateMesh(numeroVertices, numeroIndices, 32, 6);
    meshList.push_back(obj7); // solo un número

}


void CreateShaders()
{
    Shader* shader1 = new Shader();
    shader1->CreateFromFiles(vShader, fShader);
    shaderList.push_back(*shader1);
}

bool animacion = false;


//NEW// Keyframes
float posXcoco = -1.0, posYcoco = 10.0, posZcoco = 1.0;
float	movCoco_x = 0.0f, movCoco_y = 0.0f;

#define MAX_FRAMES 100 //N�mero de cuadros m�ximos
int i_max_steps = 20; //N�mero de pasos entre cuadros para interpolaci�n, a mayor n�mero , m�s lento ser� el movimiento
int i_curr_steps = 0;
typedef struct _frame
{
    //Variables para GUARDAR Key Frames
    float movCoco_x;		//Variable para PosicionX
    float movCoco_y;		//Variable para PosicionY
    float movCoco_xInc;		//Variable para IncrementoX
    float movCoco_yInc;		//Variable para IncrementoY
}FRAME;

FRAME KeyFrame[MAX_FRAMES];
int FrameIndex = 6;			//El n�mero de cuadros guardados actualmente desde 0 para no sobreescribir
bool play = false;
int playIndex = 0;

void saveFrame(void) //tecla L
{

    printf("frameindex %d\n", FrameIndex);


    KeyFrame[FrameIndex].movCoco_x = movCoco_x;
    KeyFrame[FrameIndex].movCoco_y = movCoco_y;
    //Se agregan nuevas l�neas para guardar m�s variables si es necesario

    //no volatil,se requiere agregar una forma de escribir a un archivo para guardar los frames
    FrameIndex++;

    err = fopen_s(&archivo, "datos.txt", "a");
    if (err != 0 || archivo == NULL) {
        printf("Error al abrir el archivo.\n");
        exit(EXIT_FAILURE);
    }
    fprintf(archivo, "%f\n", movCoco_x);
    fprintf(archivo, "%f\n", movCoco_y);

    fclose(archivo);

}

void readFile(void) {
    err = fopen_s(&archivo, "datos.txt", "r");
    if (err != 0 || archivo == NULL) {
        printf("Error al abrir el archivo.\n");
        return; 
    }

    // Reiniciamos FrameIndex para cargar la animación desde cero
    FrameIndex = 0;
    float x, y;

    while (fscanf_s(archivo, "%f\n%f", &x, &y) == 2)
    {
        if (FrameIndex >= MAX_FRAMES) {
            printf("Se alcanzó el máximo de frames (%d).\n", MAX_FRAMES);
            break; // Evitar que se desborde el arreglo
        }

        // Guardamos los valores en el ARREGLO, no en las variables globales
        KeyFrame[FrameIndex].movCoco_x = x;
        KeyFrame[FrameIndex].movCoco_y = y;

        printf("Leído Frame %d: x=%.1f, y=%.1f, g=%.1f\n", FrameIndex, x, y);

        FrameIndex++; // Incrementamos el índice de frames leídos
    }

    fclose(archivo);
    printf("Lectura de archivo completa. Total de frames cargados: %d\n", FrameIndex);

}

void resetElements(void) //Tecla 0
{

    movCoco_x = KeyFrame[0].movCoco_x;
    movCoco_y = KeyFrame[0].movCoco_y;
}

void interpolation(void)
{
    KeyFrame[playIndex].movCoco_xInc = (KeyFrame[playIndex + 1].movCoco_x - KeyFrame[playIndex].movCoco_x) / i_max_steps;
    KeyFrame[playIndex].movCoco_yInc = (KeyFrame[playIndex + 1].movCoco_y - KeyFrame[playIndex].movCoco_y) / i_max_steps;

}


void animate(void)
{
    //Movimiento del objeto con barra espaciadora
    if (play)
    {
        if (i_curr_steps >= i_max_steps) //fin de animaci�n entre frames?
        {
            playIndex++;
            printf("playindex : %d\n", playIndex);
            if (playIndex > FrameIndex - 2)	//Fin de toda la animaci�n con �ltimo frame?
            {
                printf("Frame index= %d\n", FrameIndex);
                printf("termino la animacion\n");
                playIndex = 0;
                play = false;
            }
            else //Interpolaci�n del pr�ximo cuadro
            {

                i_curr_steps = 0; //Resetea contador
                //Interpolar
                interpolation();
            }
        }
        else
        {
            //Dibujar Animaci�n
            movCoco_x += KeyFrame[playIndex].movCoco_xInc;
            movCoco_y += KeyFrame[playIndex].movCoco_yInc;
            i_curr_steps++;
        }

    }
}

int main()
{
    mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
    mainWindow.Initialise();

    CreateObjects();
    CreateShaders();

    camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.5f, 0.5f);

    brickTexture = Texture("Textures/brick.png");
    brickTexture.LoadTextureA();
    dirtTexture = Texture("Textures/dirt.png");
    dirtTexture.LoadTextureA();
    plainTexture = Texture("Textures/plain.png");
    plainTexture.LoadTextureA();
    pisoTexture = Texture("Textures/piso.tga");
    pisoTexture.LoadTextureA();
    AgaveTexture = Texture("Textures/Agave.tga");
    AgaveTexture.LoadTextureA();
    FlechaTexture = Texture("Textures/flechas.tga");
    FlechaTexture.LoadTextureA();
    NumerosTexture = Texture("Textures/numerosbase.tga");
    NumerosTexture.LoadTextureA();
    Numero1Texture = Texture("Textures/numero1.tga");
    Numero1Texture.LoadTextureA();
    Numero2Texture = Texture("Textures/numero2.tga");
    Numero2Texture.LoadTextureA();
    Degradado = Texture("Textures/degradado.png");
    Degradado.LoadTextureA();
    race = Texture("Textures/racecourse.png");
    race.LoadTextureA();
    bottom_Trunk = Texture("Textures/Bottom_Trunk.bmp");
    bottom_Trunk.LoadTextureA();
    coconut = Texture("Textures/Coconut_01.png");
    coconut.LoadTextureA();


    Kitt_M = Model();
    Kitt_M.LoadModel("Models/kitt_optimizado.obj");
    Llanta_M = Model();
    Llanta_M.LoadModel("Models/llanta_optimizada.obj");
    Dragon_M = Model();
    Dragon_M.LoadModel("Models/17174_Tiamat_new.obj");
    castillo = Model();
    castillo.LoadModel("Models/castillo.obj");
    constMedio = Model();
    constMedio.LoadModel("Models/constMedio.obj");
    constMedioChiquito = Model();
    constMedioChiquito.LoadModel("Models/constMedioChiquito.obj");
    esqIzq = Model();
    esqIzq.LoadModel("Models/esqIzq.obj");
    casitaIzq = Model();
    casitaIzq.LoadModel("Models/casitaIzq.obj");
    ruinasIzq = Model();
    ruinasIzq.LoadModel("Models/ruinasIzq.obj");
    esqIzqFrente = Model();
    esqIzqFrente.LoadModel("Models/esqIzqFrente.obj");
    cajaFrente = Model();
    cajaFrente.LoadModel("Models/cajaFrente.obj");
    constFrente = Model();
    constFrente.LoadModel("Models/constFrente.obj");
    explanadaDer = Model();
    explanadaDer.LoadModel("Models/explanadaDer.obj");
    esqDer = Model();
    esqDer.LoadModel("Models/esqDer.obj");
    racecourse = Model();
    racecourse.LoadModel("Models/racecourse.obj");
    palmera = Model();
    palmera.LoadModel("Models/palmera.obj");
    coco = Model();
    coco.LoadModel("Models/coco.obj");

    std::vector<std::string> skyboxFaces;
    skyboxFaces.push_back("Textures/Skybox/cupertin-lake_rt.tga");
    skyboxFaces.push_back("Textures/Skybox/cupertin-lake_lf.tga");
    skyboxFaces.push_back("Textures/Skybox/cupertin-lake_dn.tga");
    skyboxFaces.push_back("Textures/Skybox/cupertin-lake_up.tga");
    skyboxFaces.push_back("Textures/Skybox/cupertin-lake_bk.tga");
    skyboxFaces.push_back("Textures/Skybox/cupertin-lake_ft.tga");

    skybox = Skybox(skyboxFaces);

    Material_brillante = Material(4.0f, 256);
    Material_opaco = Material(0.3f, 4);


    //luz direccional, sólo 1 y siempre debe de existir
    mainLight = DirectionalLight(1.0f, 1.0f, 1.0f,
        0.3f, 0.3f,
        0.0f, 0.0f, -1.0f);
    //contador de luces puntuales
    unsigned int pointLightCount = 0;
    //Declaración de primer luz puntual
    pointLights[0] = PointLight(1.0f, 0.0f, 0.0f,
        0.0f, 1.0f,
        0.0f, 2.5f, 1.5f,
        0.3f, 0.2f, 0.1f);
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



    GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
        uniformSpecularIntensity = 0, uniformShininess = 0, uniformTextureOffset = 0;
    GLuint uniformColor = 0;
    glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);

    movCoche = 0.0f;
    movOffset = 0.01f;
    rotllanta = 0.0f;
    rotllantaOffset = 10.0f;

    glm::vec3 lowerLight(0.0f, 0.0f, 0.0f);

    glm::mat4 model(1.0);
    glm::mat4 modelaux(1.0);
    glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);
    glm::vec2 toffset = glm::vec2(0.0f, 0.0f);

    glm::vec3 poscoco = glm::vec3(0.0f, 0.0f, 0.0f);

    ////Loop mientras no se cierra la ventana

    printf("\nTeclas para uso de Keyframes:\n1.-Presionar barra espaciadora para reproducir animacion.\n2.-Presionar 0 para volver a habilitar reproduccion de la animacion\n");
    printf("3.-Presiona L para guardar frame\n4.-Presiona P para habilitar guardar nuevo frame\n5.-Presiona 1 para mover en X+\n6.-Presiona 2 para habilitar mover en X+\n");
    printf("7.-Presiona 3 para mover en X-\n8.-Presiona 4 para habilitar mover en X-\n,9.-Presiona 5 para mover en Y+\n10.-Presiona 6 para habilitar mover en Y+\n");
    printf("11.-Presiona 7 para mover en Y-\n12.-Presiona 8 para habilitar mover en Y-\n");

    while (!mainWindow.getShouldClose())
    {
        GLfloat now = glfwGetTime();
        deltaTime = now - lastTime;
        deltaTime += (now - lastTime) / limitFPS;
        lastTime = now;

        angulovaria += 0.5f * deltaTime;
        //dragonavance
        if (movCoche < 30.0f)
        {
            movCoche -= movOffset * deltaTime;
            //printf("avanza%f \n ",movCoche);
            rotllanta += rotllantaOffset * deltaTime;
        }


        //Recibir eventos del usuario
        glfwPollEvents();
        camera.keyControl(mainWindow.getsKeys(), deltaTime);
        camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

        inputKeyframes(mainWindow.getsKeys());
        animate();

        // Clear the window
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        skybox.DrawSkybox(camera.calculateViewMatrix(), projection);
        shaderList[0].UseShader();
        uniformModel = shaderList[0].GetModelLocation();
        uniformProjection = shaderList[0].GetProjectionLocation();
        uniformView = shaderList[0].GetViewLocation();
        uniformEyePosition = shaderList[0].GetEyePositionLocation();
        uniformColor = shaderList[0].getColorLocation();
        uniformTextureOffset = shaderList[0].getOffsetLocation(); // para la textura con movimiento

        //información en el shader de intensidad especular y brillo
        uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
        uniformShininess = shaderList[0].GetShininessLocation();

        glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
        glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

        // luz ligada a la cámara de tipo flash
        lowerLight = camera.getCameraPosition();
        lowerLight.y -= 0.3f;
        spotLights[0].SetFlash(lowerLight, camera.getCameraDirection());

        //información al shader de fuentes de iluminación
        shaderList[0].SetDirectionalLight(&mainLight);
        shaderList[0].SetPointLights(pointLights, pointLightCount);
        shaderList[0].SetSpotLights(spotLights, spotLightCount);


        //Reinicializando variables cada ciclo de reloj
        model = glm::mat4(1.0);
        modelaux = glm::mat4(1.0);
        color = glm::vec3(1.0f, 1.0f, 1.0f);
        toffset = glm::vec2(0.0f, 0.0f);

        glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
        model = glm::mat4(1.0);
        model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
        model = glm::scale(model, glm::vec3(30.0f, 1.0f, 30.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        glUniform3fv(uniformColor, 1, glm::value_ptr(color));
        glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
        pisoTexture.UseTexture();
        Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);

        meshList[2]->RenderMesh();

        model = glm::mat4(1.0);

        model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));

        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));

        color = glm::vec3(0.5f, 0.5f, 0.5f);
        glUniform3fv(uniformColor, 1, glm::value_ptr(color));

        Degradado.UseTexture();

        model = glm::mat4(1.0);
        model = glm::translate(model, glm::vec3(0.0f, 0.5f, 0.0f));
        modelaux = model;
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        castillo.RenderModel();

        model = glm::mat4(1.0);
        model = glm::translate(model, glm::vec3(0.0f, 0.5f, 0.0f));
        modelaux = model;
        // model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
       constMedio.RenderModel();

        model = glm::mat4(1.0);
        model = glm::translate(model, glm::vec3(0.0f, -5.0f, 0.0f));
        modelaux = model;
        // model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        esqIzq.RenderModel();

        model = glm::mat4(1.0);
        model = glm::translate(model, glm::vec3(0.0f, -3.0f, 0.0f));
        modelaux = model;
        // model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        casitaIzq.RenderModel();

        model = glm::mat4(1.0);
        model = glm::translate(model, glm::vec3(0.0f, -10.0f, 0.0f));
        modelaux = model;
        // model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        ruinasIzq.RenderModel();

        model = glm::mat4(1.0);
        model = glm::translate(model, glm::vec3(0.0f, -3.0f, 0.0f));
        modelaux = model;
        // model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        esqIzqFrente.RenderModel();

        model = glm::mat4(1.0);
        model = glm::translate(model, glm::vec3(0.0f, -3.0f, 0.0f));
        modelaux = model;
        // model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        cajaFrente.RenderModel();

        model = glm::mat4(1.0);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
        modelaux = model;
        // model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        constFrente.RenderModel();

        model = glm::mat4(1.0);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
        modelaux = model;
        // model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        explanadaDer.RenderModel();

        model = glm::mat4(1.0);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
        modelaux = model;
        // model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        esqDer.RenderModel();

        model = glm::mat4(1.0);
        model = glm::translate(model, glm::vec3(100.0f, 0.0f, 200.0f));
        modelaux = model;
        
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        race.UseTexture();
        racecourse.RenderModel();

        model = glm::mat4(1.0);
        model = glm::translate(model, glm::vec3(10.0f, -2.0f, 0.0f));
        modelaux = model;

        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        bottom_Trunk.UseTexture();
        palmera.RenderModel();
        //coco
        poscoco = glm::vec3(posXcoco + movCoco_x, posYcoco + movCoco_y, posZcoco);
        model = glm::translate(model, poscoco);
        modelaux = model;

        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        coconut.UseTexture();
        coco.RenderModel();


        glDisable(GL_BLEND);





        glUseProgram(0);

        mainWindow.swapBuffers();
    }


    return 0;
}


void inputKeyframes(bool* keys)
{
    if (keys[GLFW_KEY_SPACE])
    {
        if (reproduciranimacion < 1)
        {
            if (play == false && (FrameIndex > 1))
            {
                resetElements();
                //First Interpolation				
                interpolation();
                play = true;
                playIndex = 0;
                i_curr_steps = 0;
                reproduciranimacion++;
                printf("\n presiona 0 para habilitar reproducir de nuevo la animaci�n'\n");
                habilitaranimacion = 0;

            }
            else
            {
                play = false;

            }
        }
    }
    if (keys[GLFW_KEY_0])
    {
        if (habilitaranimacion < 1 && reproduciranimacion>0)
        {
            printf("Ya puedes reproducir de nuevo la animaci�n con la tecla de barra espaciadora'\n");
            reproduciranimacion = 0;

        }
    }

    if (keys[GLFW_KEY_L])
    {
        if (guardoFrame < 1)
        {
            saveFrame();
            printf("movCoco_x es: %f\n", movCoco_x);
            printf("movCoco_y es: %f\n", movCoco_y);
            printf("presiona P para habilitar guardar otro frame'\n");
            guardoFrame++;
            reinicioFrame = 0;
        }
    }
    if (keys[GLFW_KEY_P])
    {
        if (reinicioFrame < 1)
        {
            guardoFrame = 0;
            printf("Ya puedes guardar otro frame presionando la tecla L'\n");
        }
    }


    if (keys[GLFW_KEY_1])
    {
        if (ciclo < 1)
        {
            //printf("movCoco_x es: %f\n", movCoco_x);
            movCoco_x += 1.0f;
            printf("\n movCoco_x es: %f\n", movCoco_x);
            ciclo++;
            ciclo2 = 0;
            printf("\n Presiona la tecla 2 para poder habilitar la variable\n");
        }

    }
    if (keys[GLFW_KEY_2])
    {
        if (ciclo2 < 1)
        {
            ciclo = 0;
            ciclo2++;
            printf("\n Ya puedes modificar tu variable presionando la tecla 1\n");
        }
    }
    if (keys[GLFW_KEY_3])
    {
        if (ciclo3 < 1)
        {
            //printf("movCoco_x es: %f\n", movCoco_x);
            movCoco_x -= 1.0f;
            printf("\n movCoco_x es: %f\n", movCoco_x);
            ciclo3++;
            ciclo4 = 0;
            printf("\n Presiona la tecla 4 para poder habilitar la variable\n");
        }

    }
    if (keys[GLFW_KEY_4])
    {
        if (ciclo4 < 1)
        {
            ciclo3 = 0;
            ciclo4++;
            printf("\n Ya puedes modificar tu variable presionando la tecla 3\n");
        }
    }

    //movimiento en Y
    if (keys[GLFW_KEY_5])
    {
        if (ciclo5 < 1)
        {
            //printf("movCoco_x es: %f\n", movCoco_x);
            movCoco_y += 1.0f;
            printf("\n movAvion_y es: %f\n", movCoco_y);
            ciclo5++;
            ciclo6 = 0;
            printf("\n Presiona la tecla 6 para poder habilitar la variable\n");
        }

    }
    if (keys[GLFW_KEY_6])
    {
        if (ciclo6 < 1)
        {
            ciclo5 = 0;
            ciclo6++;
            printf("\n Ya puedes modificar tu variable presionando la tecla 5\n");
        }
    }

    if (keys[GLFW_KEY_7])
    {
        if (ciclo7 < 1)
        {
            //printf("movCoco_x es: %f\n", movCoco_x);
            movCoco_y -= 1.0f;
            printf("\n movAvion_y es: %f\n", movCoco_y);
            ciclo7++;
            ciclo8 = 0;
            printf("\n Presiona la tecla 8 para poder habilitar la variable\n");
        }

    }
    if (keys[GLFW_KEY_8])
    {
        if (ciclo8 < 1)
        {
            ciclo7 = 0;
            ciclo8++;
            printf("\n Ya puedes modificar tu variable presionando la tecla 7\n");
        }
    }

    if (keys[GLFW_KEY_R])
    {
        resetElements();
        printf("--- Leyendo archivo y reiniciando animación ---\n");

        play = false;
        i_curr_steps = 0;


        readFile();

        if (FrameIndex > 1)
        {
            interpolation();
            play = true;
            playIndex = 0;
        }
        else
        {
            printf("Archivo leído, pero no hay suficientes frames para animar.\n");
        }
    }

}
