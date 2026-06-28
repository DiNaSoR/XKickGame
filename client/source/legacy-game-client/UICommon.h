#ifndef __UI_COMMON_H__
#define __UI_COMMON_H__

#include<string>

#include "ComDefine.h"

class CResSkillTableFormat;
class CUIControl;

int UpdateSkillExplan( int nIndex, int * pCodes, int nArea );
void UpdateTrainingExplan( int nIndex, int * pCodes );
void UpdateCeremonyExplan( int nIndex, int * pCodes );
void UpdateSkillDesc( int nIndex, CResSkillTableFormat * pTable, CUIControl * pParentControl );

std::string GetAliasString( PLAYER_ALIAS eAlias );
std::string GetConditionString( PLAYER_CONDITION eCondition );
std::string GetObjectKindString( OBJECT_KIND eObjectKind );

void ViewQuestDetail( int nQuestCode , bool bIsCR );

#endif
