#include "Common.h"
#include "ResTableFormat.h"
#include "ResourceManager.h"
#include "CoreApi.h"
#include "UtlLog.h"

typedef std::vector<int> IntegerArray;

void RemoveItemDivestRecursive( NiAVObject * pkAVObject, std::string & cExtraDataName, IntegerArray & kDivestTypes )
{
    assert( pkAVObject != 0 );
    if( pkAVObject == 0 ) return;

    NiExtraData * pkExtraData = pkAVObject->GetExtraData( cExtraDataName.c_str() );
    if( pkExtraData != 0 && NiIsKindOf(NiIntegerExtraData, pkExtraData) )
    {
        NiExtraData * pProcessExtraCheck = pkAVObject->GetExtraData( "ProcessDivest" );
        if( pProcessExtraCheck != 0 )
        {
            pkAVObject->SetAppCulled( false );
        }

        NiIntegerExtraData * pkItemCodeExtraData = (NiIntegerExtraData*) pkExtraData;

        int nItemCode = pkItemCodeExtraData->GetValue();
        CResItemTableFormat * pkItemTable = (CResItemTableFormat*) CResourceManager::GetPtr()->FindResource( RESOURCE_ITEMTABLE, nItemCode, 0 );

        for( size_t i=0; i<kDivestTypes.size(); i++ )
        {
            int nDivestType = kDivestTypes[i];
            if( pkItemTable->m_nType == nDivestType )
            {
                //NiNode * pkParent = pkAVObject->GetParent();
                //if( pkParent != 0 )
                //{
                //    pkParent->DetachChild( pkAVObject );
                //}
                pkAVObject->SetAppCulled( true );

                NiIntegerExtraData * pProcessDivest = (NiIntegerExtraData*) pkAVObject->GetExtraData( "ProcessDivest" );
                if( pProcessDivest == 0 )
                {
                    pProcessDivest = NiNew NiIntegerExtraData( 0 );
                    pProcessDivest->SetName( "ProcessDivest" );

                    pkAVObject->AddExtraData( pProcessDivest );
                }
                break;
            }
        }
    }

    if( NiIsKindOf( NiNode, pkAVObject ) )
    {
        NiNode * pkNode = (NiNode*) pkAVObject;
        for( unsigned int i=0; i<pkNode->GetArrayCount(); i++ )
        {
            NiAVObject * pkChild = pkNode->GetAt(i);
            if( pkChild == 0 ) continue;

            RemoveItemDivestRecursive( pkChild, cExtraDataName, kDivestTypes );
        }
    }
}

void RemoveItemDivestRecursiveS( NiAVObject * pkAVObject, std::string & cExtraDataName, IntegerArray & kDivestTypes )
{
    assert( pkAVObject != 0 );
    if( pkAVObject == 0 ) return;

    NiExtraData * pkExtraData = pkAVObject->GetExtraData( cExtraDataName.c_str() );
    if( pkExtraData != 0 && NiIsKindOf(NiIntegerExtraData, pkExtraData) )
    {
        NiExtraData * pProcessExtraCheck = pkAVObject->GetExtraData( "ProcessDivest" );
        if( pProcessExtraCheck != 0 )
        {
            pkAVObject->SetAppCulled( false );
        }

        NiIntegerExtraData * pkItemCodeExtraData = (NiIntegerExtraData*) pkExtraData;

        int nItemCode = pkItemCodeExtraData->GetValue();
        CResItemTableFormat * pkItemTable = (CResItemTableFormat*) CResourceManager::GetPtr()->FindResource( RESOURCE_ITEMTABLE, nItemCode, 0 );
		if( pkItemTable == NULL ) return;

		for( size_t i=0; i<kDivestTypes.size(); i++ )
        {
            int nDivestType = kDivestTypes[i];
			if( pkItemTable->m_nType == nDivestType )
            {
                pkAVObject->SetAppCulled( true );

                NiIntegerExtraData * pProcessDivest = (NiIntegerExtraData*) pkAVObject->GetExtraData( "ProcessDivest" );
                if( pProcessDivest == 0 )
                {
                    pProcessDivest = NiNew NiIntegerExtraData( 0 );
                    pProcessDivest->SetName( "ProcessDivest" );

                    pkAVObject->AddExtraData( pProcessDivest );
                }
                break;
            }
        }
    }

    if( NiIsKindOf( NiNode, pkAVObject ) )
    {
        NiNode * pkNode = (NiNode*) pkAVObject;
        for( unsigned int i=0; i<pkNode->GetArrayCount(); i++ )
        {
            NiAVObject * pkChild = pkNode->GetAt(i);
            if( pkChild == 0 ) continue;

            RemoveItemDivestRecursiveS( pkChild, cExtraDataName, kDivestTypes );
        }
    }
}

void RemoveItemDivest( NiAVObject * pkAVObject )
{
    assert( pkAVObject != 0 );
    if( pkAVObject == 0 ) return;

    ExtraDataArray kExtraDataArray;
    IntegerArray   kIntegerExtraArray;

    std::string kExtraDataName = "ItemCode";

    CoreApi_RecursiveCollectExtraData( pkAVObject, kExtraDataName, kExtraDataArray );
    for( size_t i=0; i<kExtraDataArray.size(); i++ )
    {
        NiExtraData * pkExtraData = kExtraDataArray[i];

        assert( NiIsKindOf( NiIntegerExtraData, pkExtraData) );
        if( !NiIsKindOf( NiIntegerExtraData, pkExtraData) ) continue;

        NiIntegerExtraData * pkItemCodeExtraData = (NiIntegerExtraData*) pkExtraData;
        
        assert( pkItemCodeExtraData != 0 );
        if( pkItemCodeExtraData == 0 ) continue;

        int nItemCode = pkItemCodeExtraData->GetValue();
        CResItemTableFormat * pkItemTable = (CResItemTableFormat*) CResourceManager::GetPtr()->FindResource( RESOURCE_ITEMTABLE, nItemCode, 0 );

        assert( pkItemTable != 0 );
        if( pkItemTable == 0 ) continue;
		if( atoi(pkItemTable->m_sDivest) == 0 ) continue;

        int nLen = strlen(pkItemTable->m_sDivest);
		if(nLen%3)
			_Assert(NULL, "DivestCode Odd Length! : ", (float)nLen, "", 0);
		char sType[4]="";
		while(0<nLen)
		{
			nLen-=3;
			sType[0] = pkItemTable->m_sDivest[nLen];
			sType[1] = pkItemTable->m_sDivest[nLen+1];
			sType[2] = pkItemTable->m_sDivest[nLen+2];

            kIntegerExtraArray.push_back(atoi(sType));
            
			memset(sType, 0, sizeof(sType));
        }
    }

    RemoveItemDivestRecursive( pkAVObject, kExtraDataName, kIntegerExtraArray );
}

void RemoveItemDivestS( NiAVObject * pkAVObject, int* nWearCode )
{
    assert( pkAVObject != 0 );
    if( pkAVObject == 0 ) return;

    ExtraDataArray kExtraDataArray;
    IntegerArray   kIntegerExtraArray;

    std::string kExtraDataName = "ItemCode";

    CoreApi_RecursiveCollectExtraData( pkAVObject, kExtraDataName, kExtraDataArray );
    for( size_t i=0; i<kExtraDataArray.size(); i++ )
    {
        NiExtraData * pkExtraData = kExtraDataArray[i];

        assert( NiIsKindOf( NiIntegerExtraData, pkExtraData) );
        if( !NiIsKindOf( NiIntegerExtraData, pkExtraData) ) continue;

        NiIntegerExtraData * pkItemCodeExtraData = (NiIntegerExtraData*) pkExtraData;
        
        assert( pkItemCodeExtraData != 0 );
        if( pkItemCodeExtraData == 0 ) continue;

        int nItemCode = pkItemCodeExtraData->GetValue();
        CResItemTableFormat * pkItemTable = (CResItemTableFormat*) CResourceManager::GetPtr()->FindResource( RESOURCE_ITEMTABLE, nItemCode, 0 );
        if( pkItemTable == NULL) continue;

		for(int j=0;j<=ITEM_EQUIP_KNEE;j++)
		{
			CResItemTableFormat * psItemTable = (CResItemTableFormat*) CResourceManager::GetPtr()->FindResource( RESOURCE_ITEMTABLE, nWearCode[j], 0 );
			if( psItemTable == NULL) continue;
			else
			{
				if( pkItemTable->m_nWear == psItemTable->m_nWear )
				{
					pkItemTable = (CResItemTableFormat*) CResourceManager::GetPtr()->FindResource( RESOURCE_ITEMTABLE, nWearCode[j], 0 );
					break;
				}
			}
		}
		
		if( atoi(pkItemTable->m_sDivest) == 0 ) continue;

        int nLen = strlen(pkItemTable->m_sDivest);
		if(nLen%3)
			_Assert(NULL, "DivestCode Odd Length! : ", (float)nLen, "", 0);
		char sType[4]="";
		while(0<nLen)
		{
			nLen-=3;
			sType[0] = pkItemTable->m_sDivest[nLen];
			sType[1] = pkItemTable->m_sDivest[nLen+1];
			sType[2] = pkItemTable->m_sDivest[nLen+2];

			kIntegerExtraArray.push_back(atoi(sType));
            
			memset(sType, 0, sizeof(sType));
        }
    }

    RemoveItemDivestRecursiveS( pkAVObject, kExtraDataName, kIntegerExtraArray );
}