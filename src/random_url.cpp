/*
random_url.cpp 生成随机url
*/
#include<bits/stdc++.h>
using std::string;
using std::vector;

/* 顶级域名 */
const vector<string> TLDs = {
	"top", "com", "xyz", "xin", "vip", "red", "net", "org", 
	"wang", "gov", "edu", "mil", "co", "biz", "name", "info", 
	"mobi", "pro", "travel", "club", "museum", "int", "aero", "post",
	"rec", "asia", "win", "cn", "fr", "us", "uk", "ru"
}; 
/* 协议 */
const vector<string> schemes = {
	"http", "https", "https", "ftp"
};
/* 字符表 */
const string alphabet = "0123456789abcdefghijklmnopqrstuv"; // no "-wxyz", len 32

std::mt19937 rand_gen(time(0));
inline string random_scheme(){return schemes[rand_gen()&3];}
inline string random_TLD(){return TLDs[rand_gen()&31];}
inline char random_url_char(){return alphabet[rand_gen()&31];}
inline unsigned random_len_limit(){return (rand_gen()&7)+1;} 
inline unsigned random_path_layer(){return (rand_gen()&3)+1;}

inline string random_string()
{
	string str;
	unsigned len = random_len_limit();
	for(unsigned i=0; i<len; ++i)
		str.push_back(random_url_char());
	return str;
}

string buf;
int main(void)
{
	clock_t t_st = clock();

	std::ofstream fout("../data/data100mb_rand.txt");
	unsigned long long total_byte_last = 100*1024*1024; 

	while(buf.size() < total_byte_last)
	{
		buf += random_scheme() + "://";
		buf += random_string() + "." + random_string() + ".";
		buf += random_TLD();

		unsigned path_layer = random_path_layer();
		while(path_layer--)
			buf += "/" + random_string();

		buf += "\n";
	}
	fout << buf << "\n";      

	printf("%.2fs\n",double(clock()-t_st)/CLOCKS_PER_SEC );

	return 0;
}