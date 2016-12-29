// Header Files
//=============

#include "../MeshRenderer.h"

#include <D3D11.h>
#include <algorithm>
#include "../../Asserts/Asserts.h"
#include "../../Logging/Logging.h"
#include "../../Platform/Platform.h"
#include "../Graphics.h"
#include "../Includes.h"
#include "RenderContext.d3d.h"

// Initialization / Clean Up
//==========================

bool eae6320::Graphics::MeshRenderer::Initialize(uint16_t i_noOfVertices, sVertex* i_pVertices, uint16_t i_noOfIndices, uint16_t* i_pIndices)
{
	bool wereThereErrors = false;

	{
		m_noOfVertices = i_noOfVertices;
		m_noOfIndices = i_noOfIndices;
	}
	
	// Get device context
	ID3D11Device* direct3dDevice = RenderContext::Instance()->GetDirect3DDevice();

	// Creating Vertex buffer
	{
		// Eventually the vertex data should come from a file but for now it is hard-coded here.
		// You will have to update this in a future assignment
		// (one of the most common mistakes in the class is to leave hard-coded values here).

		const unsigned int vertexCount = i_noOfVertices;
		const unsigned int bufferSize = vertexCount * sizeof(sVertex);

		sVertex* vertexData = i_pVertices;

		D3D11_BUFFER_DESC bufferDescription = { 0 };
		{
			bufferDescription.ByteWidth = bufferSize;
			bufferDescription.Usage = D3D11_USAGE_IMMUTABLE;	// In our class the buffer will never change after it's been created
			bufferDescription.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			bufferDescription.CPUAccessFlags = 0;	// No CPU access is necessary
			bufferDescription.MiscFlags = 0;
			bufferDescription.StructureByteStride = 0;	// Not used
		}
		D3D11_SUBRESOURCE_DATA initialData = { 0 };
		{
			initialData.pSysMem = vertexData;
			// (The other data members are ignored for non-texture buffers)
		}

		const HRESULT result = direct3dDevice->CreateBuffer(&bufferDescription, &initialData, &m_vertexBuffer);
		if (FAILED(result))
		{
			EAE6320_ASSERT(false);
			eae6320::Logging::OutputError("Direct3D failed to create the vertex buffer with HRESULT %#010x", result);
			wereThereErrors = false;
			goto OnExit;
		}
	}

	// Creating Index Buffer
	{
		const unsigned int indexCount = i_noOfIndices;
		const unsigned int bufferSize = indexCount * sizeof(uint16_t); // Max possible indices that we can store using uint16_t

		uint16_t* indexData = i_pIndices;

		D3D11_BUFFER_DESC bufferDescription = { 0 };
		{
			bufferDescription.ByteWidth = bufferSize;
			bufferDescription.Usage = D3D11_USAGE_IMMUTABLE;	// In our class the buffer will never change after it's been created
			bufferDescription.BindFlags = D3D11_BIND_INDEX_BUFFER;
			bufferDescription.CPUAccessFlags = 0;	// No CPU access is necessary
			bufferDescription.MiscFlags = 0;
			bufferDescription.StructureByteStride = 0;	// Not used
		}
		D3D11_SUBRESOURCE_DATA initialData = { 0 };
		{
			initialData.pSysMem = indexData;
			// (The other data members are ignored for non-texture buffers)
		}

		const HRESULT result = direct3dDevice->CreateBuffer(&bufferDescription, &initialData, &m_vertexIndexBuffer);
		if (FAILED(result))
		{
			EAE6320_ASSERT(false);
			eae6320::Logging::OutputError("Direct3D failed to create the index buffer with HRESULT %#010x", result);
			wereThereErrors = false;
			goto OnExit;
		}
	}

OnExit:
	return !wereThereErrors;
}

bool eae6320::Graphics::MeshRenderer::CleanUp()
{
	if (m_vertexBuffer)
	{
		m_vertexBuffer->Release();
		m_vertexBuffer = NULL;
	}

	if (m_vertexIndexBuffer)
	{
		m_vertexIndexBuffer->Release();
		m_vertexIndexBuffer = NULL;
	}

	return true;
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
		std::reverse(indices, indices + noOfIndices);

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
	// Get device context
	ID3D11DeviceContext* direct3dImmediateContext = RenderContext::Instance()->GetDirect3DImmediateContext();
	// Setting vertex buffer
	{
		const unsigned int startingSlot = 0;
		const unsigned int vertexBufferCount = 1;
		// The "stride" defines how large a single vertex is in the stream of data
		const unsigned int bufferStride = sizeof(sVertex);
		// It's possible to start streaming data in the middle of a vertex buffer
		const unsigned int bufferOffset = 0;
	
		direct3dImmediateContext->IASetVertexBuffers(startingSlot, vertexBufferCount, &m_vertexBuffer, &bufferStride, &bufferOffset);
	}
	
	// Setting index buffer
	{
		EAE6320_ASSERT(m_vertexIndexBuffer != NULL);
		// Every index is a 16 bit unsigned integer
		const DXGI_FORMAT format = DXGI_FORMAT_R16_UINT;
		// The indices start at the beginning of the buffer
		const unsigned int offset = 0;
		direct3dImmediateContext->IASetIndexBuffer(m_vertexIndexBuffer, format, offset);
	}

	// Render triangles from the currently-bound vertex buffer
	//{
	//	// As of this comment we are only drawing a single triangle
	//	// (you will have to update this code in future assignments!)
	//	const unsigned int triangleCount = 2;
	//	const unsigned int vertexCountPerTriangle = 3;
	//	const unsigned int vertexCountToRender = triangleCount * vertexCountPerTriangle;
	//	// It's possible to start rendering primitives in the middle of the stream
	//	const unsigned int indexOfFirstVertexToRender = 0;
	//	direct3dImmediateContext->Draw(vertexCountToRender, indexOfFirstVertexToRender);
	//}

	// Render triangles from the currently-bound vertex and index buffers
	{
		// It's possible to start rendering primitives in the middle of the stream
		const unsigned int indexOfFirstIndexToUse = 0;
		const unsigned int offsetToAddToEachIndex = 0;
		direct3dImmediateContext->DrawIndexed(m_noOfIndices, indexOfFirstIndexToUse, offsetToAddToEachIndex);
	}
}