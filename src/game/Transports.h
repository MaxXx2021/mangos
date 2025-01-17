/*
 * Copyright (C) 2005-2011 MaNGOS <http://getmangos.com/>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifndef TRANSPORTS_H
#define TRANSPORTS_H

#include "GameObject.h"

#include <map>
#include <set>
#include <string>

class Transport : public GameObject
{
    public:

        static Transport* Load(Map * map, uint32 entry, const std::string& name, uint32 period);
        void Update(uint32 update_diff, uint32 p_time) override;
        bool AddPassenger(Player* passenger);
        bool RemovePassenger(Player* passenger);

        typedef std::set<Player*> PlayerSet;
        PlayerSet const& GetPassengers() const { return m_passengers; }
        uint32 MovementProgress() const { return m_timer;}

    private:
        struct WayPoint
        {
            WayPoint() : mapid(0), x(0), y(0), z(0), teleport(false) {}
            WayPoint(uint32 _mapid, float _x, float _y, float _z, bool _teleport, uint32 _arrivalEventID = 0, uint32 _departureEventID = 0)
                : mapid(_mapid), x(_x), y(_y), z(_z), teleport(_teleport),
                arrivalEventID(_arrivalEventID), departureEventID(_departureEventID)
            {
            }

            uint32 mapid;
            float x;
            float y;
            float z;
            bool teleport;
            uint32 arrivalEventID;
            uint32 departureEventID;
        };

        typedef std::map<uint32, WayPoint> WayPointMap;

        explicit Transport();
        bool Create(uint32 entry);
        bool GenerateWaypoints(uint32 pathid, std::set<uint32> &mapids);
        void TeleportTransport(uint32 newMapid, float x, float y, float z);
        void DoEventIfAny(WayPointMap::value_type const& node, bool departure);
        void MoveToNextWayPoint();                          // move m_next/m_cur to next points
        void SetPeriod(uint32 time) { SetUInt32Value(GAMEOBJECT_LEVEL, time);}
        uint32 GetPeriod() const { return GetUInt32Value(GAMEOBJECT_LEVEL);}

    private:
        WayPointMap::const_iterator m_curr;
        WayPointMap::const_iterator m_next;
        uint32 m_pathTime;
        uint32 m_timer;

        PlayerSet m_passengers;
        WayPointMap m_WayPoints;
        uint32 m_nextNodeTime;
};
#endif
