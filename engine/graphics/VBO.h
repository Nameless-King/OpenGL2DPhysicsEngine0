#pragma once

#include <GL/glew.h>

class VBO{
private:
	unsigned int m_id;
	const void *m_data;
	size_t m_byteSize;
	GLenum m_dataType;
	GLenum m_type;
	GLenum m_usage;
	unsigned int m_size;
	GLenum m_normalize;
public:
	VBO();
	VBO(const void *data, size_t byteSize,GLenum dataType, GLenum type, GLenum usage, unsigned int size, GLenum normalize);
	~VBO();
	
	void bind() const;
	void unbind() const;
	
	size_t getByteSize() const {return m_byteSize;}
	GLenum getDataType() const {return m_dataType;}
	GLenum getNormalize() const {return m_normalize;}
	unsigned int getSize() const {return m_size;}
	
	
	inline unsigned int getID() const { return m_id;}
};