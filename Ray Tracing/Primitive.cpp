// Winter 2020

#include "Primitive.hpp"

//Primitive
Primitive::~Primitive(){}


//Sphere

Sphere::~Sphere(){}
double Sphere::intersect(glm::vec4 ray_from, glm::vec4 ray_to) {
	throw 'S';
	//return hitbox.intersect(ray_from, ray_to);
}

glm::vec4 Sphere::get_normal(glm::vec4 p) {
	throw 'S';
	//return hitbox.get_normal(p);
}
string Sphere::get_type(){
	return "Sphere";
}

//Cube

Cube::~Cube()
{
}

double Cube::intersect(glm::vec4 ray_from, glm::vec4 ray_to) {
	throw 'C';
	//return hitbox.intersect(ray_from, ray_to);
}

string Cube::get_type(){
	return "Cube";
}
glm::vec4 Cube::get_normal(glm::vec4 p) {
	throw 'C';
	//return hitbox.get_normal(p);
}

//NonhierSphere
NonhierSphere::~NonhierSphere()
{
}

string NonhierSphere::get_type(){
	return "NonhierSphere";
}
double NonhierSphere::intersect(glm::vec4 ray_from, glm::vec4 ray_to) {
	double roots[2];
	double real_root;
	glm::vec4 centre = glm::vec4(m_pos, 1);
	glm::vec4 b_a = ray_to - ray_from;
	glm::vec4 a_c = ray_from - centre;

	double a = glm::dot(b_a, b_a);
	double b = 2 * glm::dot(b_a, a_c);
	double c = glm::dot(a_c, a_c) - m_radius * m_radius;
	
	size_t numRoots = quadraticRoots(a, b, c, roots);

	//cerr << numRoots << endl;

	if(numRoots == 0){

		throw 'W';
	}
	else if(numRoots == 1){
		if(roots[0] < 0){
			throw 'W';
		}
		else{
			real_root = roots[0];
		}
	}
	else{
		if(roots[0] < 0 && roots[1] < 0){
			throw 'W';
		}
		else if(roots[0] < roots[1]){
			real_root = roots[0];
		}
		else{
			real_root = roots[1];
		}
	}

	return (float) real_root;
	//return ray_from + (ray_to - ray_from) * (float) real_root;
}

glm::vec4 NonhierSphere::get_normal(glm::vec4 point) {
	glm::vec3 normal{glm::vec3(point) - m_pos};
	return glm::normalize(glm::vec4(normal, 0));
}


NonhierBox::~NonhierBox()
{
}

string NonhierBox::get_type(){
	return "NonhierBox";
}


double NonhierBox::intersect(glm::vec4 ray_from, glm::vec4 ray_to) {

	glm::vec3 A{ray_from.x, ray_from.y, ray_from.z};
	glm::vec3 B{ray_to.x, ray_to.y, ray_to.z};
	glm::vec4 intersection;
	glm::vec3 x{m_size, 0, 0};
	glm::vec3 y{0, m_size, 0};
	glm::vec3 z{0, 0, m_size};

	double t;

	glm::vec3 p[12] = {
		m_pos,
		m_pos + x,
		m_pos + y,
		m_pos + x + y, 

		m_pos + z,
		m_pos + x + z,
		m_pos + y + z,
		m_pos + x + y + z
	};

	int v[36] = {

		//front
		0, 1, 2,
		1, 3, 2,
		
		//back
		5, 4, 7,
		4, 6, 7,
		// right
		1, 5, 7,
		1, 7, 3,

		// top
		2, 3, 7,
		2, 7, 6,

		// left
		4, 0, 2,
		4, 2, 6,

		//bottom
		0, 1, 4,
		1, 5, 4
	};

	bool is_hit = false;

	for (unsigned int i = 0; i < 12; i += 1) {

		glm::vec3 P0 = p[v[i * 3]];
		glm::vec3 P1 = p[v[i * 3 + 1]];
		glm::vec3 P2 = p[v[i * 3 + 2]];

		double R1 = A.x - P0.x;
		double R2 = A.y - P0.y;
		double R3 = A.z - P0.z;

		double x1 = P1.x - P0.x;
		double y1 = P1.y - P0.y;
		double z1 = P1.z - P0.z;

		double x2 = P2.x - P0.x;
		double y2 = P2.y - P0.y;
		double z2 = P2.z - P0.z;

		double x3 = B.x - A.x;
		double y3 = B.y - A.y;
		double z3 = B.z - A.z;

		double det_d = x1 * (y2 * z3 - y3 * z2) - x2 * (y1 * z3 - y3 * z1) + x3 * (y1 * z2 - y2 * z1);
		double det_d1 = R1 * (y2 * z3 - y3 * z2) - x2 *(R2 * z3 - y3 * R3) + x3 * (R2 * z2 - y2 * R3);
		double det_d2 = x1 * (R2 * z3 - y3 * R3) - R1 *(y1 * z3 - y3 * z1) + x3 * (y1 * R3 - R2 * z1);
		double det_d3 = x1 * (y2 * R3 - R2 * z2) - x2 *(y1 * R3 - R2 * z1) + R1 * (y1 * z2 - y2 * z1);
		
		double beta = det_d1 / det_d;
		double gamma = det_d2 / det_d;

		if(beta + gamma <= 1 && beta >= 0 && gamma >= 0){
			t = -1 * det_d3 / det_d;
			glm::vec3 iii = A + ((float)t) * (B - A);
			intersection = glm::vec4{iii, 1};
			glm::vec4 n = get_normal(intersection);

				if(glm::dot({n.x, n.y,n.z}, B-A) < 0){
					is_hit = true;
				}
		}
		if(is_hit){
			return t;
			//return intersection;
		}	

	}

	throw 'W';
}

bool accurate_zero(double a, double b){
	return std::fabs(a-b) < 0.0001;
}

glm::vec4 NonhierBox::get_normal(glm::vec4 point) {
	double x = 0;
	double y = 0;
	double z = 0;

	//origin = m_pos.x, m_pos.y, m_pox.z

	//point at front
	double front = m_pos.z;

	//point at back
	double back = m_pos.z + m_size;

	//point at top
	double top = m_pos.y + m_size;

	//point at botton
	double bottom = m_pos.y;

	//point at right
	double right = m_pos.x + m_size;

	//point at left
	double left = m_pos.x;

	if(accurate_zero(point.z, front)){
		z -=1;
	}
	if(accurate_zero(point.z, back)){
		z += 1;
	}
	if(accurate_zero(point.y, top)){
		y += 1;
	}
	if(accurate_zero(point.y, bottom)){
		y -= 1;

	}if(accurate_zero(point.x, right)){
		x += 1;

	}
	if(accurate_zero(point.x, left)){
		x -= 1;
	}

	glm::vec4 normal = glm::vec4(glm::normalize(glm::vec3{x, y, z}), 0);

	return normal;
}