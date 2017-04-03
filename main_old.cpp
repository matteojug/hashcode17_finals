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
long long C_B, C_R, B, BB;
char PLANE[MAXN][MAXN];
char PLANE_TMP[MAXN][MAXN];
int PLANE_WALL[MAXN][MAXN];
bool BACKBONE[MAXN][MAXN];
bool BACKBONE_TMP[MAXN][MAXN];
int BACKBONE_COST[MAXN][MAXN];
bool COVERED[MAXN][MAXN];
bool ROUTER[MAXN][MAXN];
bool RECT_COVERED[MAXN][MAXN];
ii RECT_CHAMPION[MAXN][MAXN];
ll SCORES[MAXN][MAXN];

int backinit[2];
    
int DELTA_SKIP = 1;

int u_P[MAXN*MAXN], u_R[MAXN*MAXN];
void u_init(int sz){
    for (int i = 0; i < sz; i++) u_P[i] = i, u_R[i] = 0;
}
int u_parent(int i){
    return (u_P[i] == i ? i : u_P[i] = u_parent(u_P[i]));
}
int u_same(int a, int b){
    return u_parent(a) == u_parent(b);
}
void u_join(int a, int b){
    a = u_parent(a);
    b = u_parent(b);
    if (u_R[a] > u_R[b])
        u_P[b] = a;
    else {
        u_P[a] = b;
        if (u_R[a] == u_R[b]) u_R[b]++;
    }
}

void quick_calc_backcost(queue<ii> &q){
    while (!q.empty()){
        auto x = q.front(); q.pop();
        for (int i = -1; i <= 1; i++)
            for (int j = -1; j <= 1; j++)
                if (x.ff+i >= 0 && x.ff+i < H && x.ss+j >= 0 && x.ss+j < W){
                    if (BACKBONE_COST[x.ff+i][x.ss+j] > BACKBONE_COST[x.ff][x.ss] + C_B)
                        BACKBONE_COST[x.ff+i][x.ss+j] = min(BACKBONE_COST[x.ff][x.ss] + C_B, B+1),
                        q.push(mp(x.ff+i,x.ss+j));
                }
    }
}

void calc_backcost(){
    queue<ii> q;
    for (int i = 0; i < H; i++)
        for (int j = 0; j < W; j++){
            if (BACKBONE[i][j])
                q.push(mp(i,j)), BACKBONE_COST[i][j] = 0;
            else
                BACKBONE_COST[i][j] = B+1;
        }
    quick_calc_backcost(q);
}

int theres_a_wall(int a, int b, int c, int d){
    return PLANE_WALL[c][d]-(a>0?PLANE_WALL[a-1][d]:0)-(b>0?PLANE_WALL[c][b-1]:0)+(a>0&&b>0?PLANE_WALL[a-1][b-1]:0);
}
void calc_champions(){
    for (int i = 0; i < H; i++)
        for (int j = 0; j < W; j++){
            int ic = i, jc = j;
            if (ROUTER[ic][jc] || PLANE[ic][jc] == '#') continue;
            ll score = 0;
            for (int a = -R; a <= R; a++)
                for (int b = -R; b <= R; b++)
                    if (ic+a >= 0 && ic+a < H && jc+b >= 0 && jc+b < W){
                        if (COVERED[ic+a][jc+b] || PLANE[ic+a][b+jc] != '.') continue;
                        if (!theres_a_wall(min(ic+a,ic),min(jc+b,jc),max(ic+a,ic),max(jc+b,jc)))
                            score += 1000;
                    }
            //~ score -= BACKBONE_COST[ic][jc]*C_B+C_R;
            SCORES[ic][jc] = score;
        }
    for (int i = 0; i < H; i+=DELTA_SKIP)
        for (int j = 0; j < W; j+=DELTA_SKIP){
            RECT_CHAMPION[i][j] = mp(i,j);
            for (int a = 0; a < DELTA_SKIP; a++)
                for (int b = 0; b < DELTA_SKIP; b++)
                    if (i+a < H && j+b < W && SCORES[i+a][j+b] > SCORES[RECT_CHAMPION[i][j].ff][RECT_CHAMPION[i][j].ss])
                        RECT_CHAMPION[i][j] = mp(i+a,j+b);
        }
    
}

pair<ll, ii> calc_scores(){
    ll best = -B;//mp(-B, -B);
    vii besters;
    besters.pb(mp(0,0));
    for (int i = 0; i < H; i+=DELTA_SKIP)
        for (int j = 0; j < W; j+=DELTA_SKIP){
            int ic = i, jc = j;
            if (RECT_COVERED[i][j]) continue;
            ic = RECT_CHAMPION[i][j].ff;
            jc = RECT_CHAMPION[i][j].ss;
            if (ROUTER[ic][jc] || PLANE[ic][jc] == '#') continue;
            ll score = 0;
            for (int a = -R; a <= R; a++)
                for (int b = -R; b <= R; b++)
                    if (ic+a >= 0 && ic+a < H && jc+b >= 0 && jc+b < W){
                        if (COVERED[ic+a][jc+b] || PLANE[ic+a][b+jc] != '.') continue;
                        if (!theres_a_wall(min(ic+a,ic),min(jc+b,jc),max(ic+a,ic),max(jc+b,jc)))
                            score += 1000;
                    }
            if (BB-(BACKBONE_COST[ic][jc]*C_B+C_R) < 0) continue;
            //~ score -= BACKBONE_COST[ic][jc]*C_B * 10;
            if (score > best){
                besters.clear();
                besters.pb(mp(ic,jc));
                best = score;
                
            } else if (score == best)
                besters.pb(mp(ic,jc));
        }
    return mp(best, besters[0]);
    //~ return mp(best, besters[rand()%besters.size()]);
}
void fix_backbone(){
    vii pts;
    
    memset(BACKBONE, 0, sizeof BACKBONE);
    for (int i = 0; i < H; i++)
        for (int j = 0; j < W; j++){
            if (!ROUTER[i][j]) continue;
            pts.pb(mp(i,j));
            BACKBONE[i][j] = 1;
    }
    pts.pb(mp(backinit[0], backinit[1]));
    BACKBONE[backinit[0]][backinit[1]] = 1;
    
    vector<pair<int, ii> > edges;
    for (int i = 0; i < pts.size(); i++)
        for (int j = i+1; j < pts.size(); j++)
            edges.pb(mp(max(abs(pts[i].ff-pts[j].ff),abs(pts[i].ss-pts[j].ss)),mp(i,j)));
    sort(edges.begin(), edges.end());
    
    u_init(pts.size()+5);
    int cnt = pts.size(), i = 0, a, b;
    while (cnt > 1){
        auto x = edges[i++];
        if (u_same(x.ss.ff, x.ss.ss)) continue;
        u_join(x.ss.ff, x.ss.ss);
        cnt--;
        BACKBONE[pts[x.ss.ff].ff][pts[x.ss.ff].ss] = 0;
        //~ ii x1 = q.front(), x2 = mp(-1,-1);
        //~ if (!BACKBONE[x1.ff][x1.ss]){
            //~ memset(BACKBONE_TMP, 0, sizeof BACKBONE_TMP);
            //~ BACKBONE_TMP[x1.ff][x1.ss] = 1;
            //~ while (!q.empty()){
                //~ x1 = q.front();
                //~ q.pop();
                //~ for (int i = -1; i <= 1; i++)
                    //~ for (int j = -1; j <= 1; j++)
                        //~ if (x1.ff+i >= 0 && x1.ff+i < H && x1.ss+j >= 0 && x1.ss+j < W){
                            //~ if (!BACKBONE_TMP[x1.ff+i][x1.ss+j]){
                                //~ if (BACKBONE[x1.ff+i][x1.ss+j]){
                                    //~ x2 = mp(x1.ff+i, x1.ss+j);
                                    //~ break;
                                //~ }
                                //~ BACKBONE_TMP[x1.ff+i][x1.ss+j] = 1;
                                //~ q.push(mp(x1.ff+i,x1.ss+j));
                            //~ }
                        //~ }
                //~ if (x2.ff != -1)
                    //~ break;
            //~ }
        //~ } else
            //~ x2 = x.ss;
        
        a = pts[x.ss.ff].ff, b = pts[x.ss.ff].ss;
        //~ queue<ii> new_back;
        while (mp(a,b) != pts[x.ss.ss]){
            BACKBONE[a][b] = 1;
            //~ new_back.push(mp(a,b));
            BACKBONE_COST[a][b] = 0;
            if (a > pts[x.ss.ss].ff) a--;
            if (a < pts[x.ss.ss].ff) a++;
            if (b > pts[x.ss.ss].ss) b--;
            if (b < pts[x.ss.ss].ss) b++;
        }
    }
    int backcnt = -1, rtrcnt = 0;
    for (int i = 0; i < H; i++)
        for (int j = 0; j < W; j++){
            if (!ROUTER[i][j]) continue;
            rtrcnt++;
    }
    for (int i = 0; i < H; i++)
        for (int j = 0; j < W; j++)
            if (BACKBONE[i][j]) backcnt++;
    BB = B - (rtrcnt * C_R + backcnt * C_B);
    
    calc_backcost();
}
int main(int argc, char *argv[]){
    int seed = time(NULL);
    srand(seed);
    
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
            FILE_IN = "charleston_road.in";
            FILE_OUT = "charleston_road.out";
            break;
        case 2:
            FILE_IN = "lets_go_higher.in";
            FILE_OUT = "lets_go_higher.out_tmp";
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
    // Parameter stuff, for tuning
    //~ int param = 0;
    //~ if (argc >= 3) param = atoi(argv[2]);
    
    //~ cout<<"Got parameter "<<param<<endl;
    
    // Actual code
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
    
    BB = B;
    //~ for (int i = 0; i < H; i++){ for (int j = 0; j < W; j++) cout<<BACKBONE[i][j]; cout<<endl;}
    //~ for (int i = 0; i < H; i++){ for (int j = 0; j < W; j++) cout<<BACKBONE_COST[i][j]<<"|"; cout<<endl;}
    
    ll ANS = 0;
    
    RECT_CHAMPION[0][0] = mp(-1,-1);
    
    //~ int deb = 0;
    calc_backcost();
    calc_champions();
    
    int back_cnt_ref = 0;
    while (true){
        back_cnt_ref++;
        if (back_cnt_ref > 1000 && BB > 1000){
            fix_backbone();
            back_cnt_ref = 0;
        }
        
        auto x = calc_scores();
        //~ cout<<x.ff<<":"<<x.ss.ff<<","<<x.ss.ss<<endl;
        if (BACKBONE_COST[x.ss.ff][x.ss.ss]*C_B+C_R <= 0) break;
        
        BB -= BACKBONE_COST[x.ss.ff][x.ss.ss]*C_B+C_R;
        if (BB < 0) break;
        ROUTER[x.ss.ff][x.ss.ss] = 1;
        RECT_COVERED[x.ss.ff-(x.ss.ff % DELTA_SKIP)][x.ss.ss-(x.ss.ss % DELTA_SKIP)] = 1;
        for (int a = -R; a <= R; a++)
            for (int b = -R; b <= R; b++)
                if (x.ss.ff+a >= 0 && x.ss.ff+a < H && x.ss.ss+b >= 0 && x.ss.ss+b < W){
                    if (COVERED[x.ss.ff+a][x.ss.ss+b] || PLANE[x.ss.ff+a][x.ss.ss+b] != '.') continue;
                    if (!theres_a_wall(min(x.ss.ff+a,x.ss.ff),min(x.ss.ss+b,x.ss.ss),max(x.ss.ff+a,x.ss.ff),max(x.ss.ss+b,x.ss.ss)))
                        ANS += 1000;
                        COVERED[x.ss.ff+a][x.ss.ss+b] = 1;
                }
        cout<<"BB="<<BB<<endl;
        queue<ii> q;
        q.push(x.ss);
        ii x1 = q.front(), x2 = mp(-1,-1);
        if (!BACKBONE[x1.ff][x1.ss]){
            memset(BACKBONE_TMP, 0, sizeof BACKBONE_TMP);
            BACKBONE_TMP[x1.ff][x1.ss] = 1;
            while (!q.empty()){
                x1 = q.front();
                q.pop();
                for (int i = -1; i <= 1; i++)
                    for (int j = -1; j <= 1; j++)
                        if (x1.ff+i >= 0 && x1.ff+i < H && x1.ss+j >= 0 && x1.ss+j < W){
                            if (!BACKBONE_TMP[x1.ff+i][x1.ss+j]){
                                if (BACKBONE[x1.ff+i][x1.ss+j]){
                                    x2 = mp(x1.ff+i, x1.ss+j);
                                    break;
                                }
                                BACKBONE_TMP[x1.ff+i][x1.ss+j] = 1;
                                q.push(mp(x1.ff+i,x1.ss+j));
                            }
                        }
                if (x2.ff != -1)
                    break;
            }
        } else
            x2 = x.ss;
        a = x.ss.ff, b = x.ss.ss;
        queue<ii> new_back;
        while (!BACKBONE[a][b]){
            BACKBONE[a][b] = 1;
            new_back.push(mp(a,b));
            BACKBONE_COST[a][b] = 0;
            if (a > x2.ff) a--;
            if (a < x2.ff) a++;
            if (b > x2.ss) b--;
            if (b < x2.ss) b++;
        }
        quick_calc_backcost(new_back);
    }
    
    //~ vii pts;
    //~ for (int i = 0; i < H; i++)
        //~ for (int j = 0; j < W; j++)
            //~ if (mp(i,j)==mp(backinit[0],backinit[1]) || ROUTER[i][j])
                //~ pts.pb(mp(i,j));
    //~ vector<pair<int, ii> > edge;
    //~ for (int i = 0; i < pts.size(); i++)
        //~ for (int j = i+1; j < pts.size(); j++)
            //~ edge.pb(mp(max(abs(pts[i].ff-pts[j].ff), abs(pts[i].ss-pts[j].ss)),mp(i,j)));
    //~ sort(edge.begin(), edge.end());
    
    fix_backbone();
    
    int backcnt = -1, rtrcnt = 0;
    
    for (int i = 0; i < H; i++)
        for (int j = 0; j < W; j++){
            if (!ROUTER[i][j]) continue;
            rtrcnt++;
    }
    
    for (int i = 0; i < H; i++)
        for (int j = 0; j < W; j++)
            if (BACKBONE[i][j]) backcnt++;
            
    //~ for (int i = 0; i < H; i++){
        //~ for (int j = 0; j < W; j++)
            //~ cout<<BACKBONE[i][j];
        //~ cout<<endl;
    //~ }
    BACKBONE[backinit[0]][backinit[1]] = 0;
    
    cout<<"ANS:"<<ANS<<endl;
    cout<<"Router:"<<rtrcnt<<endl;
    cout<<"Backbone:"<<backcnt<<endl;
    if (B - (rtrcnt * C_R + backcnt * C_B) < 0){
        cerr<<"Dafaq B is negative"<<endl;
        exit(0);
    }
    cout<<FILE_OUT<<" "<<ANS + (B - (rtrcnt * C_R + backcnt * C_B))<<endl;
    
    //~ cout<<"Now i'll print the score (a+param**2) and the fileout"<<endl;
    //~ cout<<FILE_OUT<<" "<<a+param*param<<endl;
    
    //~ freopen("debug.txt","w",stdout);
    //~ memcpy(PLANE_TMP, PLANE, sizeof PLANE);
    //~ for (int i = 0; i < H; i++)
        //~ for (int j = 0; j < W; j++){
            //~ if (BACKBONE[i][j]) PLANE_TMP[i][j] = '*';
            //~ if (ROUTER[i][j]) PLANE_TMP[i][j] = 'O';
        //~ }
    //~ PLANE_TMP[backinit[0]][backinit[1]] = 'X';
    //~ for (int i = 0; i < H; i++)
        //~ cout<<PLANE_TMP[i]<<endl;
    
    // Solution printing
    freopen(FILE_OUT.c_str(),"w",stdout);
    printf("%d\n", backcnt);
    queue<ii> q;
    q.push(mp(backinit[0],backinit[1]));
    while (!q.empty()){
        auto x = q.front(); q.pop();
        for (int i = -1; i <= 1; i++)
            for (int j = -1; j <= 1; j++)
                if (x.ff+i >= 0 && x.ff+i < H && x.ss+j >= 0 && x.ss+j < W){
                    if (BACKBONE[x.ff+i][x.ss+j])
                        BACKBONE[x.ff+i][x.ss+j] = 0,
                        printf("%d %d\n", x.ff+i, x.ss+j),
                        q.push(mp(x.ff+i,x.ss+j));
                }
    }
    printf("%d\n", rtrcnt);
    for (int i = 0; i < H; i++)
        for (int j = 0; j < W; j++)
            if (ROUTER[i][j])
                printf("%d %d\n", i, j);
    return 0;
}
