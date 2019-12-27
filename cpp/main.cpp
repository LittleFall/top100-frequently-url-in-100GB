#include <bits/stdc++.h>
using std::string;

const string data_path = "../data_finder/data100gb.txt";
const string block_path = "../solver/block";
const unsigned radix = 31, blocks = 13331, limit = 100;

unsigned hash(const string &url) //获得url的hash值，范围是[0,blocks-1]
{
	unsigned res = 0;
	for(char ch:url)
		res = (res * radix + unsigned(ch)) % blocks;
	return res;
}

std::vector<unsigned> classify_to_blocks()
{
	std::ifstream fin(data_path);
	std::vector<std::ofstream> ofs(blocks);
	for(unsigned block_id=0; block_id<blocks; ++block_id)
		ofs[block_id] = std::ofstream(block_path + std::to_string(block_id));

	string url;
	std::vector<unsigned> block_count(blocks);
	while(getline(fin, url)) if(url!="")
	{
		unsigned hash_val = hash(url);
		ofs[hash_val] << url << "\n";
		++block_count[hash_val];
	}

	fin.clear();
	for(unsigned block_id=0; block_id<blocks; ++block_id)
		ofs[block_id].clear();
	return block_count;
}

std::vector<std::pair<string, unsigned>> find_top_from_blocks()
{
	using psu = std::pair<string, unsigned>;
	auto big_top = [](psu &a, psu &b){return a.second < b.second;};
	auto little_top = [](psu &a, psu &b){return a.second > b.second;};

	std::vector<psu> total_heap(limit, std::make_pair("",0)); // global max 100, little_top heap

	for(unsigned i=0; i<blocks; ++i)
	{
		std::ifstream fin(block_path + std::to_string(i));
		string url;
		std::unordered_map<string, unsigned> counter;

		while(getline(fin, url))
			++counter[url];

		std::vector<psu> heap(counter.begin(), counter.end()); // big_top heap
		std::make_heap(heap.begin(), heap.end(), big_top);

		for(unsigned j=0, lim = std::min(limit, (unsigned)heap.size()); 
			j<lim; ++j)
		{
			if(heap.front().second <= total_heap.front().second) break;
			pop_heap(total_heap.begin(), total_heap.end(), little_top); // pop min to back
			total_heap.back() = heap.front(); // update back
			push_heap(total_heap.begin(), total_heap.end(), little_top); // push back into heap

			std::pop_heap(heap.begin(), heap.end(), big_top); 
			heap.pop_back(); 
		}
		fin.close();
	}
	
	sort(total_heap.begin(), total_heap.end(), little_top); 
	return total_heap;
}

int main(void)
{
	clock_t t_st = clock();

	printf("start.\n");
	printf("blocks = %d, hash radix = %d, find top %d\n\n", blocks, radix, limit );

	std::vector<unsigned> block_count = classify_to_blocks();
	printf("classify end.\n");
	printf("total urls: %u\n", std::accumulate(block_count.begin(), block_count.end(), 0u) );
	printf("ave urls of block: %u\n", std::accumulate(block_count.begin(), block_count.end(), 0u)/blocks );
	printf("max urls of block: %u\n", *std::max_element(block_count.begin(), block_count.end()));
	printf("classify time cost = %.2fs\n\n",double(clock()-t_st)/CLOCKS_PER_SEC );

	std::vector<std::pair<string, unsigned>> top = find_top_from_blocks();
	printf("well done.\n");
	for(auto psu:top)
		printf("%d %s\n", psu.second, psu.first.c_str() );
	printf("\n");
	printf("total time cost = %.2fs\n",double(clock()-t_st)/CLOCKS_PER_SEC );

    return 0;
}
