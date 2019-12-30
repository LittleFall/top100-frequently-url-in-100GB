/*
random_url.cpp 生成随机url
http
*/
#include<bits/stdc++.h>
using std::string;
using std::vector;

/* 顶级域名 */
const vector<string> TLDs = {
	"top", "com", "cn", "xyz", "xin", "vip", 
	"red", "net", "org", "wang", "gov", "edu", 
	"mil", "co", "biz", "name", "info", "mobi", 
	"pro", "travel", "club", "museum", "int", "aero", 
	"post", "rec", "asia", "win"
};
/* 协议 */
const vector<string> schemes = {
	"http", "https", "ftp"
};
/* 字符表 */
const string alphabet = "0123456789abcdefghijklmnopqrstuvwxyz"; // no '-'

const unsigned string_len_limit = 10;
const unsigned path_layer_limit = 5;


std::mt19937 rand_gen(time(0));
inline string random_string()
{
	string str;
	unsigned len = rand_gen()%string_len_limit;
	for(unsigned i=0; i<len; ++i)
		str.push_back(alphabet[rand_gen()%alphabet.size()]);
	return str;
}

template<typename Iter>
inline Iter select_randomly(Iter start, Iter end) {
	return start + rand_gen()%(end-start);
}


int main(void)
{
	clock_t t_st = clock();

	std::ofstream fout("../data/data100mb_rand.txt");
	int total_byte_last = 100*1024*1024; //don't use unsigned


	while(total_byte_last>0)
	{
		string url;
		url += *select_randomly(schemes.begin(), schemes.end());
		url += "://";
		url += random_string() + "." + random_string() + ".";
		url += *select_randomly(TLDs.begin(), TLDs.end());
		unsigned path_layer = rand_gen() % path_layer_limit;
		while(path_layer--)
			url += "/" + random_string();
		fout << url << "\n";      
		total_byte_last -= url.size();
		//printf("%d\n",total_byte_last );
	}

	printf("%.2fs\n",double(clock()-t_st)/CLOCKS_PER_SEC );

	return 0;
}