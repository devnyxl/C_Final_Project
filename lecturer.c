#include "system.h"

void registerStudent();
void recordAttendance();
void updateAttendance();
void viewSubjectAttendance();
void lecturerMenu();

void lecturerMenu() {
    int choice;
    
    do {
        printf("\n===== LECTURER MENU =====\n");
        printf("1. Register Students for Subject\n");
        printf("2. Record Daily Attendance\n");
        printf("3. Update Attendance Records\n");
        printf("4. View Subject Attendance List\n");
        printf("5. Logout\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        
        switch(choice) {
            case 1:
                registerStudent();
                break;
            case 2:
                recordAttendance();
                break;
            case 3:
                updateAttendance();
                break;
            case 4:
                viewSubjectAttendance();
                break;
            case 5:
                printf("Logged out successfully!\n");
                break;
            default:
                printf("Invalid choice! Please try again.\n");
        }
    } while(choice != 5);
}

void registerStudent() {
    int subject_id, student_id;
    
    printf("\n===== REGISTER STUDENT FOR SUBJECT =====\n");
    
    printf("Subjects taught by you:\n");
    int lecturer_subjects[10];
    int sub_count = 0;
    
    for (int i = 0; i < subject_count; i++) {
        if (subjects[i].lecturer_id == current_user_id) {
            printf("%d. %s (%s)\n", subjects[i].id, subjects[i].name, subjects[i].code);
            lecturer_subjects[sub_count++] = subjects[i].id;
        }
    }
    
    if (sub_count == 0) {
        printf("You are not assigned to any subjects.\n");
        return;
    }
    
    printf("Select Subject ID: ");
    scanf("%d", &subject_id);
    
    printf("\n1. Register new student\n");
    printf("2. Enroll existing student\n");
    printf("Enter choice: ");
    int reg_choice;
    scanf("%d", &reg_choice);
    
    if (reg_choice == 1) {
        Student new_student;
        
        printf("\nEnter Student Name: ");
        scanf(" %[^\n]s", new_student.name);
        
        printf("Enter Username: ");
        scanf("%s", new_student.username);
        
        printf("Enter Password: ");
        scanf("%s", new_student.password);
        
        new_student.id = student_count + 3000;
        new_student.subject_count = 1;
        new_student.enrolled_subjects[0] = subject_id;
        
        students[student_count] = new_student;
        student_count++;
        
        printf("Student registered and enrolled successfully!\n");
        
    } else if (reg_choice == 2) {
        printf("\nExisting Students:\n");
        for (int i = 0; i < student_count; i++) {
            printf("%d. %s\n", students[i].id, students[i].name);
        }
        
        printf("Enter Student ID: ");
        scanf("%d", &student_id);
        
        for (int i = 0; i < student_count; i++) {
            if (students[i].id == student_id) {
                for (int j = 0; j < students[i].subject_count; j++) {
                    if (students[i].enrolled_subjects[j] == subject_id) {
                        printf("Student already enrolled in this subject.\n");
                        return;
                    }
                }
                
                students[i].enrolled_subjects[students[i].subject_count] = subject_id;
                students[i].subject_count++;
                printf("Student enrolled successfully!\n");
                return;
            }
        }
        printf("Student not found!\n");
    }
}

void recordAttendance() {
    int subject_id;
    char date[11];
    
    printf("\n===== RECORD DAILY ATTENDANCE =====\n");
    
    printf("Your Subjects:\n");
    for (int i = 0; i < subject_count; i++) {
        if (subjects[i].lecturer_id == current_user_id) {
            printf("%d. %s (%s)\n", subjects[i].id, subjects[i].name, subjects[i].code);
        }
    }
    
    printf("Enter Subject ID: ");
    scanf("%d", &subject_id);
    
    printf("Enter Date (YYYY-MM-DD): ");
    scanf("%s", date);
    
    printf("\nMark attendance for each student:\n");
    printf("==================================\n");
    
    Attendance attendance_records[100];
    int record_count = 0;
    
    for (int i = 0; i < student_count; i++) {
        for (int j = 0; j < students[i].subject_count; j++) {
            if (students[i].enrolled_subjects[j] == subject_id) {
                printf("Student: %s (ID: %d)\n", students[i].name, students[i].id);
                printf("Press 'P' for Present, 'A' for Absent: ");
                
                char status;
                scanf(" %c", &status);
                status = toupper(status);
                
                if (status == 'P' || status == 'A') {
                    attendance_records[record_count].student_id = students[i].id;
                    attendance_records[record_count].subject_id = subject_id;
                    strcpy(attendance_records[record_count].date, date);
                    attendance_records[record_count].status = status;
                    record_count++;
                } else {
                    printf("Invalid input! Defaulting to Absent.\n");
                    attendance_records[record_count].student_id = students[i].id;
                    attendance_records[record_count].subject_id = subject_id;
                    strcpy(attendance_records[record_count].date, date);
                    attendance_records[record_count].status = 'A';
                    record_count++;
                }
                break;
            }
        }
    }
    
    char filename[50];
    sprintf(filename, "database/attendance_record/attendance_%d.dat", subject_id);
    FILE *fp = fopen(filename, "ab");
    
    if (fp != NULL) {
        fwrite(attendance_records, sizeof(Attendance), record_count, fp);
        fclose(fp);
        printf("\nAttendance recorded successfully for %d students!\n", record_count);
    } else {
        printf("Error saving attendance records.\n");
    }
}

void updateAttendance() {
    int subject_id, student_id;
    char date[11];
    
    printf("\n===== UPDATE ATTENDANCE =====\n");
    
    printf("Enter Subject ID: ");
    scanf("%d", &subject_id);
    
    printf("Enter Student ID: ");
    scanf("%d", &student_id);
    
    printf("Enter Date to update (YYYY-MM-DD): ");
    scanf("%s", date);
    
    char filename[50];
    sprintf(filename, "database/attendance_record/attendance_%d.dat", subject_id);
    FILE *fp = fopen(filename, "rb+");
    
    if (fp == NULL) {
        printf("No attendance records found for this subject.\n");
        return;
    }
    
    Attendance att;
    int found = 0;
    long int pos;
    
    while (!found && fread(&att, sizeof(Attendance), 1, fp)) {
        if (att.student_id == student_id && att.subject_id == subject_id && 
            strcmp(att.date, date) == 0) {
            found = 1;
            pos = ftell(fp) - sizeof(Attendance);
            
            printf("Current status: %c\n", att.status);
            printf("Enter new status (P/A): ");
            char new_status;
            scanf(" %c", &new_status);
            new_status = toupper(new_status);
            
            if (new_status == 'P' || new_status == 'A') {
                fseek(fp, pos, SEEK_SET);
                att.status = new_status;
                fwrite(&att, sizeof(Attendance), 1, fp);
                printf("Attendance updated successfully!\n");
            } else {
                printf("Invalid status! Update cancelled.\n");
            }
        }
    }
    
    if (!found) {
        printf("Attendance record not found.\n");
    }
    
    fclose(fp);
}

void viewSubjectAttendance() {
    int subject_id;
    
    printf("\n===== SUBJECT ATTENDANCE LIST =====\n");
    
    printf("Your Subjects:\n");
    for (int i = 0; i < subject_count; i++) {
        if (subjects[i].lecturer_id == current_user_id) {
            printf("%d. %s (%s)\n", subjects[i].id, subjects[i].name, subjects[i].code);
        }
    }
    
    printf("Enter Subject ID: ");
    scanf("%d", &subject_id);
    
    char filename[50];
    sprintf(filename, "database/attendance_record/attendance_%d.dat", subject_id);
    FILE *fp = fopen(filename, "rb");
    
    if (fp == NULL) {
        printf("No attendance records found for this subject.\n");
        return;
    }
    
    Attendance att;
    printf("\nAttendance Records:\n");
    printf("Student ID\tDate\t\tStatus\n");
    printf("------------------------------------\n");
    
    while (fread(&att, sizeof(Attendance), 1, fp)) {
        if (att.subject_id == subject_id) {
            printf("%d\t\t%s\t%c\n", att.student_id, att.date, att.status);
        }
    }
    fclose(fp);
}