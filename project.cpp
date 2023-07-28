#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <cstdlib>



namespace TrainingSystem {


    std::string getEnvironmentVariable(const std::string& envVarName) {
        char* envVarValue = std::getenv(envVarName.c_str());
        return (envVarValue != nullptr) ? std::string(envVarValue) : "";
    }

    // Constants for filenames and passwords (updated to use environment variables)
    const std::string trainingSystemFolder = getEnvironmentVariable("FILE_PATH");
    const std::string employeesFilename = trainingSystemFolder + "/employees.txt";
    const std::string trainingsFilename = trainingSystemFolder + "/trainings.txt";
    const std::string requestsFilename = trainingSystemFolder + "/course_requests.txt";
    const std::string password = getEnvironmentVariable("PASSWORD");



    

    


    

    // Forward declarations
    class User;
    class Employee;
    class Manager;
    class Training;

    // Class to manage user operations
    class UserManager {
    public:
        // Function to perform login and return the user object
        static User* login();
    };

    // Class to represent a user
    class User {
    public:
        virtual std::string getUserRole() const = 0;
    };

    // Class to represent an employee
    class Employee : public User {
    private:
        std::string username;
        std::string role;
        std::vector<std::string> codingLanguages;

    public:
        Employee(const std::string& username, const std::string& role, const std::vector<std::string>& codingLanguages)
            : username(username), role(role), codingLanguages(codingLanguages) {}

        // Function to get the user role (override)
        std::string getUserRole() const override {
            return "employee";
        }

        // Getter for the username
        std::string getUsername() const {
            return username;
        }

        // Function to display available trainings for the employee
        void displayTrainingsForEmployee(const std::vector<Training>& trainings) const;
    };

    // Class to represent a manager
    class Manager : public User {
    private:
        std::string username;
        std::vector<std::string> codingLanguages;

    public:
        Manager(const std::string& username, const std::vector<std::string>& codingLanguages)
            : username(username), codingLanguages(codingLanguages) {}

        // Function to get the user role (override)
        std::string getUserRole() const override {
            return "manager";
        }

        // Getter for the username
        std::string getUsername() const {
            return username;
        }

        // Function to handle course requests and approve/deny them
        void handleCourseRequests();
    };

    // Class to represent a training
    class Training {
    private:
        std::string language;
        std::string date;

    public:
        Training(const std::string& language, const std::string& date)
            : language(language), date(date) {}

        std::string getLanguage() const {
            return language;
        }

        std::string getDate() const {
            return date;
        }
    };

    // Class to manage employee data
    class EmployeeManager {
    private:
        static std::vector<Employee> employees;

    public:
        // Function to read employees' data from a file
        static void readEmployeesData();

        // Function to find an employee by username
        static Employee* findEmployeeByUsername(const std::string& username);

        // Function to get all employees
        static const std::vector<Employee>& getAllEmployees() {
            return employees;
        }
    };

    // Class to manage training data
    class TrainingManager {
    private:
        static std::vector<Training> trainings;

    public:
        // Function to read available trainings data from a file
        static void readTrainingsData();

        // Function to find a training by language
        static Training* findTrainingByLanguage(const std::string& language);

        // Function to display available trainings
        static void displayTrainings();

        // Function to get the trainings vector
        static const std::vector<Training>& getTrainings() {
            return trainings;
        }
    };

    // Class to manage course requests
    class CourseRequestManager {
    private:
        static std::vector<std::string> courseRequests;

    public:
        // Function to read course requests data from a file
        static void readCourseRequests();

        // Function to get all course requests
        static const std::vector<std::string>& getAllCourseRequests() {
            return courseRequests;
        }

        // Function to save course requests from employees to a file
        static void saveCourseRequest(const std::string& employeeName, const std::string& courseName, const std::string& date, const std::string& status);
    };

    // Initialize the static member variables
    std::vector<Employee> EmployeeManager::employees;
    std::vector<Training> TrainingManager::trainings;
    std::vector<std::string> CourseRequestManager::courseRequests;
}

namespace TrainingSystem {
    // Function to perform login and return the user object
    User* UserManager::login() {
        std::string username, userPassword;
    std::cout << "Enter your username: ";
    std::cin >> username;

    User* user = nullptr;
    std::cout << "Enter your password: ";
    std::cin >> userPassword;
    if (userPassword != password) {
        std::cout << "Invalid username/password. Authentication failed." << std::endl;
        return nullptr;
    }

    if (username.find("emp@") == 0) {
        user = EmployeeManager::findEmployeeByUsername(username);
    } else if (username.find("mgr@") == 0) {
        std::vector<std::string> codingLanguages; // Add coding languages for the manager
        user = new Manager(username, codingLanguages);
    } else {
        std::cout << "Invalid username/password. Please enter a valid username." << std::endl;
        return nullptr;
    }

    if (user == nullptr) {
        std::cout << "Invalid username/password. Please enter a valid username." << std::endl;
        return nullptr;
    }

    return user;
    }

    // Function to read employees' data from a file
    void EmployeeManager::readEmployeesData() {
        std::ifstream file(employeesFilename);
        if (file.is_open()) {
            std::string line;
            while (std::getline(file, line)) {
                std::istringstream iss(line);
                std::string username, role, lang;
                std::vector<std::string> codingLanguages;
                std::getline(iss, username, ',');
                std::getline(iss, role, ',');
                while (std::getline(iss, lang, ',')) {
                    codingLanguages.push_back(lang);
                }
                employees.emplace_back(username, role, codingLanguages);
            }
            file.close();
        } else {
            std::cout << "Error: Unable to read employees' data from the file." << std::endl;
        }
    }

    // Function to find an employee by username
    Employee* EmployeeManager::findEmployeeByUsername(const std::string& username) {
        for (auto& emp : employees) {
            if (emp.getUsername() == username) {
                return &emp;
            }
        }
        return nullptr;
    }

    // Function to read available trainings data from a file
    void TrainingManager::readTrainingsData() {
        std::ifstream file(trainingsFilename);
        if (file.is_open()) {
            std::string line;
            while (std::getline(file, line)) {
                std::istringstream iss(line);
                std::string lang, date;
                std::getline(iss, lang, ',');
                std::getline(iss, date, ',');
                trainings.emplace_back(lang, date);
            }
            file.close();
        } else {
            std::cout << "Error: Unable to read trainings' data from the file." << std::endl;
        }
    }

    // Function to find a training by language
    Training* TrainingManager::findTrainingByLanguage(const std::string& language) {
        for (auto& training : trainings) {
            if (training.getLanguage() == language) {
                return &training;
            }
        }
        return nullptr;
    }

    // Function to display available trainings
    void TrainingManager::displayTrainings() {
        std::cout << "Available Trainings:" << std::endl;
        for (const auto& training : trainings) {
            std::cout << training.getLanguage() << " - " << training.getDate() << std::endl;
        }
    }

    // Function to read course requests data from a file
    void CourseRequestManager::readCourseRequests() {
        std::ifstream file(requestsFilename);
        if (file.is_open()) {
            std::string line;
            while (std::getline(file, line)) {
                courseRequests.push_back(line);
            }
            file.close();
        } else {
            std::cout << "Error: Unable to read course requests from the file." << std::endl;
        }
    }

    // Function to save course requests from employees to a file
    void CourseRequestManager::saveCourseRequest(const std::string& employeeName, const std::string& courseName, const std::string& date, const std::string& status) {
        std::ofstream file(requestsFilename, std::ios_base::app);
        if (file.is_open()) {
            file << employeeName << "," << courseName << "," << date << "," << status << "\n";
            file.close();
        } else {
            std::cout << "Error: Unable to save course request." << std::endl;
        }
    }

    // Function to display available trainings for the employee
    void Employee::displayTrainingsForEmployee(const std::vector<Training>& trainings) const {
        std::cout << "Available Trainings for " << username << ":" << std::endl;
        for (const auto& training : trainings) {
            if (std::find(codingLanguages.begin(), codingLanguages.end(), training.getLanguage()) != codingLanguages.end()) {
                std::cout << training.getLanguage() << " - " << training.getDate() << std::endl;
            }
        }
    }

    // Function to handle course requests and approve/deny them
   // Function to handle course requests and approve/deny them
void Manager::handleCourseRequests() {
    // Display all course requests
   std::cout << "Course Requests:" << std::endl;
    std::vector<std::string> updatedCourseRequests;
    for (const auto& request : CourseRequestManager::getAllCourseRequests()) {
        std::istringstream iss(request);
        std::string employeeName, courseName, date, status;
        std::getline(iss, employeeName, ',');
        std::getline(iss, courseName, ',');
        std::getline(iss, date, ',');
        std::getline(iss, status, ',');

        std::cout << "Employee: " << employeeName << ", Course: " << courseName << ", Date: " << date << ", Status: " << status << std::endl;

        // Ask for approval/denial and reason if needed
        if (status == "pending") {
            std::cout << "Approve (y/n): ";
            char approvalChoice;
            std::cin >> approvalChoice;

            if (std::tolower(approvalChoice) == 'y') {
                // Update the approval status in memory
                status = "approved";
            } else if (std::tolower(approvalChoice) == 'n'){
                // Save denial status and reason in memory
                std::cout << "Enter the reason for denial: ";
                std::string denialReason;
                std::cin.ignore();
                std::getline(std::cin, denialReason);
                status = "rejected: " + denialReason;
            }
        }

        // Add the updated status to the updated course requests vector
        updatedCourseRequests.push_back(employeeName + "," + courseName + "," + date + "," + status);
    }

    // Overwrite the entire file with the updated data
    std::ofstream file(requestsFilename);
    if (file.is_open()) {
        for (const auto& request : updatedCourseRequests) {
            file << request << "\n";
        }
        file.close();
        std::cout << "Course requests updated successfully." << std::endl;
    } else {
        std::cout << "Error: Unable to update course requests." << std::endl;
    }
}

}

int main() {



      try {
        // Read employees' data and trainings' data from the files
        TrainingSystem::EmployeeManager::readEmployeesData();
        TrainingSystem::TrainingManager::readTrainingsData();
        TrainingSystem::CourseRequestManager::readCourseRequests();

        // Ask for employee's username to login
        TrainingSystem::User* user = TrainingSystem::UserManager::login();
        if (user == nullptr) {
            return 1;
        }

        if (user->getUserRole() == "employee") {
            // Display available trainings for the employee
            TrainingSystem::Employee* employee = dynamic_cast<TrainingSystem::Employee*>(user);
            employee->displayTrainingsForEmployee(TrainingSystem::TrainingManager::getTrainings());

            // Ask for the training they want to apply for
            std::string trainingChoice;
            std::cout << "Enter the training you want to apply for: ";
            std::cin >> trainingChoice;

            // Check if the training choice is valid and apply for it
            TrainingSystem::Training* training = TrainingSystem::TrainingManager::findTrainingByLanguage(trainingChoice);
            if (training != nullptr) {
                TrainingSystem::CourseRequestManager::saveCourseRequest(employee->getUsername(), training->getLanguage(), training->getDate(), "pending");
                std::cout << "Application successful. You have applied for the training in " << training->getLanguage()
                          << " on " << training->getDate() << "." << std::endl;
            } else {
                std::cout << "Invalid training choice. Please choose from the available trainings." << std::endl;
            }
        } else if (user->getUserRole() == "manager") {
            TrainingSystem::Manager* manager = dynamic_cast<TrainingSystem::Manager*>(user);
            manager->handleCourseRequests();
        }
    } catch (const std::exception& e) {
        std::cerr << "An error occurred: " << e.what() << std::endl;
        return 1;
    }

    return 0;
    
}