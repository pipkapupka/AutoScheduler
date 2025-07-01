#pragma once 

#include <string>
#include <set>
#include <vector>
#include <map>
#include <unordered_set>
#include <unordered_map>

enum class DaysOfWeek {
    Mon, 
    Tue,
    Wed,
    Thu,
    Fri,
    Sat,
    Sun
};

enum class LessonType {
    Lecture,
    Lab,
    Seminar,
    Exam,    
};

struct TimeSlot {
    std::string start;
    std::string end;
    DaysOfWeek day;
    // bool is_lecture - разделеение пар 
};

struct ClassRoom {
    int id;
    std::string code;
    int capacity;
    // std::set<std::string> equipment; - оборудование
    // bool is_online; - для дистанционного обучения
};

struct Teacher {
    std::string name;
    std::unordered_set<std::string> competencies;
    std::set<StudentGroup> pinned_for_groups;
    // int max_lessons_per_day; - сколько дней в день может вести 
};

struct Subject {
    int id;
    std::string name;
    int priority;
    int duration_minutes;
    LessonType type;
};

struct StudentGroup {
    int id;
    std::string code;
    int size;
    std::vector<int> required_subjects;
};

struct Lesson {
    int id;
    Subject subject;
    std::vector<Teacher> teachers;
    StudentGroup group;
    ClassRoom classroom;
    TimeSlot time;
    // bool is_fixed; - можно ли перемещать занятие
};

struct InstitutionRules {
    int min_break_between_lessons;
    int max_lessons_per_day;
    std::map<std::string, int> subject_weights;
    bool allow_online;
};
