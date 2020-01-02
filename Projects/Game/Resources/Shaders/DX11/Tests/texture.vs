cbuffer MatrixBuffer : register(b0)
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
};

struct VertexInputType
{
    float4 position : POSITION;
    float2 uv : TEXCOORD;
};

struct PixelInputType
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD;
};

PixelInputType VS(VertexInputType input)
{
    PixelInputType output;
    
	// Change the position vector to be 4 units for proper matrix calculations.
    input.position.w = 1.0f;

	// Calculate the position of the vertex against the world, view, and projection matrices.
    output.position = mul(worldMatrix, input.position);
    output.position = mul(viewMatrix, output.position);
    output.position = mul(projectionMatrix, output.position);
    
	// Store the input uv for the pixel shader to use.
    output.uv = input.uv;
    
    return output;
}