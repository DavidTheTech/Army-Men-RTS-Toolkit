# Army Men RTS Lua API Reference

This document describes the Lua API available for scripting in Army Men RTS. All functions are exposed via the game's Lua engine.

## Table of Contents
- [MapObjManager](#mapobjmanager)
- [MapObject](#mapobject)
- [Client.Events](#clientevents)
- [Orders.Game.Move](#ordersgamemove)
- [Console](#console)
- [VarSys](#varsys)
- [Log.Client](#logclient)
- [Internals](#internals)
- [Multiplayer.Data](#multiplayerdata)
- [SquadManager](#squadmanager)
- [Terrain](#terrain)
- [Team](#team)
- [amMath](#ammath)
- [Utilities](#utilities)

---

## MapObjManager

Manages world objects. Always call `Update()` before accessing objects.

| Function | Description |
|----------|-------------|
| `Update()` | Updates the internal object list (call before accessing objects) |
| `GetObjectByID(int)` | Returns the object with the given ID, or nil |
| `GetObjectByName(string)` | Returns the first object matching the name, or nil |
| `GetObjectsByName(string)` | Returns a table of all objects with the given name |
| `GetObjectsByTeam(string)` | Returns a table of all objects belonging to the specified team |
| `GetResourceObjects()` | Returns a table of all objects that are resource nodes |
| `Objects()` | Returns a table of all objects currently in the world |

**Example:**
```lua
MapObjManager.Update()
local sarge = MapObjManager.GetObjectByName("army.unit.sarge")
if sarge then
    print(sarge::GetName())
end
```

---

## MapObject

Represents a single object in the world. Methods are called on object instances.

| Function | Description |
|----------|-------------|
| `GetID()` | Returns the object's unique ID (number) |
| `GetName()` | Returns the object's name (string) |
| `GetTeam()` | Returns the team name (string) |
| `GetHitpoints()` | Returns current hitpoints (number) |
| `GetTotalHitpoints()` | Returns maximum hitpoints (number) |
| `SetHitpoints(int)` | Sets current hitpoints |
| `GetPosition()` | Returns x, y, z coordinates (three numbers) |
| `SetPosition(x, y, z)` | Sets position (numbers) |
| `GetX()`, `GetY()`, `GetZ()` | Returns individual coordinate |
| `SetX(x)`, `SetY(y)`, `SetZ(z)` | Sets individual coordinate |
| `IsResource()` | Returns true if the object is a resource node (boolean) |
| `GetResourceCount()` | Returns the amount of resource available (number) |

**Example:**
```lua
local unit = MapObjManager.GetObjectByName("army.unit.sarge")
if unit then
    local x, y, z = unit::GetPosition()
    print(string.format("Sarge at: %.2f, %.2f, %.2f", x, y, z))
    unit::SetHitpoints(500)
end
```

---

## Client.Events

Functions for sending UI and game events.

| Function | Description |
|----------|-------------|
| `SelectUnit(unit, modifier)` | Selects a unit (modifier: 0=default, 1, 2, or 3) |
| `CreateParticle(x, y, z, string)` | Creates particle effect at position (string like `"Client::Move"`) |
| `CreateParticleCrc(x, y, z, hexString)` | Creates particle effect using a CRC hex string (e.g., `"0x12EA8886"`) |
| `UpdateSelectedLists()` | Updates the internal selected unit lists |
| `TriggerResponse(hexString)` | Triggers response event (hex string like `"0x3381FB36"`) |
| `TriggerClientMode(mode)` | Triggers client mode (mode: 0) |

**Example:**
```lua
local unit = MapObjManager.GetObjectByName("army.unit.sarge")
if unit then
    Client.Events.SelectUnit(unit, 1)
    local x, y, z = unit::GetPosition()
    for i = 1, 20 do
        Client.Events.CreateParticle(x, y, z, "Client::Attack")
        Client.Events.CreateParticleCrc(x, y, z, "0x12EA8886")
        sleep(100)
    end
end
```

---

## Orders.Game.Move

Movement orders for selected units.

| Function | Description |
|----------|-------------|
| `Generate(x, y, z, attack, modifier, a5)` | Moves selected units to position. `attack`: boolean, `modifier`: integer (1, 2, 3, or default 8), `a5`: optional integer (defaults to 1) |

**Example:**
```lua
-- Move all green dumdum units to attack a grunt
MapObjManager.Update()
local dumdums = MapObjManager.GetObjectsByName("army.unit.dumdum")
for i, dumdum in ipairs(dumdums) do
    if dumdum::GetTeam() == "green" then
        Client.Events.SelectUnit(dumdum, 1)
        local grunt = MapObjManager.GetObjectByName("army.unit.grunt")
        if grunt then
            local x, y, z = grunt::GetPosition()
            Orders.Game.Move.Generate(x, y, z, true, 1)
        end
    end
end
```

---

## Console

Console command and output functions.

| Function | Description |
|----------|-------------|
| `ProcessCmd(string)` | Executes a console command |
| `Message(string)` | Prints a message to the console |
| `SetType(hexString)` | Sets console output type (e.g., `"0xB8B548C0"`) |

**Example:**
```lua
Console.Message("Hello from Lua!")
Console.ProcessCmd("coregame.createobj army.unit.scorch 514 14 202")
```

---

## VarSys

Variable system commands.

| Function | Description |
|----------|-------------|
| `CreateCmd(string)` | Creates a VarSys command |

---

## Log.Client

Logging functions.

| Function | Description |
|----------|-------------|
| `Write(string)` | Writes a message to the client log |

**Example:**
```lua
Log.Client.Write("Script started")
```

---

## Internals

Internal game state overrides (bypass pause on alt-tab).

| Function | Description |
|----------|-------------|
| `ToggleGameUpdates()` | Toggles game update override. Returns status string. |
| `IsGameUpdating()` | Returns true if game is updating (boolean) |
| `GetGameState()` | Returns three booleans: `updating`, `bit1State`, `activeState` |
| `Keybinds.Bind(keys, fn)` | Binds a key combination to a callback function (e.g., `"ctrl x"`) |

**Example:**
```lua
if not Internals.IsGameUpdating() then
    Internals.ToggleGameUpdates()
end

local updating, bit1, active = Internals.GetGameState()
print(string.format("Updating: %s, Bit1: %s, Active: %s", updating, bit1, active))

-- Bind Ctrl+X to regenerate the terrain
Internals.Keybinds.Bind("ctrl x", function()
    perlinTerrain()
end)
```

---

## Multiplayer.Data

Multiplayer data sending.

| Function | Description |
|----------|-------------|
| `Send(string)` | Sends a string over multiplayer |

**Example:**
```lua
Multiplayer.Data.Send("Hello minty!!!")
```

---

## SquadManager

Squad management (1–4).

| Function | Description |
|----------|-------------|
| `Create(squadNumber)` | Creates a squad (1–4) |
| `Add(squadNumber)` | Adds selected units to squad (1–4) |
| `Select(squadNumber)` | Selects squad (1–4) |
| `JumpTo(squadNumber)` | Jumps camera to squad (1–4) |

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

## Terrain

Terrain manipulation functions. Uses two coordinate systems:
- **Cell coordinates**: Grid-based system where each cell represents a terrain unit (e.g., 124 cells width/height). Cluster size is typically 1/4 of cell count (e.g., 31 clusters for 124 cells).
- **World coordinates**: Standard object coordinates (x, y, z). World units = cell index × 8.

| Function | Description |
|----------|-------------|
| `CellWidth()` | Returns the number of cells on the X axis (e.g., 124) |
| `CellHeight()` | Returns the number of cells on the Z axis (e.g., 124) |
| `ClusWidth()` | Returns the number of clusters on the X axis (e.g., 31 for 124 cells) |
| `ClusHeight()` | Returns the number of clusters on the Z axis (e.g., 31 for 124 cells) |
| `FindFloor(x, z)` | Returns the Y coordinate (float) of the terrain at world position (x, z). Use when placing objects to ensure they sit on the ground. |
| `Generate(x, y, z)` | Sets the height of the terrain at cell coordinates (x, z) to the Y value |
| `SetRGB(x, z, r, g, b)` | Sets the RGB color overlay on the terrain at cell coordinates (x, z). r, g, b values are 0–255. |
| `Place(name, x, y, z, yaw?, pitch?, roll?)` | Places a named object at world coordinates. Optional rotation angles in degrees. |
| `DeleteAll()` | Deletes all objects on the map |
| `AddWaterC(x, z, y)` | Adds water at cluster coordinates (x, z). Y determines water height. |
| `AddWaterF(x, z, y, bool)` | Adds water at world coordinates (x, z). Y determines water height. |
| `Paint(x, z, material_type)` | Paints the material at cell coordinates (x, z). Accepts a material type integer or string. |
| `ActiveCountType()` | Returns the number of active material types on the current map |
| `Blend()` | Re-blends the entire terrain (recalculates material transitions) |
| `Save(version, name)` | Saves the terrain to `missions\terraingen\MapName+Ver` |


**Some prop names for `Place()`** (examples from scripts):

| Category | Names |
|----------|-------|
| Trees | `attic.prop.tree1`, `tree3`, `tree3b`, `tree3c`, `tree6` |
| Pine trees | `attic.prop.tree4`, `tree4b`, `tree5`, `tree5b` |
| Burnt trees | `attic.prop.treeburnt1` – `treeburnt4` |
| Bushes | `attic.prop.bush1a`, `bush1b`, `bush2a`, `bush2b` |
| Grass | `yard.prop.grass1_1`, `grass2_1`, `grass3_1` – `grass3_9`, `grass4_1`, `grass4_2` |
| Rocks | `yard.prop.rock1` – `rock6` |
| Graves | `attic.prop.gravecross1`–`3`, `gravemarker`, `gravemarker2`, `gravestone` |
| Structures | `yard.prop.tent`, `attic.prop.tent2`, `attic.prop.water_tower`, `attic.prop.windmill` |
| Dinosaurs | `yard.prop.trex`, `yard.prop.triceratops` |
| Resources (electric) | `resource.controller`, `resource.remotecontrol`, `resource.watchdigital`, `resource.flashlight` |
| Resources (plastic) | `resource.ball`, `resource.chewtoy`, `resource.captdan-lie`, `resource.planewreck`, `resource.watergun` |
| Resources (animal) | `resource.sheep`, `resource.sheeplie`, `resource.pig`, `resource.pigstand`, `resource.cow`, `resource.cowstand` |
| Resources (batteries) | `resource.9volt`, `resource.9volt-standing`, `resource.battery`, `resource.battery-sand` |
| Resources (VHS) | `resource.vhstape`, `resource.vhschef`, `resource.vhsblintz`, `resource.vhstape_stack` |
| Resources (aliens) | `resource.zorglie`, `resource.zorgside`, `resource.zorgsit` |
| Resources (peachies) | `resource.peacepeachy`, `resource.punkpeachy`, `resource.spacepeachy` |
| Resources (buildings) | `resource.blocko_building1`, `resource.blocko_building_damage`, `resource.blocko_towncentre` |
| Army buildings | `army.building.guardtower`, `army.building.bwpost`, `army.building.hq1` |
| Misc | `sandbox.prop.spintop`, `livingrm.prop.blocko_tree`, `attic.prop.burnt_grunt`, `attic.prop.burnt_grunt-2` |

**Example - place a random tree on the ground:**
```lua
local trees = {
    "attic.prop.tree1",
    "attic.prop.tree3",
    "attic.prop.tree3b",
    "attic.prop.tree3c",
    "attic.prop.tree6"
}
local x, z = 500, 500
local y = Terrain.FindFloor(x, z)
Terrain.Place(trees[math.random(#trees)], x, y - 20, z, math.random() * 360)
```

**Example - carve a crater:**
```lua
-- Digs a crater at cell coords (x, y) with given radius and amplitude
function crater(x, y, radius, amp)
    for tx = x - radius, x + radius do
        for ty = y - radius, y + radius do
            if tx > 0 and ty > 0 and tx < Terrain.CellWidth() and ty < Terrain.CellHeight() then
                tempTerrain[tx][ty].elevation = tempTerrain[tx][ty].elevation
                    - ((radius * (amp + 1)) - (dist(tx, ty, x, y) * amp))
            end
        end
    end
end
```

**Example - add water across the whole map:**
```lua
for x = 0, Terrain.CellWidth() * 8 do
    for y = 0, Terrain.CellHeight() * 8 do
        Terrain.AddWaterF(x, -30, y, true)
    end
end
```

**Example - paint, color, and blend terrain:**
```lua
-- Textures and RGB are set per-cell, then Blend() smooths transitions
Terrain.Paint(x, y, 1)          -- Dirt
Terrain.SetRGB(x, y, 245, 245, 100)
Terrain.Blend()
```

---

## Team

Functions for creating and managing team spawn regions.

| Function | Description |
|----------|-------------|
| `Create(name, x1, y1, x2, y2, teamNum)` | Creates a team spawn region. `name`: quoted region name string, `x1/y1`: world coordinate top-left corner, `x2/y2`: world coordinate bottom-right corner, `teamNum`: team identifier string (e.g. `"TeamNum1"`) |

**Example:**
```lua
-- Place team spawn regions for up to 8 players, spaced apart and near plastic resources
for i = 1, 8 do
    Team.Create(
        "\"team" .. i .. "-start\"",
        x * 8,       y * 8,
        (x * 8) + 8, (y * 8) + 8,
        "TeamNum" .. i
    )
    sleep(100)
end
```

> **Note:** The region name must be passed as a quoted string literal if you use a dash in the name, otherwise use `"team1Start"`, e.g. `"\"team1-start\""`. 
The region covers a single cell (8×8 world units) when using `x*8` and `(x*8)+8` as the two corners.

---

## amMath

Mathematical functions for terrain generation.

| Function | Description |
|----------|-------------|
| `PerlinNoise(x, y)` | Returns Perlin noise at (x, y). Output is a float between -1 and 1. |
| `PerlinNoiseScaled(x, y, scale)` | Returns scaled Perlin noise. `scale` controls frequency. |
| `SetPerlinSeed(seed)` | Sets the seed for Perlin noise. Use a positive integer. |

**Example - multi-layer Perlin terrain with temperature and humidity maps:**
```lua
amMath.SetPerlinSeed(os.time())

local pscale  = 50
local ampPower = 50
local pLayers  = 3
local pOffsetX = math.random() * 100
local pOffsetY = math.random() * 100

for x = 0, Terrain.CellWidth() do
    for y = 0, Terrain.CellHeight() do
        local elevation = 0
        for p = 1, pLayers do
            local tempAmp   = ampPower / p
            local tempScale = pscale / p
            local pVal = (amMath.PerlinNoise((x / tempScale) + pOffsetX,
                                             (y / tempScale) + pOffsetY) * tempAmp) - (tempAmp / 2)
            local aVal = amMath.PerlinNoise((x / 10) + pOffsetX, (y / 10) + pOffsetY) * 2
            elevation = (elevation + pVal) * aVal
        end
        Terrain.Generate(x, elevation - 20, y)
    end
end
```

---

## Utilities

Helper functions.

| Function | Description |
|----------|-------------|
| `sleep(ms)` | Sleeps for milliseconds |
| `testterrain()` | Renders terrain map to `NewTerrain.tga` |

**Example:**
```lua
sleep(1000)
testterrain()
```

---

## Full Example Script

The following script procedurally generates a full terrain using layered Perlin noise, places props and structures, adds water, and sets team spawn points. It is bound to **Ctrl+X**.

```lua
local tempTerrain = {}
local seaLevel    = -5
local plasticSources = {}

function placeTree(x, y, z)
    local trees = { "attic.prop.tree1", "attic.prop.tree3",
                    "attic.prop.tree3b", "attic.prop.tree3c", "attic.prop.tree6" }
    Terrain.Place(trees[math.random(#trees)], x * 8, z - 20, y * 8, math.random() * 360)
end

function placePine(x, y, z)
    local trees = { "attic.prop.tree4", "attic.prop.tree4b",
                    "attic.prop.tree5", "attic.prop.tree5b" }
    Terrain.Place(trees[math.random(#trees)], x * 8, z - 20, y * 8, math.random() * 360)
end

--distance between two 2-D points
function dist(x1, y1, x2, y2)
    local dx = x2 - x1
    local dy = y2 - y1
    return math.sqrt(dx * dx + dy * dy)
end

-- Carve a crater at cell (x, y)
function crater(x, y, radius, amp)
    for tx = x - radius, x + radius do
        for ty = y - radius, y + radius do
            if tx > 0 and ty > 0
               and tx < Terrain.CellWidth() and ty < Terrain.CellHeight() then
                tempTerrain[tx][ty].elevation = tempTerrain[tx][ty].elevation
                    - ((radius * (amp + 1)) - (dist(tx, ty, x, y) * amp))
            end
        end
    end
end

-- Main terrain generation, bound to Ctrl+X
function perlinTerrain()
    Terrain.DeleteAll()
    tempTerrain    = {}
    plasticSources = {}

    local ampPower  = 50
    local pscale    = 50
    local pLayers   = 3
    local seaDepth  = 30
    local mountainHeight = 30
    local terrainOffset  = -20
    local pOffsetX  = math.random() * 100
    local pOffsetY  = math.random() * 100
    amMath.SetPerlinSeed(os.time())

    -- Build elevation, temperature (t), and humidity (h) maps
    for x = 0, Terrain.CellWidth() do
        tempTerrain[x] = {}
        for y = 0, Terrain.CellHeight() do
            tempTerrain[x][y] = { elevation = 0, roughness = 0, h = 0, t = 0 }
            for p = 1, pLayers do
                local tempAmp   = ampPower / p
                local tempScale = pscale / p
                local pVal = (amMath.PerlinNoise((x / tempScale) + pOffsetX,
                                                 (y / tempScale) + pOffsetY) * tempAmp) - (tempAmp / 2)
                local aVal = amMath.PerlinNoise((x / 10) + pOffsetX, (y / 10) + pOffsetY) * 2
                tempTerrain[x][y].elevation  = (tempTerrain[x][y].elevation + pVal) * aVal
                tempTerrain[x][y].roughness  = aVal / 2
            end
        end
    end

    -- Apply elevation to terrain, color and paint each cell, place props
    for x = 0, Terrain.CellWidth() do
        for y = 0, Terrain.CellHeight() do
            local height  = tempTerrain[x][y].elevation
            local texture = 0   -- Base
            local r, g, b = 0, 0, 0

            if height <= seaLevel then
                r, g, b = 150, 150, 255
                texture = 1     -- Dirt
                tempTerrain[x][y].elevation = -seaDepth
            elseif height > mountainHeight then
                r, g, b = 250, 250, 250
                texture = 5     -- Hill2
            else
                r = 40
                g = 150
                b = 25
                -- Chance-based prop placement
                if math.random(1, 100) > 85 then
                    if math.random(1, 2) == 1 then
                        placePine(x, y, height)
                    else
                        placeTree(x, y, height)
                    end
                end
            end

            Terrain.Generate(x, height + terrainOffset, y)
            Terrain.Paint(x, y, texture)
            Terrain.SetRGB(x, y, r, g, b)
        end
    end

    -- Flood the map with water at y = -30
    for x = 0, Terrain.CellWidth() * 8 do
        for y = 0, Terrain.CellHeight() * 8 do
            Terrain.AddWaterF(x, -30, y, true)
        end
    end

    Terrain.Blend()
end

-- Bind Ctrl+X to regenerate terrain
Internals.Keybinds.Bind("ctrl x", function()
    perlinTerrain()
end)
```

---

## Notes

- `MapObjManager.Update()` must be called before accessing objects to refresh the object list.
- All coordinates are in game world units. Cell coordinates convert to world units by multiplying by 8 (e.g., cell 10 = world 80).
- Hex strings for CRCs can be passed as `"0x12EA8886"` or any format `strtoul` understands.
- The `sleep()` function blocks the script - use sparingly.
- For terrain operations, cell coordinates range from 0 to `CellWidth()-1` and 0 to `CellHeight()-1`.
- Cluster coordinates range from 0 to `ClusWidth()-1` and 0 to `ClusHeight()-1`.
- `Terrain.Place()` accepts optional rotation arguments: `yaw`, `pitch`, `roll` in degrees (not all props respect all axes).
- Objects placed at `y - 20` relative to elevation are a common offset to sink props flush to the ground.

For more examples, see the `examples/` folder.