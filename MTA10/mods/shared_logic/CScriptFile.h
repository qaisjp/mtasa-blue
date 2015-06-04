/*****************************************************************************
*
*  PROJECT:     Multi Theft Auto v1.0
*               (Shared logic for modifications)
*  LICENSE:     See LICENSE in the top level directory
*  FILE:        mods/shared_logic/CScriptFile.h
*  PURPOSE:     Script file class header
*  DEVELOPERS:  Christian Myhre Lundheim <>
*               Stanislav Bobrov <lil_toady@hotmail.com>
*               Cecill Etheredge <ijsf@gmx.net>
*
*****************************************************************************/

#ifndef __CFILE_H
#define __CFILE_H

#include "CClientEntity.h"
#include <stdio.h>
#include <string>

class CScriptFile : public CClientEntity
{
    DECLARE_CLASS( CScriptFile, CClientEntity )
public:
    enum eMode
    {
        MODE_READ,
        MODE_READWRITE,
        MODE_CREATE,
    };

                            CScriptFile             ( const char* szFilename, unsigned long ulMaxSize, SString strFile );
                            ~CScriptFile            ( void );

    // Functions required for linking
    void                    GetPosition             ( CVector& vecPosition ) const {};
    void                    SetPosition             ( const CVector& vecPosition ) {};

    // Functions required by CClientEntity
    eClientEntityType       GetType                 ( void ) const  { return SCRIPTFILE; };
    void                    Unlink                  ( void ) {};
    bool                    ReadSpecialData         ( void )    { return true; };

    // Load and unload routines
    bool                    Load                    ( eMode Mode );
    void                    Unload                  ( void );
    bool                    IsLoaded                ( void )    { return m_pFile != NULL; };
    SString                 GetPath                 ( void )    { return m_strFilePath; };

    // Only call functions these functions if you're sure
    // that the file is loaded, otherwise you will crash:
    bool                    IsEOF ( void );
    long                    GetPointer              ( void );
    long                    GetSize                 ( void );

    long                    SetPointer              ( unsigned long ulPosition );

    void                    Flush                   ( void );
    long                    Read                    ( unsigned long ulSize, char* pData );
    long                    Write                   ( unsigned long ulSize, const char* pData );

private:
    void                    DoResourceFileCheck     ( void );

    CBinaryFileInterface*   m_pFile;
    std::string             m_strFilename;
    SString                 m_strFilePath;
    unsigned long           m_ulMaxSize;
    bool                    m_bDoneResourceFileCheck;
};

#endif
