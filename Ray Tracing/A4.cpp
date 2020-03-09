// Winter 2020

#include <glm/ext.hpp>
#include <string>
#include <cmath>
#include "polyroots.hpp"
#include "GeometryNode.hpp"
#include "JointNode.hpp"
#include "PhongMaterial.hpp"
#include <algorithm>
#include "A4.hpp"
#include "Mesh.hpp"
#include <cmath>
#include <functional>
#include <queue>
#include <tuple>
#include <limits>
//#define HIER_RENDER

using namespace std;
using namespace glm;

glm::vec3 ray_colour(PhongMaterial *material, GeometryNode* node,
	glm::vec4 shadow_ray_from, glm::vec4 shadow_ray_to, Light* light,
	glm::vec4 ray_from, glm::vec4 ray_to, glm::vec4 intersection){

	//https://computergraphicsguide.blogspot.com/2015/08/raycasting-and-phong-shading.html

	//Attenuation
	const double dist_to_light = glm::distance(shadow_ray_to, shadow_ray_from);
	double const_a = light->falloff[0];
	double linear_a = light->falloff[1];
	double quad_a = light->falloff[2];
	double attenuation = 1/(const_a + linear_a * dist_to_light + quad_a *dist_to_light * dist_to_light);

	// Diffuse reflection
	glm::vec3 m_kd = material->get_kd();
	glm::vec3 normal_light = glm::vec3(node->m_primitive->get_normal(intersection));
	glm::vec3 l = glm::vec3(glm::normalize(shadow_ray_to - shadow_ray_from));
	float l_dot_normal = (float)glm::dot(normal_light, l);
	glm::vec3 diffuse = m_kd * light->colour * std::max(l_dot_normal, 0.0f);

	//specular
	glm::vec3 specular = glm::vec3(0.0);
	glm::vec3 m_specular = material->get_ks();
	double m_shining = material->get_shininess();
	glm::vec3 v = glm::vec3(glm::normalize(ray_from - ray_to));
	if (l_dot_normal > 0.0) {
  
		glm::vec3 h = glm::normalize(v + l);
		float normal_dot_h = std::max((float)glm::dot(normal_light, h), 0.0f);
		specular = m_specular * std::pow(normal_dot_h, m_shining);
	}
	return attenuation * (diffuse + specular);
}


void A4_Render(
		// What to render  
	SceneNode * root,

		// Image to write to, set to a given width and height  
	Image & image,

		// Viewing parameters  
	const glm::vec3 & lookfrom,
	const glm::vec3 & lookat,
	const glm::vec3 & up,
	double fovy,

		// Lighting parameters  
	const glm::vec3 & ambient,
	const std::list<Light *> & lights
	) {

  // Fill in raytracing code here...  

	std::cout << "Calling A4_Render(\n" <<
	"\t" << *root <<
	"\t" << "Image(width:" << image.width() << ", height:" << image.height() << ")\n"
	"\t" << "lookform:  " << glm::to_string(lookfrom) << std::endl <<
	"\t" << "lookat: " << glm::to_string(lookat) << std::endl <<
	"\t" << "up:   " << glm::to_string(up) << std::endl <<
	"\t" << "fovy: " << fovy << std::endl <<
	"\t" << "ambient: " << glm::to_string(ambient) << std::endl <<
	"\t" << "lights{" << std::endl;

	for(const Light * light : lights) {
		std::cout << "\t\t" <<  *light << std::endl;
	}
	cout << "\t}" << endl;
	cout <<")" << endl;

	double nx = image.width();
	double ny = image.height();
	double d = 1;
	//double d = glm::distance(lookfrom, lookat);

	double h = 2 * d * glm::tan(glm::radians(fovy/2));
	double w = nx / ny * h;

	cout << "nx: " << nx << endl;
	cout << "ny: " << ny << endl;
	cout << "d: " << d << endl;
	cout << "h: " << h << endl;
	cout << "w: " << w << endl;

	// step 1
	// make zk = 0 and translate
	glm::mat4 p;
	glm::vec3 init_trans = glm::vec3(-nx/2, -ny/2, d);
	//T = glm::translate(T, init_trans);

	// step 2 scale to preserve the aspect ratio
	glm::vec3 init_scale = glm::vec3( -w/nx, -h/ny, 1);
	//T = init_scale * T;

	// step 3 rotate WCS -> VCS
	// buildup R

	glm::vec3 w_vec = glm::normalize(lookat - lookfrom);
	glm::vec3 u_vec = glm::normalize(glm::cross(up, w_vec));
	glm::vec3 v_vec = glm::normalize(glm::cross(w_vec, u_vec));
	
	glm::mat4 R = mat4(
		glm::vec4(u_vec, 0),
		glm::vec4(v_vec, 0),
		glm::vec4(w_vec, 0),
		glm::vec4(0, 0, 0, 1)	
		);

	// step 4 translate lookfrom
	glm::mat4 init_trans_lookfrom = glm::mat4(
		glm::vec4(1,0,0,0),
		glm::vec4(0,1,0,0),
		glm::vec4(0,0,1,0),
		glm::vec4(lookfrom.x, lookfrom.y, lookfrom.z, 1)
		);

	p = translate(p, init_trans);
	p = scale(init_scale) * p;
	p = R * p;
	p = init_trans_lookfrom * p;

	int hit = 0;
	int miss = 0;
	int miss_P = 0;
	int miss_S = 0;
	int miss_C = 0;
	int miss_W = 0;
	int miss_Q = 0;


		// back_ground
	for (uint y = 0; y < ny; ++y) {
		for (uint x = 0; x < nx; ++x) {
			image(x, y, 0) = ((double)x )/ nx;
			image(x, y, 1) = ((double)x) + ((double)y) / (2 * nx * ny);
			image(x, y, 2) = ((double)y) / ny;
		}
	}

	#ifdef HIER_RENDER

	//define type for easier tree traverse
	typedef std::tuple<SceneNode*, const glm::mat4, const glm::mat4 > Node;

	for (unsigned int y = 0; y < ny; y += 1) {
		for (unsigned int x = 0; x < nx; x += 1) {
			// print progres
			printf("\r[%3d%%]", (int) ((y * ny + x) * 100 / (1.0 * nx * ny)));
			
			vec4 pixel = p * glm::vec4(x, y, 0, 1);
			vec4 ray_from = vec4(lookfrom, 1);
			vec4 ray_to = pixel;

			double t = std::numeric_limits<double>::infinity();
			bool t_found = false;

			glm::mat4 reverse_back_to_t;
			glm::mat4 apply_to_model;

			GeometryNode* n;

			std::queue<Node> node_q;
			node_q.push(Node{root, root->trans, root->invtrans});

			while(!node_q.empty()){

				SceneNode* parent = std::get<0>(node_q.front());
				glm::mat4 M = std::get<1>(node_q.front());
				glm::mat4 invM = std::get<2>(node_q.front());
				
				node_q.pop();

				for(SceneNode* child: parent->children){
					glm::mat4 T = M * child->trans;
					glm::mat4 invT = child->invtrans * invM;

					if(child->m_nodeType == NodeType::GeometryNode){
						GeometryNode * geometryNode = static_cast<GeometryNode*>(child);
						try{
							glm::vec4 mr_ray_from = invT * ray_from;
							glm::vec4 mr_ray_to = invT * ray_to;
							double intersection = geometryNode->m_primitive->intersect(mr_ray_from, mr_ray_to);
							if (intersection < t) {
								t = intersection;
								t_found = true;
								reverse_back_to_t = T;
								apply_to_model = invT;
								n = geometryNode;
							}

						}catch(...){
							miss++;
						};

					}
					else if(child->m_nodeType == NodeType::JointNode){
						JointNode * jointNode = static_cast<JointNode*>(child);
						float xRotation = glm::radians(
							glm::clamp(
								jointNode->m_joint_x.init,
								jointNode->m_joint_x.min,
								jointNode->m_joint_x.max
								)
							);
						float yRotation = glm::radians(
							glm::clamp(
								jointNode->m_joint_y.init,
								jointNode->m_joint_y.min,
								jointNode->m_joint_y.max
								)
							);

						T = T * glm::rotate(glm::mat4(), yRotation, glm::vec3(0, 1, 0));
						T = T * glm::rotate(glm::mat4(), xRotation, glm::vec3(1, 0, 0));

						invT = glm::rotate(glm::mat4(), -yRotation, glm::vec3(0, 1, 0)) * invT;
						invT = glm::rotate(glm::mat4(), -xRotation, glm::vec3(1, 0, 0)) * invT;
					}
					node_q.push(Node{child, T, invT});
				}
			}

			if (!t_found) {
				miss += 1;
				continue;
			}

			GeometryNode* node = n;
			glm::vec4 model_ray_from = apply_to_model * glm::vec4(lookfrom, 1);
			glm::vec4 model_ray_to = apply_to_model * pixel;

			//tmp value
			glm::vec3 iii = glm::vec3(model_ray_from) + ((float)t) * (glm::vec3(model_ray_to) - glm::vec3(model_ray_from));

			//interection
			glm::vec4 m_interaction = glm::vec4(iii, 1);
			
			PhongMaterial *material = (PhongMaterial*) node->m_material;
			glm::vec3 fragColour{ambient};

			for(Light* light: lights){

				vec4 worldShadowRay_from = reverse_back_to_t * m_interaction;
				vec4 worldShadowRay_to = glm::vec4(light->position, 1);

				bool in_shadow = false;

				//SHADOW TODO

				if(in_shadow){
					continue;
				}

				//shadow_ray_from, shadow_ray_to
				glm::vec4 shadow_ray_from = m_interaction;
				glm::vec4 shadow_ray_to = apply_to_model * glm::vec4(light->position, 1);
				
				//fragColour += diffuse;
				fragColour += ray_colour(material, node, shadow_ray_from, shadow_ray_to,
				 light, model_ray_from, model_ray_to, m_interaction);
			}

			hit++;


			image(x, y, 0) = fragColour.r;
			image(x, y, 1) = fragColour.g;
			image(x, y, 2) = fragColour.b;	
		}
	}

	#else

	for (unsigned int y = 0; y < ny; y += 1) {
		for (unsigned int x = 0; x < nx; x += 1) {
			// print progres
			printf("\r[%3d%%]", (int) ((y * ny + x) * 100 / (1.0 * nx * ny)));
			
			vec4 pixel = p * glm::vec4(x, y, 0, 1);
			vec4 ray_from = vec4(lookfrom, 1);
			vec4 ray_to = pixel;

			for (SceneNode *c: root->children){
				GeometryNode* node = (GeometryNode*) c;
				try{
					double t = node->m_primitive->intersect(ray_from, ray_to);
					//tmp values
					glm::vec3 A{ray_from.x, ray_from.y, ray_from.z};
					glm::vec3 B{ray_to.x, ray_to.y, ray_to.z};
					glm::vec3 iii = A + ((float)t) * (B - A);

					glm::vec4 intersection = glm::vec4(iii, 1);
					PhongMaterial *material = (PhongMaterial*) node->m_material;
					glm::vec3 fragColour{ambient};

					for(Light* light: lights){
						vec4 shadow_ray_from = intersection;
						vec4 shadow_ray_to = glm::vec4(light->position, 1);

						bool in_shadow = false;

						for (SceneNode * other : root->children) {
							if (in_shadow) break;
							if (other != c) {
								GeometryNode* otherNode = (GeometryNode*) other;
								try {
									otherNode->m_primitive->intersect(shadow_ray_from, shadow_ray_to);
									in_shadow = true;
								} catch (...) {}
							}
						}

						// skip light manipulation to create shadow
						if(in_shadow){
							continue;
						}
						
						fragColour += ray_colour(material, node, shadow_ray_from, shadow_ray_to,
							light, ray_from, ray_to, intersection) ;
					}

					hit++;

					
					image(x, y, 0) = fragColour.r;
					image(x, y, 1) = fragColour.g;
					image(x, y, 2) = fragColour.b;
				}
				catch(char s){
					if(s == 'Q'){
						miss_Q++;
					}
					else if(s == 'W'){
						miss_W++;
					}
					else if(s == 'C'){
						miss_C++;
					}
					else if(s == 'P'){
						miss_P++;
					}
					else if(s == 'S'){
						miss_S++;
					}
					miss++;
				}
			}
		}
	}


	#endif
	printf("\n");
	cout << "hit: " << hit << endl;
	//cout << "miss: " << miss << endl;
	cout << "miss_P: " << miss_P << endl;
	cout << "miss_C: " << miss_C << endl;
	cout << "miss_S: " << miss_S << endl;
	cout << "miss_Q: " << miss_Q << endl;
	cout << "miss_W: " << miss_W << endl;
	//cout << "Sibal" << endl;
	
}

