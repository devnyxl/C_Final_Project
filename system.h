#ifndef SYSTEM_H
#define SYSTEM_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Structure Definitions
typedef struct {
    int id;
    char name[100];
    char username[50];
    char password[50];
    char department[100];
} Lecturer;

typedef struct {
    int id;
    char code[20];
    char name[100];
    int lecturer_id;
    int credit_hours;
} Subject;

typedef struct {
    int id;
    char name[100];
    char username[50];
    char password[50];
    int enrolled_subjects[10];
    int subject_count;
} Student;

typedef struct {
    int student_id;
    int subject_id;
    char date[11];
    char status;
} Attendance;

typedef struct {
    int id;
    int student_id;
    char message[200];
    char date[11];
    int is_read;
} Reminder;

// Global variables (extern declarations)
extern Lecturer lecturers[100];
extern Subject subjects[100];
extern Student students[100];
extern Reminder reminders[200];
extern int lecturer_count;
extern int subject_count;
extern int student_count;
extern int reminder_count;
extern int current_user_id;
extern char current_user_role[20];

// Function prototypes
void loadData();
void saveData();
void mainMenu();
float calculateAttendancePercentage(int student_id, int subject_id);
int authenticateUser(char role[], char username[], char password[]);
void clearInputBuffer();
int readIntInput();

// Academic staff functions
void academicMenu();
void registerLecturer();
void registerSubject();
void viewAttendanceReport();
void viewStudentAttendancePercentage();
void issueWarning();
void viewWarningHistory();

// Lecturer functions
void lecturerMenu();
void registerStudent();
void recordAttendance();
void updateAttendance();
void viewSubjectAttendance();

// Student functions
void studentMenu();
void viewStudentAttendance();
void viewStudentWarnings();

#endif