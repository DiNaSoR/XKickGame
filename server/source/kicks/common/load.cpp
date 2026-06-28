
#include "Load.h"

CAPLoad::CAPLoad()
{}

CAPLoad::~CAPLoad()
{}

void CAPLoad::LoadLevelTable(void)
{
	tvar_t *field;
	TAPLevelRec *pRec;

	int loadfd = Table_FindTable("Table_Level_File.csv");

	int i = 0;
	while(1)
	{
		field = Table_GetData(loadfd, i, "Level");
		if(field == NULL)
			break;

		//Record를 저장할 메모리를 동적으로 생성
		pRec = (TAPLevelRec *)malloc(sizeof(TAPLevelRec));
		if(pRec == NULL)
		{
			printf("Position Table Memory Creation error\n");
			exit(1);
		}

		//Record에 데이터 삽입
		pRec->Level = atoi(field->string);
		field = Table_GetData(loadfd, i, "TargetEXP");				pRec->TargetExp = FieldToValue(field);
		field = Table_GetData(loadfd, i, "NeedEXP");				pRec->NeedExp = FieldToValue(field);
		field = Table_GetData(loadfd, i, "Zone");					pRec->Zone = FieldToValue(field);

		//Map에 등록
		m_LevelTable[pRec->Level] = pRec;
		++i;
	}
}

void CAPLoad::LoadItemTable(void)
{
	tvar_t *field;
	TAPItemRec *pRec;

	int loadfd = Table_FindTable("Table_Item_File.csv");

	int i = 0;
	while(1)
	{
		field = Table_GetData(loadfd, i, "ItemCode");
		if(field == NULL)
			break;

		//Record를 저장할 메모리를 동적으로 생성
		pRec = (TAPItemRec *)malloc(sizeof(TAPItemRec));
		if(pRec == NULL)
		{
			printf("Item Table Memory Creation error\n");
			exit(1);
		}

		//Record에 데이터 삽입
		pRec->ItemCode = atoi(field->string);
		field = Table_GetData(loadfd, i, "ItemType");			pRec->ItemType = FieldToValue(field);
		field = Table_GetData(loadfd, i, "Company");			pRec->Company = FieldToValue(field);
		field = Table_GetData(loadfd, i, "Gender");				pRec->Gender = FieldToValue(field);
		field = Table_GetData(loadfd, i, "Limit");				pRec->Limit = FieldToValue(field);
		field = Table_GetData(loadfd, i, "Divest");				pRec->Divest = FieldToValue(field);
		field = Table_GetData(loadfd, i, "Add");				pRec->Add = FieldToValue(field);
		field = Table_GetData(loadfd, i, "New");				pRec->New = FieldToValue(field);
		field = Table_GetData(loadfd, i, "Sell");				pRec->Sell = FieldToValue(field);
		field = Table_GetData(loadfd, i, "Exchange");			pRec->Exchange = FieldToValue(field);
		field = Table_GetData(loadfd, i, "Default");			pRec->Default = FieldToValue(field);

		field = Table_GetData(loadfd, i, "Cash0");				pRec->Cash[0] = FieldToValue(field);
		field = Table_GetData(loadfd, i, "Cash1");				pRec->Cash[1] = FieldToValue(field);
		field = Table_GetData(loadfd, i, "Cash2");				pRec->Cash[2] = FieldToValue(field);
		field = Table_GetData(loadfd, i, "Cash3");				pRec->Cash[3] = FieldToValue(field);
		field = Table_GetData(loadfd, i, "Cash4");				pRec->Cash[4] = FieldToValue(field);

		field = Table_GetData(loadfd, i, "Point0");				pRec->Point[0] = FieldToValue(field);
		field = Table_GetData(loadfd, i, "Point1");				pRec->Point[1] = FieldToValue(field);
		field = Table_GetData(loadfd, i, "Point2");				pRec->Point[2] = FieldToValue(field);
		field = Table_GetData(loadfd, i, "Point3");				pRec->Point[3] = FieldToValue(field);
		field = Table_GetData(loadfd, i, "Point4");				pRec->Point[4] = FieldToValue(field);

		field = Table_GetData(loadfd, i, "Pack");				pRec->Pack = FieldToValue(field);

		//Map에 등록
		m_ItemTable[pRec->ItemCode] = pRec;
		++i;
		//printf("ItemCode(%d), Pack(%d)\n", pRec->ItemCode, pRec->Pack);
	}
}

void CAPLoad::LoadPackTable(void)
{
	tvar_t *field;
	TAPPackRec *pRec;

	int loadfd = Table_FindTable("Table_Pack_File.csv");

	int i = 0;
	while(1)
	{
		field = Table_GetData(loadfd, i, "PackCode");
		if(field == NULL)
			break;

		//Record를 저장할 메모리를 동적으로 생성
		pRec = (TAPPackRec *)malloc(sizeof(TAPPackRec));
		if(pRec == NULL)
		{
			printf("Pack Table Memory Creation error\n");
			exit(1);
		}

		//Record에 데이터 삽입
		pRec->PackCode = atoi(field->string);
		field = Table_GetData(loadfd, i, "Limit1");				pRec->PackOption[0].Limit = FieldToValue(field);
		field = Table_GetData(loadfd, i, "SOption1");			pRec->PackOption[0].SOptionCode = FieldToValue(field);
		field = Table_GetData(loadfd, i, "EOption1");			pRec->PackOption[0].EOptionCode = FieldToValue(field);
		field = Table_GetData(loadfd, i, "Default1");			pRec->PackOption[0].Default = FieldToValue(field);

		field = Table_GetData(loadfd, i, "Limit2");				pRec->PackOption[1].Limit = FieldToValue(field);
		field = Table_GetData(loadfd, i, "SOption2");			pRec->PackOption[1].SOptionCode = FieldToValue(field);
		field = Table_GetData(loadfd, i, "EOption2");			pRec->PackOption[1].EOptionCode = FieldToValue(field);
		field = Table_GetData(loadfd, i, "Default2");			pRec->PackOption[1].Default = FieldToValue(field);

		field = Table_GetData(loadfd, i, "Limit3");				pRec->PackOption[2].Limit = FieldToValue(field);
		field = Table_GetData(loadfd, i, "SOption3");			pRec->PackOption[2].SOptionCode = FieldToValue(field);
		field = Table_GetData(loadfd, i, "EOption3");			pRec->PackOption[2].EOptionCode = FieldToValue(field);
		field = Table_GetData(loadfd, i, "Default3");			pRec->PackOption[2].Default = FieldToValue(field);

		//Map에 등록
		m_PackTable[pRec->PackCode] = pRec;
		++i;
	}
}

void CAPLoad::LoadOptionTable(void)
{
	tvar_t *field;
	TAPOptionRec *pRec;

	int loadfd = Table_FindTable("Table_Option_File.csv");

	int i = 0;
	while(1)
	{
		field = Table_GetData(loadfd, i, "OptionCode");
		if(field == NULL)
			break;

		//Record를 저장할 메모리를 동적으로 생성
		pRec = (TAPOptionRec *)malloc(sizeof(TAPOptionRec));
		if(pRec == NULL)
		{
			printf("Option Table Memory Creation error\n");
			exit(1);
		}

		//Record에 데이터 삽입
		pRec->OptionCode = atoi(field->string);
		field = Table_GetData(loadfd, i, "Type");				pRec->Type = FieldToValue(field);
		field = Table_GetData(loadfd, i, "Gap");				pRec->Gap = FieldToValue(field);
		field = Table_GetData(loadfd, i, "Value");				pRec->Value = FieldToValue(field);
		field = Table_GetData(loadfd, i, "CLimit");				pRec->CLimit = FieldToValue(field);
		field = Table_GetData(loadfd, i, "PLimit");				pRec->PLimit = FieldToValue(field);

		field = Table_GetData(loadfd, i, "Cash0");				pRec->Cash[0] = FieldToValue(field);
		field = Table_GetData(loadfd, i, "Cash1");				pRec->Cash[1] = FieldToValue(field);
		field = Table_GetData(loadfd, i, "Cash2");				pRec->Cash[2] = FieldToValue(field);
		field = Table_GetData(loadfd, i, "Cash3");				pRec->Cash[3] = FieldToValue(field);
		field = Table_GetData(loadfd, i, "Cash4");				pRec->Cash[4] = FieldToValue(field);

		field = Table_GetData(loadfd, i, "Point0");				pRec->Point[0] = FieldToValue(field);
		field = Table_GetData(loadfd, i, "Point1");				pRec->Point[1] = FieldToValue(field);
		field = Table_GetData(loadfd, i, "Point2");				pRec->Point[2] = FieldToValue(field);
		field = Table_GetData(loadfd, i, "Point3");				pRec->Point[3] = FieldToValue(field);
		field = Table_GetData(loadfd, i, "Point4");				pRec->Point[4] = FieldToValue(field);

		//Map에 등록
		m_OptionTable[pRec->OptionCode] = pRec;
		++i;
	}
}


void CAPLoad::LoadQuestTable()
{
	tvar_t*			field;

	int nLoadFd = Table_FindTable("Table_Quest.csv");

	int i = 0;
	while(1)
	{
		field = Table_GetData(nLoadFd, i, "Code");
		if(field == NULL)
			break;

		//Record를 저장할 메모리를 동적으로 생성
		CQuestTable* pRec = new CQuestTable;
		if(pRec == NULL)
		{
			printf("Quest Table Memory Creation error\n");
			exit(1);
		}

		//Record에 데이터 삽입
		pRec->m_nCode = ATOI(field->string);
		field = Table_GetData(nLoadFd, i, "Position");			pRec->m_nPosition	= FieldToValue(field);
		field = Table_GetData(nLoadFd, i, "Level");				pRec->m_nLevel		= FieldToValue(field);
		field = Table_GetData(nLoadFd, i, "Diff");				pRec->m_nDiff		= FieldToValue(field);
		field = Table_GetData(nLoadFd, i, "Type");				pRec->m_nMainType	= FieldToValue(field);
		field = Table_GetData(nLoadFd, i, "Type1");				pRec->m_nType[0]	= FieldToValue(field);
		field = Table_GetData(nLoadFd, i, "Type2");				pRec->m_nType[1]	= FieldToValue(field);
		field = Table_GetData(nLoadFd, i, "Type3");				pRec->m_nType[2]	= FieldToValue(field);
		field = Table_GetData(nLoadFd, i, "Limit");				pRec->m_nLimit		= FieldToValue(field);
		field = Table_GetData(nLoadFd, i, "Join");				pRec->m_nJoin		= FieldToValue(field);
		field = Table_GetData(nLoadFd, i, "Repeat");			pRec->m_nRepeat		= FieldToValue(field);
		field = Table_GetData(nLoadFd, i, "Condition");			pRec->m_nCondition	= FieldToValue(field);

		field = Table_GetData(nLoadFd, i, "Kind1");				pRec->m_cGift[0].m_nKind	= FieldToValue(field);
		field = Table_GetData(nLoadFd, i, "Gift1");				pRec->m_cGift[0].m_nCode	= FieldToValue(field);
		field = Table_GetData(nLoadFd, i, "Amount1");			pRec->m_cGift[0].m_nAmount	= FieldToValue(field);
		field = Table_GetData(nLoadFd, i, "Kind2");				pRec->m_cGift[1].m_nKind	= FieldToValue(field);
		field = Table_GetData(nLoadFd, i, "Gift2");				pRec->m_cGift[1].m_nCode	= FieldToValue(field);
		field = Table_GetData(nLoadFd, i, "Amount2");			pRec->m_cGift[1].m_nAmount	= FieldToValue(field);
		field = Table_GetData(nLoadFd, i, "Kind3");				pRec->m_cGift[2].m_nKind	= FieldToValue(field);
		field = Table_GetData(nLoadFd, i, "Gift3");				pRec->m_cGift[2].m_nCode	= FieldToValue(field);
		field = Table_GetData(nLoadFd, i, "Amount3");			pRec->m_cGift[2].m_nAmount	= FieldToValue(field);

		field = Table_GetData(nLoadFd, i, "Title");				FieldToString(pRec->m_sTitle, field, TITLE_NAME_SIZE );

		//Map에 등록

		m_cQuestTable[pRec->m_nCode] = pRec;
		++i;
	}
}



void CAPLoad::LoadPositionTable(void)
{
	tvar_t *field;
	TAPPositionRec *pRec;

	int loadfd = Table_FindTable("Table_Position_File.csv");

	int i = 0;
	while(1)
	{
		field = Table_GetData(loadfd, i, "Code");
		if(field == NULL)
		{
			printf( "FIELD LOADING ERROR!!\n");
			break;
		}

		//Record를 저장할 메모리를 동적으로 생성
		pRec = (TAPPositionRec *)malloc(sizeof(TAPPositionRec));
		if(pRec == NULL)
		{
			printf("Position Table Memory Creation error\n");
			exit(1);
		}

		//Record에 데이터 삽입
		pRec->PositionCode = atoi(field->string);
		field = Table_GetData(loadfd, i, "Run");				pRec->Run = FieldToValue(field);
		field = Table_GetData(loadfd, i, "Endurance");			pRec->Endurance = FieldToValue(field);
		field = Table_GetData(loadfd, i, "Quick");				pRec->Quick = FieldToValue(field);
		field = Table_GetData(loadfd, i, "Keeping");			pRec->Keeping = FieldToValue(field);
		field = Table_GetData(loadfd, i, "Dribble");			pRec->Dribble = FieldToValue(field);
		field = Table_GetData(loadfd, i, "Steal");				pRec->Steal = FieldToValue(field);
		field = Table_GetData(loadfd, i, "Tackle");				pRec->Tackle = FieldToValue(field);
		field = Table_GetData(loadfd, i, "Heading");			pRec->Heading = FieldToValue(field);
		field = Table_GetData(loadfd, i, "ShortShoot");			pRec->ShortShoot = FieldToValue(field);
		field = Table_GetData(loadfd, i, "LongShoot");			pRec->LongShoot = FieldToValue(field);
		field = Table_GetData(loadfd, i, "Cross");				pRec->Cross = FieldToValue(field);
		field = Table_GetData(loadfd, i, "ShortPass");			pRec->ShortPass = FieldToValue(field);
		field = Table_GetData(loadfd, i, "LongPass");			pRec->LongPass = FieldToValue(field);
		field = Table_GetData(loadfd, i, "Bodycheck");			pRec->Bodycheck = FieldToValue(field);
		field = Table_GetData(loadfd, i, "Catch");				pRec->Catch = FieldToValue(field);
		field = Table_GetData(loadfd, i, "Punch");				pRec->Punch = FieldToValue(field);
		field = Table_GetData(loadfd, i, "Defense");			pRec->Defense = FieldToValue(field);
		field = Table_GetData(loadfd, i, "Auto0");				pRec->Auto[0] = FieldToValue(field);
		field = Table_GetData(loadfd, i, "Auto1");				pRec->Auto[1] = FieldToValue(field);
		field = Table_GetData(loadfd, i, "Auto2");				pRec->Auto[2] = FieldToValue(field);
		field = Table_GetData(loadfd, i, "Auto3");				pRec->Auto[3] = FieldToValue(field);
		field = Table_GetData(loadfd, i, "Plus0");				pRec->Plus[0] = FieldToValue(field);
		field = Table_GetData(loadfd, i, "Plus1");				pRec->Plus[1] = FieldToValue(field);
		field = Table_GetData(loadfd, i, "Plus2");				pRec->Plus[2] = FieldToValue(field);
		field = Table_GetData(loadfd, i, "Minus0");				pRec->Minus[0] = FieldToValue(field);
		field = Table_GetData(loadfd, i, "Minus1");				pRec->Minus[1] = FieldToValue(field);
		field = Table_GetData(loadfd, i, "Minus2");				pRec->Minus[2] = FieldToValue(field);

		//Map에 등록
		m_PositionTable[pRec->PositionCode] = pRec;
		++i;
	}
}

void CAPLoad::LoadLearnTable(void)
{
	tvar_t *field;
	TAPLearnRec *pRec;

	int loadfd = Table_FindTable("Table_Learn_File.csv");

	int i = 0;
	while(1)
	{
		field = Table_GetData(loadfd, i, "LearnCode");
		if(field == NULL)
			break;

		//Record를 저장할 메모리를 동적으로 생성
		pRec = (TAPLearnRec *)malloc(sizeof(TAPLearnRec));
		if(pRec == NULL)
		{
			printf("Learn Table Memory Creation error\n");
			exit(1);
		}

		//Record에 데이터 삽입
		pRec->LearnCode = atoi(field->string);
		field = Table_GetData(loadfd, i, "Type");				pRec->Type = FieldToValue(field);
		field = Table_GetData(loadfd, i, "Limit");				pRec->Limit = FieldToValue(field);
		field = Table_GetData(loadfd, i, "Value");				pRec->Value = FieldToValue(field);
		field = Table_GetData(loadfd, i, "Sell");				pRec->Sell = FieldToValue(field);
		field = Table_GetData(loadfd, i, "Default");			pRec->Default = FieldToValue(field);

		field = Table_GetData(loadfd, i, "Cash0");				pRec->Cash[0] = FieldToValue(field);
		field = Table_GetData(loadfd, i, "Cash1");				pRec->Cash[1] = FieldToValue(field);
		field = Table_GetData(loadfd, i, "Cash2");				pRec->Cash[2] = FieldToValue(field);
		field = Table_GetData(loadfd, i, "Cash3");				pRec->Cash[3] = FieldToValue(field);
		field = Table_GetData(loadfd, i, "Cash4");				pRec->Cash[4] = FieldToValue(field);

		field = Table_GetData(loadfd, i, "Point0");				pRec->Point[0] = FieldToValue(field);
		field = Table_GetData(loadfd, i, "Point1");				pRec->Point[1] = FieldToValue(field);
		field = Table_GetData(loadfd, i, "Point2");				pRec->Point[2] = FieldToValue(field);
		field = Table_GetData(loadfd, i, "Point3");				pRec->Point[3] = FieldToValue(field);
		field = Table_GetData(loadfd, i, "Point4");				pRec->Point[4] = FieldToValue(field);

		field = Table_GetData(loadfd, i, "Pack");				pRec->Pack = FieldToValue(field);

		//Map에 등록
		m_LearnTable[pRec->LearnCode] = pRec;
		++i;
	}
}

void CAPLoad::LoadSkillTable(void)
{
	tvar_t *field;
	TAPSkillRec *pRec;

	int loadfd = Table_FindTable("Table_Skill_File.csv");

	//int price = 0;
	int i = 0;
	while(1)
	{
		field = Table_GetData(loadfd, i, "SkillCode");
		if(field == NULL)
			break;

		//Record를 저장할 메모리를 동적으로 생성
		pRec = (TAPSkillRec *)malloc(sizeof(TAPSkillRec));
		if(pRec == NULL)
		{
			printf("Skill Table Memory Creation error\n");
			exit(1);
		}

		//Record에 데이터 삽입
		pRec->SkillCode = atoi(field->string);
		field = Table_GetData(loadfd, i, "Position");			pRec->Position = FieldToValue(field);
		field = Table_GetData(loadfd, i, "Section");			pRec->Section = FieldToValue(field);
		field = Table_GetData(loadfd, i, "Limit");				pRec->Limit = FieldToValue(field);
		field = Table_GetData(loadfd, i, "Sell");				pRec->Sell = FieldToValue(field);
		field = Table_GetData(loadfd, i, "Default");			pRec->Default = FieldToValue(field);

		field = Table_GetData(loadfd, i, "Cash0");				pRec->Cash[0] = FieldToValue(field);
		field = Table_GetData(loadfd, i, "Cash1");				pRec->Cash[1] = FieldToValue(field);
		field = Table_GetData(loadfd, i, "Cash2");				pRec->Cash[2] = FieldToValue(field);
		field = Table_GetData(loadfd, i, "Cash3");				pRec->Cash[3] = FieldToValue(field);
		field = Table_GetData(loadfd, i, "Cash4");				pRec->Cash[4] = FieldToValue(field);

		field = Table_GetData(loadfd, i, "Point0");				pRec->Point[0] = FieldToValue(field);
		field = Table_GetData(loadfd, i, "Point1");				pRec->Point[1] = FieldToValue(field);
		field = Table_GetData(loadfd, i, "Point2");				pRec->Point[2] = FieldToValue(field);
		field = Table_GetData(loadfd, i, "Point3");				pRec->Point[3] = FieldToValue(field);
		field = Table_GetData(loadfd, i, "Point4");				pRec->Point[4] = FieldToValue(field);

		field = Table_GetData(loadfd, i, "Pack");				pRec->Pack = FieldToValue(field);

		/*
		if(pRec->Position == 0)
			price += pRec->Point;
		else if(pRec->Position == 23)
			price += pRec->Point;
		else if(pRec->Position == 20)
			price += pRec->Point;
			*/

		//Map에 등록
		m_SkillTable[pRec->SkillCode] = pRec;
		++i;
	}
	//printf("price(%d)\n", price);
}

void CAPLoad::LoadCeleTable(void)
{
	tvar_t *field;
	TAPCeleRec *pRec;

	int loadfd = Table_FindTable("Table_Cele_File.csv");

	int i = 0;
	while(1)
	{
		field = Table_GetData(loadfd, i, "CeleCode");
		if(field == NULL)
			break;

		//Record를 저장할 메모리를 동적으로 생성
		pRec = (TAPCeleRec *)malloc(sizeof(TAPCeleRec));
		if(pRec == NULL)
		{
			printf("Cele Table Memory Creation error\n");
			exit(1);
		}

		//Record에 데이터 삽입
		pRec->CeleCode = atoi(field->string);
		field = Table_GetData(loadfd, i, "Limit");				pRec->Limit = FieldToValue(field);
		field = Table_GetData(loadfd, i, "Sell");				pRec->Sell = FieldToValue(field);
		field = Table_GetData(loadfd, i, "Default");			pRec->Default = FieldToValue(field);

		field = Table_GetData(loadfd, i, "Cash0");				pRec->Cash[0] = FieldToValue(field);
		field = Table_GetData(loadfd, i, "Cash1");				pRec->Cash[1] = FieldToValue(field);
		field = Table_GetData(loadfd, i, "Cash2");				pRec->Cash[2] = FieldToValue(field);
		field = Table_GetData(loadfd, i, "Cash3");				pRec->Cash[3] = FieldToValue(field);
		field = Table_GetData(loadfd, i, "Cash4");				pRec->Cash[4] = FieldToValue(field);

		field = Table_GetData(loadfd, i, "Point0");				pRec->Point[0] = FieldToValue(field);
		field = Table_GetData(loadfd, i, "Point1");				pRec->Point[1] = FieldToValue(field);
		field = Table_GetData(loadfd, i, "Point2");				pRec->Point[2] = FieldToValue(field);
		field = Table_GetData(loadfd, i, "Point3");				pRec->Point[3] = FieldToValue(field);
		field = Table_GetData(loadfd, i, "Point4");				pRec->Point[4] = FieldToValue(field);

		field = Table_GetData(loadfd, i, "Pack");				pRec->Pack = FieldToValue(field);

		//Map에 등록
		m_CeleTable[pRec->CeleCode] = pRec;
		++i;
	}
}

void CAPLoad::LoadMissionTable(void)
{
	tvar_t *field;
	TAPMissionRec *pRec;

	int loadfd = Table_FindTable("Table_Mission_File.csv");

	int i = 0;
	while(1)
	{
		field = Table_GetData(loadfd, i, "MissionCode");
		if(field == NULL)
			break;

		//Record를 저장할 메모리를 동적으로 생성
		pRec = (TAPMissionRec *)malloc(sizeof(TAPMissionRec));
		if(pRec == NULL)
		{
			printf("Mission Table Memory Creation error\n");
			exit(1);
		}

		//Record에 데이터 삽입
		pRec->MissionCode = atoi(field->string);
		pRec->Index = i;
		field = Table_GetData(loadfd, i, "Gift");				pRec->Gift = FieldToValue(field);

		//Map에 등록
		m_MissionTable[pRec->MissionCode] = pRec;
		m_MissionIndex[i] = pRec;
		++i;
	}
}

TAPLevelRec *CAPLoad::GetLevelTable(int code)
{
	TAPLevelRec *pRec = NULL;
	pRec = m_LevelTable[code];
	if(pRec == NULL)
		m_LevelTable.erase(m_LevelTable.find(code));
	return pRec;
}

TAPItemRec *CAPLoad::GetItemTable(int code)
{
	TAPItemRec *pRec = NULL;
	pRec = m_ItemTable[code];
	if(pRec == NULL)
		m_ItemTable.erase(m_ItemTable.find(code));
	return pRec;
}

TAPPackRec *CAPLoad::GetPackTable(int code)
{
	TAPPackRec *pRec = NULL;
	pRec = m_PackTable[code];
	if(pRec == NULL)
		m_PackTable.erase(m_PackTable.find(code));
	return pRec;
}

TAPOptionRec *CAPLoad::GetOptionTable(int code)
{
	TAPOptionRec *pRec = NULL;
	pRec = m_OptionTable[code];
	if(pRec == NULL)
		m_OptionTable.erase(m_OptionTable.find(code));
	return pRec;
}

TAPPositionRec *CAPLoad::GetPositionTable(int code)
{
	TAPPositionRec *pRec = NULL;
	pRec = m_PositionTable[code];
	if(pRec == NULL)
		m_PositionTable.erase(m_PositionTable.find(code));
	return pRec;
}

TAPLearnRec *CAPLoad::GetLearnTable(int code)
{
	TAPLearnRec *pRec = NULL;
	pRec = m_LearnTable[code];
	if(pRec == NULL)
		m_LearnTable.erase(m_LearnTable.find(code));
	return pRec;
}

TAPSkillRec *CAPLoad::GetSkillTable(int code)
{
	TAPSkillRec *pRec = NULL;
	pRec = m_SkillTable[code];
	if(pRec == NULL)
		m_SkillTable.erase(m_SkillTable.find(code));
	return pRec;
}

TAPCeleRec *CAPLoad::GetCeleTable(int code)
{
	TAPCeleRec *pRec = NULL;
	pRec = m_CeleTable[code];
	if(pRec == NULL)
		m_CeleTable.erase(m_CeleTable.find(code));
	return pRec;
}

TAPMissionRec *CAPLoad::GetMissionTable(int code)
{
	TAPMissionRec *pRec = NULL;
	pRec = m_MissionTable[code];
	if(pRec == NULL)
		m_MissionTable.erase(m_MissionTable.find(code));
	return pRec;
}

TAPMissionRec *CAPLoad::GetMissionIndex(int code)
{
	TAPMissionRec *pRec = NULL;
	pRec = m_MissionIndex[code];
	if(pRec == NULL)
		m_MissionIndex.erase(m_MissionIndex.find(code));
	return pRec;
}

/*
TAPCeleRec *CAPLoad::GetCeleTable(int code)
{
	TAPCeleRec *pRec = NULL;
	pRec = m_CeleTable[code];
	if(pRec == NULL)
		m_CeleTable.erase(m_CeleTable.find(code));
	return pRec;
}
*/

/*
void CAPLoad::GetTip(CPlayer *pPly, char tip[])
{
	int index = pPly->m_TipIndex;

	if(m_Tip[0].Tip[0] == '-') //팁 없음
	{
		memcpy(tip, 0x00, TIP_SIZE);
		return;
	}

	memset(tip, 0x00, TIP_SIZE);
	while(1)
	{
		if(index >= MAX_TIP)
			index = 0;

		if(m_Tip[index].Tip[0] == '-')
		{
			++index;
			continue;
		}
		if(m_Tip[index].Kind < 0)
		{
			++index;
			continue;
		}
		if(m_Tip[index].Kind <= pPly->m_Level.Level)
		{}
		else
		{
			++index;
			continue;
		}

		memcpy(tip, m_Tip[index].Tip, TIP_SIZE);
		++index;
		pPly->m_TipIndex = index;
		return;
	}
}
*/
