#include <fstream>
#include <iostream>
#include <vector>
using namespace std;
#include <cstdlib>
#include <iomanip>
#include <string>
#include <cctype>


struct stClient {
    string Name;
    string AccountNumber;
    string PinCode;
    string PhoneNumber;
    int BalanceAccount;
};

stClient CurrentClient;

vector <string> SplitString (string OneClient, string sample = "#//#")
{
    
    vector <string> vWords;
    int position = 0;
    string Words;
    
    while ((position = OneClient.find(sample)) != std::string::npos) {
        Words = OneClient.substr(0, position);
        
        if (Words != "") {
            vWords.push_back(Words);
        }
        OneClient.erase(0, position + sample.size());
    }
    if (OneClient != "") {
        vWords.push_back(OneClient);
    }
    return vWords;
}

stClient AddToOneStClient (string OneClient)
{
    stClient Client;
    vector <string> vClients;
    
    vClients = SplitString(OneClient);
    
    Client.Name = vClients[0];
    Client.AccountNumber = vClients[1];
    Client.PinCode = vClients[2];
    Client.PhoneNumber = vClients[3];
    Client.BalanceAccount = stoi(vClients[4]);
    
    return Client;
}

vector <stClient> LoadAllClientsFromFile ()
{
    fstream myfile;
    vector <stClient> Clients;
    
    myfile.open("myfile.txt", ios::in);
    
    string OneClient;
    if (myfile.is_open()) {
        
        while (getline(myfile, OneClient)) {
            Clients.push_back(AddToOneStClient(OneClient));
        }
        myfile.close();
    }
    
    return Clients;
}

bool FindClient (string AccountNumber, string PinCode, stClient &Client)
{
    vector <stClient> Clients;
    
    Clients = LoadAllClientsFromFile();
    
    for (stClient &KUnde : Clients) {
        if (KUnde.AccountNumber == AccountNumber && KUnde.PinCode == PinCode) {
            CurrentClient = KUnde;
            return true;
        }
    }
    return false;
}

bool LoadClientFromFile (string AccountNumber, string PinCode)
{
    if (FindClient(AccountNumber, PinCode, CurrentClient)) {
        return true;
    }
    return false;
}

void DisPlayTheSection (string comment)
{
    cout << "==========================" << endl;
    cout << "   " << comment << endl;
    cout << "==========================" << endl;

}

void PrintTheOption ()
{
    DisPlayTheSection("Quick Withdraw");
    cout << "  [1] 20     [2] 50\n  [3] 100    [4] 200\n  [5] 400    [6] 600\n  [7] 800    [8] 1000\n " << endl;
    cout << "your Balance is [" << CurrentClient.BalanceAccount << "]" << endl;
}

int InsertTheChoice ()
{
    
    int Choice = 0;
    
    if (Choice < 1 || Choice > 8) {
        cout << "choose what to Withdraw from [1] to [8]" << endl;
        cin >> Choice;
    }
    return Choice;
}

int ChoiceTheValue (int Choice)
{
    
    switch (Choice) {
        case 1:
            return 20;
            break;
        case 2:
            return 50;
            break;
        case 3:
            return 100;
            break;
        case 4:
            return 200;
            break;
        case 5:
            return 400;
            break;
        case 6:
            return 600;
            break;
        case 7:
            return 800;
            break;
        case 8:
            return 1000;
        default :
            return 0;
    }
}

string convertClientsToString (stClient client, string sample = "#//#")
{
    string StringClient;
    
    StringClient += client.Name + sample;
    StringClient += client.AccountNumber + sample;
    StringClient += client.PinCode + sample;
    StringClient += client.PhoneNumber + sample;
    StringClient += to_string(client.BalanceAccount);
    return StringClient;
}

void InsertClientsTofile (vector <stClient> Clients)
{
    
    fstream myfile;
    myfile.open("myfile.txt", ios::out);
    
    if (myfile.is_open()) {
        
        for (stClient &KUnde : Clients) {
            myfile << convertClientsToString(KUnde);
        }
        
        myfile.close();
    }
}

void WithdrawAndDeposit (short TheValue, stClient &CurrentClient)
{
    vector <stClient> Clients;
    
    Clients = LoadAllClientsFromFile();
    
    for (stClient &kunde : Clients) {
        if (kunde.AccountNumber == CurrentClient.AccountNumber && kunde.PinCode == CurrentClient.PinCode) {
            kunde.BalanceAccount += TheValue;
        }
    }
    
    InsertClientsTofile(Clients);
    
    CurrentClient.BalanceAccount += TheValue;
    cout << "Done Succesfuly Your Balance [" << CurrentClient.BalanceAccount << "] " << endl;
    
}

void QuikWithDraw ()
{
     PrintTheOption();
    
    int TheChoice = InsertTheChoice();
    
    short TheValueOfWithDraw = ChoiceTheValue(TheChoice);
    
    char CheckPoint;
    cout << "Do You Want really to WithDraw" << endl;
    cin >> CheckPoint;
    
    if (CheckPoint == 'y') {
        if (CurrentClient.BalanceAccount > TheValueOfWithDraw) {
            WithdrawAndDeposit(-TheValueOfWithDraw, CurrentClient);
        } else {
            cout << "you dont Have Enough Money" << endl;
        }
    }
}

int InsertAmountOfmoneyToWithDraw ()
{
    
    int AmountOfmoney = 3;
    
    while (AmountOfmoney % 5 != 0) {
        cout << "Enter an Amount Muliple Of 5 " << endl;
        cin >> AmountOfmoney;
    }
    
    return AmountOfmoney;
}

void NormalWithDraw ()
{
    DisPlayTheSection("Normal WithDraw Screen");
    
    int AmountOfMoney = 0;
    AmountOfMoney = InsertAmountOfmoneyToWithDraw();
    
    if (CurrentClient.BalanceAccount > AmountOfMoney) {
        WithdrawAndDeposit(-AmountOfMoney, CurrentClient);
    } else {
        cout << "you Dont have Enough Money" << endl;
        cout << "your Balance is " << CurrentClient.BalanceAccount << endl;
    }
}

int InsertAmountOfMoneyToDeposit ()
{
    
    int AmountOfmoney = 0;
    
    while (AmountOfmoney <= 0) {
        cout << "Enter a Psitive Number " << endl;
        cin >> AmountOfmoney;
    }
    
    return AmountOfmoney;
}

void Deposit ()
{
    
    DisPlayTheSection("Enter Amount Of money to deposit");
    
    int DepositValue = InsertAmountOfMoneyToDeposit();
    
    WithdrawAndDeposit(DepositValue, CurrentClient);
    
}

void Login ()
{
    
    string AccountNumber, PinCode;
    
    bool LoginFailed = false;
    
    DisPlayTheSection("ATM Main Menu Screen");
    
    do {
        
        if (LoginFailed) {
            cout << "your PinCode/AccountNumber Incorrect" << endl;
        }
        
        cout << "Enter Account Number" << endl;
        getline(cin >> ws, AccountNumber);
        
        cout << "Enter PinCode" << endl;
        getline(cin >> ws, PinCode);
        
        LoginFailed = !LoadClientFromFile(AccountNumber, PinCode);
        
    } while (LoginFailed);
    
//    QuikWithDraw();
    
}

int main() {

    Login();
    
//    NormalWithDraw();
    
    Deposit();

    return 0;
}
