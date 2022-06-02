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

extern void makeMoves(string code_s);
extern string generateScramble(int length);
extern void letsGoAiStart();
extern void letsGoAiLoop();
extern void letsGoAiEnd();
string code_s = "";

Block* GLOBALblocks;

ostream& operator<<(ostream& stream, const glm::vec3& v)
{
	stream << "(" << v.x << "," << v.y << "," << v.z << ")";
	return stream;
}

int N = 3;
float v = 1;

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
int ANIMATION_DURATION = 1;
int MOVEMENT_FREEZE_AFTER_MOVE = 0; //15
int AI_DELAY = 10;

int* order;
int* neworder;
int axis;
int* toRotate;
int direction;
//obracanie tablicy order w lewo lub prawo w zaleznosi od kierunku obrotu
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
int** rotateIndex; //ustawiane raz, na pocz¹tku dzia³ania programu na podstawie wielkoœci koœci
//reinterpretacja indexów na odpowiadaj¹ce w zale¿noœci od konkretnej œciany
int indexesOfRotationX(int j, int offset) {
	return order[N * j + offset];
}
int indexesOfRotationY(int j, int offset) {
	return order[j % N + j / N * N * N + offset*N];
}
int indexesOfRotationZ(int j, int offset) {
	return order[j + offset*N*N];
}
//generowanie rotateIndex'ów
void generateRotateIndexAxis() {
	rotateIndex = new int*[N * N];
	for (int i = 0; i < N * N; i++)
		rotateIndex[i] = new int[N * N];
	for (int i = 0; i < N * N; i++) {
		for (int j = 0; j < N; j++) {
			rotateIndex[3*j+0][i] = N * i + j; //x
			rotateIndex[3*j+1][i] = i % N + (i / N) * N * N + N*j; //y
			rotateIndex[3*j+2][i] = i + j*N*N; //z
		}
	}
}
//zwracanie odpowiedniego indexu po odwrotnej interpretacji tablicy rotateIndex
int reverseRotateIndex(int k, int axis) {
	for (int i = 0; i < N * N; i++) {
		for (int j = 0; j < N; j++) {
			if (rotateIndex[axis + j*3][i] == k)
				return i;
		}	
	}
	return -1;
}
//podstawowa funkcja poprzez ktora wydaje sie polecenie obrotu
void rotate(int direction, int indexsOfRotation(int,int), int offset) {
	float a, b;
	rotateCounter = ANIMATION_DURATION;
	toRotate = rotateIndex[axis];
	axis %= 3;
	for (int i = 0, j = 0; j < N*N; j++) {
		//zamiana offsetów i radiusów przy nieparzystej iloœci obrotów danej œciany, fix do krawêdzi
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
		//reinterpretacja i w zale¿noœci od j i wzoru na i
		i = indexsOfRotation(j,offset);
		//info o tym, ¿e bêdzie siê dany blok obraca³
		GLOBALblocks[i].roll = true;
		//oblicanie offsetSideFixów -> generalnie po której stronie jest dany blok (dodatniej czy ujemnej)
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
		//obliczanie k¹ta przesuniêcia zglêdem po³o¿nienia 0 - blockOffsetFix, jest to k¹t obrotu (x * pi/2)
		//w zale¿noœci od osi rotacji, pod uwagê brane s¹ ró¿ne wspó³rzêdne
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
		float c = (N - 1) * v;
		if (N % 2) { // nieparzyste <-> poprawka wzglêdem œrodka o +-'v'
			if (b < c - v && a < c + v)
				GLOBALblocks[i].blockOffsetFix = 1 + direction;
			else if (b < c + v && a > c + v)
				GLOBALblocks[i].blockOffsetFix = 0 + direction;
			else if (b > c + v && a > c - v)
				GLOBALblocks[i].blockOffsetFix = 3 + direction;
			else if (b > c - v && a < c - v)
				GLOBALblocks[i].blockOffsetFix = 2 + direction;
		}
		else { //parzyste
			if (b < c && a < c)
				GLOBALblocks[i].blockOffsetFix = 1 + direction;
			else if (b < c && a > c)
				GLOBALblocks[i].blockOffsetFix = 0 + direction;
			else if (b > c && a > c)
				GLOBALblocks[i].blockOffsetFix = 3 + direction;
			else if (b > c && a < c)
				GLOBALblocks[i].blockOffsetFix = 2 + direction;
		}
		// przepe³enienie rotacji, zakres od 0 do 3
		if (direction) { //obrót w lewo (przeciwna strona do podstawowej)
			GLOBALblocks[i].rot[axis]++;
			if (GLOBALblocks[i].rot[axis] == 4) {
				GLOBALblocks[i].rot[axis] = 0;
				GLOBALblocks[i].prevRot[axis] = -1;
			}
		}
		else { //obrót w prawo
			GLOBALblocks[i].rot[axis]--;
			if (GLOBALblocks[i].rot[axis] == -1) {
				GLOBALblocks[i].rot[axis] = 3;
				GLOBALblocks[i].prevRot[axis] = 4;
			}
		}
	}
	//aktualizacja tablciy order
	orderUpdateRotateMatrix(direction);
}
// 0 -> waiting for input (M key), 1 -> waiting for move (FBLRUD or Escape), 2 -> input processed, reseting
int move_cube = 0;
int code_input = 0; 
int code_input_index = 0;
int row = 0;
int ai_go = 0;
int scramble_go = 0;
void input(GLFWwindow* window, glm::vec3& Position, glm::vec3& Orientation, glm::vec3& Up)
{
	if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) { //move
		if (ai_go == 0) {
			ai_go = 1;
			//code_input_index = 0;
		}		
		if (ai_go == 2)
			ai_go = 3;
	}
	if (glfwGetKey(window, GLFW_KEY_P) == GLFW_RELEASE) {
		if (ai_go == 1)
			ai_go = 2;
		if (ai_go == 3)
			ai_go = 0;
	}
	if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS) { //move
		if (scramble_go == 0)
			scramble_go = 1;
	}
	if (scramble_go == 1) {
		generateScramble(30);
		scramble_go = 2;
	}
	if (glfwGetKey(window, GLFW_KEY_O) == GLFW_RELEASE) {
		if (scramble_go == 2)
			scramble_go = 0;
	}
	Orientation = cameraFront;
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)          // sprawdzanie czy wybrany klawisz jest wciœniêty (Esc)     GLFW_PRESS | GLFW_RELEASE
		glfwSetWindowShouldClose(window, true);                     // zamykanie okienka
	if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS) { //move
		if (move_cube == 0)
			move_cube = 1;
	}
	if (glfwGetKey(window, GLFW_KEY_M) == GLFW_RELEASE) {
		if (move_cube==2)
			move_cube = 0;
	}
	//g³ówna logika inputu do obracania
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
		else if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
			if (direction) direction = 0; else direction = 1; //fix clockwise-anticlockwise
			axis = 0 + row * 3;
			rotate(direction, indexesOfRotationX,row);
			move_cube = 2;
		}
		else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
			if (direction) direction = 0; else direction = 1; //fix clockwise-anticlockwise
			axis = 1 + row * 3;
			rotate(direction, indexesOfRotationY,row);
			move_cube = 2;
		}
		else if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS) {
			if (direction) direction = 0; else direction = 1; //fix clockwise-anticlockwise
			axis = 2 + (N-1) * 3 - row * 3;
			rotate(direction, indexesOfRotationZ,N-1-row);
			move_cube = 2;
		}
		else if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) {
			axis = 0 + (N-1) * 3 - row * 3;
			rotate(direction, indexesOfRotationX,N-1-row);
			move_cube = 2;
		}
		else if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS) {
			axis = 1 + (N-1) * 3 - row * 3;
			rotate(direction, indexesOfRotationY,N-1-row);
			move_cube = 2;
		}
		else if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
			move_cube = 2;
		}
	}
	//ruch kamery
	else if (ANIMATION_DURATION - MOVEMENT_FREEZE_AFTER_MOVE > rotateCounter) {
		const float cameraSpeed = 20.0 * deltaTime; // adjust accordingly
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
	//input w postaci kodu
	//https://www.worldcubeassociation.org/regulations/history/files/scrambles/scramble_cube.htm?size=7&num=5&len=60&col=gwobyr&subbutton=Scramble%21
	if (code_input == 1) {
		getline(cin, code_s);
		code_s += " ";
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
			if (code_s[i] == '2' && code_s[i+1] == ' ') {		// zastapienie znaku 2
				code_s.erase(i, 1);
				code_s.insert(i,"^");
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
		//cerr << code_s << endl;
		code_input = 2;
		code_input_index = 0;	
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


int ai_counter = 0;
int width = 800, height = 800;
int main() {
	srand(time(NULL));
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	v = 3. / N;

	GLFWwindow* window = glfwCreateWindow(width, height, "glhf", NULL, NULL);
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
	//generowanie tablicy int** rotateIndex
	generateRotateIndexAxis();

	Block* blocks;
	blocks = new Block[pow(N, 3)];
	GLOBALblocks = blocks;
	int n_blocks = pow(N, 3);
	for (int i = 0; i < n_blocks; i++) {
		// RADIUS + OFFSET EACH AXIS
		// Z AXIS
		int k = i % (N * N);
		blocks[i].radius_[2] = (N - 1) / 2;
		for (int j = 1; j < (N + 1) / 2; j++) {
			if (k % N >= j && k % N < N - j && k / N >= j && k / N < N - j)
				blocks[i].radius_[2] = (N - 1) / 2 - j;
		}
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
		blocks[i].offset_[1] = (N - 1) / 2;
		for (int j = 1; j < (N + 1) / 2; j++) {
			if ((k % N >= j && k % N < N - j) || (k / N >= j && k / N < N - j))
				blocks[i].offset_[1] = (N - 1) / 2 - j;
		}
		// POZYCJA DANEGO BLOKU
		blocks[i].position = glm::vec3(i % N, (i / N)% N, i / (N*N)) * glm::vec3(2*v,2*v,2*v);
		// KOLORY KONKRETYNCH BLOKÓW
		// (-1): none, (0): white, (1): yellow, (2):red, (3): orange, (4): blue, (5): green
		if (i < 1*N*N) { //first side - blue
			blocks[i].color[0] = 4; //blue
			if (i % N == N-1) blocks[i].color[1] = 3; //orange
			if (i % N == 0) blocks[i].color[3] = 2; //red
			if (i % (N*N) >= N*(N-1)) blocks[i].color[4] = 1; //yellow
			if (i % (N*N) < N) blocks[i].color[5] = 0; //white
		}
		else if (i < N * N* (N-1)) {
			if (i % N == N-1) blocks[i].color[1] = 3; //orange
			if (i % N == 0) blocks[i].color[3] = 2; //red
			if (i % (N*N) >= N * (N - 1)) blocks[i].color[4] = 1; //yellow
			if (i % (N*N) < N) blocks[i].color[5] = 0; //white
		}
		else {
			blocks[i].color[2] = 5; //green
			if (i % N == N-1) blocks[i].color[1] = 3; //orange
			if (i % N == 0) blocks[i].color[3] = 2; //red
			if (i % (N*N) >= N * (N - 1)) blocks[i].color[4] = 1; //yellow
			if (i % (N*N) < N) blocks[i].color[5] = 0; //white
		}
	}
	float backgroud_r, backgroud_g, backgroud_b;
	glfwSwapInterval(1); //ograniczenie fps to synchronizacji vsync
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); //kontury
	glLineWidth(3);
	float time = 0;
	Camera camera(width, height, glm::vec3(-8,11,-8));
	cameraFront = glm::vec3(0.628001, -0.480989, 0.611772); pitch = -28.75; yaw = 44.25;
	glm::vec3 colorTemp;
	letsGoAiStart();
	//generateScramble(20);
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
		makeMoves(code_s);
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
			//rysowanie konkretnego kwadratu (2 trójk¹tów)
			blockEBO.Bind();
			for (int j = 0; j < 6; j++) {
				switch (blocks[i].color[j]) {
				// (-1): none, (0): white, (1): yellow, (2):red, (3): orange, (4): blue, (5): green
				case -1:
					colorTemp = glm::vec3(.1, .1, .1);
					break;
				case 0:
					colorTemp = glm::vec3(.9, .9, .9);
					break;
				case 1:
					colorTemp = glm::vec3(.9, .9, .1);
					break;
				case 2:
					colorTemp = glm::vec3(.9, .1, .1);
					break;
				case 3:
					colorTemp = glm::vec3(.9, .5, .1);
					break;
				case 4:
					colorTemp = glm::vec3(.1, .1, .9);
					break;
				case 5:
					colorTemp = glm::vec3(.1, .9, .1);
					break;
				}
				glUniform3f(colorLoc, colorTemp.x, colorTemp.y, colorTemp.z);
				glDrawElements(GL_TRIANGLES, 2*1*3, GL_UNSIGNED_INT, (void*)(6 * j * sizeof (GLfloat)));
			}
		}
		// AI
		if (ai_counter <= 0) {
			letsGoAiLoop();
			ai_counter = AI_DELAY;
		}	
		ai_counter--;
		// Odœwie¿ widok
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	letsGoAiEnd();
	glfwDestroyWindow(window); //Delete window before ending the program
	glfwTerminate(); //Terminate GLFW before ending the program

	blockVAO.Delete();
	blockVBO.Delete();
	blockEBO.Delete();
	delete[]order, neworder, toRotate, blocks;
	delete2dArray(rotateIndex, N*N);
	return 0;
}