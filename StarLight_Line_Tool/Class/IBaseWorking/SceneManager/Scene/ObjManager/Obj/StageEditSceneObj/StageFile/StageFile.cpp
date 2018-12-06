#include "StageFile.h"

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>

#include <d3dx9.h>

#include "../../../../../../../Singleton/Singleton.h"
#include "../Enum/STAR_TYPE.h"
#include "../Struct/StarData.h"
#include "../Struct/StarIcon.h"
#include "../Struct/MeasureBPM.h"

VOID StageFile::ReadMeasureBPM(std::ifstream* pIfs, const INT& measuresNum, std::vector<MeasureBPM*>* ppMeasureBPMVec)
{
	if (pIfs->fail()) return;

	std::string lineBuf;

	if (!ppMeasureBPMVec)
	{
		for (INT mi = 0; mi < measuresNum; ++mi) std::getline(*pIfs, lineBuf);

		return;
	}

	MeasureBPM* pMeasureBPM = nullptr;

	INT bPM = NULL;

	const INT DIGITS_MAX = MeasureBPM::m_DIGITS_MAX;
	const INT DIGIT_OVER = 10;

	for (INT mi = 0; mi < measuresNum; ++mi)
	{
		pMeasureBPM = new MeasureBPM;

		std::getline(*pIfs, lineBuf);

		std::replace(
				lineBuf.begin(), 
				lineBuf.end(), 
				m_separator, ' ');
		std::istringstream issb(lineBuf);

		issb >> bPM;

		for (INT di = 0; di < DIGITS_MAX; ++di)
		{
			pMeasureBPM->m_digitBPMs[di] =
				bPM / static_cast<INT>(pow(DIGIT_OVER, DIGITS_MAX - 1 - di)) % DIGIT_OVER;
		}

		ppMeasureBPMVec->push_back(pMeasureBPM);
	}
}

VOID StageFile::ReadStarNums(std::ifstream* pIfs, StarIcon* pStarIcon)
{
	if (pIfs->fail()) return;

	std::string lineBuf;

	std::getline(*pIfs, lineBuf);
	std::replace(
			lineBuf.begin(),
			lineBuf.end(),
			m_separator, ' ');
	std::istringstream isss(lineBuf);

	isss >> pStarIcon[STAR_DAMAGE].m_num	>>
			pStarIcon[STAR_SCORE].m_num		>>
			pStarIcon[STAR_CLEAR].m_num;
}

VOID StageFile::ReadStarData(std::ifstream* pIfs, const StarIcon* pStarIcon, std::vector<StarData*>* ppStarDataVec)
{
	if (pIfs->fail()) return;

	std::string lineBuf;

	INT totalStarsNum = NULL;
	for (INT i = 0; i < STAR_MAX; ++i) totalStarsNum += pStarIcon[i].m_num;

	StarData* pStarData = nullptr;

	for (INT i = 0; i < totalStarsNum; ++i)
	{
		pStarData = new StarData;

		std::getline(*pIfs, lineBuf);
		std::replace(
			lineBuf.begin(),
			lineBuf.end(),
			m_separator, ' ');
		std::istringstream isss(lineBuf);

		isss >> pStarData->m_starType	>>
				pStarData->m_measureNum >>
				pStarData->m_beatNum	>>
				pStarData->m_noteNum	>>
				pStarData->m_divideNum	>>
				pStarData->m_x			>>
				pStarData->m_deg;

		ppStarDataVec->push_back(pStarData);
	}
}

VOID StageFile::WriteStageData(INT measuresNum, const std::vector<MeasureBPM*>& pMeasureBPMVec,
	const StarIcon* pStarIcon, const std::vector<StarData*>& pStarDataVec)
{
	std::ofstream ofs(StagePath, std::ios::trunc);

	ofs << measuresNum << m_separator << std::endl;

	INT bPM = NULL;

	const INT DIGITS_MAX = MeasureBPM::m_DIGITS_MAX;
	const INT DIGIT_OVER = 10;

	for (MeasureBPM* pI : pMeasureBPMVec)
	{
		bPM = NULL;

		for (INT i = 0; i < DIGITS_MAX; ++i)
		{
			bPM += pI->m_digitBPMs[i] * static_cast<INT>(pow(DIGIT_OVER, DIGITS_MAX - 1 - i));
		}

		ofs << bPM << m_separator << std::endl;
	}

	ofs << pStarIcon[STAR_DAMAGE].m_num <<
		m_separator						<<
		pStarIcon[STAR_SCORE].m_num		<<
		m_separator						<<
		pStarIcon[STAR_CLEAR].m_num		<<
		std::endl;

	for (StarData* pI : pStarDataVec)
	{
		ofs << pI->m_starType	<<
			m_separator			<<
			pI->m_measureNum	<<
			m_separator			<<
			pI->m_beatNum		<<
			m_separator			<<
			pI->m_noteNum		<<
			m_separator			<<
			pI->m_divideNum		<<
			m_separator			<<
			pI->m_x				<<
			m_separator			<<
			pI->m_deg			<<
			std::endl;
	}

	ofs.close();
}
