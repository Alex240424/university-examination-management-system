# University Exam Management System

The **University Exam Management System** is a command-line application built in C that helps manage students' exam results and user accounts within a university. It allows both administrators and students to interact with exam data and manage user information effectively.

## Features and Functionality

### Admin Features

1. **Manage User Accounts**:
   - Admin can **add** new students by providing roll number, name, course, and password.
   - Admin can **delete** students based on their roll number.
   - Admin can **view student results** for any roll number.
    > **Note:** The admin's username and password are hardcoded into the program. 
    > - **Username:** `admin`
    > - **Password:** `admin123`
2. **Manage Exam Results**:
   - Admin can **view** exam results of all students or search for results by roll number.
   - Admin can **update** marks for a specific student in a specific exam.
   - Admin can **delete** results for a specific student in a specific exam.
   - Admin can **view exam statistics**, such as class average and highest marks, for any exam.
   - Admin can **sort exam results** by marks (highest to lowest).

3. **Manage Exams**:
   - Admin can **add** new exams by specifying the exam code, course name, exam date, max marks, and the number of students.
   - Admin can **view** all exams and their details.

### Student Features

1. **Login**:
   - Students can log in using their **roll number** and **password**.
   - Upon successful login, students are granted access to the student menu.

2. **View Exam Results**:
   - Students can **view their exam results** for all exams they have appeared for.
   - Students can **view detailed exam results**, which includes their marks, total marks for the exam, class average, and highest marks.
   - Students can **search exams** by **subject** or **year** to find relevant exams they appeared for.

3. **Search Exams**:
   - Students can **search exams by subject** (e.g., Computer Science, Mathematics).
   - Students can **search exams by year** (e.g., exams taken in 2024).

### Common Features

1. **Data Persistence**:
   - All data is stored in **CSV files** (`users.csv`, `results.csv`, `exams.csv`), ensuring that changes are saved across program runs.
   
2. **Error Handling**:
   - The program handles invalid input gracefully, prompting users for correct data when necessary.
   - Admins are prompted to confirm actions like deleting users or exam results before proceeding.

## File Structure

The system uses the following CSV files for storing and managing data:

- **`users.csv`**: Stores user information.
    - Example format: `BT1234567, John Doe, Computer Science, password123`
  
- **`results.csv`**: Stores students' exam results.
    - Example format: `BT1234567, CS101, 85`
  
- **`exams.csv`**: Stores details about exams.
    - Example format: `AM103,CALCULUS,12-12-2024,35,160`

## Setup Instructions

### Prerequisites
1. Install a C compiler (e.g., GCC).
2. Ensure the program has access to a terminal or command prompt to execute.

### Steps

1. **Clone or Download the Project**:
   - Download the source code files and necessary CSV files to your local machine.

2. **Compile the Program**:
   - Navigate to the directory containing the source files (`main.c`, `user.c`, `menu.c`, `file_io.c`, `exam.c`, etc.).
   - Simply run the following command to compile the project:
     ```bash
     make
     ```

3. **Run the Program**:
   - After the compilation is complete, run the program using the following command:
     ```bash
     ./system
     ```

4. **CSV Files**:
   - If the `users.csv`, `results.csv`, or `exams.csv` files do not exist, the program will create them when needed. Ensure that these files are accessible and writable in the program's directory.

## Contributions

This project was developed by a team of six members. Below is a breakdown of the contributions from each team member:

## Contributing to `user.c`

The `user.c` file is responsible for managing user accounts, student results, and user-specific operations within the system. Below is a breakdown of the contributions from each team member:

### Member 1: Ayush Patel *BT2024171*
- Contributed to the `user.c` file:
  - Developed functionality for **adding** and **deleting user accounts**.
  - Implemented the logic for **viewing user details** (e.g., name, roll number, etc.).
  - Integrated **input validation** for adding new users (e.g., checking for duplicate roll numbers).
  - Implemented **user deletion** functionality, including user choice and confirmation prompts.

### Member 2: Muzhaib Manzoor *BT2024129*
- Contributed to the `user.c` file:
  - Developed features for **viewing, updating**, and **deleting student exam results**.
  - Implemented the logic to **search for and display student results** based on roll number.
  - Created functionality for **updating student marks**, including input validation for valid marks and exam codes.
  - Integrated **deletion of student results**, including user confirmation before removing results.

### Member 3: Naman Jindal *BT2024203*
- Contributed to the `user.c` file:
  - Developed features for **displaying exam results** for students.
  - Implemented functionality to allow students to **view results** for the exams they have taken.
  - Ensured proper **input validation** for student roll numbers and exam codes when retrieving results.
  - Managed **data display** for results in a user-friendly format, including handling cases where no results are found.
  - Implemented logic to allow students to **search exams by subject** and **by year**, enhancing search functionality.


### Member 4: Boyina Saketh *BT2024085*
- Developed the `main.c` and `menu.c` files:
  - Handled the main entry point for the application.
  - Implemented the admin and student menus.
  - Integrated various features like navigating between the menus and executing actions.

### Member 5: Jaswanth Gangavarapu *BT2024010*
- Developed the `file_io.c` file:
  - Implemented file handling for `users.csv`, `results.csv`, and `exams.csv`.
  - Ensured data integrity when saving and reading files.
  - Developed functions for saving and reading data to/from CSV files.

### Member 6: Utkarsh Rastogi *BT2024119*
- Developed the `exam.c` file:
  - Handled exam-related functionalities including viewing exam details.
  - Developed functions for calculating exam statistics (class average and highest marks).
  - Implemented exam result sorting and display features.

