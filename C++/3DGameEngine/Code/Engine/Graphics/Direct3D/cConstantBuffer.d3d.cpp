// Header Files
//=============

#include "../cConstantBuffer.h"

#include <D3D11.h>
#include "../../Asserts/Asserts.h"
#include "../../Logging/Logging.h"
#include "RenderContext.d3d.h"
#include "../Math/Functions.h"
#include "../Includes.h"

bool eae6320::Graphics::cConstantBuffer::CreateConstantBuffer()
{
	bool wereThereErrors = false;
	D3D11_BUFFER_DESC bufferDescription = { 0 };
	{
		// The byte width must be rounded up to a multiple of 16
		// TODO: round this to a multiple of 16 before assigning
		bufferDescription.ByteWidth = Math::RoundUpToMultiple_powerOf2(static_cast<unsigned int>(m_bufferSize), 16u);
		bufferDescription.Usage = D3D11_USAGE_DYNAMIC;	// The CPU must be able to update the buffer
		bufferDescription.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bufferDescription.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;	// The CPU must write, but doesn't read
		bufferDescription.MiscFlags = 0;
		bufferDescription.StructureByteStride = 0;	// Not used
	}
	D3D11_SUBRESOURCE_DATA initialData = { 0 };
	{
		// Fill in the constant buffer
		initialData.pSysMem = m_pBufferData;
		// (The other data members are ignored for non-texture buffers)
	}

	// Get device context
	ID3D11Device* direct3dDevice = RenderContext::Instance()->GetDirect3DDevice();
	const HRESULT result = direct3dDevice->CreateBuffer(&bufferDescription, &initialData, &m_pConstantBuffer);
	if (!SUCCEEDED(result))
	{
		EAE6320_ASSERT(false);
		eae6320::Logging::OutputError("Direct3D failed to create a constant buffer with HRESULT %#010x", result);
		wereThereErrors = true;
		goto OnExit;
	}

OnExit:
	return !wereThereErrors;
}

bool eae6320::Graphics::cConstantBuffer::BindConstantBuffer()
{
	const unsigned int registerAssignedInShader = GetBufferRegisterID();
	const unsigned int bufferCount = 1;
	{
		// Get device context
		ID3D11DeviceContext* direct3dImmediateContext = RenderContext::Instance()->GetDirect3DImmediateContext();
		direct3dImmediateContext->VSSetConstantBuffers(registerAssignedInShader, bufferCount, &m_pConstantBuffer);
		direct3dImmediateContext->PSSetConstantBuffers(registerAssignedInShader, bufferCount, &m_pConstantBuffer);
	}
	return true;
}

void eae6320::Graphics::cConstantBuffer::Update(void* i_pConstantBufferData)
{
	// Get device context
	ID3D11DeviceContext* direct3dImmediateContext = RenderContext::Instance()->GetDirect3DImmediateContext();

	// Get a pointer from Direct3D that can be written to
	void* memoryToWriteTo = NULL;
	{
		D3D11_MAPPED_SUBRESOURCE mappedSubResource;
		{
			// Discard previous contents when writing
			const unsigned int noSubResources = 0;
			const D3D11_MAP mapType = D3D11_MAP_WRITE_DISCARD;
			const unsigned int noFlags = 0;
			const HRESULT result = direct3dImmediateContext->Map(m_pConstantBuffer, noSubResources, mapType, noFlags, &mappedSubResource);
			if (SUCCEEDED(result))
			{
				memoryToWriteTo = mappedSubResource.pData;
			}
			else
			{
				EAE6320_ASSERT(false);
			}
		}
	}
	if (memoryToWriteTo)
	{
		// Copy the new data to the memory that Direct3D has provided
		memcpy(memoryToWriteTo, i_pConstantBufferData, m_bufferSize);
		// Let Direct3D know that the memory contains the data
		// (the pointer will be invalid after this call)
		const unsigned int noSubResources = 0;
		direct3dImmediateContext->Unmap(m_pConstantBuffer, noSubResources);
		memoryToWriteTo = NULL;
	}
}

bool eae6320::Graphics::cConstantBuffer::CleanUp()
{
	if (m_pConstantBuffer)
	{
		m_pConstantBuffer->Release();
		m_pConstantBuffer = NULL;
	}

	return true;
}