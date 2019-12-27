/**
main.cpp 
从文件中读取数据，并且将结果（出现最频繁的100个url）输出在屏幕上
算法流程：
1. 将大文件的所有url按hash值分类，放入若干个小文件中，每个hash值对应一个小文件；
2. 对于每个小文件，使用hashmap统计每种url的出现次数；
3. 将每个小文件建立大顶堆，取得前100个出现频率最大的url，称为候选url；
4. 使用小顶堆维护全局的前100个出现频率最大的url，初始全为0。
5. 将每个候选url的出现次数与小顶堆中的堆顶（最小值）相比较，如果比堆顶大，就将堆顶扔掉，把这个候选url放入堆中。
*/
#include <bits/stdc++.h>
using std::string;

/**
预设的全局变量：
data_path 表示数据路径
block_path 表示存放小文件的路径前缀（之后接数字编号）

radix hash时的进制
blocks 小文件个数，也是hash时的模数；为了hash尽量平均，需要选择一个质数；为了保证内存限制，可以大一些。
limit 要筛选的出现频率最大的url个数
*/
const string data_path = "../data/data100gb.txt";
const string block_path = "../tmp/block";
const unsigned radix = 31, blocks = 13331, limit = 100;

/**
哈希函数，获得字符串url的hash值，值域是[0,blocks-1]
哈希方法是将字符串视为radix进制的整数，求其对blocks的模
*/
unsigned hash(const string &url) 
{
	unsigned res = 0;
	for(char ch:url)
		res = (res * radix + unsigned(ch)) % blocks;
	return res;
}

/**
将大文件分类到小文件中
首先将小文件都创建完毕，然后依次将url按hash值放入各自对应的小文件中
返回的count数组用于记录各个文件的信息
*/
std::vector<unsigned> classify_to_blocks()
{
	std::ifstream fin(data_path);
	if(fin.fail())
	{
		printf("data_path error: %s\n",data_path.c_str());
		exit(1);
	}
	std::vector<std::ofstream> ofs(blocks);
	for(unsigned block_id=0; block_id<blocks; ++block_id)
	{
		string block_id_path = block_path + std::to_string(block_id);
		ofs[block_id].open(block_id_path);
		if(ofs[block_id].fail())
		{
			printf("block_path error: %s\n",block_id_path.c_str() );
			exit(1);
		}
	}

	string url;
	std::vector<unsigned> block_count(blocks);
	while(getline(fin, url)) if(url!="")
	{
		unsigned hash_val = hash(url);
		ofs[hash_val] << url << "\n";
		++block_count[hash_val];
	}

	fin.close();
	for(unsigned block_id=0; block_id<blocks; ++block_id)
		ofs[block_id].close();
	return block_count;
}

/**
从小文件中获得全局出现频率最大的前100个url
1. 首先记录两个函数big_top和little_top，
	分别表示按频率从小到大排序和从大到小排序，以它们为规则所建的堆分别是大顶堆和小顶堆。
2. 建立全局的小顶堆，容量为limit(100)
3. 遍历每个小文件，使用unordered_map统计出现url次数，对所有的频率数据建立大顶堆。
4. 依次取得每个小文件的频率前100大（也可能不到100个）的数据，与小顶堆中最小的元素比较并尝试替换。
5. 最后，对小顶堆按频率从大到小排序，即得到出现次数最大的前100个url
*/
std::vector<std::pair<string, unsigned>> find_top_from_blocks()
{
	using psu = std::pair<string, unsigned>;
	auto big_top = [](psu &a, psu &b){return a.second < b.second;};
	auto little_top = [](psu &a, psu &b){return a.second > b.second;};

	std::vector<psu> total_heap(limit, std::make_pair("",0)); // global max 100, little_top heap

	for(unsigned block_id=0; block_id<blocks; ++block_id)
	{
		std::ifstream fin(block_path + std::to_string(block_id));
		if(fin.fail())
		{
			printf("block_path error: %s\n",(block_path + std::to_string(block_id)).c_str());
			exit(1);
		}

		string url;
		std::unordered_map<string, unsigned> counter;

		while(getline(fin, url))
			++counter[url];

		std::vector<psu> heap(counter.begin(), counter.end()); // big_top heap
		std::make_heap(heap.begin(), heap.end(), big_top);

		for(unsigned i=0, lim = std::min(limit, (unsigned)heap.size()); 
			i<lim; ++i)
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
/**
主函数，包括调用上方函数、计时，以及输出提示信息
*/
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
