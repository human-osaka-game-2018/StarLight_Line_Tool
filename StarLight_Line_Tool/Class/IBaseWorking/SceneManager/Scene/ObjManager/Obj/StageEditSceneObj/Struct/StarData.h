#ifndef STAR_DATA_H
#define STAR_DATA_H

#include <windows.h>

#include "../../../../../../../../GameLib/DX/DX3D/CustomVertexEditor/Data/ObjData.h"
#include "../../../../../../../../GameLib/DX/DX3D/CustomVertexEditor/Data/CustomVertex.h"
#include "../Enum/STAR_TYPE.h"

struct StarData
{
	INT m_starType = STAR_SCORE;

	INT m_measureNum	= 0;
	INT m_beatNum		= 0;
	INT m_noteNum		= 0;
	INT m_divideNum		= 0;

	FLOAT m_x			= 0.0f;
	FLOAT m_deg			= 0.0f;
	FLOAT m_xMovement	= 0.0f;

	BOOL m_isActiveChangingVector = FALSE;

	ObjData m_objData;
	CustomVertex m_customVertices[4];
};

#endif //! STAR_DATA_H
