#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
using namespace std;

#include "VAO.h"
#include "VBO.h"
#include "EBO.h"
#include "ShaderClass.h"
#include "Camera.h"
#include "Tile.h"


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


bool firstMouse = true;
float yaw = -90.0f;	// yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
float pitch = 0.0f;
float lastX = 800.0f / 2.0;
float lastY = 600.0 / 2.0;
float fov = 45.0f;
glm::vec3 cameraFront;
void framebuffer_size_callback(GLFWwindow* window, int width, int height) // uruchamiane po zmianie rozmiaru okienka
{
	glViewport(0, 0, width, height);
}
void input(GLFWwindow* window, glm::vec3& Position, glm::vec3& Orientation, glm::vec3& Up)                                      // input
{
	Orientation = cameraFront;

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)          // sprawdzanie czy wybrany klawisz jest wciœniêty (Esc)     GLFW_PRESS | GLFW_RELEASE
		glfwSetWindowShouldClose(window, true);                     // zamykanie okienka

	const float cameraSpeed = 0.05f; // adjust accordingly
	/*
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		Position.z += cameraSpeed;
		//Orientation.z += cameraSpeed;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		Position.z -= cameraSpeed;
		//Orientation.z -= cameraSpeed;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		Position.x -= cameraSpeed;
		//Orientation.x -= cameraSpeed;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		Position.x += cameraSpeed;
		//Orientation.x += cameraSpeed;
	}
	if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) {
		Position.y -= cameraSpeed;
		//Orientation.y -= cameraSpeed;
	}
	if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) {
		Position.y += cameraSpeed;
		//Orientation.y += cameraSpeed;
	}
	*/
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		Position += cameraSpeed * Orientation;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		Position -= cameraSpeed * Orientation;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		Position -= glm::normalize(glm::cross(Orientation, Up)) * cameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		Position += glm::normalize(glm::cross(Orientation, Up)) * cameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		Position += cameraSpeed * Up;
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		Position -= cameraSpeed * Up;
}
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;
	lastX = xpos;
	lastY = ypos;

	float sensitivity = 0.2f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	glm::vec3 direction;
	direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	direction.y = sin(glm::radians(pitch));
	direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront = glm::normalize(direction);
}
//GLfloat* vertices = NULL;
//GLuint* indices = NULL;
int width = 800, height = 800;
int main() {
	srand(time(NULL));
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(800, 800, "glhf", NULL, NULL);
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
	glViewport(0, 0, width, height);

	//klawiatura, mysz 
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); //capture mouse

	//
	glEnable(GL_DEPTH_TEST);
	// Utwórz obiekt Vertex Shader
	Shader shaderProgram("color_uniform.vert", "default.frag");

	VAO VAO1;
	VAO1.Bind();
	//
	int size = 7;
	int n_verticles_to_draw = 6;
	//n_verticles_to_draw = zad1(size, vertices, indices);
	//n_verticles_to_draw = zad4(size, vertices, indices);


	int n_vertices = 4 * 6;
	GLfloat vertices[] = {
		//x,y,z,r,g,b
		-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
		+0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
		-0.5f, +0.5f, 0.0f, 0.0f, 0.0f, 1.0f,
		+0.5f, +0.5f, 0.0f, 1.0f, 0.0f, 0.0f
	};
	int n_indices = 2 * 3;
	GLuint indices[] = {
		0,1,2,
		1,2,3
	};

	Tile tiles[9];
	for (int i = 0; i < 9; i++) {
		tiles[i].color = glm::vec3((rand() % 100 / 100.), (rand() % 100 / 100.), (rand() % 100 / 100.));
		tiles[i].position = glm::vec3(i, 0.0, 0.0);
		tiles[i].rotation = glm::vec3(M_PI/9*i, 0.0, 0.0);
	}

	/*
	glm::vec3 positions[] = {
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
	};
	*/
	glm::vec3 positions[] = {
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
	};



	n_verticles_to_draw = n_indices;
	//
	VBO VBO1(vertices, sizeof(GLfloat) * n_vertices);
	EBO EBO1(indices, sizeof(GLuint) * n_indices);

	VAO1.LinkVBO(VBO1, 0, 1);
	VAO1.Unbind();

	
	float a = 0.383, b = 0.336, c = 0.449;
	glfwSwapInterval(1); //ograniczenie fps to synchronizacji vsync
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); //kontury
	int counter = 0;
	float xx = 0;
	VBO1.Bind(); //dynamiczna zmiana koloru
	Camera camera(width, height);
	while (!glfwWindowShouldClose(window))
	{
		input(window, camera.Position, camera.Orientation, camera.Up);
		//VBO1.dynamicUpdate(vertices, sizeof(GLfloat) * (size + 1) * (size + 1) * 5);
		// Ustaw kolor t³a (RGBA, z przedzia³u <0, 1>)
		xx += 0.02;
		a = sin(xx) / 2 + 0.5;
		b = sin(-xx) / 2 + 0.5;
		c = sin(xx + 1.57) / 2 + 0.5;
		glClearColor(a, b, c, 1.0f);
		// color update
		//colorUpdate(b, c, a, size, vertices);
		//colorUpdateV2(a, b, c, size, vertices);
		// Wyczyœæ buffor I nadaj mu kolor
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// Wybierz, który shader bêdzie u¿ywany
		shaderProgram.Activate();
		camera.Matrix(45.0f, 0.5f, 20.0f, shaderProgram, "camMatrix");

		VAO1.Bind();
		// Narysuj trójk¹ty
		//drawIt(n_verticles_to_draw,0);
		/*
		for (int i = 0; i < 10; i++)
		{
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, positions[i]);
			float angle = 20.0f * i;
			model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			//shaderProgram.setMat4("model", model);
			//glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "model"), 1, GL_FALSE, glm::value_ptr(model));
			int modelLoc = glGetUniformLocation(shaderProgram.ID, "model");
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

			//glDrawArrays(GL_TRIANGLES, 0, 36);
			//glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, (void*)((i+6) * sizeof(float)));
			//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)((6*i)*sizeof(GLfloat)));
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		}
		*/
		for (int i = 0; i < 9; i++)
		{
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, tiles[i].position);
			float angle = 20.0f * i;
			model *= glm::rotate(glm::mat4(1.0f), tiles[i].rotation.x, glm::vec3(1, 0, 0));
			model *= glm::rotate(glm::mat4(1.0f), tiles[i].rotation.y, glm::vec3(0, 1, 0));
			model *= glm::rotate(glm::mat4(1.0f), tiles[i].rotation.z, glm::vec3(0, 0, 1));
			//modelMatrix = translateMatrix * rotateZ * rotateY * rotateX * scaleMatrix;
			//model *= rotateX * rotateY * rotateZ;


			//shaderProgram.setMat4("model", model);
			//glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "model"), 1, GL_FALSE, glm::value_ptr(model));
			int modelLoc = glGetUniformLocation(shaderProgram.ID, "model");
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

			int colorLoc = glGetUniformLocation(shaderProgram.ID, "color");
			glUniform3f(colorLoc, tiles[i].color.x, tiles[i].color.y, tiles[i].color.z);
			//glDrawArrays(GL_TRIANGLES, 0, 36);
			//glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, (void*)((i+6) * sizeof(float)));
			//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)((6*i)*sizeof(GLfloat)));
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		}
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
	//delete[] vertices;
	//delete[] indices;

	return 0;
}