// Header Files
//=============

#include "cMaterial.h"

#include "Includes.h"
#include "../Asserts/Asserts.h"
#include "../Platform/Platform.h"
#include "../Logging/Logging.h"

// Initialization and CleanUp

bool eae6320::Graphics::cMaterial::Load(const char* const i_path)
{
	bool wereThereErrors = false;

	// Load the binary material
	eae6320::Platform::sDataFromFile binaryMaterial;
	{
		std::string errorMessage;
		if (!eae6320::Platform::LoadBinaryFile(i_path, binaryMaterial, &errorMessage))
		{
			wereThereErrors = true;
			EAE6320_ASSERTF(false, errorMessage.c_str());
			eae6320::Logging::OutputError("Failed to load the binary material file \"%s\": %s", i_path, errorMessage.c_str());
			goto OnExit;
		}
	}

	// Extract binary data
	{
		size_t offset = 0;
		uint8_t* data = reinterpret_cast<uint8_t*>(binaryMaterial.data);
		
		if (!m_constantbuffer.Initialize(
				eae6320::Graphics::cConstantBuffer::CONSTANT_MATERIAL_BUFFER,
				sizeof(sConstantBufferMaterialData),
				reinterpret_cast<sConstantBufferMaterialData*>(data + offset)))
		{
			EAE6320_ASSERT(false);
			wereThereErrors = true;
			eae6320::Logging::OutputError("Failed to create a constant material buffer");
		}

		offset += sizeof(sConstantBufferMaterialData);

		uint8_t effectPathLen = *reinterpret_cast<uint8_t*>(data + offset);
		offset += sizeof(uint8_t);

		if (!m_effect.Load(reinterpret_cast<const char*>(data + offset)))
		{
			EAE6320_ASSERT(false);
			wereThereErrors = true;
			eae6320::Logging::OutputError("Failed to load effect file");
			goto OnExit;
		}

		offset += effectPathLen + 1;

		if (!m_texture.Load(reinterpret_cast<const char*>(data + offset)))
		{
			EAE6320_ASSERT(false);
			wereThereErrors = true;
			eae6320::Logging::OutputError("Failed to load texture file");
			goto OnExit;
		}
	}

OnExit:
	binaryMaterial.Free();
	return !wereThereErrors;
}

bool eae6320::Graphics::cMaterial::CleanUp()
{
	bool wereThereErrors = false;

	if (!m_constantbuffer.CleanUp())
	{
		wereThereErrors = true;
	}

	if (!m_effect.CleanUp()) 
	{
		wereThereErrors = true;
	}

	if (!m_texture.CleanUp())
	{
		wereThereErrors = true;
	}

	return wereThereErrors;
}

void eae6320::Graphics::cMaterial::Bind()
{
	// Bind Effect
	m_effect.Bind();

	// Bind texture
	{
		const unsigned int registerId = 0;
		m_texture.Bind(registerId);
	}

	// Bind constant material buffer
	m_constantbuffer.Bind();
}