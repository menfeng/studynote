http://www.tuicool.com/articles/VRfQR3U
http://www.tuicool.com/articles/7ZnYJb2

Here are some tips to keep in mind when dealing with these kinds of sites
	rotate your user agent from a pool of well-known ones from browsers (google around to get a list of them)
	disable cookies (see COOKIES_ENABLED) as some sites may use cookies to spot bot behaviour
	use download delays (2 or higher). See DOWNLOAD_DELAY setting.
	if possible, use Google cache to fetch pages, instead of hitting the sites directly
	use a pool of rotating IPs. For example, the free Tor project or paid services like ProxyMesh
	use a highly distributed downloader that circumvents bans internally, so you can just focus on parsing clean pages. One example of such downloaders is Crawlera


1.rotate your user agent from a pool of well-known ones from browsers (google around to get a list of them)
	ip,user_agent is control by  DOWNLOADER_MIDDLEWARES

	import random
	import base64
	from settings import PROXIES
	class RandomUserAgent(object):
	  def __init__(self, agents):
		self.agents = agents

	  @classmethod
	  def from_crawler(cls, crawler):
			return cls(crawler.settings.getlist('USER_AGENTS'))

	  def process_request(self, request, spider):
			request.headers.setdefault('User-Agent',random.choice(self.agents))

	USER_AGENTS = [
	  "Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1; SV1; AcooBrowser; .NET CLR 1.1.4322; .NET CLR 2.0.50727)",
	  "Mozilla/4.0 (compatible; MSIE 7.0; Windows NT 6.0; Acoo Browser; SLCC1; .NET CLR 2.0.50727; Media Center PC 5.0; .NET CLR 3.0.04506)",
	  "Mozilla/4.0 (compatible; MSIE 7.0; AOL 9.5; AOLBuild 4337.35; Windows NT 5.1; .NET CLR 1.1.4322; .NET CLR 2.0.50727)",
	  "Mozilla/5.0 (Windows; U; MSIE 9.0; Windows NT 9.0; en-US)",
	  "Mozilla/5.0 (compatible; MSIE 9.0; Windows NT 6.1; Win64; x64; Trident/5.0; .NET CLR 3.5.30729; .NET CLR 3.0.30729; .NET CLR 2.0.50727; Media Center PC 6.0)",
	  "Mozilla/5.0 (compatible; MSIE 8.0; Windows NT 6.0; Trident/4.0; WOW64; Trident/4.0; SLCC2; .NET CLR 2.0.50727; .NET CLR 3.5.30729; .NET CLR 3.0.30729; .NET CLR 1.0.3705; .NET CLR 1.1.4322)",
	  "Mozilla/4.0 (compatible; MSIE 7.0b; Windows NT 5.2; .NET CLR 1.1.4322; .NET CLR 2.0.50727; InfoPath.2; .NET CLR 3.0.04506.30)",
	  "Mozilla/5.0 (Windows; U; Windows NT 5.1; zh-CN) AppleWebKit/523.15 (KHTML, like Gecko, Safari/419.3) Arora/0.3 (Change: 287 c9dfb30)",
	  "Mozilla/5.0 (X11; U; Linux; en-US) AppleWebKit/527+ (KHTML, like Gecko, Safari/419.3) Arora/0.6",
	  "Mozilla/5.0 (Windows; U; Windows NT 5.1; en-US; rv:1.8.1.2pre) Gecko/20070215 K-Ninja/2.1.1",
	  "Mozilla/5.0 (Windows; U; Windows NT 5.1; zh-CN; rv:1.9) Gecko/20080705 Firefox/3.0 Kapiko/3.0",
	  "Mozilla/5.0 (X11; Linux i686; U;) Gecko/20070322 Kazehakase/0.4.5",
	  "Mozilla/5.0 (X11; U; Linux i686; en-US; rv:1.9.0.8) Gecko Fedora/1.9.0.8-1.fc10 Kazehakase/0.5.6",
	  "Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/535.11 (KHTML, like Gecko) Chrome/17.0.963.56 Safari/535.11",
	  "Mozilla/5.0 (Macintosh; Intel Mac OS X 10_7_3) AppleWebKit/535.20 (KHTML, like Gecko) Chrome/19.0.1036.7 Safari/535.20",
	  "Opera/9.80 (Macintosh; Intel Mac OS X 10.6.8; U; fr) Presto/2.9.168 Version/11.52",
	]
2.disable cookies (see COOKIES_ENABLED) as some sites may use cookies to spot bot behaviour
	COOKIES_ENABLED=False
3.use download delays (2 or higher). See DOWNLOAD_DELAY setting.
	DOWNLOAD_DELAY=3
4.if possible, use Google cache to fetch pages, instead of hitting the sites directly
5.use a pool of rotating IPs. For example, the free Tor project or paid services like ProxyMesh
 	class ProxyMiddleware(object):
	  def process_request(self, request, spider):
		proxy = random.choice(PROXIES)
		if proxy['user_pass'] is not None:
			request.meta['proxy'] = "http://%s" % proxy['ip_port']
			encoded_user_pass = base64.encodestring(proxy['user_pass'])
			request.headers['Proxy-Authorization'] = 'Basic ' + encoded_user_pass
		else:
			request.meta['proxy'] = "http://%s" % proxy['ip_port']
 	PROXIES = [
	  {'ip_port': '111.11.228.75:80', 'user_pass': ''},
	  {'ip_port': '120.198.243.22:80', 'user_pass': ''},
	  {'ip_port': '111.8.60.9:8123', 'user_pass': ''},
	  {'ip_port': '101.71.27.120:80', 'user_pass': ''},
	  {'ip_port': '122.96.59.104:80', 'user_pass': ''},
	  {'ip_port': '122.224.249.122:8088', 'user_pass': ''},
	]
	#PROXIES comfrom http://www.xici.net.co/
6.use a highly distributed downloader that circumvents bans internally, so you can just focus on parsing clean pages. One example of such downloaders is Crawlera
	https://app.scrapinghub.com/o/67798
	pip install scrapy-crawlera
	DOWNLOADER_MIDDLEWARES = {'scrapy_crawlera.CrawleraMiddleware': 300}
	CRAWLERA_ENABLED = True
	CRAWLERA_APIKEY = '<API key>'
