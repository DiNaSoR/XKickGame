// EMERGENT GAME TECHNOLOGIES PROPRIETARY INFORMATION
//
// This software is supplied under the terms of a license agreement or
// nondisclosure agreement with Emergent Game Technologies and may not 
// be copied or disclosed except in accordance with the terms of that 
// agreement.
//
//      Copyright (c) 1996-2006 Emergent Game Technologies.
//      All Rights Reserved.
//
// Emergent Game Technologies, Chapel Hill, North Carolina 27517
// http://www.emergent.net
//---------------------------------------------------------------------------
#include "stdafx.h"
#include "ShaderHelper.h"
#include <NiApplication.h>
#include "../UtilLibrary/UtlEtc.h"

// This sample only has static library configurations
#define _LIB
    #include <NSBShaderLib.h>
    #include <NSFParserLib.h>
#undef _LIB

#if defined(_DX9)
    #pragma comment(lib, "NiBinaryShaderLibDX9.lib")
    #pragma comment(lib, "NSBShaderLibDX9.lib")
    #pragma comment(lib, "NSFParserLibDX9.lib")
#endif

//---------------------------------------------------------------------------
bool CShaderHelper::SetupShaderSystem(const char* pcProgramDirectory)
{
    assert (NiD3DShaderProgramFactory::GetInstance());
    NiD3DShaderProgramFactory::GetInstance()->AddProgramDirectory(
        pcProgramDirectory);

    // First, we will run the NSF parser. This is done to make sure that
    // any text-based shader files that have been modified are re-compiled
    // to binary before loading all the binary representations.
    if (!RunShaderParsers())
    {
        NiMessageBox("Failed to run shader parsers!", "ERROR");
        return false;
    }

    if (!RegisterShaderLibraries())
    {
        //NiMessageBox("Failed to register shader libraries!", "ERROR");
        return false;
    }

    return true;
}
//---------------------------------------------------------------------------
bool CShaderHelper::RunShaderParsers()
{
    NiShaderFactory::RegisterRunParserCallback(RunParser);

    unsigned int uiCount = NiShaderFactory::LoadAndRunParserLibrary(
        NULL, NiApplication::ConvertMediaFilename("./Shaders"), true);

    char acTemp[MAX_PATH];
    NiSprintf(acTemp, MAX_PATH, "Parsed %d shaders from NSFShaderParser!\n", 
        uiCount);
    NiOutputDebugString(acTemp);

    return true;
}
//---------------------------------------------------------------------------
bool CShaderHelper::RegisterShaderLibraries()
{
    NiShaderFactory::RegisterClassCreationCallback(LibraryClassCreate);

    NiShaderFactory::AddShaderProgramFileDirectory("./Shaders/Dx9/");//쉐이더 경로 설정

    unsigned int uiCount = 1;
    char* apcDirectories[1];
    apcDirectories[0] = 
        (char*)NiApplication::ConvertMediaFilename("Shaders");

    if (!NiShaderFactory::LoadAndRegisterShaderLibrary(NULL,
        uiCount, apcDirectories, true))
    {
        //NiMessageBox("Failed to load shader library!", "ERROR");
        return false;
    }

    return true;
}
//---------------------------------------------------------------------------
bool CShaderHelper::CleanupShaderLibraries()
{
    // Note that we don't have to do anything here. The scene will clean up
    // the shader instances, and the renderer shutdown code will take care
    // of the factory!
    return true;
}
//---------------------------------------------------------------------------
bool CShaderHelper::LibraryClassCreate(const char* pcLibFile, 
    NiRenderer* pkRenderer, int iDirectoryCount, char* apcDirectories[], 
    bool bRecurseSubFolders, NiShaderLibrary** ppkLibrary)
{
    *ppkLibrary = NULL;

#ifndef _PACKING
    // Create the NSB Shader Library.
    return NSBShaderLib_LoadShaderLibrary(pkRenderer, iDirectoryCount, 
        apcDirectories, bRecurseSubFolders, ppkLibrary);

#else
    std::vector< std::string > kFileNameArray;

    kFileNameArray = CLoadPackFile::GetPtr()->GetPackFileName( "Shaders.pak" );
    NiTPointerList<char*> kFileNameList;

    for( size_t i=0; i<kFileNameArray.size(); i++ )
    {
        std::string & kFileName = kFileNameArray[i];       

        if( std::string::npos == kFileName.find( ".NSB" ) )
        {
            continue;
        }
        
        ReplaceString( kFileName, "/", "\\" );

        kFileName = "Shaders\\" + kFileName;        
        kFileNameList.AddTail( (char*) kFileName.c_str() );
    }

    return NSBShaderLib_LoadShaderLibrary(pkRenderer, kFileNameList, ppkLibrary );

#endif
}
//---------------------------------------------------------------------------
unsigned int CShaderHelper::RunParser(const char* pcLibFile, 
    NiRenderer* pkRenderer, const char* pcDirectory, 
    bool bRecurseSubFolders)
{
    // Run the NSF Parser.
    return NSFParserLib_RunShaderParser(pcDirectory, bRecurseSubFolders);
}
//---------------------------------------------------------------------------

