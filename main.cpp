#include <iostream>
#include <vector>
#include <set>
#include <cstdlib>
#include <ctime>
#include <fstream>

using namespace std;


class Sudoku{
public:
    vector<vector<int> > numMap;//9*9 0
    vector<vector<set<int> > > availableNum;//row,col,nine 1~9
    vector<vector<set<int> > > everyNum;

    Sudoku(vector<vector<int> > v){
        initMap(v);
    }

    bool updateMap(int small,int big,int value){
        if(numMap[big][small]==0){
            set<int> &row = availableNum[0][getRow(small,big)];
            set<int> &col = availableNum[1][getCol(small,big)];
            set<int> &nin = availableNum[2][big];
            set<int>::iterator rowIt = row.find(value);
            set<int>::iterator colIt = col.find(value);
            set<int>::iterator ninIt = nin.find(value);
            if(rowIt!=row.end()&&
               colIt!=col.end()&&
               ninIt!=nin.end()){
                row.erase(rowIt);
                col.erase(colIt);
                nin.erase(ninIt);
                numMap[big][small] = value;
                set<int> s;
                everyNum[big][small] =s;
                everyNum[big][small].insert(value);
                if(updateEve(small,big,value))
                    return true;
               }
        }
        return false;
    }
    bool updateEve(int small,int big,int value){
        for(int i=0;i!=9;++i){
            if(numMap[big][i]==0){
                //set<int> s = getEve(i,big);
                set<int>::iterator it = everyNum[big][i].find(value);
                if(it!=everyNum[big][i].end()){
                    everyNum[big][i].erase(it);
                }
                if(everyNum[big][i].size()==0)
                    return false;
            //if(s.size()==1&&)
                //numMap[big][i] = *(s.begin());
                //everyNum[big][i]=s;
            }
        }
        int r = getRow(small,big);
        for(int j=0;j!=3;++j){
            for(int k=0;k!=3;++k){
                int a=r/3*3+j;
                int b=r%3*3+k;
                if(numMap[a][b]==0){
                    //set<int> s = getEve(b,a);
                    set<int>::iterator it = everyNum[a][b].find(value);
                    if(it!=everyNum[a][b].end()){
                        everyNum[a][b].erase(it);
                    }
                    if(everyNum[a][b].size()==0)
                        return false;
                //if(s.size()==1)
                    //numMap[a][b] = *(s.begin());
                    //everyNum[a][b]=s;
                }
            }
        }
        for(int j=0;j!=3;++j){
            for(int k=0;k!=3;++k){
                int a=big%3+j*3;
                int b=small%3+k*3;
                if(numMap[a][b]==0){
                    set<int>::iterator it = everyNum[a][b].find(value);
                    if(it!=everyNum[a][b].end()){
                        everyNum[a][b].erase(it);
                    }
                    if(everyNum[a][b].size()==0)
                        return false;
                //if(s.size()==1)
                    //numMap[a][b] = *(s.begin());

                }
            }
        }
        return true;
    }
    bool check(){
        for(int i=0;i!=9;++i){
            set<int> s;
            for(int j=0;j!=9;++j){
                for(auto it:everyNum[i][j]){
                    s.insert(it);
                }
            }
            if(s.size()!=9)
                return false;
        }
        for(int r=0;r!=9;++r){
            set<int> s;
            for(int j=0;j!=3;++j){
                for(int k=0;k!=3;++k){
                    int a=r/3*3+j;
                    int b=r%3*3+k;
                    for(auto it:everyNum[a][b]){
                        s.insert(it);
                    }
                }
            }
            if(s.size()!=9)
                return false;
        }
        for(int c=0;c!=9;++c){
            set<int> s;
            for(int j=0;j!=3;++j){
                for(int k=0;k!=3;++k){
                    int a=j*3+c/3;
                    int b=k*3+c%3;
                    for(auto it:everyNum[a][b]){
                        s.insert(it);
                    }
                }
            }
            if(s.size()!=9)
                return false;
        }
        return true;
    }
    void initMap(vector<vector<int> > vv){
        vector<int> v(9,0);
        set<int> s;
        for(int i=1;i!=10;++i){
            s.insert(i);
        }
        vector<set<int> > sv(9,s);
        availableNum = vector<vector<set<int> > >(3,sv);
        everyNum = vector<vector<set<int> > >(9,sv);
        numMap = vector<vector<int> >(9,v);
        for(int i=0;i!=9;++i){
            vector<int> tmp = vv[i];
            for(int j=0;j!=3;++j){
                for(int k=0;k!=3;++k){
                    numMap[i/3*3+j][i%3*3+k]=tmp[j*3+k];
                    if(tmp[j*3+k]!=0){
                        int value = tmp[j*3+k];
                        set<int> a;
                        everyNum[i/3*3+j][i%3*3+k] = a;
                        set<int> &row = availableNum[0][getRow(i%3*3+k,i/3*3+j)];
                        set<int> &col = availableNum[1][getCol(i%3*3+k,i/3*3+j)];
                        set<int> &nin = availableNum[2][i/3*3+j];
                        set<int>::iterator rowIt = row.find(value);
                        set<int>::iterator colIt = col.find(value);
                        set<int>::iterator ninIt = nin.find(value);
                        row.erase(rowIt);
                        col.erase(colIt);
                        nin.erase(ninIt);
                    }
                }
            }
        }
        showMap();
        set<int> tmp;
        for(int i=0;i!=9;++i){
            for(int j=0;j!=9;++j){
                if(numMap[i][j]==0){
                    everyNum[i][j] = getEve(j,i);
                }
                else{
                    everyNum[i][j] = tmp;
                    everyNum[i][j].insert(numMap[i][j]);
                }

            }
        }
        showEve();
    }
    set<int> getEve(int small,int big){
        set<int> &row = availableNum[0][getRow(small,big)];
        set<int> &col = availableNum[1][getCol(small,big)];
        set<int> &nin = availableNum[2][big];
        set<int> res;
        for(auto it:row){
            if(col.find(it)!=col.end()&&nin.find(it)!=nin.end()){
                res.insert(it);
            }
        }
        return res;
    }

    pair<int,int> getMin(){
        pair<int,int> res = make_pair(9,9);
        int Min =10;
        for(int i=0;i!=9;++i){
            for(int j=0;j!=9;++j){
                if(numMap[i][j]==0&&everyNum[i][j].size()<Min){
                    res = make_pair(i,j);
                    Min = everyNum[i][j].size();
                }
            }
        }
        return res;
    }
    int getEveNum(int small,int big){
        set<int> &row = availableNum[0][getRow(small,big)];
        set<int> &col = availableNum[1][getCol(small,big)];
        set<int> &nin = availableNum[2][big];
        int res=0;
        for(auto it:row){
            if(col.find(it)!=col.end()&&nin.find(it)!=nin.end()){
                res++;
            }
        }
        return res;
    }
    int getRow(int small,int big){
        return big/3*3+small/3;
    }
    int getCol(int small,int big){
        return big%3*3+small%3;
    }
    void showMap(){
        for(int i=0;i!=9;++i){
            cout<<"                              ";
            if(i%3==0){
                cout<<"---------------------"<<endl;
                            cout<<"                              ";
            }
            for(int j=0;j!=3;++j){
                cout<<"|";
                for(int k=0;k!=3;++k){
                    if(numMap[i/3*3+j][i%3*3+k]==0)
                        cout<<"  ";
                    else
                        cout<<numMap[i/3*3+j][i%3*3+k]<<" ";
                }
            }
            cout<<"|"<<endl;
        }
        cout<<"                              ";
        cout<<"---------------------"<<endl;

    }
    void showEve(){
        for(int i=0;i!=9;++i){
            cout<<"                              ";
            if(i%3==0){
                cout<<"---------------------"<<endl;
                            cout<<"                              ";
            }
            for(int j=0;j!=3;++j){
                cout<<"|";
                for(int k=0;k!=3;++k){
                    if(numMap[i/3*3+j][i%3*3+k]==0){
                        for(auto it:everyNum[i/3*3+j][i%3*3+k])
                            cout<<it;
                        cout<<" ";
                    }
                    else
                        cout<<numMap[i/3*3+j][i%3*3+k]<<" ";
                }
            }
            cout<<"|"<<endl;
        }
        cout<<"                              ";
        cout<<"---------------------"<<endl;

    }
};

bool solu(Sudoku mSudoku,int small,int big,int value);
bool solu(Sudoku mSudoku);
bool solu1(Sudoku mSudoku,int small,int big,int value);
bool solu1(Sudoku mSudoku){
    pair<int,int> p = mSudoku.getMin();
    for(auto i:mSudoku.everyNum[p.first][p.second]){
        if(solu1(mSudoku,p.second,p.first,i))
            return true;
    }
    return false;
}
bool solu(Sudoku mSudoku){
    pair<int,int> p = mSudoku.getMin();
    for(auto i:mSudoku.everyNum[p.first][p.second]){
        if(solu(mSudoku,p.second,p.first,i))
            return true;
    }
    return false;
}

bool solu(Sudoku mSudoku,int small,int big,int value){
    if(!mSudoku.updateMap(small,big,value))
        return false;
    pair<int,int> p = mSudoku.getMin();
    if(p==make_pair(9,9)){
        mSudoku.showMap();
        return true;
    }
    for(auto i:mSudoku.everyNum[p.first][p.second]){
        if(solu(mSudoku,p.second,p.first,i))
            return true;
    }
    return false;
}

bool solu1(Sudoku mSudoku,int small,int big,int value){
    if(!mSudoku.updateMap(small,big,value)||!mSudoku.check())
        return false;
    pair<int,int> p = mSudoku.getMin();
    if(p==make_pair(9,9)){
        mSudoku.showMap();
        return true;
    }
    for(auto i:mSudoku.everyNum[p.first][p.second]){
        if(solu1(mSudoku,p.second,p.first,i))
            return true;
    }
    return false;
}

int main()
{
    vector<int> a(9,0);
    vector<vector<int> > b(9,a);
    vector<vector<vector<int> > > v(95,b);
    vector<double> tim(95,0);
    vector<double> tim1(95,0);
    string str;
    for(int i=0;i!=95;++i){
        cin>>str;
        for(int j=0;j!=9;++j){
            for(int k=0;k!=9;++k){
                if(str[j*9+k]=='.')
                    v[i][j][k]=0;
                else
                    v[i][j][k]=str[j*9+k]-'0';
            }
        }
        Sudoku mSudoku(v[i]);
        //unsigned long start = ::GetTickCount();
        //cout<<start;
        clock_t start = clock();
        solu(mSudoku);
        clock_t en   = clock();
        tim[i] = (double)(en - start) / CLOCKS_PER_SEC;
        start = clock();
        solu1(mSudoku);
        en   = clock();
        tim1[i] = (double)(en - start) / CLOCKS_PER_SEC;
    }
    ofstream file("spendTime.txt");
    for(int i=0;i!=95;++i){
        file<<tim[i]<<",";
    }
    file<<endl;
    file.close();
    ofstream file1("spendTime1.txt");
    for(int i=0;i!=95;++i){
        file1<<tim1[i]<<",";
    }
    file1.close();
    return 0;
}
