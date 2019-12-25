#include <bits/stdc++.h>
using namespace std; using ll = long long; inline int read();
const int M = 100016, MOD = 1000000007;

int main(void)
{
	#ifdef _LITTLEFALL_
	//freopen("in.txt","r",stdin);
    #endif

	string str;
	getline(cin, str);
	for(auto ch:str)
		printf("%d ",ch );

    return 0;
}

inline int read(){
    int x=0,f=1;char ch=getchar();
    while(ch<'0'||ch>'9') {if(ch=='-')f=-1;ch=getchar();}
    while(ch>='0'&&ch<='9'){x=x*10+ch-'0';ch=getchar();}
    return x*f;
}