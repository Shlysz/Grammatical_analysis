#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <stack>
#include <algorithm>
using namespace std;


struct Production//文法的产生式
{
    char left;
    string right;
};


struct Grammar
{
 vector<Production> production;
 map<char,string> FIRST;//文法的FIRST集
 map<char,string> FOLLOW;//文法的FLLOW集
 vector<char> Nonterminal;//文法的非终结符
 vector<string> Terminator;//文法的终结符

}grammar;


string ch_String;//输入串

Grammar G;//文法
Production  analysis_table[5][8];

//E→ E+T | E–T | T
// T→ T*F | T/F | F
// F→ (E) | num

void initGrammar();//初始化文法
void Set_FIRSTandFOLLOW();//设置FIRST和FOLLOW集合
void Make_analysis_table();//构造分析表
string GetFIRST(string a);//获取FIRST集合
string GetFOLLOW(char a);//获取FOLLOW集合
bool is_Nonterminal(char a);//判断是否为非终结符
bool is_Terminator(string a);//判断是否为终结符
int  GetIndex0(char left);//获取预测表横坐标
int  GetIndex1(char right);//获取预测表纵坐标

void Analysis_Input(string inputString);//构造LL(1)分析程序

int main()
{
    initGrammar();
    Set_FIRSTandFOLLOW();
    Make_analysis_table();
    cout<<"The forecast table is shown in the figure below: "<<endl;
    cout<<"N\t"<<"+\t"<<"-\t"<<"*\t"<<"/\t"<<"(\t"<<")\t"<<"num\t"<<"$\t"<<endl;
    for(int i=0;i<5;i++)
    {
        if(i==0) cout<<"E\t";
        if(i==1) cout<<"A\t";
        if(i==2) cout<<"T\t";
        if(i==3) cout<<"B\t";
        if(i==4) cout<<"F\t";

        for(int j=0;j<8;j++)
        {

            if(analysis_table[i][j].left=='#') cout<<"err\t";
            else cout<<analysis_table[i][j].left<<"->"<<analysis_table[i][j].right<<"\t";
            if(j==7)
                cout<<endl;
        }

    }

    cout<<"Now,Please input your string:"<<endl;
    cin>>ch_String;
    Analysis_Input(ch_String);

    return 0;
}

void initGrammar()
{
    G.Nonterminal.push_back('E');
    G.Nonterminal.push_back('A');
    G.Nonterminal.push_back('T');
    G.Nonterminal.push_back('B');
    G.Nonterminal.push_back('F');

    G.Terminator.emplace_back("+");
    G.Terminator.emplace_back("-");
    G.Terminator.emplace_back("*");
    G.Terminator.emplace_back("/");
    G.Terminator.emplace_back("(");
    G.Terminator.emplace_back(")");
    G.Terminator.emplace_back("n");
    G.Terminator.emplace_back("#");



    //E→ E+T | E–T | T
    // T→ T*F | T/F | F
    // F→ (E) | num

    //E->TA   A->+TA|-TA|#
    //T->FB  B->*FB|/FB|#
    //F->(E)|num
    Production tmpProduction;//这里直接就读取了消除左递归的文法了
    tmpProduction.left='E';
    tmpProduction.right="TA";
    G.production.push_back(tmpProduction);
    tmpProduction.left='A';
    tmpProduction.right="+TA";
    G.production.push_back(tmpProduction);
    tmpProduction.left='A';
    tmpProduction.right="-TA";
    G.production.push_back(tmpProduction);
    tmpProduction.left='A';
    tmpProduction.right="#";
    G.production.push_back(tmpProduction);
    tmpProduction.left='T';
    tmpProduction.right="FB";
    G.production.push_back(tmpProduction);
    tmpProduction.left='B';
    tmpProduction.right="*FB";
    G.production.push_back(tmpProduction);
    tmpProduction.left='B';
    tmpProduction.right="/FB";
    G.production.push_back(tmpProduction);
    tmpProduction.left='B';
    tmpProduction.right="#";
    G.production.push_back(tmpProduction);
    tmpProduction.left='F';
    tmpProduction.right="(E)";
    G.production.push_back(tmpProduction);
    tmpProduction.left='F';
    tmpProduction.right="n";
    G.production.push_back(tmpProduction);
    //'T','A','B','F'



}

void Set_FIRSTandFOLLOW()
{

    G.FIRST.insert(pair<char,string>('E',"(n"));
    G.FIRST.insert(pair<char,string>('T',"(n"));
    G.FIRST.insert(pair<char,string>('A',"+-#"));
    G.FIRST.insert(pair<char,string>('B',"*/#"));
    G.FIRST.insert(pair<char,string>('F',"(n"));

    G.FOLLOW.insert(pair<char,string>('E',"$)"));
    G.FOLLOW.insert(pair<char,string>('T',"+-)&"));
    G.FOLLOW.insert(pair<char,string>('A',"$)"));
    G.FOLLOW.insert(pair<char,string>('B',"+-)$"));
    G.FOLLOW.insert(pair<char,string>('F',"*/+-$"));

}

void Make_analysis_table()
{
    for(int i=0;i<5;i++)
        for(int j=0;j<8;j++)
        {
            analysis_table[i][j].left='#';
        }

    for(int i=0;i<G.production.size();i++)
    {
        //for()
        string tmpFIRST= GetFIRST(G.production[i].right);
        if("#"!=tmpFIRST)
        for(int j=0;j<tmpFIRST.length();j++)
        {
            analysis_table[GetIndex0(G.production[i].left)][GetIndex1(tmpFIRST[j])].left=G.production[i].left;
            analysis_table[GetIndex0(G.production[i].left)][GetIndex1(tmpFIRST[j])].right=G.production[i].right;
        }
        if("#"==tmpFIRST)
        {
            string tmpFOLLOW= GetFOLLOW(G.production[i].left);
            for(int k=0;k<tmpFOLLOW.length();k++)
            {

                int index1=GetIndex1(tmpFOLLOW[k]);
                analysis_table[GetIndex0(G.production[i].left)][index1].left=G.production[i].left;
                analysis_table[GetIndex0(G.production[i].left)][index1].right=G.production[i].right;

            }

        }


    }
}

string GetFIRST(string a)
{
    if(a=="#") return "#";
    string retrunString;


    string s1;
    s1.push_back(a[0]);
    if(is_Terminator(s1)&&a[0]!='#')
    {
        retrunString+=a[0];
    }
    else if(a[0]=='#')
    {
        retrunString+= GetFIRST(a.substr(1));
    }
    else if(is_Nonterminal(a[0]))//如果是非终结符号
    {
        auto it=G.FIRST.find(a[0]);
        string tmp=it->second;
        tmp.erase(remove(tmp.begin(), tmp.end(), '#'), tmp.end());
        retrunString+=tmp;

    }


    return retrunString;
}

string GetFOLLOW(char a)
{
   auto it=G.FOLLOW.find(a);
   string returnString=it->second;
    return returnString;
}

bool is_Nonterminal(char a)
{
    for(int i=0;i<G.Nonterminal.size();i++)
    {
        if(a==G.Nonterminal[i])
            return true;

    }
    return false;
}

bool is_Terminator(string a)
{
    for(int i=0;i<G.Terminator.size();i++)
    {
        if(a==G.Terminator[i])
            return true;

    }
    return false;
}
int  GetIndex0(char left)
{
    if(left=='E')
        return 0;
    if(left=='A')
        return 1;
    if(left=='T')
        return 2;
    if(left=='B')
        return 3;
    if(left=='F')
        return 4;
}
int  GetIndex1(char right)
{
    if(right=='+')
        return 0;
    if(right=='-')
        return 1;
    if(right=='*')
        return 2;
    if(right=='/')
        return 3;
    if(right=='(')
        return 4;
    if(right==')')
        return 5;
    if(right=='n')
        return 6;
    if(right=='$'||right=='#')
        return 7;

}
void Analysis_Input(string inputString)
{
    if(inputString[inputString.length()-1]!='$')
    inputString+='$';
    stack<char> AnalysisStack;
    AnalysisStack.push('$');
    AnalysisStack.push('E');
    char X;
    int ip=0;
    do {
        X=AnalysisStack.top();
        char a=inputString[ip];
        string s1;
        s1.push_back(X);
        if(X=='$'|| is_Terminator(s1))
        {
            if(X==a)
            {
                AnalysisStack.pop();
                if(ip<inputString.length())
                    ip++;

            }
            else cout<<"error"<<endl;

        }
        else
        {
            if(analysis_table[GetIndex0(X)][GetIndex1(a)].left!='#')
            {
                string tmp=analysis_table[GetIndex0(X)][GetIndex1(a)].right;
                AnalysisStack.pop();
                for(int i=tmp.length()-1;i>=0;i--)
                {
                    if(tmp[i]!='#')
                    AnalysisStack.push(tmp[i]);
                    cout<<analysis_table[GetIndex0(X)][GetIndex1(a)].left<<
                    "->"<<analysis_table[GetIndex0(X)][GetIndex1(a)].right<<endl;
                }

            }
            else cout<<"error"<<endl;
        }
    } while (X!='$');

    cout<<"done!!!"<<endl;
}



