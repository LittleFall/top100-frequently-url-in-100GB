import requests
import urllib
import bs4
from queue import Queue
	
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
	try:
		req_obj = requests.get(url, timeout = 0.3)
		bresp = bs4.BeautifulSoup(req_obj.text,'lxml')
		hrefs = bresp.find_all(lambda tag: tag.has_attr('href'))
		parse = urllib.parse.urlparse(url)
		return parse, hrefs
	except requests.exceptions.RequestException:
		return "", {}


def main():
	fo = open("data100mb.txt", "w")

	q = Queue() # 队列
	q.put("https://www.baidu.com") # 初始地址

	cnt, limit, checkunit = 0, 2000000, 1000

	while(not q.empty()):
		url = q.get()
		print("url =", url)
		parse, hrefs = get_parse_and_hrefs_from_url(url)

		for href in hrefs:
			string = href['href'];
			string = complete_url(string, parse)
			if(string.startswith("http")):
				cnt = cnt + 1
				fo.write(string+"\n")
				q.put(string)

				if(cnt % 10 == 0):
					print(cnt, q.qsize())
				if(cnt == limit):
					q.queue.clear()
					break
	fo.close()

if __name__ == '__main__':
    main()