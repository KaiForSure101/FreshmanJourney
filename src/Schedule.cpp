#include "Schedule.h"

#include <algorithm>

Schedule::Schedule() {
    generateWeeklySchedule();
}

void Schedule::generateWeeklySchedule() {
    courses = {
        {"Mathematics", CourseType::Mathematics, 9 * 60, 10 * 60, 0, "Classroom", 5, 1.0, false},
        {"Programming", CourseType::Programming, 11 * 60, 12 * 60, 0, "Classroom", 5, 1.0, false},
        {"English", CourseType::English, 14 * 60, 15 * 60, 0, "Classroom", 5, 1.0, false},
        {"Physics", CourseType::Physics, 9 * 60, 10 * 60, 1, "Classroom", 5, 1.0, false},
        {"Mathematics", CourseType::Mathematics, 13 * 60, 14 * 60, 1, "Classroom", 5, 1.0, false},
        {"Programming", CourseType::Programming, 9 * 60, 10 * 60, 2, "Classroom", 5, 1.0, false},
        {"English", CourseType::English, 11 * 60, 12 * 60, 2, "Classroom", 5, 1.0, false},
        {"History", CourseType::History, 14 * 60, 15 * 60, 2, "Classroom", 5, 1.0, false},
        {"Physics", CourseType::Physics, 10 * 60, 11 * 60, 3, "Classroom", 5, 1.0, false},
        {"Mathematics", CourseType::Mathematics, 13 * 60, 14 * 60, 3, "Classroom", 5, 1.0, false},
        {"English", CourseType::English, 9 * 60, 10 * 60, 4, "Classroom", 5, 1.0, false},
        {"Programming", CourseType::Programming, 11 * 60, 12 * 60, 4, "Classroom", 5, 1.0, false},
        {"Art", CourseType::Art, 14 * 60, 15 * 60, 4, "Classroom", 5, 1.0, false}
    };
}

std::vector<Course> Schedule::getCoursesForDay(int dayOfWeek) const {
    std::vector<Course> result;
    for (const Course& course : courses) {
        if (course.dayOfWeek == dayOfWeek) {
            result.push_back(course);
        }
    }
    return result;
}

bool Schedule::isClassActive(int currentTime, int dayOfWeek) const {
    return std::any_of(courses.begin(), courses.end(), [currentTime, dayOfWeek](const Course& course) {
        return course.dayOfWeek == dayOfWeek &&
               currentTime >= course.startTime &&
               currentTime < course.endTime;
    });
}

std::string Schedule::getCurrentClassName(int currentTime, int dayOfWeek) const {
    for (const Course& course : courses) {
        if (course.dayOfWeek == dayOfWeek &&
            currentTime >= course.startTime &&
            currentTime < course.endTime) {
            return course.name;
        }
    }
    return "";
}

void Schedule::markAttendance(int dayOfWeek, int courseIndex) {
    std::vector<int> matchingIndices;
    for (int index = 0; index < static_cast<int>(courses.size()); ++index) {
        if (courses[index].dayOfWeek == dayOfWeek) {
            matchingIndices.push_back(index);
        }
    }
    if (courseIndex >= 0 && courseIndex < static_cast<int>(matchingIndices.size())) {
        courses[matchingIndices[courseIndex]].attended = true;
    }
}

double Schedule::getAttendanceRate() const {
    if (courses.empty()) {
        return 0.0;
    }
    const int attendedCount = static_cast<int>(std::count_if(
        courses.begin(), courses.end(), [](const Course& course) { return course.attended; }));
    return static_cast<double>(attendedCount) / static_cast<double>(courses.size());
}

void Schedule::resetAttendance() {
    for (Course& course : courses) {
        course.attended = false;
    }
}

bool Schedule::isCourseAttended(int dayOfWeek, int courseIndex) const {
    int matchingIndex = 0;
    for (const Course& course : courses) {
        if (course.dayOfWeek != dayOfWeek) {
            continue;
        }
        if (matchingIndex == courseIndex) {
            return course.attended;
        }
        ++matchingIndex;
    }
    return false;
}
