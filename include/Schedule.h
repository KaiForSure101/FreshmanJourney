#pragma once

#include <string>
#include <vector>

enum class CourseType {
    Mathematics,
    Programming,
    English,
    Physics,
    History,
    Art,
    Biology
};

struct Course {
    std::string name;
    CourseType type;
    int startTime;
    int endTime;
    int dayOfWeek;
    std::string location;
    int knowledgeGain;
    double gpaWeight;
    bool attended;
};

class Schedule {
public:
    Schedule();

    void generateWeeklySchedule();
    std::vector<Course> getCoursesForDay(int dayOfWeek) const;
    bool isClassActive(int currentTime, int dayOfWeek) const;
    std::string getCurrentClassName(int currentTime, int dayOfWeek) const;
    void markAttendance(int dayOfWeek, int courseIndex);
    double getAttendanceRate() const;
    void resetAttendance();
    bool isCourseAttended(int dayOfWeek, int courseIndex) const;

private:
    std::vector<Course> courses;
};
