"""
1.Creating a project
	cmd:scrapy startproject tutorial

	tutorial/
    scrapy.cfg            # deploy configuration file
    tutorial/             # project's Python module, you'll import your code from here
        __init__.py
        items.py          # project items file
        pipelines.py      # project pipelines file
        settings.py       # project settings file
        spiders/          # a directory where you'll later put your spiders
            __init__.py
2.Defining our Item
	Items are containers that will be loaded with the scraped data
	we define subclass of scrapy.Item in tutorial.items

3.Our first Spider
	Spiders are classes that you define and Scrapy uses to scrape information from a domain (or group of domains).
	we define subclass of scrapy.Spider in tutorial.spider.dmoz_spider.py
	name: 		identifies the Spider
	start_urls: a list of URLs where the Spider will begin to crawl from
	parse(): 	a method of the spider, which will be called with the downloaded Response object of each start URL. 
				response's type is scrapy.http.Response.

4.Crawling
	cmd:scrapy crawl 360
	we will see the log:
		2016-07-03 17:08:11 [scrapy] DEBUG: Crawled (200) <GET https://www.baidu.com/> (referer: None)
		referer:None means that these URLs are the starting ones, they have no referrers

5.Extracting Items
	crapy uses a mechanism based on XPath or CSS expressions called Scrapy Selectors
	eg:
		/html/head/title			:selects the <title> element, inside the <head> element of an HTML document
		/html/head/title/text()		:selects the text inside the aforementioned <title> element.
		//td						:selects all the <td> elements
		//div[@class="mine"]		:selects all div elements which contain an attribute class="mine"

	Introduction to Selectors
		You can see selectors as objects that represent nodes in the document structure
		Selectors have four basic methods 
		xpath()		: returns a list of selectors, each of which represents the nodes selected by the xpath expression
		css()		: returns a list of selectors, each of which represents the nodes selected by the CSS expression
		extract()	: returns a unicode string with the selected data.
		re()		: returns a list of unicode strings extracted by applying the regular expression given as argument.
	
	Trying Selectors in the Shell
		scrapy shell "https://hao.360.cn/"
		>>> response.xpath('//title')
		[<Selector xpath='//title' data=u'<title>360\u5bfc\u822a_\u65b0\u4e00\u4ee3\u5b89\u5168\u4e0a\u7f51\u5bfc\u822a</title>'>]
		>>> response.xpath('//title').extract()
		[u'<title>360\u5bfc\u822a_\u65b0\u4e00\u4ee3\u5b89\u5168\u4e0a\u7f51\u5bfc\u822a</title>']
		>>> response.xpath('//title/text()')
		[<Selector xpath='//title/text()' data=u'360\u5bfc\u822a_\u65b0\u4e00\u4ee3\u5b89\u5168\u4e0a\u7f51\u5bfc\u822a'>]
		>>> response.xpath('//title/text()').extract()
		[u'360\u5bfc\u822a_\u65b0\u4e00\u4ee3\u5b89\u5168\u4e0a\u7f51\u5bfc\u822a']
		>>> response.xpath('//title/text()').re('(\w+)')
		[u'360\u5bfc\u822a_\u65b0\u4e00\u4ee3\u5b89\u5168\u4e0a\u7f51\u5bfc\u822a']

		we find tilte,description and link as bellow:
			t=response.xpath('//title/text()').extract()
			d=response.xpath('//meta[@name="description"]/@content').extract()
			l=response.xpath('//link[@rel="dns-prefetch"]/@href').extract()

6.Using our item
	Item objects are custom Python dicts
		item = DmozItem()
		item['tilte']=response.xpath('//title/text()').extract()
	cmd:scrapy crawl 360
		we will see the item be yielded

7.Following links
 	url = response.urljoin(href.extract())
	yield scrapy.Request(url, callback=self.parse_self)

8.Storing the scraped data
	scrapy crawl 360 -o items.json
"""
