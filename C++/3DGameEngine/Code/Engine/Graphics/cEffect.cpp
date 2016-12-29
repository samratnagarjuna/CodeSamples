// Header Files
//=============

#include "cEffect.h"

#include "../Asserts/Asserts.h"
#include "../Platform/Platform.h"
#include "../Logging/Logging.h"

// Load
//=====

bool eae6320::Graphics::cEffect::Load(const char* i_effectPath)
{
	bool wereThereErrors = false;

	// Load the binary effect
	eae6320::Platform::sDataFromFile binaryEffect;
	{
		std::string errorMessage;
		if (!eae6320::Platform::LoadBinaryFile(i_effectPath, binaryEffect, &errorMessage))
		{
			wereThereErrors = true;
			EAE6320_ASSERTF(false, errorMessage.c_str());
			eae6320::Logging::OutputError("Failed to load the binary effect file \"%s\": %s", i_effectPath, errorMessage.c_str());
			goto OnExit;
		}
	}
	// Extract binary data
	uint8_t renderStateBits;
	uint8_t vertexPathLen;
	char*	vertexShaderPath;
	char*	fragmentShaderPath;
	{
		size_t offset = 0;
		uint8_t* data = reinterpret_cast<uint8_t*>(binaryEffect.data);

		renderStateBits = *reinterpret_cast<uint8_t*>(data + offset);
		offset += sizeof(uint8_t);

		vertexPathLen = *reinterpret_cast<uint8_t*>(data + offset);
		offset += sizeof(uint8_t);

		vertexShaderPath = reinterpret_cast<char*>(data + offset);
		offset += vertexPathLen + 1;

		fragmentShaderPath = reinterpret_cast<char*>(data + offset);
	}

	// Load data
	{
		// Initialize render state
		if (!m_renderState.Initialize(renderStateBits)) 
		{
			wereThereErrors = true;
			goto OnExit;
		}
		Load(vertexShaderPath, fragmentShaderPath);
	}

OnExit:

	binaryEffect.Free();
	return !wereThereErrors;
}