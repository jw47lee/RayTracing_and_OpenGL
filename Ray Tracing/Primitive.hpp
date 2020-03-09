// Winter 2020

#pragma once

#include <glm/glm.hpp>
#include "polyroots.hpp"
#include <algorithm>
#include <string>
#include <iostream>
#include <vector>

using namespace std;

class Primitive {
public:
  virtual double intersect(glm::vec4 ray_from, glm::vec4 ray_to) = 0;
  virtual ~Primitive();
  virtual glm::vec4 get_normal(glm::vec4) = 0;
  virtual string get_type() = 0;
};



class NonhierSphere : public Primitive {
public:
  NonhierSphere(const glm::vec3& pos, double radius)
    : m_pos(pos), m_radius(radius)
  {
  }
  virtual ~NonhierSphere();
  virtual double intersect(glm::vec4 ray_from, glm::vec4 ray_to);
  virtual string get_type();
  virtual glm::vec4 get_normal(glm::vec4 p);

//private:
  glm::vec3 m_pos;
  double m_radius;
};

class NonhierBox : public Primitive {
public:
  NonhierBox(const glm::vec3& pos, double size)
    : m_pos(pos), m_size(size)
  {
  }
  
  virtual ~NonhierBox();
  virtual double intersect(glm::vec4 ray_from, glm::vec4 ray_to);
  virtual string get_type();
  virtual glm::vec4 get_normal(glm::vec4 p);

//private:
  glm::vec3 m_pos;
  double m_size;
};

class Sphere : public Primitive {
  //NonhierSphere hitbox;
public:
  virtual double intersect(glm::vec4 ray_from, glm::vec4 ray_to);
  virtual string get_type();
  virtual ~Sphere();
  virtual glm::vec4 get_normal(glm::vec4 p);
};

class Cube : public Primitive {
  //NonhierBox hitbox;
public:
  virtual double intersect(glm::vec4 ray_from, glm::vec4 ray_to);
  virtual string get_type();
  virtual ~Cube();
  virtual glm::vec4 get_normal(glm::vec4 p);
};