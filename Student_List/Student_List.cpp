#include <iostream>
#include <fstream>
#include <string>
using namespace std;
#pragma warning(disable : 4996)

struct Profile {
    string fullName;
    char sex;
    int group;
    int id;
    int grades[8];
    char depart[9];
    string date;
};

int count_students() {
    ifstream database("students.txt");
    if (database.is_open()) {
        int temp = 0;
        string data;
        while (!database.eof()) {
            getline(database, data);
            temp++;
        }
        database.close();
        return temp / 7;
    }
    return 0;
}
void load_students(ifstream& database, Profile* student, int& size) {
    const int NUMBER_OF_GRADES = 8;
    for (int i = 0; i < size; i++) {
        database >> student[i].fullName
            >> student[i].group
            >> student[i].id
            >> student[i].sex
            >> student[i].depart;
        for (int j = 0; j < NUMBER_OF_GRADES; j++)
            database >> student[i].grades[j];
        getline(database, student[i].date, '\n');
    }
}

void input_student_data(Profile& student) {
    cout << "Enter student`s full name\n";
    cin >> student.fullName;
    cout << "Enter student`s group\n";
    cin >> student.group;
    cout << "Enter student`s id\n";
    cin >> student.id;
    cout << "Enter student`s sex (M/F)\n";
    cin >> student.sex;
    cout << "Enter student`s department\n";
    cin >> student.depart;

    for (int i = 0; i < 8; i++) {
        cout << "Enter student`s grade number " << i + 1 << '\n';
        cin >> student.grades[i];
    }
}

void set_registration_date(Profile& student) {
    time_t tstamp;
    tm* timeinfo;
    time(&tstamp);
    timeinfo = localtime(&tstamp);
    student.date = asctime(timeinfo);
}

bool has_failing_grades(const Profile& student) {
    for (int i = 0; i < 8; i++) {
        if (student.grades[i] == 2) {
            return true;
        }
    }
    return false;
}

void save_students(ofstream& output_database, Profile* student, int& size) {
    for (int i = 0; i < size; i++) {
        output_database << student[i].fullName << '\n'
            << student[i].group << '\n'
            << student[i].id << '\n'
            << student[i].sex << '\n'
            << student[i].depart << '\n';
        for (int j = 0; j < 8; j++)
            output_database << student[i].grades[j] << " ";
        output_database << '\n' << student[i].date << '\n';
    }
}

void set_student_record(Profile& student) {
    int size = count_students();
    ofstream database("students.txt", ios::app);

    if (!database.is_open()) {
        cout << "Saving error!\n";
        return;
    }
    
    save_students(database, &student, size);

    database.close();
    cout << '\n' << "Profile is saved in the database.\n";
}

void create_record() {
    Profile student;

    input_student_data(student);
    set_registration_date(student);

    if (!has_failing_grades(student)) {
        set_student_record(student);
    }
    else {
        cout << '\n' << "This student will be expelled. The profile will not be saved in the database.\n";
    }
}

void display_student_info(const Profile& student) {
    cout << "1. student`s name: " << student.fullName << '\n';
    cout << "2. student`s group number: " << student.group << '\n';
    cout << "3. student`s id: " << student.id << '\n';
    cout << "4. student`s sex: " << student.sex << '\n';
    cout << "5. student`s department name: " << student.depart << '\n';
    cout << "6. term grades: \n";
    for (int j = 0; j < 8; j++)
        cout << student.grades[j] << " ";
    cout << "\n7. registration date: " << student.date << '\n';
}

void update_student_data(Profile& student, int& choice_number) {
    switch (choice_number) {
    case 1:
        cout << "Enter new name: ";
        cin.ignore();
        getline(cin, student.fullName);
        break;
    case 2:
        cout << "Enter new group number: ";
        cin >> student.group;
        break;
    case 3:
        cout << "Enter new id: ";
        cin >> student.id;
        break;
    case 4:
        cout << "Enter new sex (M/F): ";
        cin >> student.sex;
        break;
    case 5:
        cout << "Enter new department name: ";
        cin.ignore();
        cin.getline(student.depart, sizeof(student.depart));
        break;
    case 6:
        cout << "Enter new grades (8 total): ";
        for (int j = 0; j < 8; j++)
            cin >> student.grades[j];
        break;
    case 7:
        cout << "Enter new registration date: ";
        cin.ignore();
        getline(cin, student.date);
        break;
    default:
        cout << "Invalid choice!\n";
        break;
    }
}


void modify_record() {
    int id;
    int size = count_students();

    cout << "Enter student`s id to modify their record\n";
    cin >> id;

    ifstream database("students.txt");
    if (!database.is_open()) {
        cout << "Error!";
        return;
    }

    if (size == 0) {
        cout << "The database is empty." << endl;
        return;
    }

    Profile* students = new Profile[size];
    load_students(database, students, size);
    database.close();

    bool record_found = false;

    for (int i = 0; i < size; i++) {
        if (students[i].id == id) {
            display_student_info(students[i]);
            cout << "What would you like to change? (enter number): ";
            int choice_number;
            cin >> choice_number;
            update_student_data(students[i], choice_number);
            record_found = true;
            break;
        }
    }

    if (!record_found) {
        cout << "No records were found" << '\n';
    }
    else {
        ofstream output_database("students.txt");

        if (!output_database.is_open()) {
            cout << "Error saving the database!" << '\n';
        }
        else {
            save_students(output_database, students, size);
            output_database.close();
            cout << "Record updated successfully!" << '\n';
        }
    }

    delete[] students;
}

void show_data() {
    ifstream database("students.txt");

    if (!database.is_open())
        cout << "File opening error";
    else {
        int temp = count_students();

        if (temp == 0)
            cout << "The file is empty";
        else {
            string data;

            while (!database.eof()) {
                getline(database, data);
                cout << data << '\n';
            }
        }

        database.close();
    }
}

void show_group() {
    int n;

    cout << "Enter the group number:" << endl;
    cin >> n;

    ifstream database("students.txt");

    if (!database.is_open())
        cout << "Error!";
    else {
        int size = count_students();

        if (size == 0)
            cout << "The database is empty." << endl;
        else {
            Profile* student = new Profile[size];

            load_students(database, student, size);
            database.close();

            int temp = 0;

            for (int i = 0; i < size; i++) {
                if (student[i].group == n) {
                    cout << student[i].id << " " << student[i].fullName
                        << '\n' << student[i].sex << '\n';
                    cout << "Term grades: ";
                    for (int j = 0; j < 8; j++)
                        cout << student[i].grades[j] << " ";
                    cout << "\n\n";
                    temp++;
                }
            }
            if (temp == 0)
                cout << "No records were found" << endl;

            delete[] student;
        }
    }
}

int count_average_score(int* student_score) {
    const int NUMBER_OF_GRADES = 8;
    int average_score = 0;

    for (int i = 0; i < NUMBER_OF_GRADES; i++) {
        average_score += student_score[i];
    }

    average_score /= NUMBER_OF_GRADES;

    return average_score;
}

void show_student_information(Profile student) {
    const int NUMBER_OF_GRADES = 8;

    cout << "student`s name: " << student.fullName << '\n';
    cout << "student`s id: " << student.id << '\n';
    cout << "term grades: \n";
    for (int i = 0; i < NUMBER_OF_GRADES; i++)
        cout << student.grades[i] << " ";

    cout << "\naverage score: " << count_average_score(student.grades) << '\n';
}

void sort_students(Profile* students, int& size) {
    for (int i = 1; i < size; i++) {
        Profile key = students[i];
        int keyAverage = count_average_score(key.grades);
        int j = i - 1;

        while (j >= 0 && count_average_score(students[j].grades) < keyAverage) {
            students[j + 1] = students[j];
            j--;
        }
        students[j + 1] = key;
    }
}

void displayStudents(const Profile* students, int& size) {
    for (int i = 0; i < size; i++) {
        cout << i + 1 << ".\n";
        show_student_information(students[i]);
    }
}

void showStudentTop() {
    ifstream database("students.txt");
    if (!database.is_open()) {
        cout << "Error!";
        return;
    }

    int size = count_students();
    if (size == 0) {
        cout << "The database is empty." << endl;
        return;
    }

    Profile* students = new Profile[size];
    load_students(database, students, size);
    database.close();

    sort_students(students, size);
    displayStudents(students, size);

    delete[] students;
}

typedef enum {
    LAGGING_BEHIND_STUDENT
    , SUCCESSFUL_STUDENT
    , EXCELLENT_STUDENT
} student_type;

struct student_with_success_rate {
    Profile data;
    student_type type;
};

void load_students_with_success_rate(ifstream& database, student_with_success_rate* student, int& size) {
    const int NUMBER_OF_GRADES = 8;

    for (int i = 0; i < size; i++) {
        database >> student[i].data.fullName
            >> student[i].data.group
            >> student[i].data.id
            >> student[i].data.sex
            >> student[i].data.depart;
        for (int j = 0; j < NUMBER_OF_GRADES; j++)
            database >> student[i].data.grades[j];
        getline(database, student[i].data.date, '\n');

        int average_score = count_average_score(student[i].data.grades);
        if (average_score == 5)
            student[i].type = EXCELLENT_STUDENT;
        else if (average_score == 4)
            student[i].type = SUCCESSFUL_STUDENT;
        else
            student[i].type = LAGGING_BEHIND_STUDENT;
    }
}

void display_lagging_behind_students(student_with_success_rate* student, int& size) {
    cout << "Students that will not get bursary: \n";

    int count = 1;

    for (int i = 0; i < size; i++) {
        if (student[i].type == LAGGING_BEHIND_STUDENT) {
            cout << count << ".\n";
            show_student_information(student[i].data);
            count++;
        }
    }
}

void display_successful_and_excellent_students(student_with_success_rate* student, int& size) {
    cout << "\nSuccessful and excellent students: \n";
    int count = 1;
    for (int i = 0; i < size; i++) {
        if (student[i].type == SUCCESSFUL_STUDENT || student[i].type == EXCELLENT_STUDENT) {
            cout << count << ".\n";
            show_student_information(student[i].data);
            count++;
        }
    }
}

void display_excellent_students(student_with_success_rate* student, int& size) {
    cout << "\nOnly excellent students: \n";
    int count = 1;
    for (int i = 0; i < size; i++) {
        if (student[i].type == EXCELLENT_STUDENT) {
            cout << count << ".\n";
            show_student_information(student[i].data);
            count++;
        }
    }
}

void show_student_performance() {
    ifstream database("students.txt");

    if (!database.is_open()) {
        cout << "Error!";
        return;
    }

    int size = count_students();

    if (size == 0) {
        cout << "The database is empty." << endl;
        return;
    }

    student_with_success_rate* students = new student_with_success_rate[size];
    load_students_with_success_rate(database, students, size);
    database.close();

    display_lagging_behind_students(students, size);
    display_successful_and_excellent_students(students, size);
    display_excellent_students(students, size);

    delete[] students;
}

void show_male_and_female_student() {
    ifstream database("students.txt");

    if (!database.is_open())
        cout << "Error!";
    else {
        int size = count_students();

        if (size == 0)
            cout << "The database is empty." << endl;
        else {
            Profile* student = new Profile[size];
            const int NUMBER_OF_GRADES = 8;

            load_students(database, student, size);
            database.close();

            cout << "Male Students: \n";
            int count = 1;
            for (int i = 0; i < size; i++) {
                if (student[i].sex == 'M') {
                    cout << count << ".\n";
                    show_student_information(student[i]);
                    i++;
                }
            }

            cout << "Female Students: \n";
            count = 1;
            for (int i = 0; i < size; i++) {
                if (student[i].sex == 'F') {
                    cout << count << ".\n";
                    show_student_information(student[i]);
                    i++;
                }
            }

            delete[] student;
        }
    }
}

void show_student_with_number_k_if_file_is_open(ifstream& database, int& size, int& k) {
    Profile* student = new Profile[size];
    const int NUMBER_OF_GRADES = 8;

    for (int i = 0; i < size; i++) {
        database >> student[i].fullName 
            >> student[i].group
            >> student[i].id
            >> student[i].sex
            >> student[i].depart;
        for (int j = 0; j < NUMBER_OF_GRADES; j++)
            database >> student[i].grades[j];
        getline(database, student[i].date, '\n');
        if (i == k) {
            show_student_information(student[i]);
            break;
        }
    }
    database.close();

    delete[] student;
}

int get_k() {
    int k;
    cout << "Enter student number: \n";
    cin >> k;
    k--;
    return k;
}

void show_student_with_number_k() {
    int k = get_k();

    ifstream database("students.txt");

    if (!database.is_open())
        cout << "Error!";
    else {
        int size = count_students();
        if (size == 0)
            cout << "The database is empty." << endl;
        else
            show_student_with_number_k_if_file_is_open(database, size, k);
    }
}

void no_section() {
    int operation_choice;

    cout << "Select the operation you want to do:\n\
    1. Create a new student record.\n\
    2. Make changes to an existing record.\n\
    3. Display all student data.\n\
    4. Display information about all students in group N. N is initialized by the user.\n\
    5. Display the top most successful students with the highest average grade for the past session.\n\
    6. Display the number of male and female students.\n\
    7. Display data about the students:\n\
        - who do not receive a scholarship;\n\
        - study only with “good” and “excellent” grades;\n\
        - study only with “excellent” grades;\n\
    8. Display data about students with a number in the list – k.\n";
    cin >> operation_choice;
    switch (operation_choice) {
    case 1:
        create_record();
        break;
    case 2:
        modify_record();
        break;
    case 3:
        show_data();
        break;
    case 4:
        show_group();
        break;
    case 5:
        showStudentTop();
        break;
    case 6:
        show_male_and_female_student();
        break;
    case 7:
        show_student_performance();
        break;
    case 8:
        show_student_with_number_k();
        break;
    }
}

int main() {
    string exit_choice;
    
    while (true) {
        cout << "Do you want to exit the program?(Yes/No)\n";
        cin >> exit_choice;
        if (exit_choice == "Yes")
        {
            goto exit;
        }
        if (exit_choice == "No")
        {
            no_section();
        }
    }
exit:
    return 0;
}