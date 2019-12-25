import requests
import urllib
import bs4


def search(url):
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
			print(string) # 收录


search("https://www.baidu.com")
