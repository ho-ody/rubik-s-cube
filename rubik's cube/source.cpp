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

int N = 9;
float v = 0.5;

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
/*
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
*/
int** rotateIndex;

int indexesOfRotationX(int j, int offset) {
	return order[N * j + offset];
}
int indexesOfRotationY(int j, int offset) {
	return order[j % N + j / N * N * N + offset*N];
}
int indexesOfRotationZ(int j, int offset) {
	return order[j + offset*N*N];
}

void generateRotateIndexAxis() {
	rotateIndex = new int*[N * N];
	for (int i = 0; i < N * N; i++)
		rotateIndex[i] = new int[N * N];

	for (int i = 0; i < N * N; i++) {
		for (int j = 0; j < N; j++) {
			rotateIndex[3*j+0][i] = N * i + j; //x
			rotateIndex[3*j+1][i] = i % N + (i / N) * N * N + N*j; //y
			rotateIndex[3*j+2][i] = i + j*N*N; //z
			//cerr << rotateIndex[3 * j + 0][i] << "\t" << rotateIndex[3 * j + 1][i] << "\t" << rotateIndex[3 * j + 2][i] << "\n";
		}
	}
}
int reverseRotateIndex(int k, int axis) {
	for (int i = 0; i < N * N; i++) {
		for (int j = 0; j < N; j++) {
			if (rotateIndex[axis + j*3][i] == k)
				return i;
		}	
	}
	return -1;
}


void rotate(int direction, int indexsOfRotation(int,int), int offset) {
	float a, b;
	rotateCounter = ANIMATION_DURATION;
	toRotate = rotateIndex[axis];
	axis %= 3;
	for (int i = 0, j = 0; j < N*N; j++) {
		if (axis == 0) {
			swap(GLOBALblocks[i].offset_[1], GLOBALblocks[i].offset_[2]);
			swap(GLOBALblocks[i].radius_[1], GLOBALblocks[i].radius_[2]);
		}
		else if(axis == 1) {
			swap(GLOBALblocks[i].offset_[0], GLOBALblocks[i].offset_[2]);
			swap(GLOBALblocks[i].radius_[0], GLOBALblocks[i].radius_[2]);
		}
		else {
			swap(GLOBALblocks[i].offset_[1], GLOBALblocks[i].offset_[0]);
			swap(GLOBALblocks[i].radius_[1], GLOBALblocks[i].radius_[0]);
		}
		i = indexsOfRotation(j,offset);
		GLOBALblocks[i].roll = true;


		float _x = GLOBALblocks[i].position.x;
		float _y = GLOBALblocks[i].position.y;
		float _z = GLOBALblocks[i].position.z;
		float _c = (N - 1) * v;
		GLOBALblocks[i].offsetSideFix[2] = false;
		GLOBALblocks[i].offsetSideFix[0] = false;
		GLOBALblocks[i].offsetSideFix[1] = false;
		if ((_y < _x - v && _y > _c) || (_x < _c && _y > -_x + (N - 1) * 2 * v) || (_y > _x && _y < _c) || (_x > _c && _y < -_x + (N - 1) * 2 * v))
			GLOBALblocks[i].offsetSideFix[2] = true;
		if ((_y < _z - v && _y > _c) || (_z < _c && _y > -_z + (N - 1) * 2 * v) || (_y > _z && _y < _c) || (_z > _c && _y < -_z + (N - 1) * 2 * v))
			GLOBALblocks[i].offsetSideFix[0] = true;
		if ((_z < _x - v && _z > _c) || (_x < _c && _z > -_x + (N - 1) * 2 * v) || (_z > _x && _z < _c) || (_x > _c && _z < -_x + (N - 1) * 2 * v))
			GLOBALblocks[i].offsetSideFix[1] = true;


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
		/*
		float c = (N - 1) * v;
		if (N % 2 == 1 && j % (N * N) == (N * N - 1) / 2)
			GLOBALblocks[i].blockOffsetFix = -1;
		if (b < a - v && b > c)
			GLOBALblocks[i].blockOffsetFix = 0 + direction;
		else if (a < c && b > -a + (N - 1) * 2 * v)
			GLOBALblocks[i].blockOffsetFix = 0 + direction;
		else if (b > a && b < c)
			GLOBALblocks[i].blockOffsetFix = 0 + direction;
		else if (a > c && b < -a + (N - 1) * 2 * v)
			GLOBALblocks[i].blockOffsetFix = 0 + direction;
		*/
		float c = (N - 1) * v;
		if (b < c - v && a < c + v)
			GLOBALblocks[i].blockOffsetFix = 1 + direction;
		else if (b < c + v && a > c + v)
			GLOBALblocks[i].blockOffsetFix = 0 + direction;
		else if (b > c + v && a > c - v)
			GLOBALblocks[i].blockOffsetFix = 3 + direction;
		else if (b > c - v && a < c - v)
			GLOBALblocks[i].blockOffsetFix = 2 + direction;
		/*
		float c = (N - 1) * v;
		if (N % 2 == 1 && j % (N * N) == (N * N - 1) / 2)
			GLOBALblocks[i].blockOffsetFix = -1;
		else if (b < a - v && b > -a + (N - 2) * 2 * v)
			GLOBALblocks[i].blockOffsetFix = 0 + direction;
		else if (b > a + v && b < -a + (N) * 2 * v)
			GLOBALblocks[i].blockOffsetFix = 2 + direction;
		else if (b < c)
			GLOBALblocks[i].blockOffsetFix = 1 + direction;
		else
			GLOBALblocks[i].blockOffsetFix = 3 + direction;
		*/

			/*
			else if (b < c)
				GLOBALblocks[i].blockOffsetFix = 1 + direction;
			else
				GLOBALblocks[i].blockOffsetFix = 3 + direction;

		/*
		float c = (N - 1) * v;
		//if (N % 2 == 1 && j % (N * N) == (N * N - 1) / 2)
		//	GLOBALblocks[i].blockOffsetFix = -1;
		
		if (b < a - v && b > c)
			GLOBALblocks[i].blockOffsetFix = 0 + direction;
		else if (a < c && b > -a + (N - 1) * 2 * v)
			GLOBALblocks[i].blockOffsetFix = 0 + direction;
		else if (b > a && b < c)
			GLOBALblocks[i].blockOffsetFix = 0 + direction;
		else if (a > c && b < -a + (N - 1) * 2 * v)
			GLOBALblocks[i].blockOffsetFix = 0 + direction;
			*/
		/*
		else if (b < c)
			GLOBALblocks[i].blockOffsetFix = 1 + direction;
		else
			GLOBALblocks[i].blockOffsetFix = 3 + direction;
		*/
		/*
		float c = (N - 1) * v;
		if (b < c - v && a < c + v)
			GLOBALblocks[i].blockOffsetFix = 1 + direction;
		else if (b < c + v && a > c + v)
			GLOBALblocks[i].blockOffsetFix = 0 + direction;
		else if (b > c + v && a > c - v)
			GLOBALblocks[i].blockOffsetFix = 3 + direction;
		else if (b > c - v && a < c - v)
			GLOBALblocks[i].blockOffsetFix = 2 + direction;
		*/
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
	
	
	/*
	for (int i = 0; i < N*N*N; i++) {
		for (int l = 0; l < 6; l++)
			GLOBALblocks[i].color[l] = glm::vec3(0, 0, 0);

		if (GLOBALblocks[i].blockOffsetFix == 0)
			for (int l = 0; l < 6; l++)
				GLOBALblocks[i].color[l] = glm::vec3(.2, .2, .2);
		if (GLOBALblocks[i].blockOffsetFix == 1)
			for (int l = 0; l < 6; l++)
				GLOBALblocks[i].color[l] = glm::vec3(.4,.4,.4);
		if (GLOBALblocks[i].blockOffsetFix == 2)
			for (int l = 0; l < 6; l++)
				GLOBALblocks[i].color[l] = glm::vec3(.6, .6, .6);
		if (GLOBALblocks[i].blockOffsetFix == 3)
			for (int l = 0; l < 6; l++)
				GLOBALblocks[i].color[l] = glm::vec3(.8, .8, .8);
	}
	
	*/
}
// 0 -> waiting for input (M key), 1 -> waiting for move (FBLRUD or Escape), 2 -> input processed, reseting
int move_cube = 0;
int code_input = 0; 
int code_input_index = 0;
int COLORtest = 0;
int TESTTEST = 0;
int row = 0;



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
		if (glfwGetKey(window, GLFW_KEY_0) == GLFW_PRESS)
			row = 0;
		if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
			row = 1;
		if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
			row = 2;
		if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
			row = 3;
		if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
			row = 4;
		if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS)
			row = 5;
		if (glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS)
			row = 6;
		if (glfwGetKey(window, GLFW_KEY_7) == GLFW_PRESS)
			row = 7;
		if (glfwGetKey(window, GLFW_KEY_8) == GLFW_PRESS)
			row = 8;
		if (glfwGetKey(window, GLFW_KEY_9) == GLFW_PRESS)
			row = 9;

		if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
			direction = 1;
		else
			direction = 0;
		
		if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) {
			axis = 2 + row * 3;
			rotate(direction, indexesOfRotationZ,row);
			move_cube = 2;
		}
		if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
			if (direction) direction = 0; else direction = 1; //fix clockwise-anticlockwise
			axis = 0 + row * 3;
			rotate(direction, indexesOfRotationX,row);
			move_cube = 2;
		}
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
			if (direction) direction = 0; else direction = 1; //fix clockwise-anticlockwise
			axis = 1 + row * 3;
			rotate(direction, indexesOfRotationY,row);
			move_cube = 2;
		}
		if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS) {
			if (direction) direction = 0; else direction = 1; //fix clockwise-anticlockwise
			axis = 2 + (N-1) * 3 - row * 3;
			rotate(direction, indexesOfRotationZ,N-1-row);
			move_cube = 2;
		}
		if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) {
			axis = 0 + (N-1) * 3 - row * 3;
			rotate(direction, indexesOfRotationX,N-1-row);
			move_cube = 2;
		}
		if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS) {
			axis = 1 + (N-1) * 3 - row * 3;
			rotate(direction, indexesOfRotationY,N-1-row);
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
			row = 0; //reset row
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

	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) { //code input
		if (COLORtest == 0) {
			COLORtest = 1;
			//for (int i = 0; i < N * N * N; i++) {
			//	for (int j = 0; j < 6; j++)
			//		GLOBALblocks[i].color[j] = glm::vec3(static_cast<float>(order[i])/pow(N,3), 0, 0);
			//}
			TESTTEST++;
			if (TESTTEST == 3)
				TESTTEST = 0;

			Block* blocks = GLOBALblocks;

			for (int i = 0; i < N * N * N; i++) {

				for (int l = 0; l < 6; l++)
					blocks[i].color[l] = glm::vec3(0, 0, 0);

				if (blocks[i].offset_[TESTTEST] == 1)
					for (int l = 0; l < 6; l++)
						blocks[i].color[l].y = .5;
				if (blocks[i].offset_[TESTTEST] == 2)
					for (int l = 0; l < 6; l++)
						blocks[i].color[l].y = 1.;
				if (blocks[i].radius_[TESTTEST] == 1)
					for (int l = 0; l < 6; l++)
						blocks[i].color[l].x = .5;
				if (blocks[i].radius_[TESTTEST] == 2)
					for (int l = 0; l < 6; l++)
						blocks[i].color[l].x = 1.;
				if (blocks[i].offset_[TESTTEST] == 7)
					for (int l = 0; l < 6; l++)
						blocks[i].color[l].z = 1;
			}
		}
	}
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_RELEASE) {
		if (COLORtest == 1)
			COLORtest = 0;
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
void delete2dArray(int** tab, int n) {
	for (int i = 0; i < n; ++i)
		delete[] tab[i];
	delete[] tab;
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
	float dv = v / 10.;
	v -= dv;
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
	v += dv;
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
	
	generateRotateIndexAxis();

	/*
	rotateIndex[3*j+0][i] = N * i + j; //x
	rotateIndex[3*j+1][i] = i % N + (i / N) * N * N + N*j; //y
	rotateIndex[3*j+2][i] = i + j*N*N; //z

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
	*/

	Block* blocks;
	blocks = new Block[pow(N, 3)];
	GLOBALblocks = blocks;
	int n_blocks = pow(N, 3);
	bool skip_radius_offset = true;
	for (int i = 0; i < n_blocks; i++) {
		//int k = i % (N * N);
		skip_radius_offset = false;	
		if (i < N*N || i >= (N - 1) * N * N) { //front + back side  //|| i >= (N - 1) * N * N
		}
		else if (i % N == 0) { //left side
			//k = reverseRotateIndex(i, 0);
		}
		else if (i % N == N - 1) { //right
			//k = reverseRotateIndex(i, 0 + (N-1) * 3);
		}
		else if (i % (N * N) < N) { //down side
			//k = reverseRotateIndex(i, 1);
		}
		else if (i % (N * N) > (N - 1) * N) {// up
			//k = reverseRotateIndex(i, 1 + (N-1) * 3);
		}
		else
			skip_radius_offset = true;
		skip_radius_offset = false;

		// RADIUS + OFFSET EACH AXIS
		// Z AXIS
		int k = i % (N * N);
		blocks[i].radius_[2] = (N - 1) / 2;
		for (int j = 1; j < (N + 1) / 2; j++) {
			if (k % N >= j && k % N < N - j && k / N >= j && k / N < N - j)
				blocks[i].radius_[2] = (N - 1) / 2 - j;
		}
		//cerr << blocks[i].radius << (i % 5 == 4 ? "\n" : "\t") << (i % 25 == 24 ? "\n" : "");
		blocks[i].offset_[2] = (N - 1) / 2;
		for (int j = 1; j < (N + 1) / 2; j++) {
			if ((k % N >= j && k % N < N - j) || (k / N >= j && k / N < N - j))
				blocks[i].offset_[2] = (N - 1) / 2 - j;
		}
		// X AXIS
		k = reverseRotateIndex(i, 0);
		blocks[i].radius_[0] = (N - 1) / 2;
		for (int j = 1; j < (N + 1) / 2; j++) {
			if (k % N >= j && k % N < N - j && k / N >= j && k / N < N - j)
				blocks[i].radius_[0] = (N - 1) / 2 - j;
		}
		//cerr << blocks[i].radius << (i % 5 == 4 ? "\n" : "\t") << (i % 25 == 24 ? "\n" : "");
		blocks[i].offset_[0] = (N - 1) / 2;
		for (int j = 1; j < (N + 1) / 2; j++) {
			if ((k % N >= j && k % N < N - j) || (k / N >= j && k / N < N - j))
				blocks[i].offset_[0] = (N - 1) / 2 - j;
		}
		// Y AXIS
		k = reverseRotateIndex(i, 1);
		blocks[i].radius_[1] = (N - 1) / 2;
		for (int j = 1; j < (N + 1) / 2; j++) {
			if (k % N >= j && k % N < N - j && k / N >= j && k / N < N - j)
				blocks[i].radius_[1] = (N - 1) / 2 - j;
		}
		//cerr << blocks[i].radius << (i % 5 == 4 ? "\n" : "\t") << (i % 25 == 24 ? "\n" : "");
		blocks[i].offset_[1] = (N - 1) / 2;
		for (int j = 1; j < (N + 1) / 2; j++) {
			if ((k % N >= j && k % N < N - j) || (k / N >= j && k / N < N - j))
				blocks[i].offset_[1] = (N - 1) / 2 - j;
		}
		/*
		if (i < N * N || i >= (N - 1) * N * N) {
			blocks[i].radius_ = (N - 1) / 2;
			for (int j = 1; j < (N + 1) / 2; j++) {
				if (k % N >= j && k % N < N - j && k / N >= j && k / N < N - j)
					blocks[i].radius_ = (N - 1) / 2 - j;
			}
			//cerr << blocks[i].radius << (i % 5 == 4 ? "\n" : "\t") << (i % 25 == 24 ? "\n" : "");
			blocks[i].offset_ = (N - 1) / 2;
			for (int j = 1; j < (N + 1) / 2; j++) {
				if ((k % N >= j && k % N < N - j) || (k / N >= j && k / N < N - j))
					blocks[i].offset_ = (N - 1) / 2 - j;
			}
		}
		*/
		
		//cerr << blocks[i].offset << (i % 5 == 4 ? "\n" : "\t") << (i % 25 == 24 ? "\n" : "");
		blocks[i].position = glm::vec3(i % N, (i / N)% N, i / (N*N)) * glm::vec3(2*v,2*v,2*v);
		//blocks[i].offsetSideFix = false;

		

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
		float c = (N - 1) * v;
		//if (N % 2 == 1 && j % (N * N) == (N * N - 1) / 2)
		//	GLOBALblocks[i].blockOffsetFix = -1;

		float _x = blocks[i].position.x;
		float _y = blocks[i].position.y;
		float _z = blocks[i].position.z;

		/*
		if ((b < a - v && b > c) || (a < c && b > -a + (N - 1) * 2 * v) || (b > a && b < c) || (a > c && b < -a + (N - 1) * 2 * v)) {
			blocks[i].offsetSideFix = true;
			//blocks[i].offset_ *= -1;
			//if (i < 25)
			//for (int l = 0; l < 6; l++)
			//	blocks[i].color[l] = glm::vec3(0, 0, 0);
		}
		*/
		/*
		if ((_y < _x - v && _y > c) || (_x < c && _y > -_x + (N - 1) * 2 * v) || (_y > _x && _y < c) || (_x > c && _y < -_x + (N - 1) * 2 * v))
			blocks[i].offsetSideFix[2] = 1;
		if ((_y < _z - v && _y > c) || (_z < c && _y > -_z + (N - 1) * 2 * v) || (_y > _z && _y < c) || (_z > c && _y < -_z + (N - 1) * 2 * v))
			blocks[i].offsetSideFix[0] = 1;
		if ((_z < _x - v && _z > c) || (_x < c && _z > -_x + (N - 1) * 2 * v) || (_z > _x && _z < c) || (_x > c && _z < -_x + (N - 1) * 2 * v))
			blocks[i].offsetSideFix[1] = 1;
			*/
		//if (blocks[i].offsetSideFix[0] == true || blocks[i].offsetSideFix[1] == true || blocks[i].offsetSideFix[2] == true) {
			//blocks[i].offsetSideFix[0] = true;
			//blocks[i].offsetSideFix[1] = true;
			//blocks[i].offsetSideFix[2] = true;
		//}

		/*
		else if (a < c && b > -a + (N - 1) * 2 * v)
			blocks[i].offset_ *= -1;
		else if (b > a && b < c)
			blocks[i].offset_ *= -1;
		else if (a > c && b < -a + (N - 1) * 2 * v)
			blocks[i].offset_ *= -1;
			*/

		/*
			blocks[i].color[0] = glm::vec3(0.9, 0.1, 0.1); //red
			blocks[i].color[1] = glm::vec3(0.9, 0.9, 0.9); //white
			blocks[i].color[2] = glm::vec3(0.9, 0.5, 0.1); //orange
			blocks[i].color[3] = glm::vec3(0.9, 0.9, 0.1); //yellow
			blocks[i].color[4] = glm::vec3(0.1, 0.1, 0.9); //blue
			blocks[i].color[5] = glm::vec3(0.1, 0.9, 0.1); //green
		*/
		/*
		for (int l = 0; l < 6; l++)
			blocks[i].color[l] = glm::vec3(0, 0, 0);

		if (blocks[i].offset_[TESTTEST] == 1)
			for (int l = 0; l < 6; l++)
				blocks[i].color[l].y = .5;
		if (blocks[i].offset_[TESTTEST] == 2)
			for (int l = 0; l < 6; l++)
				blocks[i].color[l].y = 1.;
		if (blocks[i].radius_[TESTTEST] == 1)
			for (int l = 0; l < 6; l++)
				blocks[i].color[l].x = .5;
		if (blocks[i].radius_[TESTTEST] == 2)
			for (int l = 0; l < 6; l++)
				blocks[i].color[l].x = 1.;
		if (blocks[i].offset_[TESTTEST] == 7)
			for (int l = 0; l < 6; l++)
				blocks[i].color[l].z = 1;
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
		if (rotateCounter >= 0 && time > 2) {
			float time = rotateCounter * M_PI / 2. / ANIMATION_DURATION;

			for (int j = 0; j < n_blocks; j++) {
				if (blocks[j].roll)
					blocks[j].rotate((j + 1) % 2, axis, time, direction);
			}
			if (rotateCounter == 0) {
				//roll reset && block update
				for (int j = 0; j < n_blocks; j++) {
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
		camera.Matrix(45.0f, 0.5f, 50.0f, shaderProgram, "camMatrix");
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
	delete2dArray(rotateIndex, N*N);
	return 0;
}