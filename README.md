# Online Exam System in C++

This is a console-based exam system I built in C++ for my OOP course. It has two roles, teacher and student. Students log in, pick a subject and answer MCQs. Teachers manage the questions, grade the attempts by hand and the students can then see their results.

I made it to practice inheritance, encapsulation and abstraction, and also to learn file handling so the data does not disappear when the program closes.

## What it can do

Students can:
- log in and choose a subject
- answer multiple choice questions one by one (four options each)
- view their result after the teacher has graded the attempt

Teachers can:
- add new questions to any subject
- remove any question by its number, so students won't see it in future exams
- go through the submitted attempts and give marks manually
- see results stored in the system

The available subjects are OOP, DLD, Statistics, Technical Writing, Physics and Pakistan Studies.

## Files used for saving data

- addedquestions.txt stores the questions added by the teacher, and they are loaded again the next time the program runs.
- attempts.txt stores the student submissions until the teacher grades them. After grading, this file is cleared so the same attempt is not graded twice.

## How the code is organized

- User is an abstract base class. It holds the username and password and has a pure virtual function displaymenu().
- Student and Teacher both inherit from User, and each one has its own menu.
- Question stores the question text, the four options and the correct option.
- Exam holds a subject name and a list of questions, and supports adding and removing them.
- Result stores the student name, subject, score and pass or fail status.
- Two global vectors, examdatabase and resultdatabase, are shared between the classes.

Inheritance is used for Student and Teacher, abstraction through the pure virtual menu function, and encapsulation by keeping question data private inside its class.

## Built with

C++ with the standard libraries (iostream, fstream, vector, string). I wrote it in Visual Studio Code and compiled it with g++ (MinGW).

## How to run it

Clone the repo and compile the file with g++:

```
git clone https://github.com/<your-username>/<your-repo-name>.git
cd <your-repo-name>
g++ main.cpp -o ExamSystem
ExamSystem.exe
```

Change main.cpp to whatever your file is called. On Linux or Mac, run ./ExamSystem instead.

## Demo logins

These are only for testing the project.

- Student: username kashif, password 123
- Teacher: username Faheem, password admin

## How it works

When the program starts, it loads the default questions for the six subjects and any saved questions from addedquestions.txt. The main menu has Student Login, Teacher Login and Exit.

A student logs in, selects a subject, answers all the questions and submits. The answers go into attempts.txt. A teacher logs in and either manages questions or grades the attempts. In grading, each submission is shown one by one with the student name, subject, question and answer, and the teacher enters the marks. The result is then saved so the student can view it.

## Things I would like to add later

- Automatic grading, since the correct option is already stored with each question
- Passwords stored in a safer way instead of plain text
- A proper way to register new students and teachers
- Timer for each exam

## Author

Muneeb Ur Rehman
Computer Science , UET Taxila
