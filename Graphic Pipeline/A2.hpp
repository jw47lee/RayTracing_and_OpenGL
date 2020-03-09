// Winter 2020

#pragma once

#include "cs488-framework/CS488Window.hpp"
#include "cs488-framework/OpenGLImport.hpp"
#include "cs488-framework/ShaderProgram.hpp"

#include <glm/glm.hpp>
#include <sstream>
#include <vector>

//***
using namespace std;
using namespace glm;
//


// Set a global maximum number of vertices in order to pre-allocate VBO data
// in one shot, rather than reallocating each frame.
const GLsizei kMaxVertices = 1000;


// Convenience class for storing vertex data in CPU memory.
// Data should be copied over to GPU memory via VBO storage before rendering.
class VertexData {
public:
	VertexData();
	std::vector<glm::vec2> positions;
	std::vector<glm::vec3> colours;
	GLuint index;
	GLsizei numVertices;
};


class A2 : public CS488Window {
public:
	A2();
	virtual ~A2();

	void clear_all();
	void reset();

	// init
	void initModel();
	void initView();
	void initProjection();
	void initCube();
	void initGnomon();
	void initViewPort();
	
	// draw
	void draw_cube();
	void draw_m_gnomon();
	void draw_w_gnomon();
	void draw_viewport();
	void fit_in_vp(vec2 v0, vec2 v1, bool is_gnomon);
	bool is_clip(vec4 &v1, vec4 &v2);
	void edge_seg(vec4 &v1, vec4 &v2,vector<float> l1, vector<float> l2);

	//helper
	vec2 normalize(vec4 v);

protected:
	virtual void init() override;
	virtual void appLogic() override;
	virtual void guiLogic() override;
	virtual void draw() override;
	virtual void cleanup() override;

	virtual bool cursorEnterWindowEvent(int entered) override;
	virtual bool mouseMoveEvent(double xPos, double yPos) override;
	virtual bool mouseButtonInputEvent(int button, int actions, int mods) override;
	virtual bool mouseScrollEvent(double xOffSet, double yOffSet) override;
	virtual bool windowResizeEvent(int width, int height) override;
	virtual bool keyInputEvent(int key, int action, int mods) override;

	void createShaderProgram();
	void enableVertexAttribIndices();
	void generateVertexBuffers();
	void mapVboDataToVertexAttributeLocation();
	void uploadVertexDataToVbos();
	void initLineData();
	void setLineColour(const glm::vec3 & colour);
	void drawLine (const glm::vec2 & v0, const glm::vec2 & v1);

	ShaderProgram m_shader;

	GLuint m_vao;            // Vertex Array Object
	GLuint m_vbo_positions;  // Vertex Buffer Object
	GLuint m_vbo_colours;    // Vertex Buffer Object

	VertexData m_vertexData;

	glm::vec3 m_currentLineColour;

//**
private:

			
	mat4 V; // View transformation
	mat4 M; // Model transformation
	mat4 P; // Projection transformation
	mat4 g_M; // Model_gnomon transformation, scale not applied
	
	float fov; // Field of View in degree
	float near, far;
	
	// GUI
	double mouse_x_pos;
	int mode_num = 0;

	// cube vertices
	vector<pair<vec4, vec4>> cube_v;

	// gnomons vertices
	vector<pair<vec4, vec4>> w_gnomon;
	vector<pair<vec4, vec4>> m_gnomon;
	
	// viewport
	vector<vec2> view_port;

	// Transformation
	mat4 translate(mat4 m, vec3 v);
	mat4 rotate(mat4 m, vec3 v);
	mat4 scale(mat4 m, vec3 v);

	// Color
	// gnomons
	const vec3 colour_red = vec3(1.0f, 0.0f, 0.0f);
	const vec3 colour_green = vec3(0.0f, 1.0f, 0.0f);
	const vec3 colour_blue = vec3(0.0f, 0.0f, 1.0f);

	// cube
	const vec3 colour_orange = vec3(1.0f, 0.65f, 0.0f); // cube back
	const vec3 colour_purple = vec3(0.58f, 0.0f, 0.82f); //cube front
	const vec3 colour_brown = vec3(0.55f, 0.3f, 0.1f); 	//cube side

	// viewport
	const vec3 colour_black = vec3(0.0f, 0.0f, 0.0f);

};
