#pragma once
#include <iostream>
#include <string>
#include <fstream>

using namespace std;

namespace MyLip
{
	int ReadNumber()
	{
		int Number = 0;
		cout << "Pleas Enter The Number : ";
		cin >> Number;

		while (cin.fail())
		{
			cin.clear();
			cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			cout << "Invalid Number, Enter A Valid One:" << endl;
			cin >> Number;
		}
		return Number;
	}

	int RandomNumber(int From, int To)
	{
		int randNum = rand() % (To - From + 1) + From;
		return randNum;
	}


	void WriteMode(string FileName)
	{
		fstream MyFile;
		MyFile.open(FileName, ios::out);
		if (MyFile.is_open())
		{
			MyFile << "Hi, This Is My First Line.\n";
			MyFile << "Hi,This Is My Socend Line.\n";
			MyFile << "Hi, This Is My Third Line.\n";
		}
		MyFile.close();
	}
	void AppendMode(string FileName)
	{
		fstream MyFile;

		MyFile.open(FileName, ios::out | ios::app);
		if (MyFile.is_open())
		{
			MyFile << "My Name Is Ahmed Hatem.\n";
			MyFile << "I am 22 Years Old.\n";
		}
		MyFile.close();
	}
	void PrintFileContent(string FileName)
	{
		fstream MyFile;
		MyFile.open(FileName, ios::in);
		if (MyFile.is_open())
		{
			string line;
			while (getline(MyFile, line))
			{
				cout << line << endl;
			}
			MyFile.close();
		}
	}
	void LoadDataFromVectorToFile(string FileName, vector <string>& vFileContenet)
	{
		fstream MyFile;
		MyFile.open(FileName, ios::in);
		if (MyFile.is_open())
		{
			string line;
			while (getline(MyFile, line))
			{
				vFileContenet.push_back(line);
			}
			MyFile.close();
		}
	}
	void SaveVectorToFile(string FileName, vector <string> vFileContenet)
	{
		fstream MyFile;
		MyFile.open(FileName, ios::out);
		if (MyFile.is_open())
		{
			for (string& line : vFileContenet)
			{
				if (line != "")
				{
					MyFile << line << endl;
				}
			}
			MyFile.close();

		}
	}
	void DeleteDataFromFile(string FileName, string Record)
	{
		vector <string> vFileContent;
		MyLip::LoadDataFromVectorToFile(FileName, vFileContent);
		for (string& line : vFileContent)
		{
			if (line == Record)
			{
				line = "";
			}
		}
		MyLip::SaveVectorToFile(FileName, vFileContent);
	}
	void UpdateRecordinFile(string FileName, string Record, string ToUpdate)
	{
		vector <string> vFileContent;
		MyLip::LoadDataFromVectorToFile(FileName, vFileContent);
		for (string& line : vFileContent)
		{
			if (line == Record)
			{
				line = ToUpdate;
			}
		}
		MyLip::SaveVectorToFile(FileName, vFileContent);
	}

	string ReadString()
	{
		string S1;
		cout << "Pleas Enter String:\n";
		getline(cin, S1);
		return S1;
	}

	string UpperString(string S1)
	{
		for (int i = 0; i < S1.length(); i++)
		{
			S1[i] = toupper(S1[i]);
		}
		return S1;
	}
	string LowerString(string S1)
	{
		for (int i = 0; i < S1.length(); i++)
		{
			S1[i] = tolower(S1[i]);
		}
		return S1;
	}
	char InvertLetterCase(char Char1)
	{
		return isupper(Char1) ? tolower(Char1) : toupper(Char1);
	}
	string InvertAllStringLitterCase(string S1)
	{
		for (int i = 0; i < S1.length(); i++)
		{
			S1[i] = isupper(S1[i]) ? tolower(S1[i]) : toupper(S1[i]);
		}
		return S1;
	}
	void PrintEachWordInString(string S1)
	{
		cout << "\nYour String Words are:\n\n";
		string delim = " ";
		short pos = 0;
		string sWord;
		while ((pos = S1.find(delim)) != std::string::npos)
		{
			sWord = S1.substr(0, pos);
			if (sWord != "")
			{
				cout << sWord << endl;
			}
			S1.erase(0, pos + delim.length());
		}
		if (S1 != "")
		{
			cout << S1 << endl;
		}
	}
	vector<string> SplitStringToVector(string S1, string delim)
	{
		vector<string> vString;
		string sWord;
		short pos = 0;

		while ((pos = S1.find(delim)) != std::string::npos)
		{
			sWord = S1.substr(0, pos);
			if (sWord != "")
			{
				vString.push_back(sWord);
			}
			S1.erase(0, pos + delim.length());
		}
		if (S1 != "")
		{
			vString.push_back(S1);
		}
		return vString;
	}


	string TrimeLift(string S1)
	{

		for (int i = 0; i < S1.length(); i++)
		{
			if (S1[i] != ' ')
				return S1.substr(i, S1.length() - 1);
		}
		return"";
	}
	string trimRight(string S1)
	{
		for (int i = S1.length() - 1; i >= 0; i--)
		{
			if (S1[i] != ' ')
				return S1.substr(0, i + 1);
		}
		return "";
	}
	string Trim(string S1)
	{
		return TrimeLift(trimRight(S1));
	}




}