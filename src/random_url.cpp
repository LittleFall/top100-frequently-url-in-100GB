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
inline char random_url_char(){return alphabet[rand_gen()&31];}
inline unsigned random_len_limit(){return (rand_gen()&7)+1;} 
inline unsigned random_path_layer(){return (rand_gen()&3)+1;}

const unsigned buf_limit = (1<<20) * 100; //buf_size 100MB
char buf[buf_limit + 200];
unsigned buf_point = 0;

inline void add_char(char ch)
{
	buf[buf_point++] = ch;
}
inline void add_random_scheme()
{
	for(auto ch:schemes[rand_gen()&3])
		add_char(ch);
}
inline void add_random_TLD()
{
	for(auto ch:TLDs[rand_gen()&31])
		add_char(ch);
}
inline void add_random_string()
{
	unsigned len = random_len_limit();
	while(len--)
		add_char(random_url_char());
}

int main(void)
{
	clock_t t_st = clock();
	freopen("../data/data10gb_rand.txt", "w", stdout);

	using ll = long long;
	ll total_byte_need = 1ll*1000*1000*1000*10; //don't use unsigned, in ubuntu

	for(; total_byte_need>0; total_byte_need -= buf_point)
	{
		ll byte_need = std::min(ll(buf_limit), total_byte_need);

		std::cerr << byte_need << " ";
		std::cerr << total_byte_need << "\n";
		buf_point = 0;
		while(buf_point < byte_need)
		{
			add_random_scheme(); add_char(':'); add_char('/'); add_char('/');
			add_random_string(); add_char('.');
			add_random_string(); add_char('.');
			add_random_TLD();

			unsigned path_layer = random_path_layer();
			while(path_layer--)
			{
				add_char('/');
				add_random_string();
			}
			add_char('\n');
		}

		buf[buf_point] = 0;
		printf("%s", buf);
	}
	fprintf(stderr, "%.2fs\n", double(clock()-t_st)/CLOCKS_PER_SEC );


	return 0;
}