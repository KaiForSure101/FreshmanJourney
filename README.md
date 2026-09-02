# Freshman Journey

Freshman Journey is a console-based campus life simulation game written in C++.
Players manage daily activities, attend classes, handle random campus events, and try to complete the semester with a strong GPA and good wellbeing.

## Gameplay and Functionality (Detailed)

### Overview
Freshman Journey simulates a single semester of a college freshman's life. Each in-game day the player chooses how to spend time, balances schoolwork and personal needs, attends scheduled classes, and responds to random campus events. The objective is to finish the semester with a high GPA while maintaining physical and mental well-being.

### Core Loop
- Start the day (morning) and receive the day-of-week, scheduled classes, and any outstanding tasks (assignments, club meetings).
- Choose from available activities for time blocks (study, attend class, exercise, socialize, work part-time, sleep, etc.).
- Attend classes at scheduled times — participating may affect attendance, knowledge, and grades.
- Handle random events that can interrupt plans (pop quizzes, roommate conflicts, campus announcements, mini-quests).
- End the day to update stats, progress assignments, recover stamina, and advance the calendar.
- Repeat until the semester ends; final GPA and wellbeing determine the outcome.

### Player Stats and Resources
- GPA / Grades: Course-specific scores determine the semester GPA. Performance depends on attendance, study time, and class participation.
- Knowledge: Temporary metric for class performance that increases with studying and attending lectures.
- Energy / Stamina: Decreases with activities; low energy reduces effectiveness in study and class.
- Happiness / Wellbeing: Affected by social activities, rest, and events. Low wellbeing can cause penalties (missed classes, lower studying efficiency).
- Money: Earned from part-time jobs or events; used for purchases (food, textbooks, event tickets) if implemented.

### Activities and Their Effects
- Attend Class: Required for some classes to keep attendance up. Increases knowledge slightly and may unlock class-specific events.
- Study: Increases knowledge of chosen subject; more effective when energy is high.
- Do Assignments: Reduces outstanding workload and directly influences course grades if completed before deadlines.
- Sleep / Rest: Restores energy and slowly improves wellbeing.
- Socialize / Clubs: Improves wellbeing and may form relationships or unlock side quests.
- Exercise: Boosts wellbeing and long-term energy recovery.
- Part-time Job: Earns money but reduces energy and available study time.

Each activity consumes one or more time blocks; some have success chances or variable outcomes.

### Classes, Assignments, and Exams
- Courses have schedules (lecture days/times) and deliverables (homework, projects, midterms, final exam).
- Attendance, study, and assignment completion impact course grade components (participation, homework, exams).
- Exams are multi-day events (midterm/final) where accumulated knowledge is tested. Failing to prepare will lower the grade.

### Random Campus Events
- Random events trigger at various times (pop quiz, lost item, roommate needs help, a campus fair).
- Events may present choices with trade-offs (help a friend and miss study time, or prioritize studying and lose goodwill).
- Outcomes influence stats, relationships, or open temporary opportunities (extra credit, part-time gigs).

### Progression and End Conditions
- The semester runs for a fixed number of in-game days. When the calendar ends, final grades are calculated and presented.
- Achievements or endings depend on GPA, wellbeing, money, and optional objectives (join a club, maintain friendships).

### Save / Load (If implemented)
- The game may support saving the current state (player stats, calendar, assignments) to a file and loading it later.
- Save files include timestamp, player profile, and progress information.

### Controls and UI
- Console input driven by numbered menus and typed commands.
- Each day presents an ordered list of choices. Input the number or command to select an action.
- Important prompts (confirming an important choice, quitting, saving) require explicit confirmation.

### Tips for Players
- Balance short-term needs (study for an upcoming quiz) with long-term wellbeing (rest and social time).
- Prioritize assignments with approaching deadlines; they often have a strong effect on final grades.
- Use part-time work sparingly early in the semester to avoid burning out.

## Build Instructions

1. Install a C++ compiler and CMake.
2. Run the following commands from the project root:

```powershell
cmake -S . -B build
cmake --build build
```

3. Run the executable from the build directory:

```powershell
.\build\FreshmanJourney.exe
```

## Contributing
Contributions are welcome. To propose changes, open an issue or submit a pull request with a clear description of the feature or bug fix. Follow the existing code style and add tests if applicable.


--
Freshman Journey is developed as a learning project. Enjoy exploring campus life and experimenting with game mechanics!
