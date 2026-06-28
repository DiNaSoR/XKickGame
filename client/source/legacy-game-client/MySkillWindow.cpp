#include "stdafx.h"
#include "MySkillWindow.h"
#include "UICommonMethod.h"
#include "../InterfaceLibrary/UIComboBox.h"
#include "PlayModeManager.h"
#include "UIFunction.h"

MySkillWindow::MySkillWindow()
{
    m_nEquipCount  = 0;
    m_nTotalCount  = 0;
    m_nCurrentPage = 1;
    m_nTotalPage   = 1;

    m_nCurrentMode = eMode::eSkillMode;
}

MySkillWindow::~MySkillWindow()
{
}

void MySkillWindow::SetMode( char nMode )
{
    m_nCurrentMode = nMode;
	m_nCurrentPage = 1;
}

void MySkillWindow::UpdateSkillInfo()
{
	char sText[STRLEN_64]="";

    CObjPlayerInfo* pPlayer = dynamic_cast<CObjPlayerInfo*>(CObjectManager::GetPtr()->FindObject(OBJECT_SELECTCHARACTER));
    assert( pPlayer );
    if( pPlayer == 0 ) return;

    CUIControl * pControl = CClientUIManager::GetPtr()->FindEventControl( "Detail_01" );
    assert( pControl );
    
    CResMessageTableFormat* pMTable = dynamic_cast<CResMessageTableFormat *>(CResourceManager::GetPtr()->FindResource(RESOURCE_MSGTABLE, 900065 ,0));
	assert(pMTable);

	UIFunction::Set_ControlText( "HaveCount", pMTable->m_sMessage );

    m_nTotalCount = (int) pPlayer->m_vSkillList.size();    
    for( int i=0; i<LIST20_SIZE; i++ )
    {
        int nIndex = (m_nCurrentPage - 1) * 20 + i;

        char sText[STRLEN_64]="";
        _snprintf( sText, STRLEN_64, "ListBG%d", i );
        CUIControl * pButton = pControl->FindEventControl( sText );
		assert(pButton);

        pButton->SetVisible( N3UI_VISIBLE_TRUE );

        if( m_nTotalCount - 1 < nIndex )
        {
            pButton->SetVisible( N3UI_VISIBLE_FALSE );
            continue;
        }

        CObjSkillInfo * pSkillInfo = pPlayer->m_vSkillList[ nIndex ];
        assert( pSkillInfo );

        CResSkillTableFormat * pTable = dynamic_cast<CResSkillTableFormat*>(CResourceManager::GetPtr()->FindResource(RESOURCE_SKILLTABLE, pSkillInfo->m_nCode, 0));
		assert( pTable );

		CUIControl * pImg = pButton->FindEventControl( "ListImg" );
		assert(pImg);
		_snprintf( sText, STRLEN_64, "Icon/Skill/%s.png", pTable->m_sIconFile );
		CClientUIManager::GetPtr()->TextureChange(N3UI_STATE_NORMAL, pImg, sText );

        if( pSkillInfo->m_nEquipKind == 1 )          
			UIFunction::Set_UIVisibility( pButton, "UseListImg", N3UI_VISIBLE_TRUE );
        else
			UIFunction::Set_UIVisibility( pButton, "UseListImg", N3UI_VISIBLE_FALSE );
    }

    m_nEquipCount = 0;
    for( int i=0; i<m_nTotalCount; i++ )
    {
        CObjSkillInfo * pSkillInfo = pPlayer->m_vSkillList[ i ];
        if( pSkillInfo->m_nEquipKind == 1 ) m_nEquipCount++;
    }

    _snprintf( sText, STRLEN_64, "%d / %d", m_nEquipCount, m_nTotalCount );
    UIFunction::Set_ControlText( "EquipSkillNum", sText );

    m_nTotalPage = m_nTotalCount / LIST20_SIZE;
    if( m_nTotalCount % LIST20_SIZE != 0 ) m_nTotalPage++;

    _snprintf( sText, STRLEN_64, "%d / %d", m_nCurrentPage, m_nTotalPage);
	UIFunction::Set_ControlText( "Detail_Page", sText );

	UIFunction::Set_UIVisibility( "HaveCount", N3UI_VISIBLE_TRUE );
	UIFunction::Set_UIVisibility( "HaveCountBox", N3UI_VISIBLE_TRUE );
	UIFunction::Set_UIVisibility( "AllDivest", N3UI_VISIBLE_TRUE );
	UIFunction::Set_UIVisibility( "AllDivestBtn", N3UI_VISIBLE_TRUE );
}

void MySkillWindow::UpdateTrainingInfo()
{
	char sText[STRLEN_64]="";

    CObjPlayerInfo* pPlayer = dynamic_cast<CObjPlayerInfo*>(CObjectManager::GetPtr()->FindObject(OBJECT_SELECTCHARACTER));
    assert( pPlayer );
    if( pPlayer == 0 ) return;

    CUIControl * pControl = CClientUIManager::GetPtr()->FindEventControl( "Detail_01" );
    assert( pControl );
    
    CResMessageTableFormat* pMTable = dynamic_cast<CResMessageTableFormat *>(CResourceManager::GetPtr()->FindResource(RESOURCE_MSGTABLE, 900066 ,0));
	assert(pMTable);

	UIFunction::Set_ControlText( "HaveCount", pMTable->m_sMessage );

    m_nTotalCount = (int) pPlayer->m_vTrainingList.size();    
    for( int i=0; i<LIST20_SIZE; i++ )
    {
        int nIndex = (m_nCurrentPage - 1) * 20 + i;

        char sText[STRLEN_64]="";
        _snprintf( sText, STRLEN_64, "ListBG%d", i );
        CUIControl * pButton = pControl->FindEventControl( sText );
		assert(pButton);

        pButton->SetVisible( N3UI_VISIBLE_TRUE );

        if( m_nTotalCount - 1 < nIndex )
        {
            pButton->SetVisible( N3UI_VISIBLE_FALSE );
            continue;
        }

        CObjTrainingInfo * pTrainingInfo = pPlayer->m_vTrainingList[ nIndex ];
        assert( pTrainingInfo );

        CResTrainingTableFormat * pTable = dynamic_cast<CResTrainingTableFormat*>(CResourceManager::GetPtr()->FindResource(RESOURCE_TRAININGTABLE, pTrainingInfo->m_nCode, 0));
		assert( pTable );

		CUIControl * pImg = pButton->FindEventControl( "ListImg" );
		assert(pImg);
		_snprintf( sText, STRLEN_64, "Icon/Training/%s.png", pTable->m_sIconFile );
		CClientUIManager::GetPtr()->TextureChange(N3UI_STATE_NORMAL, pImg, sText );

		UIFunction::Set_UIVisibility( pButton, "UseListImg", N3UI_VISIBLE_FALSE );
    }

    m_nTotalPage = m_nTotalCount / LIST20_SIZE;
    if( m_nTotalCount % LIST20_SIZE != 0 ) m_nTotalPage++;

    _snprintf( sText, STRLEN_64, "%d / %d", m_nCurrentPage, m_nTotalPage);
	UIFunction::Set_ControlText( "Detail_Page", sText );

	UIFunction::Set_UIVisibility( "HaveCount", N3UI_VISIBLE_FALSE );
	UIFunction::Set_UIVisibility( "HaveCountBox", N3UI_VISIBLE_FALSE );
	UIFunction::Set_UIVisibility( "AllDivest", N3UI_VISIBLE_FALSE );
	UIFunction::Set_UIVisibility( "AllDivestBtn", N3UI_VISIBLE_FALSE );
}

void MySkillWindow::UpdateCeremonyInfo()
{
	char sText[STRLEN_64]="";

    CObjPlayerInfo* pPlayer = dynamic_cast<CObjPlayerInfo*>(CObjectManager::GetPtr()->FindObject(OBJECT_SELECTCHARACTER));
    assert( pPlayer );
    if( pPlayer == 0 ) return;

    CUIControl * pControl = CClientUIManager::GetPtr()->FindEventControl( "Detail_01" );
    assert( pControl );
    
    CResMessageTableFormat* pMTable = dynamic_cast<CResMessageTableFormat *>(CResourceManager::GetPtr()->FindResource(RESOURCE_MSGTABLE, 900067 ,0));
	assert(pMTable);

	UIFunction::Set_ControlText( "HaveCount", pMTable->m_sMessage );

    m_nTotalCount = (int) pPlayer->m_vCeremonyList.size();    
    for( int i=0; i<LIST20_SIZE; i++ )
    {
        int nIndex = (m_nCurrentPage - 1) * 20 + i;

        char sText[STRLEN_64]="";
        _snprintf( sText, STRLEN_64, "ListBG%d", i );
        CUIControl * pButton = pControl->FindEventControl( sText );
		assert(pButton);

        pButton->SetVisible( N3UI_VISIBLE_TRUE );

        if( m_nTotalCount - 1 < nIndex )
        {
            pButton->SetVisible( N3UI_VISIBLE_FALSE );
            continue;
        }

        CObjCeremonyInfo * pCeremonyInfo = pPlayer->m_vCeremonyList[ nIndex ];

        assert( pCeremonyInfo );
        CResCeremonyTableFormat * pTable = dynamic_cast<CResCeremonyTableFormat*>(CResourceManager::GetPtr()->FindResource(RESOURCE_CEREMONYTABLE, pCeremonyInfo->m_nCode, 0));
		assert( pTable );

		CUIControl * pImg = pButton->FindEventControl( "ListImg" );
		assert(pImg);
		_snprintf( sText, STRLEN_64, "Icon/Ceremony/%s.png", pTable->m_sIconFile );
		CClientUIManager::GetPtr()->TextureChange(N3UI_STATE_NORMAL, pImg, sText );

		if( pCeremonyInfo->m_nEquipKind )          
			UIFunction::Set_UIVisibility( pButton, "UseListImg", N3UI_VISIBLE_TRUE );
        else
			UIFunction::Set_UIVisibility( pButton, "UseListImg", N3UI_VISIBLE_FALSE );
    }

    m_nEquipCount = 0;
    for( int i=0; i<m_nTotalCount; i++ )
    {
        CObjCeremonyInfo * pCeremonyInfo = pPlayer->m_vCeremonyList[ i ];
        if( pCeremonyInfo->m_nEquipKind == 1 ) m_nEquipCount++;
    }

    _snprintf( sText, STRLEN_64, "%d / %d", m_nEquipCount, m_nTotalCount );
    UIFunction::Set_ControlText( "EquipSkillNum", sText );

    m_nTotalPage = m_nTotalCount / LIST20_SIZE;
    if( m_nTotalCount % LIST20_SIZE != 0 ) m_nTotalPage++;

    _snprintf( sText, STRLEN_64, "%d / %d", m_nCurrentPage, m_nTotalPage);
	UIFunction::Set_ControlText( "Detail_Page", sText );

	UIFunction::Set_UIVisibility( "HaveCount", N3UI_VISIBLE_TRUE );
	UIFunction::Set_UIVisibility( "HaveCountBox", N3UI_VISIBLE_TRUE );
	UIFunction::Set_UIVisibility( "AllDivest", N3UI_VISIBLE_TRUE );
	UIFunction::Set_UIVisibility( "AllDivestBtn", N3UI_VISIBLE_TRUE );
}

void MySkillWindow::NextPage()
{
	if( m_nCurrentPage >= m_nTotalPage ) return;
        m_nCurrentPage++;

    switch( m_nCurrentMode )
    {
    case eMode::eSkillMode:
		this->UpdateSkillInfo();
        break;
    case eMode::eTrainingMode:
		this->UpdateTrainingInfo();
        break;
	case eMode::eCeremonyMode:
		this->UpdateCeremonyInfo();
        break;
    }
}

void MySkillWindow::PreviousPage()
{
	if( m_nCurrentPage <= 1 ) return;
        m_nCurrentPage--;

    switch( m_nCurrentMode )
    {
    case eMode::eSkillMode:
		this->UpdateSkillInfo();
        break;
    case eMode::eTrainingMode:
		this->UpdateTrainingInfo();
        break;
	case eMode::eCeremonyMode:
		this->UpdateCeremonyInfo();
        break;
    }
}

int MySkillWindow::GetCurrentPage()
{
	return m_nCurrentPage;
}