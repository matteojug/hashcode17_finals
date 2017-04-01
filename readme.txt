Foo 


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
