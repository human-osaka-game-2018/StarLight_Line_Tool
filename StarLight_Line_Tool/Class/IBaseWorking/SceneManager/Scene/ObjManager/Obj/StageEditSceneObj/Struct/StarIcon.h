#ifndef STAR_ICON_H
#define STAR_ICON_H

#include <windows.h>

#include "../../../../../../../../GameLib/DX/DX3D/CustomVertexEditor/Data/ObjData.h"
#include "../../../../../../../../GameLib/DX/DX3D/CustomVertexEditor/Data/CustomVertex.h"

struct StarIcon
{
	ObjData m_iconData;
	CustomVertex m_icon[4];

	static const INT m_DIGITS_NUM = 4;
	INT m_num = 0;
	INT m_digitNums[m_DIGITS_NUM];

	ObjData m_digitNumData[m_DIGITS_NUM];
	CustomVertex m_digitNum[4 * m_DIGITS_NUM];
};

#endif //! STAR_ICON_H
