/*****************************************************************************
 *
 *  PROJECT:     Multi Theft Auto v1.0
 *               (Shared logic for modifications)
 *  LICENSE:     See LICENSE in the top level directory
 *  FILE:        mods/shared_logic/CClient3DMarker.h
 *  PURPOSE:     3D marker entity class
 *
 *****************************************************************************/

#ifndef __CCLIENT3DMARKER_H
#define __CCLIENT3DMARKER_H

#include "CClientMarker.h"
class CClientMarker;

class CClient3DMarker : public CClientMarkerCommon
{
public:
    enum
    {
        TYPE_CYLINDER,
        TYPE_ARROW,
        TYPE_INVALID,
    };

    CClient3DMarker(CClientMarker* pThis);
    ~CClient3DMarker(void);

    unsigned int GetMarkerType(void) const { return CClientMarkerCommon::CLASS_3DMARKER; };

    unsigned long Get3DMarkerType(void);
    void          Set3DMarkerType(unsigned long ulType);

    bool IsHit(const CVector& vecPosition) const;

    void GetPosition(CVector& vecPosition) const { vecPosition = m_Matrix.vPos; };
    void SetPosition(const CVector& vecPosition) { m_Matrix.vPos = vecPosition; };

    void GetMatrix(CMatrix& mat) { mat = m_Matrix; };
    void SetMatrix(CMatrix& mat) { m_Matrix = mat; };

    bool IsVisible(void) const { return m_bVisible; };
    void SetVisible(bool bVisible) { m_bVisible = bVisible; };

    SColor GetColor(void) const { return m_Color; }
    void   SetColor(const SColor& color) { m_Color = color; }

    float GetSize(void) const { return m_fSize; };
    void  SetSize(float fSize) { m_fSize = fSize; };

    float GetPulseFraction(void) { return static_cast<float>(m_pMarker->GetPulseFraction()); };
    void  SetPulseFraction(float fFraction) { m_pMarker->SetPulseFraction(fFraction); };

protected:
    void StreamIn(void);
    void StreamOut(void);

    void DoPulse(void);

private:
    CClientMarker* m_pThis;
    CMatrix        m_Matrix;

    bool         m_bVisible;
    DWORD        m_dwType;
    float        m_fSize;
    SColor       m_Color;
    C3DMarker*   m_pMarker;
    unsigned int m_ulIdentifier;
    bool         m_bMarkerStreamedIn;
};

#endif
