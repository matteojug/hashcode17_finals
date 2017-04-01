#include <map>
#include <set>
#include <list>
#include <cmath>
#include <ctime>
#include <deque>
#include <queue>
#include <stack>
#include <string>
#include <bitset>
#include <cstdio>
#include <limits>
#include <vector>
#include <climits>
#include <cstring>
#include <cstdlib>
#include <fstream>
#include <numeric>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <unordered_map>

using namespace std;

typedef long long ll;

typedef pair<int,int> ii;
typedef vector<int> vi;
typedef vector<ii> vii;

#define ff first
#define ss second
#define mp make_pair
#define pb push_back
#define mt make_tuple
#define tget(X,id) get<id>(X)

#define MAXN 1000000

string FILE_IN, FILE_OUT;

int main(int argc, char *argv[]){
    // Input stuff, you should ignore this
    int INPUT_FILE = -1;
    if (argc == 1){
        cerr<<"No input specified, using input 1.\n FIY, you should run "<<argv[0]<<" [(1|2|3|4|input_name) [param1 [param2 ... ]]]"<<endl;
        INPUT_FILE = 1;
    }
    if (argc >= 2){
        int z = atoi(argv[1]);
        if (z == 0 && strlen(argv[1]) == 1){
            cerr<<"You specified 0 as first arg, but input numbers are from 1 to 4.. Idiot.\nUsing input 1, just for this time"<<endl;
            INPUT_FILE = 1;
        } else if (z == 0)
            INPUT_FILE = 0;
        else {
            if (z < 1 || z > 4){
                cerr<<"Your input file was not in [1,4], using 1 because I'm merciful"<<endl;
                INPUT_FILE = 1;
            } else
                INPUT_FILE = z;
        }
    }
    switch (INPUT_FILE){
        case 1:
            FILE_IN = "A.in";
            FILE_OUT = "A.out";
            break;
        case 2:
            FILE_IN = "B.in";
            FILE_OUT = "B.out";
            break;
        case 3:
            FILE_IN = "C.in";
            FILE_OUT = "C.out";
            break;
        case 4:
            FILE_IN = "D.in";
            FILE_OUT = "D.out";
            break;
        case 0:
            FILE_IN = string(argv[1]);
            FILE_OUT = FILE_IN + ".out";
            break;
        default:
            cerr<<"Look, i dunno how you get there, ask someone.";
            return 1;
    }
    // Parameter stuff, for tuning
    int param = 0;
    if (argc >= 3) param = atoi(argv[2]);
    
    cout<<"Got parameter "<<param<<endl;
    
    // Actual code
    freopen(FILE_IN.c_str(),"r",stdin);
    
    cout<<"Here do the magic"<<endl;
    
    int a;
    scanf("%d", &a);
    
    vector<tuple<int, int, string> > v;
    v.pb(mt(1,2,"pippo"));
    v.pb(mt(4,6,"pappa"));
    v.pb(mt(2,-1,"jbfkjbgs"));
    sort(v.begin(), v.end());
    for (auto x : v)
        //~ cout<<get<0>(x)<<","<<get<1>(x)<<","<<get<2>(x)<<endl;
        cout<<tget(x,0)<<","<<tget(x,1)<<","<<tget(x,2)<<endl;
        
    bitset<10> bs;
    bs[0] = 1;
    bs[2] = 1;
    bs[9] = 1;
    cout<<bs.to_string()<<endl;
    
    cout<<"Now i'll print the score (a+param**2) and the fileout"<<endl;
    cout<<FILE_OUT<<" "<<a+param*param<<endl;
    
    // Solution printing
    freopen(FILE_OUT.c_str(),"w",stdout);
    cout<<"Here print the solution"<<endl;
    return 0;
}
