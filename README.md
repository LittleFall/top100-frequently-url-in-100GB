# LittleHomework
pingcap的小作业

[TOC]

### 题目：
>
> 100GBurl文件，使用1GB内存计算出出现次数top100的url和出现的次数
> 
> 提示：
> 
> 注意代码可读性，添加必要的注释（英文）
>
> 注意代码风格与规范，添加必要的单元测试与文档
>
> 注意异常处理，尝试优化性能

### 环境：
- 操作系统 ubuntu 18.04
- 编辑器/同步工具 sublime_text + sublime_merge
- 编译器：
	- Python 3.6.9
	- g++ 7.4.0, 编译选项：-Wall -std=c++14

### 目录：
- /data 存放制造好的数据
	- /data100kb.txt 约2000条，102.7KB，用于调试
	- /data100mb.txt 约2e6条，151.5MB，用于测试正确性与性能
	- /data100gb.txt 约2e9条，100.3GB，全量数据
- /src 存放源程序
	- /finder.py 从网络上爬取url
	- /repeater.py 通过已有的url不断重复，快速产生全量数据
	- /main.cpp 运行主程序，根据数据产生计算结果
- /tmp 存放中间运算结果


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
https://beautifulsoup.readthedocs.io/zh_CN/v4.4.0/#id27
https://blog.csdn.net/xuyinxin/article/details/91304688
https://segmentfault.com/q/1010000009200527/a-1020000009200809
https://www.jianshu.com/p/e7d87e1ed38c
https://docs.python.org/zh-cn/3.6/library/queue.html
https://blog.csdn.net/Iloveyougirls/article/details/81814524
http://www.cplusplus.com/reference/algorithm/make_heap/?kw=make_heap