#pragma once
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <conio.h>
#include <time.h>
#include <fstream>
#include <string>
#include <vector>
#define SIZE 30

using namespace std;

//-------------------------------------------------------

struct User
{
    int admin;
    string id;
    string pw;
    void SET_PRM();
    int Sign_up();
    int Sign_in();
    int Admin_access();
    int change();
    void Sign_out(string &);
    void operator=(const User&);
};

struct File
{
    double size;
    string name,extension,CDT;
};

struct Folder
{
    double size;
    string name,CDT;
};

//-------------------------------------------------------

struct Hash_Table
{
    vector <File> entry[SIZE];
    vector <Folder> Fentry[SIZE];

    int hash(string);
    int Create(string);
    int Delete(string);
    int Rename(string,string);
    int Open(string);
    void Describe(string);
    vector<File*> Search(string);

    int D_Create(string);
    int D_Delete(string);
    int D_Rename(string,string);
    int D_MoveUp(Hash_Table&);
    int D_MoveDown(Hash_Table&,string);
    int Shortcut(Hash_Table&);
    int Copy_Paste(Hash_Table&);
    void D_List();
    void Show();
    void D_Describe(string);
    Folder* D_Search(string);
};

//-------------------------------------------------------

extern User U;
extern vector <User> Log;
extern int U_Type,p[3];
extern string CMD,ROOT,PATH;

void EXEQ();
string Date_Time();
void Read_Data(Hash_Table&);
void Write_Data(Hash_Table&);
ostream& operator<<(ostream&, File&);
ostream& operator<<(ostream&, Folder&);
