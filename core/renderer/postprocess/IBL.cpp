#include "IBL.h"
#include "utils/ServiceLocator.h"
#include "core/renderer/RenderQuad.h"
#include "core/renderer/RenderCube.h"
namespace core
{
	IBL::IBL() {
		Initialize();
	}

	IBL::~IBL() {
		glDeleteFramebuffers(1, &m_BrdfFramebuffer);
		glDeleteTextures(1, &m_BrdfLUTTexture);
		glDeleteFramebuffers(1, &m_CubemapFramebuffer);
		glDeleteTextures(1, &m_CubemapTexture);
		glDeleteFramebuffers(1, &m_EquiRectangleFramebuffer);
		glDeleteTextures(1, &m_EquiRectangleTexture);
	}

	void IBL::Initialize() {
		InitBrdf();
		InitCubemap();
		//InitEquiRectangle();
		glGenFramebuffers(1, &m_IrradianceFramebuffer);
		glGenRenderbuffers(1, &m_IrradianceRenderBuffer);

		glBindFramebuffer(GL_FRAMEBUFFER, m_IrradianceFramebuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, m_IrradianceRenderBuffer);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 512, 512);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_IrradianceRenderBuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		InitIrradianceCubemap();
		InitPrefilterMap();
	}

	// BRDF LUT
	void IBL::InitBrdf() {
		m_BrdfResolution = 512;
		m_BrdfShader = Shader(
			std::string(PROJECT_ROOT) + "/assets/shader/brdf.vert",
			std::string(PROJECT_ROOT) + "/assets/shader/brdf.frag"
		);
		CreateBrdfFramebuffer();
		RenderBrdfLUT();
	}

	void IBL::CreateBrdfLUTTexture() {
		glGenTextures(1, &m_BrdfLUTTexture);
		glBindTexture(GL_TEXTURE_2D, m_BrdfLUTTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RG16F, m_BrdfResolution, m_BrdfResolution, 0, GL_RG, GL_FLOAT, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	}

	void IBL::CreateBrdfFramebuffer() {
		glGenFramebuffers(1, &m_BrdfFramebuffer);
		SetFramebufferAndViewport(m_BrdfFramebuffer, m_BrdfResolution, m_BrdfResolution);
		CreateBrdfLUTTexture();
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_BrdfLUTTexture, 0);
	}

	void IBL::RenderBrdfLUT() {
		m_BrdfShader.use();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		m_RenderQuad.renderQuad();
		UnbindFramebuffer();
	}

	// Cubemap
	void IBL::InitCubemap() {
		m_CubemapSize = 512;
		CreateCubemapTexture();
		CreateCubemapFramebuffer();
	}

	void IBL::CreateCubemapTexture() {
		glGenTextures(1, &m_CubemapTexture);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_CubemapTexture);
		for (int i = 0; i < 6; ++i) {
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, m_CubemapSize, m_CubemapSize, 0, GL_RGB, GL_FLOAT, NULL);
		}
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	}

	void IBL::CreateCubemapFramebuffer() {
		glGenFramebuffers(1, &m_CubemapFramebuffer);
		SetFramebufferAndViewport(m_CubemapFramebuffer, m_CubemapSize, m_CubemapSize);
	}

	void IBL::BindCubemapFramebuffer() {
		SetFramebufferAndViewport(m_CubemapFramebuffer, m_CubemapSize, m_CubemapSize);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void IBL::RenderAtmosphereToCubemapFace(int faceIndex) {
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + faceIndex, m_CubemapTexture, 0);
	}

	// --------------------------------------------------Equirectangular Conversion-------------------------------------------
	void IBL::InitEquiRectangle() {
		m_EquiRectangleSize = 512;
		CreateEquiRectangleTexture();
		CreateEquiRectangleFramebuffer();

		m_EquiRectangleShader = Shader(
			std::string(PROJECT_ROOT) + "/assets/shader/equiRectangle.vert",
			std::string(PROJECT_ROOT) + "/assets/shader/equiRectangle.frag"
		);
	}

	void IBL::CreateEquiRectangleTexture() {
		glGenTextures(1, &m_EquiRectangleTexture);
		glBindTexture(GL_TEXTURE_2D, m_EquiRectangleTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, m_EquiRectangleSize, m_EquiRectangleSize, 0, GL_RGB, GL_FLOAT, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	}

	void IBL::CreateEquiRectangleFramebuffer() {
		glGenFramebuffers(1, &m_EquiRectangleFramebuffer);
		SetFramebufferAndViewport(m_EquiRectangleFramebuffer, m_EquiRectangleSize, m_EquiRectangleSize);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_EquiRectangleTexture, 0);
	}

	void IBL::RenderCubemapToEquiRectangle() {
		SetFramebufferAndViewport(m_EquiRectangleFramebuffer, m_EquiRectangleSize, m_EquiRectangleSize);
		m_EquiRectangleShader.use();
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_CubemapTexture);
		m_RenderQuad.renderQuad();
		UnbindFramebuffer();
	}

	//------------------------------------irradiance map-------------------------------------------------------------------
	void IBL::InitIrradianceCubemap()
	{
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_CubemapTexture);
		glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

		glGenTextures(1, &m_IrradianceMap);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_IrradianceMap);
		for (unsigned int i = 0; i < 6; ++i)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 32, 32, 0, GL_RGB, GL_FLOAT, nullptr);
		}
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

		m_IrradianceShader = Shader(
			std::string(PROJECT_ROOT) + "/assets/shader/cubemap.vert",
			std::string(PROJECT_ROOT) + "/assets/shader/irradiance.frag"
		);

		glBindFramebuffer(GL_FRAMEBUFFER, m_IrradianceFramebuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, m_IrradianceRenderBuffer);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 32, 32);

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
			std::cout << "Framebuffer is not complete!" << std::endl;
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
	}

	

	void IBL::RenderIrradianceCubemap()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_IrradianceFramebuffer);
		// Bind and set up the framebuffer
		glViewport(0, 0, 32, 32);

		// Use the irradiance convolution shader
		m_IrradianceShader.use();
		m_IrradianceShader.setInt("environmentMap", 0);
		m_IrradianceShader.setMat4("projection", projection);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_CubemapTexture);

		// Render to each cubemap face
		for (unsigned int i = 0; i < 6; ++i)
		{
			// Set view and projection matrix for current face
			m_IrradianceShader.setMat4("view", views[i]);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, m_IrradianceMap, 0);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			RenderCube::Render();
		}

		glBindRenderbuffer(GL_RENDERBUFFER, 0);
	}

	
	//------------------------------------------prefilter map------------------------------------------
	void IBL::InitPrefilterMap()
	{
		glGenTextures(1, &m_PrefilterMap);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_PrefilterMap);
		for (unsigned int i = 0; i < 6; ++i)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 128, 128, 0, GL_RGB, GL_FLOAT, nullptr);
		}
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); // be sure to set minification filter to mip_linear 
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		// generate mipmaps for the cubemap so OpenGL automatically allocates the required memory.
		glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

		m_PrefilterShader = Shader(
			std::string(PROJECT_ROOT) + "/assets/shader/cubemap.vert",
			std::string(PROJECT_ROOT) + "/assets/shader/prefilter.frag"
		);
	}

	void IBL::RenderPrefilterCubemap()
	{
		m_PrefilterShader.use();
		m_PrefilterShader.setInt("environmentMap", 0);
		m_PrefilterShader.setMat4("projection", projection);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_CubemapTexture);

		glBindFramebuffer(GL_FRAMEBUFFER, m_IrradianceFramebuffer);
		unsigned int maxMipLevels = 5;
		for (unsigned int mip = 0; mip < maxMipLevels; ++mip)
		{
			// reisze framebuffer according to mip-level size.
			unsigned int mipWidth = static_cast<unsigned int>(128 * std::pow(0.5, mip));
			unsigned int mipHeight = static_cast<unsigned int>(128 * std::pow(0.5, mip));
			glBindRenderbuffer(GL_RENDERBUFFER, m_IrradianceRenderBuffer);
			glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, mipWidth, mipHeight);
			glViewport(0, 0, mipWidth, mipHeight);

			float roughness = (float)mip / (float)(maxMipLevels - 1);
			m_PrefilterShader.setFloat("roughness", roughness);
			for (unsigned int i = 0; i < 6; ++i)
			{
				m_PrefilterShader.setMat4("view", views[i]);
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, m_PrefilterMap, mip);

				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
				RenderCube::Render();
			}
		}
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}



	void IBL::UnbindFramebuffer() {
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		auto windowSpecifc = utils::ServiceLocator::GetWindow()->GetWinSpecification();
		glViewport(0, 0, windowSpecifc.width, windowSpecifc.height);
	}

	void IBL::SetFramebufferAndViewport(uint32_t framebuffer, uint32_t width, uint32_t height) {
		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
		glViewport(0, 0, width, height);
	}
}
