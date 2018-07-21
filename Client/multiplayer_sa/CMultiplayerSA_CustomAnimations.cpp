/*****************************************************************************
 *
 *  PROJECT:     Multi Theft Auto
 *  LICENSE:     See LICENSE in the top level directory
 *  FILE:        Client/multiplayer_sa/CMultiplayerSA_CustomAnimations.cpp
 *
 *  Multi Theft Auto is available from http://www.multitheftauto.com/
 *
 *****************************************************************************/

#include "StdInc.h"

#include <../game_sa/CAnimBlendHierarchySA.h>
#include <../game_sa/CAnimBlendStaticAssociationSA.h>
#include <../game_sa/CAnimBlendAssociationSA.h>
#include <../game_sa/CAnimBlendAssocGroupSA.h>

DWORD FUNC_CAnimBlendAssociation__ReferenceAnimBlock = 0x4CEA50;
DWORD FUNC_UncompressAnimation = 0x4D41C0;
DWORD FUNC_CAnimBlendAssociation__CAnimBlendAssociation_hierarchy = 0x4CEFC0;

DWORD RETURN_CAnimBlendAssocGroup_CopyAnimation_NORMALFLOW = 0x4CE151;
DWORD RETURN_CAnimBlendAssocGroup_CopyAnimation = 0x4CE187;
DWORD RETURN_CAnimBlendAssocGroup_CopyAnimation_ERROR = 0x4CE199;
DWORD RETURN_CAnimManager_AddAnimation = 0x4D3AB1;
DWORD RETURN_CAnimManager_AddAnimationAndSync = 0x4D3B41;
DWORD RETURN_CAnimManager_BlendAnimation_Hierarchy = 0x4D4577;

auto CAnimBlendAssociation_NewOperator_US = (hCAnimBlendAssociation_NewOperator)0x82119A;
auto CAnimBlendAssociation_NewOperator_EU = (hCAnimBlendAssociation_NewOperator)0x8211DA;

AddAnimationHandler*            m_pAddAnimationHandler = nullptr;
AddAnimationAndSyncHandler*     m_pAddAnimationAndSyncHandler = nullptr;
AssocGroupCopyAnimationHandler* m_pAssocGroupCopyAnimationHandler = nullptr;
BlendAnimationHierarchyHandler* m_pBlendAnimationHierarchyHandler = nullptr;

int _cdecl OnCAnimBlendAssocGroupCopyAnimation(AssocGroupId animGroup, int iAnimId);

void CMultiplayerSA::SetAddAnimationHandler(AddAnimationHandler* pHandler)
{
    m_pAddAnimationHandler = pHandler;
}

void CMultiplayerSA::SetAddAnimationAndSyncHandler(AddAnimationAndSyncHandler* pHandler)
{
    m_pAddAnimationAndSyncHandler = pHandler;
}

void CMultiplayerSA::SetAssocGroupCopyAnimationHandler(AssocGroupCopyAnimationHandler* pHandler)
{
    m_pAssocGroupCopyAnimationHandler = pHandler;
}

void CMultiplayerSA::SetBlendAnimationHierarchyHandler(BlendAnimationHierarchyHandler* pHandler)
{
    m_pBlendAnimationHierarchyHandler = pHandler;
}

CAnimBlendAssociationSAInterface* __cdecl CAnimBlendAssocGroup_CopyAnimation(RpClump* pClump, CAnimBlendAssocGroupSAInterface* pAnimAssocGroupInterface,
                                                                             AnimationId animID)
{
    auto CAnimBlendAssociation_NewOperator =
        pGameInterface->GetGameVersion() == VERSION_EU_10 ? CAnimBlendAssociation_NewOperator_EU : CAnimBlendAssociation_NewOperator_US;
    auto pAnimAssociationInterface =
        reinterpret_cast<CAnimBlendAssociationSAInterface*>(CAnimBlendAssociation_NewOperator(sizeof(CAnimBlendAssociationSAInterface)));

    if (pAnimAssociationInterface)
    {
        m_pAssocGroupCopyAnimationHandler(pAnimAssociationInterface, pClump, pAnimAssocGroupInterface, animID);
    }
    return pAnimAssociationInterface;
}

void _declspec(naked) HOOK_CAnimBlendAssocGroup_CopyAnimation()
{
    _asm
    {
        pushad
    }

    if (m_pAssocGroupCopyAnimationHandler)
    {
        _asm
        {
            popad

            push    esi

            push    eax // animID
            push    ecx // pAnimAssocGroupInterface
            push    edi // pClump
            call    CAnimBlendAssocGroup_CopyAnimation
            add     esp, 0Ch

            test    eax, eax
            jz      ERROR_CopyAnimation

            jmp     RETURN_CAnimBlendAssocGroup_CopyAnimation

            ERROR_CopyAnimation:
            jmp        RETURN_CAnimBlendAssocGroup_CopyAnimation_ERROR
        }
    }

    _asm
    {
        popad
        mov     ecx, [ecx+4]
        sub     eax, edx
        jmp RETURN_CAnimBlendAssocGroup_CopyAnimation_NORMALFLOW
    }
}

void _declspec(naked) HOOK_CAnimManager_AddAnimation()
{
    _asm
    {
        pushad
    }

    if (m_pAddAnimationHandler)
    {
        _asm
        {
            popad
            mov     ecx, [esp+4]  // animationClump
            mov     edx, [esp+8]  // animationGroup
            mov     eax, [esp+12] // animationID
            push    eax
            push    edx
            call    OnCAnimBlendAssocGroupCopyAnimation
            add     esp, 8
            mov     [esp+12], eax // replace animationID

            // call our handler function
            push    eax
            push    edx
            mov     ecx, [esp+12] // animationClump
            push    ecx
            call    m_pAddAnimationHandler
            add     esp, 0Ch
            pushad
            jmp     NORMAL_FLOW_AddAnimation
        }
    }

    _asm
    {
        NORMAL_FLOW_AddAnimation:
        popad
        mov     eax, dword ptr [esp+0Ch]
        mov     edx, dword ptr ds:[0B4EA34h]
        push    esi
        push    edi
        push    eax
        mov     eax, dword ptr [esp+14h] // animationGroup
        mov     edi, dword ptr [esp+10h] // animationClump
        jmp     RETURN_CAnimManager_AddAnimation
    }
}

void _declspec(naked) HOOK_CAnimManager_AddAnimationAndSync()
{
    _asm
    {
        pushad
    }

    if (m_pAddAnimationAndSyncHandler)
    {
         _asm
        {
            popad
            mov     ecx, [esp+4]  // animationClump
            mov     ebx, [esp+8]  // pAnimAssociationToSyncWith
            mov     edx, [esp+12] // animationGroup
            mov     eax, [esp+16] // animationID
            push    eax
            push    edx
            call    OnCAnimBlendAssocGroupCopyAnimation
            add     esp, 8
            mov     [esp+16], eax // replace animationID

            // call our handler function
            push    eax
            push    edx
            push    ebx
            mov     ecx, [esp+16] // animationClump
            push    ecx
            call    m_pAddAnimationAndSyncHandler
            add     esp, 10h
            pushad
            jmp     NORMAL_FLOW_AddAnimationAndSync
        }
    }

    _asm
    {
        NORMAL_FLOW_AddAnimationAndSync:
        popad
        mov     eax, dword ptr [esp+10h]
        mov     edx, dword ptr ds:[0B4EA34h]
        push    esi
        push    edi
        push    eax
        mov     eax, dword ptr [esp+18h] // animationGroup
        mov     edi, dword ptr [esp+10h] // animationClump
        jmp     RETURN_CAnimManager_AddAnimationAndSync
    }
}

void _declspec(naked) HOOK_CAnimManager_BlendAnimation_Hierarchy()
{
    _asm
    {
        pushad
    }

    if (m_pBlendAnimationHierarchyHandler)
    {
        _asm
        {
            popad
            push    eax // pAnimAssociation
            push    ecx // pAnimHierarchy

            push    ebp
            mov     ebp, esp
            sub     esp, 8

            // call our handler function
            mov     edx, [ebp+28h+4+12]
            lea     ecx, [ebp+28h+4+20]
            push    edx  // pClump
            push    ecx  // pFlags
            lea     edx, [ebp-4]
            push    edx  //  CAnimBlendHierarchySAInterface ** pOutAnimHierarchy
            mov     edx, [esp+28]
            push    edx  // pAnimAssociation
            call    m_pBlendAnimationHierarchyHandler //CAnimManager_BlendAnimation_Hierarchy
            add     esp, 10h

            mov     ecx, [ebp-4] // pCustomAnimHierarchy

            add     esp, 8 // remove space for local var
            mov     esp, ebp
            pop     ebp

             // Check if it's a custom animation or not
            cmp     al, 00
            je      NOT_CUSTOM_ANIMATION_CAnimManager_BlendAnimation_Hierarchy

            // Replace animation hierarchy with our custom one
            mov     [esp], ecx
            mov     [esp+28h+8+8], ecx

            NOT_CUSTOM_ANIMATION_CAnimManager_BlendAnimation_Hierarchy:
            pop ecx
            pop eax
            pushad
            jmp NORMAL_FLOW_BlendAnimation_Hierarchy
        }
    }

    _asm
    {
        NORMAL_FLOW_BlendAnimation_Hierarchy:
        popad
        mov     edx, [esp+28h+4]
        push    ecx      // pAnimHierarchy
        push    edx      // pClump
        mov     ecx, eax // pAnimAssociation
        call    FUNC_CAnimBlendAssociation__CAnimBlendAssociation_hierarchy
        mov     esi, eax
        mov     ax, word ptr [esp+28h+0Ch] // flags
        mov     ecx, esi
        mov     [esp+28h-4], 0FFFFFFFFh // var_4
        mov     [esp+28h-18h], esi
        mov     [esi+2Eh], ax
        call    FUNC_CAnimBlendAssociation__ReferenceAnimBlock
        mov     ecx, [esp+28h+8] // pAnimHierarchy
        push    ecx
        call    FUNC_UncompressAnimation
        jmp    RETURN_CAnimManager_BlendAnimation_Hierarchy
    }
}
