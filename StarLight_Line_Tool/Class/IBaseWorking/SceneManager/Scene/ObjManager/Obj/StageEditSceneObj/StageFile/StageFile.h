#ifndef STAGE_FILE_H
#define STAGE_FILE_H

#include <windows.h>

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>

#include <d3dx9.h>

#include "../../../../../../../Singleton/Singleton.h"
#include "../Enum/STAR_TYPE.h"
#include "../Struct/MeasureBPM.h"
#include "../Struct/StarIcon.h"
#include "../Struct/StarData.h"

class StageFile :public Singleton<StageFile>
{
public:
	friend  Singleton<StageFile>;

	~StageFile() {};

	VOID ReadMeasureNum(INT* pMeasuresNum)
	{
		std::ifstream ifs(StagePath);

		ReadMeasureNum(&ifs, pMeasuresNum);
	}

	VOID ReadMeasureNum(std::ifstream* pIfs, INT* pMeasuresNum)
	{
		//if (!pIfs->fail()) return;

		std::string lineBuf;
		std::getline(*pIfs, lineBuf);

		std::replace(lineBuf.begin(), lineBuf.end(), ',', ' ');
		std::istringstream iss(lineBuf);

		iss >> (*pMeasuresNum);
	}

	VOID ReadMeasureBPM(std::vector<MeasureBPM*>* ppMeasureBPMVec)
	{
		std::ifstream ifs(StagePath);

		INT measuresNum = NULL;
		ReadMeasureNum(&ifs, &measuresNum);
		ReadMeasureBPM(&ifs, measuresNum, ppMeasureBPMVec);
	}

	VOID ReadMeasureBPM(std::ifstream* pIfs, const INT& measuresNum, std::vector<MeasureBPM*>* ppMeasureBPMVec);

	VOID ReadStarNums(StarIcon* pStarIcon)
	{
		std::ifstream ifs(StagePath);

		INT measuresNum = NULL;
		ReadMeasureNum(&ifs, &measuresNum);
		ReadMeasureBPM(&ifs, measuresNum, nullptr);
		ReadStarNums(&ifs, pStarIcon);
	}

	VOID ReadStarNums(std::ifstream* pIfs, StarIcon* pStarIcon);

	VOID ReadStarData(std::vector<StarData*>* ppStarDataVec)
	{
		std::ifstream ifs(StagePath);

		INT measuresNum = NULL;
		ReadMeasureNum(&ifs, &measuresNum);
		ReadMeasureBPM(&ifs, measuresNum, nullptr);

		StarIcon starIcons[STAR_MAX];
		ReadStarNums(&ifs, starIcons);
		ReadStarData(&ifs, starIcons, ppStarDataVec);
	}

	VOID ReadStarData(std::ifstream* pIfs, const StarIcon* pStarIcon, std::vector<StarData*>* ppStarDataVec);
	
	VOID WriteStageData(INT measuresNum, const std::vector<MeasureBPM*>& pMeasureBPMVec,
		const StarIcon* pStarIcon, const std::vector<StarData*>& pStarDataVec);

private:
	StageFile()
	{
		StagePath = "Texts/Stage.csv";
	}

	std::string StagePath;

	CHAR m_separator = ',';
};

#endif // !STAGE_FILE_H
