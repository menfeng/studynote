"""
Spiders
	 start_requests(start_urls)[by default]-->parse(response)
	 in callback
		you can pase the page using:
			Selectors 
			BeautifulSoup
			lxml
			whatever mechanism you prefer
	 	you can return:
			Dicts with extracted data
			Item objects
			Request objects
			An iterable of these objects

scrapy.Spider(scrapy.spiders.Spider)
	It just provides a default start_requests() implementation 
	name			:it must be unique
	allowed_domains	:requests for URLs not belonging to the domain names specified in this list (or their subdomains) won’t be followed if OffsiteMiddleware is enabled.
	start_urls		:a list of URLs where the spider will begin to crawl from
	custom_settings	:a dictionary of settings that will be overridden from the project wide configuration when running this spider.
	crawler			:(scrapy.crawler.Crawler)This attribute is set by the from_crawler() class method after initializating the class, and links to the Crawler object to which this spider instance is bound
	settings		:configuration for running this spider. This is a Settings instance
	logger			:python logger created with the Spider’s name
	from_crawler	:This is the class method used by Scrapy to create your spiders
		from_crawler(crawler, *args, **kwargs)
			crawler (Crawler instance) – crawler to which the spider will be bound
			args (list) – arguments passed to the __init__() method
			kwargs (dict) – keyword arguments passed to the __init__() method
	start_requests():This method must return an iterable with the first Requests to crawl for this spider
		For example, if you need to start by logging in using a POST request:
			def start_requests(self):
				return [scrapy.FormRequest("http://www.example.com/login", formdata={'user': 'john', 'pass': 'secret'}, callback=self.logged_in)]
	make_requests_from_url:this method returns Requests with the parse() method as their callback function, and with dont_filter parameter enabled
	parse(response)	:default callback
	log				:wrapper that sends a log message through the Spider’s logger
	closed(reason)	:called when the spider closes

Spider arguments
	commandline
		scrapy crawl myspider -a category=electronics
		Spiders receive arguments in their constructors
		def __init__(self, category=None, *args, **kwargs)
	spider arguments can also be passed through the Scrapyd schedule.json
"""

"""
Generic Spiders	
	CrawlSpider(scrapy.spiders.CrawlSpider)
		rules:Which is a list of one (or more) Rule objects.If multiple rules match the same link, the first one will be used.
		parse_start_url(response):This method is called for the start_urls responses.
		Crawling rules:scrapy.spiders.Rule(link_extractor, callback=None, cb_kwargs=None, follow=None, process_links=None, process_request=None)
			link_extractor	:is a Link Extractor object which defines how links will be extracted from each crawled page.
			callback 		:is a callable or a string to be called for each link extracted with the specified link_extractor.
							When writing crawl spider rules, avoid using parse as callback, since the CrawlSpider uses the parse method itself to implement its logic. So if you override the parse method, the crawl spider will no longer work.
			cb_kwargs 		:is a dict containing the keyword arguments to be passed to the callback function.
			follow 			:is a boolean which specifies if links should be followed from each response extracted with this rule. If callback is None follow defaults to True, otherwise it defaults to False.
			process_links 	:is a callable, or a string which will be called for each list of links extracted from each response using the specified link_extractor. This is mainly used for filtering purposes.
			process_request :is a callable, or a string which will be called with every request extracted by this rule, and must return a request or None (to filter out the request).
		CrawlSpider example	:
"""
import scrapy
from scrapy.spiders import CrawlSpider, Rule
from scrapy.linkextractors import LinkExtractor

class MySpider(CrawlSpider):
    name = 'example.com'
    allowed_domains = ['example.com']
    start_urls = ['http://www.example.com']

    rules = (
        # Extract links matching 'category.php' (but not matching 'subsection.php')
        # and follow links from them (since no callback means follow=True by default).
        Rule(LinkExtractor(allow=('category\.php', ), deny=('subsection\.php', ))),

        # Extract links matching 'item.php' and parse them with the spider's method parse_item
        Rule(LinkExtractor(allow=('item\.php', )), callback='parse_item'),
    )

    def parse_item(self, response):
        self.logger.info('Hi, this is an item page! %s', response.url)
        item = scrapy.Item()
        item['id'] = response.xpath('//td[@id="item_id"]/text()').re(r'ID: (\d+)')
        item['name'] = response.xpath('//td[@id="item_name"]/text()').extract()
        item['description'] = response.xpath('//td[@id="item_description"]/text()').extract()
        return item
"""
XMLFeedSpider
CSVFeedSpider
SitemapSpider
"""


























