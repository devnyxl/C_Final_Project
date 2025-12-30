#include "system.h"

void viewStudentAttendance();
void viewStudentWarnings();
void studentMenu();

void studentMenu() {
    int choice;
    
    do {
        printf("\n===== STUDENT MENU =====\n");
        printf("1. View Attendance Percentage\n");
        printf("2. View Warning Messages\n");
        printf("3. Logout\n");
        printf("Enter your choice: ");
        
        choice = readIntInput();
        
        switch(choice) {
            case 1:
                viewStudentAttendance();
                break;
            case 2:
                viewStudentWarnings();
                break;
            case 3:
                printf("Logged out successfully!\n");
                break;
            default:
                if (choice != -1) {
                    printf("Invalid choice! Please enter a number between 1 and 3.\n");
                }
        }
    } while(choice != 3);
}

void viewStudentAttendance() {
    printf("\n===== YOUR ATTENDANCE PERCENTAGE =====\n");
    
    Student *current_student = NULL;
    for (int i = 0; i < student_count; i++) {
        if (students[i].id == current_user_id) {
            current_student = &students[i];
            break;
        }
    }
    
    if (current_student == NULL) {
        printf("Student not found!\n");
        return;
    }
    
    printf("Student: %s\n", current_student->name);
    printf("----------------------------------------\n");
    
    for (int i = 0; i < current_student->subject_count; i++) {
        int subject_id = current_student->enrolled_subjects[i];
        
        char subject_name[100] = "Unknown";
        for (int j = 0; j < subject_count; j++) {
            if (subjects[j].id == subject_id) {
                strcpy(subject_name, subjects[j].name);
                break;
            }
        }
        
        float percentage = calculateAttendancePercentage(current_user_id, subject_id);
        printf("Subject: %s\n", subject_name);
        printf("Attendance: %.2f%%\n", percentage);
        
        if (percentage < 80.0) {
            printf("Warning: Attendance below 80%%!\n");
        }
        printf("----------------------------------------\n");
    }
}

void viewStudentWarnings() {
    printf("\n===== YOUR WARNING MESSAGES =====\n");
    
    int has_warnings = 0;
    
    for (int i = 0; i < reminder_count; i++) {
        if (reminders[i].student_id == current_user_id) {
            has_warnings = 1;
            printf("\nDate: %s\n", reminders[i].date);
            printf("Message: %s\n", reminders[i].message);
            printf("Status: %s\n", reminders[i].is_read ? "Read" : "Unread");
            
            reminders[i].is_read = 1;
        }
    }
    
    if (!has_warnings) {
        printf("No warning messages found.\n");
    }
}