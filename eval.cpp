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
#include <random>

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

#define MAXN 1005

string FILE_IN, FILE_OUT;

int H, W, R;
long long C_B, C_R, B;
char PLANE[MAXN][MAXN];
int PLANE_WALL[MAXN][MAXN];
bool BACKBONE[MAXN][MAXN];
bool COVERED[MAXN][MAXN];

int backinit[2];

int theres_a_wall(int a, int b, int c, int d){
    return PLANE_WALL[c][d]-(a>0?PLANE_WALL[a-1][d]:0)-(b>0?PLANE_WALL[c][b-1]:0)+(a>0&&b>0?PLANE_WALL[a-1][b-1]:0);
}
int main(int argc, char *argv[]){
    // Input stuff, you should ignore this
    int INPUT_FILE = -1;
    if (argc != 3){
        cerr<<"You should run "<<argv[0]<<" (1|2|3|4|input_name) sol"<<endl;
        return 1;
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
            FILE_IN = "charleston_road.in";
            FILE_OUT = "charleston_road.out";
            break;
        case 2:
            FILE_IN = "lets_go_higher.in";
            FILE_OUT = "lets_go_higher.out";
            break;
        case 3:
            FILE_IN = "opera.in";
            FILE_OUT = "opera.out";
            break;
        case 4:
            FILE_IN = "rue_de_londres.in";
            FILE_OUT = "rue_de_londres.out";
            break;
        case 0:
            FILE_IN = string(argv[1]);
            FILE_OUT = FILE_IN + ".out";
            break;
        default:
            cerr<<"Look, i dunno how you get there, ask someone.";
            return 1;
    }
    
    freopen(FILE_IN.c_str(),"r",stdin);
    
    int a,b,c;
    
    scanf("%d%d%d", &H, &W, &R);
    scanf("%d%d%d", &a, &b, &c); // just trust me
    C_B = a, C_R = b, B = c;
    scanf("%d%d", &backinit[0], &backinit[1]);
    BACKBONE[backinit[0]][backinit[1]] = 1;
    for (int i = 0; i < H; i++)
        scanf("%s", PLANE[i]);
    for (int i = 0; i < H; i++)
        for (int j = 0; j < W; j++)
            PLANE_WALL[i][j] = (PLANE[i][j]=='#')+(i>0?PLANE_WALL[i-1][j]:0)+(j>0?PLANE_WALL[i][j-1]:0)-(i>0&&j>0?PLANE_WALL[i-1][j-1]:0);
    
    freopen(argv[2], "r", stdin);
    
    int backcnt = -1, rtrcnt = 0;
    
    scanf("%d", &backcnt);
    for (int i_ = 0; i_ < backcnt; i_++){
        scanf("%d%d", &b, &c);
        int z = 0;
        for (int i = -1; i <= 1; i++)
            for (int j = -1; j <= 1; j++)
                if (b+i >= 0 && b+i < H && c+j >= 0 && c+j < W && BACKBONE[b+i][c+j])
                    z++;
        if (!z){
            cerr<<"Backbone not connected"<<endl;
            return 1;
        }
        BACKBONE[b][c] = 1;
    }
    scanf("%d", &rtrcnt);
    for (int i_ = 0; i_ < rtrcnt; i_++){
        scanf("%d%d", &b, &c);
        int z = 0;
        for (int i = -1; i <= 1; i++)
            for (int j = -1; j <= 1; j++)
                if (b+i >= 0 && b+i < H && c+j >= 0 && c+j < W && BACKBONE[b+i][c+j])
                    z++;
        if (!z){
            cerr<<"Router not connected"<<endl;
            return 1;
        }
        for (int bb = -R; bb <= R; bb++)
            for (int cc = -R; cc <= R; cc++)
                if (b+bb >= 0 && b+bb < H && c+cc >= 0 && c+cc < W){
                    if (COVERED[b+bb][c+cc] || PLANE[b+bb][c+cc] != '.') continue;
                    if (!theres_a_wall(min(b+bb,b),min(c+cc,c),max(b+bb,b),max(c+cc,c)))
                        COVERED[b+bb][c+cc] = 1;
                }
    }
    
    ll ANS = 0;
    
    for (int i = 0; i < H; i++)
        for (int j = 0; j < W; j++)
            if (COVERED[i][j]) ANS += 1000;
    
    cout<<"Score(Cov*1000):"<<ANS<<endl;
    cout<<"Router:"<<rtrcnt<<endl;
    cout<<"Backbone:"<<backcnt<<endl;
    cout<<"RemainingBudget:"<<B - (rtrcnt * C_R + backcnt * C_B)<<endl;
    if (B - (rtrcnt * C_R + backcnt * C_B) < 0){
        cerr<<"Dafaq B is negative"<<endl;
        exit(0);
    }
    cout<<FILE_OUT<<" "<<ANS + (B - (rtrcnt * C_R + backcnt * C_B))<<endl;
    
    return 0;
}
