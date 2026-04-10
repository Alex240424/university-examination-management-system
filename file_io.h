#ifndef FILE_IO_H
#define FILE_IO_H

#include "user.h"
#include "exam.h"

// Function declarations for loading and saving data
void delete_user_data(const char* roll_number);
// Load user data from the CSV file into a linked list
void load_user_data(UserNode** user_head);

// Load exam data from the CSV file into a linked list
void load_exam_data(ExamNode** exam_head);

// Save user data to the CSV file
void save_user_data(const char* roll_number, const char* name, const char* course, const char* password);

// Save exam data to the CSV file
void save_exam_data(const Exam* exam);

// Delete an exam from the linked list and the CSV file
void delete_exam_data(const char* exam_name);
void get_exam_statistics(const char* exam_code, int* class_avg, int* highest_marks);
int read_results(const char* roll_number, const char* exam_code, int* student_marks);

#endif  // FILE_IO_H
