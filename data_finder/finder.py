import requests
import urllib
import bs4
from queue import Queue


def search(q, url, cnt):
	parse = urllib.parse.urlparse(url)

	req_obj = requests.get(url)
	bresp = bs4.BeautifulSoup(req_obj.text,'lxml')
	a = bresp.find_all(lambda tag: tag.has_attr('href'))
	for item in a:
		string = item['href'];
		# 双斜杠开头，补全协议
		# 单斜杠开头，补全协议与域名
		# http开头，不进行操作
		# 其他，不收录
		string.strip()
		if(string.startswith("//")):
			string = parse.scheme + ":" + string
		if(string.startswith("/")):
			string = parse.scheme + "://" + parse.netloc + string
		if(string.startswith("http")):
			cnt = cnt + 1
			if(cnt < 500):
				print(cnt, string) 
				q.put(string)

def main():
	q = Queue() # 队列
	q.put("https://www.baidu.com") # 初始地址
	q.put("http://www.runoob.com/") # 初始地址
	cnt = q.qsize()
	while(not q.empty()):
		search(q, q.get(), cnt)

if __name__ == '__main__':
    main()