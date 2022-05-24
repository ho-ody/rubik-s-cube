
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
#include <vector>
#include "Block.h"

Block* GLOBALblocks;

ostream& operator<<(ostream& stream, const glm::vec3& v)
{
	stream << "(" << v.x << "," << v.y << "," << v.z << ")";
	return stream;
}

int N = 3;
float v = 1.;

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

int ccc = 0;
int rotateCounter = -1;
int ANIMATION_DURATION = 40;
int MOVEMENT_FREEZE_AFTER_MOVE = 15;

int* order;
int* neworder;
int axis;
int* toRotate;
int direction;
void orderUpdateRotateMatrix(bool direction)
{
	int N = 3;
	if ((direction == 0 && axis % 3 != 0) || (direction == 1 && axis % 3 == 0)) {
		for (int x = 0; x < N / 2; x++) {
			for (int y = x; y < N - x - 1; y++) {

				int temp = neworder[toRotate[x + N * y]];
				neworder[toRotate[x + N * y]] = neworder[toRotate[y + (N - 1 - x) * N]];
				neworder[toRotate[y + (N - 1 - x) * N]] = neworder[toRotate[(N - 1 - x) + (N - 1 - y) * N]];
				neworder[toRotate[(N - 1 - x) + (N - 1 - y) * N]] = neworder[toRotate[(N - 1 - y) + x * N]];
				neworder[toRotate[(N - 1 - y) + x * N]] = temp;
			}
		}
	}
	else {
		for (int x = 0; x < N / 2; x++) {
			for (int y = x; y < N - x - 1; y++) {

				int temp = neworder[toRotate[x + N * y]];
				neworder[toRotate[x + N * y]] = neworder[toRotate[(N - 1 - y) + x * N]];
				neworder[toRotate[(N - 1 - y) + x * N]] = neworder[toRotate[(N - 1 - x) + (N - 1 - y) * N]];
				neworder[toRotate[(N - 1 - x) + (N - 1 - y) * N]] = neworder[toRotate[y + (N - 1 - x) * N]];
				neworder[toRotate[y + (N - 1 - x) * N]] = temp;
			}
		}
	}
}
int rotateIndex[9][9] = {
	{0,3,6,9,12,15,18,21,24},	 //x 
	{0,1,2,9,10,11,18,19,20},	 //y
	{0,1,2,3,4,5,6,7,8},		 //z
	{1,4,7,10,13,16,19,22,25},	 //x+1
	{3,4,5,12,13,14,21,22,23},	 //y+1
	{9,10,11,12,13,14,15,16,17}, //z+1
	{2,5,8,11,14,17,20,23,26},	 //x+2
	{6,7,8,15,16,17,24,25,26},	 //y+2
	{18,19,20,21,22,23,24,25,26} //z+2
};

int indexesOfRotationX(int j, int offset) {
	return order[3 * j + offset];
}
int indexesOfRotationY(int j, int offset) {
	return order[j % 3 + j / 3 * 9 + offset*3];
}
int indexesOfRotationZ(int j, int offset) {
	return order[j + offset*9];
}
void rotate(int direction, int indexsOfRotation(int,int), int offset) {
	float a, b;
	rotateCounter = ANIMATION_DURATION;
	toRotate = rotateIndex[axis];
	axis %= 3;
	for (int i = 0, j = 0; j < N*N; j++) {
		i = indexsOfRotation(j,offset);
		GLOBALblocks[i].roll = true;

		if (axis == 0) {
			a = GLOBALblocks[i].position.z;
			b = GLOBALblocks[i].position.y;
		}
		else if (axis == 1) {
			a = GLOBALblocks[i].position.x;
			b = GLOBALblocks[i].position.z;
		}
		else {
			a = GLOBALblocks[i].position.x;
			b = GLOBALblocks[i].position.y;
		}

		if (j % 9 == 4)
			GLOBALblocks[i].blockOffsetFix = -1;
		else if (j % 2 == 0) {
			if (a < 2*v && b < 2*v)
				GLOBALblocks[i].blockOffsetFix = 1 + direction;
			else if (a > 2*v && b < 2*v)
				GLOBALblocks[i].blockOffsetFix = 0 + direction;
			else if (a > 2*v && b > 2*v)
				GLOBALblocks[i].blockOffsetFix = 3 + direction;
			else
				GLOBALblocks[i].blockOffsetFix = 2 + direction;
		}
		else {
			if (a > 0.5*2*v && a < 1.5*2*v)
				if (b > 2*v)
					GLOBALblocks[i].blockOffsetFix = 3 + direction;
				else
					GLOBALblocks[i].blockOffsetFix = 1 + direction;
			else if (a < 2*v)
				GLOBALblocks[i].blockOffsetFix = 2 + direction;
			else
				GLOBALblocks[i].blockOffsetFix = 0 + direction;
		}
		if (direction) {
			GLOBALblocks[i].rot[axis]++;
			if (GLOBALblocks[i].rot[axis] == 4) {
				GLOBALblocks[i].rot[axis] = 0;
				GLOBALblocks[i].prevRot[axis] = -1;
			}
		}
		else {
			GLOBALblocks[i].rot[axis]--;
			if (GLOBALblocks[i].rot[axis] == -1) {
				GLOBALblocks[i].rot[axis] = 3;
				GLOBALblocks[i].prevRot[axis] = 4;
			}
		}
	}
	orderUpdateRotateMatrix(direction);
}
// 0 -> waiting for input (M key), 1 -> waiting for move (FBLRUD or Escape), 2 -> input processed, reseting
int move_cube = 0;
int code_input = 0; 
int code_input_index = 0;
string code_s = "";
void input(GLFWwindow* window, glm::vec3& Position, glm::vec3& Orientation, glm::vec3& Up)
{
	Orientation = cameraFront;

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)          // sprawdzanie czy wybrany klawisz jest wciœniêty (Esc)     GLFW_PRESS | GLFW_RELEASE
		glfwSetWindowShouldClose(window, true);                     // zamykanie okienka

	if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS) { //move
		if (move_cube == 0) {
			move_cube = 1;
		}
	}
	if (glfwGetKey(window, GLFW_KEY_M) == GLFW_RELEASE) {
		if (move_cube==2)
			move_cube = 0;
	}
	if (move_cube == 1 && rotateCounter < 0) {
		if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
			direction = 1;
		else
			direction = 0;
		
		if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) {
			axis = 2;
			rotate(direction, indexesOfRotationZ,0);
			move_cube = 2;
		}
		if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
			if (direction) direction = 0; else direction = 1; //fix clockwise-anticlockwise
			axis = 0;
			rotate(direction, indexesOfRotationX,0);
			move_cube = 2;
		}
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
			if (direction) direction = 0; else direction = 1; //fix clockwise-anticlockwise
			axis = 1;
			rotate(direction, indexesOfRotationY,0);
			move_cube = 2;
		}
		if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS) {
			if (direction) direction = 0; else direction = 1; //fix clockwise-anticlockwise
			axis = 8;
			rotate(direction, indexesOfRotationZ,2);
			move_cube = 2;
		}
		if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) {
			axis = 6;
			rotate(direction, indexesOfRotationX,2);
			move_cube = 2;
		}
		if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS) {
			axis = 7;
			rotate(direction, indexesOfRotationY,2);
			move_cube = 2;
		}
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
			move_cube = 2;
		}
	}
	else if (ANIMATION_DURATION - MOVEMENT_FREEZE_AFTER_MOVE > rotateCounter) {
		const float cameraSpeed = 12.0 * deltaTime; // adjust accordingly
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
	if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) { //code input
		if (code_input == 0) {
			code_input = 1;
		}
	}
	if (glfwGetKey(window, GLFW_KEY_C) == GLFW_RELEASE) {
		if (code_input == 2)
			code_input = 0;
	}
	if (code_input == 1) {
		getline(cin, code_s);
		for (int i = 0; i <= code_s.length(); i++) {
			if (code_s[i] == '\'') {	// ' zamiana na du¿¹ literê
				i--;
				char t = code_s[i] - 'Z' + 'z';
				code_s.erase(i, 2);
				code_s.insert(i, string(1, t));
			}
			if (code_s[i] == ' ') {		// usuniecie spacji
				code_s.erase(i, 1);
			}		
			if (code_s[i] == '2') {		// zastapienie znaku 2
				code_s.erase(i, 1);
				code_s.insert(i, string(1, code_s[i-1]));
			}
		}
		for (int i = 0; i <= code_s.length(); i++) {
			if (code_s[i] >= 'a' && code_s[i] <= 'z') { //zamiana duzych liter na male i na odwrót
				char t = code_s[i] + 'Z' - 'z';
				code_s.erase(i, 1);
				code_s.insert(i, string(1, t));
			}
			else if (code_s[i] >= 'A' && code_s[i] <= 'Z') {
				char t = code_s[i] - 'Z' + 'z';
				code_s.erase(i, 1);
				code_s.insert(i, string(1, t));
			}
		}
		cerr << code_s;
		code_input = 2;
		code_input_index = 0;
	}
	if (code_input_index < code_s.length() && rotateCounter < 0) {
		direction = 0;
		switch (code_s[code_input_index]) {
		case 'F':
			direction = 1;
		case 'f':
			axis = 2;
			rotate(direction, indexesOfRotationZ, 0);
			break;
		case 'r': //fix -> zamiana 'R' z 'r'
			direction = 1;
		case 'R':
			axis = 0;
			rotate(direction, indexesOfRotationX, 0);
			break;
		case 'd':
			direction = 1;
		case 'D':
			axis = 1;
			rotate(direction, indexesOfRotationY, 0);
			break;
		case 'b':
			direction = 1;
		case 'B':
			axis = 8;
			rotate(direction, indexesOfRotationZ, 2);
			break;
		case 'L':
			direction = 1;
		case 'l':
			axis = 6;
			rotate(direction, indexesOfRotationX, 2);
			break;
		case 'U':
			direction = 1;
		case 'u':
			axis = 7;
			rotate(direction, indexesOfRotationY, 2);
			break;
		}
		code_input_index++;
	}
}
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse) {
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}
	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;
	lastX = xpos;
	lastY = ypos;

	float sensitivity = 0.25f;
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
	glEnable(GL_DEPTH_TEST); //test bufora g³êbokoœci - przenikanie
	// Utwórz obiekt Vertex Shader
	Shader shaderProgram("color_uniform.vert", "default.frag");
	// VAO
	v = 0.95;
	GLfloat vertices[] = {
		-v, -v, -v,
		+v, -v, -v,

		+v, +v, -v,
		-v, +v, -v,

		-v, -v, +v,
		+v, -v, +v,

		+v, +v, +v,
		-v, +v, +v
	};
	v = 1.0;
	int n_vertices = 4 * 2 * 3;
	GLuint indices[] = {
		0, 1, 3, 3, 1, 2,
		1, 5, 2, 2, 5, 6,
		5, 4, 6, 6, 4, 7,
		4, 0, 7, 7, 0, 3,
		3, 2, 7, 7, 2, 6,
		4, 5, 0, 0, 5, 1
	};
	int n_indices = 6 * 6;
	
	VBO blockVBO(vertices, sizeof(GLfloat) * n_vertices);
	EBO blockEBO(indices, sizeof(GLuint) * n_indices);
	VAO blockVAO;
	blockVAO.Bind();
	blockVAO.LinkVBO(blockVBO, 0, 1);
	blockVAO.Unbind();
	
	order = new int[pow(N, 3)];
	neworder = new int[pow(N, 3)];
	toRotate = new int[pow(N, 2)];

	for (int i = 0; i < pow(N, 3); i++) {
		order[i] = i;
		neworder[i] = i;
	}
	
	Block* blocks;
	blocks = new Block[pow(N, 3)];
	GLOBALblocks = blocks;
	int n_blocks = pow(N, 3);

	for (int i = 0; i < n_blocks; i++) {
		blocks[i].position = glm::vec3(i % N, (i / N)% N, i / (N*N)) * glm::vec3(2*v,2*v,2*v);
		if (i < 1*N*N) { //first side - red
			blocks[i].color[0] = glm::vec3(0.9, 0.1, 0.1); //red
			if (i % N == N-1) blocks[i].color[1] = glm::vec3(0.9, 0.9, 0.9); //white
			if (i % N == 0) blocks[i].color[3] = glm::vec3(0.9, 0.9, 0.1); //yellow
			if (i % (N*N) >= N*(N-1)) blocks[i].color[4] = glm::vec3(0.1, 0.1, 0.9); //blue
			if (i % (N*N) < N) blocks[i].color[5] = glm::vec3(0.1, 0.9, 0.1); //green
		}
		else if (i < N * N* (N-1)) {
			if (i % N == N-1) blocks[i].color[1] = glm::vec3(0.9, 0.9, 0.9); //white
			if (i % N == 0) blocks[i].color[3] = glm::vec3(0.9, 0.9, 0.1); //yellow
			if (i % (N*N) >= N * (N - 1)) blocks[i].color[4] = glm::vec3(0.1, 0.1, 0.9); //blue
			if (i % (N*N) < N) blocks[i].color[5] = glm::vec3(0.1, 0.9, 0.1); //green
		}
		else {
			blocks[i].color[2] = glm::vec3(0.9, 0.5, 0.1); //orange
			if (i % N == N-1) blocks[i].color[1] = glm::vec3(0.9, 0.9, 0.9); //white
			if (i % N == 0) blocks[i].color[3] = glm::vec3(0.9, 0.9, 0.1); //yellow
			if (i % (N*N) >= N * (N - 1)) blocks[i].color[4] = glm::vec3(0.1, 0.1, 0.9); //blue
			if (i % (N*N) < N) blocks[i].color[5] = glm::vec3(0.1, 0.9, 0.1); //green
		}
		/*
			blocks[i].color[0] = glm::vec3(0.9, 0.1, 0.1); //red
			blocks[i].color[1] = glm::vec3(0.9, 0.9, 0.9); //white
			blocks[i].color[2] = glm::vec3(0.9, 0.5, 0.1); //orange
			blocks[i].color[3] = glm::vec3(0.9, 0.9, 0.1); //yellow
			blocks[i].color[4] = glm::vec3(0.1, 0.1, 0.9); //blue
			blocks[i].color[5] = glm::vec3(0.1, 0.9, 0.1); //green
		*/
	}

	float backgroud_r, backgroud_g, backgroud_b;
	glfwSwapInterval(1); //ograniczenie fps to synchronizacji vsync
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); kontury
	glLineWidth(3);
	float time = 0;
	Camera camera(width, height);
	while (!glfwWindowShouldClose(window))
	{
		//ROTATION
		if (rotateCounter >= 0) {
			float time = rotateCounter * M_PI / 2. / ANIMATION_DURATION;

			for (int j = 0; j < n_blocks; j++) {
				if (blocks[j].roll)
					blocks[j].rotate((j + 1) % 2, axis, time, direction);
			}
			if (rotateCounter == 0) {
				//roll reset && block update
				for (int j = 0; j < 3 * 3 * 3; j++) {
					if (blocks[j].roll) {
						blocks[j].roll = false;
						blocks[j].update(direction);
					}
				}				
				//order rotation
				for (int j = 0; j < n_blocks; j++)
					order[j] = neworder[j];
			}
			rotateCounter--;
		}
		//time logic
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		//input
		input(window, camera.Position, camera.Orientation, camera.Up);
		//logika koloru t³a
		time += deltaTime;
		//time += 0.003;
		backgroud_r = sin(time) / 2 + 0.5;
		backgroud_g = sin(-time) / 2 + 0.5;
		backgroud_b = sin(time + 1.57) / 2 + 0.5;
		glClearColor(backgroud_r, backgroud_g, backgroud_b, 1.0f);
		// Wyczyœæ buffor I nadaj mu kolor wraz z buforem g³êbokoœci
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// Wybierz, który shader bêdzie u¿ywany
		shaderProgram.Activate();
		camera.Matrix(45.0f, 0.5f, 40.0f, shaderProgram, "camMatrix");
		// Narysuj trójk¹ty
		blockVAO.Bind();
		for (int i = 0; i < n_blocks; i++)
		{
			glm::mat4 model = glm::mat4(1.0f);
			//transformacje konkretnego modelu - odpowiednia rotacja
			model = glm::translate(model, blocks[i].position);

			if (blocks[i].rotation.x)
				model *= glm::rotate(glm::mat4(1.0f), blocks[i].rotation.x, glm::vec3(1, 0, 0));
			if (blocks[i].rotation.y)
				model *= glm::rotate(glm::mat4(1.0f), blocks[i].rotation.y, glm::vec3(0, 1, 0));
			if (blocks[i].rotation.z)
				model *= glm::rotate(glm::mat4(1.0f), blocks[i].rotation.z, glm::vec3(0, 0, 1));

			//przekazanie modelu
			int modelLoc = glGetUniformLocation(shaderProgram.ID, "model");
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			//przekazanie koloru
			int colorLoc = glGetUniformLocation(shaderProgram.ID, "color");
			glUniform3f(colorLoc, blocks[i].color[0].x, blocks[i].color[0].y, blocks[i].color[0].z);
			//rysowanie konkretnego kwadratu (2 trójk¹tów)
			blockEBO.Bind();
			//glDrawElements(GL_TRIANGLES, n_indices, GL_UNSIGNED_INT, 0);

			for (int j = 0; j < 6; j++) {
				//cerr << (j + 1) * n_indices / 6 << endl;
				glUniform3f(colorLoc, blocks[i].color[j].x, blocks[i].color[j].y, blocks[i].color[j].z);
				//if (blocks[i].color[j].x > 0.05 && blocks[i].color[j].x < 0.15 && blocks[i].color[j].y > 0.05 && blocks[i].color[j].y < 0.15 && blocks[i].color[j].z > 0.05 && blocks[i].color[j].z < 0.15);
				//else
				glDrawElements(GL_TRIANGLES, 2*1*3, GL_UNSIGNED_INT, (void*)(6 * j * sizeof (GLfloat)));
			}
		}
		// Odœwie¿ widok
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	
	glfwDestroyWindow(window); //Delete window before ending the program
	glfwTerminate(); //Terminate GLFW before ending the program

	blockVAO.Delete();
	blockVBO.Delete();
	blockEBO.Delete();
	delete[]order, neworder, toRotate, blocks;
	return 0;
}