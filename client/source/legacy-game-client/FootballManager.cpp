#include "stdafx.h"
#include "FootballManager.h"
#include "FblEngine.h"
#include "ObjBallInfo.h"
#include "ObjRoomManager.h"
#include "PutPacket.h"

CFootballManager::CFootballManager()
{}

CFootballManager::~CFootballManager()
{}

void CFootballManager::UpdateFootball(float fFrameTime, float fAccumTime, int nState)
{
	CObjBallInfo*		pBall		= CObjBallManager::GetPtr()->GetBallPointer();
	CObjRoomInfo*		pRoom		= CObjRoomManager::GetPtr()->GetRoomPointer();

	switch(nState)
	{
	case PLAY_STATE_START:
		{
			if(CObjRoomManager::GetPtr()->GetRoomPointer()->IsMeParent()) //부모일 경우 축구 연산
			{
				CFblEngine::GetPtr()->SetFootballTime(fFrameTime);
				CFblEngine::GetPtr()->SetStampTime(fFrameTime);
				CFblEngine::GetPtr()->ProcessFootballEngine(fFrameTime);

				CFblEngine::GetPtr()->SetTicTime(fFrameTime);
				if(CFblEngine::GetPtr()->IsActionTicTime())
					PutObjectAction();
				if(CFblEngine::GetPtr()->IsBallTicTime())
					PutBallAction();
			}
			else //자식일 경우 궤적만 계산
			{
				CFblEngine::GetPtr()->SetFootballTime(fFrameTime);
				CFblEngine::GetPtr()->SetStampTime(fFrameTime);
			}
		}
		break;
	case PLAY_STATE_PLAY:
		{
			if(CObjRoomManager::GetPtr()->GetRoomPointer()->IsMeParent()) //부모일 경우 축구 연산
			{
				CFblEngine::GetPtr()->SetFootballTime(fFrameTime);
				CFblEngine::GetPtr()->SetPlayTime(fFrameTime);
				CFblEngine::GetPtr()->SetStampTime(fFrameTime);
				CFblEngine::GetPtr()->SetCoolTime(fFrameTime);
				CFblEngine::GetPtr()->ProcessFootballEngine(fFrameTime);
				CObjBallManager::GetPtr()->GetBallPointer()->CalBallPoint(); //공 궤적 계산
				CObjBallManager::GetPtr()->GetBallPointer()->CalBallCollision(); //공 충돌 계산

				CFblEngine::GetPtr()->SetTicTime(fFrameTime);
				if(CFblEngine::GetPtr()->IsActionTicTime())
					PutObjectAction();
				if(CFblEngine::GetPtr()->IsBallTicTime())
					PutBallAction();
			}
			else //자식일 경우 궤적만 계산
			{
				CFblEngine::GetPtr()->SetFootballTime(fFrameTime);
				CFblEngine::GetPtr()->SetPlayTime(fFrameTime);
				CFblEngine::GetPtr()->SetStampTime(fFrameTime);
				CFblEngine::GetPtr()->SetCoolTime(fFrameTime);
				//CObjPlayerManager::GetPtr()->GetMyPlayerPointer(PLAYER_TEAM_TOTAL)->SetCoolTime(fFrameTime);
				CObjBallManager::GetPtr()->GetBallPointer()->CalBallPoint(); //공 궤적 계산
				CObjBallManager::GetPtr()->GetBallPointer()->CalBallCollisionChild(); //공 충돌 계산
			}
		}
		break;
	case PLAY_STATE_GOAL:
		{
			if(CObjRoomManager::GetPtr()->GetRoomPointer()->IsMeParent()) //부모일 경우 축구 연산
			{
				CFblEngine::GetPtr()->SetFootballTime(fFrameTime);
				CFblEngine::GetPtr()->SetStampTime(fFrameTime);
				CFblEngine::GetPtr()->ProcessFootballEngine(fFrameTime);
				CObjBallManager::GetPtr()->GetBallPointer()->CalBallPoint(); //공 궤적 계산
				CObjBallManager::GetPtr()->GetBallPointer()->CalBallCollision(); //공 충돌 계산

				CFblEngine::GetPtr()->SetTicTime(fFrameTime);
				if(CFblEngine::GetPtr()->IsActionTicTime())
					PutObjectAction();
				if(CFblEngine::GetPtr()->IsBallTicTime())
					PutBallAction();
			}
			else //자식일 경우 궤적만 계산
			{
				CFblEngine::GetPtr()->SetFootballTime(fFrameTime);
				CFblEngine::GetPtr()->SetStampTime(fFrameTime);
				CObjBallManager::GetPtr()->GetBallPointer()->CalBallPoint(); //공 궤적 계산
				CObjBallManager::GetPtr()->GetBallPointer()->CalBallCollisionChild(); //공 충돌 계산
			}
		}
		break;
	case PLAY_STATE_REPLAY:
		{
			CFblEngine::GetPtr()->SetFootballTime(fFrameTime);
			CFblEngine::GetPtr()->SetStampTime(fFrameTime);
			CObjBallManager::GetPtr()->GetBallPointer()->CalBallPoint(); //공 궤적 계산
			CObjBallManager::GetPtr()->GetBallPointer()->CalBallCollisionChild(); //공 충돌 계산
		}
		break;
	case PLAY_STATE_CEREMONY:
		{
		}
		break;
	case PLAY_STATE_TUTORIAL:
		{
		}
		break;
	}
}
