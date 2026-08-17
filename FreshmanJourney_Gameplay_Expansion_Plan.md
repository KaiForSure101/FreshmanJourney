# Freshman Journey — Gameplay Expansion Plan

## 1. Project Direction

Freshman Journey is a C++ console-based campus life simulation game. The existing project already has semester progression, daily activities, random events, exams, player stats, endings, and save/load support.

The main gameplay problem is that the current loop is too simple:

> Choose one activity → resolve it → random event → next day.

The goal of this expansion is to turn the game into a more interactive simulation where the player actually **moves through campus, manages time, and decides how to spend each day**.

The core design principle is:

> **The player isn't simply choosing what happens today. The player is living through the day and deciding how to spend their limited time.**

---

## 2. New Core Gameplay Loop

The new gameplay loop should become:

> Day → Time → Explore → Perform actions → Manage resources → Events → Continue until bedtime → Next day

A typical day might look like:

- 08:00 — Wake up
- 08:30 — Eat breakfast
- 09:00 — Travel to class
- 09:20 — Attend class
- 10:50 — Study
- 11:50 — Lunch
- 12:20 — Travel to library
- 12:40 — Study
- 14:00 — Attend another class
- 15:30 — Club activity
- 17:30 — Dinner
- 18:00 — Part-time work
- 22:00 — Return to dorm
- 22:30 — Sleep

The player can therefore perform multiple activities during one day.

---

## 3. Campus Map

The game will introduce an ASCII-based campus map.

Possible locations:

- Dorm
- Classroom
- Library
- Gym
- Student Center
- Cafeteria

Example concept:

```text
+------------------------------------------------+
|                 CAMPUS MAP                     |
+------------------------------------------------+
|                                                |
|   [D] Dorm ---- [ ] -------- [L] Library      |
|     |             |             |              |
|    [ ]           [ ]           [ ]             |
|     |             |             |              |
|   [G] Gym ---- [S] Student -- [C] Classroom   |
|                                                |
|              [F] Cafeteria                    |
|                                                |
+------------------------------------------------+
```

The player should have a position on the map, represented internally by coordinates such as:

```cpp
player.x
player.y
```

The map can initially be represented using:

```cpp
vector<string> map;
```

Movement should use simple console controls such as:

- W — Up
- A — Left
- S — Down
- D — Right

The map should include collision so the player cannot walk through walls or inaccessible areas.

---

## 4. Time System

Time becomes one of the central gameplay mechanics.

Instead of one activity consuming an entire day, every action consumes a specific amount of time.

Example durations:

| Action | Duration |
|---|---:|
| Walking | 10–20 minutes |
| Attend class | 90 minutes |
| Study | 60 minutes |
| Eat | 30 minutes |
| Exercise | 60 minutes |
| Relax | 60 minutes |
| Club activity | 120 minutes |
| Part-time work | 240 minutes |
| Sleep | 480 minutes |

Internally, time should preferably be represented as minutes from midnight.

Examples:

```text
08:00 = 480
12:00 = 720
18:00 = 1080
22:00 = 1320
```

The system can therefore simply advance time:

```cpp
currentTime += actionDuration;
```

The game should convert the internal value into a readable format such as:

```text
08:30 AM
12:15 PM
06:30 PM
11:00 PM
```

---

## 5. Day Structure

The game can conceptually divide a day into:

- Morning — 06:00–12:00
- Afternoon — 12:00–17:00
- Evening — 17:00–22:00
- Night — 22:00–06:00

These periods should mainly help with schedules and events rather than strictly restricting all activities.

For example, certain campus events may only occur in the evening.

---

## 6. Location-Based Activities

Actions should depend on the player's current location.

### Dorm

Possible actions:

- Sleep
- Study
- Rest
- Change clothes
- Check inventory

### Classroom

Possible actions:

- Attend class
- Talk to classmates
- Review notes

### Library

Possible actions:

- Study
- Research
- Read
- Meet study group

### Gym

Possible actions:

- Exercise
- Train
- Rest

### Cafeteria

Possible actions:

- Eat
- Buy food
- Talk to friends

### Student Center

Possible actions:

- Join club
- Attend event
- Socialize

This makes the map an actual gameplay system rather than visual decoration.

---

## 7. Existing Player Stats

The existing stat system should remain and become more meaningful.

Current core stats include:

- Health
- Energy
- Stress
- Happiness
- Knowledge
- Money
- GPA

Examples of how actions can affect them:

### Studying

- Knowledge increases
- GPA potential increases
- Energy decreases
- Stress may increase
- Time decreases

### Exercise

- Health increases
- Energy decreases
- Stress decreases
- Time decreases

### Part-Time Work

- Money increases
- Energy decreases
- Stress may increase
- Time decreases

### Socializing

- Happiness increases
- Stress decreases
- Money may decrease
- Time decreases

The existing resource-management system should therefore remain underneath the new exploration and time-management layer.

---

## 8. Class Schedule

A schedule system should be introduced after the basic time and map systems work.

Example:

```text
MONDAY

09:00 - 10:30  Mathematics
11:00 - 12:30  Programming
14:00 - 15:30  English
```

Classes should have:

- Location
- Start time
- End time
- Subject
- Knowledge/GPA effects

The player should need to physically reach the appropriate location before class.

If the player is too far away, travel time may cause them to miss the class.

This creates meaningful decisions between:

- Attending class
- Studying
- Working
- Exercising
- Socializing
- Resting

---

## 9. NPC and Relationship System

NPCs can be added after the core map/time systems are stable.

NPC data may include:

```cpp
name
location
relationship
schedule
```

Possible interactions:

- Talk
- Study together
- Eat together
- Invite somewhere
- Help with assignment

Relationships can eventually unlock special events and interactions.

The NPC system should remain simple at first. Sophisticated AI is not required.

---

## 10. Contextual Events

The existing random campus event system should be upgraded rather than discarded.

Instead of events feeling completely random, events can depend on:

- Current location
- Current time
- Player stats
- Day of the semester
- Previous choices
- Relationships

Example:

> You are studying in the library at 8 PM. A classmate approaches you.

Possible choices:

1. Help them
2. Ignore them
3. Ask them for help instead

This will make events feel connected to the player's actual actions.

---

## 11. Project Architecture

The project already uses separate systems for major responsibilities. The new features should continue this approach instead of putting everything into `Game.cpp`.

Potential future structure:

```text
src/
├── main.cpp
├── Game.cpp
├── Player.cpp
├── Map.cpp
├── TimeSystem.cpp
├── Location.cpp
├── Activity.cpp
├── Schedule.cpp
├── Event.cpp
├── NPC.cpp
├── Menu.cpp
├── SaveManager.cpp
└── Utility.cpp

include/
├── Game.h
├── Player.h
├── Map.h
├── TimeSystem.h
├── Location.h
├── Activity.h
├── Schedule.h
├── Event.h
├── NPC.h
├── Menu.h
└── SaveManager.h
```

This is the target direction, not a requirement to create every file immediately.

---

# 12. Implementation Phases

## Phase 1 — Time System

First implement:

- Current day
- Current time
- Action duration
- Time advancement
- Day transition
- Sleep
- Basic time display

This should be completed and tested before adding the map.

---

## Phase 2 — Map and Movement

Implement:

- ASCII map
- Player coordinates
- W/A/S/D movement
- Collision
- Location detection
- Basic map display

The player should be able to physically travel around campus.

---

## Phase 3 — Location-Based Actions

Connect activities to locations.

For example:

```text
Library
  ├── Study — 60 min
  ├── Read — 30 min
  └── Study Group — 90 min
```

The action should advance the clock and modify player stats.

---

## Phase 4 — Class Schedule

Implement:

- Class schedules
- Class locations
- Start/end times
- Attendance
- Missed classes
- GPA/knowledge effects

This should create the first major time-management decisions.

---

## Phase 5 — NPCs and Social System

Implement:

- NPCs
- NPC schedules
- Relationships
- Dialogue/interactions
- Social activities

---

## Phase 6 — Contextual Events

Upgrade the existing random events so that they consider:

- Location
- Time
- Stats
- Previous decisions
- Relationships

---

## Phase 7 — Polish

Final improvements may include:

- Better ASCII UI
- Improved map rendering
- Cleaner menus
- Better feedback when time advances
- Optional console colors
- More varied events
- More meaningful endings
- Complete save/load support for the new systems

---

# 13. Important Development Rule

Do **not** rebuild the entire project from scratch.

The current project already has:

- Semester progression
- Daily gameplay
- Multiple activities
- Random events
- Exams
- GPA progression
- Player stats
- Save/load
- Multiple endings
- Successful CMake build

The new system should evolve the existing project.

The existing stat and semester systems should remain useful while the new gameplay layer adds:

```text
MAP
  ↓
LOCATION
  ↓
TIME
  ↓
ACTION
  ↓
STATS / MONEY / GPA
  ↓
EVENTS / NPCS
  ↓
SEMESTER OUTCOME
```

The goal is to transform Freshman Journey from a simple daily stat simulator into a **time-management campus life simulation with exploration and meaningful choices**.
