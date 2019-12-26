import requests
import urllib
import bs4
from queue import Queue

try:
	url = "http://downpack.baidu.com/baidunews_AndroidPhone_1014720b.apk"
	req_obj = requests.get(url, timeout = 0.3)
	bresp = bs4.BeautifulSoup(req_obj.text,'lxml')
	hrefs = bresp.find_all(lambda tag: tag.has_attr('href'))
	parse = urllib.parse.urlparse(url)
except requests.exceptions.RequestException:
	print("exception.")

print("end")
