#include "ShaderClass.h"

string getFileContents(const char* filename) {
	ifstream in(filename, ios::binary);
	if (in) {
		string contents;
		in.seekg(0, ios::end);
		contents.resize(in.tellg());

		in.seekg(0, ios::beg);
		in.read(&contents[0], contents.size());

		in.close();
		return contents;
	}
	throw (errno);
}

Shader::Shader(const char* vertexFile, const char* fragmentFile) {
	string vertexCode = getFileContents(vertexFile);
	string fragmentCode = getFileContents(fragmentFile);

	const char* vertexSource = vertexCode.c_str();
	const char* fragmentSource = fragmentCode.c_str();


	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	// Po³¹cz istniej¹cy obiekt z napisan¹ wczeœniej implementacj¹ shadera
	glShaderSource(vertexShader, 1, &vertexSource, NULL);
	// Skompiluj gotowy kod
	glCompileShader(vertexShader);
	// Powtórz operacjê dla fragment shadera
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
	glCompileShader(fragmentShader);
	// Utwórz program
	ID = glCreateProgram();
	// Pod³¹cz shadery pod program
	glAttachShader(ID, vertexShader);
	glAttachShader(ID, fragmentShader);
	glLinkProgram(ID);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

void Shader::Activate() {
	glUseProgram(ID);
}

void Shader::Delete() {
	glDeleteProgram(ID);
}