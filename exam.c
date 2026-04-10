#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "exam.h"
#include "file_io.h"
#include "menu.h"
#include <time.h>
#include "user.h"
#include <ctype.h>

#define RESET      "\x1b[0m"
#define BOLD       "\x1b[1m"
#define RED        "\x1b[31m"
#define GREEN      "\x1b[32m"
#define YELLOW     "\x1b[33m"
#define BLUE       "\x1b[34m"
#define MAGENTA    "\x1b[35m"
#define CYAN       "\x1b[36m"
#define WHITE      "\x1b[37m"

// Initialize the global exam list
ExamNode* exam_head = NULL;

// Function to check if an exam code is valid (must be of the form AA111)
int is_valid_exam_code2(const char* exam_code) {
    // Exam code must be exactly 5 characters long
    if (strlen(exam_code) != 5) {
        return 0;  // Invalid length
    }

    // Check if the first two characters are uppercase letters
    if (!isupper(exam_code[0]) || !isupper(exam_code[1])) {
        return 0;  // The first two characters must be uppercase letters
    }

    // Check if the last three characters are digits
    if (!isdigit(exam_code[2]) || !isdigit(exam_code[3]) || !isdigit(exam_code[4])) {
        return 0;  // The last three characters must be digits
    }

    // If all checks pass, the exam code is valid
    return 1;
}

int is_exam_code_exists(const char* exam_code) {
    FILE* file = fopen("exams.csv", "r");
    if (!file) {
        printf(RED "Error opening exams file!\n" RESET);
        return 0;  // Return false if file can't be opened
    }

    char line[256];
    int exists = 0;  // Flag to track if the exam code exists

    // Search for the exam code in the exams.csv
    while (fgets(line, sizeof(line), file)) {
        char code[50];
        
        
        if (sscanf(line, "%49[^,]", code) == 1) {  
            // Remove any extra spaces or newline characters
            code[strcspn(code, "\n")] = 0;  
            
            if (strcmp(code, exam_code) == 0) {
                exists = 1;  // Exam code found
                break;
            }
        }
    }

    fclose(file);
    return exists;  
}

// Function to add a new exam
void add_exam() {
    Exam new_exam;
    int valid_exam_code = 0;

    // Ask for the exam code and validate it
    while (!valid_exam_code) {
        printf(YELLOW "Enter exam code (e.g., AA111): " RESET);
        scanf("%s", new_exam.exam_code);

        // Validate the exam code format
        if (!is_valid_exam_code2(new_exam.exam_code)) {
            printf(RED "Invalid exam code format! Please enter a code of the form AA111.\n" RESET);
        }

        // Check if the exam code already exists
        if (is_exam_code_exists(new_exam.exam_code)) {
            printf(RED "This exam code already exists! Please enter a unique exam code.\n" RESET);
            continue;  // Ask again if the code already exists
        }

        valid_exam_code = 1;  
    }

    printf(YELLOW "Enter subject: " RESET);
    scanf(" %[^\n]%*c", new_exam.subject);  // This will allow spaces in subject name

    // Get and validate exam date
    int valid_date = 0;
    while (!valid_date) {
        printf(YELLOW "Enter exam date (DD-MM-YYYY): " RESET);
        scanf("%s", new_exam.exam_date);

        
        char current_date[11];
        time_t t = time(NULL);
        struct tm tm = *localtime(&t);
        strftime(current_date, sizeof(current_date), "%d-%m-%Y", &tm);

       
        struct tm exam_tm = {0};
        int day, month, year;

        if (sscanf(new_exam.exam_date, "%d-%d-%d", &day, &month, &year) == 3) {
            // Check if the month is valid (1-12)
            if (month < 1 || month > 12) {
                printf(RED"Invalid month! Please enter a month between 01 and 12.\n"RESET);
                continue;  // Ask again if month is invalid
            }

            int days_in_month;
            if (month == 2) {
                // Check for leap year (29 days in February)
                if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0)) {
                    days_in_month = 29;  // Leap year
                } else {
                    days_in_month = 28;
                }
            } else if (month == 4 || month == 6 || month == 9 || month == 11) {
                days_in_month = 30;  // April, June, September, November
            } else {
                days_in_month = 31;  // All other months
            }

            // Check if the day is valid for the month
            if (day < 1 || day > days_in_month) {
                printf(RED "Invalid day! The day should be between 01 and %d for month %d.\n" RESET, days_in_month, month);

                continue;  // Ask again if day is invalid
            }

            // If everything is valid, set the date and proceed
            exam_tm.tm_mday = day;
            exam_tm.tm_mon = month - 1; // tm_mon is 0-based
            exam_tm.tm_year = year - 1900; // tm_year is the number of years since 1900

            // Convert both the current and exam dates to time_t for comparison
            time_t exam_time = mktime(&exam_tm);
            time_t current_time = mktime(&tm);

            if (exam_time < current_time) {
                printf(RED "Invalid date! The exam date cannot be in the past.\n" RESET);
                continue;  // Ask again if the date is in the past
            }
        } else {
            printf(RED "Invalid date format! Please enter in DD-MM-YYYY format.\n" RESET);
            continue;  // Ask again if the date is not in the correct format
        }

        valid_date = 1;  // If the date is valid, exit the loop
    }

    // Get total marks and number of students for the exam
    printf(YELLOW "Enter total marks for the exam: " RESET);
    scanf("%d", &new_exam.total_marks);

    printf(YELLOW "Enter total number of students: " RESET);
    scanf("%d", &new_exam.total_students);

    // Create a new node for the linked list
    ExamNode* new_node = (ExamNode*)malloc(sizeof(ExamNode));
    new_node->exam = new_exam;
    new_node->next = NULL;

    // Append to the list
    if (exam_head == NULL) {
        exam_head = new_node;
    } else {
        ExamNode* temp = exam_head;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = new_node;
    }

    // Save the exam data to the CSV file
    re_save_exams_to_csv();
    printf(GREEN "Exam added successfully!\n" RESET);
    press_any_key_to_continue();
    handle_admin_menu();
}


// Function to delete an exam by name
void delete_exam() {
    // Display the list of exams
    ExamNode* temp = exam_head;
    int index = 1;
    if (temp == NULL) {
        printf(RED "No exams available to delete.\n" RESET);
        press_any_key_to_continue();
        handle_admin_menu();
        return;
    }

    printf(CYAN "List of Exams:\n" RESET);
    while (temp != NULL) {
        printf("\033[1;36m%d.\033[0m \033[1;32m%s\033[0m \033[1;34m%s\033[0m (\033[1;33m%s\033[0m)\n", 
       index, temp->exam.exam_code, temp->exam.subject, temp->exam.exam_date);
        temp = temp->next;
        index++;
    }

    int choice;
    printf(YELLOW "Enter the number of the exam you want to delete: " RESET);
    scanf("%d", &choice);

    // Find the selected exam in the list
    temp = exam_head;
    int count = 1;
    ExamNode* selected_exam = NULL;
    while (temp != NULL) {
        if (count == choice) {
            selected_exam = temp;
            break;
        }
        temp = temp->next;
        count++;
    }

    if (selected_exam == NULL) {
        printf(RED "Invalid choice. No exam deleted.\n" RESET);
        press_any_key_to_continue();
        handle_admin_menu();
        return;
    }

    // Ask for confirmation
    char confirm;
    printf(YELLOW "Are you sure you want to delete the exam: %s (Y/N)? " RESET, selected_exam->exam.exam_code);
    scanf(" %c", &confirm);  

    if (confirm == 'Y' || confirm == 'y') {
        delete_exam_data(selected_exam->exam.exam_code);
        printf(GREEN "Exam deleted successfully!\n" RESET);
        press_any_key_to_continue();
        handle_admin_menu();
    } else {
        printf(MAGENTA "Exam deletion cancelled.\n" RESET);
        press_any_key_to_continue();
        handle_admin_menu();
    }

    press_any_key_to_continue();
    handle_admin_menu();
}

// Function to view all exams
void view_exams() {
    ExamNode* temp = exam_head;
    printf("Exams List:\n");
    while (temp != NULL) {
         printf("\033[1;36mExam:\033[0m \033[1;32m%s\033[0m, \033[1;34mSubject:\033[0m \033[1;33m%s\033[0m, \033[1;35mDate:\033[0m \033[1;31m%s\033[0m, \033[1;37mTotal Marks:\033[0m \033[1;37m%d\033[0m, \033[1;38mTotal Students:\033[0m \033[1;38m%d\033[0m\n", 
                temp->exam.exam_code, temp->exam.subject, temp->exam.exam_date, 
                temp->exam.total_marks, temp->exam.total_students);
        temp = temp->next;
    }
}

// Function to search for exams by year and display them
void search_exam_by_year() {
    int year;
    printf(YELLOW "Enter exam year to search for: " RESET);
    scanf("%d", &year);

    ExamNode* temp = exam_head;
    int found = 0;

    printf(CYAN "Exams in the year %d:\n" RESET, year);
    while (temp != NULL) {
       
        int exam_year = atoi(&temp->exam.exam_date[6]);  // Extract year from exam_date
        if (exam_year == year) {
           printf("\033[1;36mExam:\033[0m \033[1;32m%s\033[0m, \033[1;34mSubject:\033[0m \033[1;33m%s\033[0m, \033[1;37mTotal Students:\033[0m \033[1;31m%d\033[0m, \033[1;35mDate:\033[0m \033[1;31m%s\033[0m\n", 
                temp->exam.exam_code, temp->exam.subject, temp->exam.total_students, temp->exam.exam_date);

            found = 1;
        }
        temp = temp->next;
    }

    if (!found) {
        printf(RED "No exams found for the year %d.\n" RESET, year);
    }
}

// Function to search for exams by subject and display them
void search_exam_by_subject() {
    char subject[50];
    printf(YELLOW "Enter exam subject to search for: " RESET);
    scanf("%s", subject);

    ExamNode* temp = exam_head;
    int found = 0;

    printf(CYAN "Exams for the subject %s:\n" RESET, subject);
    while (temp != NULL) {
        if (strcmp(temp->exam.subject, subject) == 0) {
            printf("\033[1;36mExam:\033[0m \033[1;32m%s\033[0m, \033[1;35mDate:\033[0m \033[1;31m%s\033[0m, \033[1;33mTotal Marks:\033[0m \033[1;33m%d\033[0m, \033[1;37mTotal Students:\033[0m \033[1;34m%d\033[0m\n", 
                temp->exam.exam_code, temp->exam.exam_date, temp->exam.total_marks, temp->exam.total_students);

            found = 1;
        }
        temp = temp->next;
    }

    if (!found) {
        printf(RED "No exams found for the subject %s.\n" RESET, subject);
    }
}

// Function to re-save all exams to the CSV after deletion or updates
void re_save_exams_to_csv() {
    FILE* file = fopen("exams.csv", "w");
    if (!file) {
        printf(RED "Error opening file for saving exam data!\n" RESET);
        return;
    }

    ExamNode* temp = exam_head;
    while (temp != NULL) {
        // Write exam data back to the file in the format: exam_code,subject,exam_date,total_marks,total_students
        fprintf(file, "%s,%s,%s,%d,%d\n", temp->exam.exam_code, temp->exam.subject, temp->exam.exam_date, temp->exam.total_marks, temp->exam.total_students);
        temp = temp->next;
    }
    fclose(file);
}
