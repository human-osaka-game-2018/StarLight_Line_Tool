#ifndef OBJ_H
#define OBJ_H

#include <windows.h>

#include <d3dx9.h>

#include "../../../../IBaseWorking.h"
#include "Enum\OBJ_TYPE.h"
#include "../../../../../../GameLib/GameLib.h"

class Obj :public IBaseWorking
{
public:
	Obj(OBJ_TYPE objectType, FLOAT z) :m_rGameLib(GameLib::GetInstance()),
		m_WND_SIZE(m_rGameLib.GetWndSize()), m_OBJECT_TYPE(objectType), m_Z(z) {};
	virtual ~Obj() {};

	inline OBJ_TYPE GetObjectType() const
	{
		return m_OBJECT_TYPE;
	}

	inline FLOAT GetZ() const
	{
		return m_Z;
	}

protected:
	inline BOOL UpKeyIsPressed() const
	{
		if (m_rGameLib.KeyboardIsPressed(DIK_W)			||
			m_rGameLib.KeyboardIsPressed(DIK_NUMPAD8)	||
			m_rGameLib.KeyboardIsPressed(DIK_UP))
		{
			return TRUE;
		}

		return FALSE;
	}

	inline BOOL DownKeyIsPressed() const
	{
		if (m_rGameLib.KeyboardIsPressed(DIK_S)			||
			m_rGameLib.KeyboardIsPressed(DIK_NUMPAD2)	||
			m_rGameLib.KeyboardIsPressed(DIK_DOWN))
		{
			return TRUE;
		}

		return FALSE;
	}

	inline BOOL RightKeyIsPressed() const
	{
		if (m_rGameLib.KeyboardIsPressed(DIK_D)			||
			m_rGameLib.KeyboardIsPressed(DIK_NUMPAD6)	||
			m_rGameLib.KeyboardIsPressed(DIK_RIGHT))
		{
			return TRUE;
		}

		return FALSE;
	}

	inline BOOL LeftKeyIsPressed() const
	{
		if (m_rGameLib.KeyboardIsPressed(DIK_A)			||
			m_rGameLib.KeyboardIsPressed(DIK_NUMPAD4)	||
			m_rGameLib.KeyboardIsPressed(DIK_LEFT))
		{
			return TRUE;
		}

		return FALSE;
	}

	inline BOOL ReturnKeyIsPressed() const
	{
		if (m_rGameLib.KeyboardIsPressed(DIK_RETURN) ||
			m_rGameLib.KeyboardIsPressed(DIK_NUMPADENTER))
		{
			return TRUE;
		}

		return FALSE;
	}

	GameLib& m_rGameLib;

	const RectSize m_WND_SIZE = { 0,0 };

	const OBJ_TYPE m_OBJECT_TYPE = OT_TRANSPARENCY;
	const FLOAT m_Z = 1.0f;
};

#endif //! OBJ_H
