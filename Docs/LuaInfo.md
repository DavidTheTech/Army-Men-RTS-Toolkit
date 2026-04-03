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

```lua
MapObjManager.Update()

-- Find resources
local resources = MapObjManager.GetObjectsByName("resource.ore")
for i, res in ipairs(resources) do
    if res:IsResource() then
        print(string.format("Resource: %s, Amount: %d, Position: %.0f, %.0f, %.0f",
            res:GetName(), res:GetResourceCount(), res:GetX(), res:GetY(), res:GetZ()))
    end
end

-- Find and harvest a resource
local ore = MapObjManager.GetObjectByName("resource.ore")
if ore and ore:IsResource() and ore:GetResourceCount() > 0 then
    print("Resource found with", ore:GetResourceCount(), "units")
end

--[[local obj2 = MapObjManager.GetObjectByID(77)
if obj2 then
    print("Found:", obj2:GetName())
end]]

local unit = MapObjManager.GetObjectByName("army.unit.sarge")
if unit then
    print("found:", unit:GetID())
end

for i, obj in ipairs(MapObjManager.GetObjectsByName("army.unit.grunt")) do
    print(string.format("[%d] ID=%d, Name=%s, Team=%s, HP=%d, X=%.3f, Y=%.3f, Z=%.3f", i, obj:GetID(), obj:GetName(), obj:GetTeam(), obj:GetHitpoints(), obj:GetX(), obj:GetY(), obj:GetZ()))
end

local obj2 = MapObjManager.GetObjectByName("army.unit.sarge")
if obj2 then
    local x, y, z = obj2:GetPosition()
    --Console.Message(string.format("Position: X=%.3f, Y=%.3f, Z=%.3f", x, y, z))
    print(string.format("Position: X=%.3f, Y=%.3f, Z=%.3f", x, y, z))
end

local tan_units = MapObjManager.GetObjectsByTeam("tan")
for i, obj in ipairs(tan_units) do
    print(string.format("Tan[%d] %s (ID=%d) HP=%d", i, obj:GetName(), obj:GetID(), obj:GetHitpoints()))
end

--[[local obj = MapObjManager.GetObjectByID(77)
obj:SetPosition(100.5, 200.3, 50.0)
--or
obj:SetX(100.5)
obj:SetY(200.3)
obj:SetZ(50.0)]]

--print("aa")

for i, obj in ipairs(MapObjManager.Objects()) do
    if (obj:IsResource()) then
        print(string.format("ID=%d, Name=%s, count=%d, IsResource=%s", obj:GetID(), obj:GetName(), obj:GetResourceCount(), tostring(obj:IsResource())))
    end
end

local resources = MapObjManager.GetResourceObjects()
for i, res in ipairs(resources) do
    print(string.format("[%d] %s - Resource count: %d", i, res:GetName(), res:GetResourceCount()))
end```