#include "system.h"
#include <direct.h>  // For _mkdir on Windows (use mkdir for Linux/Mac)

// Function to create directory if it doesn't exist
void createDirectory(const char *path) {
    _mkdir(path);
}

// Function to clear input buffer
void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Function to read integer with validation
int readIntInput() {
    char input[100];
    int value;
    
    while (1) {
        if (fgets(input, sizeof(input), stdin) == NULL) {
            printf("Error reading input.\n");
            return -1;
        }
        
        if (sscanf(input, "%d", &value) == 1) {
            return value;
        }
        
        printf("Invalid input! Please enter the correct value: ");
    }
}

// Global variable definitions
Lecturer lecturers[100];
Subject subjects[100];
Student students[100];
Reminder reminders[200];
int lecturer_count = 0;
int subject_count = 0;
int student_count = 0;
int reminder_count = 0;
int current_user_id = -1;
char current_user_role[20] = "";

void loadData() {
    FILE *fp;
    
    // Create database directory if it doesn't exist
    createDirectory("database");
    createDirectory("database/attendance_record");
    
    fp = fopen("database/lecturers.dat", "rb");
    if (fp != NULL) {
        fread(&lecturer_count, sizeof(int), 1, fp);
        fread(lecturers, sizeof(Lecturer), lecturer_count, fp);
        fclose(fp);
    }
    
    fp = fopen("database/subjects.dat", "rb");
    if (fp != NULL) {
        fread(&subject_count, sizeof(int), 1, fp);
        fread(subjects, sizeof(Subject), subject_count, fp);
        fclose(fp);
    }
    
    fp = fopen("database/students.dat", "rb");
    if (fp != NULL) {
        fread(&student_count, sizeof(int), 1, fp);
        fread(students, sizeof(Student), student_count, fp);
        fclose(fp);
    }
    
    fp = fopen("database/reminders.dat", "rb");
    if (fp != NULL) {
        fread(&reminder_count, sizeof(int), 1, fp);
        fread(reminders, sizeof(Reminder), reminder_count, fp);
        fclose(fp);
    }
}

void saveData() {
    FILE *fp;
    
    // Create database directory if it doesn't exist
    createDirectory("database");
    createDirectory("database/attendance_record");
    
    fp = fopen("database/lecturers.dat", "wb");
    if (fp != NULL) {
        fwrite(&lecturer_count, sizeof(int), 1, fp);
        fwrite(lecturers, sizeof(Lecturer), lecturer_count, fp);
        fclose(fp);
    }
    
    fp = fopen("database/subjects.dat", "wb");
    if (fp != NULL) {
        fwrite(&subject_count, sizeof(int), 1, fp);
        fwrite(subjects, sizeof(Subject), subject_count, fp);
        fclose(fp);
    }
    
    fp = fopen("database/students.dat", "wb");
    if (fp != NULL) {
        fwrite(&student_count, sizeof(int), 1, fp);
        fwrite(students, sizeof(Student), student_count, fp);
        fclose(fp);
    }
    
    fp = fopen("database/reminders.dat", "wb");
    if (fp != NULL) {
        fwrite(&reminder_count, sizeof(int), 1, fp);
        fwrite(reminders, sizeof(Reminder), reminder_count, fp);
        fclose(fp);
    }
}

// CORRECTED ATTENDANCE PERCENTAGE CALCULATION
float calculateAttendancePercentage(int student_id, int subject_id) {
    char filename[100];
    sprintf(filename, "database/attendance_record/attendance_%d.dat", subject_id);
    FILE *fp = fopen(filename, "rb");
    
    if (fp == NULL) {
        return 0.0;
    }
    
    // We need to count distinct dates and student's presence
    Attendance att;
    char dates[100][11];  // Store unique dates (max 100 classes)
    int date_count = 0;
    int student_present_count = 0;
    
    // First pass: Count unique dates
    rewind(fp);
    while (fread(&att, sizeof(Attendance), 1, fp)) {
        if (att.subject_id == subject_id) {
            // Check if this date is already recorded
            int found = 0;
            for (int i = 0; i < date_count; i++) {
                if (strcmp(dates[i], att.date) == 0) {
                    found = 1;
                    break;
                }
            }
            if (!found && date_count < 100) {
                strcpy(dates[date_count], att.date);
                date_count++;
            }
        }
    }
    
    // Second pass: Count student's presence
    rewind(fp);
    while (fread(&att, sizeof(Attendance), 1, fp)) {
        if (att.subject_id == subject_id && att.student_id == student_id) {
            // Check if student was present on each date
            for (int i = 0; i < date_count; i++) {
                if (strcmp(dates[i], att.date) == 0 && att.status == 'P') {
                    student_present_count++;
                    break;  // Student can only be present once per date
                }
            }
        }
    }
    
    fclose(fp);
    
    if (date_count == 0) {
        return 0.0;
    }
    
    return ((float)student_present_count / date_count) * 100.0;
}

int authenticateUser(char role[], char username[], char password[]) {
    if (strcmp(role, "lecturer") == 0) {
        for (int i = 0; i < lecturer_count; i++) {
            if (strcmp(lecturers[i].username, username) == 0 && 
                strcmp(lecturers[i].password, password) == 0) {
                current_user_id = lecturers[i].id;
                return 1;
            }
        }
    } else if (strcmp(role, "student") == 0) {
        for (int i = 0; i < student_count; i++) {
            if (strcmp(students[i].username, username) == 0 && 
                strcmp(students[i].password, password) == 0) {
                current_user_id = students[i].id;
                return 1;
            }
        }
    }
    return 0;
}

void mainMenu() {
    int choice;
    char username[50], password[50];
    
    do {
        printf("\n===== STUDENT ATTENDANCE MANAGEMENT SYSTEM =====\n");
        printf("1. Login as Academic Staff\n");
        printf("2. Login as Lecturer\n");
        printf("3. Login as Student\n");
        printf("4. Exit\n");
        printf("Enter your choice: ");
        
        choice = readIntInput();
        
        switch(choice) {
            case 1:
                printf("\n===== ACADEMIC STAFF LOGIN =====\n");
                printf("Username: ");
                scanf("%s", username);
                printf("Password: ");
                scanf("%s", password);
                clearInputBuffer();
                
                if (strcmp(username, "academic") == 0 && strcmp(password, "admin123") == 0) {
                    strcpy(current_user_role, "academic");
                    academicMenu();
                } else {
                    printf("Invalid credentials!\n");
                }
                break;
                
            case 2:
                printf("\n===== LECTURER LOGIN =====\n");
                printf("Username: ");
                scanf("%s", username);
                printf("Password: ");
                scanf("%s", password);
                clearInputBuffer();
                
                if (authenticateUser("lecturer", username, password)) {
                    strcpy(current_user_role, "lecturer");
                    lecturerMenu();
                } else {
                    printf("Invalid credentials!\n");
                }
                break;
                
            case 3:
                printf("\n===== STUDENT LOGIN =====\n");
                printf("Username: ");
                scanf("%s", username);
                printf("Password: ");
                scanf("%s", password);
                clearInputBuffer();
                
                if (authenticateUser("student", username, password)) {
                    strcpy(current_user_role, "student");
                    studentMenu();
                } else {
                    printf("Invalid credentials!\n");
                }
                break;
                
            case 4:
                printf("Thank you for using the system!\n");
                break;
                
            default:
                if (choice != -1) {
                    printf("Invalid choice! Please enter a number between 1 and 4.\n");
                }
        }
    } while(choice != 4);
}

int main() {
    loadData();
    mainMenu();
    saveData();
    return 0;
}