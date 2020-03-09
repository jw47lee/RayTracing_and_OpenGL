// Winter 2020

#pragma once

#include <glm/glm.hpp>

#include "cs488-framework/CS488Window.hpp"
#include "cs488-framework/OpenGLImport.hpp"
#include "cs488-framework/ShaderProgram.hpp"

#include "maze.hpp"
#include <memory>
#include <vector>

using namespace std;

class A1 : public CS488Window {
public:
	A1();
	virtual ~A1();

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

private:
	void initGrid();
	void init_cube(glm::vec4 p, GLfloat s);
	void init_avatar(glm::vec4 p, GLfloat s);
	void init_floor(glm::vec4 p, GLfloat fs);
	void draw_cube(int x, int z, int h);
	void draw_avatar(int x, int z, int h);
	void draw_floor(int x, int z);
	void reset();
	// Fields related to the shader and uniforms.
	ShaderProgram m_shader;
	GLint P_uni; // Uniform location for Projection matrix.
	GLint V_uni; // Uniform location for View matrix.
	GLint M_uni; // Uniform location for Model matrix.
	GLint col_uni;   // Uniform location for cube colour.
	shared_ptr<Maze> m;
	float zoom_factor;
	int zoom_num = 0;
	
	// Fields related to grid geometry.
	GLuint m_grid_vao; // Vertex Array Object
	GLuint m_grid_vbo; // Vertex Buffer Object
	
	// Fields related to cube geometry.
	GLuint m_vao;   // Vertex Array Object
	GLuint m_vbo;   // Vertex Buffer Object
	GLuint m_ebo;   // Element Buffer Object
	
	// Fields related to floor geometry.
	GLuint f_vao;
	GLuint f_vbo;
	GLuint f_ebo;
	
	// Fields related to avatar geometry.
	GLuint a_vao;
	GLuint a_vbo;
	GLuint a_ebo;
	int lats = 100;
	int longs = 100;
	int num_tri_a = 0;
	std::vector<GLfloat> v;
	std::vector<GLuint> id;
	// Matrices controlling the camera and projection.
	glm::mat4 proj;
	glm::mat4 view;
	
	int level;

	float colour[3];
	float fcolour[3];
	float acolour[3];
	
	int a_x, a_z;

	bool is_shifted = false;


	int current_col; // 0 = cube, 1 = floor, 2= avatar;
	float rotation_x, rotation_y;
	double mouse_x, mouse_y;
	double diff_x, diff_y;
	
	//std::shared_ptr<Cube> firstCube;
};
