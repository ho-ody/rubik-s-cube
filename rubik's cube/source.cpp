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

Tile* GLOBALtiles;

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


bool firstMouse = true;
float yaw = -90.0f;	// yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
float pitch = 0.0f;
float lastX = 800.0f / 2.0;
float lastY = 600.0 / 2.0;
float fov = 45.0f;
glm::vec3 cameraFront;
float deltaTime = 0., lastFrame = 0.;
void framebuffer_size_callback(GLFWwindow* window, int width, int height) // uruchamiane po zmianie rozmiaru okienka
{
	glViewport(0, 0, width, height);
}


int rotateCounter = 0;
int ANIMATION_DURATION = 150;
void rotate(int side) {
	rotateCounter = ANIMATION_DURATION;
	for (int i = side * 9; i < (side + 1) * 9; i++) {
		GLOBALtiles[i].rotate = true;
		if (i % 2)
			GLOBALtiles[i].genPositionOffset(true);
		else
			GLOBALtiles[i].genPositionOffset(false);
	}	
}


void input(GLFWwindow* window, glm::vec3& Position, glm::vec3& Orientation, glm::vec3& Up)                                      // input
{
	Orientation = cameraFront;

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)          // sprawdzanie czy wybrany klawisz jest wci�ni�ty (Esc)     GLFW_PRESS | GLFW_RELEASE
		glfwSetWindowShouldClose(window, true);                     // zamykanie okienka

	const float cameraSpeed = 4.00f * deltaTime; // adjust accordingly
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

	if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) {
		rotate(1);
	}
	if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS) {
		rotate(0);
	}
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
	glfwMakeContextCurrent(window); //Introduce the window into the current context
	gladLoadGL(); //Load GLAD so it configures OpenGL
	glViewport(0, 0, width, height); //Specify the viewport of OpenGL in the Window - from x = 0, y = 0, to x = width, y = height

	//input - klawiatura, mysz 
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); //capture mouse
	//
	glEnable(GL_DEPTH_TEST); //test bufora g��boko�ci - przenikanie
	// Utw�rz obiekt Vertex Shader
	Shader shaderProgram("color_uniform.vert", "default.frag");
	// VAO
	VAO VAO1;
	VAO1.Bind();
	//
	int n_vertices = 4 * 3;
	GLfloat vertices[] = {
		//x    y      z
		-0.5f, -0.5f, 0.0f,
		+0.5f, -0.5f, 0.0f,
		-0.5f, +0.5f, 0.0f,
		+0.5f, +0.5f, 0.0f
	};
	int n_indices = 2 * 3;
	GLuint indices[] = {
		0,1,2,
		1,2,3
	};
	int n_lineIndices = 4 * 2;
	GLuint lineIndices[] = {
		0,1,
		0,2,
		3,1,
		3,2
	};

	Tile tiles[6*9];
	GLOBALtiles = tiles;
	int n_tiles = 6*9;
	int j = 0;
	for (int i = 0; j < 1 * 9; j++, i++) { //front side - red
		tiles[j].color = glm::vec3(0.9,0.1,0.1);
		tiles[j].position = glm::vec3(i%3, i/3, 0.0);
		tiles[j].rotation = glm::vec3(0.0, 0.0, 0.0);
		if (i != 4) tiles[j].side = j / 9;
	}
	for (int i = 0; j < 2 * 9; j++, i++) { //back side - orange
		tiles[j].color = glm::vec3(0.9, 0.5, 0.1);
		tiles[j].position = glm::vec3(i % 3, i / 3, -3.0);
		tiles[j].rotation = glm::vec3(0.0, 0.0, 0.0);
		if (i != 4) tiles[j].side = j / 9;
	}
	for (int i = 0; j < 3 * 9; j++, i++) { //bottom side - green
		tiles[j].color = glm::vec3(0.1, 0.9, 0.1);
		tiles[j].position = glm::vec3(i / 3, -0.5, i % 3 - 2.5);
		tiles[j].rotation = glm::vec3(M_PI/2.0, 0.0, 0.0);
		if (i != 4) tiles[j].side = j / 9;
	}
	for (int i = 0; j < 4 * 9; j++, i++) { //top side - blue
		tiles[j].color = glm::vec3(0.1, 0.1, 0.9);
		tiles[j].position = glm::vec3(i / 3, 2.5, i % 3 - 2.5);
		tiles[j].rotation = glm::vec3(M_PI / 2.0, 0.0, 0.0);
		if (i != 4) tiles[j].side = j / 9;
	}
	for (int i = 0; j < 5 * 9; j++, i++) { //left side - white
		tiles[j].color = glm::vec3(0.9, 0.9, 0.9);
		tiles[j].position = glm::vec3(-0.5, i/3, i % 3 - 2.5);
		tiles[j].rotation = glm::vec3(0.0, M_PI / 2.0, 0.0);
		if (i != 4) tiles[j].side = j / 9;
	}
	for (int i = 0; j < 6 * 9; j++, i++) { //right side - yellow
		tiles[j].color = glm::vec3(0.9, 0.9, 0.1);
		tiles[j].position = glm::vec3(2.5, i / 3, i % 3 - 2.5);
		tiles[j].rotation = glm::vec3(0.0, M_PI / 2.0, 0.0);
		if (i != 4) tiles[j].side = j / 9;
	}
	//
	VBO VBO1(vertices, sizeof(GLfloat) * n_vertices);
	EBO EBO1(indices, sizeof(GLuint) * n_indices);
	EBO EBO2(lineIndices, sizeof(GLuint) * n_lineIndices);

	VAO1.LinkVBO(VBO1, 0, 1);
	VAO1.Unbind();

	float backgroud_r, backgroud_g, backgroud_b;
	glfwSwapInterval(1); //ograniczenie fps to synchronizacji vsync
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); //kontury
	glLineWidth(3);
	float time = 0;
	VBO1.Bind(); //dynamiczna zmiana koloru
	Camera camera(width, height);
	while (!glfwWindowShouldClose(window))
	{
		if (rotateCounter >= 0) { 
			float multi = rotateCounter * M_PI / 2. / ANIMATION_DURATION;
			for (int i = 0; i < n_tiles; i++) {
				if (tiles[i].rotate == true) {
					if ((i % 2 && !(tiles[i].side % 2)) || (!(i % 2) && tiles[i].side % 2))
						tiles[i].genRotationPosition(multi, 1.);
					else
						tiles[i].genRotationPosition(multi, sqrt(2));
				}
			}
			if (rotateCounter == 0)
				for (int i = 0; i < n_tiles; i++)
					tiles[i].rotate = false;
			rotateCounter--;
		}



		/*
		if (rotateCounter > 0) {
			if (rotateCounter == ANIMATION_DURATION) {
				tiles[1].posOffset = genPosOffsetCross(tiles[1].position.x, tiles[1].position.y);
				tiles[3].posOffset = genPosOffsetCross(tiles[3].position.x, tiles[3].position.y);
				tiles[5].posOffset = genPosOffsetCross(tiles[5].position.x, tiles[5].position.y);
				tiles[7].posOffset = genPosOffsetCross(tiles[7].position.x, tiles[7].position.y);

				tiles[0].posOffset = genPosOffsetDiag(tiles[0].position.x, tiles[0].position.y);
				tiles[2].posOffset = genPosOffsetDiag(tiles[2].position.x, tiles[2].position.y);
				tiles[6].posOffset = genPosOffsetDiag(tiles[6].position.x, tiles[6].position.y);
				tiles[8].posOffset = genPosOffsetDiag(tiles[8].position.x, tiles[8].position.y);
			}
			
			
			rotateCounter--;

			float multi = rotateCounter * M_PI /2. / ANIMATION_DURATION;

			tiles[1].position.x = sin(multi + tiles[1].posOffset) + 1;
			tiles[1].position.y = cos(multi + tiles[1].posOffset) + 1;

			tiles[3].position.x = sin(multi + tiles[3].posOffset) + 1;
			tiles[3].position.y = cos(multi + tiles[3].posOffset) + 1;

			tiles[5].position.x = sin(multi + tiles[5].posOffset) + 1;
			tiles[5].position.y = cos(multi + tiles[5].posOffset) + 1;

			tiles[7].position.x = sin(multi + tiles[7].posOffset) + 1;
			tiles[7].position.y = cos(multi + tiles[7].posOffset) + 1;

			tiles[1].rotation.z = -multi;
			tiles[3].rotation.z = -multi;
			tiles[5].rotation.z = -multi;
			tiles[7].rotation.z = -multi;

			tiles[0].position.x = sqrt(2) * sin(multi + tiles[0].posOffset) + 1;
			tiles[0].position.y = sqrt(2) * cos(multi + tiles[0].posOffset) + 1;

			tiles[2].position.x = sqrt(2) * sin(multi + tiles[2].posOffset) + 1;
			tiles[2].position.y = sqrt(2) * cos(multi + tiles[2].posOffset) + 1;

			tiles[6].position.x = sqrt(2) * sin(multi + tiles[6].posOffset) + 1;
			tiles[6].position.y = sqrt(2) * cos(multi + tiles[6].posOffset) + 1;

			tiles[8].position.x = sqrt(2) * sin(multi + tiles[8].posOffset) + 1;
			tiles[8].position.y = sqrt(2) * cos(multi + tiles[8].posOffset) + 1;

			tiles[0].rotation.z = -multi;
			tiles[2].rotation.z = -multi;
			tiles[6].rotation.z = -multi;
			tiles[8].rotation.z = -multi;

			tiles[4].rotation.z = -multi;
		}
		*/
		//time logic
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		//cerr << "time= " << time << "\tdelta= " << deltaTime << "\tc= " << counter << endl;
		input(window, camera.Position, camera.Orientation, camera.Up);
		//logika koloru t�a
		time += deltaTime;
		//time += 0.003;
		backgroud_r = sin(time) / 2 + 0.5;
		backgroud_g = sin(-time) / 2 + 0.5;
		backgroud_b = sin(time + 1.57) / 2 + 0.5;
		glClearColor(backgroud_r, backgroud_g, backgroud_b, 1.0f);
		// Wyczy�� buffor I nadaj mu kolor wraz z buforem g��boko�ci
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// Wybierz, kt�ry shader b�dzie u�ywany
		shaderProgram.Activate();
		camera.Matrix(45.0f, 0.5f, 20.0f, shaderProgram, "camMatrix");

		VAO1.Bind();
		// Narysuj tr�jk�ty
		for (int i = 0; i < n_tiles; i++)
		{
			//model
			glm::mat4 model = glm::mat4(1.0f);
			//transformacje konkretnego modelu - odpowiednia rotacja
			model = glm::translate(model, tiles[i].position);
			if (tiles[i].rotation.x)
				model *= glm::rotate(glm::mat4(1.0f), tiles[i].rotation.x, glm::vec3(1, 0, 0));
			if (tiles[i].rotation.y)
				model *= glm::rotate(glm::mat4(1.0f), tiles[i].rotation.y, glm::vec3(0, 1, 0));
			if (tiles[i].rotation.z)
				model *= glm::rotate(glm::mat4(1.0f), tiles[i].rotation.z, glm::vec3(0, 0, 1));
			//przekazanie modelu
			int modelLoc = glGetUniformLocation(shaderProgram.ID, "model");
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			//przekazanie koloru
			int colorLoc = glGetUniformLocation(shaderProgram.ID, "color");
			glUniform3f(colorLoc, tiles[i].color.x, tiles[i].color.y, tiles[i].color.z);
			//rysowanie konkretnego kwadratu (2 tr�jk�t�w)
			EBO1.Bind();
			glDrawElements(GL_TRIANGLES, n_indices, GL_UNSIGNED_INT, 0);
			EBO2.Bind();
			glUniform3f(colorLoc, 0.1, 0.1, 0.1);
			glDrawElements(GL_LINES, 4, GL_UNSIGNED_INT, 0);
		}
		// Od�wie� widok
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	
	glfwDestroyWindow(window); //Delete window before ending the program
	glfwTerminate(); //Terminate GLFW before ending the program

	VAO1.Delete();
	VBO1.Delete();
	EBO1.Delete();
	//delete[] vertices;
	//delete[] indices;
	return 0;
}