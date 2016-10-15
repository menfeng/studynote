1.Designating the settings
	environment variable SCRAPY_SETTINGS_MODULE.
	eg: myproject.settings
2.Populating the settings
	precedence
	Command line options (most precedence)
		scrapy crawl myspider -s LOG_FILE=scrapy.log
	Settings per-spider
		class MySpider(scrapy.Spider):
    		custom_settings = { 'SOME_SETTING': 'some value', }
	Project settings module
		settings.py
	Default settings per-command
		Those custom command settings are specified in the default_settings attribute of the command class.
	Default global settings (less precedence)
		 scrapy.settings.default_settings
3.How to access settings
	1.self.settings.attributes.keys() in spider
	2.scrapy.crawler.Crawler.settings attribute of the Crawler that is passed to from_crawler method in extensions, middlewares and item pipelines:
		class MyExtension(object):
			def __init__(self, log_is_enabled=False):
				if log_is_enabled:
					print("log is enabled!")

			@classmethod
			def from_crawler(cls, crawler):
				settings = crawler.settings
			return cls(settings.getbool('LOG_ENABLED'))
4.Built-in settings reference
	notice:DOWNLOAD_DELAY

	AWS_ACCESS_KEY_ID
		The AWS access key used by code that requires access to Amazon Web services, such as the S3 feed storage backend.
	AWS_SECRET_ACCESS_KEY
		The AWS secret key used by code that requires access to Amazon Web services, such as the S3 feed storage backend.
	BOT_NAME
		Default: scrapybot
		This will be used to construct the User-Agent by default, and also for logging.
		It¡¯s automatically populated with your project name when you create your project with the startproject command.
	CONCURRENT_ITEMS
		Default: 100
		Maximum number of concurrent items (per response) to process in parallel in the Item Processor
	CONCURRENT_REQUESTS
		Default: 16
		The maximum number of concurrent (ie. simultaneous) requests that will be performed by the Scrapy downloader.
	CONCURRENT_REQUESTS_PER_DOMAIN
		Default: 8
		The maximum number of concurrent (ie. simultaneous) requests that will be performed to any single domain.
	CONCURRENT_REQUESTS_PER_IP
		Default: 0
		If non-zero, 
		 	CONCURRENT_REQUESTS_PER_DOMAIN setting is ignored
		 	DOWNLOAD_DELAY is enforced per IP, not per domain.
	DEFAULT_ITEM_CLASS
		Default: 'scrapy.item.Item'
		The default class that will be used for instantiating items in the the Scrapy shell.
	DEFAULT_REQUEST_HEADERS
		Default:
		{
			'Accept': 'text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8',
			'Accept-Language': 'en',
		}
	DEPTH_LIMIT
		Default: 0
		Scope: scrapy.spidermiddlewares.depth.DepthMiddleware
	DEPTH_PRIORITY
		Default: 0
		Scope: scrapy.spidermiddlewares.depth.DepthMiddleware
		a positive value will decrease the priority
		a negative value will increase priority
	DEPTH_STATS
		Default: True
		Scope: scrapy.spidermiddlewares.depth.DepthMiddleware
		Whether to collect maximum depth stats.
	DEPTH_STATS_VERBOSE
		Default: False
		Scope: scrapy.spidermiddlewares.depth.DepthMiddleware
		Whether to collect verbose depth stats. If this is enabled, the number of requests for each depth is collected in the stats.
	DNSCACHE_ENABLED
		Default: True
	DNSCACHE_SIZE
		Default: 10000
		DNS in-memory cache size.
	DNS_TIMEOUT
		Default: 60
		Timeout for processing of DNS queries in seconds. Float is supported.
	DOWNLOADER
		Default: 'scrapy.core.downloader.Downloader'
	DOWNLOADER_HTTPCLIENTFACTORY
		Default: 'scrapy.core.downloader.webclient.ScrapyHTTPClientFactory'
		Defines a Twisted protocol.ClientFactory class to use for HTTP/1.0 connections (for HTTP10DownloadHandler).
	DOWNLOADER_CLIENTCONTEXTFACTORY
		Represents the classpath to the ContextFactory to use
		Default: 'scrapy.core.downloader.contextfactory.ScrapyClientContextFactory'
		If you do need remote server certificate verification enabled
			'scrapy.core.downloader.contextfactory.BrowserLikeContextFactory', which uses the platform¡¯s certificates to validate remote endpoints. This is only available if you use Twisted>=14.0.
	DOWNLOADER_CLIENT_TLS_METHOD
		Use this setting to customize the TLS/SSL method used by the default HTTP/1.1 downloader.
	DOWNLOADER_MIDDLEWARES
		Default:: {}
		A dict containing the downloader middlewares enabled in your project
	DOWNLOADER_MIDDLEWARES_BASE
		Default:
		{
			'scrapy.downloadermiddlewares.robotstxt.RobotsTxtMiddleware': 100,
			'scrapy.downloadermiddlewares.httpauth.HttpAuthMiddleware': 300,
			'scrapy.downloadermiddlewares.downloadtimeout.DownloadTimeoutMiddleware': 350,
			'scrapy.downloadermiddlewares.useragent.UserAgentMiddleware': 400,
			'scrapy.downloadermiddlewares.retry.RetryMiddleware': 500,
			'scrapy.downloadermiddlewares.defaultheaders.DefaultHeadersMiddleware': 550,
			'scrapy.downloadermiddlewares.ajaxcrawl.AjaxCrawlMiddleware': 560,
			'scrapy.downloadermiddlewares.redirect.MetaRefreshMiddleware': 580,
			'scrapy.downloadermiddlewares.httpcompression.HttpCompressionMiddleware': 590,
			'scrapy.downloadermiddlewares.redirect.RedirectMiddleware': 600,
			'scrapy.downloadermiddlewares.cookies.CookiesMiddleware': 700,
			'scrapy.downloadermiddlewares.httpproxy.HttpProxyMiddleware': 750,
			'scrapy.downloadermiddlewares.chunked.ChunkedTransferMiddleware': 830,
			'scrapy.downloadermiddlewares.stats.DownloaderStats': 850,
			'scrapy.downloadermiddlewares.httpcache.HttpCacheMiddleware': 900,
		}
		A dict containing the downloader middlewares enabled by default in Scrapy
	DOWNLOADER_STATS
		Default: True
		Whether to enable downloader stats collection.
	DOWNLOAD_DELAY
		Default: 0
		The amount of time (in secs) that the downloader should wait before downloading consecutive pages from the same website.
		This setting is also affected by the RANDOMIZE_DOWNLOAD_DELAY setting.
		Scrapy doesn¡¯t wait a fixed amount of time between requests, but uses a random interval between 0.5 and 1.5 * DOWNLOAD_DELAY.
		When CONCURRENT_REQUESTS_PER_IP is non-zero, delays are enforced per ip address instead of per domain.
		You can also change this setting per spider by setting download_delay spider attribute.
	DOWNLOAD_HANDLERS
		Default: {}
		A dict containing the request downloader handlers enabled in your project
	DOWNLOAD_HANDLERS_BASE
		Default:
		{
			'file': 'scrapy.core.downloader.handlers.file.FileDownloadHandler',
			'http': 'scrapy.core.downloader.handlers.http.HTTPDownloadHandler',
			'https': 'scrapy.core.downloader.handlers.http.HTTPDownloadHandler',
			's3': 'scrapy.core.downloader.handlers.s3.S3DownloadHandler',
			'ftp': 'scrapy.core.downloader.handlers.ftp.FTPDownloadHandler',
		}
		A dict containing the request download handlers enabled by default in Scrapy. 
	DOWNLOAD_TIMEOUT
		The amount of time (in secs) that the downloader will wait before timing out.
		This timeout can be set per spider using download_timeout spider attribute and per-request using download_timeout Request.meta key.
	DOWNLOAD_MAXSIZE
		Default: 1073741824 (1024MB)
		The maximum response size (in bytes) that downloader will download.
		If you want to disable it set to 0.
		This size can be set per spider using download_maxsize spider attribute and per-request using download_maxsize Request.meta key.
	DOWNLOAD_WARNSIZE
		Default: 33554432 (32MB)
		The response size (in bytes) that downloader will start to warn.
		his size can be set per spider using download_warnsize spider attribute and per-request using download_warnsize Request.meta key.
	DUPEFILTER_CLASS
		Default: 'scrapy.dupefilters.RFPDupeFilter'
		The class used to detect and filter duplicate requests.
	DUPEFILTER_DEBUG
		Default: False
		By default, RFPDupeFilter only logs the first duplicate request. Setting DUPEFILTER_DEBUG to True will make it log all duplicate requests.
	EDITOR
		Default: depends on the environment
		The editor to use for editing spiders with the edit command
	EXTENSIONS
		Default:: {}
		A dict containing the extensions enabled in your project, and their orders.
	EXTENSIONS_BASE
		Default:
		{
			'scrapy.extensions.corestats.CoreStats': 0,
			'scrapy.extensions.telnet.TelnetConsole': 0,
			'scrapy.extensions.memusage.MemoryUsage': 0,
			'scrapy.extensions.memdebug.MemoryDebugger': 0,
			'scrapy.extensions.closespider.CloseSpider': 0,
			'scrapy.extensions.feedexport.FeedExporter': 0,
			'scrapy.extensions.logstats.LogStats': 0,
			'scrapy.extensions.spiderstate.SpiderState': 0,
			'scrapy.extensions.throttle.AutoThrottle': 0,
		}
	FEED_TEMPDIR
		The Feed Temp dir allows you to set a custom folder to save crawler temporary files before uploading with FTP feed storage and Amazon S3.
	FILES_STORE_S3_ACL
		Default: 'private'
		S3-specific access control policy (ACL) for S3 files store.
	ITEM_PIPELINES
		Default: {}
		A dict containing the item pipelines to use, and their orders. Order values are arbitrary, but it is customary to define them in the 0-1000 range. Lower orders process before higher orders.
		Example:
		ITEM_PIPELINES = {
			'mybot.pipelines.validate.ValidateMyItem': 300,
			'mybot.pipelines.validate.StoreMyItem': 800,
		}
	ITEM_PIPELINES_BASE
		A dict containing the pipelines enabled by default in Scrapy

	#log
	LOG_ENABLED
		Default: True
		Whether to enable logging.
	LOG_ENCODING
		Default: 'utf-8'
		The encoding to use for logging.
	LOG_FILE
		Default: None
		File name to use for logging output. If None, standard error will be used.
	LOG_FORMAT
		Default: '%(asctime)s [%(name)s] %(levelname)s: %(message)s'
	LOG_DATEFORMAT
		Default: '%Y-%m-%d %H:%M:%S'
	LOG_LEVEL
		Default: 'DEBUG'
	LOG_STDOUT
		Default: False
		If True, all standard output (and error) of your process will be redirected to the log. For example if you print 'hello' it will appear in the Scrapy log.
	#memory
	MEMDEBUG_ENABLED	
		Default: []
		When memory debugging is enabled a memory report will be sent to the specified addresses if this setting is not empty, otherwise the report will be written to the log.
		Example:
		MEMDEBUG_NOTIFY = ['user@example.com']
	MEMUSAGE_ENABLED
		Default: False
		Scope: scrapy.extensions.memusage
		Whether to enable the memory usage extension that will shutdown the Scrapy process when it exceeds a memory limit, and also notify by email when that happened.
	MEMUSAGE_LIMIT_MB
		Default: 0
		Scope: scrapy.extensions.memusage
		The maximum amount of memory to allow (in megabytes) before shutting down Scrapy
	MEMUSAGE_CHECK_INTERVAL_SECONDS
		Default: 60.0
		Scope: scrapy.extensions.memusage
		The Memory usage extension checks the current memory usage, versus the limits set by MEMUSAGE_LIMIT_MB and MEMUSAGE_WARNING_MB, at fixed time intervals.
	MEMUSAGE_NOTIFY_MAIL
		Default: False
		Scope: scrapy.extensions.memusage
		A list of emails to notify if the memory limit has been reached.
		Example:
		MEMUSAGE_NOTIFY_MAIL = ['user@example.com']
	MEMUSAGE_REPORT
		Default: False
		Scope: scrapy.extensions.memusage
		Whether to send a memory usage report after each spider has been closed.
	MEMUSAGE_WARNING_MB
		Default: 0
		Scope: scrapy.extensions.memusage
		The maximum amount of memory to allow (in megabytes) before sending a warning email notifying about it. If zero, no warning will be produced.

	NEWSPIDER_MODULE
		Default: ''
		Module where to create new spiders using the genspider command.
		Example:
		NEWSPIDER_MODULE = 'mybot.spiders_dev'
	RANDOMIZE_DOWNLOAD_DELAY
		Default: True
		If enabled, Scrapy will wait a random amount of time (between 0.5 and 1.5 * DOWNLOAD_DELAY) while fetching requests from the same website.
		If DOWNLOAD_DELAY is zero (default) this option has no effect.
	REACTOR_THREADPOOL_MAXSIZE
		Default: 10
		The maximum limit for Twisted Reactor thread pool size. This is common multi-purpose thread pool used by various Scrapy components. Threaded DNS Resolver, BlockingFeedStorage, S3FilesStore just to name a few. Increase this value if you¡¯re experiencing problems with insufficient blocking IO.
	REDIRECT_MAX_TIMES
		Default: 20
		Defines the maximum times a request can be redirected. After this maximum the request¡¯s response is returned as is
	REDIRECT_PRIORITY_ADJUST
		Default: +2
		Scope: scrapy.downloadermiddlewares.redirect.RedirectMiddleware
		Adjust redirect request priority relative to original request:
		a positive priority adjust (default) means higher priority.
		a negative priority adjust means lower priority.
	RETRY_PRIORITY_ADJUST
		Default: -1
		Scope: scrapy.downloadermiddlewares.retry.RetryMiddleware
		Adjust retry request priority relative to original request:
		a positive priority adjust means higher priority.
		a negative priority adjust (default) means lower priority.
	ROBOTSTXT_OBEY
		Default: False
		Scope: scrapy.downloadermiddlewares.robotstxt
		If enabled, Scrapy will respect robots.txt policies
		While the default value is False for historical reasons, this option is enabled by default in settings.py file generated by scrapy startproject command.
	SCHEDULER	
		Default: 'scrapy.core.scheduler.Scheduler'
		The scheduler to use for crawling.
	SPIDER_CONTRACTS
		Default:: {}
		A dict containing the spider contracts enabled in your project, used for testing spiders.
	SPIDER_CONTRACTS_BASE
		Default:
		{
			'scrapy.contracts.default.UrlContract' : 1,
			'scrapy.contracts.default.ReturnsContract': 2,
			'scrapy.contracts.default.ScrapesContract': 3,
		}
	SPIDER_LOADER_CLASS
		Default: 'scrapy.spiderloader.SpiderLoader'
	SPIDER_MIDDLEWARES
		Default:: {}
		A dict containing the spider middlewares enabled in your project, and their orders
	SPIDER_MIDDLEWARES_BASE
		Default:
		{
			'scrapy.spidermiddlewares.httperror.HttpErrorMiddleware': 50,
			'scrapy.spidermiddlewares.offsite.OffsiteMiddleware': 500,
			'scrapy.spidermiddlewares.referer.RefererMiddleware': 700,
			'scrapy.spidermiddlewares.urllength.UrlLengthMiddleware': 800,
			'scrapy.spidermiddlewares.depth.DepthMiddleware': 900,
		}
	SPIDER_MODULES
		Default: []
		A list of modules where Scrapy will look for spiders.
		Example:
		SPIDER_MODULES = ['mybot.spiders_prod', 'mybot.spiders_dev']
	STATS_CLASS
		Default: 'scrapy.statscollectors.MemoryStatsCollector'
		The class to use for collecting stats,
	STATS_DUMP
		Default: True
		Dump the Scrapy stats (to the Scrapy log) once the spider finishes.
	STATSMAILER_RCPTS
		Default: [] (empty list)
		Send Scrapy stats after spiders finish scraping. Se
	TELNETCONSOLE_ENABLED
		Default: True
		A boolean which specifies if the telnet console will be enabled 
	TELNETCONSOLE_PORT
		Default: [6023, 6073]
		The port range to use for the telnet console. If set to None or 0, a dynamically assigned port is used
	TEMPLATES_DIR
		Default: templates dir inside scrapy module
		The directory where to look for templates when creating new projects with startproject command and new spiders with genspider command.
	URLLENGTH_LIMIT
		Default: 2083
		Scope: spidermiddlewares.urllength
		The maximum URL length to allow for crawled URLs
