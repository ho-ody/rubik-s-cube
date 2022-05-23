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
#include <vector>
#include "Block.h"

Tile* GLOBALtiles;
Block* GLOBALblocks;

ostream& operator<<(ostream& stream, const glm::vec3& v)
{
	stream << "(" << v.x << "," << v.y << "," << v.z << ")";
	return stream;
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
int ANIMATION_DURATION = 70;

int* order;
int* neworder;

void rotate(int side) {
	rotateCounter = ANIMATION_DURATION;
	for (int i = side * 9; i < (side + 1) * 9; i++) {
		GLOBALtiles[order[i]].rotate = 1;
		//cerr << order[i] << " - ";
		if (order[i] % 2)
			GLOBALtiles[order[i]].genPositionOffset(true);
		else
			GLOBALtiles[order[i]].genPositionOffset(false);
	}	
	//cerr << order[1] << "\t" << GLOBALtiles[order[1]].placeOfBlock << "\t" << GLOBALtiles[order[1]].posOffset << "\t" << GLOBALtiles[order[1]].rotate << "\t" << GLOBALtiles[order[1]].side << "\t" << GLOBALtiles[order[1]].vao << "\t" << GLOBALtiles[order[1]].position.x << "\t" << GLOBALtiles[order[1]].position.y << "\t" << GLOBALtiles[order[1]].position.z << "\t" << GLOBALtiles[order[1]].rotation.x << "\t" << GLOBALtiles[order[1]].rotation.y << "\t" << GLOBALtiles[order[1]].rotation.z << "\n";


	int sides_0[] = { 2,5,3,4 };
	int blocks_0[] = { 2,5,8 };
	int orders_0[] = { 2,5,3,4 };

	int sides_1[] = { 0,4,1,5 };
	int blocks_1[] = { 0,1,2 };
	int orders_1[] = { 4,0,5,1 };

	int* sides = NULL, * blocks = NULL, *orders = NULL;
	if (ccc == 2) {
		sides = sides_1; blocks = blocks_1; orders = orders_1;
	}
	else {
		sides = sides_0; blocks = blocks_0; orders = orders_0;
	}
	for (int i = 0; i < 4; i++) { //every side around
		for (int j = 0; j < 3; j++) { //blocks (3 of them)
			GLOBALtiles[order[sides[i] * 9 + blocks[j]]].rotate = 2;
			cerr << GLOBALtiles[order[sides[i] * 9 + blocks[j]]].side << " > ";
			if (i < 2)
				GLOBALtiles[order[sides[i] * 9 + blocks[j]]].genPositionOffsetPerpendicular(side, 2-j); //!!!!!!!
			else
				GLOBALtiles[order[sides[i] * 9 + blocks[j]]].genPositionOffsetPerpendicular(side, j);
		}
	}
	cerr << endl;
	//for (int i = 0; i < 6 * 9; i++)
	//	cerr << GLOBALtiles[i].rotate << (i % 3 == 2 ? "\n" : "\t") << (i % 9 == 8 ? "\n" : "");
	//cerr << endl;

	for (int i = 0; i < 6 * 9; i++)
	cerr << order[i] << " (" << GLOBALtiles[order[i]].rotate << ")" << (i % 3 == 2 ? "\n" : "\t") << (i % 9 == 8 ? "\n" : "");
	//order update
	for (int i = 0; i < 3; i++) {
		int temp = order[orders[0] * 9 + blocks[i]];
		order[orders[0] * 9 + blocks[i]] = order[orders[1] * 9 + blocks[i]];
		order[orders[1] * 9 + blocks[i]] = order[orders[2] * 9 + blocks[i]];
		order[orders[2] * 9 + blocks[i]] = order[orders[3] * 9 + blocks[i]];
		order[orders[3] * 9 + blocks[i]] = temp;
	}
	
	/*
	int temp = order[side * 9 + 8];
	order[side * 9 + 8] = order[side * 9 + 0];
	order[side * 9 + 0] = order[side * 9 + 2];
	order[side * 9 + 2] = temp;
	*/

	/*
	int temp = order[order[side * 9 + 0]];
	order[order[side * 9 + 0]] = order[order[side * 9 + 2]];
	order[order[side * 9 + 2]] = order[order[side * 9 + 8]];
	order[order[side * 9 + 8]] = order[order[side * 9 + 6]];
	order[order[side * 9 + 6]] = temp;
	temp = order[order[side * 9 + 1]];
	order[order[side * 9 + 1]] = order[order[side * 9 + 5]];
	order[order[side * 9 + 5]] = order[order[side * 9 + 7]];
	order[order[side * 9 + 7]] = order[order[side * 9 + 3]];
	order[order[side * 9 + 3]] = temp;
	*/
	/*
	temp = order[side * 9 + 0];
	order[side * 9 + 0] = order[side * 9 + 3];
	order[side * 9 + 3] = order[side * 9 + 6];
	order[side * 9 + 6] = order[side * 9 + 7];
	order[side * 9 + 7] = order[side * 9 + 8];
	order[side * 9 + 8] = order[side * 9 + 5];
	order[side * 9 + 5] = order[side * 9 + 2];
	order[side * 9 + 2] = order[side * 9 + 1];
	order[side * 9 + 1] = temp;
	*/

	//cerr << "    into    \n\n";
	//for (int i = 0; i < 6 * 9; i++)
	//	cerr << order[i] << (i%3==2 ? "\n" : "\t") << (i%9==8 ? "\n" : "");
	//cerr << "\n\n\n\n";
	
	/*
	GLOBALtiles[2 * 9 + 2].genPositionOffsetPerpendicular(2);
	GLOBALtiles[2 * 9 + 8].genPositionOffsetPerpendicular(1);
	GLOBALtiles[3 * 9 + 2].genPositionOffsetPerpendicular(1);
	GLOBALtiles[3 * 9 + 8].genPositionOffsetPerpendicular(2);
	GLOBALtiles[4 * 9 + 2].genPositionOffsetPerpendicular(1);
	GLOBALtiles[4 * 9 + 8].genPositionOffsetPerpendicular(2);
	GLOBALtiles[5 * 9 + 2].genPositionOffsetPerpendicular(2);
	GLOBALtiles[5 * 9 + 8].genPositionOffsetPerpendicular(1);
	*/
	/*
	GLOBALtiles[2 * 9 + 2].rotate = 2;
	GLOBALtiles[2 * 9 + 2].genPositionOffsetPerpendicular(0);
	GLOBALtiles[2 * 9 + 5].rotate = 2;
	GLOBALtiles[2 * 9 + 5].genPositionOffsetPerpendicular(1);
	GLOBALtiles[2 * 9 + 8].rotate = 2;
	GLOBALtiles[2 * 9 + 8].genPositionOffsetPerpendicular(2);
	
	GLOBALtiles[3 * 9 + 2].rotate = 2;
	GLOBALtiles[3 * 9 + 2].genPositionOffsetPerpendicular(2);
	GLOBALtiles[3 * 9 + 5].rotate = 2;
	GLOBALtiles[3 * 9 + 5].genPositionOffsetPerpendicular(1);
	GLOBALtiles[3 * 9 + 8].rotate = 2;
	GLOBALtiles[3 * 9 + 8].genPositionOffsetPerpendicular(0);
	
	GLOBALtiles[4 * 9 + 2].rotate = 2;
	GLOBALtiles[4 * 9 + 2].genPositionOffsetPerpendicular(2);
	GLOBALtiles[4 * 9 + 5].rotate = 2;
	GLOBALtiles[4 * 9 + 5].genPositionOffsetPerpendicular(1);
	GLOBALtiles[4 * 9 + 8].rotate = 2;
	GLOBALtiles[4 * 9 + 8].genPositionOffsetPerpendicular(0);
	
	GLOBALtiles[5 * 9 + 2].rotate = 2;
	GLOBALtiles[5 * 9 + 2].genPositionOffsetPerpendicular(0);
	GLOBALtiles[5 * 9 + 5].rotate = 2;
	GLOBALtiles[5 * 9 + 5].genPositionOffsetPerpendicular(1);
	GLOBALtiles[5 * 9 + 8].rotate = 2;
	GLOBALtiles[5 * 9 + 8].genPositionOffsetPerpendicular(2);
	*/
}

bool block = false;
bool p_flipflop = 0;
bool i_flipflop = 0;
bool u_flipflop = 0;
double xxx = 0.0, yyy = 0.0;
void input(GLFWwindow* window, glm::vec3& Position, glm::vec3& Orientation, glm::vec3& Up)                                      // input
{
	Orientation = cameraFront;

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)          // sprawdzanie czy wybrany klawisz jest wciœniêty (Esc)     GLFW_PRESS | GLFW_RELEASE
		glfwSetWindowShouldClose(window, true);                     // zamykanie okienka

	const float cameraSpeed = 7.00f * deltaTime; // adjust accordingly
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
		if (p_flipflop == 0 && rotateCounter > -1)
			p_flipflop = 1;
		//z rot
		if (p_flipflop == 0) {
			p_flipflop = 1;
			//rotate(ccc);
			ccc = 0;
			rotateCounter = ANIMATION_DURATION;
			for (int i = 0, j = 0; j < 9; j++) {
				i = order[j];
				GLOBALblocks[i].roll = true;
				if (j % 9 == 4)
					GLOBALblocks[i].blockOffsetFix = -1;
				else if (j % 2 == 0) {
					//cerr << "c:" << GLOBALblocks[i].position.x << "\t" << GLOBALblocks[i].position.y << endl;
					if (GLOBALblocks[i].position.x < 1 && GLOBALblocks[i].position.y < 1)
						GLOBALblocks[i].blockOffsetFix = 1;
					else if (GLOBALblocks[i].position.x > 1 && GLOBALblocks[i].position.y < 1)
						GLOBALblocks[i].blockOffsetFix = 0;
					else if (GLOBALblocks[i].position.x > 1 && GLOBALblocks[i].position.y > 1)
						GLOBALblocks[i].blockOffsetFix = 3;
					else
						GLOBALblocks[i].blockOffsetFix = 2;
				}
				else {
					//cerr << "d:" << GLOBALblocks[i].position.x << "\t" << GLOBALblocks[i].position.y << endl;
					if (GLOBALblocks[i].position.x > 0.5 && GLOBALblocks[i].position.x < 1.5)
						if (GLOBALblocks[i].position.y > 1)
							GLOBALblocks[i].blockOffsetFix = 3;
						else
							GLOBALblocks[i].blockOffsetFix = 1;
					else if (GLOBALblocks[i].position.x < 1)
						GLOBALblocks[i].blockOffsetFix = 2;
					else
						GLOBALblocks[i].blockOffsetFix = 0;
				}
				GLOBALblocks[i].rot[2]++;
				if (GLOBALblocks[i].rot[2] == 4) {
					GLOBALblocks[i].rot[2] = 0;
					GLOBALblocks[i].prevRot[2] = -1;
				}
					
			}
			//order
			int t = neworder[0];
			neworder[0] = neworder[6];
			neworder[6] = neworder[8];
			neworder[8] = neworder[2];
			neworder[2] = t;
			
			t = neworder[1];
			neworder[1] = neworder[3];
			neworder[3] = neworder[7];
			neworder[7] = neworder[5];
			neworder[5] = t;

			//for (int i = 0; i < 27; i++)
			//	cerr << order[i] << " ";
			//cerr << endl;
			//for (int i = 0; i < 27; i++)
			//	cerr << neworder[i] << " ";
		}
	}
	if (glfwGetKey(window, GLFW_KEY_P) == GLFW_RELEASE) {
		p_flipflop = 0;
	}
	if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS) {

	}
	if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS) {
		if (i_flipflop == 0 && rotateCounter > -1)
			i_flipflop = 1;
		//x rot
		if (i_flipflop == 0) {
			i_flipflop = 1;
			//rotate(ccc);
			ccc = 2;
			rotateCounter = ANIMATION_DURATION;
			for (int i = 0, j = 0; j < 9; j++) {
				i = order[3*j];
				//cerr << i << " -> ";
				GLOBALblocks[i].roll = true;
				if (j % 9 == 4)
					GLOBALblocks[i].blockOffsetFix = -1;
				else if (j % 2 == 0) {
					//cerr << "c:";
					//cerr << "c:" << GLOBALblocks[i].position.x << "\t" << GLOBALblocks[i].position.y << endl;
					if (GLOBALblocks[i].position.z < 1 && GLOBALblocks[i].position.y < 1)
						GLOBALblocks[i].blockOffsetFix = 1;
					else if (GLOBALblocks[i].position.z > 1 && GLOBALblocks[i].position.y < 1)
						GLOBALblocks[i].blockOffsetFix = 0;
					else if (GLOBALblocks[i].position.z > 1 && GLOBALblocks[i].position.y > 1)
						GLOBALblocks[i].blockOffsetFix = 3;
					else
						GLOBALblocks[i].blockOffsetFix = 2;
				}
				else {
					//cerr << " d:";
					//cerr << "d:" << GLOBALblocks[i].position.x << "\t" << GLOBALblocks[i].position.y << endl;
					if (GLOBALblocks[i].position.z > 0.5 && GLOBALblocks[i].position.z < 1.5)
						if (GLOBALblocks[i].position.y > 1)
							GLOBALblocks[i].blockOffsetFix = 3;
						else
							GLOBALblocks[i].blockOffsetFix = 1;
					else if (GLOBALblocks[i].position.z < 1)
						GLOBALblocks[i].blockOffsetFix = 2;
					else
						GLOBALblocks[i].blockOffsetFix = 0;
				}
				GLOBALblocks[i].rot[0]--;
				if (GLOBALblocks[i].rot[0] == -1) {
					GLOBALblocks[i].rot[0] = 3;
					GLOBALblocks[i].prevRot[0] = 4;
				}
					
				//cerr << GLOBALblocks[i].position.x << "\t" << GLOBALblocks[i].position.y << "\t" << GLOBALblocks[i].position.z << "  ->  " << GLOBALblocks[i].blockOffsetFix << endl;
				//cerr << GLOBALblocks[i].position << endl;
				//cerr << GLOBALblocks[i].blockOffsetFix << endl;
			}
			//order
			int t = neworder[0];
			neworder[0] = neworder[6];
			neworder[6] = neworder[24];
			neworder[24] = neworder[18];
			neworder[18] = t;

			t = neworder[9];
			neworder[9] = neworder[3];
			neworder[3] = neworder[15];
			neworder[15] = neworder[21];
			neworder[21] = t;
		}
	}
	if (glfwGetKey(window, GLFW_KEY_I) == GLFW_RELEASE) {
		i_flipflop = 0;
	}
	if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS) {
		if (u_flipflop == 0 && rotateCounter > -1)
			u_flipflop = 1;
		if (u_flipflop == 0) {
			u_flipflop = 1;
			//rotate(ccc);
			ccc = 4;
			rotateCounter = ANIMATION_DURATION;
			for (int i = 0, j = 0; j < 9; j++) {
				i = order[j%3 + j/3 * 9];
				cerr << i << endl;
				GLOBALblocks[i].roll = true;
				if (j % 9 == 4)
					GLOBALblocks[i].blockOffsetFix = -1;
				else if (j % 2 == 0) {
					//cerr << "c:" << GLOBALblocks[i].position.x << "\t" << GLOBALblocks[i].position.y << endl;
					if (GLOBALblocks[i].position.x < 1 && GLOBALblocks[i].position.z < 1)
						GLOBALblocks[i].blockOffsetFix = 1;
					else if (GLOBALblocks[i].position.x > 1 && GLOBALblocks[i].position.z < 1)
						GLOBALblocks[i].blockOffsetFix = 0;
					else if (GLOBALblocks[i].position.x > 1 && GLOBALblocks[i].position.z > 1)
						GLOBALblocks[i].blockOffsetFix = 3;
					else
						GLOBALblocks[i].blockOffsetFix = 2;
				}
				else {
					//cerr << "d:" << GLOBALblocks[i].position.x << "\t" << GLOBALblocks[i].position.y << endl;
					if (GLOBALblocks[i].position.x > 0.5 && GLOBALblocks[i].position.x < 1.5)
						if (GLOBALblocks[i].position.z > 1)
							GLOBALblocks[i].blockOffsetFix = 3;
						else
							GLOBALblocks[i].blockOffsetFix = 1;
					else if (GLOBALblocks[i].position.x < 1)
						GLOBALblocks[i].blockOffsetFix = 2;
					else
						GLOBALblocks[i].blockOffsetFix = 0;
				}
				GLOBALblocks[i].rot[1]++;
				if (GLOBALblocks[i].rot[1] == 4) {
					GLOBALblocks[i].rot[1] = 0;
					GLOBALblocks[i].prevRot[1] = -1;
				}

			}
			//order
			int t = neworder[0];
			neworder[0] = neworder[18];
			neworder[18] = neworder[20];
			neworder[20] = neworder[2];
			neworder[2] = t;

			t = neworder[1];
			neworder[1] = neworder[9];
			neworder[9] = neworder[19];
			neworder[19] = neworder[11];
			neworder[11] = t;
		}
	}
	if (glfwGetKey(window, GLFW_KEY_U) == GLFW_RELEASE) {
		u_flipflop = 0;
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



VAO* vao_x, *vao_y, *vao_z;


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
	//
	float v = 0.45;
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
	/*
	int n_vertices = 4 * 3;
	GLfloat vertices_z[] = {
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
	GLfloat vertices_x[] = {
		//x    y      z
		-0.5f, 0.0f, -0.5f,
		+0.5f, 0.0f, -0.5f,
		-0.5f, 0.0f, +0.5f,
		+0.5f, 0.0f, +0.5f
	};
	GLfloat vertices_y[] = {
		//x    y      z
		0.0f, -0.5f, -0.5f,
		0.0f, +0.5f, -0.5f,
		0.0f, -0.5f, +0.5f,
		0.0f, +0.5f, +0.5f
	};
	*/
	// VAO, VBO, EBO
	/*
	VBO VBO_z(vertices_z, sizeof(GLfloat) * n_vertices);
	VBO VBO_x(vertices_x, sizeof(GLfloat) * n_vertices);
	VBO VBO_y(vertices_y, sizeof(GLfloat) * n_vertices);
	EBO EBO1(indices, sizeof(GLuint) * n_indices);
	EBO EBO2(lineIndices, sizeof(GLuint) * n_lineIndices);
	*/
	VBO blockVBO(vertices, sizeof(GLfloat) * n_vertices);
	EBO blockEBO(indices, sizeof(GLuint) * n_indices);
	VAO blockVAO;
	blockVAO.Bind();
	blockVAO.LinkVBO(blockVBO, 0, 1);
	blockVAO.Unbind();
	/*
	VAO VAO_z;
	VAO_z.Bind();
	VAO_z.LinkVBO(VBO_z, 0, 1);
	VAO_z.Unbind();
	VAO VAO_x;
	VAO_x.Bind();
	VAO_x.LinkVBO(VBO_x, 0, 1);
	VAO_x.Unbind();
	VAO VAO_y;
	VAO_y.Bind();
	VAO_y.LinkVBO(VBO_y, 0, 1);
	VAO_y.Unbind();
	vao_x = &VAO_x;
	vao_y = &VAO_y;
	vao_z = &VAO_z;
	*/
	//

	
	int order_[3*3*3];
	int order__[3*3*3];
	order = order_;
	neworder = order__;
	for (int i = 0; i < 3*3*3; i++) {
		order[i] = i;
	}
	for (int i = 0; i < 3 * 3 * 3; i++) {
		neworder[i] = i;
	}
	

	Block blocks[3 * 3 * 3];
	GLOBALblocks = blocks;
	int n_blocks = 3 * 3 * 3;
	for (int i = 0; i < 3 * 3 * 3; i++) {
		blocks[i].position = glm::vec3(i % 3, (i / 3)%3, i / 9);
		//for (int j = 0; j < 3; j++)
		//	blocks[i].color[j] = glm::vec3(0.9, 0.1, 0.1);
		if (i < 1*9) { //first side - red
			blocks[i].color[0] = glm::vec3(0.9, 0.1, 0.1); //red
			if (i % 3 == 2) blocks[i].color[1] = glm::vec3(0.9, 0.9, 0.9); //white
			if (i % 3 == 0) blocks[i].color[3] = glm::vec3(0.9, 0.9, 0.1); //yellow
			if (i > 0 * 9 + 5) blocks[i].color[4] = glm::vec3(0.1, 0.1, 0.9); //blue
			if (i < 0 * 9 + 3) blocks[i].color[5] = glm::vec3(0.1, 0.9, 0.1); //green
		}
		else if (i < 2 * 9) {
			if (i % 3 == 2) blocks[i].color[1] = glm::vec3(0.9, 0.9, 0.9); //white
			if (i % 3 == 0) blocks[i].color[3] = glm::vec3(0.9, 0.9, 0.1); //yellow
			if (i > 1*9 + 5) blocks[i].color[4] = glm::vec3(0.1, 0.1, 0.9); //blue
			if (i < 1*9 + 3) blocks[i].color[5] = glm::vec3(0.1, 0.9, 0.1); //green
		}
		else if (i < 3 * 9) {
			blocks[i].color[2] = glm::vec3(0.9, 0.5, 0.1); //orange
			if (i % 3 == 2) blocks[i].color[1] = glm::vec3(0.9, 0.9, 0.9); //white
			if (i % 3 == 0) blocks[i].color[3] = glm::vec3(0.9, 0.9, 0.1); //yellow
			if (i > 2*9 + 5) blocks[i].color[4] = glm::vec3(0.1, 0.1, 0.9); //blue
			if (i < 2*9 + 3) blocks[i].color[5] = glm::vec3(0.1, 0.9, 0.1); //green
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

	/*
	Tile tiles[6*9];
	GLOBALtiles = tiles;
	int n_tiles = 6*9;
	int j = 0;
	for (int i = 0; j < 1 * 9; j++, i++) { //front side - red
		tiles[j].color = glm::vec3(0.9,0.1,0.1);
		tiles[j].position = glm::vec3(i%3, i/3, 0.0);
		tiles[j].vao = &VAO_z;
		if (i != 4) tiles[j].side = j / 9; else tiles[j].side = j / 9 + 10;
	}
	for (int i = 0; j < 2 * 9; j++, i++) { //back side - orange
		tiles[j].color = glm::vec3(0.9, 0.5, 0.1);
		tiles[j].position = glm::vec3(i % 3, i / 3, -3.0);
		tiles[j].vao = &VAO_z;
		if (i != 4) tiles[j].side = j / 9; else tiles[j].side = j / 9 + 10;
	}
	for (int i = 0; j < 3 * 9; j++, i++) { //bottom side - green
		tiles[j].color = glm::vec3(0.1, 0.9, 0.1);
		tiles[j].position = glm::vec3(i / 3, -0.5, i % 3 - 2.5);
		tiles[j].vao = &VAO_x;
		if (i != 4) tiles[j].side = j / 9; else tiles[j].side = j / 9 + 10;
	}
	for (int i = 0; j < 4 * 9; j++, i++) { //top side - blue
		tiles[j].color = glm::vec3(0.1, 0.1, 0.9);
		tiles[j].position = glm::vec3(i / 3, 2.5, i % 3 - 2.5);
		tiles[j].vao = &VAO_x;
		if (i != 4) tiles[j].side = j / 9; else tiles[j].side = j / 9 + 10;
	}
	for (int i = 0; j < 5 * 9; j++, i++) { //left side - white
		tiles[j].color = glm::vec3(0.9, 0.9, 0.9);
		tiles[j].position = glm::vec3(-0.5, i/3, i % 3 - 2.5);
		tiles[j].vao = &VAO_y;
		if (i != 4) tiles[j].side = j / 9; else tiles[j].side = j / 9 + 10;
	}
	for (int i = 0; j < 6 * 9; j++, i++) { //right side - yellow
		tiles[j].color = glm::vec3(0.9, 0.9, 0.1);
		tiles[j].position = glm::vec3(2.5, i / 3, i % 3 - 2.5);
		tiles[j].vao = &VAO_y;
		if (i != 4) tiles[j].side = j / 9; else tiles[j].side = j / 9 + 10;
	}
	//
	*/

	float backgroud_r, backgroud_g, backgroud_b;
	glfwSwapInterval(1); //ograniczenie fps to synchronizacji vsync
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); //kontury
	glLineWidth(3);
	float time = 0;
	Camera camera(width, height);
	int counterFlex = 50;
	int idFlex = 0;
	glm::vec3 colorSave[27][6], flex = glm::vec3(0.8, 0.2, 0.8);
	int counterFlex2 = 50;
	int idFlex2 = 0;
	glm::vec3 flex2 = glm::vec3(0.2, 0.8, 0.2);

	for (int i = 0; i < 27; i++)
		for (int j = 0; j < 6; j++)
			colorSave[i][j] = blocks[i].color[j];
	while (!glfwWindowShouldClose(window))
	{
		/*
		counterFlex--;
		if (counterFlex == 0) {
			counterFlex = 50;
			for (int i = 0; i < 6; i++) {
				glm::vec3 t = blocks[order[idFlex]].color[i];
				blocks[order[idFlex]].color[i] = flex;
				if (idFlex != 0)
					blocks[order[idFlex-1]].color[i] = colorSave[order[idFlex - 1]][i];
				else
					blocks[order[26]].color[i] = colorSave[26][i];
			}	
			idFlex++;
			if (idFlex == 27)
				idFlex = 0;
		}
		counterFlex2--;
		if (counterFlex2 == 0) {
			counterFlex2 = 50;
			for (int i = 0; i < 6; i++) {
				glm::vec3 t = blocks[idFlex2].color[i];
				//blocks[idFlex2].color[i] = flex2;
				if (idFlex2 != 0)
					blocks[idFlex2 - 1].color[i] = colorSave[idFlex2 - 1][i];
				else
					blocks[order[26]].color[i] = colorSave[26][i];
			}
			idFlex2++;
			if (idFlex2 == 27)
				idFlex2 = 0;
		}
		*/

		//ROTATION
		if (rotateCounter >= 0) {
			float time = rotateCounter * M_PI / 2. / ANIMATION_DURATION;

			for (int j = 0; j < 3 * 3 * 3; j++) {
				if (blocks[j].roll)
					blocks[j].rotate((j + 1) % 2, ccc, time, 0);
			}
			if (rotateCounter == 0) {
				//roll reset && block update
				for (int j = 0; j < 3 * 3 * 3; j++) {
					if (blocks[j].roll) {
						blocks[j].roll = false;
						blocks[j].update();
					}
				}				
				//order rotation
				for (int j = 0; j < 3 * 3 * 3; j++)
					order[j] = neworder[j];
			}
				
			rotateCounter--;

			/*
			blocks[0].rotate(1, 0, time, 0);
			blocks[1].rotate(0, 0, time, 1);
			blocks[2].rotate(1, 0, time, 0);
			blocks[3].rotate(0, 0, time, 2);
			blocks[4].rotate(0, 0, time, -1);
			blocks[5].rotate(0, 0, time, 0);
			blocks[6].rotate(1, 0, time, 0);
			blocks[7].rotate(0, 0, time, 3);
			blocks[8].rotate(1, 0, time, 0);
			*/
			/*
			blocks[9].rotate(1, 0, time, 0);
			blocks[10].rotate(0, 0, time, 1);
			blocks[11].rotate(1, 0, time, 0);
			blocks[12].rotate(0, 0, time, 2);
			blocks[13].rotate(0, 0, time, -1);
			blocks[14].rotate(0, 0, time, 0);
			blocks[15].rotate(1, 0, time, 0);
			blocks[16].rotate(0, 0, time, 3);
			blocks[17].rotate(1, 0, time, 0);
			*/
			//blocks[ccc].rotate(1, 0, time, 0);
			/*
			blocks[0].rotate(1, 2, time, 0);
			blocks[3].rotate(0, 2, time, 1); 
			blocks[6].rotate(1, 2, time, 0);
			blocks[9].rotate(0, 2, time, 2);
			blocks[12].rotate(0, 2, time, -1); 
			blocks[15].rotate(0, 2, time, 0);
			blocks[18].rotate(1, 2, time, 0);
			blocks[21].rotate(0, 2, time, 3); 
			blocks[24].rotate(1, 2, time, 0);
			rotateCounter--;
			*/
		}



		/* 
		if (rotateCounter >= 0) { 
			float multi = rotateCounter * M_PI / 2. / ANIMATION_DURATION;
			for (int i = 0; i < n_tiles; i++) {
				if (tiles[order[i]].rotate == 1) {
					if ((order[i] % 2 && !(tiles[order[i]].side % 2)) || (!(order[i] % 2) && tiles[order[i]].side % 2))
						tiles[order[i]].genRotationPosition(multi, 1.);
					else
						tiles[order[i]].genRotationPosition(multi, sqrt(2));
				}
				if (tiles[order[i]].rotate == 2) {
					tiles[order[i]].genRotationPositionPerpendicular(ccc, multi, 1.5); ///!!!!!!!! ccc
					if (rotateCounter == 0)
						tiles[order[i]].updateSide(ccc); ///!!!!!!!! ccc
				}
			}
			if (rotateCounter == 0)
				for (int i = 0; i < n_tiles; i++)
					tiles[order[i]].rotate = 0;
			rotateCounter--;
		}
		*/
		//time logic
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		//cerr << "time= " << time << "\tdelta= " << deltaTime << "\tc= " << counter << endl;
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
		camera.Matrix(45.0f, 0.5f, 20.0f, shaderProgram, "camMatrix");

		//VAO_z.Bind();
		// Narysuj trójk¹ty
		blockVAO.Bind();
		for (int i = 0; i < n_blocks; i++)
		{
			//tiles[i].vao->Bind();
			//model
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
				glDrawElements(GL_TRIANGLES, 2*1*3, GL_UNSIGNED_INT, (void*)(6 * j * sizeof (GLfloat)));
			}



			//EBO2.Bind();
			//glUniform3f(colorLoc, 0.1, 0.1, 0.1);
			//glDrawElements(GL_LINES, 4, GL_UNSIGNED_INT, 0);
			//tiles[i].vao->Unbind();
		}
		// Odœwie¿ widok
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	
	glfwDestroyWindow(window); //Delete window before ending the program
	glfwTerminate(); //Terminate GLFW before ending the program

	/*
	VAO_x.Delete();
	VAO_y.Delete();
	VAO_z.Delete();
	VBO_x.Delete();
	VBO_y.Delete();
	VBO_z.Delete();
	EBO1.Delete();
	EBO2.Delete();
	*/
	blockVAO.Delete();
	blockVBO.Delete();
	blockEBO.Delete();
	//delete[] vertices;
	//delete[] indices;
	return 0;
}