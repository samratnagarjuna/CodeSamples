// Header Files
//=============

#include "../MeshRenderer.h"

#include <cstdlib>
#include "../../Asserts/Asserts.h"
#include "../../Logging/Logging.h"
#include "../../Platform/Platform.h"
#include "../Includes.h"

// Initialization / Clean Up
//==========================

bool eae6320::Graphics::MeshRenderer::Initialize(uint16_t i_noOfVertices, sVertex* i_pVertices, uint16_t i_noOfIndices, uint16_t* i_pIndices)
{
	bool wereThereErrors = false;

	{
		m_noOfVertices = i_noOfVertices;
		m_noOfIndices = i_noOfIndices;
	}

	GLuint vertexBufferId = 0;
	GLuint vertexIndexBufferId = 0;

	// Create a vertex array object and make it active
	{
		const GLsizei arrayCount = 1;
		glGenVertexArrays(arrayCount, &m_vertexArrayId);
		const GLenum errorCode = glGetError();
		if (errorCode == GL_NO_ERROR)
		{
			glBindVertexArray(m_vertexArrayId);
			const GLenum errorCode = glGetError();
			if (errorCode != GL_NO_ERROR)
			{
				wereThereErrors = true;
				EAE6320_ASSERTF(false, reinterpret_cast<const char*>(gluErrorString(errorCode)));
				eae6320::Logging::OutputError("OpenGL failed to bind the vertex array: %s",
					reinterpret_cast<const char*>(gluErrorString(errorCode)));
				goto OnExit;
			}
		}
		else
		{
			wereThereErrors = true;
			EAE6320_ASSERTF(false, reinterpret_cast<const char*>(gluErrorString(errorCode)));
			eae6320::Logging::OutputError("OpenGL failed to get an unused vertex array ID: %s",
				reinterpret_cast<const char*>(gluErrorString(errorCode)));
			goto OnExit;
		}
	}

	// Create a vertex buffer object and make it active
	{
		const GLsizei bufferCount = 1;
		glGenBuffers(bufferCount, &vertexBufferId);
		const GLenum errorCode = glGetError();
		if (errorCode == GL_NO_ERROR)
		{
			glBindBuffer(GL_ARRAY_BUFFER, vertexBufferId);
			const GLenum errorCode = glGetError();
			if (errorCode != GL_NO_ERROR)
			{
				wereThereErrors = true;
				EAE6320_ASSERTF(false, reinterpret_cast<const char*>(gluErrorString(errorCode)));
				eae6320::Logging::OutputError("OpenGL failed to bind the vertex buffer: %s",
					reinterpret_cast<const char*>(gluErrorString(errorCode)));
				goto OnExit;
			}
		}
		else
		{
			wereThereErrors = true;
			EAE6320_ASSERTF(false, reinterpret_cast<const char*>(gluErrorString(errorCode)));
			eae6320::Logging::OutputError("OpenGL failed to get an unused vertex buffer ID: %s",
				reinterpret_cast<const char*>(gluErrorString(errorCode)));
			goto OnExit;
		}
	}

	// Create a index buffer object and make it active
	{
		const GLsizei bufferCount = 1;
		glGenBuffers(bufferCount, &vertexIndexBufferId);
		const GLenum errorCode = glGetError();
		if (errorCode == GL_NO_ERROR)
		{
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertexIndexBufferId);
			const GLenum errorCode = glGetError();
			if (errorCode != GL_NO_ERROR)
			{
				wereThereErrors = true;
				EAE6320_ASSERTF(false, reinterpret_cast<const char*>(gluErrorString(errorCode)));
				eae6320::Logging::OutputError("OpenGL failed to bind the index buffer: %s",
					reinterpret_cast<const char*>(gluErrorString(errorCode)));
				goto OnExit;
			}
		}
		else
		{
			wereThereErrors = true;
			EAE6320_ASSERTF(false, reinterpret_cast<const char*>(gluErrorString(errorCode)));
			eae6320::Logging::OutputError("OpenGL failed to get an unused vertex index buffer ID: %s",
				reinterpret_cast<const char*>(gluErrorString(errorCode)));
			goto OnExit;
		}

	}
	// Initialize the vertex format
	{
		// The "stride" defines how large a single vertex is in the stream of data
		// (or, said another way, how far apart each position element is)
		const GLsizei stride = sizeof(sVertex);

		// Position (0)
		// 3 floats == 12 bytes
		// Offset = 0
		{
			const GLuint vertexElementLocation = 0;
			const GLint elementCount = 3;
			const GLboolean notNormalized = GL_FALSE;	// The given floats should be used as-is
			glVertexAttribPointer(vertexElementLocation, elementCount, GL_FLOAT, notNormalized, stride,
				reinterpret_cast<GLvoid*>(offsetof(sVertex, x)));
			const GLenum errorCode = glGetError();
			if (errorCode == GL_NO_ERROR)
			{
				glEnableVertexAttribArray(vertexElementLocation);
				const GLenum errorCode = glGetError();
				if (errorCode != GL_NO_ERROR)
				{
					wereThereErrors = true;
					EAE6320_ASSERTF(false, reinterpret_cast<const char*>(gluErrorString(errorCode)));
					eae6320::Logging::OutputError("OpenGL failed to enable the POSITION vertex attribute at location %u: %s",
						vertexElementLocation, reinterpret_cast<const char*>(gluErrorString(errorCode)));
					goto OnExit;
				}
			}
			else
			{
				wereThereErrors = true;
				EAE6320_ASSERTF(false, reinterpret_cast<const char*>(gluErrorString(errorCode)));
				eae6320::Logging::OutputError("OpenGL failed to set the POSITION vertex attribute at location %u: %s",
					vertexElementLocation, reinterpret_cast<const char*>(gluErrorString(errorCode)));
				goto OnExit;
			}
		}

		// Color (1)
		// 4 uint8_t == 4 bytes
		// Offset = 12
		{
			const GLuint vertexElementLocation = 1;
			const GLint elementCount = 4;
			const GLboolean notNormalized = GL_TRUE;	// The given floats should be used as-is
			glVertexAttribPointer(vertexElementLocation, elementCount, GL_UNSIGNED_BYTE, notNormalized, stride,
				reinterpret_cast<GLvoid*>(offsetof(sVertex, r)));
			const GLenum errorCode = glGetError();
			if (errorCode == GL_NO_ERROR)
			{
				glEnableVertexAttribArray(vertexElementLocation);
				const GLenum errorCode = glGetError();
				if (errorCode != GL_NO_ERROR)
				{
					wereThereErrors = true;
					EAE6320_ASSERTF(false, reinterpret_cast<const char*>(gluErrorString(errorCode)));
					eae6320::Logging::OutputError("OpenGL failed to enable the COLOR vertex attribute at location %u: %s",
						vertexElementLocation, reinterpret_cast<const char*>(gluErrorString(errorCode)));
					goto OnExit;
				}
			}
			else
			{
				wereThereErrors = true;
				EAE6320_ASSERTF(false, reinterpret_cast<const char*>(gluErrorString(errorCode)));
				eae6320::Logging::OutputError("OpenGL failed to set the COLOR vertex attribute at location %u: %s",
					vertexElementLocation, reinterpret_cast<const char*>(gluErrorString(errorCode)));
				goto OnExit;
			}
		}

		// TexCoord (2)
		// 2 floats == 8 bytes
		// offset = 16
		{
			const GLuint vertexElementLocation = 2;
			const GLint elementCount = 2;
			const GLboolean notNormalized = GL_TRUE;	// The given floats should be used as-is
			glVertexAttribPointer(vertexElementLocation, elementCount, GL_FLOAT, notNormalized, stride,
				reinterpret_cast<GLvoid*>(offsetof(sVertex, u)));
			const GLenum errorCode = glGetError();
			if (errorCode == GL_NO_ERROR)
			{
				glEnableVertexAttribArray(vertexElementLocation);
				const GLenum errorCode = glGetError();
				if (errorCode != GL_NO_ERROR)
				{
					wereThereErrors = true;
					EAE6320_ASSERTF(false, reinterpret_cast<const char*>(gluErrorString(errorCode)));
					eae6320::Logging::OutputError("OpenGL failed to enable the TEXCOORD vertex attribute at location %u: %s",
						vertexElementLocation, reinterpret_cast<const char*>(gluErrorString(errorCode)));
					goto OnExit;
				}
			}
			else
			{
				wereThereErrors = true;
				EAE6320_ASSERTF(false, reinterpret_cast<const char*>(gluErrorString(errorCode)));
				eae6320::Logging::OutputError("OpenGL failed to set the TEXCOORD vertex attribute at location %u: %s",
					vertexElementLocation, reinterpret_cast<const char*>(gluErrorString(errorCode)));
				goto OnExit;
			}
		}
	}
	// Assign the data to the buffer
	{
		// Eventually the vertex data should come from a file but for now it is hard-coded here.
		// You will have to update this in a future assignment
		// (one of the most common mistakes in the class is to leave hard-coded values here).

		const unsigned int vertexCount = i_noOfVertices;
		const unsigned int bufferSize = vertexCount * sizeof(sVertex);
		sVertex* vertexData = i_pVertices;

		glBufferData(GL_ARRAY_BUFFER, bufferSize, reinterpret_cast<GLvoid*>(vertexData),
			// In our class we won't ever read from the buffer
			GL_STATIC_DRAW);
		const GLenum errorCode = glGetError();
		if (errorCode != GL_NO_ERROR)
		{
			wereThereErrors = true;
			EAE6320_ASSERTF(false, reinterpret_cast<const char*>(gluErrorString(errorCode)));
			eae6320::Logging::OutputError("OpenGL failed to allocate the vertex buffer: %s",
				reinterpret_cast<const char*>(gluErrorString(errorCode)));
			goto OnExit;
		}
	}

	// Assign the data to the index buffer
	{
		const unsigned int indexCount = i_noOfIndices;
		const unsigned int bufferSize = indexCount * sizeof(uint16_t);
		uint16_t* indexData = i_pIndices;

		glBufferData(GL_ELEMENT_ARRAY_BUFFER, bufferSize, reinterpret_cast<GLvoid*>(indexData),
			// In our class we won't ever read from the buffer
			GL_STATIC_DRAW);
		const GLenum errorCode = glGetError();
		if (errorCode != GL_NO_ERROR)
		{
			wereThereErrors = true;
			EAE6320_ASSERTF(false, reinterpret_cast<const char*>(gluErrorString(errorCode)));
			eae6320::Logging::OutputError("OpenGL failed to allocate the index buffer: %s",
				reinterpret_cast<const char*>(gluErrorString(errorCode)));
			goto OnExit;
		}

	}

OnExit:

	if (m_vertexArrayId != 0)
	{
		// Unbind the vertex array
		// (this must be done before deleting the vertex buffer)
		glBindVertexArray(0);
		const GLenum errorCode = glGetError();
		if (errorCode == GL_NO_ERROR)
		{
			// The vertex and index buffer objects can be freed
			// (the vertex array object will still hold references to them,
			// and so they won't actually goes away until it gets freed).
			// Unfortunately debuggers don't work well when these are freed
			// (gDEBugger just doesn't show anything and RenderDoc crashes),
			// and so don't free them if debug info is enabled.
			if (vertexBufferId != 0)
			{
#ifndef EAE6320_GRAPHICS_ISDEVICEDEBUGINFOENABLED
				const GLsizei bufferCount = 1;
				glDeleteBuffers(bufferCount, &vertexBufferId);
				const GLenum errorCode = glGetError();
				if (errorCode != GL_NO_ERROR)
				{
					wereThereErrors = true;
					EAE6320_ASSERTF(false, reinterpret_cast<const char*>(gluErrorString(errorCode)));
					eae6320::Logging::OutputError("OpenGL failed to delete vertex buffer: %s",
						reinterpret_cast<const char*>(gluErrorString(errorCode)));
					goto OnExit;
				}
				vertexBufferId = 0;
#else
				m_vertexBufferId = vertexBufferId;
#endif
			}

			if (vertexIndexBufferId != 0)
			{
#ifndef EAE6320_GRAPHICS_ISDEVICEDEBUGINFOENABLED
				const GLsizei bufferCount = 1;
				glDeleteBuffers(bufferCount, &vertexIndexBufferId);
				const GLenum errorCode = glGetError();
				if (errorCode != GL_NO_ERROR)
				{
					wereThereErrors = true;
					EAE6320_ASSERTF(false, reinterpret_cast<const char*>(gluErrorString(errorCode)));
					eae6320::Logging::OutputError("OpenGL failed to delete vertex index buffer: %s",
						reinterpret_cast<const char*>(gluErrorString(errorCode)));
					goto OnExit;
				}
				vertexIndexBufferId = 0;
#else
				m_vertexIndexBufferId = vertexIndexBufferId;
#endif
			}
		}
		else
		{
			wereThereErrors = true;
			EAE6320_ASSERTF(false, reinterpret_cast<const char*>(gluErrorString(errorCode)));
			eae6320::Logging::OutputError("OpenGL failed to unbind the vertex array: %s",
				reinterpret_cast<const char*>(gluErrorString(errorCode)));
			goto OnExit;
		}
	}

	return !wereThereErrors;
}

bool eae6320::Graphics::MeshRenderer::CleanUp()
{
	bool wereThereErrors = false;

#ifdef EAE6320_GRAPHICS_ISDEVICEDEBUGINFOENABLED
	if (m_vertexBufferId != 0)
	{
		const GLsizei bufferCount = 1;
		glDeleteBuffers(bufferCount, &m_vertexBufferId);
		const GLenum errorCode = glGetError();
		if (errorCode != GL_NO_ERROR)
		{
			wereThereErrors = true;
			EAE6320_ASSERTF(false, reinterpret_cast<const char*>(gluErrorString(errorCode)));
			Logging::OutputError("OpenGL failed to delete the vertex buffer: %s",
				reinterpret_cast<const char*>(gluErrorString(errorCode)));
		}
		m_vertexBufferId = 0;
	}

	if (m_vertexIndexBufferId != 0)
	{
		const GLsizei bufferCount = 1;
		glDeleteBuffers(bufferCount, &m_vertexIndexBufferId);
		const GLenum errorCode = glGetError();
		if (errorCode != GL_NO_ERROR)
		{
			wereThereErrors = true;
			EAE6320_ASSERTF(false, reinterpret_cast<const char*>(gluErrorString(errorCode)));
			Logging::OutputError("OpenGL failed to delete the index buffer: %s",
				reinterpret_cast<const char*>(gluErrorString(errorCode)));
		}
		m_vertexIndexBufferId = 0;
	}
#endif
	if (m_vertexArrayId != 0)
	{
		const GLsizei arrayCount = 1;
		glDeleteVertexArrays(arrayCount, &m_vertexArrayId);
		const GLenum errorCode = glGetError();
		if (errorCode != GL_NO_ERROR)
		{
			wereThereErrors = true;
			EAE6320_ASSERTF(false, reinterpret_cast<const char*>(gluErrorString(errorCode)));
			Logging::OutputError("OpenGL failed to delete the vertex array: %s",
				reinterpret_cast<const char*>(gluErrorString(errorCode)));
		}
		m_vertexArrayId = 0;
	}
	return !wereThereErrors;
}

// Load Mesh
//==========

bool eae6320::Graphics::MeshRenderer::Load(const char* const i_path)
{
	bool wereThereErrors = false;

	// Load the binary mesh
	eae6320::Platform::sDataFromFile binaryMesh;
	{
		std::string errorMessage;
		if (!eae6320::Platform::LoadBinaryFile(i_path, binaryMesh, &errorMessage))
		{
			wereThereErrors = true;
			EAE6320_ASSERTF(false, errorMessage.c_str());
			eae6320::Logging::OutputError("Failed to load the binary mesh file \"%s\": %s", i_path, errorMessage.c_str());
			goto OnExit;
		}
	}

	// Extract data from binary
	uint16_t	noOfVertices;
	sVertex*	vertices;

	uint16_t	noOfIndices;
	uint16_t*	indices;
	{
		size_t offset = 0;
		uint8_t* data = reinterpret_cast<uint8_t*>(binaryMesh.data);

		noOfVertices = *reinterpret_cast<uint16_t*>(data + offset);
		offset += sizeof(uint16_t);

		noOfIndices = *reinterpret_cast<uint16_t*>(data + offset);
		offset += sizeof(uint16_t);

		vertices = reinterpret_cast<sVertex*>(data + offset);
		offset += noOfVertices * sizeof(sVertex);

		indices = reinterpret_cast<uint16_t*>(data + offset);
	}

	// Initialize data
	{
		if (!Initialize(noOfVertices, vertices, noOfIndices, indices))
		{
			wereThereErrors = true;
			EAE6320_ASSERT(false);
			eae6320::Logging::OutputError("Graphics failed to initialize mesh data file:  %s", i_path);
		}
	}

OnExit:
	// Clean binary data
	{
		binaryMesh.Free();
	}
	return !wereThereErrors;
}

// Render Mesh
//============

void eae6320::Graphics::MeshRenderer::Render()
{
	// Bind a specific vertex buffer to the device as a data source
	{
		glBindVertexArray(m_vertexArrayId);
		EAE6320_ASSERT(glGetError() == GL_NO_ERROR);
	}

	{
		// The mode defines how to interpret multiple vertices as a single "primitive";
		// we define a triangle list
		// (meaning that every primitive is a triangle and will be defined by three vertices)
		const GLenum mode = GL_TRIANGLES;
		// Every index is a 16 bit unsigned integer
		const GLenum indexType = GL_UNSIGNED_SHORT;
		// It's possible to start rendering primitives in the middle of the stream
		//const GLint indexOfFirstVertexToRender = 0;
		//// As of this comment we are only drawing a single triangle
		//// (you will have to update this code in future assignments!)
		//const unsigned int triangleCount = 2;
		//const unsigned int vertexCountPerTriangle = 3;
		//const unsigned int vertexCountToRender = triangleCount * vertexCountPerTriangle;
		//glDrawArrays(mode, indexOfFirstVertexToRender, vertexCountToRender);

		const GLvoid* const offset = 0;
		glDrawElements(mode, m_noOfIndices, indexType, offset);
		EAE6320_ASSERT(glGetError() == GL_NO_ERROR);
	}
}