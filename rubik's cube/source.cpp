#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
using namespace std;

#include "VAO.h"
#include "VBO.h"
#include "EBO.h"
#include "ShaderClass.h"


int zad1(int size, GLfloat*& vertices, GLuint*& indices) {
	int n_verticles_to_draw = 2 * 3;
	vertices = new GLfloat[4 * 5];
	indices = new GLuint[n_verticles_to_draw];

	GLfloat Tvertices[] = {
		-0.5f,-0.5f,1.0f,0.0f,0.0f,
		+0.5f,-0.5f,0.0f,1.0f,0.0f,
		-0.5f,+0.5f,0.0f,0.0f,1.0f,
		+0.5f,+0.5f,1.0f,0.0f,0.0f,
	};
	GLuint Tindices[] = {
		0,1,2,
		1,2,3
	};
	for (int i = 0; i < 20; i++)
		vertices[i] = Tvertices[i];
	for (int i = 0; i < 6; i++)
		indices[i] = Tindices[i];
	return n_verticles_to_draw; //returns number of verticles to draw
}
int zad4(int size, GLfloat*& vertices, GLuint*& indices) {
	int n_verticles_to_draw = 3 * size * size;
	vertices = new GLfloat[(size + 1) * (size + 1) * 5];
	indices = new GLuint[n_verticles_to_draw];

	GLfloat x, y, z = 0.;
	int iterator = 0;
	for (int i = 0; i <= size; i++) {
		for (int j = 0; j <= size; j++, iterator += 5) {
			x = (GLfloat)j * 2. / size - 1.;
			y = (GLfloat)i * 2. / size - 1.;
			vertices[iterator + 0] = x; //pos  x
			vertices[iterator + 1] = y; //pos  y
			vertices[iterator + 2] = 0.f; //rgb  r
			vertices[iterator + 3] = 1.f; //rgb  g
			vertices[iterator + 4] = 0.f; //rgb  b
		}
	}
	iterator = 0;
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			if ((j % 2 == 0 && i % 2 == 1) || (j % 2 == 1 && i % 2 == 0)) {
				indices[iterator + 0] = i * (size + 1) + j;
				indices[iterator + 1] = i * (size + 1) + j + 1;
				indices[iterator + 2] = i * (size + 1) + j + size + 2;

				indices[iterator + 3] = i * (size + 1) + j;
				indices[iterator + 4] = i * (size + 1) + j + size + 1;
				indices[iterator + 5] = i * (size + 1) + j + size + 2;
				iterator += 6;
			}
		}
	}
	return iterator; //returns number of verticles to draw
}

void drawIt(int n_verticles_to_draw, int mode = 0) {
	if (mode == 0) { //wszystko wypelnione
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glDrawElements(GL_TRIANGLES, n_verticles_to_draw, GL_UNSIGNED_INT, 0);
	}
	else if (mode == 1) { //polowa wypelniona, polowa linie
		for (int i = 0; i < n_verticles_to_draw; i+=6) {
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, (void*)(i * sizeof(float)));
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, (void*)((i+3) * sizeof(float)));
		}
	}
}

void colorUpdate(float r, float g, float b, int size, GLfloat*& vertices) {
	int iterator = 0;
	for (int i = 0; i <= size; i++) {
		for (int j = 0; j <= size; j++, iterator += 5) {
			vertices[iterator + 2] = r; //rgb  r
			vertices[iterator + 3] = g; //rgb  g
			vertices[iterator + 4] = b; //rgb  b
		}
	}
}
void colorUpdateV2(float r, float g, float b, int size, GLfloat*& vertices) {
	int iterator = 0;
	for (int i = 0; i <= size; i++) {
		for (int j = 0; j <= size; j++, iterator += 5) {
			if (i % 2 == 0) {
				vertices[iterator + 2] = g; //rgb  r
				vertices[iterator + 3] = b; //rgb  g
				vertices[iterator + 4] = r; //rgb  b
			}
			else {
				vertices[iterator + 2] = b; //rgb  r
				vertices[iterator + 3] = r; //rgb  g
				vertices[iterator + 4] = g; //rgb  b
			}
		}
	}
}

GLfloat* vertices = NULL;
GLuint* indices = NULL;
int main() {
	srand(time(NULL));
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(800, 800, "lab4", NULL, NULL);
	// Error check if the window fails to create
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" <<
			std::endl;
		glfwTerminate();
		return -1;
	}
	// Introduce the window into the current context
	glfwMakeContextCurrent(window);


	//Load GLAD so it configures OpenGL
	gladLoadGL();
	// Specify the viewport of OpenGL in the Window
	// In this case the viewport goes from x = 0, y = 0, to x = 800, y = 800
	glViewport(0, 0, 800, 800);


	// Utwórz obiekt Vertex Shader
	Shader shaderProgram("default.vert", "default.frag");

	VAO VAO1;
	VAO1.Bind();
	//
	int size = 7;
	int n_verticles_to_draw = 6;
	//n_verticles_to_draw = zad1(size, vertices, indices);
	n_verticles_to_draw = zad4(size, vertices, indices);

	//
	VBO VBO1(vertices, sizeof(GLfloat) * (size + 1) * (size + 1) * 5);
	EBO EBO1(indices, sizeof(GLuint) * n_verticles_to_draw);

	VAO1.LinkVBO(VBO1, 0, 1);
	VAO1.Unbind();

	
	float a = 0.383, b = 0.336, c = 0.449;
	glfwSwapInterval(1); //ograniczenie fps to synchronizacji vsync
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); //kontury
	int counter = 0;
	float xx = 0;
	VBO1.Bind(); //dynamiczna zmiana koloru
	while (!glfwWindowShouldClose(window))
	{
		VBO1.dynamicUpdate(vertices, sizeof(GLfloat) * (size + 1) * (size + 1) * 5);
		// Ustaw kolor t³a (RGBA, z przedzia³u <0, 1>)
		xx += 0.02;
		a = sin(xx) / 2 + 0.5;
		b = sin(-xx) / 2 + 0.5;
		c = sin(xx + 1.57) / 2 + 0.5;
		glClearColor(a, b, c, 1.0f);
		// color update
		colorUpdate(b, c, a, size, vertices);
		//colorUpdateV2(a, b, c, size, vertices);
		// Wyczyœæ buffor I nadaj mu kolor
		glClear(GL_COLOR_BUFFER_BIT);
		// Wybierz, który shader bêdzie u¿ywany
		shaderProgram.Activate();
		VAO1.Bind();
		// Narysuj trójk¹ty
		drawIt(n_verticles_to_draw,0);
		// Odœwie¿ widok
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	// Delete window before ending the program
	glfwDestroyWindow(window);
	// Terminate GLFW before ending the program
	glfwTerminate();

	VAO1.Delete();
	VBO1.Delete();
	EBO1.Delete();
	delete[] vertices;
	delete[] indices;

	return 0;
}