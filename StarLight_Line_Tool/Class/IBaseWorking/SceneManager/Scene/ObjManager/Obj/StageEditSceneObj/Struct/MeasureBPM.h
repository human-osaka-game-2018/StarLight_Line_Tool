#ifndef MEASURE_BPM_H
#define MEASURE_BPM_H

#include <windows.h>

#include "../../../../../../../../GameLib/DX/DX3D/CustomVertexEditor/Data/ObjData.h"
#include "../../../../../../../../GameLib/DX/DX3D/CustomVertexEditor/Data/CustomVertex.h"

struct MeasureBPM
{
public:
	static const INT m_DIGITS_MAX = 3;				//! BPMの桁数
	INT m_digitBPMs[m_DIGITS_MAX] = { 0, 9, 0 };	//! デフォルトのBPM

	ObjData m_bPMDatas[m_DIGITS_MAX];
	CustomVertex m_bPM[4 * m_DIGITS_MAX];

	ObjData m_bPMUpperDatas[m_DIGITS_MAX];
	CustomVertex m_bPMUppers[4 * m_DIGITS_MAX];

	ObjData m_bPMDownerDatas[m_DIGITS_MAX];
	CustomVertex m_bPMDowners[4 * m_DIGITS_MAX];
};

#endif // !MEASURE_BPM_H
