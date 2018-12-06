#include "SceneManager.h"

#include <windows.h>

#include "Scene\Scene.h"
#include "Scene\Enum\SCENE_KIND.h"
#include "Scene\TitleScene\TitleScene.h"
#include "Scene\StageSelectScene\StageSelectScene.h"
#include "Scene\SaveDataScene\SaveDataScene.h"
#include "Scene\StageEditScene\StageEditScene.h"

VOID SceneManager::Factory()
{
	if (m_currentScene == m_nextScene || m_isRequestedChangeResent) return;

	m_currentScene = m_nextScene;

	switch (m_nextScene)
	{
	case SK_TITLE:
		delete m_pScene;

		m_pScene = new TitleScene();

		break;

	case SK_GAME:
		delete m_pScene;

		m_pScene = new TitleScene();

		break;

	case SK_STAGE_SELECT:
		delete m_pScene;

		m_pScene = new StageSelectScene();

		break;

	case SK_SAVE_DATA:
		delete m_pScene;

		m_pScene = new SaveDataScene();

		break;

	case SK_STAGE_EDIT:
		delete m_pScene;

		m_pScene = new StageEditScene();
		
	default:
		break;
	}
}

VOID SceneManager:: StageSceneTransration()
{
	GameLib& rGameLib = GameLib::GetInstance();

	rGameLib.AddtionBlendMode();

	ObjData data;
	const RectSize WND_SIZE = rGameLib.GetWndSize();
	data.m_center		= { WND_SIZE.m_x * 0.5f, WND_SIZE.m_y * 0.5f, 0.0f };
	data.m_halfScale	= { WND_SIZE.m_x * 0.5f, WND_SIZE.m_y * 0.5f, 0.0f };

	static INT alpha = 0;
	alpha += 4 * ((m_isRequestedChangeResent) ? 1 : -1);
	if (alpha >= 255) m_isRequestedChangeResent = !(alpha = 255);
	alpha = (alpha > 0) * alpha;
	data.m_aRGB = D3DCOLOR_ARGB(static_cast<UCHAR>(alpha), 255, 255, 255);

	CustomVertex cover[4];
	rGameLib.CreateRect(cover, data);

	rGameLib.Render(cover);

	rGameLib.DefaultBlendMode();
}
