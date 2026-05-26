# Merge Requests — XYZ Roguelike Gameplay

Рекомендуемое разбиение фич по MR для защиты проекта.

---

## MR-1: Система уровней и переходов (лёгкий уровень)

**Что сделано:**
- `ILevel`, `LevelManager`, `GameplayLevel`
- Локации: `ForestLocation`, `CaveLocation`, `CastleLocation`
- Портал выхода (`LevelExit` trigger) — загрузка следующей сцены
- `GameSession` — единая точка входа игровой логики

**Зачем:** отделить сценарий уровня от движка, упростить добавление новых карт.

---

## MR-2: Боссы и уникальные способности (средний уровень)

**Что сделано:**
- Класс `Boss` (HP 120, броня 8)
- `BossAbilityComponent` в движке: Rush, Heal, PowerStrike
- Выход на Cave/Castle заблокирован до победы над боссом

**Зачем:** геймплейная цель на карте и разнообразие боёв.

---

## MR-3: Инвентарь (сложный уровень)

**Что сделано:**
- `Inventory` — map предметов, add/remove/use
- Предметы: HealthPotion, Key, Weapon, SpeedPotion (зарезервирован)
- Подбор с карты (trigger), использование клавишами 1/2/3
- Связь с `CombatStatsComponent` и `AttackComponent`

**Зачем:** мета-прогрессия и тактический выбор без обязательного UI.

---

## MR-4: HUD, логирование, документация

**Что сделано:**
- Расширенный `CombatHud` (локация, босс, инвентарь)
- `GameLog` / `game_log.txt`
- README, Release-сборка

---
