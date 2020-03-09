// Winter 2020

#include "A2.hpp"
#include "cs488-framework/GlErrorCheck.hpp"

#include <iostream>
using namespace std;

#include <imgui/imgui.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/io.hpp>
using namespace glm;

//----------------------------------------------------------------------------------------
// Constructor
VertexData::VertexData()
: numVertices(0), index(0){
	positions.resize(kMaxVertices);
	colours.resize(kMaxVertices);
}


//----------------------------------------------------------------------------------------
// Constructor
A2::A2()
: m_currentLineColour(vec3(0.0f)){

}

//----------------------------------------------------------------------------------------
// Destructor
A2::~A2(){}

//----------------------------------------------------------------------------------------
/*
 * Called once, at program start.
 */
void A2::init(){

	cout << "START!" << endl;

	// Set the background colour.
	glClearColor(0.3, 0.5, 0.7, 1.0);

	createShaderProgram();

	glGenVertexArrays(1, &m_vao);

	enableVertexAttribIndices();

	generateVertexBuffers();

	mapVboDataToVertexAttributeLocation();
	
	reset();

}


//**

void A2::reset(){
	
	initModel();
	initView();

	fov = 30.0; // field of view
	near = 5; // near
	far = 20; // far
	
	mouse_x_pos = 0;
	mode_num = 0;

	clear_all();
	initCube();
	initGnomon();
	initViewPort();
}

void A2::clear_all(){
	cube_v.clear();
	w_gnomon.clear();
	m_gnomon.clear();
	view_port.clear();
}

void A2::initCube(){

	//back
	cube_v.push_back({vec4(-1.0f,-1.0f, 1.0f, 1), vec4( 1.0f, -1.0f, 1.0f, 1)});
	cube_v.push_back({vec4(-1.0f, 1.0f, 1.0f, 1), vec4( 1.0f,  1.0f, 1.0f, 1)});
	cube_v.push_back({vec4(-1.0f,-1.0f, 1.0f, 1), vec4(-1.0f,  1.0f, 1.0f, 1)});
	cube_v.push_back({vec4( 1.0f,-1.0f, 1.0f, 1), vec4( 1.0f,  1.0f, 1.0f, 1)});

	//side
	cube_v.push_back({vec4( 1.0f,-1.0f, 1.0f, 1), vec4( 1.0f, -1.0f,-1.0f, 1)});
	cube_v.push_back({vec4(-1.0f,-1.0f,-1.0f, 1), vec4(-1.0f, -1.0f, 1.0f, 1)});
	cube_v.push_back({vec4( 1.0f, 1.0f, 1.0f, 1), vec4( 1.0f,  1.0f,-1.0f, 1)});
	cube_v.push_back({vec4(-1.0f, 1.0f, 1.0f, 1), vec4(-1.0f,  1.0f,-1.0f, 1)});

	//face
	cube_v.push_back({vec4( 1.0f,-1.0f,-1.0f, 1), vec4( 1.0f,  1.0f,-1.0f, 1)});
	cube_v.push_back({vec4(-1.0f,-1.0f,-1.0f, 1), vec4(-1.0f,  1.0f,-1.0f, 1)});
	cube_v.push_back({vec4( 1.0f, 1.0f,-1.0f, 1), vec4(-1.0f,  1.0f,-1.0f, 1)});
	cube_v.push_back({vec4(-1.0f,-1.0f,-1.0f, 1), vec4( 1.0f, -1.0f,-1.0f, 1)});
	
}

void A2::initGnomon(){

	m_gnomon.push_back({vec4(0,0,0,1), vec4(1.0f, 0.0f, 0.0f, 1)}); // x
	m_gnomon.push_back({vec4(0,0,0,1), vec4(0.0f, 1.0f, 0.0f, 1)}); // y
	m_gnomon.push_back({vec4(0,0,0,1), vec4(0.0f, 0.0f, 1.0f, 1)});	// z

	w_gnomon.push_back({vec4(0,0,0,1), vec4(1.0f, 0.0f, 0.0f, 1)}); // x
	w_gnomon.push_back({vec4(0,0,0,1), vec4(0.0f, 1.0f, 0.0f, 1)}); // y
	w_gnomon.push_back({vec4(0,0,0,1), vec4(0.0f, 0.0f, 1.0f, 1)}); // z
}

void A2::initModel(){
	M = mat4();
	g_M = M;
}

void A2::initView(){
	V = mat4(vec4(1,0,0,0), vec4(0,1,0,0), vec4(0,0,1,0), vec4(0,0,15,1));
}

void A2::initProjection(){
	P = mat4(vec4(1/tan(radians(fov/2)),					0,						0,		0),
			 vec4(0,					1/tan(radians(fov/2)),						0,		0),
			 vec4(0,										0,	(far+near)/(far-near),		1),
			 vec4(0,										0, -2*far*near/(far-near), 		0));
}


void A2::initViewPort(){
	view_port.push_back({-0.90, 0.90});
	view_port.push_back({0.90, 0.90});
	view_port.push_back({0.90, -0.90});
	view_port.push_back({-0.90, -0.90});
}

void A2::fit_in_vp(vec2 v0, vec2 v1, bool is_gnomon){

	mat4 s = scale(mat4(), vec3(0.90, 0.90, 1));
	mat4 t = translate(mat4(), vec3(0, 0, 0));

	vec4 p1 = t * s * vec4(v0.x, v0.y, 0, 1);
	vec4 p2 = t * s * vec4(v1.x, v1.y, 0, 1);

	drawLine({p1.x, p1.y}, {p2.x, p2.y});
	
}

void A2::edge_seg(vec4 &v1, vec4 &v2,
	vector<float> l1, vector<float> l2){

	for (int i = 0; i < 6; i++){
		float t = 0.0;
		t = l1[i]  / (l1[i]-l2[i]);
		if(l1[i] < 0){
			v1 = (1 - t) * v1 + t * v2;
		}
		if(l2[i] < 0){
			v2 = (1 - t) * v1 + t * v2;
		}
	}
}

bool A2::is_clip(vec4& v1, vec4& v2){
	vector<float> v_1;
	vector<float> v_2;
	int c1 = 0;
	int c2 = 0;

	v_1.push_back(v1.w + v1.x);
	v_1.push_back(v1.w - v1.x);
	v_1.push_back(v1.w + v1.y);
	v_1.push_back(v1.w - v1.y);
	v_1.push_back(v1.w + v1.z);
	v_1.push_back(v1.w - v1.z);

	v_2.push_back(v2.w + v2.x);
	v_2.push_back(v2.w - v2.x);
	v_2.push_back(v2.w + v2.y);
	v_2.push_back(v2.w - v2.y);
	v_2.push_back(v2.w + v2.z);
	v_2.push_back(v2.w - v2.z);

	for(int i = 0; i < 6; i++){
		if(v_1[i] < 0){
			c1 = c1 << 1 | 1;
		}
		else{
			c1 = c1 << 1 | 0;
		}
		if(v_2[i] < 0){
			c2 = c2 << 1 | 1;
		}
		else{
			c2 = c2 << 1 | 0;
		}
	}

	if (c1 & c2){
		return false;
	}
	
	if ((c1 | c2) == 0){
		return true;
	}

	edge_seg(v1, v2, v_1, v_2);

    return true;
}


// course note P 45
mat4 A2::translate(mat4 m, vec3 v){
	mat4 T = mat4(vec4(1,	    0,	  0,	v.x),
				  vec4(0,	    1,	  0,	v.y),
				  vec4(0,		0,    1,	v.z),
				  vec4(0,		0, 	  0,	1));

	// why transpose?
	// OpenGL use column-major matrix ordering
	// http://www.mindcontrol.org/~hplus/graphics/matrix-layout.html
	T = transpose(T);
	
	return T * m;
	
}

mat4 A2::rotate(mat4 m, vec3 v){
	
	mat4 R_z = mat4(vec4(cos(v.z),		-sin(v.z),		0,		0),
	 				vec4(sin(v.z),	   	 cos(v.z),		0,		0),
	 				vec4(0,				0,				1,		0),
	 				vec4(0,				0,	  			0,		1));

	mat4 R_x = mat4(vec4(1,				0,				0,		0),
					vec4(0,		 cos(v.x),	  	-sin(v.x),		0),
					vec4(0,		 sin(v.x),		 cos(v.x),		0),
					vec4(0,				0,	  			0,		1));

	mat4 R_y = mat4(vec4(cos(v.y),		0,		 sin(v.y),		0),
					vec4(0,		 		1,	  	 		0,		0),
					vec4(-sin(v.y),		0,		 cos(v.y),		0),
					vec4(0,				0,	  			0,		1));

	return R_x * R_y * R_z * m;
}

mat4 A2::scale(mat4 m, vec3 v){
	mat4 S = mat4(vec4(v.x,		0,	  0,	0), 
				  vec4(0,	  v.y,	  0,	0),
				  vec4(0,		0,  v.z,	0),
				  vec4(0,		0,	  0,	1));
	return S * m;
}



//

//----------------------------------------------------------------------------------------
void A2::createShaderProgram()
{
	m_shader.generateProgramObject();
	m_shader.attachVertexShader( getAssetFilePath("VertexShader.vs").c_str() );
	m_shader.attachFragmentShader( getAssetFilePath("FragmentShader.fs").c_str() );
	m_shader.link();
}

//---------------------------------------------------------------------------------------- Winter 2020
void A2::enableVertexAttribIndices()
{
	glBindVertexArray(m_vao);

	// Enable the attribute index location for "position" when rendering.
	GLint positionAttribLocation = m_shader.getAttribLocation( "position" );
	glEnableVertexAttribArray(positionAttribLocation);

	// Enable the attribute index location for "colour" when rendering.
	GLint colourAttribLocation = m_shader.getAttribLocation( "colour" );
	glEnableVertexAttribArray(colourAttribLocation);

	// Restore defaults
	glBindVertexArray(0);

	CHECK_GL_ERRORS;
}

//----------------------------------------------------------------------------------------
void A2::generateVertexBuffers()
{
	// Generate a vertex buffer to store line vertex positions
	{
		glGenBuffers(1, &m_vbo_positions);

		glBindBuffer(GL_ARRAY_BUFFER, m_vbo_positions);

		// Set to GL_DYNAMIC_DRAW because the data store will be modified frequently.
		glBufferData(GL_ARRAY_BUFFER, sizeof(vec2) * kMaxVertices, nullptr,
				GL_DYNAMIC_DRAW);


		// Unbind the target GL_ARRAY_BUFFER, now that we are finished using it.
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		CHECK_GL_ERRORS;
	}

	// Generate a vertex buffer to store line colors
	{
		glGenBuffers(1, &m_vbo_colours);

		glBindBuffer(GL_ARRAY_BUFFER, m_vbo_colours);

		// Set to GL_DYNAMIC_DRAW because the data store will be modified frequently.
		glBufferData(GL_ARRAY_BUFFER, sizeof(vec3) * kMaxVertices, nullptr,
				GL_DYNAMIC_DRAW);


		// Unbind the target GL_ARRAY_BUFFER, now that we are finished using it.
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		CHECK_GL_ERRORS;
	}
}

//----------------------------------------------------------------------------------------
void A2::mapVboDataToVertexAttributeLocation()
{
	// Bind VAO in order to record the data mapping.
	glBindVertexArray(m_vao);

	// Tell GL how to map data from the vertex buffer "m_vbo_positions" into the
	// "position" vertex attribute index for any bound shader program.
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo_positions);
	GLint positionAttribLocation = m_shader.getAttribLocation( "position" );
	glVertexAttribPointer(positionAttribLocation, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

	// Tell GL how to map data from the vertex buffer "m_vbo_colours" into the
	// "colour" vertex attribute index for any bound shader program.
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo_colours);
	GLint colorAttribLocation = m_shader.getAttribLocation( "colour" );
	glVertexAttribPointer(colorAttribLocation, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

	//-- Unbind target, and restore default values:
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	CHECK_GL_ERRORS;
}

//---------------------------------------------------------------------------------------
void A2::initLineData()
{
	m_vertexData.numVertices = 0;
	m_vertexData.index = 0;
}

//---------------------------------------------------------------------------------------
void A2::setLineColour (const glm::vec3 & colour) {
	m_currentLineColour = colour;
}

//---------------------------------------------------------------------------------------
void A2::drawLine(
		const glm::vec2 & V0,   // Line Start (NDC coordinate)
		const glm::vec2 & V1    // Line End (NDC coordinate)
) {

	m_vertexData.positions[m_vertexData.index] = V0;
	m_vertexData.colours[m_vertexData.index] = m_currentLineColour;
	++m_vertexData.index;
	m_vertexData.positions[m_vertexData.index] = V1;
	m_vertexData.colours[m_vertexData.index] = m_currentLineColour;
	++m_vertexData.index;

	m_vertexData.numVertices += 2;
}

//----------------------------------------------------------------------------------------
/*
 * Called once per frame, before guiLogic().
 */
void A2::appLogic()
{
	// Place per frame, application logic here ...

	// Call at the beginning of frame, before drawing lines:
	initLineData();
	initProjection();
	draw_cube();
	draw_w_gnomon();
	draw_m_gnomon();
	draw_viewport();

}

vec2 A2::normalize(vec4 v){
	return {v.x/v.w , v.y/v.w};
}

void A2::draw_cube(){
	for(int i = 0; i < 12; i++){
		if(i == 0){
			// back
			setLineColour(colour_purple);
		}
		else if(i == 4){
			// side
			setLineColour(colour_brown);
		}
		else if(i == 8){
			// face
			setLineColour(colour_orange);
		}
		vec4 v1 = P * V * M * cube_v[i].first;
		vec4 v2 = P * V * M * cube_v[i].second;
		if(is_clip(v1, v2)){
			fit_in_vp(normalize(v1),normalize(v2), false);
		}		
	}
}

void A2::draw_w_gnomon(){
	for (int i = 0; i < 3; i++){
		vec4 v1_w = w_gnomon[i].first;
		vec4 v2_w = w_gnomon[i].second;
		if(i == 0){
			setLineColour(vec3(1.0f, 0.0f, 0.0f));
		}
		else if(i == 1){
			setLineColour(vec3(0.0f, 1.0f, 0.0f));
		}
		else if(i == 2){
			setLineColour(vec3(0.0f, 0.0f, 1.0f));
		}
		v1_w = P * V * v1_w;
		v2_w = P * V * v2_w;
		if(is_clip(v1_w, v2_w)){
			fit_in_vp(normalize(v1_w),normalize(v2_w), false);
		}
	}
}

void A2::draw_m_gnomon(){
	for (int i = 0; i < 3; i++){
		vec4 v1_m = m_gnomon[i].first;
		vec4 v2_m = m_gnomon[i].second;
		if(i == 0){
			setLineColour(vec3(1.0f, 0.0f, 0.0f));
		}
		else if(i == 1){
			setLineColour(vec3(0.0f, 1.0f, 0.0f));
		}
		else if(i == 2){
			setLineColour(vec3(0.0f, 0.0f, 1.0f));
		}
		// unscale model gnomon
		v1_m = P * V * g_M * v1_m;
		v2_m = P * V * g_M * v2_m;
		if(is_clip(v1_m, v2_m)){
			fit_in_vp(normalize(v1_m),normalize(v2_m), false);
		}
	}
}

void A2::draw_viewport(){
	setLineColour(colour_black);
	drawLine(view_port[0], view_port[1]);
	drawLine(view_port[1], view_port[2]);
	drawLine(view_port[2], view_port[3]);
	drawLine(view_port[3], view_port[0]);
}

//----------------------------------------------------------------------------------------
/*
 * Called once per frame, after appLogic(), but before the draw() method.
 */
void A2::guiLogic(){
	static bool firstRun(true);
	if (firstRun) {
		ImGui::SetNextWindowPos(ImVec2(50, 50));
		firstRun = false;
	}

	static bool showDebugWindow(true);
	ImGuiWindowFlags windowFlags(ImGuiWindowFlags_AlwaysAutoResize);
	float opacity(0.5f);

	ImGui::Begin("Properties", &showDebugWindow, ImVec2(100,100), opacity,
			windowFlags);


		// Add more gui elements here here ...
		ImGui::PushID(0);
		if(ImGui::RadioButton("##Inst", &mode_num, 0)){
				cout << "Rotate View" << endl;
		}
		ImGui::SameLine();
		ImGui::Text("Rotate View O");
		ImGui::PopID();
		
		ImGui::PushID(1);
		if(ImGui::RadioButton("##Inst", &mode_num, 1)){
				cout << "Translate View" << endl;
		}
		ImGui::SameLine();
		ImGui::Text("Translate View E");
		ImGui::PopID();
		
		
		ImGui::PushID(2);
		if(ImGui::RadioButton("##Inst", &mode_num, 2)){
				cout << "Perspective" << endl;
		}
		ImGui::SameLine();
		ImGui::Text("Perspective P");
		ImGui::PopID();
		
		ImGui::PushID(3);
		if(ImGui::RadioButton("##Inst", &mode_num, 3)){
				cout << "Rotate Model" << endl;
		}
		ImGui::SameLine();
		ImGui::Text("Rotate Model R");
		ImGui::PopID();
		
		ImGui::PushID(4);
		if(ImGui::RadioButton("##Inst", &mode_num, 4)){
				cout << "Translate Model" << endl;
		}
		ImGui::SameLine();
		ImGui::Text("Translate Model T");
		ImGui::PopID();
		
		
		ImGui::PushID(5);
		if(ImGui::RadioButton("##Inst", &mode_num, 5)){
				cout << "Scale S" << endl;
		}
		ImGui::SameLine();
		ImGui::Text("Scale Model S");
		ImGui::PopID();
		
		
		ImGui::PushID(6);
		if(ImGui::RadioButton("##Inst", &mode_num, 6)){
				cout << "Viewport" << endl;
		}
		ImGui::SameLine();
		ImGui::Text("Viewport V");
		ImGui::PopID();
		
		if ( ImGui::Button( "Reset A" )){
				cout << "Reset!" << endl;
				reset();
		}

		if( ImGui::Button( "Quit Application Q" ) ) {
			glfwSetWindowShouldClose(m_window, GL_TRUE);
		}

		ImGui::Text( "Framerate: %.1f FPS", ImGui::GetIO().Framerate );

	ImGui::End();
}

//----------------------------------------------------------------------------------------
void A2::uploadVertexDataToVbos() {

	//-- Copy vertex position data into VBO, m_vbo_positions:
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo_positions);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vec2) * m_vertexData.numVertices,
				m_vertexData.positions.data());
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		CHECK_GL_ERRORS;
	}

	//-- Copy vertex colour data into VBO, m_vbo_colours:
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo_colours);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vec3) * m_vertexData.numVertices,
				m_vertexData.colours.data());
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		CHECK_GL_ERRORS;
	}
}

//----------------------------------------------------------------------------------------
/*
 * Called once per frame, after guiLogic().
 */
void A2::draw()
{
	uploadVertexDataToVbos();

	glBindVertexArray(m_vao);

	m_shader.enable();
		glDrawArrays(GL_LINES, 0, m_vertexData.numVertices);
	m_shader.disable();

	// Restore defaults
	glBindVertexArray(0);

	CHECK_GL_ERRORS;
}

//----------------------------------------------------------------------------------------
/*
 * Called once, after program is signaled to terminate.
 */
void A2::cleanup(){

}

//----------------------------------------------------------------------------------------
/*
 * Event handler.  Handles cursor entering the window area events.
 */
bool A2::cursorEnterWindowEvent (int entered) {
	bool eventHandled(false);

	// Fill in with event handling code...

	return eventHandled;
}

//----------------------------------------------------------------------------------------
/*
 * Event handler.  Handles mouse cursor movement events.
 */
bool A2::mouseMoveEvent (double xPos,double yPos) {
	bool eventHandled(false);

	// Fill in with event handling code...
	if(!ImGui::IsMouseHoveringAnyWindow()){

		double dx = xPos - mouse_x_pos;
		
		if(mode_num == 0){
			if(ImGui::IsMouseDown(0)){
				V = rotate(mat4(), vec3(dx * 0.01, 0,0)) * V;
			}
			if(ImGui::IsMouseDown(1)){
				V = rotate(mat4(), vec3(0, 0, dx * 0.01)) * V;
			}
			if(ImGui::IsMouseDown(2)){
				V = rotate(mat4(), vec3(0, dx * 0.01,0)) * V;				
			}
		}
		else if(mode_num == 1){
			if(ImGui::IsMouseDown(0)){
				V = translate(mat4(), vec3(dx * 0.01, 0,0)) * V;
			}
			if(ImGui::IsMouseDown(1)){
				V = translate(mat4(), vec3(0, 0, dx * 0.01)) * V;
			}
			if(ImGui::IsMouseDown(2)){
				V = translate(mat4(), vec3(0, dx * 0.01,0)) * V;			
			}	
		}
		else if(mode_num == 2){
			if(ImGui::IsMouseDown(0)){
				float f = fov + dx * 0.01;
				if(f <= 160 && f >= 5){
					fov = f;	
				}
			}
			if(ImGui::IsMouseDown(1)){
				float ff = far + dx * 0.01;
				if(ff > 1 + near){
					far = ff;
				}
			}
			if(ImGui::IsMouseDown(2)){
				float nn = near + dx * 0.01;
				if(far > 1 + nn){
					if(nn >= 0){
						near = nn;
					}
				}
			}
			eventHandled = true;
		}
		else if(mode_num == 3){
			if(ImGui::IsMouseDown(0)){
				M = M * rotate(mat4(), vec3(dx * 0.01, 0,0));
				g_M = g_M * rotate(mat4(), vec3(dx * 0.01, 0,0));
				
			}
			if(ImGui::IsMouseDown(1)){
				M = M * rotate(mat4(), vec3(0,0,dx * 0.01));
				g_M = g_M * rotate(mat4(), vec3(0,0,dx * 0.01));
			}
			if(ImGui::IsMouseDown(2)){
				M = M * rotate(mat4(), vec3(0,dx * 0.01, 0));
				g_M = g_M * rotate(mat4(), vec3(0,dx * 0.01, 0));
			}
		}
		else if(mode_num == 4){
			if(ImGui::IsMouseDown(0)){
				M = M * translate(mat4(), vec3(dx * 0.01, 0,0));
				g_M = g_M * translate(mat4(), vec3(dx * 0.01, 0,0));
			}
			if(ImGui::IsMouseDown(1)){
				M = M * translate(mat4(), vec3(0,0,dx * 0.01));
				g_M = g_M * translate(mat4(), vec3(0,0,dx * 0.01));
			}
			if(ImGui::IsMouseDown(2)){
				M = M * translate(mat4(), vec3(0,dx * 0.01, 0));
				g_M = g_M * translate(mat4(), vec3(0,dx * 0.01, 0));
			}
		}		
		else if(mode_num == 5){
			if(ImGui::IsMouseDown(0)){
				M = M * scale(mat4(), vec3(1+dx * 0.01, 1,1));
			}
			if(ImGui::IsMouseDown(1)){
				M = M * scale(mat4(), vec3(1,1,1+dx * 0.01));	
			}
			if(ImGui::IsMouseDown(2)){
				M = M * scale(mat4(), vec3(1,1+dx * 0.01, 1));
			}
		}
		else if(mode_num == 6){

		}
		
		mouse_x_pos = xPos;
		
	}
	return eventHandled;
}

//----------------------------------------------------------------------------------------
/*
 * Event handler.  Handles mouse button events.
 */
bool A2::mouseButtonInputEvent (int button,int actions, int mods) {
	bool eventHandled(false);

	// Fill in with event handling code...

	return eventHandled;
}

//----------------------------------------------------------------------------------------
/*
 * Event handler.  Handles mouse scroll wheel events.
 */
bool A2::mouseScrollEvent (double xOffSet, double yOffSet) {
	bool eventHandled(false);

	// Fill in with event handling code...

	return eventHandled;
}

//----------------------------------------------------------------------------------------
/*
 * Event handler.  Handles window resize events.
 */
bool A2::windowResizeEvent (int width,int height) {
	bool eventHandled(false);

	// Fill in with event handling code...

	return eventHandled;
}

//----------------------------------------------------------------------------------------
/*
 * Event handler.  Handles key input events.
 */
bool A2::keyInputEvent (int key, int action, int mods) {
	bool eventHandled(false);

	// Fill in with event handling code...
	if(action == GLFW_PRESS){
		if(key == GLFW_KEY_O){
			mode_num = 0;
			cout << "Rotate View" << endl;
			eventHandled = true;
		}
		if(key == GLFW_KEY_E){
			mode_num = 1;
			cout << "Translate View" << endl;
			eventHandled = true;
		}
		if(key == GLFW_KEY_P){
			mode_num = 2;
			cout << "Perspective" << endl;
			eventHandled = true;
		}
		if(key == GLFW_KEY_R){
			mode_num = 3;
			cout << "Rotate Model" << endl;
			eventHandled = true;
		}
		if(key == GLFW_KEY_T){
			mode_num = 4;
			cout << "Translate Model" << endl;
			eventHandled = true;
		}
		if(key == GLFW_KEY_S){
			mode_num = 5;
			cout << "Scale Model" << endl;
			eventHandled = true;
		}
		if(key == GLFW_KEY_V){
			mode_num = 6;
			cout << "Viewport " << endl;
			eventHandled = true;
		}
		if(key == GLFW_KEY_Q){
			cout << "QUIT" << endl;
			glfwSetWindowShouldClose(m_window, GL_TRUE);
			eventHandled = true;
		}
		if(key == GLFW_KEY_A){
			cout << "RESET" << endl;
			reset();
			eventHandled = true;
		}
	}
	return eventHandled;
}
