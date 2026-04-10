#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "user.h"
#include "exam.h"  // Ensure this is included
#include "menu.h"
#include "file_io.h"

// Function to display the login menu and handle user authentication
void login_menu() {
    int choice;
    const int max_attempts = 3;

    while (1) {
        // Clear the screen before each new menu display
        printf("\033[H\033[J");  // ANSI escape code to clear the terminal screen

        printf("\033[1;34m\u2022=================================================================\u2022\n");
        printf("   \033[1;32mWelcome to the University Examination Management System (UEMS)\033[0m\n");
        printf("\033[1;34m\u2022=================================================================\u2022\n");
        printf("\033[1;33mPlease select an option to continue:\033[0m\n");
        printf("\033[1;35m1. \033[1;36mLogin as Admin\033[0m\n");  
        printf("\033[1;35m2. \033[1;36mLogin as Student\033[0m\n");  
        printf("\033[1;31m0. \033[1;31mExit\033[0m\n");   
        printf("\033[1;33mEnter your choice: \033[0m");

        // Validate input and ensure we only get a valid integer
        if (scanf("%d", &choice) != 1) {
            
            while (getchar() != '\n');  // Clear input buffer
            printf("\033[1;31mInvalid input. Please enter a valid integer.\033[0m\n");
            continue;  
        }

        // Check if the choice is within the valid range (0 to 2)
        if (choice < 0 || choice > 2) {
            printf("\033[1;31mInvalid choice. Please select a valid option (0, 1, or 2).\033[0m\n");
            continue;  
        }

        switch (choice) {
            case 1: // Admin login
                {
                    // Admin credentials
                    const char *admin_username = "admin";
                    const char *admin_password = "admin123";
                    char username[50], password[50];
                    int attempts = 0;

                    printf("\n\033[1;34m===== Admin Login =====\033[0m\n");

                    while (attempts < max_attempts) {
                        printf("\033[1;33mEnter admin username: \033[0m");
                        scanf("%s", username);
                        printf("\033[1;33mEnter admin password: \033[0m");
                        scanf("%s", password);

                        // Check if admin credentials match
                        if (strcmp(username, admin_username) == 0 && strcmp(password, admin_password) == 0) {
                            printf("\033[1;32mAdmin login successful!\033[0m\n");
                            press_any_key_to_continue();
                            handle_admin_menu();  
                            return;  
                        } else {
                            printf("\033[1;31mInvalid admin credentials. You have %d attempt(s) remaining.\033[0m\n", max_attempts - attempts - 1);
                        }

                        attempts++;
                    }

                    printf("\033[1;31mToo many invalid attempts. Exiting...\033[0m\n");
                    exit(0);  // Exit the program after 3 invalid attempts
                }
                break;

            case 2: // Student login
                {
                  
                    char roll_number[15], password[50];
                    int attempts = 0;

                    printf("\n\033[1;34m===== Student Login =====\033[0m\n");

                    while (attempts < max_attempts) {
                        int login_result = authenticate_user(roll_number, password, &user_head);

                        if (login_result == 2) {
                            
                            printf("\033[1;32m\nWelcome, %s!\033[0m\n", roll_number);
                            press_any_key_to_continue();
                            handle_student_menu(roll_number);  
                            return;  
                        } else {
                            printf("\033[1;31mInvalid student credentials. You have %d attempt(s) remaining.\033[0m\n", max_attempts - attempts - 1);
                        }

                        attempts++;
                    }

                    printf("\033[1;31mToo many invalid attempts. Exiting...\033[0m\n");
                    exit(0);  // Exit the program after 3 invalid attempts
                }
                break;

            case 0: // Exit
                printf("\033[1;31mExiting the program...\033[0m\n");
                exit(0);  // Exit the program
                break;

            default:
              
                printf("\033[1;31mInvalid choice. Please try again.\033[0m\n");
        }
    }
}

int main() {
    // Load user data and exam data from CSV files
    load_user_data(&user_head);
    load_exam_data(&exam_head);  // Ensure this loads the exam data

    // Display login menu
    login_menu();  // Call the login menu function, which handles login and redirects to appropriate menu

    return 0;
}