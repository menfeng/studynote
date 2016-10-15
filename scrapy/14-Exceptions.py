1.scrapy.exceptions.DropItem
	The exception that must be raised by item pipeline stages to stop processing an Item.
2.scrapy.exceptions.CloseSpider(reason='cancelled')
	This exception can be raised from a spider callback to request the spider to be closed/stopped. Supported arguments:
	Parameters:	reason (str) ¨C the reason for closing
	For example:
	def parse_page(self, response):
		if 'Bandwidth exceeded' in response.body:
			raise CloseSpider('bandwidth_exceeded')
3.scrapy.exceptions.IgnoreRequest
	This exception can be raised by the Scheduler or any downloader middleware to indicate that the request should be ignored.
4.scrapy.exceptions.NotConfigured
	This exception can be raised by some components to indicate that they will remain disabled. Those components include:
	Extensions
	Item pipelines
	Downloader middlewares
	Spider middlewares
	The exception must be raised in the component constructor.
5.scrapy.exceptions.NotSupported
	This exception is raised to indicate an unsupported feature.
