//prctica 3: Modelado Geomtrico y Cmara Sinttica.
#include <stdio.h>
#include <string.h>
#include<cmath>
#include<vector>
#include <glew.h>
#include <glfw3.h>
//glm
#include<glm.hpp>
#include<gtc\matrix_transform.hpp>
#include<gtc\type_ptr.hpp>
#include <gtc\random.hpp>
//clases para dar orden y limpieza al cdigo
#include"Mesh.h"
#include"Shader.h"
#include"Sphere.h"
#include"Window.h"
#include"Camera.h"
//tecla E: Rotar sobre el eje X
//tecla R: Rotar sobre el eje Y
//tecla T: Rotar sobre el eje Z


using std::vector;

//Dimensiones de la ventana
const float toRadians = 3.14159265f / 180.0; //grados a radianes
const float PI = 3.14159265f;
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;
Camera camera;
Window mainWindow;
vector<Mesh*> meshList;
vector<Shader>shaderList;
//Vertex Shader
static const char* vShader = "shaders/shader.vert";
static const char* fShader = "shaders/shader.frag";
static const char* vShaderColor = "shaders/shadercolor.vert";
Sphere sp = Sphere(1.0, 20, 20); //recibe radio, slices, stacks



// Pirmide triangular regular
void CrearPiramideTriangular()
{
	unsigned int indices_piramide_triangular[] = {
			0,1,2,
			1,3,2,
			3,0,2,
			1,0,3

	};
	GLfloat vertices_piramide_triangular[] = {
		-0.5f, -0.2025f,0.3f,	//0
		0.5f,-0.2025f,0.3f,	//1
		0.0f,-0.2025f, -0.566f,	//2
		0.0f,0.6075f,0.0f,	//3

	};
	Mesh* obj1 = new Mesh();
	obj1->CreateMesh(vertices_piramide_triangular, indices_piramide_triangular, 12, 12);
	meshList.push_back(obj1);

}

// Pirmide triangular regular
void CrearPiramideTriangularV()
{
	unsigned int indices_piramide_triangularv[] = {
			0,1,2,
			1,3,2,
			3,0,2,
			1,0,3

	};
	GLfloat vertices_piramide_triangularv[] = {
		-0.5f, 0.2025f,0.3f,	//0
		0.5f,0.2025f,0.3f,	//1
		0.0f,0.2025f, -0.566f,	//2
		0.0f,-0.405f,0.0f,	//3

	};
	Mesh* obj1 = new Mesh();
	obj1->CreateMesh(vertices_piramide_triangularv, indices_piramide_triangularv, 12, 12);
	meshList.push_back(obj1);

}



void CreateShaders()
{
	Shader* shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);

	Shader* shader2 = new Shader();
	shader2->CreateFromFiles(vShaderColor, fShader);
	shaderList.push_back(*shader2);
}


int main()
{
	mainWindow = Window(800, 600);
	mainWindow.Initialise();
	//Cilindro y cono reciben resolucin (slices, rebanadas) y Radio de circunferencia de la base y tapa

	CrearPiramideTriangularV();//ndice 0 en MeshList
	CrearPiramideTriangular();//ndice 1 en MeshList
	CreateShaders();



	/*Cmara se usa el comando: glm::lookAt(vector de posicin, vector de orientacin, vector up));
	En la clase Camera se reciben 5 datos:
	glm::vec3 vector de posicin,
	glm::vec3 vector up,
	GlFloat yaw rotacin para girar hacia la derecha e izquierda
	GlFloat pitch rotacin para inclinar hacia arriba y abajo
	GlFloat velocidad de desplazamiento,
	GlFloat velocidad de vuelta o de giro
	Se usa el Mouse y las teclas WASD y su posicin inicial est en 0,0,1 y ve hacia 0,0,-1.
	*/
	// Adjusted camera position to compensate for the new object position
	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.3f, 0.3f);


	GLuint uniformProjection = 0;
	GLuint uniformModel = 0;
	GLuint uniformView = 0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(glm::radians(60.0f), mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 100.0f);
	//glm::mat4 projection = glm::ortho(-1, 1, -1, 1, 1, 10);

	//Loop mientras no se cierra la ventana
	sp.init(); //inicializar esfera
	sp.load();//enviar la esfera al shader

	glm::mat4 model(1.0);//Inicializar matriz de Modelo 4x4

	glm::vec3 color = glm::vec3(0.0f, 0.0f, 0.0f); //inicializar Color para enviar a variable Uniform;
	const float toRadians = 3.14159265f / 180.0;

	// Calculate the translation vector to move the black pyramid from (0.0f, 0.2025f, -3.8f) to (0,0,0)
	// This vector will be applied to all objects.
	glm::vec3 baseTranslation = glm::vec3(0.0f, -0.2025f, 3.8f);

	while (!mainWindow.getShouldClose())
	{

		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;
		//Recibir eventos del usuario
		glfwPollEvents();
		//Cmara
		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		//Limpiar la ventana
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Se agrega limpiar el buffer de profundidad
		shaderList[0].useShader();
		uniformModel = shaderList[0].getModelLocation();
		uniformProjection = shaderList[0].getProjectLocation();
		uniformView = shaderList[0].getViewLocation();
		uniformColor = shaderList[0].getColorLocation();


		//Negro
		model = glm::mat4(1.0);
		//otras transformaciones para el objeto
		model = glm::rotate(model, glm::radians(mainWindow.getrotax()), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getrotay()), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getrotaz()), glm::vec3(0.0f, 0.0f, 1.0f));
		// The new translation moves the black pyramid to (0,0,0)
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.3f, 1.3f, 1.3f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//la lnea de proyeccin solo se manda una vez a menos que en tiempo de ejecucin
		//se programe cambio entre proyeccin ortogonal y perspectiva
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		color = glm::vec3(0.0f, 0.0f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		meshList[1]->RenderMesh(); //dibuja cubo y pirmide triangular


		//Rojo
		model = glm::mat4(1.0);
		//otras transformaciones para el objeto
		model = glm::rotate(model, glm::radians(mainWindow.getrotax()), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getrotay()), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getrotaz()), glm::vec3(0.0f, 0.0f, 1.0f));
		// The old translate vector (0.0f, 0.68f, -3.75f) - (0.0f, 0.2025f, -3.8f) = (0.0f, 0.4775f, 0.05f)
		model = glm::translate(model, glm::vec3(0.0f, 0.4775f, 0.05f));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//la lnea de proyeccin solo se manda una vez a menos que en tiempo de ejecucin
		//se programe cambio entre proyeccin ortogonal y perspectiva
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		color = glm::vec3(1.0f, 0.0f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		meshList[1]->RenderMesh(); //dibuja cubo y pirmide triangular

		model = glm::mat4(1.0);
		model = glm::rotate(model, glm::radians(mainWindow.getrotax()), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getrotay()), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getrotaz()), glm::vec3(0.0f, 0.0f, 1.0f));
		// (0.0f, 0.45f, -3.63f) + (0.0f, -0.2025f, 3.8f) = (0.0f, 0.2475f, 0.17f)
		model = glm::translate(model, glm::vec3(-0.0f, 0.2475f, 0.17f));
		model = glm::rotate(model, 248 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//la lnea de proyeccin solo se manda una vez a menos que en tiempo de ejecucin
		//se programe cambio entre proyeccin ortogonal y perspectiva
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		color = glm::vec3(1.0f, 0.0f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		meshList[1]->RenderMesh(); //dibuja cubo y pirmide triangular

		model = glm::mat4(1.0);
		model = glm::rotate(model, glm::radians(mainWindow.getrotax()), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getrotay()), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getrotaz()), glm::vec3(0.0f, 0.0f, 1.0f));
		// (-0.19f, 0.38f, -3.63f) + (0.0f, -0.2025f, 3.8f) = (-0.19f, 0.1775f, 0.17f)
		model = glm::translate(model, glm::vec3(-0.19f, 0.1775f, 0.17f));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//la lnea de proyeccin solo se manda una vez a menos que en tiempo de ejecucin
		//se programe cambio entre proyeccin ortogonal y perspectiva
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		color = glm::vec3(1.0f, 0.0f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		meshList[1]->RenderMesh(); //dibuja cubo y pirmide triangular

		model = glm::mat4(1.0);
		model = glm::rotate(model, glm::radians(mainWindow.getrotax()), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getrotay()), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getrotaz()), glm::vec3(0.0f, 0.0f, 1.0f));
		// (0.19f, 0.37f, -3.63f) + (0.0f, -0.2025f, 3.8f) = (0.19f, 0.1675f, 0.17f)
		model = glm::translate(model, glm::vec3(0.19f, 0.1675f, 0.17f));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//la lnea de proyeccin solo se manda una vez a menos que en tiempo de ejecucin
		//se programe cambio entre proyeccin ortogonal y perspectiva
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		color = glm::vec3(1.0f, 0.0f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		meshList[1]->RenderMesh(); //dibuja cubo y pirmide triangular

		model = glm::mat4(1.0);
		model = glm::rotate(model, glm::radians(mainWindow.getrotax()), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getrotay()), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getrotaz()), glm::vec3(0.0f, 0.0f, 1.0f));
		// (0.0f, 0.08f, -3.52f) + (0.0f, -0.2025f, 3.8f) = (0.0f, -0.1225f, 0.28f)
		model = glm::translate(model, glm::vec3(0.0f, -0.1225f, 0.28f));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//la lnea de proyeccin solo se manda una vez a menos que en tiempo de ejecucin
		//se programe cambio entre proyeccin ortogonal y perspectiva
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		color = glm::vec3(1.0f, 0.0f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		meshList[1]->RenderMesh(); //dibuja cubo y pirmide triangular

		model = glm::mat4(1.0);
		model = glm::rotate(model, glm::radians(mainWindow.getrotax()), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getrotay()), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getrotaz()), glm::vec3(0.0f, 0.0f, 1.0f));
		// (-0.19f, 0.16f, -3.52f) + (0.0f, -0.2025f, 3.8f) = (-0.19f, -0.0425f, 0.28f)
		model = glm::translate(model, glm::vec3(-0.19f, -0.0425f, 0.28f));
		model = glm::rotate(model, 245 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//la lnea de proyeccin solo se manda una vez a menos que en tiempo de ejecucin
		//se programe cambio entre proyeccin ortogonal y perspectiva
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		color = glm::vec3(1.0f, 0.0f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		meshList[1]->RenderMesh(); //dibuja cubo y pirmide triangular

		model = glm::mat4(1.0);
		model = glm::rotate(model, glm::radians(mainWindow.getrotax()), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getrotay()), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getrotaz()), glm::vec3(0.0f, 0.0f, 1.0f));
		// (-0.37f, 0.08f, -3.52f) + (0.0f, -0.2025f, 3.8f) = (-0.37f, -0.1225f, 0.28f)
		model = glm::translate(model, glm::vec3(-0.37f, -0.1225f, 0.28f));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//la lnea de proyeccin solo se manda una vez a menos que en tiempo de ejecucin
		//se programe cambio entre proyeccin ortogonal y perspectiva
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		color = glm::vec3(1.0f, 0.0f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		meshList[1]->RenderMesh(); //dibuja cubo y pirmide triangular

		model = glm::mat4(1.0);
		model = glm::rotate(model, glm::radians(mainWindow.getrotax()), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getrotay()), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getrotaz()), glm::vec3(0.0f, 0.0f, 1.0f));
		// (0.19f, 0.16f, -3.52f) + (0.0f, -0.2025f, 3.8f) = (0.19f, -0.0425f, 0.28f)
		model = glm::translate(model, glm::vec3(0.19f, -0.0425f, 0.28f));
		model = glm::rotate(model, 245 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//la lnea de proyeccin solo se manda una vez a menos que en tiempo de ejecucin
		//se programe cambio entre proyeccin ortogonal y perspectiva
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		color = glm::vec3(1.0f, 0.0f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		meshList[1]->RenderMesh(); //dibuja cubo y pirmide triangular

		model = glm::mat4(1.0);
		model = glm::rotate(model, glm::radians(mainWindow.getrotax()), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getrotay()), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getrotaz()), glm::vec3(0.0f, 0.0f, 1.0f));
		// (0.37f, 0.08f, -3.52f) + (0.0f, -0.2025f, 3.8f) = (0.37f, -0.1225f, 0.28f)
		model = glm::translate(model, glm::vec3(0.37f, -0.1225f, 0.28f));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//la lnea de proyeccin solo se manda una vez a menos que en tiempo de ejecucin
		//se programe cambio entre proyeccin ortogonal y perspectiva
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		color = glm::vec3(1.0f, 0.0f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		meshList[1]->RenderMesh(); //dibuja cubo y pirmide triangular

		//verde
		model = glm::mat4(1.0);
		model = glm::rotate(model, glm::radians(mainWindow.getrotax()), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getrotay()), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getrotaz()), glm::vec3(0.0f, 0.0f, 1.0f));
		// (-0.05f, 0.68f, -3.83f) + (0.0f, -0.2025f, 3.8f) = (-0.05f, 0.4775f, -0.03f)
		model = glm::translate(model, glm::vec3(-0.05f, 0.4775f, -0.03f));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//la lnea de proyeccin solo se manda una vez a menos que en tiempo de ejecucin
		//se programe cambio entre proyeccin ortogonal y perspectiva
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		color = glm::vec3(0.0f, 1.0f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		meshList[1]->RenderMesh(); //dibuja cubo y pirmide triangular

		model = glm::mat4(1.0);
		model = glm::rotate(model, glm::radians(mainWindow.getrotax()), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getrotay()), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getrotaz()), glm::vec3(0.0f, 0.0f, 1.0f));
		// (-0.04f, 0.37f, -4.05f) + (0.0f, -0.2025f, 3.8f) = (-0.04f, 0.1675f, -0.25f)
		model = glm::translate(model, glm::vec3(-0.04f, 0.1675f, -0.25f));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//la lnea de proyeccin solo se manda una vez a menos que en tiempo de ejecucin
		//se programe cambio entre proyeccin ortogonal y perspectiva
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		color = glm::vec3(0.0f, 1.0f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color)); //para cambiar el color del objetos
		meshList[1]->RenderMesh(); //dibuja cubo y pirmide triangular

		model = glm::mat4(1.0);
		model = glm::rotate(model, glm::radians(mainWindow.getrotax()), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getrotay()), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getrotaz()), glm::vec3(0.0f, 0.0f, 1.0f));
		// (-0.15f, 0.45f, -3.875f) + (0.0f, -0.2025f, 3.8f) = (-0.15f, 0.2475f, -0.075f)
		model = glm::translate(model, glm::vec3(-0.15f, 0.2475f, -0.075f));
		model = glm::rotate(model, 165 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, -30 * toRadians, glm::vec3(1.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		color = glm::vec3(0.0f, 1.0f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::rotate(model, glm::radians(mainWindow.getrotax()), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getrotay()), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getrotaz()), glm::vec3(0.0f, 0.0f, 1.0f));
		// (-0.25f, 0.37f, -3.7f) + (0.0f, -0.2025f, 3.8f) = (-0.25f, 0.1675f, 0.1f)
		model = glm::translate(model, glm::vec3(-0.25f, 0.1675f, 0.1f));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		color = glm::vec3(0.0f, 1.0f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::rotate(model, glm::radians(mainWindow.getrotax()), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getrotay()), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getrotaz()), glm::vec3(0.0f, 0.0f, 1.0f));
		// (-0.23f, 0.08f, -3.92f) + (0.0f, -0.2025f, 3.8f) = (-0.23f, -0.1225f, -0.12f)
		model = glm::translate(model, glm::vec3(-0.23f, -0.1225f, -0.12f));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		color = glm::vec3(0.0f, 1.0f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::rotate(model, glm::radians(mainWindow.getrotax()), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getrotay()), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getrotaz()), glm::vec3(0.0f, 0.0f, 1.0f));
		// (-0.12f, 0.17f, -4.085f) + (0.0f, -0.2025f, 3.8f) = (-0.12f, -0.0325f, -0.285f)
		model = glm::translate(model, glm::vec3(-0.12f, -0.0325f, -0.285f));
		model = glm::rotate(model, 165 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, -30 * toRadians, glm::vec3(1.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		color = glm::vec3(0.0f, 1.0f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::rotate(model, glm::radians(mainWindow.getrotax()), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getrotay()), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getrotaz()), glm::vec3(0.0f, 0.0f, 1.0f));
		// (-0.03f, 0.08f, -4.26f) + (0.0f, -0.2025f, 3.8f) = (-0.03f, -0.1225f, -0.46f)
		model = glm::translate(model, glm::vec3(-0.03f, -0.1225f, -0.46f));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		color = glm::vec3(0.0f, 1.0f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::rotate(model, glm::radians(mainWindow.getrotax()), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getrotay()), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getrotaz()), glm::vec3(0.0f, 0.0f, 1.0f));
		// (-0.3f, 0.17f, -3.75f) + (0.0f, -0.2025f, 3.8f) = (-0.3f, -0.0325f, 0.05f)
		model = glm::translate(model, glm::vec3(-0.3f, -0.0325f, 0.05f));
		model = glm::rotate(model, 165 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, -30 * toRadians, glm::vec3(1.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		color = glm::vec3(0.0f, 1.0f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::rotate(model, glm::radians(mainWindow.getrotax()), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getrotay()), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getrotaz()), glm::vec3(0.0f, 0.0f, 1.0f));
		// (-0.42f, 0.08f, -3.61f) + (0.0f, -0.2025f, 3.8f) = (-0.42f, -0.1225f, 0.19f)
		model = glm::translate(model, glm::vec3(-0.42f, -0.1225f, 0.19f));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		color = glm::vec3(0.0f, 1.0f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[1]->RenderMesh();

		// azul
		model = glm::mat4(1.0);
		model = glm::rotate(model, glm::radians(mainWindow.getrotax()), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getrotay()), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getrotaz()), glm::vec3(0.0f, 0.0f, 1.0f));
		// (0.05f, 0.68f, -3.83f) + (0.0f, -0.2025f, 3.8f) = (0.05f, 0.4775f, -0.03f)
		model = glm::translate(model, glm::vec3(0.05f, 0.4775f, -0.03f));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		color = glm::vec3(0.0f, 0.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::rotate(model, glm::radians(mainWindow.getrotax()), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getrotay()), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getrotaz()), glm::vec3(0.0f, 0.0f, 1.0f));
		// (0.04f, 0.37f, -4.05f) + (0.0f, -0.2025f, 3.8f) = (0.04f, 0.1675f, -0.25f)
		model = glm::translate(model, glm::vec3(0.04f, 0.1675f, -0.25f));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		color = glm::vec3(0.0f, 0.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::rotate(model, glm::radians(mainWindow.getrotax()), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getrotay()), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getrotaz()), glm::vec3(0.0f, 0.0f, 1.0f));
		// (0.13f, 0.48f, -3.87f) + (0.0f, -0.2025f, 3.8f) = (0.13f, 0.2775f, -0.07f)
		model = glm::translate(model, glm::vec3(0.13f, 0.2775f, -0.07f));
		model = glm::rotate(model, 220 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, -22 * toRadians, glm::vec3(1.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		color = glm::vec3(0.0f, 0.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::rotate(model, glm::radians(mainWindow.getrotax()), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getrotay()), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getrotaz()), glm::vec3(0.0f, 0.0f, 1.0f));
		// (0.25f, 0.37f, -3.7f) + (0.0f, -0.2025f, 3.8f) = (0.25f, 0.1675f, 0.1f)
		model = glm::translate(model, glm::vec3(0.25f, 0.1675f, 0.1f));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		color = glm::vec3(0.0f, 0.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::rotate(model, glm::radians(mainWindow.getrotax()), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getrotay()), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getrotaz()), glm::vec3(0.0f, 0.0f, 1.0f));
		// (0.23f, 0.08f, -3.92f) + (0.0f, -0.2025f, 3.8f) = (0.23f, -0.1225f, -0.12f)
		model = glm::translate(model, glm::vec3(0.23f, -0.1225f, -0.12f));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		color = glm::vec3(0.0f, 0.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::rotate(model, glm::radians(mainWindow.getrotax()), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getrotay()), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getrotaz()), glm::vec3(0.0f, 0.0f, 1.0f));
		// (0.317f, 0.185f, -3.75f) + (0.0f, -0.2025f, 3.8f) = (0.317f, -0.0175f, 0.05f)
		model = glm::translate(model, glm::vec3(0.317f, -0.0175f, 0.05f));
		model = glm::rotate(model, 220 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, -22 * toRadians, glm::vec3(1.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		color = glm::vec3(0.0f, 0.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::rotate(model, glm::radians(mainWindow.getrotax()), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getrotay()), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getrotaz()), glm::vec3(0.0f, 0.0f, 1.0f));
		// (0.03f, 0.08f, -4.26f) + (0.0f, -0.2025f, 3.8f) = (0.03f, -0.1225f, -0.46f)
		model = glm::translate(model, glm::vec3(0.03f, -0.1225f, -0.46f));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		color = glm::vec3(0.0f, 0.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::rotate(model, glm::radians(mainWindow.getrotax()), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getrotay()), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getrotaz()), glm::vec3(0.0f, 0.0f, 1.0f));
		// (0.127f, 0.185f, -4.08f) + (0.0f, -0.2025f, 3.8f) = (0.127f, -0.0175f, -0.28f)
		model = glm::translate(model, glm::vec3(0.127f, -0.0175f, -0.28f));
		model = glm::rotate(model, 220 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, -22 * toRadians, glm::vec3(1.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		color = glm::vec3(0.0f, 0.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::rotate(model, glm::radians(mainWindow.getrotax()), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getrotay()), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getrotaz()), glm::vec3(0.0f, 0.0f, 1.0f));
		// (0.42f, 0.08f, -3.61f) + (0.0f, -0.2025f, 3.8f) = (0.42f, -0.1225f, 0.19f)
		model = glm::translate(model, glm::vec3(0.42f, -0.1225f, 0.19f));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		color = glm::vec3(0.0f, 0.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[1]->RenderMesh();


		// amarillo
		model = glm::mat4(1.0);
		model = glm::rotate(model, glm::radians(mainWindow.getrotax()), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getrotay()), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getrotaz()), glm::vec3(0.0f, 0.0f, 1.0f));
		// (0.0f, -0.01f, -4.25f) + (0.0f, -0.2025f, 3.8f) = (0.0f, -0.2125f, -0.45f)
		model = glm::translate(model, glm::vec3(0.0f, -0.2125f, -0.45f));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		color = glm::vec3(0.98f, 0.88f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::rotate(model, glm::radians(mainWindow.getrotax()), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getrotay()), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getrotaz()), glm::vec3(0.0f, 0.0f, 1.0f));
		// (-0.19f, -0.01f, -3.9f) + (0.0f, -0.2025f, 3.8f) = (-0.19f, -0.2125f, -0.1f)
		model = glm::translate(model, glm::vec3(-0.19f, -0.2125f, -0.1f));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		color = glm::vec3(0.98f, 0.88f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::rotate(model, glm::radians(mainWindow.getrotax()), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getrotay()), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getrotaz()), glm::vec3(0.0f, 0.0f, 1.0f));
		// (0.0f, -0.01f, -4.02f) + (0.0f, -0.2025f, 3.8f) = (0.0f, -0.2125f, -0.22f)
		model = glm::translate(model, glm::vec3(0.0f, -0.2125f, -0.22f));
		model = glm::rotate(model, PI, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		color = glm::vec3(0.98f, 0.88f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::rotate(model, glm::radians(mainWindow.getrotax()), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getrotay()), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getrotaz()), glm::vec3(0.0f, 0.0f, 1.0f));
		// (0.19f, -0.01f, -3.9f) + (0.0f, -0.2025f, 3.8f) = (0.19f, -0.2125f, -0.1f)
		model = glm::translate(model, glm::vec3(0.19f, -0.2125f, -0.1f));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		color = glm::vec3(0.98f, 0.88f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::rotate(model, glm::radians(mainWindow.getrotax()), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getrotay()), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getrotaz()), glm::vec3(0.0f, 0.0f, 1.0f));
		// (0.0f, -0.01f, -3.55f) + (0.0f, -0.2025f, 3.8f) = (0.0f, -0.2125f, 0.25f)
		model = glm::translate(model, glm::vec3(0.0f, -0.2125f, 0.25f));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		color = glm::vec3(0.98f, 0.88f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::rotate(model, glm::radians(mainWindow.getrotax()), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getrotay()), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getrotaz()), glm::vec3(0.0f, 0.0f, 1.0f));
		// (-0.19f, -0.01f, -3.66f) + (0.0f, -0.2025f, 3.8f) = (-0.19f, -0.2125f, 0.14f)
		model = glm::translate(model, glm::vec3(-0.19f, -0.2125f, 0.14f));
		model = glm::rotate(model, PI, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		color = glm::vec3(0.98f, 0.88f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::rotate(model, glm::radians(mainWindow.getrotax()), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getrotay()), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getrotaz()), glm::vec3(0.0f, 0.0f, 1.0f));
		// (-0.39f, -0.01f, -3.55f) + (0.0f, -0.2025f, 3.8f) = (-0.39f, -0.2125f, 0.25f)
		model = glm::translate(model, glm::vec3(-0.39f, -0.2125f, 0.25f));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		color = glm::vec3(0.98f, 0.88f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::rotate(model, glm::radians(mainWindow.getrotax()), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getrotay()), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getrotaz()), glm::vec3(0.0f, 0.0f, 1.0f));
		// (0.19f, -0.01f, -3.66f) + (0.0f, -0.2025f, 3.8f) = (0.19f, -0.2125f, 0.14f)
		model = glm::translate(model, glm::vec3(0.19f, -0.2125f, 0.14f));
		model = glm::rotate(model, PI, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		color = glm::vec3(0.98f, 0.88f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::rotate(model, glm::radians(mainWindow.getrotax()), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getrotay()), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(mainWindow.getrotaz()), glm::vec3(0.0f, 0.0f, 1.0f));
		// (0.39f, -0.01f, -3.55f) + (0.0f, -0.2025f, 3.8f) = (0.39f, -0.2125f, 0.25f)
		model = glm::translate(model, glm::vec3(0.39f, -0.2125f, 0.25f));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		color = glm::vec3(0.98f, 0.88f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[1]->RenderMesh();

		glUseProgram(0);
		mainWindow.swapBuffers();
	}
	return 0;
}