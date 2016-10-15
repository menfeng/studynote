1.Spider Middleware
	SPIDER_MIDDLEWARES = {
    'myproject.middlewares.CustomSpiderMiddleware': 543,
	}
	1.SPIDER_MIDDLEWARES is merged with the SPIDER_MIDDLEWARES_BASE
	2.sort
	3.engine--first---second---...---spider

2.class scrapy.spidermiddlewares.SpiderMiddleware
	process_spider_input(response, spider)
		this method is called for each response that goes through the spider middleware and into the spider, for processing.
		return None		:continue next
		raises exception:halt and  will call the request errback.
	process_spider_output(response, result, spider)
		This method is called with the results returned from the Spider, after it has processed the response.
		must return an iterable of Request, dict or Item objects.
		result	:(an iterable of Request, dict or Item objects) ¨C the result returned by the spider
	process_spider_exception(response, exception, spider)
		This method is called when when a spider or process_spider_input() method (from other spider middleware) raises an exception.
		return None		:continue next
		return iterable	:start handle iterable(process_spider_output())
	process_start_requests(start_requests, spider)
		This method is called with the start requests of the spider, 
		and works similarly to the process_spider_output() method, 
		except that it doesn¡¯t have a response associated and must return only requests (not items).
		It receives an iterable (in the start_requests parameter) and must return another iterable of Request objects.

3.Built-in spider middleware reference
	class scrapy.spidermiddlewares.depth.DepthMiddleware
		DEPTH_LIMIT
			The maximum depth that will be allowed to crawl for any site. If zero, no limit will be imposed.
		DEPTH_STATS
			Whether to collect depth stats.
		DEPTH_PRIORITY
			Whether to prioritize the requests based on their depth.
	
	class scrapy.spidermiddlewares.httperror.HttpErrorMiddleware
		According to the HTTP standard, successful responses are those whose status codes are in the 200-300 range
		Request.meta
			handle_httpstatus_list
			handle_httpstatus_all
		Spider attr
			handle_httpstatus_list = [404] #if you want to handle response with 404 status code
		HTTPERROR_ALLOWED_CODES
			Default: []
			Pass all responses with non-200 status codes contained in this list.
		HTTPERROR_ALLOW_ALL
			Default: False
			Pass all responses, regardless of its status code.

	class scrapy.spidermiddlewares.offsite.OffsiteMiddleware
		Filters out Requests for URLs outside the domains covered by the spider.
		spider.allowed_domains
		request.dont_filter
	
	class scrapy.spidermiddlewares.referer.RefererMiddleware
		Populates Request Referer header, based on the URL of the Response which generated it.
		REFERER_ENABLED
			Default: True
			Whether to enable referer middleware.

	class scrapy.spidermiddlewares.urllength.UrlLengthMiddleware	
		Filters out requests with URLs longer than URLLENGTH_LIMIT
		URLLENGTH_LIMIT
			The maximum URL length to allow for crawled URLs.

