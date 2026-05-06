#include <ctime>
#include <iostream>
#include <string>
#include <cmath>
#include <vector>
#include <iomanip>
#include <fstream>
#include <cctype>
#include "MyLip.h"

using namespace std;
const string FileNameClient = "Client.txt";
const string UserFileName = "Users.txt";

void ShowMainMenue();
void GoBackToMainMenue();
void ShowTransactionMenueScreen();
void ShowMangeUserScreen();
void Login();


enum enCheckClient
{
	ShowClient = 1, AddClient = 2, DeletClient = 3, UpdateClient = 4, FindClient = 5, Transactions = 6, ManageUser = 7, Logout = 8
};

enum enCheckTransactions
{
	Deposit = 1, Withdraw = 2, TotalBalance = 3, ManMenue = 4
};

enum enCheckManageUser
{
	listUser = 1, AddUser = 2, DeletUser = 3, UpdateUser = 4, FindUser = 5, MainMenue = 6
};

enum enMainMenuePermissions
{
	All = -1, pListClients = 1, pAddNewClient = 2, pDeletClient = 4, pUpdateClient = 8,
	pFindClient = 16, pTranactions = 32, pManageUser = 64
};

struct sClient
{
	string AccountNumber;
	string PinCode;
	string Name;
	string Phone;
	double AccountBalance;
	bool MarkForDelet = false;
	
};
struct stUser
{
	string UserName;
	string Password;
	int Permissions;
	bool MarkForDelete = false;
};

stUser CurrentUser;



bool CheckAccessPermissions(enMainMenuePermissions Permossions)
{
	if (CurrentUser.Permissions == enMainMenuePermissions::All)
		return true;
	if ((Permossions & CurrentUser.Permissions) == Permossions)
		return true;
	else 
		return false;
}
void ShowAccessDeniedMessage()
{
	cout << "\n------------------------------------\n";
	cout << "Access Denied, \nYou dont Have Permission To Dothis, \nPlease Conact Your Admin.";
	cout << "\n------------------------------------\n";
}


sClient ChangeClientRecord(string AccountNumber)
{
	sClient Client;

	Client.AccountNumber = AccountNumber;
	cout << "Enter PinCode? ";
	getline(cin >> ws, Client.PinCode);
	cout << "Enter Name? ";
	getline(cin, Client.Name);
	cout << "Enter Phone? ";
	getline(cin, Client.Phone);
	cout << "Enter AccountBalance? ";
	cin >> Client.AccountBalance;
	return Client;
}



sClient ConvertLineToRecord(string Line, string Seperator = "#//#")
{
	sClient Client;
	vector<string>vString;
	vString = MyLip::SplitStringToVector(Line, Seperator);
	Client.AccountNumber = vString[0];
	Client.PinCode = vString[1];
	Client.Name = vString[2];
	Client.Phone = vString[3];
	Client.AccountBalance = stod(vString[4]);
	return Client;
}
stUser ConvertUserLineToRecord(string Line, string Seperator = "#//#")
{
	stUser User;
	vector<string> vString = MyLip::SplitStringToVector(Line, Seperator);
	User.UserName = vString[0];
	User.Password = vString[1];
	User.Permissions = stoi(vString[2]);

	return User;
}


string ConvertRecordToLine(sClient Client, string Seperator = "#//#")
{
	string stClientRecord = "";
	stClientRecord += Client.AccountNumber + Seperator;
	stClientRecord += Client.PinCode + Seperator;
	stClientRecord += Client.Name + Seperator;
	stClientRecord += Client.Phone + Seperator;
	stClientRecord += to_string(Client.AccountBalance);
	return stClientRecord;
}

string ConvertUserRecordToLine(stUser User, string Seperator = "#//#")
{
	string stUserRescord = "";
	stUserRescord += User.UserName + Seperator;
	stUserRescord += User.Password + Seperator;
	stUserRescord += to_string(User.Permissions);
	return stUserRescord;
}

vector<sClient> LoadDataFromFile(string FileName)
{
	vector<sClient> vClient;
	fstream MyFile;
	MyFile.open(FileName, ios::in);
	if (MyFile.is_open())
	{
		string line;
		sClient Client;
		while (getline(MyFile, line))
		{
			Client = ConvertLineToRecord(line);
			vClient.push_back(Client);
		}
		MyFile.close();
	}
	return vClient;
}
vector<stUser> LoadUserDataFromFile(string FileName)
{
	vector<stUser> vUser;
	fstream MyFile;
	MyFile.open(FileName, ios::in);
	if (MyFile.is_open())
	{
		string Line;
		stUser User;
		while (getline(MyFile, Line))
		{
			User = ConvertUserLineToRecord(Line);
			vUser.push_back(User);
		}
		MyFile.close();
	}
	return vUser;
}



bool ClientExistsByAccountNumber(string AccountNumber, string FileName)
{
	vector <sClient> vClients;
	fstream MyFile;
	MyFile.open(FileName, ios::in);
	{
		string Line;
		sClient Client;
		while (getline(MyFile, Line))
		{
			Client = ConvertLineToRecord(Line);
			if (Client.AccountNumber == AccountNumber)
			{
				MyFile.close();
				return true;
			}
			vClients.push_back(Client);
		}
		MyFile.close();
	}
	return false;
}
sClient ReadNewClient()
{
	sClient Client;

	cout << "------------------------------\n";
	cout << "    Add New Client Screen.\n";
	cout << "______________________________\n";
	cout << "Adding New Client\n\n";

	cout << "Pleas Enter Account Number? ";
	getline(cin >> ws, Client.AccountNumber);

	while (ClientExistsByAccountNumber(Client.AccountNumber, FileNameClient))
	{
		cout << "\nClient with [" << Client.AccountNumber << "] already exists, Enter another Account Number ? ";
		getline(cin >> ws, Client.AccountNumber);
	}

	cout << "Enter PinCode? ";
	getline(cin, Client.PinCode);
	cout << "Enter Name? ";
	getline(cin, Client.Name);
	cout << "Enter Phone? ";
	getline(cin, Client.Phone);
	cout << "Enter AccountBalance? ";
	cin >> Client.AccountBalance;
	return Client;
}

void AddDataLineToFile(string FileName, string DataLine)
{
	fstream MyFile;
	MyFile.open(FileName, ios::out | ios::app);
	if (MyFile.is_open())
	{
		MyFile << DataLine << endl;
		MyFile.close();
	}
}
void AddNewClients()
{
	sClient Client = ReadNewClient();
	AddDataLineToFile(FileNameClient, ConvertRecordToLine(Client));
}
void AddAllClient()
{
	if (!CheckAccessPermissions(enMainMenuePermissions::pAddNewClient))
	{
		ShowAccessDeniedMessage();
		GoBackToMainMenue();
		return;
	}


	char AddMore;

	do
	{
		system("cls");
		AddNewClients();
		cout << "\n\nClient Added Successfully, Do you Want To Add More Client? ";
		cin >> AddMore;
	} while (AddMore == 'Y' || AddMore == 'y');
}

bool UserExistsByUserName(string UserName, string FileName)
{
	vector<stUser> vUser;
	fstream MyFile;
	MyFile.open(FileName, ios::in);
	if (MyFile.is_open())
	{
		string line;
		stUser User;
		while (getline(MyFile, line))
		{
			User = ConvertUserLineToRecord(line);
			if (User.UserName == UserName)
			{
				MyFile.close();
				return true;
			}
			vUser.push_back(User);
		}
		MyFile.clear();
	}
	return false;
	
}


int ReadPermissionsToSet()
{
	int Permissions = 0;
	char Answer = 'n';
	cout << "\n\n Do You Want To Give Full Access? y/n? ";
	cin >> Answer;
	if (toupper(Answer) == 'Y')
	{
		return -1;
	}
	cout << "\n\nDo You Want To Give Access to :";

	cout << "\n\nShw Client List? y/n? ";
	cin >> Answer;
	if (toupper(Answer) == 'Y')
	{
		Permissions += enMainMenuePermissions::pListClients;
	}

	cout << "\n\nAdd New Client? y/n? ";
	cin >> Answer;
	if (toupper(Answer) == 'Y')
	{
		Permissions += enMainMenuePermissions::pAddNewClient;
	}
	cout << "\n\nDelete Client? y/n? ";
	cin >> Answer;
	if (toupper(Answer) == 'Y')
	{
		Permissions += enMainMenuePermissions::pDeletClient;
	}
	cout << "\n\nUpdate Client? y/n? ";
	cin >> Answer;
	if (toupper(Answer) == 'Y')
	{
		Permissions += enMainMenuePermissions::pUpdateClient;
	}
	cout << "\n\nFind Client? y/n? ";
	cin >> Answer;
	if (toupper(Answer) == 'Y')
	{
		Permissions += enMainMenuePermissions::pFindClient;
	}
	cout << "\n\nTransactions? y/n? ";
	cin >> Answer;
	if (toupper(Answer) == 'Y')
	{
		Permissions += enMainMenuePermissions::pTranactions;
	}
	cout << "\n\nMange User? y/n? ";
	cin >> Answer;
	if (toupper(Answer) == 'Y')
	{
		Permissions += enMainMenuePermissions::pManageUser;
	}
	return Permissions;
}

stUser ChangeUserRecord(string UserName)
{
	stUser User;
	User.UserName = UserName;
	cout << "\n\nEnter Password? ";
	getline(cin >> ws, User.Password);
	User.Permissions = ReadPermissionsToSet();
	return User;
}
stUser ReadNewUser()
{
	stUser User;
	cout << "Enter User Name? ";
	getline(cin >> ws, User.UserName);
	while (UserExistsByUserName(User.UserName, UserFileName))
	{
		cout << "\nUser With [" << User.UserName << "] Already Exist, Enter Another User Name: ";
		getline(cin >> ws, User.UserName);
	}
	cout << "Enter Password? ";
	getline(cin, User.Password);

	User.Permissions = ReadPermissionsToSet();
	return User;

}
void AddNewUser()
{
	stUser User = ReadNewUser();
	AddDataLineToFile(UserFileName, ConvertUserRecordToLine(User));
}
void AddAllUser()
{
	char AddMore = 'n';
	cout << "------------------------------\n";
	cout << "    Add New User Screen.\n";
	cout << "______________________________\n";
	
	do
	{
		cout << "Adding New User\n\n";
		AddNewUser();
		cout << "\n\nUser Added Successfully, Do you Want To Add More Users? y/n? ";
		cin >> AddMore;
	} while (toupper(AddMore) == 'Y');
}




void PrintClientToRecord(sClient Client)
{
	cout << "| " << setw(15) << left << Client.AccountNumber;
	cout << "| " << setw(10) << left << Client.PinCode;
	cout << "| " << setw(40) << left << Client.Name;
	cout << "| " << setw(12) << left << Client.Phone;
	cout << "| " << setw(12) << left << Client.AccountBalance;
}
void PrintAllClientToFile(vector<sClient> vClient)
{

	if (!CheckAccessPermissions(enMainMenuePermissions::pListClients))
	{
		ShowAccessDeniedMessage();
		GoBackToMainMenue();
		return;
	}


	cout << "\t\t\t\tClient(" << vClient.size() << ") Client(s).\n\n";
	cout << "________________________________________________________________________________________________\n\n";
	cout << "| " << left << setw(15) << "Account Number";
	cout << "| " << left << setw(10) << "Pin Code";
	cout << "| " << left << setw(40) << "Client Name";
	cout << "| " << left << setw(12) << "Phone";
	cout << "| " << left << setw(12) << "Balance";
	cout << "\n\n________________________________________________________________________________________________\n\n";
	for (sClient Client : vClient)
	{
		PrintClientToRecord(Client);
		cout << endl;
	}
	cout << "\n________________________________________________________________________________________________\n";
}

void PrintClients(sClient Client)
{
	cout << "\n\nThe Follwing Are The Client Deteails:\n";
	cout << "------------------------------------------------\n";
	cout << "Acoount Number : " << Client.AccountNumber << endl;
	cout << "Pin Code       : " << Client.PinCode << endl;
	cout << "Name           : " << Client.Name << endl;
	cout << "Phone          : " << Client.Phone << endl;
	cout << "Account Balance: " << Client.AccountBalance;
	cout << "\n------------------------------------------------\n";
}
void PrentUserCard(stUser User)
{
	cout << "\n\nThe Follwing Are The User Deteails:\n";
	cout << "------------------------------------------------\n";
	cout << "User Name  : " << User.UserName << endl;
	cout << "Password   : " << User.Password << endl;
	cout << "Permissions: " << User.Permissions << endl;
	cout << "\n------------------------------------------------\n";
}


string ReadAccountNumber()
{
	string Num;
	cout << "Pleas Enter Account Number? ";
	cin >> Num;
	return Num;
}
string ReadUserName()
{
	string Num;
	cout << "Pleas Enter User Name? ";
	cin >> Num;
	return Num;
}
bool FindClientByAccountNumber(string Accountnumber, vector<sClient> vClient, sClient& Client)
{

	for (sClient C : vClient)
	{
		if (C.AccountNumber == Accountnumber)
		{
			Client = C;
			return true;
		}
	}
	return false;
}
bool FindUserByUserName(string UserName, vector<stUser> vUser, stUser& User)
{
	for (stUser U : vUser)
	{
		if (U.UserName == UserName)
		{
			User = U;
			return true;
		}
	}
	return false;
}


void PrintFindAccountNumber(vector<sClient> vClient)
{

	if (!CheckAccessPermissions(enMainMenuePermissions::pFindClient))
	{
		ShowAccessDeniedMessage();
		GoBackToMainMenue();
		return;
	}


	sClient Client;
	string AccountNumber = ReadAccountNumber();
	if (FindClientByAccountNumber(AccountNumber, vClient, Client))
	{
		PrintClients(Client);
	}
	else
	{
		cout << "\nClient With Account Number (" << AccountNumber << ") IS Not Found.\n";
	}

}
void PrintFindUserByUserName(vector<stUser> vUser)
{
	stUser User;
	string UserName = ReadUserName();
	if (FindUserByUserName(UserName, vUser, User))
	{
		PrentUserCard(User);
	}
	else
	{
		cout << "\nUser With User Name (" << UserName << ") IS Not Found.\n";
	}
}


bool MarkClientForDeletByAccountNumber(string AccountNumber, vector<sClient>& vClient)
{
	for (sClient& C : vClient)
	{
		if (C.AccountNumber == AccountNumber)
		{
			C.MarkForDelet = true;
			return true;
		}
	}
	return false;
}
bool MarkUserForDeletByUserName(string UserName, vector<stUser>& vUser)
{
	for (stUser& U : vUser)
	{
		if (U.UserName == UserName)
		{
			U.MarkForDelete = true;
			return true;
		}
	}
	return false;
}

vector<sClient> SaveClientDateToFile(string FileName, vector<sClient> vCleint)
{
	fstream Myfile;
	string DataLine;
	Myfile.open(FileName, ios::out);
	if (Myfile.is_open())
	{
		for (sClient C : vCleint)
		{
			if (C.MarkForDelet == false)
			{
				DataLine = ConvertRecordToLine(C);
				Myfile << DataLine << endl;
			}

		}
		Myfile.close();
	}
	return vCleint;
}
vector<stUser> SaveUserDateToFile(string FileName, vector<stUser> vUser)
{
	fstream Myfile;
	string DataLine;
	Myfile.open(FileName, ios::out);
	if (Myfile.is_open())
	{
		for (stUser U : vUser)
		{
			if (U.MarkForDelete == false)
			{
				DataLine = ConvertUserRecordToLine(U);
				Myfile << DataLine << endl;
			}

		}
		Myfile.close();
	}
	return vUser;
}


bool DeletClientByAccountNumber(vector<sClient> vClient)
{

	sClient Client;
	char Answer = 'n';
	string AccountNumber = ReadAccountNumber();
	if (FindClientByAccountNumber(AccountNumber, vClient, Client))
	{
		
		PrintClients(Client);
		cout << "\n\nAre You Sure You Want Delet This Clients? y?n";
		cin >> Answer;
		if (toupper(Answer) == 'Y')
		{
			MarkClientForDeletByAccountNumber(AccountNumber, vClient);
			SaveClientDateToFile(FileNameClient, vClient);

			vClient = LoadDataFromFile(FileNameClient);
			cout << "\n\nClient Deleted Successfully.";
			return true;
		}
	}
	else
	{
		cout << "\nClient with Account Number (" << AccountNumber << ") is Not Found!";
		return false;
	}
}

void ShowDeletClientScreen()
{
	if (!CheckAccessPermissions(enMainMenuePermissions::pDeletClient))
	{
		ShowAccessDeniedMessage();
		GoBackToMainMenue();
		return;
	}

	cout << "------------------------------\n";
	cout << "    Delet Client Screen\n";
	cout << "______________________________\n\n";

	vector<sClient> vClient = LoadDataFromFile(FileNameClient);
	DeletClientByAccountNumber(vClient);

}


bool DeletUserByUserName(vector<stUser> vUser)
{
	stUser User;
	char Answer = 'n';
	
	string UserName = ReadUserName();
	if (FindUserByUserName(UserName, vUser, User))
	{
		PrentUserCard(User);
		cout << "\n\nAre You Sure You Want Delet This User? y?n";
		cin >> Answer;
		if (toupper(Answer) == 'Y')
		{
			MarkUserForDeletByUserName(UserName, vUser);
			SaveUserDateToFile(UserFileName, vUser);
			vUser = LoadUserDataFromFile(UserFileName);
			cout << "\n\nUser Deleted Successfully.";
			return true;
		}

	}
	else
	{
		cout << "\nUser with User Name (" << UserName << ") is Not Found!";
		return false;
	}
}

bool UpdateClientByAccountNumber(vector<sClient>& vClient)
{

	


	sClient Client;
	char Answer = 'n';
	string AccountNumber = ReadAccountNumber();
	if (FindClientByAccountNumber(AccountNumber, vClient, Client))
	{
		
		PrintClients(Client);
		cout << "\n\nAre You Sure You Want Update This Clients? y?n";
		cin >> Answer;
		if (toupper(Answer) == 'Y')
		{
			for (sClient& C : vClient)
			{
				if (C.AccountNumber == AccountNumber)
				{
					C = ChangeClientRecord(AccountNumber);
					break;
				}
			}

			SaveClientDateToFile(FileNameClient, vClient);

			cout << "\n\nClient Ubdated Successfully.";
			return true;
		}
	}
	else
	{
		cout << "\nClient with Account Number (" << AccountNumber << ") is Not Found!";
		return false;
	}
}
void ShowUpdateClientScreen()
{
	if (!CheckAccessPermissions(enMainMenuePermissions::pUpdateClient))
	{
		ShowAccessDeniedMessage();
		GoBackToMainMenue();
		return;
	}
	
	cout << "-----------------------------------\n";
	cout << "    Update Client Info Screen\n";
	cout << "___________________________________\n\n";
	vector<sClient> vClient = LoadDataFromFile(FileNameClient);
	UpdateClientByAccountNumber(vClient);

}



bool UpdateUserByUserName(vector<stUser>& vUser)
{
	stUser User;
	char Answer = 'n';
	cout << "-----------------------------------\n";
	cout << "    Update User Info Screen\n";
	cout << "___________________________________\n\n";

	string UserName = ReadUserName();
	if (FindUserByUserName(UserName, vUser, User))
	{
		PrentUserCard(User);
		cout << "\n\nAre You Sure You Want Update This User? y?n";
		cin >> Answer;
		if (toupper(Answer) == 'Y')
		{
			for (stUser U : vUser)
			{
				if (U.UserName == UserName)
				{
					U = ChangeUserRecord(UserName);
					break;
				}
			}

			SaveUserDateToFile(UserFileName, vUser);

			cout << "\n\nUser Ubdated Successfully.";
			return true;
		}
	}
	else
	{
		cout << "\nUser with Account Number (" << UserName << ")is Not Found!";
		return false;
	}

}


void GoBackToMainMenue()
{
	cout << "\nPress Any Key To Go Back To Main Menue... ";
	system("pause>0");
	system("cls");
	ShowMainMenue();

}
short ReadNumber()
{

	short Num = 0;
	cin >> Num;
	return Num;
}


void GoToBackTransactionMenue()
{
	cout << "\nPress Any Key To Go Back To Transaction Menue... ";
	system("pause>0");
	system("cls");
	ShowTransactionMenueScreen();
}

void GoBackToMangeUserMenue()
{
	cout << "\n\n\nPress Any Key To Go Back To Mange User Menue... ";
	system("pause>0");
	system("cls");
	ShowMangeUserScreen();
}

bool DepositeBalanceToClientByAccount(string AccountNumber, double Amount, vector<sClient> vClient)
{
	char Answer = 'n';
	cout << "\n\n Are You Sure Wou Want Perform This Transaction? y/n? ";
	cin >> Answer;

	if (toupper(Answer) == 'Y')
	{
		for (sClient& C : vClient)
		{
			if (C.AccountNumber == AccountNumber)
			{
				C.AccountBalance += Amount;
				SaveClientDateToFile(FileNameClient, vClient);
				cout << "\n\nDone Successfully. New Balance is: " << C.AccountBalance;
				return true;
			}
		}
	}
	return false;
}
void ShowDepositScreen()
{
	cout << "------------------------------\n";
	cout << "      Deposit Screen\n";
	cout << "______________________________\n\n";
	vector<sClient> vClient = LoadDataFromFile(FileNameClient);
	string AccountNumber = ReadAccountNumber();
	sClient Client;


	if (!FindClientByAccountNumber(AccountNumber, vClient, Client))
	{
		cout << "Client With [" << AccountNumber << "] Does Not Exist.\n\n";
		AccountNumber = ReadAccountNumber();
	}
	PrintClients(Client);

	double Amount = 0;
	cout << "Pleas Enter Depost Amount? ";
	cin >> Amount;

	DepositeBalanceToClientByAccount(AccountNumber, Amount, vClient);

}

void ShowWithdrawScreen()
{
	cout << "------------------------------\n";
	cout << "      Withdraw Screen\n";
	cout << "______________________________\n\n";
	vector<sClient> vClient = LoadDataFromFile(FileNameClient);
	string AccountNumber = ReadAccountNumber();
	sClient Client;

	if (!FindClientByAccountNumber(AccountNumber, vClient, Client))
	{
		cout << "Client With [" << AccountNumber << "] Does Not Exist.\n\n";
		AccountNumber = ReadAccountNumber();
	}
	PrintClients(Client);

	double Amount = 0;
	cout << "Pleas Enter Depost Amount? ";
	cin >> Amount;
	while (Amount > Client.AccountBalance)
	{
		cout << "\n\nAmount Exceeds The Balance, You Can Withdraw up to: " << Client.AccountBalance;
		cout << "\nPleas Enter Depost Amount? ";
		cin >> Amount;
	}
	DepositeBalanceToClientByAccount(AccountNumber, Amount * -1, vClient);

}

void PrintClientToTotalBalance(sClient Client)
{
	cout << "| " << setw(15) << left << Client.AccountNumber;
	cout << "| " << setw(40) << left << Client.Name;
	cout << "| " << setw(12) << left << Client.AccountBalance;
}

void ShowTotalBlance()
{
	vector<sClient> vClient = LoadDataFromFile(FileNameClient);
	double SumTotalBalance = 0;
	cout << "\t\t\t\tClient(" << vClient.size() << ") Client(s).\n\n";
	cout << "________________________________________________________________________________________________\n\n";
	cout << "| " << left << setw(15) << "Account Number";
	cout << "| " << left << setw(40) << "Client Name";
	cout << "| " << left << setw(12) << "Balance";
	cout << "\n\n________________________________________________________________________________________________\n\n";
	for (sClient Client : vClient)
	{

		PrintClientToTotalBalance(Client);
		cout << endl;
		SumTotalBalance += Client.AccountBalance;
	}
	cout << "________________________________________________________________________________________________\n\n";
	cout << "\t\t\tTotal Balance = " << SumTotalBalance << endl;
}
void CheckTransactionsData(enCheckTransactions CheckTransactions)
{
	vector<sClient> vClient = LoadDataFromFile(FileNameClient);
	switch (CheckTransactions)
	{
	case enCheckTransactions::Deposit:
	{
		system("cls");
		ShowDepositScreen();
		GoToBackTransactionMenue();
		break;
	}
	case enCheckTransactions::Withdraw:
	{
		system("cls");
		ShowWithdrawScreen();
		GoToBackTransactionMenue();
		break;
	}
	case enCheckTransactions::TotalBalance:
	{
		system("cls");
		ShowTotalBlance();
		GoToBackTransactionMenue();
		break;
	}
	case enCheckTransactions::ManMenue:
	{
		system("cls");
		ShowMainMenue();
	}
	}
}
void ShowTransactionMenueScreen()
{
	if (!CheckAccessPermissions(enMainMenuePermissions::pTranactions))
	{
		ShowAccessDeniedMessage();
		GoBackToMainMenue();
		return;
	}



	cout << "==================================================\n";
	cout << "\t\tTransactions Menue Screen\n";
	cout << "==================================================\n";
	cout << "\t[1] Deposit.\n";
	cout << "\t[2] Withdraw.\n";
	cout << "\t[3] Total Balance.\n";
	cout << "\t[4] Main Menue.\n";
	cout << "==================================================\n";
	cout << "Choose What Do You Want to do? [1 to 4]? ";
	CheckTransactionsData(enCheckTransactions(ReadNumber()));

}




void PrintUser(stUser User)
{
	cout << "| " << setw(15) << left << User.UserName;
	cout << "| " << setw(15) << left << User.Password;
	cout << "| " << setw(15) << left << User.Permissions;
}
void ShowListUser()
{
	vector<stUser> vUser = LoadUserDataFromFile(UserFileName);
	cout << "\t\t\tUser List (" << vUser.size() << ") User(s).\n";
	cout << "________________________________________________________________________________________________\n\n";
	cout << "| " << setw(15) << left << "User Name";
	cout << "| " << setw(15) << left << "Password";
	cout << "| " << setw(15) << left << "Perimssions";
	cout << "\n________________________________________________________________________________________________\n\n";

	if (vUser.size() == 0)
		cout << "\t\t\tNo User Available in The System!";

	for (stUser User : vUser)
	{
		PrintUser(User);
		cout << endl;
	}
}


void CheckManageUser(enCheckManageUser ManageUser)
{
	vector<stUser> vUser = LoadUserDataFromFile(UserFileName);
	switch (ManageUser)
	{
	case enCheckManageUser::listUser:
	{
		system("cls");
		ShowListUser();
		GoBackToMangeUserMenue();
		break;
	}
	case enCheckManageUser::AddUser:
	{
		system("cls");
		AddAllUser();
		GoBackToMangeUserMenue();
		break;
	}
	case enCheckManageUser::DeletUser:
	{
		system("cls");
		DeletUserByUserName(vUser);
		GoBackToMangeUserMenue();
		break;
	}
	case enCheckClient::UpdateClient:
	{
		system("cls");
		UpdateUserByUserName(vUser);
		GoBackToMangeUserMenue();
		break;
	}
	case enCheckManageUser::FindUser:
	{
		system("cls");
		PrintFindUserByUserName(vUser);
		GoBackToMangeUserMenue();
		break;
	}
	case enCheckManageUser::MainMenue:
	{
		system("cls");
		ShowMainMenue();
	}

	}
}
void ShowMangeUserScreen()
{

	if (!CheckAccessPermissions(enMainMenuePermissions::pManageUser))
	{
		ShowAccessDeniedMessage();
		GoBackToMainMenue();
		return;
	}


	cout << "==================================================\n";
	cout << "\t   Mange Users Munue Screen\n";
	cout << "==================================================\n";
	cout << "\t[1] List Users.\n";
	cout << "\t[2] Add New User.\n";
	cout << "\t[3] Delet User.\n";
	cout << "\t[4] Update User.\n";
	cout << "\t[5] Find User.\n";
	cout << "\t[6] Main Menue.\n";
	cout << "==================================================\n";
	cout << "Choose What Do You Want To Do? [1 to 6]? ";
	CheckManageUser(enCheckManageUser(ReadNumber()));
}

void CheckClientData(enCheckClient CheckClient)
{
	vector<sClient> vClient = LoadDataFromFile(FileNameClient);

	switch (CheckClient)
	{
	case enCheckClient::ShowClient:
	{
		system("cls");
		PrintAllClientToFile(vClient);
		GoBackToMainMenue();
		break;
	}
	case enCheckClient::AddClient:
	{
		system("cls");
		AddAllClient();
		GoBackToMainMenue();
		break;
	}
	case enCheckClient::DeletClient:
	{
		system("cls");
		ShowDeletClientScreen();
		GoBackToMainMenue();
		break;
	}
	case enCheckClient::UpdateClient:
	{
		system("cls");
		ShowUpdateClientScreen();
		GoBackToMainMenue();
		break;
	}
	case enCheckClient::FindClient:
	{
		system("cls");
		PrintFindAccountNumber(vClient);
		GoBackToMainMenue();
		break;
	}
	case enCheckClient::Transactions:
	{
		system("cls");
		ShowTransactionMenueScreen();
		break;
	}
	case enCheckClient::ManageUser:
	{
		system("cls");
		ShowMangeUserScreen();
		break;
	}
	case enCheckClient::Logout:
	{
		system("cls");
		Login();
		break;
	}

	}
}

void ShowMainMenue()
{
	system("cls");
	cout << "==================================================\n";
	cout << "\t\tMian Menue Screen\n";
	cout << "==================================================\n";
	cout << "\t[1] Show Client List.\n";
	cout << "\t[2] Add New Client.\n";
	cout << "\t[3] Delet Client.\n";
	cout << "\t[4] Update Client Info.\n";
	cout << "\t[5] Find Client.\n";
	cout << "\t[6] Transactions.\n";
	cout << "\t[7] Manage Users.\n";
	cout << "\t[8] Logout.\n";
	cout << "==================================================\n";
	cout << "Choos What Do you Want To Do? [1 to 8]? ";
	CheckClientData(enCheckClient(ReadNumber()));


}
bool FindUserByUserNameAndPassword(string UserName, string Password, stUser& User)
{
	vector<stUser> vUser = LoadUserDataFromFile(UserFileName);

	for (stUser U : vUser)
	{
		if (U.UserName == UserName && U.Password == Password)
		{
			User = U;
			return true;
		}
	}
	return false;
}

bool LoadUserInfo(string UserName, string Password)
{
	if (FindUserByUserNameAndPassword(UserName, Password, CurrentUser))
		return true;

	return false;
}

void Login()
{
	bool LoginFild = false;

	string UserName, Password;
	do
	{
		system("cls");

		cout << "------------------------------\n";
		cout << "      Login Screen\n";
		cout << "______________________________\n\n";

		if (LoginFild)
		{
			cout << "Invlaid UserName/Password!\n";
		}
		cout << "Enter UsserName? ";
		cin >> UserName;
		cout << "Enter Password? ";
		cin >> Password;

		LoginFild = !LoadUserInfo(UserName, Password);

	} while (LoginFild);
	ShowMainMenue();

}

int main()
{
	Login();

	system("pause>0");
}
