#include "Window.h"
#include <cmath>

Window::Window()
{
	width = 800;
	height = 600;
	for (size_t i = 0; i < 1024; i++)
	{
		keys[i] = 0;
	}
}
Window::Window(GLint windowWidth, GLint windowHeight)
{
	width = windowWidth;
	height = windowHeight;
	muevex = 2.0f;
	articulacion1 = 0.0f;
	articulacion2 = 0.0f;
	articulacion3 = 0.0f;
	articulacion4 = 0.0f;
	articulacion5 = 0.0f;
	articulacion6 = 0.0f;
	llanta1 = 0.0f;
	llanta2 = 0.0f;
	llanta3 = 0.0f;
	llanta4 = 0.0f;
	mov = 0.0f;
	mov2 = 0.0f;
	prendido = 1.0f;
	prendido2 = 1.0f;
	prendido3 = 1.0f;
	puerta = 0.0f;
	dado = 0.0;
	for (size_t i = 0; i < 1024; i++)
	{
		keys[i] = 0;
	}
}

int Window::Initialise()
{
	//Inicialización de GLFW
	if (!glfwInit())
	{
		printf("Falló inicializar GLFW");
		glfwTerminate();
		return 1;
	}
	//Asignando variables de GLFW y propiedades de ventana
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//para solo usar el core profile de OpenGL y no tener retrocompatibilidad
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	//CREAR VENTANA
	mainWindow = glfwCreateWindow(width, height, "PracticaXX:Nombre de la practica", NULL, NULL);

	if (!mainWindow)
	{
		printf("Fallo en crearse la ventana con GLFW");
		glfwTerminate();
		return 1;
	}
	//Obtener tamaño de Buffer
	glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

	//asignar el contexto
	glfwMakeContextCurrent(mainWindow);

	//MANEJAR TECLADO y MOUSE
	createCallbacks();


	//permitir nuevas extensiones
	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK)
	{
		printf("Falló inicialización de GLEW");
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return 1;
	}

	glEnable(GL_DEPTH_TEST); //HABILITAR BUFFER DE PROFUNDIDAD
	// Asignar valores de la ventana y coordenadas

	//Asignar Viewport
	glViewport(0, 0, bufferWidth, bufferHeight);
	//Callback para detectar que se está usando la ventana
	glfwSetWindowUserPointer(mainWindow, this);
}

void Window::createCallbacks()
{
	glfwSetKeyCallback(mainWindow, ManejaTeclado);
	glfwSetCursorPosCallback(mainWindow, ManejaMouse);
}
GLfloat Window::getXChange()
{
	GLfloat theChange = xChange;
	xChange = 0.0f;
	return theChange;
}

GLfloat Window::getYChange()
{
	GLfloat theChange = yChange;
	yChange = 0.0f;
	return theChange;
}




void Window::ManejaTeclado(GLFWwindow * window, int key, int code, int action, int mode)
{
	float tiempo = 0.0f;
	float velocidad = 2.0f;

	float minValmand = -0.0f;
	float maxValmand = 60.0f;

	float minValpata = -45.0f;
	float maxValpata = 45.0f;

	Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
	if (key == GLFW_KEY_Y)
	{
		theWindow->muevex += 1.0;
	}
	if (key == GLFW_KEY_U)
	{
		theWindow->muevex -= 1.0;
	}
	if (key == GLFW_KEY_F)
	{
		float offset = (maxValmand + minValmand) / 2.0f;
		float amplitud = (maxValmand - minValmand) / 2.0f;


		float deltaTime = glfwGetTime();
		tiempo += deltaTime * velocidad;

		theWindow->articulacion1 = offset + amplitud * sin(tiempo);
	}
	//lantas
	if (key == GLFW_KEY_G && action == GLFW_PRESS)
	{
		if (theWindow->prendido3 > 0.5) {
			theWindow->prendido3 = 0;
		}
		else {
			theWindow->prendido3 = 1;
		}
	}
	if (key == GLFW_KEY_O)
	{
		theWindow->mov += 0.5;
		theWindow->llanta1 += 10.0;
		theWindow->llanta2 += 10.0;
		theWindow->llanta3 += 10.0;
		theWindow->llanta4 += 10.0;
		theWindow->prendido2 = 0.0;
	}
	if (key == GLFW_KEY_P)
	{
		theWindow->mov -= 0.5;
		theWindow->llanta1 -= 10.0;
		theWindow->llanta2 -= 10.0;
		theWindow->llanta3 -= 10.0;
		theWindow->llanta4 -= 10.0;
		theWindow->prendido2 = 1.0;
	}
	if (key == GLFW_KEY_Z)
	{
		theWindow->mov2 += 0.5;
	}
	if (key == GLFW_KEY_X)
	{
		theWindow->mov2 -= 0.5;
	}
	if (key == GLFW_KEY_M && action == GLFW_PRESS)
	{	
		if(theWindow->prendido > 0.5){
			theWindow->prendido = 0;
		}
		else {
			theWindow->prendido = 1;
		}
		
	}
	if (key == GLFW_KEY_P && action == GLFW_PRESS)
	{
		if (theWindow->puerta > 0.5) {
			theWindow->puerta = 0;
		}
		else {
			theWindow->puerta = 1;
		}

	}

	if (key == GLFW_KEY_R && action == GLFW_PRESS) 
	{
		theWindow->dado +=0.5;
	}

	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
			theWindow->keys[key] = true;
			//printf("se presiono la tecla %d'\n", key);
		}
		else if (action == GLFW_RELEASE)
		{
			theWindow->keys[key] = false;
			//printf("se solto la tecla %d'\n", key);
		}
	}
}

void Window::ManejaMouse(GLFWwindow * window, double xPos, double yPos)
{
	Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

	if (theWindow->mouseFirstMoved)
	{
		theWindow->lastX = xPos;
		theWindow->lastY = yPos;
		theWindow->mouseFirstMoved = false;
	}

	theWindow->xChange = xPos - theWindow->lastX;
	theWindow->yChange = theWindow->lastY - yPos;

	theWindow->lastX = xPos;
	theWindow->lastY = yPos;
}


Window::~Window()
{
	glfwDestroyWindow(mainWindow);
	glfwTerminate();

}
