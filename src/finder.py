'''
finder.py 从网络上爬取url
使用队列维护已经发现的url，多线程处理队首的url
对于每个url，得到网页后提取出所有href指向的url，再加入到队列中
当已发现的url达到目标上限时停止
'''
import requests
import urllib
import threading
import bs4
import queue
import time

'''
href中提取出的url通常有以下情况：
1. 带有协议的完整url，如href="https://www.baidu.com"，此时可以直接使用
2. 以双斜杠开头，需要补全协议的，如href="//www.baidu.com"，此时以原网页的协议补全
3. 以单斜杠开头，需要补全协议与域名的，如href="/events"，此时以原网页的协议和域名补全
4. 其他情形，如href="javascript:;"，这样的href不做url记录

补全函数的处理逻辑是，如果能补全就补全，不能的话就不修改。
返回后通过检查是否以"http"开头来判断是否是合法的url.
'''
def complete_url(candidate, parse):
	candidate.strip()
	if(candidate.startswith("//")):
		candidate = parse.scheme + ":" + candidate
	if(candidate.startswith("/")):
		candidate = parse.scheme + "://" + parse.netloc + candidate
	return candidate

'''
请求网页，返回href集合以及url的解析
1. 使用requests请求网页，如果0.3s未成功返回，认为此url无效，返回空。
2. 使用BeautifulSoup分析网页，提取出所有的href项.
3. 使用urllib.parse解析url字符串，主要目的是得到协议、域名等部分，方便填充url.
'''
def get_parse_and_hrefs_from_url(url):
	if url.endswith(".apk"):
		return "", {}
	try:
		req_obj = requests.get(url, timeout = 0.3)
		bresp = bs4.BeautifulSoup(req_obj.text,'lxml')
		hrefs = bresp.find_all(lambda tag: tag.has_attr('href'))

		parse = urllib.parse.urlparse(url)
		return parse, hrefs
	except requests.exceptions.RequestException:
		return "", {}

'''
每个线程的处理逻辑：
1. 从队列中取得url，调用以上两个函数，得到若干新url
2. 如果新url合法，就将其记录到文件中，并且给计数器加一
3. 当计数达到上限时，函数返回，表示这个线程工作执行完毕
4. 否则，如果队列空间充足，就将新url添加到队列中
5. 当计数每增加一个检查量(checkunit)时，在屏幕上打印计数，便于了解运行进度
'''
def worker():
	global cnt
	while True:
		url = q.get()
		#print("url =", url)
		parse, hrefs = get_parse_and_hrefs_from_url(url)

		for href in hrefs:
			new_url = href['href'];
			new_url = complete_url(new_url, parse)
			if(new_url.startswith("http")):
				fo.write(new_url+"\n")
				cnt += 1
				if(cnt > limit):
					return
				if(q.qsize() < qlimit):
					q.put(new_url)
				if(cnt % checkunit == 0):
					print(cnt)

'''
全局变量
数据结构：队列
计数器，计数上限，检查量，队列上限
'''
q = queue.Queue() 
cnt, limit, checkunit, qlimit = 0, 2000, 1000, 1000000

'''
主函数
1. 计时
2. 打开文件
3. 向队列中添加遍历起点
4. 设置线程数，启动线程，阻塞
5. 关闭文件，输出计时
'''
def main():
	time_start=time.time()

	fo = open("../data/data100kb.txt", "w")
	q.put("https://www.baidu.com") # init url

	thread_nums = 8
	threads = []
	for i in range(thread_nums):
		t = threading.Thread(target = worker)
		t.start()
		threads.append(t)

	for t in threads:
		t.join()

	fo.close()

	print("time cost =", time.time() - time_start)

if __name__ == '__main__':
    main()