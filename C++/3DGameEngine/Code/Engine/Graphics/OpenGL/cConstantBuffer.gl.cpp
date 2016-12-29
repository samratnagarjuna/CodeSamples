// Header Files
//=============

#include "../cConstantBuffer.h"

#include "../../Asserts/Asserts.h"
#include "../../Logging/Logging.h"

bool eae6320::Graphics::cConstantBuffer::CreateConstantBuffer()
{
	bool wereThereErrors = false;

	// Create a uniform buffer object and make it active
	{
		const GLsizei bufferCount = 1;
		glGenBuffers(bufferCount, &m_constantBufferId);
		const GLenum errorCode = glGetError();
		if (errorCode == GL_NO_ERROR)
		{
			glBindBuffer(GL_UNIFORM_BUFFER, m_constantBufferId);
			const GLenum errorCode = glGetError();
			if (errorCode != GL_NO_ERROR)
			{
				wereThereErrors = true;
				EAE6320_ASSERTF(false, reinterpret_cast<const char*>(gluErrorString(errorCode)));
				eae6320::Logging::OutputError("OpenGL failed to bind the new uniform buffer %u: %s",
					m_constantBufferId, reinterpret_cast<const char*>(gluErrorString(errorCode)));
				goto OnExit;
			}
		}
		else
		{
			wereThereErrors = true;
			EAE6320_ASSERTF(false, reinterpret_cast<const char*>(gluErrorString(errorCode)));
			eae6320::Logging::OutputError("OpenGL failed to get an unused uniform buffer ID: %s",
				reinterpret_cast<const char*>(gluErrorString(errorCode)));
			goto OnExit;
		}
	}
	// Fill in the constant buffer
	// Allocate space and copy the constant data into the uniform buffer
	{
		const GLenum usage = GL_DYNAMIC_DRAW;	// The buffer will be modified frequently and used to draw
		glBufferData(GL_UNIFORM_BUFFER, m_bufferSize, reinterpret_cast<const GLvoid*>(m_pBufferData), usage);
		const GLenum errorCode = glGetError();
		if (errorCode != GL_NO_ERROR)
		{
			wereThereErrors = true;
			EAE6320_ASSERTF(false, reinterpret_cast<const char*>(gluErrorString(errorCode)));
			eae6320::Logging::OutputError("OpenGL failed to allocate the new uniform buffer %u: %s",
				m_constantBufferId, reinterpret_cast<const char*>(gluErrorString(errorCode)));
			goto OnExit;
		}
	}

OnExit:

	return !wereThereErrors;
}

bool eae6320::Graphics::cConstantBuffer::BindConstantBuffer()
{
	bool wereThereErrors = false;
	{
		const GLuint bindingPointAssignedInShader = GetBufferRegisterID();
		glBindBufferBase(GL_UNIFORM_BUFFER, bindingPointAssignedInShader, m_constantBufferId);
		const GLenum errorCode = glGetError();
		if (errorCode != GL_NO_ERROR)
		{
			wereThereErrors = true;
			EAE6320_ASSERTF(false, reinterpret_cast<const char*>(gluErrorString(errorCode)));
			eae6320::Logging::OutputError("OpenGL failed to bind buffer %u: %s",
				m_constantBufferId, reinterpret_cast<const char*>(gluErrorString(errorCode)));
			goto OnExit;
		}
	}
OnExit:
	return !wereThereErrors;
}

void eae6320::Graphics::cConstantBuffer::Update(void* i_pConstantBufferData)
{
	// Make the uniform buffer active
	{
		glBindBuffer(GL_UNIFORM_BUFFER, m_constantBufferId);
		EAE6320_ASSERT(glGetError() == GL_NO_ERROR);
	}
	// Copy the updated memory to the GPU
	{
		GLintptr updateAtTheBeginning = 0;
		GLsizeiptr updateTheEntireBuffer = static_cast<GLsizeiptr>(m_bufferSize);
		glBufferSubData(GL_UNIFORM_BUFFER, updateAtTheBeginning, updateTheEntireBuffer, i_pConstantBufferData);
		EAE6320_ASSERT(glGetError() == GL_NO_ERROR);
	}
}

bool eae6320::Graphics::cConstantBuffer::CleanUp()
{
	bool wereThereErrors = false;
	if (m_constantBufferId != 0)
	{
		const GLsizei bufferCount = 1;
		glDeleteBuffers(bufferCount, &m_constantBufferId);
		const GLenum errorCode = glGetError();
		if (errorCode != GL_NO_ERROR)
		{
			wereThereErrors = true;
			EAE6320_ASSERTF(false, reinterpret_cast<const char*>(gluErrorString(errorCode)));
			Logging::OutputError("OpenGL failed to delete the constant buffer: %s",
				reinterpret_cast<const char*>(gluErrorString(errorCode)));
		}
		m_constantBufferId = 0;
	}

	return !wereThereErrors;
}