// landlord.cpp
// models the finances for an apartment building
#include "landlord.h"   // for class declarations, etc.

////////////////////////////global functions////////////////////////
void getaLine(string& inStr)    // get line of text
{
    char temp[21];
    cin.get(temp,20,'\n');
    cin.ignore(80,'\n');
    inStr = temp;
}
//--------------------------------------------------------------------
char getaChar()                 // get a character
{
    char ch = cin.get();
    cin.ignore(80,'\n');
    return ch;
}
//--------------------------------------------------------------------

//////////////////////methods for class tenant////////////////////////
tenant::tenant() : name(""), aptNumber(0)
{   }
tenant::tenant(string n, int aNo) : name(n), aptNumber(aNo)
{   }
//--------------------------------------------------------------------
tenant::~tenant()
{   }
//--------------------------------------------------------------------
int tenant::getAptNumber()
{ return aptNumber; }
//--------------------------------------------------------------------
bool operator < (const tenant& t1, const tenant& t2)
{
    return t1.name < t2.name; 
}
//--------------------------------------------------------------------
bool operator == (const tenant& t1, const tenant& t2)
{
    return t1.name == t2.name;
}
//---------------------------------------------------------------------
ostream& operator << (ostream& s, const tenant& t)
{
    s << t.aptNumber << '\t' << t.name << endl;
    return s;
}
//---------------------------------------------------------------------
void tenant::serialize(ofstream& out) const
{
    size_t nameLength = name.size();
    out.write(reinterpret_cast<const char*>(&nameLength), sizeof(nameLength));
    out.write(name.c_str(), nameLength);
    out.write(reinterpret_cast<const char*>(&aptNumber), sizeof(aptNumber));
}
//---------------------------------------------------------------------
void tenant::deserilize(ifstream& in)
{
    size_t nameLength;
    in.read(reinterpret_cast<char*>(&nameLength),sizeof(nameLength));
    name.resize(nameLength);
    in.read(&name[0], nameLength);
    in.read(reinterpret_cast<char*>(&aptNumber), sizeof(aptNumber));
}   
//---------------------------------------------------------------------

//////////////////method for class tenantInputScreen///////////////////
void tenantInputScreen::getTenant()     // get tenant info
{
    cout << "Enter tenant's name (Geoger Smith): ";
    getaLine(tName);
    cout << "Enter tenant's apartment number (101): ";
    cin >> aptNo;
    cin.ignore(80,'\n');
                                // create tenant object
    tenant* ptrTenant = new tenant(tName, aptNo);
                                // insert tenant pointer to tenant list
    ptrTenantList->insertTenant(ptrTenant);  

    // write the tenant object to a file            // open in append mode
    ofstream outfile("tenants.dat", ios::binary | ios::app); 
    if (outfile.is_open())
    {
        ptrTenant->serialize(outfile);
        outfile.close();
    }
    else
        cerr << "Error: Could not open file for writing" << endl;
}

////////////////function object -- compareTenants//////////////////////
bool compareTenants::operator()(tenant* ptrT1, tenant* ptrT2) const
{
    return *ptrT1 < *ptrT2;
}
///////////////////////////////////////////////////////////////////////

///////////////method for class tenantlist/////////////////////////////
tenantList::~tenantList()       // destructor
{
    while ( !setPtrsTens.empty() )      // delete all tenants,
    {                                   // remove ptrs from set
        iter = setPtrsTens.begin();
        delete *iter;
        setPtrsTens.erase(iter);
    }
}       // end ~tenantList()
//--------------------------------------------------------------------
void tenantList::insertTenant(tenant* ptrT)
{
    setPtrsTens.insert(ptrT);       // insert
}
//---------------------------------------------------------------------
int tenantList::getAptNo(string tName)      // name on list
{
    int aptNo;
    tenant dummy(tName,0);
    iter = setPtrsTens.begin();
    while (iter != setPtrsTens.end())
    {
        aptNo = (*iter)->getAptNumber();        // look for tenant
        if (dummy == **iter++)              // on the list?
            return aptNo;                   // yes
    }
        return -1;                          // no
}
//----------------------------------------------------------------------
void tenantList::display()
{
    
    cout << "\nApt No \t\t Tenant Name List\n--------------------------\n";
    if (setPtrsTens.empty())
    {
        cout << "***No tenants***\n";
    }
    else
    {
        iter = setPtrsTens.begin();
        while (iter != setPtrsTens.end())
            {
                cout << **iter++;
            }
    }
}       // end display()
//----------------------------------------------------------------------
void tenantList::loadfromFile(const string& filename)
{
    ifstream inFile(filename, ios::binary);
    if (inFile.is_open())
    {
        while (inFile.peek()!=EOF)
        {
            tenant* ptrTenant = new tenant();
            ptrTenant->deserilize(inFile);
            setPtrsTens.insert(ptrTenant);  
        }
        inFile.close();
    }
    else
        cerr << "Error: Could not open file for reading" << endl;
}
//-----------------------------------------------------------------------

///////////////////method for class rentRow////////////////////////////
rentRow::rentRow()
{
    aptNo = 0;
    fill(&rent[0],&rent[12],0.0f);
}
//----------------------------------------------------------------------
rentRow::rentRow(int an) : aptNo(an)    // 1-arg constructor
{
    fill(&rent[0], &rent[12],0.0f);
}
//----------------------------------------------------------------------
void rentRow::setRent(int m, float am)
{
    rent[m] = am;
}
//----------------------------------------------------------------------
float rentRow::getSumOfRow()            // sum of rents in row
{
    return accumulate(&rent[0], &rent[12],0.0f);
}
//----------------------------------------------------------------------
bool operator < (const rentRow& t1, const rentRow& t2)
{
    return t1.aptNo < t2.aptNo;
}
//----------------------------------------------------------------------
bool operator == (const rentRow& t1, const rentRow& t2)
{
    return t1.aptNo == t2.aptNo;
}
//----------------------------------------------------------------------
ostream& operator << (ostream& s, const rentRow& t)
{
    s << t.aptNo << '\t';           // print apartment number
    for (int i = 0; i < 12; i++)    // print 12 rents
    {
        if (t.rent[i] == 0)
            s << "  0  ";
        else
            s << t.rent[i] << "  ";
    }
    s << endl;
    return s;
}
//----------------------------------------------------------------------
int rentRow::getaptNo()
{
    return aptNo;
}
//----------------------------------------------------------------------
void rentRow::serialize(ofstream& out) const
{
    // write apartment number
    out.write(reinterpret_cast<const char*>(&aptNo),sizeof(aptNo));
    // write rent array
    out.write(reinterpret_cast<const char*>(rent),sizeof(rent));
}
//----------------------------------------------------------------------
void rentRow::deserilize(ifstream& in)
{
    // read apartment number
    in.read(reinterpret_cast<char*>(&aptNo),sizeof(aptNo));
    // read rent array
    in.read(reinterpret_cast<char*>(rent), sizeof(rent));
}
/////////////////////////////end of class rentRow///////////////////////

/////////////////////function object -- compare rentRows////////////////
bool compareRows::operator()(rentRow* ptrRR1, rentRow* ptrRR2) const
{
    return *ptrRR1 < *ptrRR2;
}
////////////////////////////////////////////////////////////////////////

/////////////////////method for class rentRecord////////////////////////
rentRecord::~rentRecord()                   // destructor
{
    
    while (!setPtrsRR.empty())       // delete rent rows,
    {                                       // remove ptrs from set
        iter = setPtrsRR.begin();
        delete *iter;
        setPtrsRR.erase(iter);

    }
}
//----------------------------------------------------------------------
void rentRecord::insertRent(int apNo, int m, float r)
{
    rentRow searchRow(apNo);        // temp row with same aptNo // dummy object
    iter = setPtrsRR.begin();       // search setPtrRR
    while (iter != setPtrsRR.end())
    {
        if (**iter == searchRow)    // rentRow found?
        {                               // yes,
            (*iter)->setRent(m, r);     // put rent in row    

            // update the exisiting record in the file
            updateRentInFile(apNo, m, r);
            return;
        }
        else iter++;
    }         

    // didn't find it, create new rentRow object
    rentRow* ptrRow = new rentRow(apNo);    
    ptrRow->setRent(m, r);                  // put rent it row
    setPtrsRR.insert(ptrRow);                 // put row in set

    // Write the tenant object to a file    // open in append mode
    ofstream outfile("rent.dat", ios::binary | ios::app);
    if (outfile.is_open())
    {
        ptrRow->serialize(outfile);
        outfile.close();
    }
    else
        cerr << "Error: Could not open file for writing" << endl;
}   // end insertRent()
//----------------------------------------------------------------------
void rentRecord::updateRentInFile(int apNo, int m, float r)
{
    // open the file in read mode
    ifstream infile("rent.dat", ios::binary);
    ofstream tempfile("temp.dat", ios::binary | ios::trunc);

    if (!infile.is_open() || !tempfile.is_open())
    {
        cerr << "Error: Could not open file for updating" << endl;
        return;
    }
    
    rentRow row;    // create a new row object for rentRow and read all rows to this 
    while (infile.read(reinterpret_cast<char*>(&row), sizeof(rentRow)))
    {
        if (row.getaptNo() == apNo)
            row.setRent(m,r);   // Update rent for the specific month
        
        row.serialize(tempfile);
    }

    infile.close();
    tempfile.close();

    // Replace the original file with the updated temp file
    remove("rent.dat");
    rename("temp.dat", "rent.dat");
}
//----------------------------------------------------------------------
void rentRecord::display()
{
    cout << "Apt No\t Jan  Feb  Mar  Apr  May  Jun  Jul  Aug  "
        << "Sep  Oct  Nov  Dec\n"
        << "--------------------------------------------------"
        << "--------------------\n";

    if (setPtrsRR.empty())
        cout << "\n***No rentss***\n";
    else
    {
        iter = setPtrsRR.begin();
        while (iter != setPtrsRR.end())
            cout << **iter++;
    } 
}
//----------------------------------------------------------------------
float rentRecord::getSumOfRents()
{
    float total = 0.0;
    iter = setPtrsRR.begin();
    while (iter != setPtrsRR.end())
    {
        total += (*iter)->getSumOfRow();
        iter++;
    }
    return total;
}
//----------------------------------------------------------------------
void rentRecord::loadfromFile(const string& filename)
{
    ifstream inFile(filename, ios::binary);
    if(inFile.is_open())
    {
        while (inFile.peek()!=EOF)
        {
            rentRow* ptrRow = new rentRow();
            ptrRow->deserilize(inFile);
            setPtrsRR.insert(ptrRow);
        }
        inFile.close();
    }
    else
        cerr << "Error: Could not open file for reading" << endl;
}
//----------------------------------------------------------------------

///////////////////method for class rentInputScreen/////////////////////
void rentInputScreen::getRent()     // rent for one tenant and one month
{
    cout << "Enter tenant's name: ";
    getaLine(renterName);
    aptNo = ptrTenantList->getAptNo(renterName);
    if (aptNo != -1)          // renter name found
    {
        cout << "Enter month (1-12): ";
        cin >> month;
        cin.ignore(80,'\n');
        cout << "Enter amount to be paid (345.67): ";
        cin >> rentPaid;
        cin.ignore(80, '\n');
        month--;            // internal month is 0-11
        ptrRentRecord->insertRent(aptNo, month, rentPaid);
    }
    else
        cout << "No tenant with that name.\n";
}   // end getRent()
//----------------------------------------------------------------------


/////////////////////////method for class expense///////////////////////
bool operator < (const expense& e1, const expense& e2)
{
    if (e1.month == e2.month)       // sort as per date of expense
        return e1.day < e2.day;     // compare months,
    else                            // if same month compare days
        return e1.month < e2.month; 
    
}
//----------------------------------------------------------------------
bool operator == (const expense& e1, const expense &e2)
{
    return e1.month == e2.month && e1.day == e2.day;
}
//----------------------------------------------------------------------
ostream& operator << (ostream& s, const expense& e)
{
    s << e.month << "/" << e.day << '\t' << e.payee << '\t';
    s << e.amount << '\t' << e.category << '\n';
    return s;
}
//----------------------------------------------------------------------
void expense::serialize(ofstream& out) const
{
    out.write(reinterpret_cast<const char*>(&month), sizeof(month));
    out.write(reinterpret_cast<const char*>(&day), sizeof(day));

    size_t categorySize = category.size();
    out.write(reinterpret_cast<const char*>(&categorySize), sizeof(categorySize));
    out.write(category.c_str(), categorySize);

    size_t payeeSize = payee.size();
    out.write(reinterpret_cast<const char*>(&payeeSize), sizeof(payeeSize));
    out.write(payee.c_str(), payeeSize);

    out.write(reinterpret_cast<const char*>(&amount), sizeof(amount));
}
//----------------------------------------------------------------------
void expense::deserialize(ifstream& in)
{
    in.read(reinterpret_cast<char*>(&month), sizeof(month));
    in.read(reinterpret_cast<char*>(&day), sizeof(day));

    size_t categorySize;
    in.read(reinterpret_cast<char*>(&categorySize), sizeof(categorySize));
    category.resize(categorySize);
    in.read(&category[0], categorySize);

    size_t payeeSize;
    in.read(reinterpret_cast<char*>(&payeeSize), sizeof(payeeSize));
    payee.resize(payeeSize);
    in.read(&payee[0], payeeSize);

    in.read(reinterpret_cast<char*>(&amount), sizeof(amount));
}
//----------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////
bool compareDates::operator() (expense* ptrE1, expense* ptrE2) const
{  return *ptrE1 < *ptrE2;  }
//----------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////
bool compareCategories::operator() (expense* ptrE1, expense* ptrE2) const
{  return ptrE1->category < ptrE2->category; }
//----------------------------------------------------------------------

//////////////////////method for class expenseRecord////////////////////
expenseRecord::~expenseRecord()
{
    if (vectPtrsExpenses.empty())       // destructor
    {  }
    else
    {
        while (!vectPtrsExpenses.empty())              
        {
            iter = vectPtrsExpenses.begin();
            delete *iter;
            vectPtrsExpenses.erase(iter);
        }
        
    }
}
//----------------------------------------------------------------------
void expenseRecord::insertExp(expense* ptrExp)
{
    vectPtrsExpenses.push_back(ptrExp);
}
//----------------------------------------------------------------------
void expenseRecord::display()
{
    cout << "\nDate\tPayee\t\tAmount\tCategory\n";
    if (vectPtrsExpenses.size() == 0)
        cout << "***No expenses***\n";
    else
    {                                       // sort by date
        sort(vectPtrsExpenses.begin(), vectPtrsExpenses.end(),
                 compareDates());

        iter = vectPtrsExpenses.begin();
        while (iter != vectPtrsExpenses.end())
            cout << **iter++;
    }    
}
//----------------------------------------------------------------------
float expenseRecord::displaySummary()       // used by annualReport
{
    float totalExpenses = 0;                    // total, all categories
    if (vectPtrsExpenses.size() == 0)
    {
        cout << "\tAll categories\t0\n";
        return 0;
    }
    
    // sort by category
    sort(vectPtrsExpenses.begin(), vectPtrsExpenses.end(),
             compareCategories());
    
    // for each category, summ the entries, print category wise sum as well

    iter = vectPtrsExpenses.begin();
    string tempCat = (*iter)->category;
    float sumCat = 0.0;
    while (iter != vectPtrsExpenses.end())
    {
        if (tempCat == (*iter)->category)
            sumCat += (*iter)->amount;      // same categoru
        else                                // different category
            {
                cout << '\t' << tempCat << '\t' << sumCat << endl;
                totalExpenses += sumCat;        // add previous category
                tempCat = (*iter)->category;
                sumCat = (*iter)->amount;
            }
            iter++;
    }   // end while
    totalExpenses += sumCat;            // add final category
    cout << '\t' << tempCat << '\t' << sumCat << endl;
    return totalExpenses;   
}   // end displaySummary()
//----------------------------------------------------------------------
void expenseRecord::loadFromFile(const string& filename)
{
    ifstream inFile(filename, ios::binary);
    if (inFile.is_open())
    {
        while (inFile.peek()!=EOF)
        {
            expense* ptrExpense = new expense();
            ptrExpense->deserialize(inFile);
            vectPtrsExpenses.push_back(ptrExpense);
        }
        inFile.close();
    }
    else
        cerr << "Error: Could not open file for reading" << endl; 
}
//----------------------------------------------------------------------

////////////////methods for class expenseInputScreen////////////////////
expenseInputScreen::expenseInputScreen(expenseRecord* ptrER)
{
    ptrExpenseRecord = ptrER;
}
//----------------------------------------------------------------------
void expenseInputScreen::getExpense()
{
    int month ,day;
    string category,payee;
    float amount;

    cout << "Enter month (1-12): ";
    cin >> month;
    cin.ignore(80,'\n');
    cout << "Enter day(1-31): ";
    cin >> day;
    cin.ignore(80, '\n');
    cout << "Enter expense category (Repairing, Utilities): ";
    getaLine(category);
    cout << "Enter payee (Bob's Hardware, Big Electric Co): ";
    getaLine(payee);
    cout << "Enter amount (39.95): ";
    cin >> amount;
    cin.ignore(80,'\n');

    expense* ptrExpense = new expense(month, day, category, payee, amount);
    ptrExpenseRecord->insertExp(ptrExpense);

    // Write the expense object to a file       // open in append mode
    ofstream outfile("expense.dat", ios::binary | ios::app);
    if (outfile.is_open())
    {
        ptrExpense->serialize(outfile);
        outfile.close();
    }
    else
        cerr << "Error: Could not open file for writing" << endl;
}
//----------------------------------------------------------------------


////////////////////method for class maintenance Request/////////////////
bool operator == (const maintenanceRequest& m1, const maintenanceRequest& m2)
{
    return m1.reqID == m2.reqID;
}
//-----------------------------------------------------------------------
bool operator < (const maintenanceRequest& m1, const maintenanceRequest& m2)
{
    return m1.reqID < m2.reqID;
}
//-----------------------------------------------------------------------
ostream& operator << (ostream& s, const maintenanceRequest& m)
{
    s << '\n' << m.reqID << '\t' << m.month << '/' << m.day << '\t'
         << m.aptNo << '\t' << m.description << '\t' << m.status;
    return s;
}
//-----------------------------------------------------------------------
void maintenanceRequest::serialize(ofstream& out) const
{
    out.write(reinterpret_cast<const char*>(&reqID), sizeof(reqID));
    out.write(reinterpret_cast<const char*>(&aptNo), sizeof(aptNo));
    out.write(reinterpret_cast<const char*>(&month), sizeof(month));
    out.write(reinterpret_cast<const char*>(&day), sizeof(day));

    size_t descriptionSize = description.size();
    out.write(reinterpret_cast<const char*>(&descriptionSize), sizeof(descriptionSize));
    out.write(description.c_str(), descriptionSize);

    size_t statusSize = status.size();
    out.write(reinterpret_cast<const char*>(&statusSize), sizeof(statusSize));
    out.write(status.c_str(), statusSize);
}
//-----------------------------------------------------------------------
void maintenanceRequest::deserialize(ifstream& in)
{
    in.read(reinterpret_cast<char*>(&reqID), sizeof(reqID));
    in.read(reinterpret_cast<char*>(&aptNo), sizeof(aptNo));
    in.read(reinterpret_cast<char*>(&month), sizeof(month));
    in.read(reinterpret_cast<char*>(&day), sizeof(day));

    size_t descriptionSize;
    in.read(reinterpret_cast<char*>(&descriptionSize), sizeof(descriptionSize));
    description.resize(descriptionSize);
    in.read(&description[0], descriptionSize);

    size_t statusSize;
    in.read(reinterpret_cast<char*>(&statusSize), sizeof(statusSize));
    status.resize(statusSize);
    in.read(&status[0], statusSize);
}
//-----------------------------------------------------------------------
/////////////////////////////////////////////////////////////////////////
bool compareReqID::operator() (maintenanceRequest* ptrMR1, maintenanceRequest* ptrMR2) const
{
    return *ptrMR1 < *ptrMR2;
}
//----------------------------------------------------------------------

/////////////////method for class maintenance Record/////////////////////
maintenanceRecord::~maintenanceRecord()     // destructor
{
    while(!setPtrMR.empty())
    {
        iter = setPtrMR.begin();
        delete *iter;           // delete maintenance Request object
        setPtrMR.erase(iter);   // remove pointer from set
    }
}
//-----------------------------------------------------------------------
void maintenanceRecord::insertRequest(maintenanceRequest* ptrMR)
{
    setPtrMR.insert(ptrMR);
}
//-----------------------------------------------------------------------
void maintenanceRecord::display()
{
    cout << "ReqId" << "\t" << "Request Date" << "\t" << "AptNo" << "\t"
            << "Description" << "\t\t\t" << "Status" << "\n";
    cout << "----------------------------------------------------------------------------------------------";
    if (setPtrMR.empty())
        cout << "\n***No Maintenance Request***\n";
    else
    {
        iter = setPtrMR.begin();
        while (iter != setPtrMR.end())
            cout << **iter++;
    }
    cout << endl;
}
//-----------------------------------------------------------------------
void maintenanceRecord::loadFromFile(const string& filename)
{
    ifstream inFile(filename, ios::binary);
    if (inFile.is_open())
    {
        while (inFile.peek() != EOF)
        {
            maintenanceRequest* ptMReq = new maintenanceRequest;
            ptMReq->deserialize(inFile);
            setPtrMR.insert(ptMReq);
        }
        inFile.close();
    }
    else
        cerr << "Error: Could not open file for reading";
        cout << endl;
}
//-----------------------------------------------------------------------

//////////////method for class maintenance Input Screen//////////////////
maintenanceInputScreen::maintenanceInputScreen(maintenanceRecord* ptrMRecord)
{
    ptrMaintenanceRecord = ptrMRecord;
}
//-----------------------------------------------------------------------
void maintenanceInputScreen::getRequest()
{
    int reqID;
    int aptNo;
    int month, day;
    string description;
    string status;

    cout << "Enter ReqID: "; cin >> reqID;
    cin.ignore(80,'\n');
    cout << "Enter Apartment No: "; cin >> aptNo;
    cin.ignore(80,'\n');
    cout << "Enter month: "; cin >> month;
    cin.ignore(80,'\n');
    cout << "Enter day: "; cin >> day;
    cin.ignore(80,'\n');
    cout << "Enter maintainence details: "; 
    getaLine(description);
    cout << "Enter status: ";
    getaLine(status);

    maintenanceRequest* ptrMRequest = new maintenanceRequest(reqID, aptNo, month, day, description, status);
    ptrMaintenanceRecord->insertRequest(ptrMRequest);

    // write maintenance Request object to a file
    ofstream outfile("maintenance.dat", ios::binary | ios::app);
    if (outfile.is_open())
    {
        ptrMRequest->serialize(outfile);
        outfile.close();
    }
    else
        cerr << "Error: Coud not open file for writing" << endl;
}
//----------------------------------------------------------------------


///////////////////method for class annualReport////////////////////////
annualReport::annualReport(rentRecord* pRR, expenseRecord* pER)
{
    ptrRR = pRR;
    ptrER = pER;
}
//----------------------------------------------------------------------
void annualReport::display()
{
    cout << "Annual Summary\n-------------------------------\n";
    cout << "Income\n";
    cout << "\t Rent \t\t";
    rents =  ptrRR->getSumOfRents();
    cout << rents << endl;

    cout << "Expenses\n";
    expenses = ptrER->displaySummary();
    cout << "\t\t\t" << expenses << endl;

    cout << "Balace\t\t\t" << rents - expenses << endl;
}
//----------------------------------------------------------------------

//////////////////method for class userLogin////////////////////////////
userLogin::userLogin(string u, string p) : username(u), password(p)
{   }
//----------------------------------------------------------------------
string userLogin::getUsername()
{
    return username;
}
//----------------------------------------------------------------------
string userLogin::getPassword()
{
    return password;
}
/*
/////////////////method for class landlordLogin////////////////////////
landlordLogin::landlordLogin(string& u, string& p) : userLogin(u, p)
{   }
//-----------------------------------------------------------------------
void landlordLogin::displayOptions() const override
{
    cout << "1. View Reports\n2. Manage Tenants\n3. Manage Expenses\n4. Exit\n";
}

///////////////method for class tenantLogin//////////////////////////////
tenantLogin::tenantLogin(string& u, string& p) : userLogin(u, p)
{   }
//-----------------------------------------------------------------------
void tenantLogin::displayOptions() const override
{
    cout << "1. Submit Maintenance Request\n2. Exit\n";
}
*/

//////////////////method for class userLoginRecord//////////////////////
userLoginRecord::userLoginRecord()
{
    setPtrsUsers.insert(new userLogin("subek", "acharya"));
    setPtrsUsers.insert(new userLogin("landlord", "password"));
}
//----------------------------------------------------------------------
userLoginRecord::~userLoginRecord()
{
    if (setPtrsUsers.empty())              // destructor
    {   }
    else
    {
        while (!setPtrsUsers.empty())
        {
            iter = setPtrsUsers.begin();
            delete *iter;
            setPtrsUsers.erase(iter);
        }   
    }
}
//----------------------------------------------------------------------
bool userLoginRecord::authenticate(const string& u, const string& p)
{
    if (setPtrsUsers.empty())
        cout << "No user details found in database. Sign up to continue.";
    else
    {
        iter = setPtrsUsers.begin();
        while (iter != setPtrsUsers.end())
        {
            if (((*iter)->getUsername() == u) && ((*iter)->getPassword() == p))
                return true;
            iter++;
        }
    }
    return false;
}
//-----------------------------------------------------------------------
void userLoginRecord::insertNewUser(userLogin* ptrUL)
{
    setPtrsUsers.insert(ptrUL);
}
//-----------------------------------------------------------------------
void userLoginRecord::display()
{
    iter = setPtrsUsers.begin();
    while (iter != setPtrsUsers.end())
    { 
        cout << (**iter).getUsername() << "\t" << (**iter).getPassword() << "\n";
        iter++;
    }
    cout << endl;
}

//////////////////method for class loginInputScreen/////////////////////
loginInputScreen::loginInputScreen(userLoginRecord* ptrULR)
{
    ptrUserLoginRecord = ptrULR;
}
//----------------------------------------------------------------------
char loginInputScreen::interface()
{
    char ch;
    cout << "Enter 'l' to login in, \n"
         << "      's' to sign up: ";
    ch = getaChar();
    return ch;
}
//----------------------------------------------------------------------
bool loginInputScreen::getRequest()
{
    string username;
    string password;
    char retry;

    while (true)
    {
        cout << "Enter username: ";
        getaLine(username);
        cout << "Enter password: ";
        getaLine(password);

        if (ptrUserLoginRecord->authenticate(username,password))
            return true;
        else
        {
            cout << "Login Failed. Would you like to try again?(y/n): ";
            retry = getaChar();
            if (retry != 'y' && retry != 'Y')
                return false;
        }
    }
}
//----------------------------------------------------------------------
void loginInputScreen::createNewUser()
{
    string username, password;
    cout << "Enter username: ";
    getaLine(username);
    cout << "Enter password: ";
    getaLine(password);

    userLogin* ptrUserLogin = new userLogin(username, password);
    ptrUserLoginRecord->insertNewUser(ptrUserLogin);

    cout << "Your account has been created \n";
       //  << "Your usename is: " << username << "\n"
       //  << "Your password is: " << password << "\n";
    cout << endl;

   // ptrUserLoginRecord->display();
}


//////////////////method for class userInterface////////////////////////
userInterface::userInterface()
{
    // these reports exist for the life of a program
    ptrTenantList = new tenantList;
    ptrRentRecord = new rentRecord;
    ptrExpenseRecord = new expenseRecord;
    ptrMaintenanceRecord = new maintenanceRecord;
    ptrUserLoginRecord = new userLoginRecord;

    // load tenants from file
    ptrTenantList->loadfromFile("tenants.dat");
    ptrRentRecord->loadfromFile("rent.dat");
    ptrExpenseRecord->loadFromFile("expense.dat");
    ptrMaintenanceRecord->loadFromFile("maintenance.dat");
    //ptrUserLoginRecord->loadFromFile("login.dat");
}
//----------------------------------------------------------------------
userInterface::~userInterface()
{
    delete ptrTenantList;
    delete ptrRentRecord;
    delete ptrExpenseRecord;
    delete ptrMaintenanceRecord;
    delete ptrUserLoginRecord;
    
}
//----------------------------------------------------------------------
void userInterface::interact()
{
    ptrLoginInputScreen = new loginInputScreen(ptrUserLoginRecord);

    while (ptrLoginInputScreen->interface() == 's')
    {
        ptrLoginInputScreen->createNewUser();
    }

    if (!ptrLoginInputScreen->getRequest())
    {
        cout << "Login process ended. Exiting...\n";
        delete ptrLoginInputScreen;
        return;
    }
    delete ptrLoginInputScreen;

    while (true)
    {
        cout << "Enter 'i' to input data, \n"
            << "      'd' to display a report, \n"
            << "      'q' to quit program: ";
        
        ch = getaChar();

        if (ch == 'i')
        {
            cout << "Enter 't' to add tenant, \n"
                << "      'r' to record rent payment, \n"
                << "      'e' to record expense, \n"
                << "      'm' to record maintenane: ";
        
            ch = getaChar();
            switch (ch)
            {
                    // imput screens exist only while being used 
                case 't':
                    ptrTenantInputScreen = new tenantInputScreen(ptrTenantList);
                    ptrTenantInputScreen->getTenant();
                    delete ptrTenantInputScreen;
                    break;
                case 'r':
                    ptrRentInputScreen = new rentInputScreen(ptrTenantList, ptrRentRecord);
                    ptrRentInputScreen->getRent();
                    delete ptrRentInputScreen;
                    break;
                case 'e':
                    ptrExpenseInputScreen = new expenseInputScreen(ptrExpenseRecord);
                    ptrExpenseInputScreen->getExpense();
                    delete ptrExpenseInputScreen;
                    break;
                case 'm':
                    ptrMaintenanceInputScreen = new maintenanceInputScreen(ptrMaintenanceRecord);
                    ptrMaintenanceInputScreen->getRequest();
                    delete ptrMaintenanceInputScreen;
                    break;
                default:
                    cout << "Unknown input option\n";
                    break;
            }  // end switch
        }   // end if
        else if ( ch == 'd' )                       // display data
        {
            cout << "Enter 't' to display tenants, \n"
                << "       'r' to display rents, \n"
                << "       'e' to display expenses, \n "
                << "       'm' to display Maintenance, \n "
                << "       'a' to display annual report: ";
            ch = getaChar();
            switch (ch)
            {
            case 't': ptrTenantList->display();
                break;
            case 'r': ptrRentRecord->display();
                break;
            case 'e': ptrExpenseRecord->display();
                break;
            case 'm': ptrMaintenanceRecord->display();
                break;
            case 'a': 
                ptrAnnualReport = new annualReport(ptrRentRecord, ptrExpenseRecord);
                ptrAnnualReport->display();
                delete ptrAnnualReport;
                break;
            default:
                cout << "Unknown input option\n";
                break;
            }   // end switch
        }   // end else-if
        else if (ch == 'q')
            return;
        else
            cout << "Unknown option. Enter only 'i', 'd' or 'q'\n";
    }   // end while
}   // end interact()
//-----------------------------------------------------------------------

////////////////////end of file landlord.cpp/////////////////////////////