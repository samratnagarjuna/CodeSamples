// Header Files
//=============

#include "../cEffect.h"

#include "../../../Engine/Platform/Platform.h"
#include "../../../Engine/Asserts/Asserts.h"
#include "../../../Engine/Logging/Logging.h"
#include "../../../Engine/Graphics/Direct3D/RenderContext.d3d.h"
#include "../Includes.h"

// Initialization / CleanUp

eae6320::Graphics::cEffect::cEffect() : m_vertexShader(NULL), m_fragmentShader(NULL), m_vertexLayout(NULL)
{

}

bool eae6320::Graphics::cEffect::Load(const char* i_vertexShaderPath, const char* i_fragmentShaderPath)
{
	bool wereThereErrors = false;

	ID3D11Device* direct3dDevice = eae6320::Graphics::RenderContext::Instance()->GetDirect3DDevice();

	eae6320::Platform::sDataFromFile compiledVertexShader;
	eae6320::Platform::sDataFromFile compiledFragementShader;

	// Load vertex shader
	{
		std::string errorMessage;
		if (!eae6320::Platform::LoadBinaryFile(i_vertexShaderPath, compiledVertexShader, &errorMessage))
		{
			wereThereErrors = true;
			EAE6320_ASSERTF(false, errorMessage.c_str());
			eae6320::Logging::OutputError("Failed to load the shader file \"%s\": %s", i_vertexShaderPath, errorMessage.c_str());
			goto OnExit;
		}
	}

	// Create the vertex shader object
	{
		ID3D11ClassLinkage* const noInterfaces = NULL;
		const HRESULT result = direct3dDevice->CreateVertexShader(compiledVertexShader.data, compiledVertexShader.size,
			noInterfaces, &m_vertexShader);
		if (FAILED(result))
		{
			EAE6320_ASSERT(false);
			eae6320::Logging::OutputError("Direct3D failed to create the vertex shader with HRESULT %#010x", result);
			wereThereErrors = true;
			goto OnExit;
		}
	}

	// Load fragment shader
	{
		std::string errorMessage;
		if (!eae6320::Platform::LoadBinaryFile(i_fragmentShaderPath, compiledFragementShader, &errorMessage))
		{
			wereThereErrors = true;
			EAE6320_ASSERTF(false, errorMessage.c_str());
			eae6320::Logging::OutputError("Failed to load the shader file \"%s\": %s", i_fragmentShaderPath, errorMessage.c_str());
			goto OnExit;
		}
	}
	// Create the fragment shader object
	{
		ID3D11ClassLinkage* const noInterfaces = NULL;
		const HRESULT result = direct3dDevice->CreatePixelShader(
			compiledFragementShader.data, compiledFragementShader.size, noInterfaces, &m_fragmentShader);
		if (FAILED(result))
		{
			wereThereErrors = true;
			EAE6320_ASSERT(false);
			eae6320::Logging::OutputError("Direct3D failed to create the shader %s with HRESULT %#010x", i_fragmentShaderPath, result);
			goto OnExit;
		}
	}

	// Create the vertex layout
	{
		// These elements must match the VertexFormat::sVertex layout struct exactly.
		// They instruct Direct3D how to match the binary data in the vertex buffer
		// to the input elements in a vertex shader
		// (by using so-called "semantic" names so that, for example,
		// "POSITION" here matches with "POSITION" in shader code).
		// Note that OpenGL uses arbitrarily assignable number IDs to do the same thing.
		const unsigned int vertexElementCount = 3;
		D3D11_INPUT_ELEMENT_DESC layoutDescription[vertexElementCount] = { 0 };
		{
			// Slot 0

			// POSITION
			// 3 floats == 12 bytes
			// Offset = 0
			{
				D3D11_INPUT_ELEMENT_DESC& positionElement = layoutDescription[0];

				positionElement.SemanticName = "POSITION";
				positionElement.SemanticIndex = 0;	// (Semantics without modifying indices at the end can always use zero)
				positionElement.Format = DXGI_FORMAT_R32G32B32_FLOAT;
				positionElement.InputSlot = 0;
				positionElement.AlignedByteOffset = offsetof(sVertex, x);
				positionElement.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
				positionElement.InstanceDataStepRate = 0;	// (Must be zero for per-vertex data)
			}

			// Slot 1
			// COLOR
			// 4 uint8_t == 4 bytes
			// Offset = 12 bytes
			{
				D3D11_INPUT_ELEMENT_DESC& colorElement = layoutDescription[1];

				colorElement.SemanticName = "COLOR";
				colorElement.SemanticIndex = 0;	// (Semantics without modifying indices at the end can always use zero)
				colorElement.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
				colorElement.InputSlot = 0;
				colorElement.AlignedByteOffset = offsetof(sVertex, r);
				colorElement.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
				colorElement.InstanceDataStepRate = 0;	// (Must be zero for per-vertex data)
			}

			// Slot 2
			// TEXCOORD
			// 2 floats == 8 bytes
			// Offset = 16 bytes
			{
				D3D11_INPUT_ELEMENT_DESC& texCoordElement = layoutDescription[2];

				texCoordElement.SemanticName = "TEXCOORD";
				texCoordElement.SemanticIndex = 0;	// (Semantics without modifying indices at the end can always use zero)
				texCoordElement.Format = DXGI_FORMAT_R32G32_FLOAT;
				texCoordElement.InputSlot = 0;
				texCoordElement.AlignedByteOffset = offsetof(sVertex, u);
				texCoordElement.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
				texCoordElement.InstanceDataStepRate = 0;	// (Must be zero for per-vertex data)
			}
		}

		const HRESULT result = direct3dDevice->CreateInputLayout(layoutDescription, vertexElementCount,
			compiledVertexShader.data, compiledVertexShader.size, &m_vertexLayout);
		if (FAILED(result))
		{
			EAE6320_ASSERT(false);
			eae6320::Logging::OutputError("Direct3D failed to create a vertex input layout with HRESULT %#010x", result);
			wereThereErrors = true;
			goto OnExit;
		}
	}

OnExit:

	compiledVertexShader.Free();
	compiledFragementShader.Free();

	return !wereThereErrors;
}

bool eae6320::Graphics::cEffect::CleanUp()
{
	bool wereThereErrors = false;

	m_renderState.CleanUp();

	if (m_vertexLayout)
	{
		m_vertexLayout->Release();
		m_vertexLayout = NULL;
	}

	if (m_vertexShader)
	{
		m_vertexShader->Release();
		m_vertexShader = NULL;
	}

	if (m_fragmentShader)
	{
		m_fragmentShader->Release();
		m_fragmentShader = NULL;
	}

	return !wereThereErrors;
}

void eae6320::Graphics::cEffect::Bind() 
{
	// Bind RenderState
	{
		m_renderState.Bind();
	}

	ID3D11DeviceContext* direct3dImmediateContext = eae6320::Graphics::RenderContext::Instance()->GetDirect3DImmediateContext();
	// Set the vertex and fragment shaders
	{
		ID3D11ClassInstance** const noInterfaces = NULL;
		const unsigned int interfaceCount = 0;
		direct3dImmediateContext->VSSetShader(m_vertexShader, noInterfaces, interfaceCount);
		direct3dImmediateContext->PSSetShader(m_fragmentShader, noInterfaces, interfaceCount);
	}

	{
		// Set the layout (which defines how to interpret a single vertex)
		direct3dImmediateContext->IASetInputLayout(m_vertexLayout);
		// Set the topology (which defines how to interpret multiple vertices as a single "primitive";
		// we have defined the vertex buffer as a triangle list
		// (meaning that every primitive is a triangle and will be defined by three vertices)
		direct3dImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	}
}