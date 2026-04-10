#ifndef EXAM_H
#define EXAM_H

// Define the Exam structure
typedef struct {
    char exam_code[50];
    char subject[50];
    char exam_date[11];  // Format: DD-MM-YYYY
    int total_marks;
    int total_students;
} Exam;

// Define the ExamNode structure for the linked list
typedef struct ExamNode {
    Exam exam;
    struct ExamNode* next;  // Pointer to the next node
} ExamNode;

// Declare the global linked list head pointer for exams
extern ExamNode* exam_head;

// Function declarations
void add_exam();                              // Function to add a new exam
void delete_exam();                           // Function to delete an exam by name
void view_exams();                            // Function to view all exams
void search_exam_by_year();                   // Function to search exams by year
void search_exam_by_subject();                // Function to search exams by subject
void re_save_exams_to_csv();                  // Function to re-save all exams to CSV after deletion or updates

#endif  // EXAM_H
