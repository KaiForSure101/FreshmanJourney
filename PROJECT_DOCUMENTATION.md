# Freshman Journey

Freshman Journey is a C++17 console-based campus-life simulation game. The
player manages a 30-day semester by balancing academic progress, personal
wellbeing, finances, campus movement, scheduled classes, random events, and
exams.

## Contents

- [Project Overview](#project-overview)
- [Requirements](#requirements)
- [Building and Running](#building-and-running)
- [Gameplay Guide](#gameplay-guide)
- [Player State](#player-state)
- [Time and Day Progression](#time-and-day-progression)
- [Class Schedule and Attendance](#class-schedule-and-attendance)
- [Campus Map](#campus-map)
- [Activities and Events](#activities-and-events)
- [Exams and GPA](#exams-and-gpa)
- [Saving and Loading](#saving-and-loading)
- [Project Structure](#project-structure)
- [Architecture](#architecture)
- [Extending the Game](#extending-the-game)
- [Troubleshooting](#troubleshooting)
- [Development Notes](#development-notes)

## Project Overview

The game starts at a main menu with three options:

1. Start a new game
2. Load a saved game
3. Exit

A new game starts on Day 1 at 8:00 AM. Each turn lets the player sleep, explore
the campus, move to a location, or perform an activity available at the current
location. Activities consume time and modify player statistics.

The semester normally ends after Day 30. It may end earlier if the player's
health reaches zero, happiness reaches zero, or stress reaches 100.

## Requirements

- CMake 3.19 or newer
- A C++17-compatible compiler
- Windows, Linux, or macOS with a terminal

The current Visual Studio Code workspace is configured for CMake Tools. The
project has no external third-party libraries.

## Building and Running

Run these commands from the project root:

```powershell
cmake -S . -B build
cmake --build build --config Debug
```

On Windows, run the generated executable with:

```powershell
.\build\Debug\FreshmanJourney.exe
```

Depending on the generator, the executable may instead be placed directly at:

```powershell
.\build\FreshmanJourney.exe
```

The CMake target is named `FreshmanJourney`.

## Gameplay Guide

### Starting a game

Choose **New Game** to reset all player statistics, position, day, weekday,
and time. The player starts at the Dorm area.

Choose **Load Game** to restore the latest file at
`data/savegame.txt`. If the file is missing or invalid, the game returns to the
main menu.

### Main activity menu

The main menu currently provides:

- **Sleep**: advances to the next day and restores wellbeing.
- **Explore Campus**: opens the movement map.
- **Save and Exit**: writes the current player state and leaves the game loop.

Location-specific activities appear after the player reaches an interactable
campus location.

### Map controls

While in map mode:

- `W`: move up
- `A`: move left
- `S`: move down
- `D`: move right
- `Q`: leave map mode

Each successful movement costs 3 minutes. Walls and outside-map positions are
not walkable.

## Player State

The `Player` class owns the persistent game state.

| Statistic | Range or format | Purpose |
|---|---:|---|
| Health | 0-100 | Physical wellbeing; zero ends the run |
| Energy | 0-100 | Limits activities that consume energy |
| Stress | 0-100 | High stress is harmful; 100 ends the run |
| Happiness | 0-100 | Wellbeing; zero ends the run |
| Knowledge | 0-100 | Contributes to exam performance |
| Money | 0 or greater | Used by activities and events |
| GPA | 0.0-4.0 | Academic result and ending selection |
| Day | 1-30 | Current semester day |
| Day of week | 0-6 | Monday through Sunday |
| Time | Minutes since midnight | Current in-game time |
| Position | Map coordinates | Current campus tile |

All bounded statistics are clamped through `Player::clampStats()`. Money cannot
become negative and GPA is limited to the 0.0-4.0 range.

Initial values are:

```text
Health:    80
Energy:    80
Stress:    20
Happiness: 60
Knowledge: 20
Money:     $300
GPA:       0.0
Day:       1
Weekday:   Monday
Time:      08:00 AM
```

## Time and Day Progression

The `TimeSystem` stores time as an integer number of minutes and formats it for
display as a 12-hour clock.

- New games begin at 8:00 AM.
- Movement costs 3 minutes per tile.
- Activity durations are measured in minutes.
- Time wraps across midnight and advances the day.
- Sleeping always advances one day and resets the time to 8:00 AM.
- At 10:00 PM (1320 minutes), the game displays a warning and automatically
  sleeps after the current action.

Sleeping applies these changes:

```text
Health    +20
Energy    +30
Stress    -15
Happiness +5
```

The changes are clamped after they are applied.

The weekday sequence is:

```text
0 Monday
1 Tuesday
2 Wednesday
3 Thursday
4 Friday
5 Saturday
6 Sunday
```

## Class Schedule and Attendance

`Schedule` stores the weekly course list. Course times are represented as
minutes since midnight, and every scheduled course takes place in the
Classroom location.

| Day | Course | Start |
|---|---|---:|
| Monday | Mathematics | 9:00 AM |
| Monday | Programming | 11:00 AM |
| Monday | English | 2:00 PM |
| Tuesday | Physics | 9:00 AM |
| Tuesday | Mathematics | 1:00 PM |
| Wednesday | Programming | 9:00 AM |
| Wednesday | English | 11:00 AM |
| Wednesday | History | 2:00 PM |
| Thursday | Physics | 10:00 AM |
| Thursday | Mathematics | 1:00 PM |
| Friday | English | 9:00 AM |
| Friday | Programming | 11:00 AM |
| Friday | Art | 2:00 PM |

Saturday and Sunday have no scheduled classes. Biology is available in the
`CourseType` enumeration for future schedule expansion but is not currently
part of the default weekly schedule.

When the game loop checks the schedule:

- If a class is active and the player is in the Classroom, the course is
  attended once and grants its knowledge gain.
- If a class is active elsewhere, the game displays a missed-class warning.
- Attendance is stored per course and counted by `getAttendanceRate()`.
- Attendance is reset after the scheduled exam day.

The current exam attendance bonus is:

```text
attendanceBonus = attendanceRate * 0.1
```

An attendance rate of 100% therefore adds 0.1 to the exam score calculation.

## Campus Map

The map is an ASCII layout rendered with emoji or Unicode representations:

| Tile | Location |
|---|---|
| `#` | Wall |
| `.` | Campus path |
| `D` | Dorm |
| `L` | Library |
| `G` | Gym |
| `S` | Student Center |
| `C` | Classroom |
| `F` | Cafeteria |
| Player position | Player marker |

Rendered representations include:

- Player: `🧑`
- Wall: `██`
- Dorm: `🏠`
- Library: `📚`
- Gym: `💪`
- Student Center: `🎯`
- Classroom: `📖`
- Cafeteria: `🍽️`

The map uses a fixed layout defined in `Map.cpp`. The `Map` class translates
the tile under the player into a `LocationType`, which controls the available
activities.

## Activities and Events

### Location activities

Available activities depend on the current location:

- Dorm: Rest, Study
- Library: Study, Read, Deep Study
- Gym: Exercise, Heavy Training, Rest
- Cafeteria: Eat, Cheap Meal, Social Meal
- Student Center: Socialize, Relax, Club Activity
- Classroom: Review Notes, Study

Each activity has:

```text
name
description
duration
healthChange
energyChange
stressChange
happinessChange
knowledgeChange
moneyChange
gpaChange
```

An activity that requires more energy than the player currently has is rejected.
After an activity, the game advances time, clamps statistics, and prints a
summary.

### Random events

After actions, the game has approximately a 35% chance to trigger one random
event. Current events include:

- Surprise Quiz
- Club Invitation
- Rainy Day
- Scholarship Offer
- Lost Wallet
- Group Project
- Illness
- Campus Festival

Events modify the `Player` object through the callback stored in the `Event`
struct. Statistics are clamped after an event.

## Exams and GPA

An exam occurs every 7 days, including Day 7, Day 14, Day 21, and Day 28.

The score uses:

- Knowledge
- Energy
- Stress as a negative factor
- Attendance bonus
- Small random variation

The calculated GPA change is bounded between 0.0 and 0.4. Completing an exam
also increases stress, decreases energy, and may increase happiness according
to the GPA improvement.

At the end of the semester, the game selects an ending using GPA, money,
happiness, stress, and health:

- **Burned Out**: health is depleted or the run is not alive
- **Academic Failure**: GPA is below 1.5
- **Financial Crisis**: money is below $50
- **Honor Student**: GPA is at least 3.5 and happiness is at least 70
- **Balanced Student**: all other successful completions

## Saving and Loading

Save files are stored at:

```text
data/savegame.txt
```

The directory is created when a new game starts.

The current save fields are written one value per line:

```text
health
energy
stress
happiness
knowledge
money
gpa
day
dayOfWeek
currentTimeMinutes
x
y
```

The loader also accepts older save files that do not contain `dayOfWeek`. For
those files, the weekday is derived from the day number using Monday as day
zero. Save files should not be edited while the game is running.

Schedule attendance is session state and is not currently serialized. Loading a
save starts with a newly generated weekly schedule and no attendance records.

## Project Structure

```text
FreshmanJourney_versionX/
├── CMakeLists.txt
├── README.md
├── PROJECT_STATUS.md
├── PROJECT_DOCUMENTATION.md
├── include/
│   ├── Activity.h
│   ├── Event.h
│   ├── Game.h
│   ├── InputManager.h
│   ├── Map.h
│   ├── Menu.h
│   ├── Player.h
│   ├── SaveManager.h
│   ├── Schedule.h
│   ├── TimeSystem.h
│   └── Utility.h
├── src/
│   ├── Game.cpp
│   ├── Map.cpp
│   ├── Menu.cpp
│   ├── Player.cpp
│   ├── SaveManager.cpp
│   ├── Schedule.cpp
│   ├── TimeSystem.cpp
│   ├── Utility.cpp
│   └── main.cpp
├── data/
│   └── savegame.txt
└── build/
    └── generated CMake output
```

The `build/` directory is generated and should not be edited manually.

## Architecture

### `Game`

`Game` is the application coordinator. It owns the player, time system, map,
schedule, activities, and events. It is responsible for:

- Main menu flow
- New-game and load-game initialization
- The daily game loop
- Activity dispatch
- Schedule checks
- Day-end handling
- Random events
- Exams
- Save and final-result messages

### `Player`

`Player` stores mutable game state and provides controlled methods for applying
stat changes, clamping values, advancing days, and reading or setting
persistent fields.

### `TimeSystem`

`TimeSystem` operates on the player's current time. It handles minute
advancement, day rollover, sleep restoration, and formatted time output.

### `Map`

`Map` owns the fixed campus layout and player position integration. It validates
movement and identifies the current location.

### `Schedule`

`Schedule` owns the default weekly course data and attendance state. It provides
queries for daily courses, active classes, current class names, and attendance
rates.

### `Menu` and `InputManager`

`Menu` formats menus and asks for choices. `InputManager` validates integer
input and keeps invalid input from breaking the game loop.

### `SaveManager`

`SaveManager` provides static save and load functions. It serializes the
player's persistent state without coupling file-format details to the game
loop.

### `Utility`

`Utility` contains shared helper functionality, including random integer
generation used by events and exams.

## Extending the Game

### Add an activity

1. Open `Game.cpp`.
2. Add an `Activity` entry to the appropriate branch of
   `getActivitiesForLocation()`.
3. Set its duration and stat deltas.
4. Ensure the energy requirement and time cost produce the intended gameplay.

### Add an event

1. Open `Game::initEvents()`.
2. Add an `Event` with a name, description, and
   `std::function<void(Player&)>` callback.
3. Apply stat changes through `Player` methods.
4. Print a clear event result.

### Add a course

1. Add or reuse a `CourseType` in `Schedule.h`.
2. Add a `Course` initializer in `Schedule::generateWeeklySchedule()`.
3. Set the weekday using 0 for Monday through 6 for Sunday.
4. Use minutes since midnight for start and end times.
5. Keep the location set to `Classroom` unless location-aware classes are
   implemented.

### Add a map location

1. Add a tile character to the layout in `Map.cpp`.
2. Add a corresponding `LocationType` in `Map.h`.
3. Update `Map::getLocationType()`.
4. Update `Map::locationToString()`.
5. Add the rendered representation in `Map::render()`.
6. Add location activities in `Game::getActivitiesForLocation()`.

### Change semester rules

The semester length and exam interval are constants initialized in the
`Game` constructor:

```cpp
semesterLength = 30;
examInterval = 7;
```

Change these values carefully because they affect the main loop, exam timing,
and final-result conditions.

## Troubleshooting

### CMake cannot configure

Verify that CMake is installed and available on `PATH`:

```powershell
cmake --version
```

Delete only the generated `build/` directory if the CMake cache is stale, then
configure again.

### The executable is not found

The output path depends on the selected generator. Search under `build/` for
`FreshmanJourney.exe`, especially `build/Debug/`.

### Save or load fails

Check that:

- The process has write permission in the project directory.
- `data/` exists or can be created.
- `data/savegame.txt` contains numeric values in the expected order.

### Emoji display is incorrect

Use a terminal configured for UTF-8 and a font containing the required emoji
glyphs. The game remains playable if the terminal substitutes missing glyphs.

### Input appears stuck

The game expects menu numbers and single-letter map commands. Invalid numeric
input is cleared and requested again. In map mode, press `Q` to return to the
activity menu.

## Development Notes

- The project uses C++17 and does not require external runtime libraries.
- CMake source registration is maintained in `CMakeLists.txt`.
- There is currently no separate automated test suite; compilation is the
  primary validation step.
- Runtime save data belongs in `data/` and generated build artifacts belong in
  `build/`.
- Keep game rules in the relevant subsystem rather than duplicating them in
  menu code.
- When changing serialized fields, preserve backward-compatible loading where
  practical.
