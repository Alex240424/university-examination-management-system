// user.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "user.h"
#include "file_io.h"
#include "menu.h"
#include <ctype.h>

// Initialize the global user list
UserNode* user_head = NULL;

// Function to validate roll number (e.g., BT1234567)
int is_valid_roll_number(const char* roll_number) {
    // Roll number must be in the format: Two uppercase letters followed by seven digits
    if (strlen(roll_number) != 9) {
        return 0; // Invalid length
    }
    
    // Check first two characters (must be uppercase letters)
    for (int i = 0; i < 2; i++) {
        if (!isupper(roll_number[i])) {
            return 0; // Not an uppercase letter
        }
    }

    // Check the remaining characters (must be digits)
    for (int i = 2; i < 9; i++) {
        if (!isdigit(roll_number[i])) {
            return 0; // Not a digit
        }
    }

    return 1; // Valid roll number
}


// Function to get the total marks for a given exam
int get_max_marks_for_exam(const char* exam_code) {
    FILE* file = fopen("exams.csv", "r");
    if (!file) {
        printf("Error opening exams file!\n");
        return -1;  // Error opening file
    }

    char line[MAX_LINE_LENGTH];
    int max_marks = -1;
    
    // Search for the exam in the exams.csv and get the total marks
    while (fgets(line, sizeof(line), file)) {
        char code[50], subject[50], date[50];
        int total_marks, total_students;

        // Parse each line with comma separation
        if (sscanf(line, "%49[^,],%49[^,],%49[^,],%d,%d", code, subject, date, &total_marks, &total_students) == 5) {
            // Check if this is the correct exam code
            if (strcmp(code, exam_code) == 0) {
                max_marks = total_marks;  // Set max_marks to the total marks for the exam
                break;
            }
        }
    }

    fclose(file);
    return max_marks;
}

// Trim leading and trailing whitespaces
void trim_whitespace(char* str) {
    int end;
    while (isspace((unsigned char)*str)) str++;  // Trim leading spaces
    end = strlen(str) - 1;
    while (end >= 0 && isspace((unsigned char)str[end])) end--;  // Trim trailing spaces
    str[end + 1] = '\0';  // Null-terminate the string
}

// Clear the input buffer to avoid leftover characters (such as newline)
void clear_input_buffer() {
    while (getchar() != '\n');  // Discard until we reach the newline
}

// Function to check if the exam code is valid by checking in exams.csv
int is_valid_exam_code(const char* exam_code) {
    FILE* exams_file = fopen("exams.csv", "r");
    if (!exams_file) {
        printf("Error opening exams file!\n");
        return 0;  // Error opening file
    }

    char line[200];
    while (fgets(line, sizeof(line), exams_file)) {
        char code[50];
        sscanf(line, "%[^,],", code);  // Read the exam code from the file

        // Check if the entered exam code matches the code from the file
        if (strcmp(exam_code, code) == 0) {
            fclose(exams_file);
            return 1;  // Exam code found
        }
    }

    fclose(exams_file);
    return 0;  // Exam code not found
}

// Function to check if the course is valid by checking in users.csv
int is_course_valid(const char* course) {
    FILE* users_file = fopen("users.csv", "r");
    if (!users_file) {
        printf("Error opening users file!\n");
        return 0;  // Error opening file
    }

    char line[200];
    while (fgets(line, sizeof(line), users_file)) {
        char user_course[50];
        sscanf(line, "%*[^,],%*[^,],%[^,],", user_course);  // Extract course from the file

        // Check if the entered course matches any course in users.csv
        if (strcmp(course, user_course) == 0) {
            fclose(users_file);
            return 1;  // Course found
        }
    }

    fclose(users_file);
    return 0;  // Course not found
}

// Function to check if the result for a student already exists in results.csv
int result_exists(const char* roll_number, const char* exam_code) {
    FILE* results_file = fopen("results.csv", "r");
    if (!results_file) {
        printf("Error opening results file!\n");
        return 0;  // Error opening file
    }

    char line[200];
    while (fgets(line, sizeof(line), results_file)) {
        char stored_roll_number[15], stored_exam_code[50];
        sscanf(line, "%[^,],%[^,],", stored_roll_number, stored_exam_code);  // Extract roll_number and exam_code

        // Check if both roll_number and exam_code match
        if (strcmp(roll_number, stored_roll_number) == 0 && strcmp(exam_code, stored_exam_code) == 0) {
            fclose(results_file);
            return 1;  // Result already exists
        }
    }

    fclose(results_file);
    return 0;  // No result found
}

// Function to add results for students in a particular course
void add_results() {
    char exam_code[50], course[50];
    int valid_exam = 0, valid_course = 0;

    // Step 1: Ask for the exam code and validate
    while (!valid_exam) {
        printf("Enter the exam code (e.g., CS101): ");
        scanf("%s", exam_code);

        // Validate the exam code by checking if it exists in exams.csv
        if (!is_valid_exam_code(exam_code)) {
            printf("This exam code does not exist in the exams list.\n");
            continue;  // Re-prompt for the exam code
        }

        valid_exam = 1;  // Proceed if valid
    }

    // Step 2: Ask for the course and validate
    while (!valid_course) {
        printf("Enter the course name (e.g., B.Tech): ");
        scanf(" %[^\n]%*c", course);  // Allows spaces in course name

        if (!is_course_valid(course)) {
            printf("Invalid course name. Please enter a valid course.\n");
            continue;  // Re-ask for course if invalid
        }

        valid_course = 1;  // Proceed if valid
    }

    // Step 3: Get the list of students in the course
    FILE* user_file = fopen("users.csv", "r");
    if (!user_file) {
        printf("Error opening users file!\n");
        return;
    }

    char line[200];
    char roll_numbers[300][15];  // Array to store roll numbers of students in the course
    int student_count = 0;

    // Read the users.csv and store roll numbers of students in the given course
    while (fgets(line, sizeof(line), user_file)) {
        char roll_number[15], student_course[50];
        sscanf(line, "%[^,],%*[^,],%[^,],", roll_number, student_course);  // Extract roll_number and course

        // Check if this student belongs to the course
        if (strcmp(student_course, course) == 0) {
            strcpy(roll_numbers[student_count], roll_number);
            student_count++;
        }
    }

    fclose(user_file);

    if (student_count == 0) {
        printf("No students found in this course.\n");
        return;  // Exit if no students are enrolled in the course
    }

    // Step 4: Open results.csv file to append results
    FILE* results_file = fopen("results.csv", "a");
    if (!results_file) {
        printf("Error opening results file!\n");
        return;
    }

    // Step 5: Ask for marks for each student in the batch
    int new_marks;
    for (int i = 0; i < student_count; i++) {
        // Check if result already exists for this student
        if (result_exists(roll_numbers[i], exam_code)) {
            printf("Results have already been added for %s in exam %s.\n", roll_numbers[i], exam_code);
            continue;  // Skip to the next student if result already exists
        }

        // Get the marks for each student
        while (1) {
            printf("Marks for %s in exam %s: ", roll_numbers[i], exam_code);
            scanf("%d", &new_marks);

            // Validate marks
            if (new_marks < 0) {
                printf("Invalid marks. Marks must be positive.\n");
            } else {
                break;  // Proceed if valid marks
            }
        }

        // Write the result to the CSV file
        fprintf(results_file, "%s,%s,%d\n", roll_numbers[i], exam_code, new_marks);
    }

    fclose(results_file);
    printf("Results added successfully!\n");
    // Add a function to pause before returning to the admin menu
    press_any_key_to_continue();
    handle_admin_menu();  // Go back to admin menu after adding results
}

/// Function to register a new user (student) with error handling for roll number only
void register_user() {
    User new_user;

    // Validate roll number
    int valid_roll = 0;
    while (!valid_roll) {
        printf("Enter roll number (format: BTXXXXXXX): ");
        scanf("%s", new_user.roll_number);

        // Check if roll number already exists in the users.csv file
        FILE* file = fopen("users.csv", "r");
        if (!file) {
            printf("Error opening users file!\n");
            press_any_key_to_continue();
            handle_admin_menu();
            return;
        }

        char line[MAX_LINE_LENGTH];
        int roll_exists = 0;
        
        // Read through the file to check if the roll number already exists
        while (fgets(line, sizeof(line), file)) {
            char existing_roll[15];
            sscanf(line, "%[^,],", existing_roll);  // Extract roll number from the line

            // Check if the entered roll number matches any in the file
            if (strcmp(existing_roll, new_user.roll_number) == 0) {
                roll_exists = 1;
                break;  // Roll number already exists, break out of the loop
            }
        }
        fclose(file);

        if (roll_exists) {
            printf("Error: Roll number %s already exists! Please enter a different roll number.\n", new_user.roll_number);
            press_any_key_to_continue();
            handle_admin_menu();  // Return to the admin menu if roll number exists
            return;  // Exit the function if roll number exists
        }

        // If no duplicate roll number found, proceed
        if (is_valid_roll_number(new_user.roll_number)) {
            valid_roll = 1;
        } else {
            printf("Invalid roll number format! Please enter in the format: BT1234567\n");
            press_any_key_to_continue();
            handle_admin_menu();  // Return to admin menu if format is invalid
            return;
        }
    }

    // Proceed with the remaining registration (roll number validated)
    printf("Enter name: ");
    scanf(" %[^\n]%*c", new_user.name);  // Capture spaces in name
    printf("Enter course name: ");
    scanf(" %[^\n]%*c", new_user.course);  // Capture spaces in course name
    printf("Enter password: ");
    scanf("%s", new_user.password);

    // Create a new node for the linked list (assuming you have a UserNode type)
    UserNode* new_node = (UserNode*)malloc(sizeof(UserNode));
    new_node->user = new_user;
    new_node->next = NULL;

    // If the list is empty, set the new node as head
    if (user_head == NULL) {
        user_head = new_node;
    } else {
        // Otherwise, append it to the list
        UserNode* temp = user_head;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = new_node;
    }

    // Save user data to file
    save_user_data(new_user.roll_number, new_user.name, new_user.course, new_user.password);
    printf("User registered successfully!\n");
    press_any_key_to_continue();
    handle_admin_menu();  // After registration, return to admin menu
}



// Function to authenticate a user by roll number and password
int authenticate_user(char* roll_number, char* password, UserNode** user_head) {
    int attempts = 0;
    const int max_attempts = 3;
    
    while (attempts < max_attempts) {
        printf("\033[1;33mEnter Roll Number: \033[0m");
        scanf("%s", roll_number);
        printf("\033[1;33mEnter password: \033[0m");
        scanf("%s", password);

        // Trim any whitespace from the input to avoid mismatch due to leading/trailing spaces
        trim_whitespace(roll_number);
        trim_whitespace(password);

        // Reset current to the head of the user list at the beginning of each attempt
        UserNode* current = *user_head;
        
        // Check if credentials are valid for any student
        while (current != NULL) {
           
            // Trim any whitespace from the stored user roll number and password
            trim_whitespace(current->user.roll_number);
            trim_whitespace(current->user.password);

            if (strcmp(current->user.roll_number, roll_number) == 0 && 
                strcmp(current->user.password, password) == 0) {
                printf("\033[1;32mStudent login successful!\n\033[0m");
                return 2;  // Student login successful
            }
            current = current->next;
        }

        // Increment attempt counter
        attempts++;

        // Provide feedback on remaining attempts
        if (attempts < max_attempts) {
            printf("\033[1;31mInvalid credentials. You have %d attempt(s) remaining.\n\033[0m", max_attempts - attempts);
        }
    }

    // If user reaches the maximum attempts
    printf("\033[1;31mExiting the program... Too many invalid attempts.\n\033[0m");
    exit(0);  // Exit the program after 3 invalid attempts
}

// Function to view all users (students)
void view_users() {
    UserNode* temp = user_head;
    if (temp == NULL) {
        printf("\033[1;31mNo users found!\n\033[0m");
        return;
    }
    printf("Users List:\n");
    while (temp != NULL) {
        printf("Roll Number: %s, Name: %s, Course: %s\n", 
               temp->user.roll_number, temp->user.name, temp->user.course);
        temp = temp->next;
    }
}

// Function to delete a user (student) by roll number
void delete_user() {
    // Display the list of users
    UserNode* temp = user_head;
    int index = 1;
    if (temp == NULL) {
        printf("No users available to delete.\n");
        return;
    }

    printf("List of Users:\n");
    while (temp != NULL) {
        printf("%d. Roll Number: %s, Name: %s, Course: %s\n", 
               index, temp->user.roll_number, temp->user.name, temp->user.course);
        temp = temp->next;
        index++;
    }

    // Ask the admin to select a user
    int choice;
    printf("Enter the number of the user you want to delete: ");
    scanf("%d", &choice);

    // Find the selected user in the list
    temp = user_head;
    int count = 1;
    UserNode* selected_user = NULL;
    while (temp != NULL) {
        if (count == choice) {
            selected_user = temp;
            break;
        }
        temp = temp->next;
        count++;
    }

    if (selected_user == NULL) {
        printf("Invalid choice. No user deleted.\n");
        return;
    }

    // Ask for confirmation
    char confirm;
    printf("Are you sure you want to delete the user: %s (Y/N)? ", selected_user->user.roll_number);
    scanf(" %c", &confirm);  // Note the space before %c to consume any leftover newline character

    if (confirm == 'Y' || confirm == 'y') {
        delete_user_data(selected_user->user.roll_number);
        printf("User deleted successfully!\n");
    } else {
        printf("User deletion cancelled.\n");
    }

    // Wait for the user to press any key to return to the admin menu
    printf("\nPress any key to continue...\n");
    getchar();  // Consume any leftover newline character
    getchar();  // Wait for the user to press a key

    // Return to the admin menu (you can call the admin menu function or navigate back)
    handle_admin_menu();
}

// Function to re-save all users to the CSV after deletion or updates
void re_save_users_to_csv() {
    FILE* file = fopen("users.csv", "w");
    if (!file) {
        printf("Error opening file for saving user data!\n");
        press_any_key_to_continue();
        handle_admin_menu();        
        return;
    }

    UserNode* temp = user_head;
    while (temp != NULL) {
        // Write user data back to the file in the format: roll_number,name,course,password
        fprintf(file, "%s,%s,%s,%s\n", temp->user.roll_number, temp->user.name, temp->user.course, temp->user.password);
        temp = temp->next;
    }
    fclose(file);
}

void view_results(const char* roll_number) {
    ExamNode* exam_temp = exam_head;

    // Check if any exams exist for the student
    int exam_count = 0;
    printf("Exams you have appeared for:\n");

    // We will store exam codes to avoid printing duplicates
    char displayed_exams[100][20]; // Array to store displayed exam codes
    int num_displayed = 0;

    while (exam_temp != NULL) {
        int student_marks = 0;
        // Check if the student has appeared for this exam
        if (read_results(roll_number, exam_temp->exam.exam_code, &student_marks)) {
            int already_displayed = 0;
            // Check if this exam has already been displayed
            for (int i = 0; i < num_displayed; i++) {
                if (strcmp(exam_temp->exam.exam_code, displayed_exams[i]) == 0) {
                    already_displayed = 1;
                    break;
                }
            }

            // If not already displayed, print it and mark as displayed
            if (!already_displayed) {
                printf("%d. %s - %s\n", ++exam_count, exam_temp->exam.exam_code, exam_temp->exam.subject);
                strcpy(displayed_exams[num_displayed], exam_temp->exam.exam_code);
                num_displayed++;
            }
        }
        exam_temp = exam_temp->next;
    }

    if (exam_count == 0) {
        printf("No results available for you.\n");
        press_any_key_to_continue();
        handle_student_menu(roll_number);  // Return to the student menu
        return;
    }

    // Ask the student to select an exam
    int choice;
    printf("\nEnter the exam number to view results: ");
    scanf("%d", &choice);

    // Validate the choice
    if (choice < 1 || choice > exam_count) {
        printf("Invalid choice. Please try again.\n");
        press_any_key_to_continue();
        handle_student_menu(roll_number);  // Return to the student menu
        return;
    }

    // Now we have the exam code for the selected exam number
    const char* selected_exam_code = displayed_exams[choice - 1];

    // Reset exam_temp to the start of the exam list to find the selected exam
    exam_temp = exam_head;
    while (exam_temp != NULL) {
        // If the current exam matches the selected exam code
        if (strcmp(exam_temp->exam.exam_code, selected_exam_code) == 0) {
            int student_marks = 0;
            // Now that we have the correct exam, read the results for that exam
            if (read_results(roll_number, exam_temp->exam.exam_code, &student_marks)) {
                int class_avg = 0, highest_marks = 0;
                get_exam_statistics(exam_temp->exam.exam_code, &class_avg, &highest_marks);

                printf("\n\033[1;33mExam:\033[0m \033[1;32m%s\033[0m - \033[1;34m%s\033[0m\n", 
                    exam_temp->exam.exam_code, exam_temp->exam.subject);
            printf("\033[1;36mYour Marks:\033[0m \033[1;32m%d\033[0m\n", student_marks);
            printf("\033[1;36mTotal Marks:\033[0m \033[1;32m%d\033[0m\n", exam_temp->exam.total_marks);
            printf("\033[1;36mClass Average:\033[0m \033[1;32m%d\033[0m\n", class_avg);
            printf("\033[1;36mHighest Marks:\033[0m \033[1;32m%d\033[0m\n", highest_marks);

            }
            break;
        }
        exam_temp = exam_temp->next;
    }

    // Wait for user to press any key to continue and return to the student menu
    press_any_key_to_continue();
    handle_student_menu(roll_number);  // Return to the student menu
}

// Function to view results of a specific exam
void view_exam_results(const char* exam_code) {
    FILE* file = fopen("results.csv", "r");
    if (!file) {
        printf("Error opening results file!\n");
        press_any_key_to_continue();
        handle_admin_menu();
        return;
    }

    // Arrays to store the data
    char roll_numbers[MAX_RESULTS][15];  // Array to store roll numbers
    int marks[MAX_RESULTS];               // Array to store marks
    int count = 0;                        // Number of results found for the selected exam

    // Read the results for the selected exam
    char line[MAX_LINE_LENGTH];
    while (fgets(line, sizeof(line), file)) {
        char exam[50];
        int student_marks;
        sscanf(line, "%[^,],%[^,],%d", roll_numbers[count], exam, &student_marks);

        if (strcmp(exam, exam_code) == 0) {
            strcpy(roll_numbers[count], strtok(line, ","));  // Extract roll number
            marks[count] = student_marks;  // Store marks
            count++;
        }
    }

    fclose(file);

    // Display results in the format: roll_number, exam_code, marks
    printf("\nResults for Exam: %s\n", exam_code);
    for (int i = 0; i < count; i++) {
        printf("%s, %s: %d marks\n", roll_numbers[i], exam_code, marks[i]);
    }

    // Get and display statistics
    int class_avg = 0, highest_marks = 0;
    get_exam_statistics(exam_code, &class_avg, &highest_marks);
    printf("\033[1;34mClass Average: \033[1;32m%d\n\033[0m", class_avg);
    printf("\033[1;34mHighest Marks: \033[1;32m%d\n\033[0m", highest_marks);

    // Ask if admin wants to sort the results by marks
    char sort_choice;
    printf("\033[1;34m\nDo you want to view the sorted results by marks (Highest to Lowest)? (Y/n): \033[0m");
    scanf(" %c", &sort_choice);

    if (sort_choice == 'Y' || sort_choice == 'y') {
        // Sort results by marks in descending order using a simple bubble sort
        for (int i = 0; i < count - 1; i++) {
            for (int j = i + 1; j < count; j++) {
                if (marks[i] < marks[j]) {
                    // Swap marks
                    int temp_marks = marks[i];
                    marks[i] = marks[j];
                    marks[j] = temp_marks;

                    // Swap roll numbers to maintain alignment
                    char temp_roll[15];
                    strcpy(temp_roll, roll_numbers[i]);
                    strcpy(roll_numbers[i], roll_numbers[j]);
                    strcpy(roll_numbers[j], temp_roll);
                }
            }
        }

        // Display sorted results
        printf("\nSorted Results (Highest to Lowest):\n");
        for (int i = 0; i < count; i++) {
            printf("%s: \033[1;32m%d marks\033[0m\n", roll_numbers[i], marks[i]);
        }
        press_any_key_to_continue();
        handle_admin_menu();
    }
    else{
        press_any_key_to_continue();
        handle_admin_menu();
    }
}

// Function to display a menu of exams and allow admin to select one
void view_results_of_exam() {
    // Display available exams
    printf("\nAvailable Exams:\n");
    
    ExamNode* exam_temp = exam_head;  // Assuming exam_head is the head of the linked list of exams
    int exam_count = 0;

    while (exam_temp != NULL) {
        exam_count++;
         printf("\033[1;36m%d. \033[0m%s - %s\n", exam_count, exam_temp->exam.exam_code, exam_temp->exam.subject);
        exam_temp = exam_temp->next;
    }

    if (exam_count == 0) {
        printf("No exams found.\n");
        press_any_key_to_continue();
        handle_admin_menu();
        return;
    }

    // Get the admin's choice
    int exam_choice;
    printf("\nEnter the exam number to view results: ");
    scanf("%d", &exam_choice);

    if (exam_choice < 1 || exam_choice > exam_count) {
        printf("Invalid choice. Returning to Admin Menu.\n");
        press_any_key_to_continue();
        handle_admin_menu();
        return;
    }

    // Reset exam_temp to the head of the linked list to find the selected exam
    exam_temp = exam_head;
    for (int i = 1; i < exam_choice; i++) {
        exam_temp = exam_temp->next;
    }

    // Display the results for the selected exam
    view_exam_results(exam_temp->exam.exam_code);
}

// Function to view results of a student (Admin version)
void view_results_of_a_student() {
    char roll_number[15];
    printf("Enter the roll number of the student (e.g., BT1234567): ");
    scanf("%s", roll_number);

    // Validate roll number format
    if (!is_valid_roll_number(roll_number)) {
        printf("Invalid roll number format. Returning to Admin Menu...\n");
        press_any_key_to_continue();
        handle_admin_menu();
        return;
    }

    // Open the results file to search for the student's results
    FILE* file = fopen("results.csv", "r");
    if (!file) {
        printf("Error opening results file!\n");
        press_any_key_to_continue();
        handle_admin_menu();
        return;
    }

    // Arrays to store data of exams and marks
    char exam_codes[MAX_LINE_LENGTH][50];
    int marks[MAX_LINE_LENGTH];
    int count = 0;

    // Read through the results and find the student's results
    char line[MAX_LINE_LENGTH];
    while (fgets(line, sizeof(line), file)) {
        char student_roll[15], exam_code[50];
        int student_marks;

        // Parse the line (roll_number, exam_code, marks)
        sscanf(line, "%[^,],%[^,],%d", student_roll, exam_code, &student_marks);

        // Check if the current line is for the student with the entered roll number
        if (strcmp(student_roll, roll_number) == 0) {
            // Store the exam code and marks for this student
            strcpy(exam_codes[count], exam_code);
            marks[count] = student_marks;
            count++;
        }
    }

    fclose(file);

    // Check if the student has appeared for any exams
    if (count == 0) {
        printf("No results found for the student with roll number %s.\n", roll_number);
        press_any_key_to_continue();
        handle_admin_menu();
        return;
    }

    // Display results for the student
    printf("\033[1;34m\nResults for Student %s:\033[0m\n", roll_number);
    for (int i = 0; i < count; i++) {
        printf("Exam Code: \033[1;36m%s\033[0m - Marks: \033[1;32m%d\033[0m\n", exam_codes[i], marks[i]);
    }

    press_any_key_to_continue();
    handle_admin_menu();
}

// Function to update the results of a student for a specific exam
void update_results() {
    char roll_number[15], exam_code[50];
    int new_marks;
    int attempts = 3;

    // Step 1: Input and validate roll number
    int roll_number_valid = 0;
    while (attempts > 0 && !roll_number_valid) {
        printf("Enter the roll number of the student (e.g., BT1234567): ");
        scanf("%s", roll_number);
        if (is_valid_roll_number(roll_number)) {
            roll_number_valid = 1;  // Valid roll number entered
        } else {
            printf("Invalid roll number format. You have %d attempts left.\n", --attempts);
        }
    }
    if (!roll_number_valid) {
        printf("Invalid roll number format after 3 attempts. Returning to Admin Menu...\n");
        press_any_key_to_continue();
        handle_admin_menu();
        return;
    }

    // Step 2: Input and validate exam code
    int exam_code_valid = 0;
    attempts = 3;  // Reset attempts for exam code validation
    while (attempts > 0 && !exam_code_valid) {
        printf("Enter the exam code (e.g., CS101): ");
        scanf("%s", exam_code);
        if (is_valid_exam_code(exam_code)) {
            exam_code_valid = 1;  // Valid exam code entered
        } else {
            printf("Invalid exam code. You have %d attempts left.\n", --attempts);
        }
    }
    if (!exam_code_valid) {
        printf("Invalid exam code after 3 attempts. Returning to Admin Menu...\n");
        press_any_key_to_continue();
        handle_admin_menu();
        return;
    }

    // Step 3: Get the maximum marks for the given exam
    int max_marks = get_max_marks_for_exam(exam_code);
    if (max_marks == -1) {
        printf("Error retrieving max marks for the exam. Returning to Admin Menu...\n");
        press_any_key_to_continue();
        handle_admin_menu();
        return;
    }

    // Step 4: Input new marks and validate
    int marks_valid = 0;
    attempts = 3;  // Reset attempts for marks validation
    while (attempts > 0 && !marks_valid) {
        printf("Enter the new marks (0 to %d): ", max_marks);
        scanf("%d", &new_marks);

        // Validate marks (should be non-negative and <= max marks)
        if (new_marks >= 0 && new_marks <= max_marks) {
            marks_valid = 1;  // Valid marks entered
        } else {
            printf("Invalid marks. Marks must be between 0 and %d. You have %d attempts left.\n", max_marks, --attempts);
        }
    }
    if (!marks_valid) {
        printf("Invalid marks entered after 3 attempts. Returning to Admin Menu...\n");
        press_any_key_to_continue();
        handle_admin_menu();
        return;
    }

    // Step 5: Update the marks in the results.csv
    FILE* file = fopen("results.csv", "r+");
    if (!file) {
        printf("Error opening results file!\n");
        press_any_key_to_continue();
        handle_admin_menu();
        return;
    }

    char line[MAX_LINE_LENGTH];
    long position;
    int found = 0;

    // Search for the student result to update
    while (fgets(line, sizeof(line), file)) {
        char existing_roll[15], existing_exam[50];
        int existing_marks;
        position = ftell(file) - strlen(line);  // Save the current position to update later

        // Read the data from the current line
        sscanf(line, "%[^,],%[^,],%d", existing_roll, existing_exam, &existing_marks);

        // If we find the matching roll number and exam code, update the marks
        if (strcmp(existing_roll, roll_number) == 0 && strcmp(existing_exam, exam_code) == 0) {
            found = 1;
            fseek(file, position, SEEK_SET);  // Move back to the position of the record
            fprintf(file, "%s,%s,%d\n", roll_number, exam_code, new_marks);  // Write the updated marks
            break;
        }
    }

    fclose(file);

    if (found) {
        printf("Marks updated successfully for %s in %s.\n", roll_number, exam_code);
        press_any_key_to_continue();
        handle_admin_menu();
    } else {
        printf("No results found for the student in the specified exam.\n");
        press_any_key_to_continue();
        handle_admin_menu();
    }
    
    press_any_key_to_continue();
    handle_admin_menu();
}

// Function to delete the result from the CSV file
void delete_result() {
    char roll_number[15], exam_code[50];
    char line[MAX_LINE_LENGTH], current_roll[15], current_exam_code[50];
    int found = 0;

    // Ask for student roll number and exam code
    printf("Enter the student roll number: ");
    scanf("%s", roll_number);
    printf("Enter the exam code: ");
    scanf("%s", exam_code);

    FILE *results_file = fopen("results.csv", "r");
    if (!results_file) {
        printf("Error opening results file.\n");
        return;
    }

    // Create a temporary file to store the updated results
    FILE *temp_file = fopen("temp_results.csv", "w");
    if (!temp_file) {
        printf("Error creating temporary file.\n");
        fclose(results_file);
        return;
    }

    // Read through the results.csv line by line
    while (fgets(line, sizeof(line), results_file)) {
        // Remove newline at the end of the line if it exists
        trim_whitespace(line);

        // Parse the roll number and exam code from the current line
        sscanf(line, "%[^,],%[^,],", current_roll, current_exam_code);

        // Trim any possible whitespace after reading from the line
        trim_whitespace(current_roll);
        trim_whitespace(current_exam_code);

        // Check if the current line matches the roll number and exam code
        if (strcmp(current_roll, roll_number) == 0 && strcmp(current_exam_code, exam_code) == 0) {
            found = 1;
            // Ask for confirmation to delete
            char confirm;
            printf("Do you want to proceed with deleting the result for %s in exam %s? (Y/n): ", roll_number, exam_code);
            scanf(" %c", &confirm);  // Space before %c to handle previous newline

            if (confirm == 'Y' || confirm == 'y') {
                printf("Result of %s deleted successfully.\n", roll_number);
                // Skip writing this line to the temporary file (effectively deleting it)
                continue;
            } else {
                printf("Deletion canceled for %s in exam %s.\n", roll_number, exam_code);
            }
        }

        // Write the line to the temporary file with proper newline at the end
        fprintf(temp_file, "%s\n", line);
    }

    // If the result was not found
    if (!found) {
        printf("Result not found for %s in exam %s.\n", roll_number, exam_code);
    }

    // Close the files
    fclose(results_file);
    fclose(temp_file);

    // Replace the original results.csv with the temporary file
    remove("results.csv");
    rename("temp_results.csv", "results.csv");
}