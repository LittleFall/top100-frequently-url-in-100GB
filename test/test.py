import urllib
try:
    url = "http://www.baidu.com"
    f = urllib2.urlopen(url, timeout=0) #timeout设置超时的时间
    result = f.read()
    print result
except Exception,e:
    print 'a',str(e)
