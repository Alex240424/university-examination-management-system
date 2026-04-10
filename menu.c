#include <stdio.h>
#include <stdlib.h>
#include "menu.h"
#include "user.h"
#include "exam.h"
#include "file_io.h"
#include "main.h"

// Platform-independent function to clear the screen
void clear_screen() {
    #ifdef _WIN32
        system("cls");  // Windows
    #else
        system("clear");  // Linux/macOS
    #endif
}

// Function to handle Admin menu operations
void handle_admin_menu() {
    int choice;
    do {
        clear_screen();  // Clear screen
        printf("\033[1;34m=============================================\n");
        printf("   \033[1;32mAdmin Menu\033[0m\n");
        printf("=============================================\n");
        printf("\033[1;35m1. \033[1;36mRegister a Student\033[0m\n");
        printf("\033[1;35m2. \033[1;36mView Students\033[0m\n");
        printf("\033[1;35m3. \033[1;36mDelete a Student\033[0m\n");
        printf("\033[1;35m4. \033[1;36mAdd Exam\033[0m\n");
        printf("\033[1;35m5. \033[1;36mView Exams\033[0m\n");
        printf("\033[1;35m6. \033[1;36mDelete Exam\033[0m\n");
        printf("\033[1;35m7. \033[1;36mView Results of an Exam\033[0m\n");
        printf("\033[1;35m8. \033[1;36mView Results of a Student\033[0m\n");
        printf("\033[1;35m9. \033[1;36mAdd Results\033[0m\n");
        printf("\033[1;35m10. \033[1;36mUpdate Results\033[0m\n");
        printf("\033[1;35m11. \033[1;36mDelete a Result\033[0m\n");
        printf("\033[1;31m0. \033[1;31mExit\033[0m\n");

        choice = get_menu_choice();

        switch (choice) {
            case 1:
                register_user();
                break;
            case 2:
                view_users();
                break;
            case 3: {
                delete_user();
                break;
            }
            case 4:
                add_exam();
                break;
            case 5:
                view_exams();
                break;
            case 6: {
                delete_exam();
                break;
            }
            case 7: {
                view_results_of_exam();
                break;
            }
            case 8: {
                view_results_of_a_student();
                break;
            }
            case 9: {
                add_results();
                break;
            }
            case 10: {
                update_results();
                break;
            }
            case 11: {
                delete_result();
                break;
            }
            case 0:
                login_menu();
                return;
            default:
                printf("\033[1;31mInvalid choice. Please try again.\033[0m\n");
                break;
        }

        press_any_key_to_continue();
    } while (choice != 0);
}

// Function to handle Student menu operations
void handle_student_menu(const char* roll_number) {
    int choice;
    do {
        clear_screen();  // Clear screen
        printf("\033[1;34m=============================================\n");
        printf("        \033[1;32mStudent Menu\033[0m\n");
        printf("=============================================\n");
        printf("\033[1;35m1. \033[1;36mView Exams\033[0m\n");
        printf("\033[1;35m2. \033[1;36mSearch Exams by Year\033[0m\n");
        printf("\033[1;35m3. \033[1;36mSearch Exams by Subject\033[0m\n");
        printf("\033[1;35m4. \033[1;36mView Results\033[0m\n");
        printf("\033[1;31m0. \033[1;31mExit\033[0m\n");

        choice = get_menu_choice();

        switch (choice) {
            case 1:
                view_exams();
                break;
            case 2:
                search_exam_by_year();
                break;
            case 3:
                search_exam_by_subject();
                break;
            case 4:
                view_results(roll_number);  // Passing the roll number of the logged-in student
                break;
            case 0:
                login_menu();
                return;
            default:
                printf("\033[1;31mInvalid choice. Please try again.\033[0m\n");
                break;
        }

        press_any_key_to_continue();
    } while (choice != 0);
}


void press_any_key_to_continue() {
    printf("\033[1;33mPress Enter key to continue...\033[0m");
    getchar();  
    getchar();  
}


int get_menu_choice() {
    int choice;
    printf("\033[1;33mEnter your choice: \033[0m");
    scanf("%d", &choice);
    return choice;
}
