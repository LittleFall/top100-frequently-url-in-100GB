import requests
import urllib
import threading
import bs4
import queue
import time
	
# 双斜杠开头，补全协议
# 单斜杠开头，补全协议与域名
# http开头，不进行操作
# 其他，不收录
def complete_url(candidate, parse):
	candidate.strip()
	if(candidate.startswith("//")):
		candidate = parse.scheme + ":" + candidate
	if(candidate.startswith("/")):
		candidate = parse.scheme + "://" + parse.netloc + candidate
	return candidate

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

def worker():
	global cnt
	while True:
		url = q.get()
		#print("url =", url)
		parse, hrefs = get_parse_and_hrefs_from_url(url)

		for href in hrefs:
			string = href['href'];
			string = complete_url(string, parse)
			if(string.startswith("http")):
				fo.write(string+"\n")
				cnt += 1
				if(cnt > limit):
					return
				if(q.qsize() < qlimit):
					q.put(string)
				if(cnt % 1000 == 0):
					print(cnt)


fo = open("data100gb.txt", "w")
q = queue.Queue() # 队列
cnt, limit, checkunit, qlimit = 0, 1.5e9, 1000, 1000000

def main():
	time_start=time.time()

	q.put("https://www.baidu.com") # 初始地址

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