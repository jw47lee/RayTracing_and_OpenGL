// Winter 2020

#include <iostream>
#include <fstream>

#include <glm/ext.hpp>

// #include "cs488-framework/ObjFileDecoder.hpp"
#include "Mesh.hpp"

Mesh::Mesh( const std::string& fname )
: m_vertices()
, m_faces(), bound(NULL)
{
	std::string code;
	double vx, vy, vz;
	size_t s1, s2, s3;


	std::ifstream ifs( ("Assets/"+ fname).c_str() );
	while( ifs >> code ) {
		if( code == "v" ) {
			ifs >> vx >> vy >> vz;
			m_vertices.push_back( glm::vec3( vx, vy, vz ) );
		} else if( code == "f" ) {
			ifs >> s1 >> s2 >> s3;
			m_faces.push_back( Triangle( s1 - 1, s2 - 1, s3 - 1 ) );
		}
	}

	float minX = 1000000.0;
	float minY = 1000000.0;
	float minZ = 1000000.0;

	float maxX = -1000000.0;
	float maxY = -1000000.0;
	float maxZ = -1000000.0;

	for (const glm::vec3& v: m_vertices) {
		minX = std::min(minX, v.x);
		minY = std::min(minY, v.y);
		minZ = std::min(minZ, v.z);

		maxX = std::max(maxX, v.x);
		maxY = std::max(maxY, v.y);
		maxZ = std::max(maxZ, v.z);
	}
	const glm::vec3 min{minX, minY, minZ};
	const glm::vec3 max{maxX, maxY, maxZ};
	glm::vec3 pos = (max + min)/2;
	double radius =  glm::length(max - min)/2;
	bound =  make_shared<NonhierSphere>(pos, radius);
}

string Mesh::get_type(){
	return "Mesh";
}

glm::vec4 Mesh::get_normal(glm::vec4 point) {
	for (Triangle& tri : m_faces) {
		glm::vec3 P0{m_vertices.at(tri.v1)};
		glm::vec3 P1{m_vertices.at(tri.v2)};
		glm::vec3 P2{m_vertices.at(tri.v3)};

		glm::vec3 normal{glm::normalize(glm::cross(P1 - P0, P2 - P0))};

		float dot_with_normal = glm::dot(normal, glm::vec3(point) - P0);

		if (std::fabs(dot_with_normal) < 0.01) {
			return glm::vec4{normal, 0};
		}
	}

	throw 'S';
}

double Mesh::intersect(glm::vec4 ray_from, glm::vec4 ray_to) {


	//cout << "Mesh intersect" << endl;

	glm::vec3 A{ray_from.x, ray_from.y, ray_from.z};
	glm::vec3 B{ray_to.x, ray_to.y, ray_to.z};
	glm::vec4 intersection;

	bool is_hit = false;
	double t;

	#ifdef RENDER_BOUNDING_VOLUMES
		t = bound->intersect(ray_from, ray_to);
		//cout << t << endl;
		return t;
	#endif


	//cout << m_faces.size() << endl;
	for (const Triangle& tri : m_faces) {
		//cout << "run" << endl;
		const glm::vec3 P0{m_vertices.at(tri.v1)};
		const glm::vec3 P1{m_vertices.at(tri.v2)};
		const glm::vec3 P2{m_vertices.at(tri.v3)};

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
			//cout << t << endl;
			glm::vec3 iii = A + ((float)t) * (B - A);
			intersection = glm::vec4{iii, 1};
			glm::vec4 n = get_normal(intersection);

			if(glm::dot({n.x, n.y,n.z}, B-A) < 0){
				is_hit = true;
			}
		}
		if(is_hit){
			
			
			return t;
		}	

	}
	//cout << "end loop" << endl;
	throw 'Q';
}


std::ostream& operator<<(std::ostream& out, const Mesh& mesh)
{
	out << "mesh {";
  /*
  
  for( size_t idx = 0; idx < mesh.m_verts.size(); ++idx ) {
  	const MeshVertex& v = mesh.m_verts[idx];
  	out << glm::to_string( v.m_position );
	if( mesh.m_have_norm ) {
  	  out << " / " << glm::to_string( v.m_normal );
	}
	if( mesh.m_have_uv ) {
  	  out << " / " << glm::to_string( v.m_uv );
	}
  }

*/
	out << "}";
	return out;
}
