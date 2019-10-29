#include <iostream>
#include "Render.h"
using namespace std;

float xMin = -1.0, xMax = 1.0, yMin = -1.0, yMax = 1.0; 

struct enemy {
	bool alive = true;
	int xPos;
	int yPos;
};

//Player
float movex, movey, lastmovex;
bool shoot = false;
bool alive = true;

//Enemies
const int enemiesCount = 5;
enemy enemies[enemiesCount];

void DrawEnemies()
{
	glColor3f(0.52, 0.8, 0.22);
	glBegin(GL_QUADS);
	glVertex2f(-0.05, 0.5);
	glVertex2f(0.05, 0.5);
	glVertex2f(0.05, 0.6);
	glVertex2f(-0.05, 0.6);
	glEnd();
}

void SpawnEnemies()
{
	float xPos = 0.05;
	float yPos = 0.05;

	for (int i = 0; i < enemiesCount; i++)
	{
		enemies[i].xPos = xPos;
		enemies[i].yPos = yPos;

		xPos = xPos + 0.02;
	}
}

void Animation() {

	if (shoot && movey <= 1.5)
		movey += 0.1;
	else {
		shoot = false;
		movey = 0.0;
	}
}

void DrawScene() {

	if (!shoot)
		lastmovex = movex;

	//Shoot
	if (alive)
	{
		glColor3f(0.75, 0.2, 0.2);
		glBegin(GL_QUADS);
		glVertex2f(-0.05 + lastmovex, -0.7 + movey);
		glVertex2f(0.05 + lastmovex, -0.7 + movey);
		glVertex2f(0.05 + lastmovex, -0.6 + movey);
		glVertex2f(-0.05 + lastmovex, -0.6 + movey);
		glEnd();
	}

	//Player
	if (alive)
	{
		glColor3f(0.8, 0.8, 0.8);
		glBegin(GL_TRIANGLES);
		glVertex2f(-0.2 + movex, -0.8);
		glVertex2f(0.2 + movex, -0.8);
		glVertex2f(0.0 + movex, -0.5);
		glEnd();

		glColor3f(0.65, 0.65, 0.65);
		glBegin(GL_QUADS);
		glVertex2f(-0.1 + movex, -0.5);
		glVertex2f(-0.2 + movex, -0.5);
		glVertex2f(-0.2 + movex, -0.8);
		glVertex2f(-0.1 + movex, -0.8);
		glEnd();

		glColor3f(0.65, 0.65, 0.65);
		glBegin(GL_QUADS);
		glVertex2f(0.1 + movex, -0.5);
		glVertex2f(0.2 + movex, -0.5);
		glVertex2f(0.2 + movex, -0.8);
		glVertex2f(0.1 + movex, -0.8);
		glEnd();

		glColor3f(0.4, 0.4, 0.4);
		glBegin(GL_QUADS);
		glVertex2f(-0.05 + movex, -0.7);
		glVertex2f(0.05 + movex, -0.7);
		glVertex2f(0.05 + movex, -0.6);
		glVertex2f(-0.05 + movex, -0.6);
		glEnd();
	}
	else
	{
		glColor3f(0.3, 0.3, 0.3);
		glBegin(GL_TRIANGLES);
		glVertex2f(-0.2 + movex, -0.8);
		glVertex2f(0.2 + movex, -0.8);
		glVertex2f(0.0 + movex, -0.5);
		glEnd();

		glColor3f(0.25, 0.25, 0.25);
		glBegin(GL_QUADS);
		glVertex2f(-0.1 + movex, -0.5);
		glVertex2f(-0.2 + movex, -0.5);
		glVertex2f(-0.2 + movex, -0.8);
		glVertex2f(-0.1 + movex, -0.8);
		glEnd();

		glColor3f(0.25, 0.25, 0.25);
		glBegin(GL_QUADS);
		glVertex2f(0.1 + movex, -0.5);
		glVertex2f(0.2 + movex, -0.5);
		glVertex2f(0.2 + movex, -0.8);
		glVertex2f(0.1 + movex, -0.8);
		glEnd();
	}

	//Enemies
	for (int i = 0; i < enemiesCount; i++)
	{
		DrawEnemies();
		glTranslatef(0.2, 0, 1);
	}
}

static void error_callback(int error, const char* description)
{
	fputs(description, stderr);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	if (key == GLFW_KEY_ENTER && action == GLFW_PRESS)
		alive = false;

	if ((key == GLFW_KEY_RIGHT || key == GLFW_KEY_D) && alive)
		movex += 0.08;
	if ((key == GLFW_KEY_LEFT || key == GLFW_KEY_A) && alive)
		movex -= 0.08;
	if ((key == GLFW_KEY_UP || key == GLFW_KEY_SPACE || key == GLFW_KEY_W) && !shoot && alive)
		shoot = true;
}

int main()
{
	GLFWwindow* window;

	glfwSetErrorCallback(error_callback);

	if (!glfwInit())
		exit(EXIT_FAILURE);

	window = glfwCreateWindow(640, 480, "Space invaders", NULL, NULL);

	if (!window)
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	glfwSetKeyCallback(window, key_callback);

	SpawnEnemies();

	while (!glfwWindowShouldClose(window))
	{
		float ratio;
		int width, height;

		glfwGetFramebufferSize(window, &width, &height);
		glViewport(0, 0, width, height);

		glClearColor(0.1, 0.1, 0.1, 0);
		glClear(GL_COLOR_BUFFER_BIT);

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		if (width >= height) 
		{
			ratio = width / (float)height;
			gluOrtho2D(xMin*ratio, xMax*ratio, yMin, yMax);
		}
		else
		{
			ratio = height / (float)width;
			gluOrtho2D(xMin, xMax, yMin*ratio, yMax*ratio);
		}
		
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		DrawScene();
		Animation();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);

	glfwTerminate();
	exit(EXIT_SUCCESS);

	return 0;
}