#include "header.h"

void User::operator=(const User& P)
{
    admin=P.admin;
    id=P.id;
    pw=P.pw;
}

void User::SET_PRM()
{
    if(!U_Type)
    {
        p[0]=1;
        p[1]=0;     //create, rename, delete
        p[2]=0;
    }
    else
    {
        p[0]=1;
        p[1]=1;
        p[2]=1;
    }
}

int User::Sign_up()
{
    User P;
    cout<<"\n User ID :  ";
    cin.ignore();
    getline(cin,P.id);
    cout<<"Password :  ";
    getline(cin,P.pw);

    for(int i=0;i<Log.size();i++)
        if(P.id==Log[i].id)
            return 1;           //Username already exists!


    cout<<"Account Type:  ";
    cin>>P.admin;
    if(P.admin)
    {
        P.admin=0;
        cin.ignore();
        if(Admin_access())
            P.admin=1;

        else
            return 2;
    }

    Log.push_back(P);
    return 0;
}

int User::Sign_in()
{
    string Uid,Upw;
    cout<<"\n User ID :  ";
    cin.ignore();
    getline(cin,Uid);
    cout<<"Password :  ";
    getline(cin,Upw);

    for(int i=0;i<Log.size();i++)
        if(Uid==Log[i].id)
        {
            if(Upw!=Log[i].pw)
                return 2;       //Incorrect Password!

            U=Log[i];
            U_Type=U.admin;
            SET_PRM();
            return 0;
        }
    return 1;                   //No such user found!
}

void User::Sign_out(string &s)
{
    U_Type=-1;
    cout<<"\n\n\n\t\t\tGoodbye !!!!!";
    s="Signed Out!";
}

int User::Admin_access()
{
    int i;
    string w;
    cout<<"\nEnter Admin Password:  ";
    getline(cin,w);
    for(i=0;i<Log.size();i++)
        if(Log[i].admin)
            if(Log[i].pw==w)
                return 1;

    return 0;           //Incorrect Admin Password!

}

int User::change()
{
    string Uid,Upw,New;
    cout<<"\n User ID :  ";
    cin.ignore();
    getline(cin,Uid);
    cout<<"Password :  ";
    getline(cin,Upw);

    for(int i=0;i<Log.size();i++)
        if(Uid==Log[i].id)
        {
            if(Upw!=Log[i].pw)
                return 2;       //Incorrect Password!

            cout<<"New Password :  ";
            getline(cin,New);
            Log[i].pw=New;
            return 0;
        }
    return 1;                   //No such user found!
}
