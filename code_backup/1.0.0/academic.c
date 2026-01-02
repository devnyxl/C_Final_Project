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
        printf("4. View Student Attendance Percentage\n");
        printf("5. Issue Warning to Students\n");
        printf("6. View Warning History\n");
        printf("7. Logout\n");
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
                viewStudentAttendancePercentage();
                break;
            case 5:
                issueWarning();
                break;
            case 6:
                viewWarningHistory();
                break;
            case 7:
                printf("Logged out successfully!\n");
                break;
            default:
                if (choice != -1) {
                    printf("Invalid choice! Please enter a number between 1 and 7.\n");
                }
        }
    } while(choice != 7);
}

void registerLecturer() {
    Lecturer new_lecturer;
    
    printf("\n===== REGISTER NEW LECTURER =====\n");
    printf("Lecturer ID: %d\n", lecturer_count + 1000);
    new_lecturer.id = lecturer_count + 1000;
    
    printf("Enter Name: ");
    scanf(" %[^\n]s", new_lecturer.name);
    
    printf("Enter Username: ");
    scanf("%s", new_lecturer.username);
    
    printf("Enter Password: ");
    scanf("%s", new_lecturer.password);
    
    printf("Enter Department: ");
    scanf(" %[^\n]s", new_lecturer.department);
    
    lecturers[lecturer_count] = new_lecturer;
    lecturer_count++;
    
    printf("Lecturer registered successfully!\n");
}

void registerSubject() {
    Subject new_subject;
    
    printf("\n===== REGISTER NEW SUBJECT =====\n");
    printf("Subject ID: %d\n", subject_count + 2000);
    new_subject.id = subject_count + 2000;
    
    printf("Enter Subject Code: ");
    scanf("%s", new_subject.code);
    
    printf("Enter Subject Name: ");
    scanf(" %[^\n]s", new_subject.name);
    
    printf("Enter Credit Hours: ");
    scanf("%d", &new_subject.credit_hours);
    
    printf("\nAvailable Lecturers:\n");
    for (int i = 0; i < lecturer_count; i++) {
        printf("%d. %s\n", lecturers[i].id, lecturers[i].name);
    }
    
    printf("Assign Lecturer ID: ");
    scanf("%d", &new_subject.lecturer_id);
    
    subjects[subject_count] = new_subject;
    subject_count++;
    
    printf("Subject registered successfully!\n");
}

void viewAttendanceReport() {
    int choice, subject_id, lecturer_id;
    
    printf("\n===== ATTENDANCE REPORTS =====\n");
    printf("1. By Subject\n");
    printf("2. By Lecturer\n");
    printf("Enter your choice: ");
    
    choice = readIntInput();
    
    if (choice == 1) {
        // View by Subject
        printf("\nAvailable Subjects:\n");
        for (int i = 0; i < subject_count; i++) {
            printf("%d. %s (%s)\n", subjects[i].id, subjects[i].name, subjects[i].code);
        }
        printf("Enter Subject ID: ");
        subject_id = readIntInput();
        
        char filename[100];
        sprintf(filename, "database/attendance_record/attendance_%d.dat", subject_id);
        FILE *fp = fopen(filename, "rb");
        
        if (fp == NULL) {
            printf("No attendance records found for this subject.\n");
            return;
        }
        
        Attendance att;
        printf("\n=== Attendance Records for Subject %d ===\n", subject_id);
        printf("Student ID\tDate\t\tStatus\n");
        printf("------------------------------------\n");
        
        int record_count = 0;
        while (fread(&att, sizeof(Attendance), 1, fp)) {
            if (att.subject_id == subject_id) {
                printf("%d\t\t%s\t%c\n", att.student_id, att.date, att.status);
                record_count++;
            }
        }
        fclose(fp);
        
        if (record_count == 0) {
            printf("No attendance records found.\n");
        } else {
            printf("\nTotal records: %d\n", record_count);
        }
        
    } else if (choice == 2) {
        // View by Lecturer
        printf("\nAvailable Lecturers:\n");
        for (int i = 0; i < lecturer_count; i++) {
            printf("%d. %s\n", lecturers[i].id, lecturers[i].name);
        }
        printf("Enter Lecturer ID: ");
        lecturer_id = readIntInput();
        
        // Show subjects taught by this lecturer
        printf("\nSubjects taught by Lecturer %d:\n", lecturer_id);
        int lecturer_subjects[10];
        int sub_count = 0;
        
        for (int i = 0; i < subject_count; i++) {
            if (subjects[i].lecturer_id == lecturer_id) {
                printf("%d. %s (%s)\n", subjects[i].id, subjects[i].name, subjects[i].code);
                lecturer_subjects[sub_count] = subjects[i].id;
                sub_count++;
            }
        }
        
        if (sub_count == 0) {
            printf("This lecturer is not assigned to any subjects.\n");
            return;
        }
        
        printf("\nSelect Subject ID to view attendance: ");
        subject_id = readIntInput();
        
        // Check if the selected subject is taught by this lecturer
        int valid_subject = 0;
        for (int i = 0; i < sub_count; i++) {
            if (lecturer_subjects[i] == subject_id) {
                valid_subject = 1;
                break;
            }
        }
        
        if (!valid_subject) {
            printf("Invalid subject ID or subject not taught by this lecturer.\n");
            return;
        }
        
        // Display attendance for the selected subject
        char filename[100];
        sprintf(filename, "database/attendance_record/attendance_%d.dat", subject_id);
        FILE *fp = fopen(filename, "rb");
        
        if (fp == NULL) {
            printf("No attendance records found for this subject.\n");
            return;
        }
        
        Attendance att;
        printf("\n=== Attendance Records for Subject %d ===\n", subject_id);
        printf("Student ID\tDate\t\tStatus\n");
        printf("------------------------------------\n");
        
        int record_count = 0;
        while (fread(&att, sizeof(Attendance), 1, fp)) {
            if (att.subject_id == subject_id) {
                printf("%d\t\t%s\t%c\n", att.student_id, att.date, att.status);
                record_count++;
            }
        }
        fclose(fp);
        
        if (record_count == 0) {
            printf("No attendance records found.\n");
        } else {
            printf("\nTotal records: %d\n", record_count);
        }
    } else {
        printf("Invalid choice!\n");
    }
}

void viewStudentAttendancePercentage() {
    int student_id, subject_id;
    
    printf("\n===== STUDENT ATTENDANCE PERCENTAGE =====\n");
    
    // Show all registered students
    printf("\nRegistered Students:\n");
    printf("ID\tName\n");
    printf("----------------------------\n");
    for (int i = 0; i < student_count; i++) {
        printf("%d\t%s\n", students[i].id, students[i].name);
    }
    
    printf("\nEnter Student ID: ");
    student_id = readIntInput();
    
    // Find student
    int student_index = -1;
    for (int i = 0; i < student_count; i++) {
        if (students[i].id == student_id) {
            student_index = i;
            break;
        }
    }
    
    if (student_index == -1) {
        printf("Student not found!\n");
        return;
    }
    
    // Check if student is enrolled in any subjects
    if (students[student_index].subject_count == 0) {
        printf("\nStudent %s is not enrolled in any subjects.\n", students[student_index].name);
        return;
    }
    
    printf("\n=== Attendance Percentage for %s (ID: %d) ===\n", 
           students[student_index].name, student_id);
    printf("--------------------------------------------------\n");
    printf("Subject Code\tSubject Name\t\tAttendance %%\n");
    printf("--------------------------------------------------\n");
    
    // Loop through ALL subjects the student is enrolled in
    for (int i = 0; i < students[student_index].subject_count; i++) {
        subject_id = students[student_index].enrolled_subjects[i];
        
        // Find subject details
        char subject_name[100] = "Unknown";
        char subject_code[20] = "N/A";
        for (int j = 0; j < subject_count; j++) {
            if (subjects[j].id == subject_id) {
                strcpy(subject_name, subjects[j].name);
                strcpy(subject_code, subjects[j].code);
                break;
            }
        }
        
        // Calculate attendance percentage
        float percentage = calculateAttendancePercentage(student_id, subject_id);
        
        // Format output for better readability
        printf("%-12s\t%-20s\t%.2f%%", subject_code, subject_name, percentage);
        
        // Show warning if attendance is low
        if (percentage < 80.0) {
            printf("(Below 80%%)");
        }
        printf("\n");
    }
    
    // Show summary
    printf("--------------------------------------------------\n");
    printf("Total Subjects: %d\n", students[student_index].subject_count);
}

void issueWarning() {
    int student_id, subject_id;
    
    printf("\n===== ISSUE WARNING =====\n");
    
    printf("Students with attendance below 80%%:\n");
    printf("ID\tName\t\tSubject Code\tSubject Name\t\tPercentage\n");
    printf("-----------------------------------------------------------------------------\n");
    
    int low_attendance_students[100];
    int low_count = 0;
    
    // Check all students for low attendance in ANY subject
    for (int i = 0; i < student_count; i++) {
        for (int j = 0; j < students[i].subject_count; j++) {
            subject_id = students[i].enrolled_subjects[j];
            float percentage = calculateAttendancePercentage(students[i].id, subject_id);
            
            if (percentage < 80.0) {
                // Get subject details
                char subject_name[100] = "Unknown";
                char subject_code[20] = "N/A";
                for (int k = 0; k < subject_count; k++) {
                    if (subjects[k].id == subject_id) {
                        strcpy(subject_name, subjects[k].name);
                        strcpy(subject_code, subjects[k].code);
                        break;
                    }
                }
                
                printf("%d\t%-12s\t%-12s\t%-20s\t%.2f%%\n", 
                       students[i].id, 
                       students[i].name,
                       subject_code,
                       subject_name, 
                       percentage);
                
                low_attendance_students[low_count++] = students[i].id;
            }
        }
    }
    
    if (low_count > 0) {
        printf("\nTotal students with low attendance: %d\n", low_count);
        
        printf("\nEnter Student ID to issue warning: ");
        student_id = readIntInput();
        
        // Check if student exists
        int student_exists = 0;
        for (int i = 0; i < student_count; i++) {
            if (students[i].id == student_id) {
                student_exists = 1;
                break;
            }
        }
        
        if (!student_exists) {
            printf("Invalid Student ID!\n");
            return;
        }
        
        Reminder new_reminder;
        new_reminder.id = reminder_count + 1;
        new_reminder.student_id = student_id;
        new_reminder.is_read = 0;
        
        printf("Enter warning message: ");
        scanf(" %[^\n]s", new_reminder.message);
        
        printf("Enter date (YYYY-MM-DD): ");
        scanf("%s", new_reminder.date);
        
        reminders[reminder_count] = new_reminder;
        reminder_count++;
        
        printf("Warning issued successfully to student ID %d!\n", student_id);
    } else {
        printf("\nNo students with low attendance found.\n");
    }
}

void viewWarningHistory() {
    printf("\n===== WARNING HISTORY =====\n");
    printf("ID\tStudent ID\tDate\t\tMessage\t\t\tRead Status\n");
    printf("-----------------------------------------------------------------\n");
    
    for (int i = 0; i < reminder_count; i++) {
        printf("%d\t%d\t\t%s\t%s\t\t%s\n", 
               reminders[i].id,
               reminders[i].student_id,
               reminders[i].date,
               reminders[i].message,
               reminders[i].is_read ? "Read" : "Unread");
    }
}