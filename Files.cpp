#include "header.h"

double File_size(string s)
{
    CMD="for %I in ("+PATH+"\\"+s+") do @echo %~zI >> $size.txt";
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

ostream& operator<<(ostream& os, File& f)
{
    cout<<"Name:\t\t";
    os<<f.name<<endl;
    cout<<"Extension:\t";
    os<<f.extension<<endl;
    cout<<"Size(in MB):\t";
    f.size=File_size(f.name);
    os<<f.size<<endl;
    cout<<"Path:\t\t"<<PATH;
    cout<<"\nDate-time:\t";
    os<<f.CDT<<endl;
    os<<endl;
    return os;
}

//-------------------------------------------------------

vector <File*> Hash_Table::Search(string s)
{
    int j,h,n,N;
    string New;
    vector <File*> S;
    h=hash(s);
    N = s.rfind(".");

    for(j=0;j<entry[h].size();j++)
    {
        if(N!=-1)
        {
            if(!strcmpi(s.c_str(),entry[h][j].name.c_str()))
                S.push_back(&entry[h][j]);
        }
        else
        {
            New = entry[h][j].name;
            n = New.rfind(".");
            New = New.substr(0,n);

            if(!strcmpi(s.c_str(),New.c_str()))
                S.push_back(&entry[h][j]);
        }
    }
    return S;
}

int Hash_Table::Create(string s)
{
    int n;
    n = s.rfind(".");
    if(n==-1)
        return 1;           //Full name not given!

    vector<File*> S=Search(s);
    if(S.size())
        return 2;           //Name already exists!

    if(!p[1])
        if(!U.Admin_access())
            return 3;           //Permission denied!

    File F;
    F.name = s;
    F.CDT = Date_Time();
    n = s.rfind(".");
    F.extension = s.substr(n);
    F.size=0;
    entry[hash(s)].push_back(F);
    cout<<F<<endl;


    CMD="copy /y NUL "+PATH+"\\"+s+" >NUL";
    system(CMD.c_str());
    return 0;
}

int Hash_Table::Rename(string s, string New)
{
    int i,n,h,nh;
    vector<File*> S;
    File F;
    h=hash(s);
    n = New.rfind(".");
    if(n==-1)
        return 1;           //Full Name not given!

    S=Search(New);
    if(S.size())
        return 2;           //File already exists!

    S=Search(s);
    if(!S.size())
        return 3;           //No such file found!

    if(!p[1])
        if(!U.Admin_access())
            return 4;       //Permission denied!

    for(i=0;i<S.size();i++)
    {
        if(strcmpi(S[i]->extension.c_str() , New.substr(n).c_str()))
            return 5;       //Extension can't be changed!

        F=entry[h][S[i] - &entry[h][0]];
        F.name=New;

        nh=hash(New);
        if(h!=nh)
        {
            entry[nh].push_back(F);
            entry[h].erase(entry[h].begin()+(S[i] - &entry[h][0]));
        }
        else
            entry[h][S[i] - &entry[h][0]]=F;
    }


    CMD="ren "+PATH+"\\"+s+" "+New;
    system(CMD.c_str());
    return 0;
}

int Hash_Table::Delete(string s)
{
    int i,h;
    i = s.rfind(".");
    if(i==-1)
        return 1;               //Full name not given!

    h=hash(s);
    vector<File*> S=Search(s);
    if(!S.size())
        return 2;               //No such file found!

    if(!p[1])
        if(!U.Admin_access())
            return 3;       //Permission denied!

    for(i=0;i<S.size();i++)
        entry[h].erase(entry[h].begin()+(S[i] - &entry[h][0]));


    CMD="del /f "+PATH+"\\"+s;
    system(CMD.c_str());
    return 0;
}

int Hash_Table::Open(string s)
{
    int n;
    n = s.rfind(".");
    if(n==-1)
        return 1;               //Full name not given!

    vector<File*> S=Search(s);
    if(!S.size())
        return 2;               //No such file found!


    CMD=PATH+"\\"+s;
    system(CMD.c_str());
    return 0;
}

void Hash_Table::Describe(string s)
{
    int i;
    vector<File*> S=Search(s);
    for(i=0;i<S.size();i++)
        cout<<*S[i]<<endl;
}
