# Army Men RTS Lua API Reference

This document describes the Lua API available for scripting in Army Men RTS. All functions are exposed via the game's Lua engine.

## Table of Contents
- [MapObjManager]#mapobjmanager
- [MapObject]#mapobject
- [Client.Events]#clientevents
- [Orders.Game.Move]#ordersgamemove
- [Console]#console
- [VarSys]#varsys
- [Log.Client]#logclient
- [Internals]#internals
- [Multiplayer.Data]#Multiplayerdata
- [SquadManager]#squadmanager
- [Utilities]#utilities

---

## MapObjManager

Manages world objects. Always call `Update() before accessing objects.

| Function | Description |
|----------|--------------|
| Update() | Updates the internal object list (call before accessing objects) |
| GetObjectByID(int) | Returns the object with the given ID, or nil |
| GetObjectByName(string) | Returns the first object matching the name, or nil |
| GetObjectsByName(string) | Returns a table of all objects with the given name |
| GetObjectsByTeam(string) | Returns a table of all objects belonging to the specified team |
| GetResourceObjects() | Returns a table of all objects that are resource nodes |
| Objects() | Returns a table of all objects currently in the world |

**Example:**
```lua
MapObjManager.Update()
local sarge = MapObjManager.GetObjectByName("army.unit.sarge")
if sarge then
    print(sarge:GetName())
end
```

---

## MapObject

Represents a single object in the world. Methods are called on object instances.

| Function | Description |
|----------|--------------|
| GetID() | Returns the object's unique ID (number) |
| GetName() | Returns the object's name (string) |
| GetTeam() | Returns the team name (string) |
| GetHitpoints() | Returns current hitpoints (number) |
| GetTotalHitpoints() | Returns maximum hitpoints (number) |
| SetHitpoints(int) | Sets current hitpoints |
| GetPosition() | Returns x, y, z coordinates (three numbers) |
| SetPosition(x, y, z) | Sets position (numbers) |
| GetX(), `GetY(), `GetZ() | Returns individual coordinate |
| SetX(x), `SetY(y), `SetZ(z) | Sets individual coordinate |
| IsResource() | Returns true if the object is a resource node (boolean) |
| GetResourceCount() | Returns the amount of resource available (number) |

**Example:**
```lua
local unit = MapObjManager.GetObjectByName("army.unit.sarge")
if unit then
    local x, y, z = unit:GetPosition()
    print(string.format("Sarge at: %.2f, %.2f, %.2f", x, y, z))
    unit:SetHitpoints(500)
end
```

---

## Client.Events

Functions for sending UI and game events.

| Function | Description |
|----------|--------------|
| SelectUnit(unit, modifier) | Selects a unit (modifier: 0=default, 1, 2, or 3) |
| CreateParticle(x, y, z, string) | Creates particle effect at position (string like "Client::Move") |
| CreateParticleCrc(x, y, z, hexString) | Creates particle effect using a CRC hex string (e.g., "0x12EA8886") |
| UpdateSelectedLists() | Updates the internal selected unit lists |
| TriggerResponse(hexString) | Triggers response event (hex string like "0x3381FB36") |
| TriggerClientMode(mode) | Triggers client mode (mode: 0) |

**Example:**
```lua
local unit = MapObjManager.GetObjectByName("army.unit.sarge")
if unit then
    Client.Events.SelectUnit(unit, 1)
    local x, y, z = unit:GetPosition()
    for i = 1, 20 do
        Client.Events.CreateParticle(x, y, z, "Client::Attack")
        Client.Events.CreateParticleCrc(x, y, z, "0x12EA8886")
        sleep(100)
    end
end
```

## Orders.Game.Move

Movement orders for selected units.

| Function | Description |
|----------|--------------|
 | Generate(x, y, z, attack, modifier, a5) | Moves selected units to position. `attack:` boolean, `modifier: integer (1,2,3, or default 8), `a5`: optional integer (defaults to 1) |

**Example:**
```lua
local grunt = MapObjManager.GetObjectByName("army.unit.grunt")
if grunt then
    local x, y, z = grunt:GetPosition()
    Orders.Game.Move.Generate(x, y, z, true, 1)
end
```

---

## Console

Console command and output functions.

| Function | Description |
|----------|--------------|
| ProcessCmd(string) | Executes a console command |
| Message(string) | Prints a message to the console |
| SetType(hexString) | Sets console output type (e.g., "0xB8B548C0") |

**Example:**
```lua
Console.Message("Hello from Lua!")
Console.ProcessCmd("coregame.createobj army.unit.scorch 514 14 202")
```

---

## VarSys

Variable system commands.

| Function | Description |
|----------|--------------|
 | CreateCmd(string) | Creates a VarSys command |

---

## Log.Client

Logging functions.

| Function | Description |
|----------|--------------|
 | Write(string) | Writes a message to the client log |

**Example:**
```lua
Log.Client.Write("Script started")
```

---

## Internals

Internal game state overrides (bypass pause on alt-tab).

| Function | Description |
|----------|--------------|
 | ToggleGameUpdates() | Toggles game update override. Returns status string. |
 | IsGameUpdating() | Returns true if game is updating (boolean) |
 | GetGameState() | Returns three booleans: `updating`, `bit1State`, `activeState` |

**Example:**
```lua
if not Internals.IsGameUpdating() then
    Internals.ToggleGameUpdates()
end

local updating, bit1, active = Internals.GetGameState()
print(string.format("Updating: %s, Bit1: %s, Active: %s", updating, bit1, active))
```

---

## Multiplayer.Data

Multiplayer data sending.

| Function | Description |
|----------|--------------|
 | Send(string) | Sends a string over multiplayer |

**Example:**
```lua
Multiplayer.Data.Send("Hello minty!!!")
```

---

## SquadManager

Squad management (1-4).

| Function | Description |
|----------|--------------|
| Create(squadNumber) | Creates a squad (1-4) |
| Add(squadNumber) | Adds selected units to squad (1-4) |
| Select(squadNumber) | Selects squad (1-4) |
| JumpTo(squadNumber) | Jumps camera to squad (1-4) |

**Example:**
```lua
local sarge = MapObjManager.GetObjectByName("army.unit.sarge")
if sarge then
    Client.Events.SelectUnit(sarge, 1)
    SquadManager.Create(1)
    SquadManager.Add(1)
    SquadManager.Select(1)
    SquadManager.JumpTo(1)
end
```

---

## Utilities

Helper functions.

| Function | Description |
|----------|--------------|
 | sleep(ms) | Sleeps for milliseconds |
 | testterrain() | Renders terrain map to "NewTerrain.tga" |

**Example:**
```lua
sleep(1000)
testterrain()
```

---

## Full Example Script

```lua
-- Wait for game to initialize
sleep(1000)

-- Ensure game updates are running (override alt-tab pause)
if not Internals.IsGameUpdating() then
    Internals.ToggleGameUpdates()
end

-- Select sarge, create squad 1, add him, select squad, jump to it
local sarge = MapObjManager.GetObjectByName("army.unit.sarge")
if sarge then
    Client.Events.SelectUnit(sarge, 1)
    SquadManager.Create(1)
    SquadManager.Add(1)
    SquadManager.Select(1)
    SquadManager.JumpTo(1)
end

-- Move all green dumdum units to attack a grunt
MapObjManager.Update()
local dumdums = MapObjManager.GetObjectsByName("army.unit.dumdum")
for i, dumdum in ipairs(dumdums) do
    if dumdum:GetTeam() ``` "green" then
        Client.Events.SelectUnit(dumdum, 1)
        local grunt = MapObjManager.GetObjectByName("army.unit.grunt")
        if grunt then
            local x, y, z = grunt:GetPosition()
            Orders.Game.Move.Generate(x, y, z, true, 1)
        end
    end
end
```

---

## Notes

- `MapObjManager.Update()` must be called before accessing objects to refresh the object list.
- All coordinates are in game world units.
- Hex strings for CRCs can be passed as `"0x12EA8886"` or any format `strul` understands.
- The `sleep()` function blocks the script - use sparingly.

For more examples, see the `examples/` folder.
