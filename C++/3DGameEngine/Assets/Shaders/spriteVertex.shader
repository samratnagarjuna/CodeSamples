/*
	This is the vertex shader used to render sprites
*/

#if defined( EAE6320_PLATFORM_D3D )

// Constants
//==========

cbuffer constantBuffer_frame : register( b0 )
{
	float4x4 g_transform_worldToCamera;
	float4x4 g_transform_cameraToScreen;
	float g_elapsedSecondCount_total;
}
cbuffer constantBuffer_drawCall : register( b1 )
{
	float4x4 g_transform_localToWorld;
}
// Entry Point
//============

void main(

	// Input
	//======

	// The "semantics" (the keywords in all caps after the colon) are arbitrary,
	// but must match the C call to CreateInputLayout()

	// These values come from one of the sVertex that we filled the vertex buffer with in C code
	in const float3 i_vertexPosition_screen : POSITION,
	in const float4 i_color : COLOR,
	in const float2 i_uv : TEXCOORD,

	// Output
	//=======

	// An SV_POSITION value must always be output from every vertex shader
	// so that the GPU can figure out which fragments need to be shaded
	out float4 o_vertexPosition_screen : SV_POSITION,

	// Any other data is optional; the GPU doesn't know or care what it is,
	// and will merely interpolate it across the triangle
	// and give us the resulting interpolated value in a fragment shader.
	// It is then up to us to use it however we want to.
	// The "semantics" are used to match the vertex shader outputs
	// with the fragment shader inputs
	// (note that OpenGL uses arbitrarily assignable number IDs to do the same thing).
	out float4 o_color : COLOR,
	out float2 o_uv : TEXCOORD

	)
{
	o_vertexPosition_screen = float4( i_vertexPosition_screen, 1.0 );
	
	// Pass the input color to the fragment shader unchanged
	{
		o_color = i_color;
	}
	// Pass the input uv to the fragment shader unchanged
	{
		o_uv = i_uv;
	}
}


#elif defined( EAE6320_PLATFORM_GL )
// The version of GLSL to use must come first
#version 420

// Constants
//==========

layout( std140, binding = 0 ) uniform constantBuffer_frame
{
	mat4 g_transform_worldToCamera;
	mat4 g_transform_cameraToScreen;
	float g_elapsedSecondCount_total;
};
layout( std140, binding = 1 ) uniform constantBuffer_drawCall
{
	mat4 g_transform_localToWorld;
};

// Input
//======

// The locations assigned are arbitrary
// but must match the C calls to glVertexAttribPointer()

// These values come from one of the sVertex that we filled the vertex buffer with in C code
layout( location = 0 ) in vec3 i_vertexPosition_screen;
layout( location = 1 ) in vec4 i_color;
layout( location = 2 ) in vec2 i_uv;

// Output
//=======

// None; the vertex shader must always output a position value,
// but unlike HLSL where the value is explicit
// GLSL has an implicit required variable called "gl_Position"

// Any other data is optional; the GPU doesn't know or care what it is,
// and will merely interpolate it across the triangle
// and give us the resulting interpolated value in a fragment shader.
// It is then up to us to use it however we want to.
// The locations are used to match the vertex shader outputs
// with the fragment shader inputs
// (note that Direct3D uses arbitrarily assignable "semantics").
layout( location = 0 ) out vec4 o_color;
layout( location = 1 ) out vec2 o_uv;

// Entry Point
//============

void main()
{

	gl_Position = vec4( i_vertexPosition_screen, 1.0 );
	// Pass the input color to the fragment shader unchanged
	{
		o_color = i_color;
	}
	// Pass the input uv to the fragment shader unchanged
	{
		o_uv = i_uv;
	}
}

#endif