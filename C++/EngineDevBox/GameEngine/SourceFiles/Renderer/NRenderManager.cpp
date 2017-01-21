#include "Renderer\NRenderManager.h"

#include <assert.h>

#include "GLib.h"

namespace Illehc
{
	namespace Renderer
	{
		void Init()
		{
			RenderManager::Instance()->Init();
		}

		void Draw(void)
		{
			RenderManager::Instance()->Draw();
		}

		void ShutDown(void)
		{
			delete RenderManager::Instance();
		}

		RenderManager * RenderManager::_instance = 0;

		RenderManager * RenderManager :: Instance(void)
		{
			if (_instance == 0) {
				_instance = new RenderManager();
			}
			return _instance;
		}

		RenderManager :: RenderManager()
		{

		}

		void RenderManager :: Init()
		{
			
		}

		void RenderManager :: Draw(void) 
		{
			GLib::BeginRendering();
			GLib::Sprites::BeginRendering();

			for (std::list< RenderInfo * >::iterator it = m_Infos.begin(); it != m_Infos.end(); it++)
			{
				RenderInfo * info = *it;
				Vector3 position = info->gameObject->position;
				GLib::Point2D glibPosition = { position.getX(), position.getY() };
				GLib::Sprites::RenderSprite(*(info->sprite), glibPosition);
			}

			GLib::Sprites::EndRendering();
			GLib::EndRendering();
		}

		void * RenderManager :: LoadFile(const char * i_pFilename, size_t & o_sizeFile)
		{
			assert(i_pFilename != NULL);

			FILE * pFile = NULL;

			errno_t fopenError = fopen_s(&pFile, i_pFilename, "rb");
			if (fopenError != 0)
				return NULL;

			assert(pFile != NULL);

			int FileIOError = fseek(pFile, 0, SEEK_END);
			assert(FileIOError == 0);

			long FileSize = ftell(pFile);
			assert(FileSize >= 0);

			FileIOError = fseek(pFile, 0, SEEK_SET);
			assert(FileIOError == 0);

			uint8_t * pBuffer = new uint8_t[FileSize];
			assert(pBuffer);

			size_t FileRead = fread(pBuffer, 1, FileSize, pFile);
			assert(FileRead == FileSize);

			fclose(pFile);

			o_sizeFile = FileSize;

			return pBuffer;
		}

		GLib::Sprites::Sprite * RenderManager :: CreateSprite(const char * i_pFileName)
		{
			assert(i_pFileName);

			size_t sizeTextureFile = 0;

			void * pTextureFile = LoadFile(i_pFileName, sizeTextureFile);

			GLib::Texture * pTexture = pTextureFile ? GLib::CreateTexture(pTextureFile, sizeTextureFile) : nullptr;

			if (pTextureFile)
				delete[] pTextureFile;

			if (pTexture == nullptr)
				return NULL;

			unsigned int width = 0;
			unsigned int height = 0;
			unsigned int depth = 0;

			bool result = GLib::GetDimensions(pTexture, width, height, depth);
			assert(result == true);
			assert((width > 0) && (height > 0));

			GLib::Sprites::SpriteEdges	Edges = { -float(width / 2.0f), float(height), float(width / 2.0f), 0.0f };
			GLib::Sprites::SpriteUVs	UVs = { { 0.0f, 0.0f },{ 1.0f, 0.0f },{ 0.0f, 1.0f },{ 1.0f, 1.0f } };
			GLib::RGBA							Color = { 255, 255, 255, 255 };

			GLib::Sprites::Sprite * pSprite = GLib::Sprites::CreateSprite(Edges, 0.1f, Color, UVs);
			if (pSprite == nullptr)
			{
				GLib::Release(pTexture);
				return nullptr;
			}

			GLib::Sprites::SetTexture(*pSprite, *pTexture);

			return pSprite;
		}

		RenderInfo * RenderManager :: AddRenderer(SmartPtr<GameObject>& i_GameObject, const char * const i_pFileName)
		{
			RenderInfo * info = new RenderInfo;
			info->gameObject = i_GameObject;

			GLib::Sprites::Sprite * sprite = CreateSprite(i_pFileName);
			info->sprite = sprite;

			m_Infos.push_back(info);

			return info;
		}

		void RenderManager :: RemoveRenderer(RenderInfo * i_Info)
		{
			m_Infos.remove(i_Info);
			GLib::Sprites::Release(i_Info->sprite);
			delete i_Info;
		}

		RenderManager :: ~RenderManager()
		{
			for (std::list< RenderInfo * >::iterator it = m_Infos.begin(); it != m_Infos.end(); it++)
			{
				delete (*it);
			}

			m_Infos.clear();
			GLib::Shutdown();
		}
	} // namespace Renderer
}// namespace Illehc