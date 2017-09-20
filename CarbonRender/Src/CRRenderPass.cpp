#include "..\Inc\CRRenderPass.h"

RenderPass::~RenderPass()
{
	glDeleteFramebuffers(1, &fbo);
	glDeleteTextures(output.cout, output.RTS);
	glDeleteBuffers(1, &dBuffer);
	glDeleteVertexArrays(1, &quadVao);
	glDeleteBuffers(3, quadBos);
}

void RenderPass::AttachShader(GLuint program)
{
	shaderProgram = program;
}

void RenderPass::Init()
{
}

PassOutput* RenderPass::Draw(PassOutput* input)
{
	if (!bReady)
	{
		GetReady4Render(input);
		bReady = true;
	}
	if (!enable)
		return &output;
	Render(input);

	return &output;
}

void RenderPass::Enable(bool bEnable)
{
	enable = bEnable;
}

void RenderPass::GetReady4Render(PassOutput * input)
{
}

void RenderPass::Render(PassOutput * input)
{
}

void RenderPass::DrawFullScreenQuad()
{
	if (!fullScreenQuadReady)
	{
		float vert[8] = { -1.0f, -1.0f,
						1.0f, -1.0f,
						1.0f, 1.0f,
						-1.0f, 1.0f };

		unsigned int index[6] = { 0,1,3,
								1,2,3 };

		float uv[8] = { 0.0f, 0.0f,
						1.0f, 0.0f,
						1.0f, 1.0f,
						0.0f, 1.0f};

		glGenVertexArrays(1, &quadVao);
		glGenBuffers(3, quadBos);

		glBindVertexArray(quadVao);
		SetGLArrayBuffer(quadBos[0], sizeof(vert), vert, 2, GL_FLOAT, CR_VERTATTRIPOS_POS);
		glEnableVertexAttribArray(CR_VERTATTRIPOS_POS);
		SetGLArrayBuffer(quadBos[1], sizeof(uv), uv, 2, GL_FLOAT, CR_VERTATTRIPOS_UVS);
		glEnableVertexAttribArray(CR_VERTATTRIPOS_UVS);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, quadBos[2]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(index), index, GL_STATIC_DRAW);

		glBindVertexArray(NULL);
		glBindBuffer(GL_ARRAY_BUFFER, NULL);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, NULL);

		fullScreenQuadReady = true;
	}

	glBindVertexArray(quadVao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, quadBos[2]);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);

	glBindVertexArray(NULL);
	glBindBuffer(GL_ARRAY_BUFFER, NULL);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, NULL);
}