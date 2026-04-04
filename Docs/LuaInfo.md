MapObjManager
`Update()` Updates the internal object list (call before accessing objects)
`GetObjectByID(int)` Returns the object with the given ID, or nil
`GetObjectByName(string)` Returns the first object matching the name, or nil
`GetObjectsByName(string)` Returns a table of all objects with the given name
`GetObjectsByTeam(string)` Returns a table of all objects belonging to the specified team
`GetResourceObjects()` Returns a table of all objects that are resource nodes
`Objects()` Returns a table of all objects currently in the world

MapObject
`GetID()` Returns the object's unique ID (number)
`GetName()` Returns the object's name (string)
`GetTeam()` Returns the team name (string)
`GetHitpoints()` Returns current hitpoints (number)
`GetTotalHitpoints()` Returns maximum hitpoints (number)
`SetHitpoints(int)` Sets current hitpoints
`GetPosition()` Returns x, y, z coordinates (three numbers)
`SetPosition(x, y, z)` Sets position (numbers)
`GetX()`, `GetY()`, `GetZ()` Returns individual coordinate
`SetX(x)`, `SetY(y)`, `SetZ(z)` Sets individual coordinate
`IsResource()` Returns true if the object is a resource node (boolean)
`GetResourceCount()` Returns the amount of resource available (number)

Client.Events
`SelectUnit(unit, modifier)` Selects a unit (modifier: 0=default, 1, 2, or 3)
`CreateParticle(x, y, z, crc)` Creates particle effect at position (crc: hex string like "0x12EA8886")
`CreateParticle(x, y, z, string)` Creates particle effect at position (string: "Client::Move")
`TriggerResponse(crc)` Triggers response event (crc: hex string like "0x3381FB36")
`TriggerClientMode(mode)` Triggers client mode (mode: 0)

Orders.Game.Move
`Generate(x, y, z, attack, modifier, a5)` Moves selected units to position (x, y, z) - attack: boolean, modifier: integer (1,2,3, or default 8), a5: optional integer (defaults to 1)

```lua
-- Example: Select and move all green dumdum units to attack grunt position
MapObjManager.Update()

local dumdums = MapObjManager.GetObjectsByName("army.unit.dumdum")
for i, dumdum in ipairs(dumdums) do
    if dumdum:GetTeam() == "green" then
        Client.Events.SelectUnit(dumdum, 1)
        local grunt = MapObjManager.GetObjectByName("army.unit.grunt")
        if grunt then
            local x, y, z = grunt:GetPosition()
            Orders.Game.Move.Generate(x, y, z, true, 1)
        end
    end
end

-- Example: Create particle effects at sarge position
local sarge = MapObjManager.GetObjectByName("army.unit.sarge")
if sarge then
    local x, y, z = sarge:GetPosition()
    for i = 0, 20 do
        Client.Events.CreateParticleCrc(x, y, z, "0x12EA8886")
        Client.Events.CreateParticle(x, y, z, "Client::Move")
        sleep(100)
    end
end

-- Example: Find and list all resources
local resources = MapObjManager.GetResourceObjects()
for i, res in ipairs(resources) do
    print(string.format("[%d] %s - Resource count: %d", i, res:GetName(), res:GetResourceCount()))
end

-- Example: List all grunt units
for i, obj in ipairs(MapObjManager.GetObjectsByName("army.unit.grunt")) do
    print(string.format("[%d] ID=%d, Name=%s, Team=%s, HP=%d, X=%.3f, Y=%.3f, Z=%.3f", i, obj:GetID(), obj:GetName(), obj:GetTeam(), obj:GetHitpoints(), obj:GetX(), obj:GetY(), obj:GetZ()))
end

-- Example: Get sarge position
local sarge = MapObjManager.GetObjectByName("army.unit.sarge")
if sarge then
    local x, y, z = sarge:GetPosition()
    print(string.format("Sarge Position: X=%.3f, Y=%.3f, Z=%.3f", x, y, z))
end

-- Example: Get all tan team units
local tan_units = MapObjManager.GetObjectsByTeam("tan")
for i, obj in ipairs(tan_units) do
    print(string.format("Tan[%d] %s (ID=%d) HP=%d", i, obj:GetName(), obj:GetID(), obj:GetHitpoints()))
end

-- Example: Find a resource
local ore = MapObjManager.GetObjectByName("resource.chewtoy")
if ore and ore:IsResource() and ore:GetResourceCount() > 0 then
    print("Resource found with", ore:GetResourceCount(), "units")
end

-- Example: Iterate all objects and check if they are resources
for i, obj in ipairs(MapObjManager.Objects()) do
    if obj:IsResource() then
        print(string.format("ID=%d, Name=%s, count=%d, IsResource=%s", obj:GetID(), obj:GetName(), obj:GetResourceCount(), tostring(obj:IsResource())))
    end
end

-- Example: Select and move sarge
local unit = MapObjManager.GetObjectByName("army.unit.sarge")
if unit then
    Client.Events.SelectUnit(unit, 0)
    Orders.Game.Move.Generate(100, 50, 200, false, 1, 1)
end

-- Example: Trigger response and client mode
Client.Events.TriggerResponse("0x3381FB36")
Client.Events.TriggerClientMode(0)```