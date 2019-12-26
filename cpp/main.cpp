#include <bits/stdc++.h>
using std::string;

const string data_path = "../data_finder/data100mb.txt";
const string block_path = "../solver/block";
const unsigned radix = 31, blocks = 131, limit = 100;

unsigned count[blocks];
unsigned hash(const string &url) //获得url的hash值，范围是[0,blocks-1]
{
	unsigned res = 0;
	for(char ch:url)
		res = (res * radix + unsigned(ch)) % blocks;
	return res;
}
std::ofstream ofs[blocks];
void init()
{
	for(unsigned i=0; i<blocks; ++i)
		ofs[i] = std::ofstream(block_path + std::to_string(i));
}
int main(void)
{
	clock_t t_st = clock();
	init();
	std::ifstream fin(data_path);

	string url;
	while(getline(fin, url))
	{
		unsigned hash_val = hash(url);
		ofs[hash_val] << url << "\n";
		++count[hash_val];
	}
	printf("classify cost = %.2fs\n",double(clock()-t_st)/CLOCKS_PER_SEC );

	using psu = std::pair<string, unsigned>;
	auto big_top = [](psu &a, psu &b){return a.second < b.second;};
	auto little_top = [](psu &a, psu &b){return a.second > b.second;};
	std::vector<psu> total_heap(limit+1);

	make_heap(total_heap.begin(), total_heap.end(), little_top);
	for(unsigned i=0; i<blocks; ++i)
	{
		std::ifstream fin(block_path + std::to_string(i));
		string url;
		std::unordered_map<string, unsigned> counter;
		while(getline(fin, url))
			++counter[url];

		std::vector<psu> heap(counter.begin(), counter.end());

		std::make_heap(heap.begin(), heap.end(), big_top);
		for(unsigned j=0; j<std::min(limit, (unsigned)heap.size()); ++j)
		{
			if(heap.front().second <= total_heap.front().second) break;
			total_heap.back() = heap.front(); //新来一个放在100处
			push_heap(total_heap.begin(), total_heap.end(), little_top); //插入堆，最小一个在0处
			pop_heap(total_heap.begin(), total_heap.end(), little_top); //把最小的放在100处

			//printf("%d ",heap.front().second);
			std::pop_heap(heap.begin(), heap.end(), big_top);
			heap.pop_back();
		}
	}
	//printf("\n");
	total_heap.pop_back();
	sort(total_heap.begin(), total_heap.end(), little_top);
	// for(auto p:total_heap)
	// 	printf("%d %s\n", p.second, p.first.c_str() );
	// printf("\n");

	printf("total cost = %.2fs\n",double(clock()-t_st)/CLOCKS_PER_SEC );
    return 0;
}
