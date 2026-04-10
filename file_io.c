#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "file_io.h"
#include "user.h"
#include "exam.h"


// Function to load user data from CSV file into a linked list
void load_user_data(UserNode** user_head) {
    FILE* file = fopen("users.csv", "r");
    if (!file) {
        printf("\033[1;31mError opening users file!\033[0m\n");
        return;
    }

    char line[200];  
    while (fgets(line, sizeof(line), file)) {
        User new_user;
        // Load user data from the CSV 
        sscanf(line, "%[^,],%[^,],%[^,],%[^,]", new_user.roll_number, new_user.name, new_user.course, new_user.password);
        
        // Create a new node for the user
        UserNode* new_node = (UserNode*)malloc(sizeof(UserNode));
        new_node->user = new_user;
        new_node->next = NULL;

        // If the list is empty, set new_node as the head
        if (*user_head == NULL) {
            *user_head = new_node;
        } else {
            // Otherwise, append to the end of the list
            UserNode* temp = *user_head;
            while (temp->next != NULL) {
                temp = temp->next;
            }
            temp->next = new_node;
        }
    }
    fclose(file);
}

// Function to load exam data from CSV file into a linked list
void load_exam_data(ExamNode** exam_head) {
    FILE* file = fopen("exams.csv", "r");
    if (!file) {
        printf("\033[1;31mError opening exams file!\033[0m\n");
        return;
    }

    char line[250];  
    while (fgets(line, sizeof(line), file)) {
        Exam new_exam;
        // Loading exam data from the CSV 
        sscanf(line, "%[^,],%[^,],%[^,],%d,%d", new_exam.exam_code, new_exam.subject, new_exam.exam_date, &new_exam.total_marks, &new_exam.total_students);
        
        // Create a new node for the exam
        ExamNode* new_node = (ExamNode*)malloc(sizeof(ExamNode));
        new_node->exam = new_exam;
        new_node->next = *exam_head;
        *exam_head = new_node;
    }
    fclose(file);
}

// Function to save user data to the CSV file
void save_user_data(const char* roll_number, const char* name, const char* course, const char* password) {
    FILE* file = fopen("users.csv", "a");
    if (!file) {
        printf("\033[1;31mError opening users file for saving!\033[0m\n");
        return;
    }
    // Write user data in the format: roll_number,name,course,password
    fprintf(file, "%s,%s,%s,%s\n", roll_number, name, course, password);
    fclose(file);
}

// Function to save exam data to the CSV file
void save_exam_data(const Exam* exam) {
    FILE* file = fopen("exams.csv", "a");
    if (!file) {
        printf("\033[1;31mError opening exams file for saving!\033[0m\n");
        return;
    }
    // Write exam data in the format: exam_code,subject,exam_date,total_marks,total_students
    fprintf(file, "%s,%s,%s,%d,%d\n", exam->exam_code, exam->subject, exam->exam_date, exam->total_marks, exam->total_students);
    fclose(file);
}

// Function to delete an exam's data from the linked list and CSV
void delete_exam_data(const char* exam_name) {
    ExamNode *temp = exam_head, *prev = NULL;
    
    // Check if the head node itself holds the exam to be deleted
    if (temp != NULL && strcmp(temp->exam.exam_code, exam_name) == 0) {
        exam_head = temp->next;  // Move head to next node
        free(temp);
        re_save_exams_to_csv();  // Re-save to CSV
        return;
    }

    // Search for the exam to be deleted
    while (temp != NULL && strcmp(temp->exam.exam_code, exam_name) != 0) {
        prev = temp;
        temp = temp->next;
    }

    // If exam was not found
    if (temp == NULL) return;

    // Unlink the node from the linked list
    prev->next = temp->next;
    free(temp);

    // Re-save exams to CSV
    re_save_exams_to_csv();
}

// Function to delete a user's data from the linked list and CSV
void delete_user_data(const char* roll_number) {
    UserNode *temp = user_head, *prev = NULL;

    // Check if the head node itself holds the user to be deleted
    if (temp != NULL && strcmp(temp->user.roll_number, roll_number) == 0) {
        user_head = temp->next;  // Move head to next node
        free(temp);
        re_save_users_to_csv();  // Re-save to CSV
        return;
    }

    // Search for the user to be deleted
    while (temp != NULL && strcmp(temp->user.roll_number, roll_number) != 0) {
        prev = temp;
        temp = temp->next;
    }

    // If user was not found
    if (temp == NULL) return;

    // Unlink the node from the linked list
    prev->next = temp->next;
    free(temp);

    // Re-save users to CSV
    re_save_users_to_csv();
}

// Function to read student results from results.csv
int read_results(const char* roll_number, const char* exam_code, int* student_marks) {
    FILE* file = fopen("results.csv", "r");
    if (!file) {
        printf("\033[1;31mError opening results file!\033[0m\n");
        return 0;  // Error opening file
    }

    char line[150];
    while (fgets(line, sizeof(line), file)) {
        char roll[15], exam[50];
        int marks;

        // Read the line in CSV format (roll_number, exam_code, marks)
        sscanf(line, "%[^,],%[^,],%d", roll, exam, &marks);

        if (strcmp(roll, roll_number) == 0 && strcmp(exam, exam_code) == 0) {
            *student_marks = marks;
            fclose(file);
            return 1;  // Successfully found result
        }
    }

    fclose(file);
    return 0;  // No result found
}

// Function to get the class average and highest marks for a specific exam
void get_exam_statistics(const char* exam_code, int* class_avg, int* highest_marks) {
    FILE* file = fopen("results.csv", "r");
    if (!file) {
        printf("\033[1;31mError opening results file!\033[0m\n");
        return;
    }

    int total_marks = 0, num_students = 0, max_marks = 0;
    
    char line[150];
    while (fgets(line, sizeof(line), file)) {
        char exam[50];
        int marks;

        sscanf(line, "%*[^,],%[^,],%d", exam, &marks);
        
        if (strcmp(exam, exam_code) == 0) {
            total_marks += marks;
            if (marks > max_marks) {
                max_marks = marks;
            }
            num_students++;
        }
    }

    if (num_students > 0) {
        *class_avg = total_marks / num_students;
        *highest_marks = max_marks;
    }

    fclose(file);
}
