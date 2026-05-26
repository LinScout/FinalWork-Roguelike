# Список фич для защиты (Gameplay)

## Лёгкий уровень
- [x] 3 локации: Forest → Cave → Castle
- [x] `LevelManager` + `ILevel` / `GameplayLevel`
- [x] Портал выхода (trigger) — переход на следующую карту
- [x] Forest: выход без босса (демо лёгкого уровня)

## Средний уровень
- [x] Босс на Cave и Castle
- [x] Выход закрыт до победы над боссом
- [x] `BossAbilityComponent`: Rush, Heal, PowerStrike

## Сложный уровень
- [x] `Inventory` (map предметов)
- [x] Подбор предметов с карты
- [x] Использование: 1 зелье, 2 ключ, 3 меч
- [x] Связь с HP (`CombatStatsComponent`) и уроном (`AttackComponent`)

## Инфраструктура
- [x] `GameSession` — центр игровой логики
- [x] HUD + консольный вывод инвентаря
- [x] README, MR-описания, Release/Debug сборка
