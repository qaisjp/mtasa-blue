/*****************************************************************************
 *
 *  PROJECT:     Multi Theft Auto v1.0
 *  LICENSE:     See LICENSE in the top level directory
 *  FILE:        mods/deathmatch/logic/CUnoccupiedVehicleSync.h
 *  PURPOSE:     Header for unoccupied vehicle sync class
 *
 *  Multi Theft Auto is available from http://www.multitheftauto.com/
 *
 *****************************************************************************/

#pragma once

#include <CClientCommon.h>
#include "CDeathmatchVehicle.h"
#define MIN_ROTATION_DIFF   0.1
class CUnoccupiedVehicleSync
{
public:
    CUnoccupiedVehicleSync(CClientVehicleManager* pVehicleManager);
    ~CUnoccupiedVehicleSync(void);

    bool ProcessPacket(unsigned char ucPacketID, NetBitStreamInterface& bitStream);
    void DoPulse(void);

    void AddVehicle(CDeathmatchVehicle* pVehicle);
    void RemoveVehicle(CDeathmatchVehicle* pVehicle);
    void ClearVehicles(void);

    std::list<CDeathmatchVehicle*>::const_iterator IterBegin(void) { return m_List.begin(); };
    std::list<CDeathmatchVehicle*>::const_iterator IterEnd(void) { return m_List.end(); };

    bool Exists(CDeathmatchVehicle* pVehicle);

private:
    void Packet_UnoccupiedVehicleStartSync(NetBitStreamInterface& BitStream);
    void Packet_UnoccupiedVehicleStopSync(NetBitStreamInterface& BitStream);
    void Packet_UnoccupiedVehicleSync(NetBitStreamInterface& BitStream);

    void UpdateDamageModels(void);
    void UpdateStates(void);
    bool WriteVehicleInformation(NetBitStreamInterface* pBitStream, CDeathmatchVehicle* pVehicle);

    CClientVehicleManager*           m_pVehicleManager;
    CMappedList<CDeathmatchVehicle*> m_List;
    unsigned long                    m_ulLastSyncTime;
};
