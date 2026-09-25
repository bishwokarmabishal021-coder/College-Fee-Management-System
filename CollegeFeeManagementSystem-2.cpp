#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <iomanip>
#include <algorithm>
using namespace std;

/*
    COLLEGE FEE MANAGEMENT SYSTEM
    Console based C++ OOP project
    Data files: students.txt, fees.txt, payments.txt
*/

class Student {
public:
    int id, semester;
    string name, program, phone, email;

    Student() {}
    Student(int i, string n, string p, int s, string ph, string e) {
        id=i; name=n; program=p; semester=s; phone=ph; email=e;
    }

    void display() const {
        cout << "\nID       : " << id;
        cout << "\nName     : " << name;
        cout << "\nProgram  : " << program;
        cout << "\nSemester : " << semester;
        cout << "\nPhone    : " << phone;
        cout << "\nEmail    : " << email << "\n";
    }

    string save() const {
        return to_string(id)+"|"+name+"|"+program+"|"+
               to_string(semester)+"|"+phone+"|"+email;
    }

    bool load(string line) {
        vector<string> a;
        string x;
        for(char c: line) {
            if(c=='|') { a.push_back(x); x=""; }
            else x+=c;
        }
        a.push_back(x);
        if(a.size()!=6) return false;
        try {
            id=stoi(a[0]); semester=stoi(a[3]);
        } catch(...) { return false; }
        name=a[1]; program=a[2]; phone=a[4]; email=a[5];
        return true;
    }
};

class Fee {
public:
    int id, semester;
    string program;
    double tuition, library, lab, exam, other;

    Fee() {}
    Fee(int i,string p,int s,double t,double l,double lb,double e,double o) {
        id=i; program=p; semester=s;
        tuition=t; library=l; lab=lb; exam=e; other=o;
    }

    double total() const {
        return tuition+library+lab+exam+other;
    }

    void display() const {
        cout << "\nFee ID: " << id
             << "\nProgram: " << program
             << "\nSemester: " << semester
             << "\nTuition: Rs. " << tuition
             << "\nLibrary: Rs. " << library
             << "\nLab: Rs. " << lab
             << "\nExam: Rs. " << exam
             << "\nOther: Rs. " << other
             << "\nTotal: Rs. " << total() << "\n";
    }

    string save() const {
        return to_string(id)+"|"+program+"|"+to_string(semester)+"|"+
               to_string(tuition)+"|"+to_string(library)+"|"+
               to_string(lab)+"|"+to_string(exam)+"|"+to_string(other);
    }

    bool load(string line) {
        vector<string> a;
        string x;
        for(char c: line) {
            if(c=='|') { a.push_back(x); x=""; }
            else x+=c;
        }
        a.push_back(x);
        if(a.size()!=8) return false;
        try {
            id=stoi(a[0]); semester=stoi(a[2]);
            tuition=stod(a[3]); library=stod(a[4]);
            lab=stod(a[5]); exam=stod(a[6]); other=stod(a[7]);
        } catch(...) { return false; }
        program=a[1];
        return true;
    }
};

class Payment {
public:
    int id, studentId;
    double amount, discount, fine;
    string date, method;

    Payment() {}
    Payment(int i,int s,double a,double d,double f,string dt,string m) {
        id=i; studentId=s; amount=a; discount=d; fine=f;
        date=dt; method=m;
    }

    double net() const {
        return amount-discount+fine;
    }

    string save() const {
        return to_string(id)+"|"+to_string(studentId)+"|"+
               to_string(amount)+"|"+to_string(discount)+"|"+
               to_string(fine)+"|"+date+"|"+method;
    }

    bool load(string line) {
        vector<string> a;
        string x;
        for(char c: line) {
            if(c=='|') { a.push_back(x); x=""; }
            else x+=c;
        }
        a.push_back(x);
        if(a.size()!=7) return false;
        try {
            id=stoi(a[0]); studentId=stoi(a[1]);
            amount=stod(a[2]); discount=stod(a[3]);
            fine=stod(a[4]);
        } catch(...) { return false; }
        date=a[5]; method=a[6];
        return true;
    }
};

class FeeSystem {
private:
    vector<Student> students;
    vector<Fee> fees;
    vector<Payment> payments;

    void clear() {
        cin.clear();
        cin.ignore(10000,'\n');
    }

    int getInt(string msg,int low,int high) {
        int x;
        while(true) {
            cout << msg;
            if(cin>>x && x>=low && x<=high) {
                cin.ignore(10000,'\n');
                return x;
            }
            cout << "Invalid input. Try again.\n";
            clear();
        }
    }

    double getDouble(string msg,double low,double high) {
        double x;
        while(true) {
            cout << msg;
            if(cin>>x && x>=low && x<=high) {
                cin.ignore(10000,'\n');
                return x;
            }
            cout << "Invalid amount. Try again.\n";
            clear();
        }
    }

    string getString(string msg) {
        string x;
        do {
            cout << msg;
            getline(cin,x);
            if(x.empty()) cout << "This field is required.\n";
        } while(x.empty());
        for(char &c:x) if(c=='|') c='/';
        return x;
    }

    Student* findStudent(int id) {
        for(auto &s:students)
            if(s.id==id) return &s;
        return nullptr;
    }

    Fee* findFee(int id) {
        for(auto &f:fees)
            if(f.id==id) return &f;
        return nullptr;
    }

    Fee* studentFee(int sid) {
        Student *s=findStudent(sid);
        if(!s) return nullptr;
        for(auto &f:fees)
            if(f.program==s->program && f.semester==s->semester)
                return &f;
        return nullptr;
    }

    double paid(int sid) {
        double total=0;
        for(auto &p:payments)
            if(p.studentId==sid) total+=p.net();
        return total;
    }

    void pause() {
        cout << "\nPress Enter to continue...";
        cin.ignore(10000,'\n');
    }

    void header(string title) {
        cout << "\n==================================================\n";
        cout << "          COLLEGE FEE MANAGEMENT SYSTEM\n";
        cout << "==================================================\n";
        cout << title << "\n";
        cout << "--------------------------------------------------\n";
    }

    void saveStudents() {
        ofstream out("students.txt");
        for(auto &s:students) out<<s.save()<<'\n';
    }

    void saveFees() {
        ofstream out("fees.txt");
        for(auto &f:fees) out<<f.save()<<'\n';
    }

    void savePayments() {
        ofstream out("payments.txt");
        for(auto &p:payments) out<<p.save()<<'\n';
    }

    void loadStudents() {
        ifstream in("students.txt");
        string line;
        while(getline(in,line)) {
            Student s;
            if(s.load(line)) students.push_back(s);
        }
    }

    void loadFees() {
        ifstream in("fees.txt");
        string line;
        while(getline(in,line)) {
            Fee f;
            if(f.load(line)) fees.push_back(f);
        }
    }

    void loadPayments() {
        ifstream in("payments.txt");
        string line;
        while(getline(in,line)) {
            Payment p;
            if(p.load(line)) payments.push_back(p);
        }
    }

    int nextStudent() {
        int n=1000;
        for(auto &s:students) n=max(n,s.id);
        return n+1;
    }

    int nextFee() {
        int n=0;
        for(auto &f:fees) n=max(n,f.id);
        return n+1;
    }

    int nextPayment() {
        int n=0;
        for(auto &p:payments) n=max(n,p.id);
        return n+1;
    }

    string today() {
        return "2026-09-25";
    }

public:
    FeeSystem() {
        loadStudents();
        loadFees();
        loadPayments();

        if(fees.empty()) {
            fees.push_back(Fee(1,"BCA",1,35000,2000,4000,3000,1000));
            fees.push_back(Fee(2,"BCA",2,35000,2000,4000,3000,1000));
            fees.push_back(Fee(3,"BIM",1,38000,2000,4500,3000,1000));
            fees.push_back(Fee(4,"BBS",1,30000,1500,2500,2500,1000));
            saveFees();
        }
    }

    void addStudent() {
        header("ADD STUDENT");

        int id=nextStudent();
        string name=getString("Name: ");
        string program=getString("Program: ");
        int sem=getInt("Semester (1-8): ",1,8);
        string phone=getString("Phone: ");
        string email=getString("Email: ");

        students.push_back(
            Student(id,name,program,sem,phone,email)
        );
        saveStudents();

        cout << "\nStudent added successfully.";
        cout << "\nStudent ID: " << id << "\n";
    }

    void viewStudents() {
        header("VIEW STUDENTS");

        if(students.empty()) {
            cout<<"No student records.\n";
            return;
        }

        cout<<left<<setw(8)<<"ID"
            <<setw(24)<<"Name"
            <<setw(12)<<"Program"
            <<setw(8)<<"Sem"
            <<setw(15)<<"Phone"
            <<setw(14)<<"Balance"<<"\n";

        cout<<string(81,'-')<<"\n";

        for(auto &s:students) {
            double due=0;
            Fee *f=studentFee(s.id);
            if(f) due=max(0.0,f->total()-paid(s.id));

            cout<<left<<setw(8)<<s.id
                <<setw(24)<<s.name.substr(0,23)
                <<setw(12)<<s.program
                <<setw(8)<<s.semester
                <<setw(15)<<s.phone
                <<"Rs. "<<fixed<<setprecision(2)<<due<<"\n";
        }
    }

    void searchStudent() {
        header("SEARCH STUDENT");

        string key=getString("Enter name/program/phone: ");
        int count=0;

        for(auto &s:students) {
            if(s.name.find(key)!=string::npos ||
               s.program.find(key)!=string::npos ||
               s.phone.find(key)!=string::npos) {
                s.display();
                cout<<"Balance: Rs. ";
                Fee *f=studentFee(s.id);
                cout<<fixed<<setprecision(2)
                    <<(f?max(0.0,f->total()-paid(s.id)):0)<<"\n";
                count++;
            }
        }

        cout<<"\nMatches: "<<count<<"\n";
    }

    void updateStudent() {
        header("UPDATE STUDENT");

        int id=getInt("Student ID: ",1,999999);
        Student *s=findStudent(id);

        if(!s) {
            cout<<"Student not found.\n";
            return;
        }

        cout<<"Enter new details.\n";
        s->name=getString("Name: ");
        s->program=getString("Program: ");
        s->semester=getInt("Semester: ",1,8);
        s->phone=getString("Phone: ");
        s->email=getString("Email: ");

        saveStudents();
        cout<<"Student updated successfully.\n";
    }

    void deleteStudent() {
        header("DELETE STUDENT");

        int id=getInt("Student ID: ",1,999999);

        auto it=find_if(
            students.begin(),students.end(),
            [id](Student &s){return s.id==id;}
        );

        if(it==students.end()) {
            cout<<"Student not found.\n";
            return;
        }

        students.erase(it);
        saveStudents();
        cout<<"Student deleted successfully.\n";
    }

    void addFee() {
        header("ADD FEE STRUCTURE");

        int id=nextFee();
        string p=getString("Program: ");
        int s=getInt("Semester: ",1,8);

        for(auto &f:fees)
            if(f.program==p && f.semester==s) {
                cout<<"Fee structure already exists.\n";
                return;
            }

        double t=getDouble("Tuition: Rs. ",0,10000000);
        double l=getDouble("Library: Rs. ",0,10000000);
        double lb=getDouble("Lab: Rs. ",0,10000000);
        double e=getDouble("Exam: Rs. ",0,10000000);
        double o=getDouble("Other: Rs. ",0,10000000);

        fees.push_back(Fee(id,p,s,t,l,lb,e,o));
        saveFees();

        cout<<"Fee structure added.\n";
    }

    void viewFees() {
        header("FEE STRUCTURES");

        for(auto &f:fees) {
            f.display();
            cout<<string(40,'-')<<"\n";
        }
    }

    void updateFee() {
        header("UPDATE FEE");

        int id=getInt("Fee ID: ",1,999999);
        Fee *f=findFee(id);

        if(!f) {
            cout<<"Fee structure not found.\n";
            return;
        }

        f->tuition=getDouble("New tuition: Rs. ",0,10000000);
        f->library=getDouble("New library: Rs. ",0,10000000);
        f->lab=getDouble("New lab: Rs. ",0,10000000);
        f->exam=getDouble("New exam: Rs. ",0,10000000);
        f->other=getDouble("New other: Rs. ",0,10000000);

        saveFees();
        cout<<"Fee structure updated.\n";
    }

    void deleteFee() {
        header("DELETE FEE");

        int id=getInt("Fee ID: ",1,999999);

        auto it=find_if(
            fees.begin(),fees.end(),
            [id](Fee &f){return f.id==id;}
        );

        if(it==fees.end()) {
            cout<<"Fee structure not found.\n";
            return;
        }

        fees.erase(it);
        saveFees();
        cout<<"Fee structure deleted.\n";
    }

    void payFee() {
        header("RECORD PAYMENT");

        int sid=getInt("Student ID: ",1,999999);
        Student *s=findStudent(sid);

        if(!s) {
            cout<<"Student not found.\n";
            return;
        }

        Fee *f=studentFee(sid);

        if(!f) {
            cout<<"Fee structure not found for this student.\n";
            return;
        }

        double due=max(0.0,f->total()-paid(sid));

        cout<<"Student: "<<s->name<<"\n";
        cout<<"Total Fee: Rs. "<<f->total()<<"\n";
        cout<<"Already Paid: Rs. "<<paid(sid)<<"\n";
        cout<<"Outstanding: Rs. "<<due<<"\n\n";

        double amount=getDouble("Payment: Rs. ",0.01,10000000);
        double discount=getDouble("Discount: Rs. ",0,amount);
        double fine=getDouble("Fine: Rs. ",0,10000000);

        string method=getString(
            "Payment Method (Cash/Bank/Card/Wallet): "
        );

        payments.push_back(
            Payment(
                nextPayment(),sid,amount,discount,fine,
                today(),method
            )
        );

        savePayments();

        cout<<"\nPayment successful.\n";
        cout<<"Net payment: Rs. "
            <<fixed<<setprecision(2)
            <<amount-discount+fine<<"\n";

        cout<<"New balance: Rs. "
            <<max(0.0,f->total()-paid(sid))<<"\n";

        ofstream receipt("receipts.txt",ios::app);
        receipt<<"Receipt "<<payments.back().id
               <<" | Student "<<sid
               <<" | Rs. "<<payments.back().net()
               <<" | "<<today()<<"\n";
    }

    void paymentHistory() {
        header("PAYMENT HISTORY");

        if(payments.empty()) {
            cout<<"No payment records.\n";
            return;
        }

        for(auto &p:payments) {
            cout<<"\nPayment ID : "<<p.id;
            cout<<"\nStudent ID : "<<p.studentId;
            cout<<"\nAmount     : Rs. "<<p.amount;
            cout<<"\nDiscount   : Rs. "<<p.discount;
            cout<<"\nFine       : Rs. "<<p.fine;
            cout<<"\nNet Paid   : Rs. "<<p.net();
            cout<<"\nDate       : "<<p.date;
            cout<<"\nMethod     : "<<p.method<<"\n";
            cout<<string(40,'-')<<"\n";
        }
    }

    void studentSummary() {
        header("STUDENT FEE SUMMARY");

        int id=getInt("Student ID: ",1,999999);
        Student *s=findStudent(id);

        if(!s) {
            cout<<"Student not found.\n";
            return;
        }

        Fee *f=studentFee(id);

        cout<<"\nStudent: "<<s->name;
        cout<<"\nProgram: "<<s->program;
        cout<<"\nSemester: "<<s->semester;

        cout<<"\nRequired Fee: Rs. "
            <<(f?f->total():0);

        cout<<"\nTotal Paid: Rs. "<<paid(id);

        cout<<"\nOutstanding: Rs. "
            <<(f?max(0.0,f->total()-paid(id)):0)
            <<"\n";
    }

    void outstandingReport() {
        header("OUTSTANDING FEE REPORT");

        double total=0;
        int count=0;

        for(auto &s:students) {
            Fee *f=studentFee(s.id);
            double due=f?max(0.0,f->total()-paid(s.id)):0;

            if(due>0) {
                cout<<s.id<<" | "<<s.name
                    <<" | "<<s.program
                    <<" | Rs. "<<fixed<<setprecision(2)
                    <<due<<"\n";
                total+=due;
                count++;
            }
        }

        cout<<"\nStudents with outstanding fee: "<<count;
        cout<<"\nTotal outstanding: Rs. "
            <<fixed<<setprecision(2)<<total<<"\n";
    }

    void collectionReport() {
        header("COLLECTION REPORT");

        double gross=0,discount=0,fine=0,net=0;

        for(auto &p:payments) {
            gross+=p.amount;
            discount+=p.discount;
            fine+=p.fine;
            net+=p.net();
        }

        cout<<"Transactions : "<<payments.size();
        cout<<"\nGross Amount : Rs. "<<gross;
        cout<<"\nDiscount     : Rs. "<<discount;
        cout<<"\nFine         : Rs. "<<fine;
        cout<<"\nNet Collection: Rs. "<<net<<"\n";
    }

    void dashboard() {
        header("DASHBOARD");

        cout<<"Total Students   : "<<students.size()<<"\n";
        cout<<"Fee Structures   : "<<fees.size()<<"\n";
        cout<<"Payments         : "<<payments.size()<<"\n";

        double collection=0;
        double outstanding=0;

        for(auto &p:payments) collection+=p.net();

        for(auto &s:students) {
            Fee *f=studentFee(s.id);
            if(f) outstanding+=max(0.0,f->total()-paid(s.id));
        }

        cout<<"Total Collection : Rs. "
            <<fixed<<setprecision(2)<<collection<<"\n";

        cout<<"Outstanding      : Rs. "
            <<outstanding<<"\n";
    }


    void sortStudents() {
        header("STUDENTS SORTED BY NAME");

        vector<Student> list=students;

        sort(list.begin(),list.end(),
            [](const Student &a,const Student &b) {
                return a.name<b.name;
            });

        for(const auto &s:list) {
            cout<<s.id<<" | "
                <<s.name<<" | "
                <<s.program<<" | Semester "
                <<s.semester<<"\n";
        }

        if(list.empty())
            cout<<"No students available.\n";
    }

    void filterProgram() {
        header("FILTER STUDENTS BY PROGRAM");

        string program=getString("Program: ");
        int count=0;

        for(const auto &s:students) {
            if(s.program==program) {
                cout<<s.id<<" | "
                    <<s.name<<" | Semester "
                    <<s.semester<<" | "
                    <<s.phone<<"\n";
                count++;
            }
        }

        cout<<"\nMatching students: "<<count<<"\n";
    }

    void studentFeeDetails() {
        header("DETAILED FEE INFORMATION");

        int id=getInt("Student ID: ",1,999999);
        Student *s=findStudent(id);

        if(!s) {
            cout<<"Student not found.\n";
            return;
        }

        Fee *f=studentFee(id);

        if(!f) {
            cout<<"No fee structure assigned.\n";
            return;
        }

        cout<<"\nStudent: "<<s->name<<"\n";
        cout<<"Program: "<<s->program<<"\n";
        cout<<"Semester: "<<s->semester<<"\n";
        cout<<"-----------------------------\n";
        cout<<"Tuition : Rs. "<<f->tuition<<"\n";
        cout<<"Library : Rs. "<<f->library<<"\n";
        cout<<"Lab     : Rs. "<<f->lab<<"\n";
        cout<<"Exam    : Rs. "<<f->exam<<"\n";
        cout<<"Other   : Rs. "<<f->other<<"\n";
        cout<<"Total   : Rs. "<<f->total()<<"\n";
        cout<<"Paid    : Rs. "<<paid(id)<<"\n";
        cout<<"Balance : Rs. "
            <<max(0.0,f->total()-paid(id))<<"\n";
    }

    void searchPayment() {
        header("SEARCH PAYMENT");

        string key=getString("Payment ID / Student ID / Method: ");
        int count=0;

        for(const auto &p:payments) {
            string id=to_string(p.id);
            string sid=to_string(p.studentId);

            if(id==key || sid==key || p.method==key) {
                cout<<"\nPayment ID: "<<p.id;
                cout<<"\nStudent ID: "<<p.studentId;
                cout<<"\nAmount: Rs. "<<p.amount;
                cout<<"\nDiscount: Rs. "<<p.discount;
                cout<<"\nFine: Rs. "<<p.fine;
                cout<<"\nNet: Rs. "<<p.net();
                cout<<"\nDate: "<<p.date;
                cout<<"\nMethod: "<<p.method<<"\n";
                count++;
            }
        }

        cout<<"\nMatches: "<<count<<"\n";
    }

    void updatePayment() {
        header("UPDATE PAYMENT");

        int id=getInt("Payment ID: ",1,999999);
        Payment *p=nullptr;

        for(auto &item:payments)
            if(item.id==id) p=&item;

        if(!p) {
            cout<<"Payment not found.\n";
            return;
        }

        p->amount=getDouble("New amount: Rs. ",0.01,10000000);
        p->discount=getDouble("New discount: Rs. ",0,p->amount);
        p->fine=getDouble("New fine: Rs. ",0,10000000);
        p->method=getString("New payment method: ");

        savePayments();
        cout<<"Payment updated successfully.\n";
    }

    void deletePayment() {
        header("DELETE PAYMENT");

        int id=getInt("Payment ID: ",1,999999);

        auto it=find_if(
            payments.begin(),payments.end(),
            [id](Payment &p){return p.id==id;}
        );

        if(it==payments.end()) {
            cout<<"Payment not found.\n";
            return;
        }

        payments.erase(it);
        savePayments();
        cout<<"Payment deleted successfully.\n";
    }

    void paymentByStudent() {
        header("PAYMENTS BY STUDENT");

        int sid=getInt("Student ID: ",1,999999);
        Student *s=findStudent(sid);

        if(!s) {
            cout<<"Student not found.\n";
            return;
        }

        int count=0;
        double total=0;

        cout<<"Student: "<<s->name<<"\n";

        for(const auto &p:payments) {
            if(p.studentId==sid) {
                cout<<"\nPayment ID: "<<p.id;
                cout<<"\nAmount: Rs. "<<p.amount;
                cout<<"\nDiscount: Rs. "<<p.discount;
                cout<<"\nFine: Rs. "<<p.fine;
                cout<<"\nNet: Rs. "<<p.net();
                cout<<"\nDate: "<<p.date;
                cout<<"\nMethod: "<<p.method<<"\n";

                total+=p.net();
                count++;
            }
        }

        cout<<"\nTransactions: "<<count;
        cout<<"\nTotal Paid: Rs. "<<total<<"\n";
    }

    void methodReport() {
        header("PAYMENT METHOD REPORT");

        double cash=0,bank=0,card=0,wallet=0,other=0;

        for(const auto &p:payments) {
            if(p.method=="Cash") cash+=p.net();
            else if(p.method=="Bank") bank+=p.net();
            else if(p.method=="Card") card+=p.net();
            else if(p.method=="Wallet") wallet+=p.net();
            else other+=p.net();
        }

        cout<<fixed<<setprecision(2);
        cout<<"Cash       : Rs. "<<cash<<"\n";
        cout<<"Bank       : Rs. "<<bank<<"\n";
        cout<<"Card       : Rs. "<<card<<"\n";
        cout<<"Wallet     : Rs. "<<wallet<<"\n";
        cout<<"Other      : Rs. "<<other<<"\n";
    }

    void programReport() {
        header("PROGRAM-WISE REPORT");

        vector<string> programs;

        for(const auto &s:students) {
            bool exists=false;

            for(const auto &p:programs)
                if(p==s.program) exists=true;

            if(!exists) programs.push_back(s.program);
        }

        for(const auto &program:programs) {
            int count=0;
            double required=0;
            double collected=0;

            for(const auto &s:students) {
                if(s.program==program) {
                    Fee *f=studentFee(s.id);

                    if(f) required+=f->total();

                    collected+=paid(s.id);
                    count++;
                }
            }

            cout<<"\nProgram: "<<program;
            cout<<"\nStudents: "<<count;
            cout<<"\nRequired: Rs. "<<required;
            cout<<"\nCollected: Rs. "<<collected;
            cout<<"\nOutstanding: Rs. "
                <<max(0.0,required-collected)<<"\n";
        }

        if(programs.empty())
            cout<<"No student records.\n";
    }

    void feeSearch() {
        header("SEARCH FEE STRUCTURE");

        string program=getString("Program: ");
        int count=0;

        for(const auto &f:fees) {
            if(f.program==program) {
                f.display();
                count++;
            }
        }

        cout<<"\nMatching fee structures: "<<count<<"\n";
    }

    void studentMenu() {
        while(true) {
            header("STUDENT MANAGEMENT");

            cout<<"1. Add Student\n";
            cout<<"2. View Students\n";
            cout<<"3. Search Student\n";
            cout<<"4. Update Student\n";
            cout<<"5. Delete Student\n";
            cout<<"6. Sort by Name\n";
            cout<<"7. Filter by Program\n";
            cout<<"8. Detailed Fee Information\n";
            cout<<"0. Back\n";

            int c=getInt("Choice: ",0,8);
            if(c==0) return;

            if(c==1) addStudent();
            if(c==2) viewStudents();
            if(c==3) searchStudent();
            if(c==4) updateStudent();
            if(c==5) deleteStudent();
            if(c==6) sortStudents();
            if(c==7) filterProgram();
            if(c==8) studentFeeDetails();

            pause();
        }
    }

    void feeMenu() {
        while(true) {
            header("FEE STRUCTURE MANAGEMENT");

            cout<<"1. Add Fee Structure\n";
            cout<<"2. View Fee Structures\n";
            cout<<"3. Search Fee Structure\n";
            cout<<"4. Update Fee Structure\n";
            cout<<"5. Delete Fee Structure\n";
            cout<<"6. Student Fee Details\n";
            cout<<"0. Back\n";

            int c=getInt("Choice: ",0,6);
            if(c==0) return;

            if(c==1) addFee();
            if(c==2) viewFees();
            if(c==3) feeSearch();
            if(c==4) updateFee();
            if(c==5) deleteFee();
            if(c==6) studentFeeDetails();

            pause();
        }
    }

    void paymentMenu() {
        while(true) {
            header("PAYMENT MANAGEMENT");

            cout<<"1. Record Payment\n";
            cout<<"2. Payment History\n";
            cout<<"3. Student Fee Summary\n";
            cout<<"4. Search Payment\n";
            cout<<"5. Update Payment\n";
            cout<<"6. Delete Payment\n";
            cout<<"7. Student Payments\n";
            cout<<"0. Back\n";

            int c=getInt("Choice: ",0,7);
            if(c==0) return;

            if(c==1) payFee();
            if(c==2) paymentHistory();
            if(c==3) studentSummary();
            if(c==4) searchPayment();
            if(c==5) updatePayment();
            if(c==6) deletePayment();
            if(c==7) paymentByStudent();

            pause();
        }
    }


    void paymentSummaryReport() {
        header("PAYMENT SUMMARY REPORT");

        double totalAmount=0;
        double totalDiscount=0;
        double totalFine=0;
        double totalNet=0;

        for(const auto &p:payments) {
            totalAmount+=p.amount;
            totalDiscount+=p.discount;
            totalFine+=p.fine;
            totalNet+=p.net();
        }

        cout<<fixed<<setprecision(2);
        cout<<"\nNumber of payment records: "
            <<payments.size()<<"\n";
        cout<<"Total payment amount: Rs. "
            <<totalAmount<<"\n";
        cout<<"Total discount: Rs. "
            <<totalDiscount<<"\n";
        cout<<"Total fine: Rs. "
            <<totalFine<<"\n";
        cout<<"Total net collection: Rs. "
            <<totalNet<<"\n";
    }

    void feeStatistics() {
        header("FEE STRUCTURE STATISTICS");

        if(fees.empty()) {
            cout<<"No fee structures available.\n";
            return;
        }

        double total=0;
        double highest=fees[0].total();
        double lowest=fees[0].total();

        for(const auto &f:fees) {
            double amount=f.total();

            total+=amount;
            if(amount>highest) highest=amount;
            if(amount<lowest) lowest=amount;
        }

        cout<<fixed<<setprecision(2);
        cout<<"Number of fee structures: "
            <<fees.size()<<"\n";
        cout<<"Average fee: Rs. "
            <<total/fees.size()<<"\n";
        cout<<"Highest fee: Rs. "
            <<highest<<"\n";
        cout<<"Lowest fee: Rs. "
            <<lowest<<"\n";
    }

    void studentStatistics() {
        header("STUDENT STATISTICS");

        int bca=0;
        int bim=0;
        int bbs=0;
        int other=0;

        for(const auto &s:students) {
            if(s.program=="BCA")
                bca++;
            else if(s.program=="BIM")
                bim++;
            else if(s.program=="BBS")
                bbs++;
            else
                other++;
        }

        cout<<"Total students: "<<students.size()<<"\n";
        cout<<"BCA students: "<<bca<<"\n";
        cout<<"BIM students: "<<bim<<"\n";
        cout<<"BBS students: "<<bbs<<"\n";
        cout<<"Other programs: "<<other<<"\n";
    }

    void semesterReport() {
        header("SEMESTER-WISE REPORT");

        for(int sem=1;sem<=8;sem++) {
            int count=0;

            for(const auto &s:students)
                if(s.semester==sem)
                    count++;

            if(count>0)
                cout<<"Semester "<<sem
                    <<" : "<<count
                    <<" student(s)\n";
        }
    }

    void unpaidStudents() {
        header("STUDENTS WITH UNPAID FEES");

        int count=0;

        for(const auto &s:students) {
            Fee *f=studentFee(s.id);

            if(!f)
                continue;

            double due=f->total()-paid(s.id);

            if(due>0) {
                cout<<"\nID: "<<s.id;
                cout<<"\nName: "<<s.name;
                cout<<"\nProgram: "<<s.program;
                cout<<"\nDue: Rs. "<<fixed
                    <<setprecision(2)<<due<<"\n";

                count++;
            }
        }

        cout<<"\nTotal unpaid students: "
            <<count<<"\n";
    }

    void completeStudentReport() {
        header("COMPLETE STUDENT REPORT");

        for(const auto &s:students) {
            Fee *f=studentFee(s.id);
            double required=f?f->total():0;
            double totalPaid=paid(s.id);
            double balance=max(0.0,required-totalPaid);

            cout<<"\nID: "<<s.id;
            cout<<"\nName: "<<s.name;
            cout<<"\nProgram: "<<s.program;
            cout<<"\nSemester: "<<s.semester;
            cout<<"\nPhone: "<<s.phone;
            cout<<"\nEmail: "<<s.email;
            cout<<"\nRequired Fee: Rs. "<<required;
            cout<<"\nPaid Fee: Rs. "<<totalPaid;
            cout<<"\nBalance: Rs. "<<balance;
            cout<<"\n-----------------------------\n";
        }
    }

    void reportMenu() {
        while(true) {
            header("REPORTS / SUMMARIES");

            cout<<"1. Dashboard\n";
            cout<<"2. Collection Report\n";
            cout<<"3. Outstanding Report\n";
            cout<<"4. Student Fee Summary\n";
            cout<<"5. Payment Method Report\n";
            cout<<"6. Program-wise Report\n";
            cout<<"7. Payment Summary\n";
            cout<<"8. Fee Statistics\n";
            cout<<"9. Student Statistics\n";
            cout<<"10. Semester Report\n";
            cout<<"11. Unpaid Students\n";
            cout<<"12. Complete Student Report\n";
            cout<<"0. Back\n";

            int c=getInt("Choice: ",0,12);
            if(c==0)
                return;

            if(c==1)
                dashboard();

            if(c==2)
                collectionReport();

            if(c==3)
                outstandingReport();

            if(c==4)
                studentSummary();

            if(c==5)
                methodReport();

            if(c==6)
                programReport();

            if(c==7)
                paymentSummaryReport();

            if(c==8)
                feeStatistics();

            if(c==9)
                studentStatistics();

            if(c==10)
                semesterReport();

            if(c==11)
                unpaidStudents();

            if(c==12)
                completeStudentReport();

            pause();
        }
    }

    void run() {
        while(true) {
            header("MAIN MENU");

            cout<<"1. Student Management\n";
            cout<<"2. Fee Structure Management\n";
            cout<<"3. Payment Management\n";
            cout<<"4. Reports / Summaries\n";
            cout<<"0. Exit\n";

            int c=getInt("Choice: ",0,4);

            if(c==0) {
                saveStudents();
                saveFees();
                savePayments();
                cout<<"\nThank you. Program closed.\n";
                break;
            }

            if(c==1) studentMenu();
            if(c==2) feeMenu();
            if(c==3) paymentMenu();
            if(c==4) reportMenu();
        }
    }
};

int main() {
    FeeSystem system;
    system.run();
    return 0;
}
