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
        clearInputBuffer();
        
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
                printf("Invalid choice! Please try again!\n");
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
           printf("SUB%03d. %s (%s)\n", subjects[i].id, subjects[i].name, subjects[i].code);
           lecturer_subjects[sub_count++] = subjects[i].id;
       }
   }
   
   if (sub_count == 0) {
       printf("You are not assigned to any subjects.\n");
       return;
   }
   
   printf("Select Subject ID (without SUB prefix): ");
   scanf("%d", &subject_id);
   clearInputBuffer();
   
   // Check if subject is taught by this lecturer
   int valid_subject = 0;
   for (int i = 0; i < sub_count; i++) {
       if (lecturer_subjects[i] == subject_id) {
           valid_subject = 1;
           break;
       }
   }
   
   if (!valid_subject) {
       printf("Invalid subject ID or you don't teach this subject!\n");
       return;
   }
   
   printf("\n1. Register new student\n");
   printf("2. Enroll existing student\n");
   printf("Enter choice: ");
   int reg_choice;
   scanf("%d", &reg_choice);
   clearInputBuffer();
   
   if (reg_choice == 1) {
       Student new_student;
       
       printf("\nEnter Student Name: ");
       scanf(" %[^\n]", new_student.name);
       clearInputBuffer();
       
       printf("Enter Username: ");
       scanf("%s", new_student.username);
       clearInputBuffer();
       
       printf("Enter Password: ");
       scanf("%s", new_student.password);
       clearInputBuffer();
       
       new_student.id = student_count + 1;  // Start from 1
       new_student.subject_count = 1;
       new_student.enrolled_subjects[0] = subject_id;
       
       students[student_count] = new_student;
       student_count++;
       
       printf("Student registered and enrolled successfully! Student ID: STU%03d\n", new_student.id);
       
   } else if (reg_choice == 2) {
       printf("\nExisting Students:\n");
       for (int i = 0; i < student_count; i++) {
           printf("STU%03d. %s\n", students[i].id, students[i].name);
       }
       
       printf("Enter Student ID (without STU prefix): ");
       scanf("%d", &student_id);
       clearInputBuffer();
       
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
   } else {
       printf("Invalid choice!\n");
   }
}

int isDateExistsForSubject(int subject_id, char date[]) {
    char filename[50];
    sprintf(filename, "database/attendance_record/attendance_%d.dat", subject_id);
    FILE *fp = fopen(filename, "rb");
    
    if (fp == NULL) {
        return 0;
    }
    
    Attendance att;
    while (fread(&att, sizeof(Attendance), 1, fp)) {
        if (att.subject_id == subject_id && strcmp(att.date, date) == 0) {
            fclose(fp);
            return 1;
        }
    }
    
    fclose(fp);
    return 0;
}

void recordAttendance() {
    int subject_id;
    char date[11];
    char subject_name[100] = "Unknown";
    char subject_code[20] = "N/A";
    
    printf("\n===== RECORD DAILY ATTENDANCE =====\n");
    
    printf("Your Subjects:\n");
    int lecturer_subjects[10];
    int sub_count = 0;
    
    for (int i = 0; i < subject_count; i++) {
        if (subjects[i].lecturer_id == current_user_id) {
            printf("SUB%03d. %s (%s)\n", subjects[i].id, subjects[i].name, subjects[i].code);
            lecturer_subjects[sub_count++] = subjects[i].id;
        }
    }
    
    if (sub_count == 0) {
        printf("You are not assigned to any subjects.\n");
        return;
    }
    
    printf("Enter Subject ID (without SUB prefix): ");
    scanf("%d", &subject_id);
    clearInputBuffer();
    
    // Check if subject is taught by this lecturer
    int valid_subject = 0;
    for (int i = 0; i < sub_count; i++) {
        if (lecturer_subjects[i] == subject_id) {
            valid_subject = 1;
            break;
        }
    }
    
    if (!valid_subject) {
        printf("Invalid subject ID or you don't teach this subject!\n");
        return;
    }
    
    // Get subject details for display
    for (int i = 0; i < subject_count; i++) {
        if (subjects[i].id == subject_id) {
            strcpy(subject_name, subjects[i].name);
            strcpy(subject_code, subjects[i].code);
            break;
        }
    }
    
    printf("Enter Date (DD-MM-YYYY): ");
    scanf("%10s", date);
    clearInputBuffer();
    
    // Check if date already exists
    if (isDateExistsForSubject(subject_id, date)) {
        printf("Attendance for date %s already exists for this subject!\n", date);
        printf("Please use 'Update Attendance Records' to modify existing attendance.\n");
        return;
    }
    
    printf("\nATTENDANCE %s %s:\n", subject_code, date);
    printf("==================================\n");
    printf("Student Name        |    ID    |   Status (Input 'P' for Present, 'A' for Absent)\n");
    printf("-------------------------------------------------------------------------------\n");
    
    Attendance attendance_records[100];
    int record_count = 0;
    int student_found = 0;
    
    for (int i = 0; i < student_count; i++) {
        for (int j = 0; j < students[i].subject_count; j++) {
            if (students[i].enrolled_subjects[j] == subject_id) {
                student_found = 1;
                printf("%-20s|  STU%03d  |   ", students[i].name, students[i].id);
                
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
    
    if (!student_found) {
        printf("No students enrolled in this subject!\n");
        return;
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
    
    printf("Enter Subject ID (without SUB prefix): ");
    scanf("%d", &subject_id);
    clearInputBuffer();
    
    printf("Enter Student ID (without STU prefix): ");
    scanf("%d", &student_id);
    clearInputBuffer();
    
    printf("Enter Date to update (DD-MM-YYYY): ");
    scanf("%10s", date);
    clearInputBuffer();
    
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
            
            printf("Current status for student STU%03d on %s: %c\n", student_id, date, att.status);
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
            printf("SUB%03d. %s (%s)\n", subjects[i].id, subjects[i].name, subjects[i].code);
        }
    }
    
    printf("Enter Subject ID (without SUB prefix): ");
    scanf("%d", &subject_id);
    clearInputBuffer();
    
    char filename[50];
    sprintf(filename, "database/attendance_record/attendance_%d.dat", subject_id);
    FILE *fp = fopen(filename, "rb");
    
    if (fp == NULL) {
        printf("No attendance records found for this subject.\n");
        return;
    }
    
    Attendance att;
    printf("\nAttendance Records for Subject ID: SUB%03d\n", subject_id);
    printf("Student ID\tDate\t\tStatus\n");
    printf("------------------------------------\n");
    
    while (fread(&att, sizeof(Attendance), 1, fp)) {
        if (att.subject_id == subject_id) {
            printf("STU%03d\t\t%s\t%c\n", att.student_id, att.date, att.status);
        }
    }
    fclose(fp);
}