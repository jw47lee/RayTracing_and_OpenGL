// Winter 2020

#include "A1.hpp"
#include "cs488-framework/GlErrorCheck.hpp"

#include <iostream>
#include <memory>
#include <sstream>

#include <sys/types.h>
#include <unistd.h>

#include <imgui/imgui.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace glm;


static const size_t DIM = 16;

//----------------------------------------------------------------------------------------
// Constructor
A1::A1()
	: current_col( 0 )
{
	level = 1;
	colour[0] = 0.0f;
	colour[1] = 0.0f;
	colour[2] = 0.0f;
	fcolour[0] = 1.0f;
	fcolour[1] = 1.0f;
	fcolour[2] = 1.0f;
	acolour[0] = 0.8f;
	acolour[1] = 0.0f;
	acolour[2] = 0.0f;
	a_x = 0;
	a_z = 0;


}

//----------------------------------------------------------------------------------------
// Destructor
A1::~A1()
{}

//----------------------------------------------------------------------------------------
/*
 * Called once, at program start.
 */
void A1::init()
{
	// Initialize random number generator
	int rseed=getpid();
	srandom(rseed);
	// Print random number seed in case we want to rerun with
	// same random numbers
	cout << "Random number seed = " << rseed << endl;
	
	// Set the background colour.
	glClearColor( 0.3, 0.5, 0.7, 1.0 );

	// Build the shader
	m_shader.generateProgramObject();
	m_shader.attachVertexShader(
		getAssetFilePath( "VertexShader.vs" ).c_str() );
	m_shader.attachFragmentShader(
		getAssetFilePath( "FragmentShader.fs" ).c_str() );
	m_shader.link();

	// Set up the uniforms
	P_uni = m_shader.getUniformLocation( "P" );
	V_uni = m_shader.getUniformLocation( "V" );
	M_uni = m_shader.getUniformLocation( "M" );
	col_uni = m_shader.getUniformLocation( "colour" );

	initGrid();

	// Set up initial view and projection matrices (need to do this here,
	// since it depends on the GLFW window being set up correctly).
	view = glm::lookAt( 
		glm::vec3( 0.0f, 2.*float(DIM)*2.0*M_SQRT1_2, float(DIM)*2.0*M_SQRT1_2 ),
		glm::vec3( 0.0f, 0.0f, 0.0f ),
		glm::vec3( 0.0f, 1.0f, 0.0f ) );

	proj = glm::perspective( 
		glm::radians( 30.0f ),
		float( m_framebufferWidth ) / float( m_framebufferHeight ),
		1.0f, 1000.0f );
		
	init_cube(glm::vec4(glm::vec3(0.0f, 0.0f, 0.0f), 1), 0.5f);
	init_floor(glm::vec4(glm::vec3(0.0f, 0.0f, 0.0f), 1), 0.5f);
	init_avatar(glm::vec4(glm::vec3(0.0f, 0.0f, 0.0f), 1), 0.5f);
}

//Reset Function
void A1::reset(){
		m = nullptr;
		colour[0] = 0.0f;
		colour[1] = 0.0f;
		colour[2] = 0.0f;
		fcolour[0] = 1.0f;
		fcolour[1] = 1.0f;
		fcolour[2] = 1.0f;
		acolour[0] = 0.8f;
		acolour[1] = 0.0f;
		acolour[2] = 0.0f;
		a_x = 0;
		a_z = 0;


		// reset rotation
		


/*
		// reset zoom
		bool is_out = (zoom_num < 0);
		if(is_out){
			zoom_num *= -1;
		}
		for(int i = 0; i < zoom_num; i++){
			view = glm::scale(view, glm::vec3(1/zoom_factor, 1/zoom_factor, 1/zoom_factor));		
		}
		zoom_num = 0;
	*/	
		view = glm::lookAt( 
		glm::vec3( 0.0f, 2.*float(DIM)*2.0*M_SQRT1_2, float(DIM)*2.0*M_SQRT1_2 ),
		glm::vec3( 0.0f, 0.0f, 0.0f ),
		glm::vec3( 0.0f, 1.0f, 0.0f ) );


		//view = glm::rotate(view,0.0f, glm::vec3(0, 1, 0));
}


void A1::initGrid()
{
	size_t sz = 3 * 2 * 2 * (DIM+3);

	float *verts = new float[ sz ];
	size_t ct = 0;
	for( int idx = 0; idx < DIM+3; ++idx ) {
		verts[ ct ] = -1;
		verts[ ct+1 ] = 0;
		verts[ ct+2 ] = idx-1;
		verts[ ct+3 ] = DIM+1;
		verts[ ct+4 ] = 0;
		verts[ ct+5 ] = idx-1;
		ct += 6;

		verts[ ct ] = idx-1;
		verts[ ct+1 ] = 0;
		verts[ ct+2 ] = -1;
		verts[ ct+3 ] = idx-1;
		verts[ ct+4 ] = 0;
		verts[ ct+5 ] = DIM+1;
		ct += 6;
	}

	// Create the vertex array to record buffer assignments.
	glGenVertexArrays( 1, &m_grid_vao );
	glBindVertexArray( m_grid_vao );

	// Create the cube vertex buffer
	glGenBuffers( 1, &m_grid_vbo );
	
	glBindBuffer( GL_ARRAY_BUFFER, m_grid_vbo );
	glBufferData( GL_ARRAY_BUFFER, sz*sizeof(float),
		verts, GL_STATIC_DRAW );

	// Specify the means of extracting the position values properly.
	GLint posAttrib = m_shader.getAttribLocation( "position" );
	glEnableVertexAttribArray( posAttrib );
	glVertexAttribPointer( posAttrib, 3, GL_FLOAT, GL_FALSE, 0, nullptr );

	// Reset state to prevent rogue code from messing with *my* 
	// stuff!
	glBindVertexArray( 0 );
	glBindBuffer( GL_ARRAY_BUFFER, 0 );
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );

	// OpenGL has the buffer now, there's no need for us to keep a copy.
	delete [] verts;

	CHECK_GL_ERRORS;
}

//----------------------------------------------------------------------------------------
/*
 * Called once per frame, before guiLogic().
 */
void A1::appLogic()
{
	// Place per frame, application logic here ...
}

//----------------------------------------------------------------------------------------
/*
 * Called once per frame, after appLogic(), but before the draw() method.
 */
void A1::guiLogic()
{
	// We already know there's only going to be one window, so for 
	// simplicity we'll store button states in static local variables.
	// If there was ever a possibility of having multiple instances of
	// A1 running simultaneously, this would break; you'd want to make
	// this into instance fields of A1.
	static bool showTestWindow(false);
	static bool showDebugWindow(true);

	ImGuiWindowFlags windowFlags(ImGuiWindowFlags_AlwaysAutoResize);
	float opacity(0.5f);

	ImGui::Begin("Debug Window", &showDebugWindow, ImVec2(100,100), opacity, windowFlags);
		if( ImGui::Button( "Quit Application" ) ) {
			glfwSetWindowShouldClose(m_window, GL_TRUE);
		}

		// Eventually you'll create multiple colour widgets with
		// radio buttons.  If you use PushID/PopID to give them all
		// unique IDs, then ImGui will be able to keep them separate.
		// This is unnecessary with a single colour selector and
		// radio button, but I'm leaving it in as an example.

		// Prefixing a widget name with "##" keeps it from being
		// displayed.


		// Wall 
		ImGui::PushID(0);
		if(ImGui::RadioButton("##Col", &current_col, 0)){
			cout << current_col << endl;
			
		}
		ImGui::SameLine();
		ImGui::Text( "Wall Colour" );
		ImGui::PopID();
		
		ImGui::PushID(1);
		if(ImGui::RadioButton("##Col", &current_col, 1)){
			cout << current_col << endl;
		}
		ImGui::SameLine();
		ImGui::Text( "Floor Colour" );
		ImGui::PopID();
		

		//Avatar
		ImGui::PushID(2);
		if(ImGui::RadioButton("##Col", &current_col, 2)){
			
		}
		ImGui::SameLine();
		ImGui::Text( "Avatar Colour" );
		ImGui::PopID();

		//Colour
		ImGui::PushID(3);
		if(current_col == 0){
			ImGui::ColorEdit3("##Colour", colour);
		}
		else if(current_col == 1){
			ImGui::ColorEdit3("##Colour", fcolour);			
		}
		else if(current_col == 2){
			ImGui::ColorEdit3("##Colour", acolour);			
		}
		ImGui::PopID(); 

		//Regenerate Maze
		if( ImGui::Button( "Regenerate Maze" ) ) {
			m = make_shared<Maze>(DIM);
			m->digMaze();
			m->printMaze();
		}
		
		//Reset
		if( ImGui::Button( "Reset" ) ) {
			reset();

		}

		ImGui::Text( "Framerate: %.1f FPS", ImGui::GetIO().Framerate );

	ImGui::End();

	if( showTestWindow ) {
		ImGui::ShowTestWindow( &showTestWindow );
	}
}

void A1::init_floor(glm::vec4 p_v, GLfloat fs){
	glGenBuffers(1, &f_vbo);
	glGenBuffers(1, &f_ebo);
	glGenVertexArrays(1, &f_vao);
	glBindVertexArray(f_vao);
	
	glm::vec3 p = {p_v[0]+0.5f, p_v[1]+0.5f, p_v[2]+0.5f};

	vec3 vertices[] = {
		p + vec3(-0.5f, -0.5f, 0.5f),  //v2
		p + vec3(0.5f, -0.5f, 0.5f),  //v3
		p + vec3(0.5f, -0.5f, -0.5f),  //v4
		p + vec3(-0.5f, -0.5f, -0.5f), //v7
	};
	
	GLushort indices[] = {
		0,1,2, 2,3,0		
	};

	glBindBuffer(GL_ARRAY_BUFFER, f_vbo);
	glBufferData(GL_ARRAY_BUFFER,
				sizeof(glm::vec3) * 4,
				vertices,
				GL_STATIC_DRAW);
	CHECK_GL_ERRORS;
	
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, f_ebo);
	
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,
				sizeof(GLushort) * 6,
				indices,
				GL_STATIC_DRAW);

	CHECK_GL_ERRORS;

	GLint posAttrib = m_shader.getAttribLocation( "position" );
	glEnableVertexAttribArray( posAttrib );

	glVertexAttribPointer( posAttrib, 3, GL_FLOAT, GL_FALSE, 0, nullptr );
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	
	CHECK_GL_ERRORS;

}

void A1::init_cube(glm::vec4 p_v, GLfloat s){
	
	glGenBuffers(1, &m_vbo);
	glGenBuffers(1, &m_ebo);
	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	glm::vec3 p = {p_v[0]+0.5f, p_v[1]+0.5f, p_v[2]+0.5f};
	
	GLushort indices[] = {
	0,1,2, 2,3,0,
	0,3,4, 4,5,0,
	0,5,6, 6,1,0,
	1,6,7, 7,2,1,
	7,4,3, 3,2,7,
	4,7,6, 6,5,4};
	
	vec3 vertices[] = {
		p + vec3(0.5f, 0.5f, 0.5f), //v0
		p + vec3(-0.5f, 0.5f, 0.5f), //v1
		p + vec3(-0.5f, -0.5f, 0.5f),  //v2
		p + vec3(0.5f, -0.5f, 0.5f),  //v3
		p + vec3(0.5f, -0.5f, -0.5f),  //v4
		p + vec3(0.5f, 0.5f, -0.5f),  //v5
		p + vec3(-0.5f, 0.5f, -0.5f),  //v6
		p + vec3(-0.5f, -0.5f, -0.5f), //v7
	};
	
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER,
				sizeof(glm::vec3) * 8,
				vertices,
				GL_STATIC_DRAW);
	CHECK_GL_ERRORS;
	
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
	
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,
				sizeof(GLushort) * 36,
				indices,
				GL_STATIC_DRAW);

	CHECK_GL_ERRORS;

	GLint posAttrib = m_shader.getAttribLocation( "position" );
	glEnableVertexAttribArray( posAttrib );

	glVertexAttribPointer( posAttrib, 3, GL_FLOAT, GL_FALSE, 0, nullptr );
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	
	CHECK_GL_ERRORS;
}

void A1::init_avatar(glm::vec4 p_v, GLfloat s){
	
	int indicator = 0;
	
	for(int i = 0; i <= lats; i++){
		double lat0 = glm::pi<double>() * (-0.5 + (double) (i-1)/lats);
		double lat1 = glm::pi<double>() * (-0.5 + (double) i / lats);
		double z0 = sin(lat0);
		double zr0 = cos(lat0);
		double z1 = sin(lat1);
		double zr1 = cos(lat1);
		
		for(int j = 0; j <= longs; j++){
			double lng = 2 * glm::pi<double>() * (double) (j - 1) / longs;
			double x = cos(lng);
			double y = sin(lng);
			
			v.push_back(x*zr0+0.5f);
			v.push_back(y*zr0+0.5f);
			v.push_back(z0+0.5f);
			id.push_back(indicator);
			indicator++;
			
			v.push_back(x*zr1+0.5f);
			v.push_back(y*zr1+0.5f);
			v.push_back(z1+0.5f);
			id.push_back(indicator);
			indicator++;
		}
		id.push_back(GL_PRIMITIVE_RESTART_FIXED_INDEX);
	}
	
	glGenVertexArrays(1, &a_vao);
	glBindVertexArray(a_vao);
	
	glGenBuffers(1, &a_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, a_vbo);
	glBufferData(GL_ARRAY_BUFFER, v.size() * sizeof(GLfloat), &v[0], GL_STATIC_DRAW);
	
	GLint posAttrib = m_shader.getAttribLocation( "position" );
	glEnableVertexAttribArray( posAttrib );	

	glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	
	glGenBuffers(1, &a_ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, a_ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, id.size() * sizeof(GLuint), &id[0], GL_STATIC_DRAW);

	num_tri_a = id.size();
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	
}

void A1::draw_avatar(int x, int z, int h){
	glBindVertexArray(a_vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, a_ebo);
	glBindBuffer(GL_ARRAY_BUFFER, a_vbo);
	
	GLint transformLoc = m_shader.getUniformLocation("M");
	mat4 transform = glm::mat4(1.0f);

	transform = glm::translate(transform, glm::vec3(x-8+0.2, h, z-8+0.2));
	transform *= glm::scale(mat4(), vec3(0.5f));

	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
	
	glUniform3f(col_uni, acolour[0], acolour[1], acolour[2]);
	
	glDrawElements(GL_TRIANGLE_STRIP, num_tri_a, GL_UNSIGNED_INT, nullptr);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void A1::draw_cube(int x, int z, int h){

	glBindVertexArray(m_vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	
	GLint transformLoc = m_shader.getUniformLocation("M");
	mat4 transform = glm::mat4(1.0f);
	transform = glm::translate(transform, glm::vec3(x-8, h, z-8));
	
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
	
	glUniform3f(col_uni, colour[0], colour[1], colour[2]);
	
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, nullptr);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void A1::draw_floor(int x, int z){

	glBindVertexArray(f_vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, f_ebo);
	glBindBuffer(GL_ARRAY_BUFFER, f_vbo);
	
	GLint transformLoc = m_shader.getUniformLocation("M");
	mat4 transform = glm::mat4(1.0f);
	transform = glm::translate(transform, glm::vec3(x-8, 0, z-8));
	
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
	
	glUniform3f(col_uni, fcolour[0], fcolour[1], fcolour[2]);
	
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, nullptr);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	
}


//----------------------------------------------------------------------------------------
/*
 * Called once per frame, after guiLogic().
 */
void A1::draw()
{
	// Create a global transformation for the model (centre it).
	mat4 W;
	//W = glm::rotate(W, rotation_x, glm::vec3(0,1,0));
	W = glm::translate( W, vec3( -float(DIM)/2.0f, 0, -float(DIM)/2.0f ) );

	m_shader.enable();
		glEnable( GL_DEPTH_TEST );

		glUniformMatrix4fv( P_uni, 1, GL_FALSE, value_ptr( proj ) );
		glUniformMatrix4fv( V_uni, 1, GL_FALSE, value_ptr( view ) );
		glUniformMatrix4fv( M_uni, 1, GL_FALSE, value_ptr( W ) );

		// Just draw the grid for now.
		glBindVertexArray( m_grid_vao );
		glUniform3f( col_uni, 1, 1, 1 );
		glDrawArrays( GL_LINES, 0, (3+DIM)*4 );
		//draw_floor(0, 0);
		draw_avatar(a_x,a_z,0);
		if(m != nullptr){
			for(int i = 0; i < DIM; i++){
				for(int z = 0; z < DIM; z++){
					draw_floor(i,z);
					if(m->getValue(z, i) == 1){
							if(level > 0){
								for(int h = 0; h < level; h++){
										draw_cube(i,z,h);
								}
							}
							else{
								draw_cube(i,z,0);
							}
					}
				}
			}
		}
		// Highlight the active square.
	m_shader.disable();

	// Restore defaults
	glBindVertexArray( 0 );

	CHECK_GL_ERRORS;
}

//----------------------------------------------------------------------------------------
/*
 * Called once, after program is signaled to terminate.
 */
void A1::cleanup()
{}

//----------------------------------------------------------------------------------------
/*
 * Event handler.  Handles cursor entering the window area events.
 */
bool A1::cursorEnterWindowEvent (
		int entered
) {
	bool eventHandled(false);

	// Fill in with event handling code...

	return eventHandled;
}

//----------------------------------------------------------------------------------------
/*
 * Event handler.  Handles mouse cursor movement events.
 */
bool A1::mouseMoveEvent(double xPos, double yPos) 
{
	bool eventHandled(false);

	if (!ImGui::IsMouseHoveringAnyWindow()) {
		// Put some code here to handle rotations.  Probably need to
		// check whether we're *dragging*, not just moving the mouse.
		// Probably need some instance variables to track the current
		// rotation amount, and maybe the previous X position (so 
		// that you can rotate relative to the *change* in X.
		if(ImGui::IsMouseDown(0)){
			diff_x = xPos - mouse_x;
			diff_y = yPos - mouse_y;
			if(mouse_y <= 400){
				rotation_x -= diff_x * 0.00005;
			}	
			else{
				rotation_x += diff_x * 0.00005;
			}				
			rotation_y += diff_y * 0.01;
			view = glm::rotate(view,rotation_x, glm::vec3(0, 1, 0));
			mouse_x = xPos;
			mouse_y = yPos;
		}
		else{
			rotation_x = 0;
		}

	}

	eventHandled = true;
		
	

	return eventHandled;
}

//----------------------------------------------------------------------------------------
/*
 * Event handler.  Handles mouse button events.
 */
bool A1::mouseButtonInputEvent(int button, int actions, int mods) {
	bool eventHandled(false);

	if (!ImGui::IsMouseHoveringAnyWindow()) {
		// The user clicked in the window.  If it's the left
		// mouse button, initiate a rotation.
	}

	return eventHandled;
}

//----------------------------------------------------------------------------------------
/*
 * Event handler.  Handles mouse scroll wheel events.
 */
bool A1::mouseScrollEvent(double xOffSet, double yOffSet) {
	bool eventHandled(false);

	// Zoom in or out.

	if(yOffSet > 0){
		zoom_factor = 1.1f;
		zoom_num--;
	}else{
		zoom_factor = 1 / 1.1f;
		zoom_num++;
	}
	cout << zoom_num << endl;
	view = glm::scale(view, glm::vec3(zoom_factor, zoom_factor, zoom_factor));
	

	return eventHandled;
}

//----------------------------------------------------------------------------------------
/*
 * Event handler.  Handles window resize events.
 */
bool A1::windowResizeEvent(int width, int height) {
	bool eventHandled(false);

	// Fill in with event handling code...

	return eventHandled;
}

//----------------------------------------------------------------------------------------
/*
 * Event handler.  Handles key input events.
 */
bool A1::keyInputEvent(int key, int action, int mods) {
	bool eventHandled(false);

	// Fill in with event handling code...
	if( action == GLFW_RELEASE && key == GLFW_KEY_LEFT_SHIFT){
			is_shifted = false;
	}

	if( action == GLFW_PRESS ) {
		// Respond to some key events.
		if(key == GLFW_KEY_Q){
			glfwSetWindowShouldClose(m_window, GL_TRUE);
			eventHandled = true;
		}
		//DIG
		if(key == GLFW_KEY_D){
			m = make_shared<Maze>(DIM);
			m->digMaze();
			m->printMaze();
			
			for(int z = 0; z < DIM; z++){
				if(m->getValue(0, z) != 1){
					a_x = z;
					a_z = -1;
					break;
				}
			}
			
					
			
			eventHandled = true;
		}
		//RESET
		if(key == GLFW_KEY_R){
			reset();
			eventHandled = true;
		}
		//grow
		if(key == GLFW_KEY_SPACE){
			cout << level << endl;
			level++;
			cout << level << endl;
			eventHandled = true;
		}
		//shrink
		bool is_m;
		is_m = (m != nullptr);
		if(key == GLFW_KEY_BACKSPACE){
			if(level > 1){
				level--;
			}
			eventHandled = true;
		}

		if(key == GLFW_KEY_LEFT_SHIFT){
				is_shifted = true;
				cout << "shift pressed" << endl;
		}

		//left
		if(key == GLFW_KEY_LEFT){
			if(a_x == -1){
				
			}
			else if(!is_m){
				if(a_x > -1){
						a_x--;
				}
			}
			else if(a_z == -1 || a_z == 16){
					if(a_x >= 0){
						a_x--;
					}
			}
			else if(!is_shifted || a_x == 0){
				if(m->getValue(a_z, a_x-1) != 1){
						a_x--;
				}
				
			}
			else if(is_shifted || a_x == 0){
				if(a_x > -1){
					if(a_x > 0){
						m->setValue(a_z, a_x-1, 0);
					}
					a_x--;
				}
			}

		}
		//right
		if(key == GLFW_KEY_RIGHT){
			if(a_x == 16){
				
			}
			else if(!is_m){
				if(a_x < 16){
						a_x++;
				}
			}
			else if(a_z == -1 || a_z == 16){
					if(a_x <= 15){
						a_x++;
					}
			}
			else if(!is_shifted || a_x == 15){
				if(m->getValue(a_z, a_x+1) != 1){
						a_x++;
				}
				
			}
			else if(is_shifted || a_x == 15){
				if(a_x < 16){
					if(a_x < 15){
						m->setValue(a_z, a_x+1, 0);
					}
					a_x++;
				}
			}
		}
		//UP
		if(key == GLFW_KEY_UP){
			if(a_z == -1){
				
			}
			else if(!is_m){
				if(a_z > -1){
						a_z--;
				}
			}
			else if(a_x == -1 || a_x == 16){
					if(a_z >= 0){
						a_z--;
					}
			}
			else if(!is_shifted || a_z == 0){
				if(m->getValue(a_z-1, a_x) != 1 || a_z == 0){
						a_z--;
				}
			}
			else if(is_shifted || a_z == 0){
				if(a_z > -1){
					if(a_z > 0){
						m->setValue(a_z-1, a_x, 0);
					}
					a_z--;
				}
			}
		}

		//DOWN
		if(key == GLFW_KEY_DOWN){
			if(a_z == 16){
				
			}
			else if(!is_m){
				if(a_z < 16){
						a_z++;
				}
			}
			else if(a_x == -1 || a_x == 16){
					if(a_z <= 15){
						a_z++;
					}
			}
			else if(!is_shifted ){
				if(m->getValue(a_z+1, a_x) != 1|| a_z == 15){
						a_z++;
				}
				
			}
			else if(is_shifted || a_z == 15){
				if(a_z < 16){
					if(a_z < 15){
						m->setValue(a_z+1, a_x, 0);
					}
					a_z++;
				}
			}
		
		}
		if(key == GLFW_KEY_F){
			cout << a_x  << " " << a_z << endl;
		}
	}

	return eventHandled;
}
