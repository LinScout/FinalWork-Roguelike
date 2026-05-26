# Инструкция по сборке релизной версии XYZ Roguelike

## Шаг 1: Настройка конфигурации Release

1. Откройте проект `Game.sln` в Visual Studio 2022
2. В верхней панели измените конфигурацию с **Debug** на **Release**
3. Убедитесь что платформа установлена на **x64**

## Шаг 2: Проверка настроек проекта

### XYZEngine (Release x64):

1. Правый клик на проект **XYZEngine** → Properties
2. Configuration Properties → C/C++ → Preprocessor
   - Убедитесь что есть: `SFML_STATIC;NDEBUG;_LIB`
3. Configuration Properties → C/C++ → Code Generation
   - Runtime Library: **Multi-threaded (/MT)**
4. Нажмите OK

### XYZRoguelike (Release x64):

1. Правый клик на проект **XYZRoguelike** → Properties
2. Configuration Properties → C/C++ → Preprocessor
   - Убедитесь что есть: `SFML_STATIC;NDEBUG;_CONSOLE`
3. Configuration Properties → C/C++ → Code Generation
   - Runtime Library: **Multi-threaded (/MT)**
4. Configuration Properties → Linker → System
   - SubSystem: **Windows (/SUBSYSTEM:WINDOWS)** (чтобы скрыть консоль)
5. Configuration Properties → Linker → Input
   - Additional Dependencies должны включать:
     ```
     XYZEngine.lib
     sfml-main.lib
     sfml-graphics-s.lib
     sfml-window-s.lib
     sfml-system-s.lib
     sfml-audio-s.lib
     ws2_32.lib
     opengl32.lib
     freetype.lib
     winmm.lib
     gdi32.lib
     openal32.lib
     flac.lib
     vorbisenc.lib
     vorbisfile.lib
     vorbis.lib
     ogg.lib
     ```
6. Нажмите OK

## Шаг 3: Сборка проекта

1. Build → Clean Solution (очистка старых файлов)
2. Build → Rebuild Solution (полная пересборка)
3. Дождитесь завершения сборки (должно быть "Build succeeded")

## Шаг 4: Поиск .exe файла

После успешной сборки файл будет находиться в:
```
XYZRoguelike\x64\Release\XYZRoguelike.exe
```

## Шаг 5: Подготовка папки для распространения

Создайте папку `XYZRoguelike_Release` и скопируйте туда:

1. **XYZRoguelike.exe** из `XYZRoguelike\x64\Release\`
2. Папку **Resources** (целиком) из корня проекта
   - Resources/Fonts/
   - Resources/Sounds/
   - Resources/Textures/
   - Resources/TextureMaps/

Структура должна быть:
```
XYZRoguelike_Release/
├── XYZRoguelike.exe
└── Resources/
    ├── Fonts/
    ├── Sounds/
    ├── Textures/
    └── TextureMaps/
```

## Шаг 6: Тестирование

1. Запустите `XYZRoguelike.exe` из папки `XYZRoguelike_Release`
2. Проверьте что:
   - Игра запускается без ошибок
   - Загружаются все текстуры
   - Работает звук
   - Управление работает корректно
   - При смерти игрок возрождается на 1 уровне

## Возможные проблемы и решения

### Ошибка: "MSVCP140.dll not found"
**Решение:** Установите Visual C++ Redistributable 2015-2022:
https://aka.ms/vs/17/release/vc_redist.x64.exe

### Ошибка: "Cannot find Resources folder"
**Решение:** Убедитесь что папка Resources находится рядом с .exe файлом

### Консоль не скрывается
**Решение:** Проверьте что SubSystem установлен в Windows, а не Console

### Игра не запускается
**Решение:** 
1. Проверьте что Runtime Library установлена в /MT
2. Убедитесь что все SFML библиотеки статически слинкованы
3. Проверьте Event Viewer Windows для деталей ошибки

## Создание установщика (опционально)

Для создания профессионального установщика можно использовать:
- **Inno Setup** (бесплатно): https://jrsoftware.org/isinfo.php
- **NSIS** (бесплатно): https://nsis.sourceforge.io/
- **Advanced Installer** (платно): https://www.advancedinstaller.com/

## Архивирование для распространения

Создайте ZIP архив:
```
XYZRoguelike_v1.0.zip
├── XYZRoguelike.exe
├── Resources/
├── README.txt
└── CONTROLS.txt
```

Где:
- **README.txt** - описание игры
- **CONTROLS.txt** - управление (WASD, Space, ESC)
