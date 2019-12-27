# top100-frequently-url-in-100GB
100GBurl文件，使用1GB内存计算出出现次数top100的url和出现的次数

### 环境
- 操作系统 ubuntu 18.04
- 编辑器/同步工具 sublime_text + sublime_merge
- 编译器：
	- Python 3.6.9
	- g++ 7.4.0, 编译选项：-Wall -std=c++14

##### 文件限制
我的ubuntu默认同时最多打开的文件句柄是1024，而程序中要同时用到一万多个文件句柄，导致程序会出现bug.

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


### TODO
1. 异常处理，如创建文件失败等
2. C++考虑多线程实现

### 运行结果
```
start.
blocks = 13331, hash radix = 31, find top 100

classify end.
total urls: 2111614819
ave urls of block: 158398
max urls of block: 22973882
classify time cost = 1319.15s

well done.
22919119 https://ai.baidu.com/customer
17133441 https://ai.baidu.com/tech/agriculture
5455255 http://v.baidu.com/
5097019 https://aim.baidu.com/product/search?productType=DATA
3986975 https://ai.baidu.com/solution/faceidentify
3971787 https://ai.baidu.com/tech/vr
3959843 https://ai.baidu.com/solution/bmvs
3034159 https://ai.baidu.com/tech/ocr/iocr_finance
2980905 https://ai.baidu.com/forum
2738871 https://ai.baidu.com/
2676057 https://aim.baidu.com/product/83d5eab9-a414-4dc7-afe8-8c8603e446aa
2641886 https://ai.baidu.com/solution/recruitment
2414043 http://top.baidu.com/
2011533 http://v.baidu.com/channel/short/newlvyou
1734038 http://di.baidu.com/product/mike
1734038 http://di.baidu.com/product/xuanke
1710756 http://paddlepaddle.org.cn/feature
1700460 https://ai.baidu.com/partner/data
1689605 https://ai.baidu.com/docs#/QuickStart-Review
1689605 https://aistudio.baidu.com/aistudio/competition
1689605 https://ai.baidu.com/docs#/Pricing/top
1689605 https://aistudio.baidu.com/aistudio/datasetoverview
1429711 http://e.baidu.com?refer=1009
1373257 https://ai.baidu.com/support/news?action=detail&id=435
1340752 https://ai.baidu.com/tech/imageprocess/dehaze
1333160 https://ai.baidu.com/tech/imagesearch/product
1330445 https://ai.baidu.com/tech/ocr_cars/vehicle_license
1329894 https://ai.baidu.com/customer/dida
1328813 https://ai.baidu.com/tech/bicc
1328811 https://ai.baidu.com/solution/servicerobot
1328808 https://ai.baidu.com/customer/lhqdj
1327184 http://dumix.baidu.com/coretech
1326644 https://ai.baidu.com/tech/ocr_receipts/quota_invoice
1317957 https://aim.baidu.com/provider/9a7bde21-71d7-4677-93b4-717dda9cf7c6
1317957 https://aim.baidu.com/provider/e9e59229-32cb-4c0c-9c40-cc108a40ae5d
1317957 http://di.baidu.com/solution/retail?castk=LTE%3D
1317957 https://aim.baidu.com/provider/b2e238cb-074f-481b-87db-aab78a42e926
1317957 https://ai.baidu.com/customer/taicd
1317957 https://aim.baidu.com/provider/4174ed38-c4d5-48ab-83d5-a708220df5de
1090112 http://dueros.baidu.com/html/2017/jjfasj_0608/7.html
1021306 http://news.baidu.com/finance
1021306 http://news.baidu.com/house
1021306 http://news.baidu.com/lady
985436 http://duer.baidu.com/html/2017/jjfasj_0608/7.html
946309 https://ai.baidu.com/tech/nlp_custom
852079 https://www.baidu.com/cache/icon/favicon.ico
733798 http://www.zhibo8.cc/shouji.htm
664310 http://www.hao123.com/health
664310 http://www.hao123.com/fangchan
601858 http://report.12377.cn:13225/toreportinputNormal_anis.do
556996 http://duer.baidu.com/
532070 https://b2b.baidu.com/s?&fr=wenku&key=word
526544 http://ai.baidu.com/
510653 http://news-bos.cdn.bcebos.com/mvideo/pcnews_licence.html
502031 http://game.hao123.com/vr
484674 http://wy.hao123.com/top
483121 http://vip.hao123.com
445869 https://wenku.baidu.com/ndbgc/channel/fitnesscoach?ch=fitnesscoach
445869 https://wenku.baidu.com/activity/browse/yunxuerd?ch=yunxuerd
439883 http://wenku.baidu.com/cashier/browse/dispatch?dqStatCode=topnav_joinvip
414480 http://m.cnr.cn/ttpd/sogoudh/xbjx/20191213/t20191213_524896883.html
406645 http://v.baidu.com/channel/short/newbomb
406645 http://v.baidu.com/channel/short/newsong
397938 http://home.baidu.com/home/index/company
390556 http://wenku.baidu.com/org/zone?zoneid=2
381880 http://wenku.baidu.com/ndbgc/org/legal?fr=syjp
381069 http://www.188bifen.com/
370621 http://v.baidu.com/tv/29384.htm?frp=browse
370621 http://v.baidu.com/tv/29341.htm?frp=browse
369020 http://wenku.baidu.com/p/ÍÈÁëÌ½Ñ°¿åµ¹
366014 http://tianqi.hao123.com/
326667 http://wenku.baidu.com/topic/ixdc.html
321244 http://wenku.baidu.com/topic/nzgzzj.html
319314 http://y.taihe.com/artist/1925
311850 http://www.hao123.com/tianqi
301659 https://itunes.apple.com/cn/app/id468623917?mt=8
289176 http://music.baidu.com/mv
271235 http://di.baidu.com/solution/log
271235 http://di.baidu.com/solution/lot
268325 https://dianshi.baidu.com/
268325 http://e.baidu.com?refer=1006
264820 http://v.baidu.com/show/23628.htm?frp=browse
260475 http://v.baidu.com
255942 https://vip.jd100.com/zt/stepactive/?c=wenku7&grade=%E9%AB%98%E4%BA%8C&page=2&day=652675&play=2
240657 http://v.baidu.com/tv/29383.htm?frp=browse
229509 https://ai.baidu.com/docs
225168 http://m.cnr.cn/ttpd/sogoudh/xbjx/20191217/t20191217_524901371.html
209047 http://music.taihe.com/doc/help
202356 http://www.qstheory.cn
202272 http://s0.hao123img.com/resource/fe/widget/ui/header/common/v2/logo/logo.07657246b.css
182008 http://v.baidu.com/show/1745.htm?frp=browse
180608 http://m.cnr.cn/ttpd/sogoudh/xbjx/20191213/t20191213_524896879.html
179203 http://yuedu.baidu.com/
177009 http://music.taihe.com/download/index.html?pst=web_fixed_right_btn
168153 http://v.hao123.baidu.com/dianying
160622 https://wenku.baidu.com/qiushi
160622 https://edu-xtopic.bdimg.com/v1/topic/output691.html
155378 http://v.baidu.com/comic/22105.htm?frp=browse
151164 http://v.baidu.com/movie/138186.htm?frp=browse
149235 http://wenku.baidu.com/p/yangkuiwenku

total time cost = 1426.80s
```

### 参考链接
- https://beautifulsoup.readthedocs.io/zh_CN/v4.4.0/#id27

- https://blog.csdn.net/xuyinxin/article/details/91304688

- https://segmentfault.com/q/1010000009200527/a-1020000009200809

- https://docs.python.org/zh-cn/3.6/library/queue.html

- https://blog.csdn.net/Iloveyougirls/article/details/81814524

- http://www.cplusplus.com/reference/algorithm/make_heap/?kw=make_heap