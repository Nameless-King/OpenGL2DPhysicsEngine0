#pragma once

#include <cmath>

#include "../../Dependencies/glm/glm.hpp"
#include "../maths/EngineMath.h"

enum class BoundingType{
	NA = 0,
	AxisAligned = 1,
	Circle = 2,
	Oriented = 3
};

class Bound{
private:
	BoundingType m_type;
	glm::vec2* m_halfExtents;
	glm::vec3* m_center;
	glm::vec3* m_scale;
	float m_rotation;
	glm::vec2 m_localX;
	glm::vec2 m_localY;
	float m_furthestDistance;
public:
	Bound(); // COMPLETE
	Bound(glm::vec3* center, glm::vec3* scale, const float* pvertices, BoundingType type); // COMPLETE
	Bound(const Bound& bound);
	~Bound(); // COMPLETE
	
	inline const float getRotation() const {return m_rotation;}
	inline const float getFurthestDistance() const {return m_furthestDistance;}
	inline const BoundingType getBoundingType() const {return m_type;} //COMPLETE
	inline const glm::vec2 getLocalX() const {return m_localX;}
	inline const glm::vec2 getLocalY() const {return m_localY;}
	inline const glm::vec3* getCenter() const {return m_center;} // COMPLETE
	inline const glm::vec2* getHalfExtents() const {return m_halfExtents;} // COMPLETE
	inline const glm::vec3* getScale() const {return m_scale;} // COMPLETE

	glm::vec2 getCopyCenterXY() const; // COMPLETE
	glm::vec3 getCopyCenterXYZ() const; // COMPLETE
	glm::vec2 getCopyHalfExtentsXY() const; // COMPLETE
	
	void setHalfExtents(glm::vec2* halfExtents) ; // COMPLETE
	void rotateDegrees(float offset);
};