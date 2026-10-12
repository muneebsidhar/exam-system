#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <stdexcept> 
#include <limits>    

using namespace std;

int getSafeInt() {
    int value;
    while (!(cin >> value)) {
        cin.clear(); 
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
        cout << "Invalid input! Please enter a number: ";
    }
    return value;
}

vector<string> makeOptions(string op1, string op2, string op3, string op4) {
    vector<string> opts;
    opts.push_back(op1);
    opts.push_back(op2);
    opts.push_back(op3);
    opts.push_back(op4);
    return opts;
}

class Question {
private:
    string text;
    vector<string> options;
    int correctoption;
public:
    Question(string t, vector<string> opts, int correct) {
        text = t;
        options = opts;
        correctoption = correct;
    }
    void display() const {
        cout << "\n Question : " << text << endl;
        for (size_t i = 0; i < options.size(); i++) {
            cout << i + 1 << ". " << options[i] << endl;
        }
    }
    bool checkanswer(int choice) const {
        return choice == correctoption;
    }
    string getquestiontext() const {
        return text;
    }
};

class Exam {
private:
    string subjectname;
    vector<Question> questions;
public:
    Exam(string name) {
        subjectname = name;
    }
    string getsubjectname() const {
        return subjectname;
    }
    void addquestion(const Question& q) {
        questions.push_back(q);
    }
    void removequestion(int index) {
        if (index < 0 || (size_t)index >= questions.size()) {
            throw out_of_range("Invalid index! No question exists at this position.");
        }
        questions.erase(questions.begin() + index);
        cout << "\n Question removed successfully \n";
    }
    void displayquestionsforteacher() const {
        if (questions.empty()) {
            cout << "No questions yet \n";
            return;
        }
        for (size_t i = 0; i < questions.size(); i++) {
            cout << "[ " << i + 1 << "]  ";
            questions[i].display();
        }
    }
    vector<Question>& getquestion() {
        return questions;
    }
};

class Result {
public:
    string studentname;
    string subject;
    int score;
    int remaininghealth;
    bool passed;
    Result(string name, string sub, int s, int h, bool p) : studentname(name), subject(sub), score(s), remaininghealth(h), passed(p) {}
};

vector<Exam> examdatabase;
vector<Result> resultdatabase;

class User {
protected:
    string username;
    string password;
public:
    User(string u, string p) : username(u), password(p) {}
    string getusername() const { return username; }
    string getpassword() const { return password; }
    virtual void displaymenu() = 0;
};

class Student : public User {
private:
    int score;
public:
    Student(string u, string p) : User(u, p), score(0) {}
    void takeexam(Exam& exam) {
        cout << "\n========== EXAM START ==========\n";
        cout << "Subject : " << exam.getsubjectname() << endl;

        vector<Question>& question = exam.getquestion();

        if (question.empty()) {
            cout << "No questions available.\n";
            return;
        }

        for (size_t i = 0; i < question.size(); i++) {
            cout << "\n---------------------------------\n";
            cout << "Question " << i + 1 << endl;
            question[i].display();

            cout << "Enter Answer (1-4) : ";
            int answer = getSafeInt();

            try {
                ofstream fout("attempts.txt", ios::app);
                if (!fout.is_open()) {
                    throw runtime_error("Could not open attempts.txt to save answer.");
                }

                fout << username << endl;
                fout << exam.getsubjectname() << endl;
                fout << question[i].getquestiontext() << endl;
                fout << answer << endl;
                fout << "-----" << endl;

                fout.close();
            }
            catch (const exception& e) {
                cout << "Error: " << e.what() << endl;
            }
        }
        cout << "\nExam submitted successfully.\n";
        cout << "Teacher will evaluate your attempt later.\n";
    }
    
    void viewmyresult() {
        cout << "\n==MY Result==\n";
        bool found = false;
        for (size_t i = 0; i < resultdatabase.size(); i++) {
            if (resultdatabase[i].studentname == username) {
                cout << "Subject : " << resultdatabase[i].subject 
                     << " | Score : " << resultdatabase[i].score 
                     << " | Status : " << (resultdatabase[i].passed ? "pass" : "fail") << endl;
                found = true;
            }
        }
        if (!found) {
            cout << "No Exam record found\n";
        }
    }
    
    void displaymenu() { 
        int choice;
        do {
            cout << "\n===Student Portal==\n1.Take Exam\n2.View my result\n3.Logout \nSelect : ";
            choice = getSafeInt();
            
            if (choice == 1) {
                for (size_t i = 0; i < examdatabase.size(); i++) {
                    cout << i + 1 << "." << examdatabase[i].getsubjectname() << endl;
                }
                int subchoice = getSafeInt();
                
                try {
                    if (subchoice <= 0 || (size_t)subchoice > examdatabase.size()) {
                        throw out_of_range("Invalid subject selection.");
                    }
                    takeexam(examdatabase.at(subchoice - 1));
                }
                catch (const exception& e) {
                    cout << "Error: " << e.what() << endl;
                }
            }
            else if (choice == 2) {
                viewmyresult();
            }
        } while (choice != 3);
    }
};

void savequestiontofile(string subject, string qtext, string op1, string op2, string op3, string op4, int correct) {
    try {
        ofstream fout("addedquestions.txt", ios::app);
        if (!fout.is_open()) {
            throw runtime_error("Failed to open addedquestions.txt");
        }

        fout << subject << endl;
        fout << qtext << endl;
        fout << op1 << endl;
        fout << op2 << endl;
        fout << op3 << endl;
        fout << op4 << endl;
        fout << correct << endl;
        fout << "-----" << endl;

        fout.close();
    }
    catch (const exception& e) {
        cout << "File Error: " << e.what() << endl;
    }
}

class Teacher : public User {
public:
    Teacher(string u, string p) : User(u, p) {}
    
    void managequestions() {
        for (size_t i = 0; i < examdatabase.size(); i++) {
            cout << i + 1 << ". " << examdatabase[i].getsubjectname() << endl;
        }
        cout << "\nSelect Subject : (1-" << examdatabase.size() << ") : ";
        int subchoice = getSafeInt();

        if (subchoice <= 0 || (size_t)subchoice > examdatabase.size()) {
            cout << "Invalid subject selection.\n";
            return;
        }

        Exam& selectedexam = examdatabase[subchoice - 1];
        
        cout << "1.Add Question\n2.Remove Question\nSelect : ";
        int action = getSafeInt();
        
        if (action == 1) {
            string qtext, op1, op2, op3, op4;
            int correct;
            cout << "Question Text : ";
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); getline(cin, qtext);
            cout << "option 1 : "; getline(cin, op1);
            cout << "option 2 : "; getline(cin, op2);
            cout << "option 3 : "; getline(cin, op3);
            cout << "option 4 : "; getline(cin, op4);
            cout << "correct option(1-4) : ";
            correct = getSafeInt();
            selectedexam.addquestion(Question(qtext, makeOptions(op1, op2, op3, op4), correct));
            savequestiontofile(selectedexam.getsubjectname(), qtext, op1, op2, op3, op4, correct);
            cout << "Question added Successfully!\n";
        }
        else if (action == 2) {
            cout << "Question number to remove: \n";
            int qnum = getSafeInt();
            try {
                selectedexam.removequestion(qnum - 1);
            }
            catch (const exception& e) {
                cout << "Error: " << e.what() << endl;
            }
        }
    }
    
    void mannualgrading() {
        try {
            ifstream fin("attempts.txt");
            if (!fin) {
                throw runtime_error("No attempts found (could not open attempts.txt).");
            }
            
            ofstream fout("temp.txt");
            if (!fout) {
                throw runtime_error("Failed to create temporary file for grading.");
            }

            string student, subject, question, separator;
            int answer;

            while (getline(fin, student) && getline(fin, subject) && getline(fin, question)) {
                if (!(fin >> answer)) break;
                fin.ignore(numeric_limits<streamsize>::max(), '\n');
                if (!getline(fin, separator)) break;

                cout << "\n========================\n";
                cout << "Student : " << student << endl;
                cout << "Subject : " << subject << endl;
                cout << "Question : " << question << endl;
                cout << "Student Answer : " << answer << endl;

                cout << "Give Marks : ";
                int marks = getSafeInt();

                bool passed = (marks > 0);
                resultdatabase.push_back(Result(student, subject, marks, 0, passed));
            }

            fin.close();
            fout.close();

            remove("attempts.txt");
            rename("temp.txt", "attempts.txt");
            cout << "Grading completed.\n";
        }
        catch (const exception& e) {
            cout << "Grading Error: " << e.what() << endl;
        }
    }
    
    void displaymenu() { 
        int choice;
        do {
            cout << "\n===Teacher Portal==\n1.Manage Questions\n2.Mannual Grading\n3.Logout\nSelect :\n";
            choice = getSafeInt();
            
            if (choice == 1) {
                managequestions();
            }
            else if (choice == 2) {
                mannualgrading();
            }
        } while (choice != 3);
    }
};

void loadquestionsfromfile() {
    ifstream fin("addedquestions.txt");
    if (!fin) {
        return; 
    }

    string subject, qtext, op1, op2, op3, op4, separator;
    int correct;

    while (getline(fin, subject)) {
        getline(fin, qtext);
        getline(fin, op1);
        getline(fin, op2);
        getline(fin, op3);
        getline(fin, op4);
        
        fin >> correct;
        fin.ignore(numeric_limits<streamsize>::max(), '\n');
        getline(fin, separator);

        for (size_t i = 0; i < examdatabase.size(); i++) {
            if (examdatabase[i].getsubjectname() == subject) {
                examdatabase[i].addquestion(Question(qtext, makeOptions(op1, op2, op3, op4), correct));
            }
        }
    }
    fin.close();
}

void populateinitialdata() {
    examdatabase.push_back(Exam("Object Oriented Programing"));
    examdatabase.push_back(Exam("Digital Logic Design"));
    examdatabase.push_back(Exam("Statistics and Probability"));
    examdatabase.push_back(Exam("Technical and Bussiness Writing"));
    examdatabase.push_back(Exam("Applied Physics"));
    examdatabase.push_back(Exam("Pakistan Studeies"));

    examdatabase[0].addquestion(Question("If a new AI 'NeuralNet' class derives features from a base 'Model' class, what OOP concept is this?", makeOptions("Encapsulation", "Polymorphism", "Inheritance", "Abstraction"), 3));
    examdatabase[1].addquestion(Question("Which 7400 series IC is commonly known as a Quad 2-Input AND gate?", makeOptions("7404", "7408", "7432", "7400"), 2));
    examdatabase[2].addquestion(Question("As a sample size grows, its mean gets closer to the average of the whole population. This rule is called the...", makeOptions("Central Limit Theorem", "Law of Large Numbers", "Standard Deviation", "P-Value"), 2));
    examdatabase[3].addquestion(Question("Which communication skill is most important in presentations?", makeOptions("Sleeping", "Clear speaking", "Ignoring audience", "Reading silently"), 2));
    examdatabase[4].addquestion(Question("The SI unit of force", makeOptions("joule", "watt", "newton", "pascal"), 3));
    examdatabase[5].addquestion(Question("Who is known as the founder of Pakistan?", makeOptions("Quaid e Azam", "Allama Iqbal", "Sir syed Ahmed Khan", "Liaqat Ali Khan"), 1));
}

int main() {
    populateinitialdata();
    loadquestionsfromfile();

    Student s1("kashif", "123");
    Teacher t1("faheem", "admin");

    int mainchoice;
    cout << "=========================================\n";
    cout << "              EXAM SYSTEM                \n";
    cout << "=========================================\n";

    do {
        cout << "\n1.Student login\n2.Teacher Login\n3.Exit\nSelect : \n";
        mainchoice = getSafeInt();
        
        if (mainchoice == 1 || mainchoice == 2) {
            string u, p;
            cout << "User : ";
            cin >> u;
            cout << "Password : ";
            cin >> p;
            
            if (mainchoice == 1) {
                if (u == s1.getusername() && p == s1.getpassword()) {
                    s1.displaymenu();
                } else {
                    cout << "Invalid credentials \n";
                }
            } else {
                if (u == t1.getusername() && p == t1.getpassword()) {
                    t1.displaymenu();
                } else {
                    cout << "Invalid credentials \n";
                }
            }
        }
    } while (mainchoice != 3);
    
    return 0;
}
