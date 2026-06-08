---
name: reforger-enforce-syntax
description: Enforce Script compiler constraints, encoding rules, reserved keywords, entity deletion patterns, common API hallucinations, modded class pattern, and attribute decorators.
---

# Reforger Enforce Script -- Syntax & Compiler Reference

## Compiler Constraints (HARD RULES)

### 1. NO Ternary Operators

```enforce
// BROKEN -- Compiler error: "Broken expression"
int value = (condition) ? 10 : 20;

// CORRECT
int value;
if (condition)
    value = 10;
else
    value = 20;
```

### 2. NO `f` Suffix on Float Literals

```enforce
// BROKEN
float speed = 50.0f;

// CORRECT
float speed = 50.0;
```

### 3. Case Sensitivity

```enforce
// WRONG
entity.getOwner();

// CORRECT
entity.GetOwner();
```

### 4. No Generics

```enforce
// WRONG
array<T> GenericMethod<T>(T param) { }

// CORRECT
array<ref BaseClass> MethodName(BaseClass param) { }
```

### 5. `owned` is a Reserved Keyword

`owned` is a built-in type modifier. Using it as a variable/parameter/field name causes a compiler error.

```enforce
// BROKEN
bool owned = purchaseController.IsItemPermanentlyOwned(item);

// CORRECT
bool bAlreadyOwned = purchaseController.IsItemPermanentlyOwned(item);
```

### 6. String Concatenation

```enforce
// Prefer PrintFormat
PrintFormat("Player %1 has %2 points", playerName, score);

// NOT: string msg = "Player " + playerName + " has " + score + " points";
```

### 7. No `#include` or `import`

Enforce Script uses a flat global namespace. The engine resolves all types at compile time.

---

## File Encoding -- NO UTF-8 BOM, NO CRLF, Pure ASCII Only

> **CRITICAL:** All three encoding issues produce the same misleading error -- `"Broken expression (missing ';'?)"` at a **wrong line number**.

### 7a. No UTF-8 BOM

Files must not have a UTF-8 BOM (`EF BB BF`) at the start.

PowerShell strip:
```powershell
$file = "path\to\File.c"
$bytes = [System.IO.File]::ReadAllBytes($file)
if ($bytes[0] -eq 0xEF -and $bytes[1] -eq 0xBB -and $bytes[2] -eq 0xBF) {
    [System.IO.File]::WriteAllBytes($file, $bytes[3..($bytes.Length-1)])
}
```

### 7b. LF-only Line Endings -- CRLF is FATAL

Any `\r\n` anywhere causes cascading `"Broken expression"` errors. The reported line number is wrong.

**Symptom:** Error line does not move after fixing syntax at that line.

Detection:
```powershell
$bytes = [System.IO.File]::ReadAllBytes("File.c")
$crlf = 0
for ($i = 0; $i -lt $bytes.Length - 1; $i++) {
    if ($bytes[$i] -eq 0x0D -and $bytes[$i+1] -eq 0x0A) { $crlf++ }
}
"CRLF count: $crlf"
```

Fix:
```powershell
$path = "path\to\File.c"
$bytes = [System.IO.File]::ReadAllBytes($path)
$stripped = [System.Collections.Generic.List[byte]]::new($bytes.Length)
foreach ($b in $bytes) { if ($b -ne 0x0D) { $stripped.Add($b) } }
[System.IO.File]::WriteAllBytes($path, $stripped.ToArray())
```

### 7c. Pure ASCII Only -- No Non-ASCII Characters Anywhere

Non-ASCII (code points > U+007E) cause compiler errors in executable code. Comments may contain non-ASCII freely.

| Character | Safe replacement |
|-----------|-----------------|
| `->`(arrow U+2192) | `->` |
| `--` (em-dash U+2014) | `-` |
| `"` `"` (smart quotes) | `"` |
| `'` `'` (smart apostrophe) | `'` |

Detection:
```powershell
$bytes = [System.IO.File]::ReadAllBytes("File.c")
$nonAscii = ($bytes | Where-Object { $_ -gt 0x7E }).Count
"Non-ASCII bytes: $nonAscii"
```

The `enforce-code-linter.py` tool catches and auto-fixes all three issues with `--output fix`.

---

## Entity Deletion Patterns

### Local / preview entities (not replicated):
```enforce
// CORRECT for preview world entities and other non-networked local entities
SCR_EntityHelper.DeleteEntityAndChildren(localEntity);

// WRONG -- GetGame() has no DeleteEntity method
GetGame().DeleteEntity(entity);
```

### Networked (replicated) entities:
```enforce
// WRONG -- breaks replication
SCR_EntityHelper.DeleteEntityAndChildren(networkedEntity);

// CORRECT
RplComponent rpl = RplComponent.Cast(entity.FindComponent(RplComponent));
if (rpl)
    rpl.DeleteRplEntity(entity, false);
```

### CRITICAL -- Never delete certain entity types synchronously

> Calling `RplComponent.DeleteRplEntity` synchronously on the entity types below causes
> `SEH 0xc0000374` Windows heap corruption on the dedicated server. See `reforger-inventory`
> skill section **"CRITICAL -- Known Server Crash Catalogue"** for full root cause and fix.

**Entities that MUST be deleted via a deferred queue (never synchronously):**
- Weapons with under-barrel sub-weapons (`WeaponAttachmentsStorageComponent` children that are themselves `BaseWeaponComponent`)
- Any `BaseLoadoutClothComponent` entity (cloth items -- always unsafe regardless of slot state)
- Any entity with `BaseInventoryStorageComponent` AND occupied slots (backpacks with contents, helmets with NVG, vests with armor plates)

**Safe test before any forced delete:**
```enforce
protected bool IsUnsafeForcedDeleteEntity(IEntity entity)
{
    if (BaseLoadoutClothComponent.Cast(entity.FindComponent(BaseLoadoutClothComponent)))
        return true;
    if (BaseWeaponComponent.Cast(entity.FindComponent(BaseWeaponComponent)))
        return true;
    if (WeaponAttachmentsStorageComponent.Cast(entity.FindComponent(WeaponAttachmentsStorageComponent)))
        return true;
    BaseInventoryStorageComponent storage = BaseInventoryStorageComponent.Cast(entity.FindComponent(BaseInventoryStorageComponent));
    if (storage)
    {
        int slots = storage.GetSlotsCount();
        for (int i = 0; i < slots; i++)
        {
            InventoryStorageSlot slot = storage.GetSlot(i);
            if (slot && slot.GetAttachedEntity())
                return true;
        }
    }
    return false;
}
```

---

## Modded Class Override Pattern

```enforce
modded class SCR_CharacterDamageManagerComponent
{
    override void OnPostInit(IEntity owner)
    {
        super.OnPostInit(owner); // CRITICAL: preserve engine init
        GetOnDamage().Insert(OnCustomDamage);
    }

    void OnCustomDamage(notnull BaseDamageContext damageContext)
    {
        // Custom logic
    }
}
```

**Key rules:**
- Use `modded class` prefix, not `class`
- Always check if `super` call is needed
- Cannot modify `sealed class` -- find non-sealed parent

---

## Attribute Decorator Patterns

```enforce
class KOTH_ConfigComponent : ScriptComponent
{
    [Attribute("100", UIWidgets.EditBox, "Price to buy once")]
    protected int m_priceBuyOnce;

    [Attribute("5", UIWidgets.Slider, "Check interval (seconds)", "1 60 1")]
    protected float m_checkInterval;

    [Attribute("0", UIWidgets.CheckBox, "Enable auto-respawn")]
    protected bool m_bAutoRespawn;

    [Attribute(desc: "Item prefab", params: "et")]
    protected ResourceName m_itemPrefab;

    [Attribute("0", UIWidgets.ComboBox, "Category", "",
               ParamEnumArray.FromEnum(KOTH_ItemCategory))]
    protected KOTH_ItemCategory m_category;

    [Attribute("", UIWidgets.Object, category: "Effects")]
    protected ref SCR_ConsumableEffectBase m_effect;
}
```

Format: `[Attribute(defaultValue, UIWidget, description, params)]`

---

## ScriptComponent Class Pair -- REQUIRED

> **CRITICAL:** Every `ScriptComponent` subclass **must** be preceded by a paired `*Class`
> that inherits from `ScriptComponentClass`. Omitting it produces:
> `error: Missing Component Class for 'MyComponentClass' for Component 'MyComponent'`

```enforce
// BROKEN -- ComponentClass is missing
class KOTH_MyComponent : ScriptComponent { }

// CORRECT -- empty ComponentClass is sufficient
class KOTH_MyComponentClass : ScriptComponentClass {}

class KOTH_MyComponent : ScriptComponent { }
```

The `*Class` is the component's Workbench data-container descriptor. It must exist even when
empty. The engine derives the expected name by appending `Class` to the component class name.

The same rule applies when the `*Class` needs `[Attribute]` fields that are shared across
instances (class-level config, as opposed to per-instance `[Attribute]` on the component):

```enforce
class KOTH_VehicleArmorDegradeComponentClass : ScriptComponentClass {}

class KOTH_VehicleArmorDegradeComponent : ScriptComponent
{
    [Attribute("0.15", UIWidgets.EditBox, "Glass stress scale")]
    protected float m_fGlassStressScale;
    // ...
}
```

---

## Resource GUIDs & .meta Files

Every `.conf`/`.layout` file needs a `.meta` file with a unique GUID. `.c` files do NOT need `.meta` files.

**GUID format:** `{XXXXXXXXXXXXXXXX}` -- exactly 16 uppercase hex characters.

**CRITICAL:** A `.conf` and a `.layout` must NEVER share the same GUID.

**Meta file template:**
```
MetaFileClass {
 Name "{XXXXXXXXXXXXXXXX}Configs/Shop/ShopItems.conf"
 Configurations {
  CONFResourceClass PC {
  }
  CONFResourceClass XBOX_ONE : PC {
  }
  CONFResourceClass XBOX_SERIES : PC {
  }
  CONFResourceClass PS4 : PC {
  }
  CONFResourceClass PS5 : PC {
  }
  CONFResourceClass HEADLESS : PC {
  }
 }
}
```

**GUID in code:**
```enforce
private const string SHOP_CONFIG = "{F0DBA538DDEF4260}Configs/Shop/ShopItems.conf";
Resource configResource = BaseContainerTools.LoadContainer(SHOP_CONFIG);
```

**Generate random GUIDs:**
```powershell
python -c "import secrets; print('{' + secrets.token_hex(8).upper() + '}')"
```

**ANTI-PATTERN:** NEVER use sequential/patterned GUIDs. They WILL collide with other mods.

---

## Config Root Class Pattern

```enforce
[BaseContainerProps(configRoot: true)]
class KOTH_ShopItemConfig
{
    [Attribute(desc: "Item display name")]
    string m_displayName;

    [Attribute(desc: "Item prefab", params: "et")]
    ResourceName m_prefab;

    [Attribute("100", UIWidgets.EditBox, "Price")]
    int m_price;
}
```

---

## Common API Hallucinations (Avoid These)

| Hallucinated API | Actual Reforger API | Notes |
|:---|:---|:---|
| `Math.DiffAngle(a, b)` | `SCR_Math.DeltaAngle(a, b)` | Returns unsigned difference |
| `Math.Clamp01(x)` | `Math.Clamp(x, 0, 1)` | No dedicated 01 variant |
| `vector.Angle(a, b)` | `vector.Dot` + `Math.Acos` | No direct vector angle |
| `Math.RoundToInt(x)` | `(int)Math.Round(x)` | `Math.Round` returns float |
| `vector(x, y, z).Length()` | `Math.Sqrt(x*x + z*z)` | No `vector()` constructor |

> If you need a Math function, check `SCR_Math` or `SCR_Math2D` first. If not found, implement a private helper.

---

## Timing & Scheduling

```enforce
// One-time delayed call (1000ms)
GetGame().GetCallqueue().CallLater(MyMethod, 1000, false, param1, param2);

// Repeating call every 500ms
GetGame().GetCallqueue().CallLater(MyRepeatingMethod, 500, true);

// Cancel scheduled call
GetGame().GetCallqueue().Remove(MyMethod);
```

## Component Access Patterns

```enforce
IEntity entity = GetOwner();
KOTH_Component comp = KOTH_Component.Cast(entity.FindComponent(KOTH_Component));

// Cache on init
protected KOTH_Component m_cachedComp;

override void OnPostInit(IEntity owner)
{
    super.OnPostInit(owner);
    m_cachedComp = KOTH_Component.Cast(owner.FindComponent(KOTH_Component));
}
```

## Logging

```enforce
Print("Message here");
PrintFormat("Player %1 score: %2", name, score);
LogWorkbench("Workbench-only output");
```

## Authority Check Pattern

```enforce
void ProcessGameEvent()
{
    if (!Replication.IsServer())
        return;

    UpdateScores();
    CheckWinCondition();
}
```

---

## CRITICAL -- Never `modded class` a Data/Config Class That Is Deserialized from .conf

> **Bug confirmed in production (KOTH MISC Mod, April 2026).**

### Symptom
Kill feed (and any notification using `SCR_SplitNotificationUIInfo`) silently disappears.
Server log shows:
```
SCRIPT (E): (PLAYER_KILLED_PLAYER) SCR_NotificationDisplayData has no UIInfo assigned!
```
Followed immediately by the notification being destroyed with no display.

### Root Cause
`modded class SCR_SplitNotificationUIInfo` (or any modded class targeting a type that is
instantiated via `SCR_BaseContainerTools.CreateInstanceFromPrefab` / conf deserialization)
disrupts the engine's typename resolution during conf loading. The cast of the deserialized
`m_info` field back to `SCR_SplitNotificationUIInfo` fails silently, leaving `m_info = null`.
`SCR_NotificationDisplayData.GetText()` then hits the null guard, returns `string.Empty`, and
`SCR_NotificationMessageUIComponent.UpdateText()` calls `ForceRemoveNotification()` -- the
entry is destroyed before it renders.

### Rule
**Never use `modded class` on any class whose instances are created by conf deserialization**
(i.e., any class used as a field type inside a `[BaseContainerProps(configRoot: true)]`
config tree, or any class instantiated via `BaseContainerTools.CreateInstanceFromPrefab` or
`BaseContainerTools.CreateInstanceFromContainer`).

This includes:
- `SCR_SplitNotificationUIInfo`
- `SCR_UINotificationInfo` and all subclasses
- `SCR_NotificationDisplayData` and all subclasses
- Any `SCR_UIInfo` subclass used as a conf field type

### Correct Fix
Mod the **UI component** (the widget handler) instead of the data class:

```enforce
// BROKEN -- breaks conf deserialization of m_info, kills kill feed
modded class SCR_SplitNotificationUIInfo
{
    override ResourceName GetIconPath() { ... }
}

// CORRECT -- mod the widget component, not the data class
modded class SCR_SplitNotificationMessageUIComponent : SCR_NotificationMessageUIComponent
{
    override void Init(SCR_NotificationData data, SCR_NotificationsLogComponent notificationLog, float fadeDelay)
    {
        super.Init(data, notificationLog, fadeDelay);
        // custom logic here, e.g. skip the false imageset icon warning
    }
}
```
