# top100-frequently-url-in-100GB
100GBurl文件，使用1GB内存计算出出现次数top100的url和出现的次数

### 环境
- 操作系统 ubuntu 18.04
- 编辑器/同步工具 sublime_text + sublime_merge
- 编译器：
	- Python 3.6.9
	- g++ 7.4.0, 编译选项：-Wall -std=c++14

##### 文件限制
ubuntu默认同时最多打开的文件句柄是1024，而程序中要同时用到一万多个文件句柄，导致运行失败.

解决方法见[Ubuntu 永久修改 ulimit -n](https://learnku.com/articles/21457)

### 目录
- /data 存放制造好的数据
	- /data100kb.txt 约2000条，102.7KB，用于调试
	- /data100mb.txt 约2e6条，151.5MB，用于测试正确性与性能
	- /data100gb.txt 约2e9条，100.3GB，全量数据
- /src 存放源程序
	- /finder.py 从网络上爬取url
	- /repeater.py 通过已有的url不断重复，快速产生全量数据
	- /main.cpp 运行主程序，根据数据产生计算结果
- /tmp 存放中间运算结果

### 算法流程
##### 构造数据
1. 选定一个起点url，加入队列中；
2. 从队首取得一个url，获得网页HTML代码，遍历所有href，将合法的url加入到队列中；
3. 当已经发现的url数量达到目标时停止，否则返回第二步；
4. 考虑到100GB实在太大，爬到200MB左右的数据时停止，不断进行自身复制得到100GB的数据。

##### 选择算法
1. 将大文件的所有url按hash值分类，放入若干个小文件中，每个hash值对应一个小文件；
2. 对于每个小文件，使用hashmap统计每种url的出现次数；
3. 将每个小文件建立大顶堆，取得前100个出现频率最大的url，称为候选url；
4. 使用小顶堆维护全局的前100个出现频率最大的url，初始全为0。
5. 将每个候选url的出现次数与小顶堆中的堆顶（最小值）相比较，如果比堆顶大，就将堆顶扔掉，把这个候选url放入堆中。
6. 最后把小顶堆中所有url按频率从大到小排好序，就是所要求的答案。

### 已实现的优化
1. 获得url时，有些url是apk文件，对其调用request.get方法会很慢，在程序中通过判断忽略掉了这类文件；
2. 获得url时，使用多线程，显著增加执行速度；
3. 大文件分类到小文件时，预先把所有输出流准备好，而不是每写一次都打开关闭一次流。
4. 小顶堆只保留100个元素.
5. 小文件的大顶堆从大到小试图插入小顶堆时，只要出现第一个插入失败，就可以直接break，因为之后肯定会更小。

### 复杂度分析
用cnt表示url总条数，blocks表示小文件个数。

1. 获得url时涉及到网络通信，时间复杂度是O(cnt) * 网络通信的平均时间
2. 复制url时，仅在尾部追加已有内容，时间复杂度O(cnt)，磁盘空间复杂度O(cnt)
3. 大文件分类到小文件时，时间复杂度和磁盘空间复杂度都是O(cnt + blocks)，内存空间复杂度是O(blocks)
4. 小文件找到100个频率最大的url时，时间复杂度是O(cnt + 100 * blocks * log (cnt/blocks))， 内存空间复杂度与每个小文件中的不同url种数成正比，hash足够平均时，是O(cnt/blocks).
5. 找到全局100个频率最大的url时，时间复杂度是O(blocks * 100 * log 100)，空间复杂度O(100).
6. 综上，平均内存空间复杂度是O(max(cnt/blocks,blocks)),在cnt=2e9,blocks=13331的情况下，不会超过内存限制


### TODO
1. C++考虑多线程实现
2. 内存限制仅在复杂度分析中理论上证明满足，以及在运行过程中用top命令观察不超限，没有找到合适的直接输出最大内存占用的方法。

### 运行结果
```
start.
blocks = 13331, hash radix = 31, find top 100

classify end.
total urls: 2111614819
ave urls of block: 158398
max urls of block: 22973882
classify time cost = 1767.57s

well done.
22919119 https://ai.baidu.com/customer
17133441 https://ai.baidu.com/tech/agriculture
15815484 https://ai.baidu.com/tech/industry
11960410 https://ai.baidu.com/tech/imagecensoring
9274013 http://di.baidu.com/#products
7940850 https://ai.baidu.com/solution/facegate
7913716 https://ai.baidu.com/solution/private
7321241 http://list.video.baidu.com/iph_promote.html
7119312 https://ai.baidu.com/solution/cvpaas
6636453 https://ai.baidu.com/solution/faceprint
5670623 https://ai.baidu.com/solution/censoring
5660510 http://www.baidu.com/duty/
5455255 http://v.baidu.com/
5303845 https://ai.baidu.com/solution/faceattendance
5111121 https://aim.baidu.com/product/search?productType=SOFTWARE
5097019 https://aim.baidu.com/product/search?productType=HARDWARE
5097019 https://aim.baidu.com/product/search?productType=SOLUTION
5097019 https://aim.baidu.com/product/search?productType=DATA
4941243 https://aim.baidu.com/
4775156 http://v.baidu.com/channel/star
4775156 http://v.baidu.com/channel/amuse
4434079 https://ai.baidu.com/tech/unit
4364582 https://aim.baidu.com/product/7040880d-4d04-4667-97b7-ecafdf9b7609
4334202 https://ai.baidu.com/tech/intelligentwriting
4161909 http://qingting.baidu.com/index?pid=4
4052100 https://ai.baidu.com/tech/speech/tts
4038379 http://v.baidu.com/channel/shaoer
4022228 https://ar.baidu.com/interactive
4022228 https://ar.baidu.com/aidevice
4017888 https://ar.baidu.com/education
4017888 https://ar.baidu.com/car
3994568 https://ai.baidu.com/solution/bsic
3986975 https://ai.baidu.com/solution/faceidentify
3974486 https://ai.baidu.com/solution/kgaas
3971787 https://ai.baidu.com/tech/vr
3964182 https://ai.baidu.com/solution/factory
3959843 https://ai.baidu.com/solution/defectdetect
3959843 https://ai.baidu.com/solution/bmvs
3380799 https://ai.baidu.com/support/video
3379210 https://aistudio.baidu.com/aistudio/education
3379210 https://aistudio.baidu.com/aistudio/course
3379210 https://ai.baidu.com/support/news?action=detail&id=1303
3363679 http://v.baidu.com/top/
3296169 https://ai.baidu.com/sdk
3144869 https://ai.baidu.com/easydl/
3122637 https://ai.baidu.com/tech/smartasr
3118266 https://ai.baidu.com/partner
3099798 https://ai.baidu.com/accelerator
3073780 https://ai.baidu.com/tech/ocr/iocr
3052049 https://ai.baidu.com/easydl/lite
3036887 https://ai.baidu.com/tech/iocr
3034159 https://ai.baidu.com/tech/ocr/iocr_finance
3019506 https://ai.baidu.com/easyedge/home
3007563 https://ai.baidu.com/support/news?action=detail&id=1603
3007562 https://ai.baidu.com/support/news?action=detail&id=1498
3007562 https://ai.baidu.com/support/news?action=detail&id=1497
2980905 https://ai.baidu.com/forum
2854669 https://ai.baidu.com/tech/body/skeleton-sdk
2849181 http://news.baidu.com/
2824150 https://console.bce.baidu.com/?fromai=1#/aip/overview
2759055 https://aim.baidu.com/product/5b8d8817-9141-4cfc-ae58-640e2815dfd4
2738871 https://ai.baidu.com/
2708618 https://ai.baidu.com/tech/hardware/xunxiang
2704271 https://ar.baidu.com/aimarket
2698299 https://ar.baidu.com/attraction
2689083 https://ai.baidu.com/customer/huazhishuidao
2685839 https://ai.baidu.com/tech/video/vcr
2676057 https://aim.baidu.com/product/83d5eab9-a414-4dc7-afe8-8c8603e446aa
2672815 https://ai.baidu.com/tech/vehicle
2669011 https://cloud.baidu.com/product/abc-robot.html
2666322 https://ai.baidu.com/customer/jiankewang
2663060 https://ai.baidu.com/customer/shougangjituan
2650029 https://ai.baidu.com/solution/ivs
2650019 http://apollo.auto/parksolution_cn.html
2647313 https://ai.baidu.com/solution/roboticvision
2647308 https://ai.baidu.com/solution/aifenzhen
2646215 https://aim.baidu.com/provider/c02562e7-a15a-4c60-a537-2cd3d5a63a1c
2645142 http://fanyi-api.baidu.com/api/trans/product/prodinfo#4
2644599 https://ai.baidu.com/solution/class
2641887 https://ai.baidu.com/tech/hardware/speechkit
2641886 https://ai.baidu.com/solution/ecomment
2641886 https://ai.baidu.com/solution/virtualassistant
2641886 https://ai.baidu.com/solution/display
2641886 https://ai.baidu.com/solution/cashier
2641886 https://aim.baidu.com/product/search?productTag=40005
2641886 https://ai.baidu.com/solution/recruitment
2641886 https://ai.baidu.com/solution/cabinet
2641886 https://ai.baidu.com/solution/mtp
2641886 https://ai.baidu.com/solution/monitor_indoor
2639169 https://ai.baidu.com/customer/wps
2638087 https://ai.baidu.com/customer/hcb
2635914 https://aim.baidu.com/provider/dc49c1ee-3d11-47f4-beb4-35a23d2cb210
2635914 https://aim.baidu.com/provider/e312b615-3c32-42cc-9f77-b21186c2f16d
2576675 http://v.baidu.com/tv
2518169 http://v.baidu.com/musicindex/
2507876 http://home.baidu.com/
2434575 http://v.baidu.com/show
2434575 http://v.baidu.com/movie
2433491 http://v.baidu.com/comic
2414043 http://top.baidu.com/

total time cost = 3041.11s
```

### 参考链接
- https://beautifulsoup.readthedocs.io/zh_CN/v4.4.0/#id27

- https://blog.csdn.net/xuyinxin/article/details/91304688

- https://segmentfault.com/q/1010000009200527/a-1020000009200809

- https://docs.python.org/zh-cn/3.6/library/queue.html

- https://blog.csdn.net/Iloveyougirls/article/details/81814524

- http://www.cplusplus.com/reference/algorithm/make_heap/?kw=make_heap