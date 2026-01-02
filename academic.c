#include "system.h"

void registerLecturer();
void registerSubject();
void viewAttendanceReport();
void viewStudentAttendancePercentage();
void issueWarning();
void viewWarningHistory();
void academicMenu();

void academicMenu() {
    int choice;
    
    do {
        printf("\n===== ACADEMIC STAFF MENU =====\n");
        printf("1. Register New Lecturer\n");
        printf("2. Register New Subject\n");
        printf("3. View Attendance Report\n");
        printf("4. Issue Warning to Students\n");
        printf("5. View Warning History\n");
        printf("6. Logout\n");
        printf("Enter your choice: ");
        
        choice = readIntInput();
        
        switch(choice) {
            case 1:
                registerLecturer();
                break;
            case 2:
                registerSubject();
                break;
            case 3:
                viewAttendanceReport();
                break;
            case 4:
                issueWarning();
                break;
            case 5:
                viewWarningHistory();
                break;
            case 6:
                printf("Logged out successfully!\n");
                break;
            default:
                if (choice != -1) {
                    printf("Invalid choice! Please enter a number between 1 and 6.\n");
                }
        }
    } while(choice != 6);
}

void registerLecturer() {
    Lecturer new_lecturer;
    
    printf("\n===== REGISTER NEW LECTURER =====\n");
    new_lecturer.id = lecturer_count + 1;  // Start from 1
    
    printf("Enter Name: ");
    scanf(" %[^\n]", new_lecturer.name);
    
    printf("Enter Username: ");
    scanf("%s", new_lecturer.username);
    
    printf("Enter Password: ");
    scanf("%s", new_lecturer.password);
    
    printf("Enter Department: ");
    scanf(" %[^\n]", new_lecturer.department);
    
    lecturers[lecturer_count] = new_lecturer;
    lecturer_count++;
    
    printf("Lecturer registered successfully! Lecturer ID: LEC%03d\n", new_lecturer.id);
}

void registerSubject() {
    Subject new_subject;
    
    printf("\n===== REGISTER NEW SUBJECT =====\n");
    new_subject.id = subject_count + 1;  // Start from 1
    
    printf("Enter Subject Code: ");
    scanf("%s", new_subject.code);
    
    printf("Enter Subject Name: ");
    scanf(" %[^\n]", new_subject.name);
    
    printf("Enter Credit Hours: ");
    scanf("%d", &new_subject.credit_hours);
    
    printf("\nAvailable Lecturers:\n");
    for (int i = 0; i < lecturer_count; i++) {
        printf("LEC%03d. %s\n", lecturers[i].id, lecturers[i].name);
    }
    
    printf("Assign Lecturer ID (without LEC prefix): ");
    scanf("%d", &new_subject.lecturer_id);
    
    subjects[subject_count] = new_subject;
    subject_count++;
    
    printf("Subject registered successfully! Subject ID: SUB%03d\n", new_subject.id);
}

void viewAttendanceReport() {
    int subject_id;
    
    printf("\n===== ATTENDANCE REPORTS =====\n");
    
    printf("Available Subjects:\n");
    for (int i = 0; i < subject_count; i++) {
        printf("SUB%03d. %s (%s)\n", subjects[i].id, subjects[i].name, subjects[i].code);
    }
    
    printf("Enter Subject ID (without SUB prefix): ");
    subject_id = readIntInput();
    
    char filename[100];
    sprintf(filename, "database/attendance_record/attendance_%d.dat", subject_id);
    FILE *fp = fopen(filename, "rb");
    
    if (fp == NULL) {
        printf("No attendance records found for this subject.\n");
        return;
    }
    
    // Get subject name
    char subject_name[100] = "Unknown";
    char subject_code[20] = "N/A";
    for (int i = 0; i < subject_count; i++) {
        if (subjects[i].id == subject_id) {
            strcpy(subject_name, subjects[i].name);
            strcpy(subject_code, subjects[i].code);
            break;
        }
    }
    
    // First, collect all unique dates and students
    Attendance att;
    char unique_dates[100][11];
    int date_count = 0;
    int unique_students[100];
    int student_count_in_subject = 0;
    
    rewind(fp);
    while (fread(&att, sizeof(Attendance), 1, fp)) {
        if (att.subject_id == subject_id) {
            // Check if date is unique
            int date_found = 0;
            for (int i = 0; i < date_count; i++) {
                if (strcmp(unique_dates[i], att.date) == 0) {
                    date_found = 1;
                    break;
                }
            }
            if (!date_found && date_count < 100) {
                strcpy(unique_dates[date_count], att.date);
                date_count++;
            }
            
            // Check if student is unique
            int student_found = 0;
            for (int i = 0; i < student_count_in_subject; i++) {
                if (unique_students[i] == att.student_id) {
                    student_found = 1;
                    break;
                }
            }
            if (!student_found && student_count_in_subject < 100) {
                unique_students[student_count_in_subject] = att.student_id;
                student_count_in_subject++;
            }
        }
    }
    
    // Sort dates chronologically (DD-MM-YYYY format)
    for (int i = 0; i < date_count - 1; i++) {
        for (int j = i + 1; j < date_count; j++) {
            // Convert DD-MM-YYYY to comparable format
            int day1, month1, year1;
            int day2, month2, year2;
            sscanf(unique_dates[i], "%d-%d-%d", &day1, &month1, &year1);
            sscanf(unique_dates[j], "%d-%d-%d", &day2, &month2, &year2);
            
            if (year1 > year2 || (year1 == year2 && month1 > month2) || 
                (year1 == year2 && month1 == month2 && day1 > day2)) {
                char temp[11];
                strcpy(temp, unique_dates[i]);
                strcpy(unique_dates[i], unique_dates[j]);
                strcpy(unique_dates[j], temp);
            }
        }
    }
    
    printf("\n%s ATTENDANCE REPORT\n", subject_code);
    printf("========================================================================================================\n");
    printf("No | Student ID | Absent Percentage | ");
    for (int i = 0; i < date_count; i++) {
        printf("%-10s | ", unique_dates[i]);
    }
    printf("\n");
    printf("--------------------------------------------------------------------------------------------------------\n");
    
    // For each student, calculate attendance and display
    for (int s = 0; s < student_count_in_subject; s++) {
        int student_id = unique_students[s];
        
        // Get student name
        char student_name[100] = "Unknown";
        for (int i = 0; i < student_count; i++) {
            if (students[i].id == student_id) {
                strcpy(student_name, students[i].name);
                break;
            }
        }
        
        // Calculate attendance percentage
        float percentage = calculateAttendancePercentage(student_id, subject_id);
        float absent_percentage = 100.0 - percentage;
        
        printf("%-2d | STU%03d     | %-16.1f%% | ", s + 1, student_id, absent_percentage);
        
        // For each date, show attendance status
        for (int d = 0; d < date_count; d++) {
            // Check if student was present on this date
            char status = '-';
            rewind(fp);
            while (fread(&att, sizeof(Attendance), 1, fp)) {
                if (att.student_id == student_id && att.subject_id == subject_id && 
                    strcmp(att.date, unique_dates[d]) == 0) {
                    status = att.status;
                    break;
                }
            }
            printf("%-10c | ", status);
        }
        printf("\n");
    }
    
    fclose(fp);
    printf("========================================================================================================\n");
}

void issueWarning() {
    char warning_date[11];
    
    printf("\n===== ISSUE WARNING =====\n");
    
    // Check each subject for students with attendance below 80%
    int total_warnings = 0;
    
    for (int sub_idx = 0; sub_idx < subject_count; sub_idx++) {
        int subject_id = subjects[sub_idx].id;
        int low_attendance_count = 0;
        
        // Count students with low attendance in this subject
        for (int stu_idx = 0; stu_idx < student_count; stu_idx++) {
            for (int j = 0; j < students[stu_idx].subject_count; j++) {
                if (students[stu_idx].enrolled_subjects[j] == subject_id) {
                    float percentage = calculateAttendancePercentage(students[stu_idx].id, subject_id);
                    if (percentage < 80.0) {
                        low_attendance_count++;
                    }
                    break;
                }
            }
        }
        
        if (low_attendance_count > 0) {
            printf("There are %d students in %s (%s) that their attendance below 80%%\n", 
                   low_attendance_count, subjects[sub_idx].name, subjects[sub_idx].code);
            total_warnings += low_attendance_count;
        }
    }
    
    if (total_warnings == 0) {
        printf("\nNo students with low attendance found.\n");
        return;
    }
    
    printf("\nTotal students with low attendance: %d\n", total_warnings);
    printf("Give warning to %d students? (y/n): ", total_warnings);
    
    char confirm;
    scanf(" %c", &confirm);
    clearInputBuffer();
    
    if (confirm != 'y' && confirm != 'Y') {
        printf("Warning cancelled.\n");
        return;
    }
    
    printf("Enter date of warning message (DD-MM-YYYY): ");
    scanf("%10s", warning_date);
    clearInputBuffer();
    
    // Issue warnings to all students with attendance below 80%
    int warnings_issued = 0;
    for (int sub_idx = 0; sub_idx < subject_count; sub_idx++) {
        int subject_id = subjects[sub_idx].id;
        
        for (int stu_idx = 0; stu_idx < student_count; stu_idx++) {
            for (int j = 0; j < students[stu_idx].subject_count; j++) {
                if (students[stu_idx].enrolled_subjects[j] == subject_id) {
                    float percentage = calculateAttendancePercentage(students[stu_idx].id, subject_id);
                    if (percentage < 80.0) {
                        Reminder new_reminder;
                        new_reminder.id = reminder_count + 1;
                        new_reminder.student_id = students[stu_idx].id;
                        new_reminder.is_read = 0;
                        strcpy(new_reminder.date, warning_date);
                        
                        // Create warning message with course name
                        sprintf(new_reminder.message, "Please Attend your %s class", subjects[sub_idx].name);
                        
                        reminders[reminder_count] = new_reminder;
                        reminder_count++;
                        warnings_issued++;
                    }
                    break;
                }
            }
        }
    }
    
    printf("Successfully warned %d students\n", warnings_issued);
}

void viewWarningHistory() {
    printf("\n===== WARNING HISTORY =====\n");
    printf("ID  | Student ID | Date       | Message                                 | Read Status\n");
    printf("----------------------------------------------------------------------------------------\n");
    
    for (int i = 0; i < reminder_count; i++) {
        // Get student name for display
        char student_name[100] = "Unknown";
        for (int j = 0; j < student_count; j++) {
            if (students[j].id == reminders[i].student_id) {
                strcpy(student_name, students[j].name);
                break;
            }
        }
        
        printf("%-3d | STU%03d     | %-10s | %-40s | %s\n", 
               reminders[i].id,
               reminders[i].student_id,
               reminders[i].date,
               reminders[i].message,
               reminders[i].is_read ? "Read" : "Unread");
    }
}