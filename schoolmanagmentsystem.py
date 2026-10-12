import json
from abc import ABC , abstractmethod
from pathlib import Path

database="school_data.json"
data={"students" : [] , "teachers" : []}

if Path(database).exists():
    with open(database,"r") as f:
        contant=f.read()
        if contant:
            data=json.loads(contant)
 
def save():
    with open(database,"w") as f:
        json.dump(data,f,indent=4)
                
class person(ABC):
    @abstractmethod
    def get_roles(self):
        pass
    
    @abstractmethod
    def register(self):
        pass          
    
    @abstractmethod
    def show_details(self):
        pass
    @staticmethod
    def validate_email(email):
        if "@" in email and "." in email:
            return True
        else:
            False
            
            
    
class student(person):
    def get_roles(self):
        return "student"
    
    def register(self):
        name=input("Enter your name :- ")
        age=int(input("Enter your age :- "))
        email=input("Enter your email :- ")
        roll_no=input("Enter your roll number :- ")
        
        if not person.validate_email(email):
            print("invalid email")
            return
        for i in data['students']:
            if i['roll_no'] == roll_no:
                print("student already exists")
                return
        data['students'].append({
            "name" : name,
            "age" : age,
            "email" : email,
            "roll_no" : roll_no,
            "grade" : {}
        })
        save()
        print(f"student {name} registered successfully!")
        
    def show_details(self):
        roll_no=input("roll no :- ")
        for i in data['students']:
            if i['roll_no'] == roll_no:
                grades=i['grade']
                avg=sum(grades.values()) / len(grades) if grades else 0
                
                print(f"\nname : {i['name']}")
                print(f"Roll no : {i['roll_no']}")
                print(f"Grade : {grades}")
                print(f"Average : {avg:.1f}")
                return
    
    def add_grades(self):
        roll_no=input("tell your roll number")
        subject= input("subject : ")
        marks=float(input("marks : "))
        
        for i in data['students']:
            i["roll_no"]==roll_no
            i['grade'][subject]= marks
            save()
            print("grade added successfully!")
            return
        print("student not found!")

class teacher(person):
    def get_roles(self):
        return "Teacher"
    
    def register(self):
        name=input("Enter your name :- ")
        age=int(input("Enter your age :- "))         
        email=input("Enter your email :- ")
        subject=input("enter name of subject you teach")
        emp_id=input("Enter your employee id :- ")
        
        if not person.validate_email(email):
            print("invalid email")
            return
        
        for i in data['teachers']:
            if i['emp_id'] == emp_id:
                print("employee  already exists")
                return
        
        data['teachers'].append({
            "name" : name,
            "age" : age,
            "email" : email,
            "subject" : subject,
            "emp_id" : emp_id,
            }) 
        save()
        print(f"Teacher {name} registered successfully!") 
          
    def show_details(self):
        emp_id=input("emp id  :- ")
        
        for i in data['teachers']:
            if i['emp_id'] == emp_id:                     
                print(f"\nname : {i['name']}")
                print(f"emp id : {i['emp_id']}")
                print(f"subject : {i['subject']}")
                return 
        print("employee not found!")    
            
               
stud=student()
tech=teacher()

print("press 1 to register a student")
print("press 2 to register a teacher")
print("press 3 to add grades")
print("press 4 to show student details")
print("press s to shoe teacher detail")

choice=int(input("Enter your choice :- "))

if choice==1:
    stud.register()
    
elif choice==2:
    tech.register()

elif choice==3:
    stud.add_grades() 

elif choice==4:
    stud.show_details()
    
elif choice==5:
    tech.show_details()
    
else:
    print("please choose 1-5")                   