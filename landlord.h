// landlord.h
// header file for landlord.cpp -- contains class declarations, etc.
#include <iostream>
#include <vector>
#include <set>
#include <string>
#include <algorithm>           // for sort()
#include <numeric>              // for accumulate()
#include <fstream>
using namespace std;

/////////////////////////global methods/////////////////////////////////
void getaLine(string& inStr);       // get line of text
char getaChar();                    // get a character

/////////////////////////class tenant///////////////////////////////////
class tenant
{
    private:
        string name;        // tenant's name
        int aptNumber;      // tenant's apartment number
        // other tenant information (phone, etc.) could go here

    public:
        tenant();   // no-arg constructor
        tenant(string n, int aNo);      // 2-arg constructor
        ~tenant();
        int getAptNumber();

        // needed for use in 'set'
        friend bool operator < (const tenant&, const tenant&);
        friend bool operator == (const tenant&, const tenant&);

        // for I/O
        friend ostream& operator << (ostream&, const tenant&);

        // Serialization    // write to a file
        void serialize(ofstream&) const;

        // Deserialization // read object from file
        void deserilize(ifstream&);
};  // end class tenant

/////////////////////////class compareTenants///////////////////////////
class compareTenants        // function object  -- compare tenants
{
    public:
        bool operator () (tenant*, tenant*) const;
};

////////////////////////class tenantList////////////////////////////////
class tenantList
{
    private:
        // set of pointers to tenants
        set<tenant*, compareTenants> setPtrsTens;
        set<tenant*, compareTenants>::iterator iter;

    public:
        ~tenantList();      // destructor(deletes tenants)
        void insertTenant(tenant*);     // put tenant pointer to list
        int getAptNo(string);       // return apartment number
        void display();             // display tenant list
        void loadfromFile(const string&);   // load tenant object from file
} ;   // end class tenantList

////////////////////////class tenantInputScreen/////////////////////////
class tenantInputScreen
{
    private:
        tenantList* ptrTenantList;
        string tName;
        int aptNo;

    public:                             // 1-arg constructor
        tenantInputScreen(tenantList* ptrTL) : ptrTenantList(ptrTL)
        {   }
        void getTenant();
};   // end class tenantInputScreen

//////////////////////////class rentRow/////////////////////////////////
// one row of the rent record: an address and 12 rent amounts
class rentRow
{
    private:
        int aptNo;
        float rent[12];
    public:
        rentRow();  // no-arg constructor
        rentRow(int);       // 1-arg constructor
        int getaptNo();
        void setRent(int, float);       // record rent for one month
        float getSumOfRow();            // return sum of rents in row

        // needed tp store in 'set'
        friend bool operator < (const rentRow&, const rentRow&);
        friend bool operator == (const rentRow&, const rentRow&);

        // for I/O
        friend ostream& operator << (ostream&, const rentRow&);

        // Serialization    // write to a file
        void serialize(ofstream&) const;

        // Deserialization // read object from file
        void deserilize(ifstream&);
};  // end class rentRow

///////////////////////////class compareRows/////////////////////////////
class compareRows   // function object -- compares rentRows
{
    public:
        bool operator() (rentRow*, rentRow*) const;
};

////////////////////////////class rentRecord////////////////////////////
class rentRecord
{
    private:
        //set of pointers to rentRow objects (one per tenant)
        set<rentRow*, compareRows> setPtrsRR;
        set<rentRow*, compareRows>::iterator iter;

    public:
        ~rentRecord();
        void insertRent(int, int,float);
        void display();
        float getSumOfRents();          // sum all rents in record
        void loadfromFile(const string&);   // load rentRow object from file
        void updateRentInFile(int, int, float);  // update the existing recod in file
};  // end class rentRecord

/////////////////////////////rentInputScreen////////////////////////////
class rentInputScreen
{
    private:
        tenantList* ptrTenantList;
        rentRecord* ptrRentRecord;

        string renterName;
        float rentPaid;
        int month;
        int aptNo;

    public:                         // 2-arg constructor
        rentInputScreen(tenantList* ptrTL, rentRecord* ptrRR) 
        {
            ptrTenantList = ptrTL;
            ptrRentRecord = ptrRR;
        }
        void getRent();     // rent for one tenant and one month
};  // end class rentInputScreen

/////////////////////////////class expense//////////////////////////////
class expense
{
    public:
        int month, day;
        string category, payee;
        float amount;

        expense()               // no-arg constructor
        {   }
        expense(int m, int d, string c, string p, float a):
        month(m), day(d), category(c), payee(p), amount(a)
        {   }
        
        // needed for use in 'set'
        friend bool operator < (const expense&, const expense&);
        friend bool operator == (const expense&, const expense&);

        // needed for output
        friend ostream& operator << (ostream&, const expense&);

        // serialization    // write to a file
        void serialize(ofstream&) const;

        // Deserialization  // read object from file
        void deserialize(ifstream&);
};  // end class expense

////////////////////////////////////////////////////////////////////////
class compareDates  // function object -- compares expenses
{
    public:
        bool operator () (expense*, expense*) const;
};
////////////////////////////////////////////////////////////////////////
class compareCategories     // function object -- compares expenses
{
    public:
        bool operator () (expense*, expense*) const;
};

////////////////////////////class expenseRecord////////////////////////
class expenseRecord
{
    private:
        // vector of pointers to expenses
        vector<expense*> vectPtrsExpenses;
        vector<expense*>::iterator iter;

    public:
        ~expenseRecord();
        void insertExp(expense*);
        void display();
        float displaySummary();     // used by annualReport
        void loadFromFile(const string&);
};  // end class expenseRecord

/////////////////////////class expenseInputScreen///////////////////////
class expenseInputScreen
{
    private:
        expenseRecord* ptrExpenseRecord;
    
    public:
        expenseInputScreen(expenseRecord*);     // 1-arg constructor
        void getExpense();
};  // end class expenseInputScreen

////////////////////////class Maintenance Request////////////////////////
class maintenanceRequest
{
    public:
        int reqID;
        int aptNo;
        int month, day;
        string description;
        string status;

        maintenanceRequest()    // no-arg constructor
        {   }
        
        maintenanceRequest(int r, int a, int m, int d, string des, string s):
        reqID(r), aptNo(a), month(m), day(d), description(des), status(s)
        {   }

        //needed for use in 'set'
        friend bool operator == (const maintenanceRequest&, const maintenanceRequest&);
        friend bool operator < (const maintenanceRequest&, const maintenanceRequest&);

        //needed for output
        friend ostream& operator << (ostream&, const maintenanceRequest&);

        // Serialization    // write to a file
        void serialize(ofstream&) const;

        // Deseialization   // read object from file
        void deserialize(ifstream&);
};  // end of class Maintenance Request

/////////////////////////////////////////////////////////////////////////
class compareReqID // function object -- compare reqID
{
    public:
        bool operator()  (maintenanceRequest*, maintenanceRequest* ) const;
};

/////////////////////class Maintenance Record////////////////////////////
class maintenanceRecord
{
    private:
        set<maintenanceRequest*, compareReqID> setPtrMR;
        set<maintenanceRequest*, compareReqID>:: iterator iter;
    
    public:
        ~maintenanceRecord();
        void insertRequest(maintenanceRequest*); // insert request object to set
        void display();
        void loadFromFile(const string&);
};   // end of class Maintenance Record

///////////////////class Maintenance Input Screen//////////////////////
class maintenanceInputScreen
{
    private:
        maintenanceRecord* ptrMaintenanceRecord;

    public:
        maintenanceInputScreen(maintenanceRecord*);  // 1-arg constructor
        void getRequest();   
};      // end of class Maintenance Input Screen

//////////////////////////class annualReport///////////////////////////
class annualReport
{
    private:
        rentRecord* ptrRR;
        expenseRecord* ptrER;
        float expenses, rents;

    public:
        annualReport(rentRecord*, expenseRecord*); // 2-arg constructor
        void display();
};  // end class annualReport
//---------------------------------------------------------------------------

////////////////////////////class userLogin///////////////////////////////////
class userLogin    
{
    private:
        string username;
        string password;
    
    public:         // 2-arg constructor
        userLogin(string, string);
       // virtual ~userLogin()     // destructor
       // {   }
       // virtual void displayOptions() const = 0;  // pure virtual funciton
        string getUsername();
        string getPassword();      
};  // end class userLogin


/*
////////////////////////class landlordLogin//////////////////////////////
class landlordLogin : public userLogin       // class landLord
{
    public:
        landlordLogin(string&, string&);
        void displayOptions() const override;
};  // end class landlordLogin

///////////////////////class tenantLogin/////////////////////////////////
class tenantLogin : public userLogin      // class tenant user
{
    public:
        tenantLogin(string&, string&);
        void displayOptions() const override;
};  // end class tenantLogin
*/

///////////////////////class userLoginRecord/////////////////////////////
class userLoginRecord
{
    private:
        set<userLogin*> setPtrsUsers;
        set<userLogin*>::iterator iter;
    
    public:
        userLoginRecord();
        ~userLoginRecord();
        //void loadFromFile(const strin&);
        bool authenticate(const string&, const string&);
        void insertNewUser(userLogin*);
        void display();
};  // end class userLogin Record

///////////////////////class loginInputScreen////////////////////////////
class loginInputScreen
{
    private:
        userLoginRecord* ptrUserLoginRecord;
    public:
        loginInputScreen(userLoginRecord*);     // 1-arg constructor
        bool getRequest();
        char interface();
        void createNewUser();
};  // end class loginInputScreen

////////////////////////////class userIntrface//////////////////////////
class userInterface
{
    private:

        tenantList* ptrTenantList;
        tenantInputScreen* ptrTenantInputScreen;
        
        rentRecord* ptrRentRecord;
        rentInputScreen* ptrRentInputScreen;
        
        expenseRecord* ptrExpenseRecord;
        expenseInputScreen* ptrExpenseInputScreen;
        
        annualReport* ptrAnnualReport;
        
        maintenanceInputScreen* ptrMaintenanceInputScreen;
        maintenanceRecord* ptrMaintenanceRecord;

        loginInputScreen* ptrLoginInputScreen;
        userLoginRecord* ptrUserLoginRecord;

        char ch;
    public:
        userInterface();
        ~userInterface();

        void interact();
};  // end class userInterface

////////////////////////////end file landlord.h//////////////////////////


