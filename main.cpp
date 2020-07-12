#include "header.h"

//-------------------------------------------------------

User U;
vector <User> Log;
int p[3],U_Type=-1;
string CMD, ROOT="%USERPROFILE%\\Desktop\\FMS_Root", PATH=ROOT;

string Date_Time()
{
    struct tm *tmp;
    char MY_TIME[50];
    time_t t = time(0);

    tmp = localtime( &t );
    strftime(MY_TIME, sizeof(MY_TIME), "%A : %d %B, %Y - %I:%M%p", tmp);
    return MY_TIME;
}

//-------------------------------------------------------

int Hash_Table::hash(string str)
{
    int G=12,SEED=19,i,n,next,result,val=0;

    n = str.rfind(".");
    if(n!=-1)
        for(i=0;i<n;i++)
            val += str[i];

    else
        for(i=0;i<str.size();i++)
            val+=str[i];

    val = val % SIZE;
    SEED = SEED % SIZE;
    next = SEED ^ val;
    result = G;

    next = next<<7;
    next = next%SIZE;
    result = result^next;
    result = result+val;
    result = result%SIZE;

    return result;
}

int Hash_Table::Copy_Paste(Hash_Table& T)
{
    int n;
    string temp,src,New;
    cout<<"Enter name of the source file/folder:\n";
    cin.ignore();
    getline(cin,temp);

    Folder* fo=D_Search(temp);
    vector <File*> fi=Search(temp);
    if(!fo && !fi.size())
        return 1;        //No such File Exists!

    if(!p[1])
        if(!U.Admin_access())
            return 2;       //Permission denied!

    cout<<"Enter path of the destination file/folder:\n";
    getline(cin,New);

    Write_Data(T);
    src=PATH+"\\";
    New=ROOT+"\\"+New;
    temp=PATH;
    PATH=New;
    Read_Data(T);
    system("cls");
    if(fo)
    {
        Folder F;
        F.size=0;
        F.name=fo->name;
        F.CDT=Date_Time();
        Fentry[hash(F.name)].push_back(F);
        CMD="xcopy \""+src+F.name+"\" \""+New+"\\"+F.name+"\" /E /I";
        system(CMD.c_str());
        cout<<F<<endl;
    }
    if(fi.size())
    {
        File F;
        for(int i=0;i<fi.size();i++)
        {
            F.name = fi[i]->name;
            F.CDT = Date_Time();
            F.extension = fi[i]->extension;
            F.size=0;
            entry[hash(F.name)].push_back(F);

            CMD="copy \""+src+F.name+"\" \""+New+"\"";
            system(CMD.c_str());
            cout<<F<<endl;
        }
    }

    Write_Data(T);
    PATH=temp;
    Read_Data(T);
    system("cls");
    return 0;
}

void Hash_Table::Show()
{
    int i,j;
    cout<<"\nFiles' Hash Table:\n";
    for(i=0;i<SIZE;i++)
    {
        cout<<i<<"\t";
        for(j=0;j<entry[i].size();j++)
            cout<<entry[i][j].name<<" ---> ";
        cout<<"."<<endl;
    }
    cout<<"\n==============================================\n";
    cout<<"\nFolders' Hash Table:\n";
    for(i=0;i<SIZE;i++)
    {
        cout<<i<<"\t";
        for(j=0;j<Fentry[i].size();j++)
            cout<<Fentry[i][j].name<<" ---> ";
        cout<<"."<<endl;
    }
}

//-------------------------------------------------------

void Write_Data(Hash_Table& T)
{
    int i,j;
    fstream f;
    string src=PATH+"\\$data.txt";
    f.open(src.c_str(),ios::out|ios::trunc);

    for(i=0;i<SIZE;i++)
        for(j=0;j<T.entry[i].size();j++)
            f<<T.entry[i][j];
    f<<"=============================================\n";
    for(i=0;i<SIZE;i++)
        for(j=0;j<T.Fentry[i].size();j++)
            f<<T.Fentry[i][j];

    for(i=0;i<SIZE;i++)
    {
        T.entry[i].clear();
        T.Fentry[i].clear();
    }
    f.close();
}

void Read_Data(Hash_Table& T)
{
    fstream f;
    File F;
    Folder FO;
    string src=PATH+"\\$data.txt";
    f.open(src.c_str(),ios::in);
    if(!f)  return;
    while(!f.eof())
    {
        getline(f,src);
        if(src=="=============================================")
            break;
        F.name=src;
        f>>F.extension;
        f>>F.size;
        f.ignore();
        getline(f,F.CDT);
        f.ignore();

        T.entry[T.hash(F.name)].push_back(F);
    }
    f.ignore();
    while(!f.eof())
    {
        getline(f,FO.name);
        f>>FO.size;
        f.ignore();
        getline(f,FO.CDT);
        f.ignore();

        T.Fentry[T.hash(FO.name)].push_back(FO);
    }
    f.close();
}

//-------------------------------------------------------

void EXEQ()
{
    int i,n=0,ch;
    string s,s2;
    Folder* T;
    vector <File*> S;
    Hash_Table HT;
    Read_Data(HT);
    while(s!="Signed Out!")
    {
        n=0;
        T=NULL;
        S.clear();
        system("cls");
        cout<<"\n------------------------------------------------------------------";
        cout<<"\n\t  --------: File Management System :--------";
        cout<<"\n------------------------------------------------------------------";
        cout<<"\n01.\t Create a file.";
        cout<<"\n02.\t Rename a file.";
        cout<<"\n03.\t Search a file.";
        cout<<"\n04.\t Delete a file.";
        cout<<"\n05.\t Launch a file.";
        cout<<"\n06.\t File Description.";
        cout<<"\n07.\t Create a directory.";
        cout<<"\n08.\t Rename a directory.";
        cout<<"\n09.\t Search a directory.";
        cout<<"\n10.\t Delete a directory.";
        cout<<"\n11.\t Folder Description.";
        cout<<"\n12.\t Move up to Parent Directory.";
        cout<<"\n13.\t Move in to a Child Directory.";
        cout<<"\n14.\t List the Directory.";
        cout<<"\n15.\t Create a Shortcut.";
        cout<<"\n16.\t Copy & Paste.";
        cout<<"\n17.\t View Hash Table.";
        cout<<"\n18.\t Sign Out.";
        cout<<"\n=================================================================";
        cout<<"\nEnter your choice : ";
        cin>>ch;
        cout<<"=================================================================\n\n";
        switch(ch)
        {
            case 1:     cout<<"Enter the filename : ";
                        cin.ignore();
                        getline(cin,s);
                        n=HT.Create(s);
                        if(!n)
                            cout<<"\nFile created."<<endl;
                        else if(n==1)
                            cout<<"\nInvalid Input!!!!!\nNo Extension specified.";
                        else if(n==2)
                            cout<<"\nInvalid Input!!!!!\nFile with same name already exists.";
                        else if(n==3)
                            cout<<"\nPermission Denied! Admin access required.";
                        getch();
                        break;

            case 2:     cout<<"Enter old name : ";
                        cin.ignore();
                        getline(cin,s);
                        cout<<"Enter New name: ";
                        getline(cin,s2);
                        n=HT.Rename(s,s2);
                        if(!n)
                            cout<<"\nFile "<<s<<" renamed.";
                        else if(n==1)
                            cout<<"\nInvalid Input!!!!!\nPlease Enter full name.";
                        else if(n==2)
                            cout<<"\nInvalid Input!!!!!\nFilename already exists.";
                        else if(n==3)
                            cout<<"\nInvalid Input!!!!!\nNo such file found.";
                        else if(n==4)
                            cout<<"\nPermission Denied! Admin access required.";
                        else if(n==5)
                            cout<<"\nInvalid Input!!!!!\nFile's extension can't be changed.";
                        getch();
                        break;

            case 3:     cout<<"File to be searched : ";
                        cin.ignore();
                        getline(cin,s);
                        S=HT.Search(s);
                        if(!S.size())
                            cout<<endl<<s<<" Not Found!"<<endl;
                        else
                        {
                            cout<<"\nFound Matches:\n";
                            for(i=0;i<S.size();i++)
                                cout<<*S[i]<<endl;
                        }
                        getch();
                        break;

            case 4:     cout<<"File to be Deleted : ";
                        cin.ignore();
                        getline(cin,s);
                        n=HT.Delete(s);
                        if(!n)
                            cout<<"\nFile "<<s<<" deleted.";
                        else if(n==1)
                            cout<<"\nInvalid Input!!!!!\nPlease Enter full name.";
                        else if(n==2)
                            cout<<"\nInvalid Input!!!!!\nNo such file found.";
                        else if(n==3)
                            cout<<"\nPermission Denied! Admin access required.";
                        getch();
                        break;

            case 5:     cout<<"Enter file to be opened : ";
                        cin.ignore();
                        getline(cin,s);
                        n=HT.Open(s);
                        if(!n)
                            cout<<"\n"<<s<<" opened.";
                        else if(n==1)
                            cout<<"\nInvalid Input!!!!!\nPlease Enter full name.";
                        else if(n==2)
                            cout<<"\nInvalid Input!!!!!\nNo such file found.";
                        getch();
                        break;

            case 6:     cout<<"Enter the file name: ";
                        cin.ignore();
                        getline(cin,s);
                        HT.Describe(s);
                        getch();
                        break;

            case 7:     cout<<"Enter the folder name: ";
                        cin.ignore();
                        getline(cin,s);
                        n=HT.D_Create(s);
                        if(!n)
                            cout<<"\nFolder created."<<endl;
                        else if(n==1)
                            cout<<"\nInvalid Input!!!!!\nName Already exists.";
                        else if(n==2)
                            cout<<"\nPermission Denied! Admin access required.";
                        getch();
                        break;

            case 8:     cout<<"Enter the Folder name: ";
                        cin.ignore();
                        getline(cin,s);
                        cout<<"Enter the New name: ";
                        getline(cin,s2);
                        n=HT.D_Rename(s,s2);
                        if(!n)
                            cout<<"\nFolder "<<s<<" renamed.";
                        else if(n==1)
                            cout<<"\nInvalid Input!!!!!\nNo such folder found.";
                        else if(n==2)
                            cout<<"\nPermission Denied! Admin access required.";
                        getch();
                        break;

            case 9:     cout<<"Enter the folder name: ";
                        cin.ignore();
                        getline(cin,s);
                        T=HT.D_Search(s);
                        if(T)
                            cout<<"\nFound Match:\n"<<*T<<endl;
                        else
                            cout<<endl<<s<<" Not Found!"<<endl;
                        getch();
                        break;

            case 10:    cout<<"Enter the folder name: ";
                        cin.ignore();
                        getline(cin,s);
                        n=HT.D_Delete(s);
                        if(!n)
                            cout<<"\nFolder "<<s<<" deleted.";
                        else if(n==1)
                            cout<<"\nInvalid Input!!!!!\nNo such folder found.";
                        else if(n==2)
                            cout<<"\nPermission Denied! Admin access required.";
                        getch();
                        break;

            case 11:    cout<<"Enter the folder name: ";
                        cin.ignore();
                        getline(cin,s);
                        HT.D_Describe(s);
                        getch();
                        break;

            case 12:    n=HT.D_MoveUp(HT);
                        if(n)
                            cout<<"\nInvalid Input!!!!!\nThis is the Root Directory!";
                        getch();
                        break;

            case 13:    cout<<"Enter the folder name: ";
                        cin.ignore();
                        getline(cin,s);
                        n=HT.D_MoveDown(HT,s);
                        if(n)
                            cout<<"\nInvalid Input!!!!!\nNo such folder found.";
                        getch();
                        break;

            case 14:    HT.D_List();
                        getch();
                        break;

            case 15:    n=HT.Shortcut(HT);
                        if(!n)
                            cout<<"\nShortcut created."<<endl;
                        else if(n==1)
                            cout<<"\nInvalid Input!!!!!\nNo such Folder exists.";
                        else if(n==2)
                            cout<<"\nPermission Denied! Admin access required.";
                        getch();
                        break;

            case 16:    n=HT.Copy_Paste(HT);
                        if(!n)
                            cout<<"\nFiles Copied."<<endl;
                        else if(n==1)
                            cout<<"\nInvalid Input!!!!!\nNo such File/Folder exists.";
                        else if(n==2)
                            cout<<"\nPermission Denied! Admin access required.";
                        getch();
                        break;

            case 17:    HT.Show();
                        getch();
                        break;

            case 18:    Write_Data(HT);
                        system("cls");
                        U.Sign_out(s);
                        break;

            default:    cout<<"Please Enter a valid Choice !!!!!!\n";
                        Sleep(1500);
                        cin.clear();
                        cin.ignore(100, '\n');
                        break;
        }
    }
}

//-------------------------------------------------------

int main()
{
    int n;
    CMD="mkdir "+PATH;
    system(CMD.c_str());
    system("cls");

    User P;
    P.id="admin";
    P.pw="kernel";
    P.admin=1;
    Log.push_back(P);

    while(1)
    {
        system("cls");
        cout<<"\n------------------------------------------------------------------";
        cout<<"\n\t  --------: File Management System :--------";
        cout<<"\n------------------------------------------------------------------";
        cout<<"\n01.\t Sign In.";
        cout<<"\n02.\t Sign Up.";
        cout<<"\n03.\t Change Password.";
        cout<<"\n04.\t Shutdown.";
        cout<<"\n=================================================================";
        cout<<"\nEnter your choice : ";
        cin>>n;
        cout<<"=================================================================\n\n";
        switch(n)
        {
            case 1:     n=U.Sign_in();
                        if(!n)
                        {
                            system("cls");
                            cout<<"\n\n\n\t\t\tWelcome "<<U.id<<" !";
                            Sleep(1200);
                            EXEQ();
                        }
                        else if(n==1)
                            cout<<"\nNo such user found!";
                        else if(n==2)
                            cout<<"\nInvalid Password!";
                        Sleep(1500);
                        break;

            case 2:     n=U.Sign_up();
                        if(!n)
                            cout<<"\nAccount created.";
                        else if(n==1)
                            cout<<"\nUsername already exists!";
                        else if(n==2)
                            cout<<"\nInvalid Admin Password!";
                        Sleep(1500);
                        break;

            case 3:     n=U.change();
                        if(!n)
                            cout<<"\nPassword changed.";
                        else if(n==1)
                            cout<<"\nNo such user found!";
                        else if(n==2)
                            cout<<"\nInvalid Password!";
                        Sleep(1500);
                        break;

            case 4:     system("cls");
                        cout<<"\n\n\n\t\t\tShutting Down.....\n\n\n\n\n\n\n\n\n\n\n\n";
                        Sleep(1200);
                        return 0;
                        break;

            default:    cout<<"Please Enter a valid Choice !!!!!!\n";
                        Sleep(1500);
                        cin.clear();
                        cin.ignore(100, '\n');
                        break;
        }
    }
}
