// Header Files
//=============

#include "cConstantBuffer.h"

const eae6320::Graphics::cConstantBuffer::ConstantBufferRegisterID eae6320::Graphics::cConstantBuffer::s_registerIds = 
{
	0, //frameRegisterId
	1, //drawRegisterId
	2  //materialRegisterId
};

bool eae6320::Graphics::cConstantBuffer::Initialize(ConstantBufferType i_bufferType, size_t i_bufferSize, void* i_pBufferData)
{
	bool wereThereErrors = false;

	m_bufferType = i_bufferType;
	m_bufferSize = i_bufferSize;
	m_pBufferData = i_pBufferData;

	if (!CreateConstantBuffer())
	{
		wereThereErrors = true;
		goto OnExit;
	}

	if (!BindConstantBuffer())
	{
		wereThereErrors = true;
		goto OnExit;
	}
OnExit:
	return !wereThereErrors;
}

unsigned int eae6320::Graphics::cConstantBuffer::GetBufferRegisterID()
{
	unsigned int registerId = -1;
	switch (m_bufferType)
	{
	case CONSTANT_FRAME_BUFFER: registerId = s_registerIds.frameRegisterId; break;
	case CONSTANT_DRAW_BUFFER: registerId = s_registerIds.drawRegisterId; break;
	case CONSTANT_MATERIAL_BUFFER:registerId = s_registerIds.materialRegisterId; break;
	default: registerId = -1;
	}

	return registerId;
}

bool eae6320::Graphics::cConstantBuffer::Bind() 
{
	return BindConstantBuffer();
}

