#include "header.h"

double Folder_size(string s)
{
    CMD="powershell -command \"Set-Location -Path \""+PATH+"\\"+s+"\";";
    CMD=CMD+"$totalsize=[long]0;gci -File -r -fo -ea Silent|%{$totalsize+=$_.Length};";
    CMD=CMD+"$totalsize\" >> $size.txt";
    system(CMD.c_str());

    double n;
    fstream f;
    f.open("$size.txt",ios::in);
    f>>n;
    n/=1000000;
    f.close();
    s="$size.txt";
    remove(s.c_str());
    return n;
}

//-------------------------------------------------------

ostream& operator<<(ostream& os, Folder& f)
{
    os<<endl;
    cout<<"Name:\t\t";
    os<<f.name<<endl;
    cout<<"Size(in MB):\t";
    f.size=Folder_size(f.name);
    os<<f.size<<endl;
    cout<<"Path:\t\t"<<PATH<<endl;
    cout<<"Date-time:\t";
    os<<f.CDT;
    os<<endl;
    return os;
}

//-------------------------------------------------------

Folder* Hash_Table::D_Search(string s)
{
    int j,h=hash(s);
    Folder* S=NULL;

    for(j=0;j<Fentry[h].size();j++)
        if(!strcmpi(s.c_str(),Fentry[h][j].name.c_str()))
        {
            S=&Fentry[h][j];
            break;
        }
    return S;
}

int Hash_Table::D_Create(string s)
{
    Folder* S=NULL;
    S=D_Search(s);
    if(S)
        return 1;                   //Name Already exists!

    if(!p[1])
        if(!U.Admin_access())
            return 2;              //Permission denied!


    Folder F;
    F.size=0;
    F.name=s;
    F.CDT=Date_Time();
    Fentry[hash(s)].push_back(F);
    cout<<F<<endl;

    CMD="mkdir "+PATH+"\\"+s;
    system(CMD.c_str());
    return 0;
}

int Hash_Table::D_Rename(string s, string New)
{

    int h,nh;
    Folder F;
    Folder* S;
    h=hash(s);
    nh=hash(New);

    S=D_Search(s);
    if(!S)
        return 1;                   //No such Folder Found!

    if(!p[1])
        if(!U.Admin_access())
            return 2;              //Permission denied!


    F=Fentry[h][S - &Fentry[h][0]];
    F.name=New;

    if(h!=nh)
    {
        Fentry[nh].push_back(F);
        Fentry[h].erase(Fentry[h].begin()+(S - &Fentry[h][0]));
    }
    else
        Fentry[h][S - &Fentry[h][0]]=F;


    CMD="ren "+PATH+"\\"+s+" "+New;
    system(CMD.c_str());
    return 0;
}

int Hash_Table::D_Delete(string s)
{
    int h=hash(s);
    Folder* S=D_Search(s);
    if(!S)
        return 1;                   //No such Folder Found!

    if(!p[1])
        if(!U.Admin_access())
            return 2;              //Permission denied!

    Fentry[h].erase(Fentry[h].begin()+(S - &Fentry[h][0]));

    CMD="rmdir "+PATH+"\\"+s+" /s";
    system(CMD.c_str());
    return 0;
}

//-------------------------------------------------------

int Hash_Table::D_MoveUp(Hash_Table& T)
{
    if(!strcmp(PATH.c_str(),ROOT.c_str()))
        return 1;                   //This is Root Directory!

    Write_Data(T);
    system("cls");

    cout<<"\nOld Path:\t"<<PATH;
    int n = PATH.rfind("\\");
    PATH = PATH.substr(0,n);
    cout<<"\nNew Path:\t"<<PATH;

    Read_Data(T);
    return 0;
}

int Hash_Table::D_MoveDown(Hash_Table& T,string s)
{
    Folder* S=D_Search(s);
    if(!S)
        return 1;                   //No such Folder Found!

    Write_Data(T);
    system("cls");

    cout<<"\nOld Path:\t"<<PATH;
    PATH = PATH+"\\"+s;
    cout<<"\nNew Path:\t"<<PATH;

    Read_Data(T);
    return 0;
}

//-------------------------------------------------------

void Hash_Table::D_List()
{
    int i,j;
    cout<<"Path:\t"<<PATH<<endl;

    cout<<"\n\nContained Files:"<<endl;
    for(i=0;i<SIZE;i++)
        for(j=0;j<entry[i].size();j++)
            cout<<entry[i][j].name<<"\t";

    cout<<"\n\nContained Folders:"<<endl;
    for(i=0;i<SIZE;i++)
        for(j=0;j<Fentry[i].size();j++)
            cout<<Fentry[i][j].name<<"\t";
}

int Hash_Table::Shortcut(Hash_Table& T)
{
    int n;
    string temp,src,New;
    cout<<"Enter name of the source folder:\n";
    cin.ignore();
    getline(cin,temp);

    Folder* S=D_Search(temp);
    if(!S)
        return 1;                //No such Folder Exists!

    if(!p[1])
        if(!U.Admin_access())
            return 2;           //Permission denied!


    cout<<"Enter path of the destination folder:\n";
    getline(cin,New);

    Write_Data(T);
    src=PATH+"\\"+temp;
    New=ROOT+"\\"+New;
    temp=PATH;
    n=New.rfind("\\");
    PATH=New.substr(0,n);
    Read_Data(T);

    Folder F;
    F.size=0;
    F.name=New.substr(n+1);
    F.CDT=Date_Time();
    Fentry[hash(F.name)].push_back(F);

    Write_Data(T);
    PATH=temp;
    Read_Data(T);

    system("cls");
    CMD="mklink /J \""+New+"\" \""+src+"\"";
    system(CMD.c_str());
    cout<<endl<<F<<endl;
    return 0;
}

void Hash_Table::D_Describe(string s)
{
    Folder* S=D_Search(s);
    cout<<*S<<endl;
}
