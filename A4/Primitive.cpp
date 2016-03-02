#include "Primitive.hpp"
#include "polyroots.hpp"
#include "Mesh.hpp"


Primitive::~Primitive()
{
}

Intersection Primitive::intersect(const Ray &ray)
{
	Intersection result(ray, 0);
	return result;
}

Intersection Primitive::intersect(const Ray & ray, std::list<glm::mat4> transformations)
{
	assert(m_primitive);
	auto result = m_primitive->intersect(ray, transformations);
	return result;
}

Sphere::Sphere()
{
	m_primitive = new NonhierSphere({0, 0, 0}, 1.0);
}

Sphere::~Sphere()
{
	delete m_primitive;
}

Cube::Cube()
{
	m_primitive = new NonhierBox({0, 0, 0}, 1.0);
}

Cube::~Cube()
{
	delete m_primitive;
}

NonhierSphere::~NonhierSphere()
{
}

Intersection Sphere::intersect(const Ray & ray)
{
	Intersection result = m_primitive->intersect(ray);
	return result;
}

Intersection Cube::intersect(const Ray &ray)
{
	Intersection result = m_primitive->intersect(ray);
	return result;
}

// Reference: http://www.cs.utah.edu/~awilliam/box/box.pdf
Intersection NonhierBox::intersect(const Ray &r)
{
	Intersection result(r, 0);
    std::vector<glm::vec3> vertices = {
        m_pos + glm::vec3(0, 0, m_size),
        m_pos + glm::vec3(m_size, 0, m_size),
        m_pos + glm::vec3(m_size, m_size, m_size),
        m_pos + glm::vec3(0, m_size, m_size),
        
        m_pos + glm::vec3(0, 0, 0),
        m_pos + glm::vec3(m_size, 0, 0),
        m_pos + glm::vec3(m_size, m_size, 0),
        m_pos + glm::vec3(0, m_size, 0)
    };
    
    std::vector<Triangle> faces = {
        Triangle(2, 3, 0),
        Triangle(0, 1, 2),
        Triangle(2, 1, 5),
        Triangle(5, 6, 2),
        Triangle(3, 7, 4),
        Triangle(4, 0, 3),
        Triangle(7, 6, 5),
        Triangle(5, 4, 7),
        
        Triangle(1, 0, 4),
        Triangle(4, 5, 1),
        Triangle(2, 6, 7),
        Triangle(7, 3, 2)
    };
	
	glm::vec3 baryPosition;
	for (auto triangle : faces) {
		auto o = glm::vec3(r.origin);
		auto d = glm::vec3(r.direction);
		
		bool hit = glm::intersectRayTriangle(o, d,
											 vertices[triangle.v1],
											 vertices[triangle.v2],
											 vertices[triangle.v3],
											 baryPosition);
		
		if (hit && baryPosition.z > 0) {
			if (!result.hit) {
				result.t = baryPosition.z;
				result.hit = true;
				auto normal = -glm::normalize(
											  glm::cross(
														 vertices[triangle.v3] - vertices[triangle.v1],
														 vertices[triangle.v2] - vertices[triangle.v1]));
				result.normal = glm::dvec4(normal, 0);
			} else if (baryPosition.z < result.t) {
				result.t = baryPosition.z;
				auto normal = -glm::normalize(
											  glm::cross(
														 vertices[triangle.v3] - vertices[triangle.v1],
														 vertices[triangle.v2] - vertices[triangle.v1]));
				result.normal = glm::dvec4(normal, 0);
			}
		} else {
			if (hit && baryPosition.z < 0) {
				assert("Got normal problem?");
			}
		}
	}
	return result;
}

Intersection NonhierBox::intersect(const Ray & ray, std::list<glm::mat4> transformations)
{
	return intersect(ray);
}

Intersection NonhierSphere::intersect(const Ray & ray, std::list<glm::mat4> transformations)
{
	return intersect(ray);
}

Intersection NonhierSphere::intersect(const Ray &ray)
{
	Intersection result(ray, 0);
	glm::dvec4 c(m_pos.x, m_pos.y, m_pos.z, 1);
	const glm::dvec4 & l = ray.direction;
	const glm::dvec4 & o = ray.origin;
	const glm::dvec4 & o_c = o - c;
	
	double A = glm::dot(l, l);
	double B = 2 * glm::dot(l, o_c);
	double C = glm::dot(o_c, o_c) - m_radius * m_radius;
	double roots[2];
	size_t num_of_roots = quadraticRoots(A, B, C, roots);
	
	if (num_of_roots == 0) {
		// No roots, miss.
		result.hit = false;
	} else if (num_of_roots == 1) {
		result.hit = roots[0] > 0;
		result.t = roots[0];
	} else if (num_of_roots == 2) {
//        auto hitpoint = ray.origin + ray.direction * roots[0];
        
		// Return the smallest positive number
		result.t = std::min(roots[0], roots[1]);
		result.hit = result.t > 0;
	} else {
		// Should never happen here.
		assert(false);
	}
	
	if (result.hit) {
		assert(result.t > 0);
	}
	// update normal
	glm::dvec4 point = ray.origin + ray.direction * result.t;
	glm::dvec4 normal = glm::normalize(point - c);
	result.normal = normal;
	
	return result;
}



NonhierBox::~NonhierBox()
{
}
