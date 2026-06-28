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

#ifndef _SHADER_HELPER_H
#define _SHADER_HELPER_H

#include <NiDirectXVersion.h>
#include <NiD3DDefines.h>

#include <NiShader.h>
#include <NiShaderFactory.h>
#include <NiShaderLibrary.h>
#include <NiShaderLibraryDesc.h>
#include <NiD3DShaderProgramFactory.h>
#include <NiD3DRendererHeaders.h>

class CShaderHelper : public NiMemObject
{
public:
    bool SetupShaderSystem(const char* pcProgramDirectory);
    bool RunShaderParsers();
    bool RegisterShaderLibraries();
    bool CleanupShaderLibraries();

protected:
    static bool LibraryClassCreate(const char* pcLibFile,
        NiRenderer* pkRenderer, int iDirectoryCount, char* apcDirectories[], 
        bool bRecurseSubFolders, NiShaderLibrary** ppkLibrary);
    static unsigned int RunParser(const char* pcLibFile, 
        NiRenderer* pkRenderer, const char* pcDirectory, 
        bool bRecurseSubFolders);
};

#endif
