#ifndef USER_H
#define USER_H

#define MAX_LINE_LENGTH 250
#define MAX_RESULTS 200  // Max number of results to handle

// Predefined admin credentials
#define ADMIN_USERNAME "admin"
#define ADMIN_PASSWORD "admin123"

// Define the User structure
typedef struct {
    char roll_number[15];  // Roll number (used as username)
    char name[50];         // Student's name
    char course[50];       // Course name (e.g., B.Tech, M.Tech)
    char password[50];     // Student's password
} User;

// Define the UserNode structure for the linked list
typedef struct UserNode {
    User user;
    struct UserNode* next;  // Pointer to the next node
} UserNode;

// Declare the global linked list head pointer
extern UserNode* user_head;

// Function declarations
void register_user();                           // Function to register a new user
int authenticate_user(char* username, char* password, UserNode** user_head);
void view_users();                              // Function to view all users
void delete_user();        // Function to delete a user by username
void re_save_users_to_csv();                   // Function to re-save all users to CSV after deletion
void view_results(const char* roll_number);
void view_exam_results(const char* exam_code); //for the admin
void view_results_of_exam();
void view_results_of_a_student(); 
void update_results();
void add_results();
void delete_result();

#endif  // USER_H
