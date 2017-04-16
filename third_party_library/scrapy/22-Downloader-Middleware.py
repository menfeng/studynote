1.Downloader Middleware
	DOWNLOADER_MIDDLEWARES = {
	'myproject.middlewares.CustomDownloaderMiddleware': 543,
	}
	1.DOWNLOADER_MIDDLEWARES is merged with the DOWNLOADER_MIDDLEWARES_BASE
	2.sort
	3.engine---first---second...---downloader

2.Writing your own downloader middleware
	class scrapy.downloadermiddlewares.DownloaderMiddleware
		process_request(request, spider)
			return None			:continue next
			return Response		:start to handle response(process_response())
			return Request		:start to handle new request(reschedule the returned request)
			raises IgnoreRequest:start to handle exception(process_exception())
								 if none of them handle the exception,(Request.errback) is called
		process_response(request, response, spider)
			return Response		:continue next
			return Request		:start to handle new request(reschedule the returned request)
			raises IgnoreRequest:(Request.errback) is called
		process_exception(request, exception, spider)
			return None			:continue next
			return Response		:start to handle response(process_response())
			return Request		:start to handle new request(reschedule the returned request)

3.Built-in downloader middleware reference
	class scrapy.downloadermiddlewares.cookies.CookiesMiddleware
		It keeps track of cookies sent by web servers, and send them back on subsequent requests (from that spider), just like web browsers do.
		config
			COOKIES_ENABLED
				Default: True
				Whether to enable the cookies middleware. If disabled, no cookies will be sent to web servers.
			COOKIES_DEBUG
				Default: False
				If enabled, Scrapy will log all cookies sent in requests (ie. Cookie header) and all cookies received in responses (ie. Set-Cookie header).
				2011-04-06 14:35:10-0300 [scrapy] DEBUG: Sending cookies to: <GET http://www.diningcity.com/netherlands/index.html> Cookie: clientlanguage_nl=en_EN
				2011-04-06 14:35:14-0300 [scrapy] DEBUG: Received cookies from: <200 http://www.diningcity.com/netherlands/index.html>
		Multiple cookie sessions per spider
			There is support for keeping multiple cookie sessions per spider by using the cookiejar Request meta key
			for i, url in enumerate(urls):
    			yield scrapy.Request(url, meta={'cookiejar': i},callback=self.parse_page)
			Keep in mind that the cookiejar meta key is not “sticky”. You need to keep passing it along on subsequent requests. For example:
			def parse_page(self, response):
				return scrapy.Request("http://www.example.com/otherpage", meta={'cookiejar': response.meta['cookiejar']}, callback=self.parse_other_page)

	class scrapy.downloadermiddlewares.defaultheaders.DefaultHeadersMiddleware
		This middleware sets all default requests headers specified in the DEFAULT_REQUEST_HEADERS setting 

	class scrapy.downloadermiddlewares.downloadtimeout.DownloadTimeoutMiddleware
		This middleware sets the download timeout for requests specified in the DOWNLOAD_TIMEOUT setting or download_timeout spider attribute.
		You can also set download timeout per-request using download_timeout Request.meta key; this is supported even when DownloadTimeoutMiddleware is disabled.

	class scrapy.downloadermiddlewares.httpauth.HttpAuthMiddleware	
		This middleware authenticates all requests generated from certain spiders using Basic access authentication
			Basic access authentication
				In the context of an HTTP transaction, basic access authentication is a method for an HTTP user agent to provide a user name and password when making a request
				HTTP BA implementation is the simplest technique for enforcing access controls to web resources 
				because it doesnt require cookies, session identifiers, or login pages; 
				rather, HTTP BA uses standard fields in the HTTP header, obviating the need for handshakes.
				The BA mechanism provides no confidentiality protection for the transmitted credentials. 
				They are merely encoded with Base64 in transit, but not encrypted or hashed in any way. 
				HTTPS is, therefore, typically preferred used in conjunction with BA
		To enable HTTP authentication from certain spiders, set the http_user and http_pass attributes of those spiders.
		class SomeIntranetSiteSpider(CrawlSpider):
			http_user = 'someuser'
			http_pass = 'somepass'

	class scrapy.downloadermiddlewares.httpcache.HttpCacheMiddleware
		This middleware provides low-level cache to all HTTP requests and responses. 
		It has to be combined with a cache storage backend as well as a cache policy.

		Scrapy ships with two HTTP cache storage backends: 
			Filesystem storage backend (default)
				File system storage backend is available for the HTTP cache middleware.
				Each request/response pair is stored in a different directory containing the following files:
					request_body - the plain request body
					request_headers - the request headers (in raw HTTP format)
					response_body - the plain response body
					response_headers - the request headers (in raw HTTP format)
					meta - some metadata of this cache resource in Python repr() format (grep-friendly format)
					pickled_meta - the same metadata in meta but pickled for more efficient deserialization
			DBM storage backend
					By default, it uses the anydbm module, but you can change it with the HTTPCACHE_DBM_MODULE setting.
			LevelDB storage backend

			setting:
				 HTTPCACHE_STORAGE

		Scrapy ships with two HTTP cache policies
			RFC2616 policy
				This policy provides a RFC2616 compliant HTTP cache, i.e. with HTTP Cache-Control awareness, aimed at production and used in continuous runs to avoid downloading unmodified data (to save bandwidth and speed up crawls).
			Dummy policy (default)
				This policy has no awareness of any HTTP Cache-Control directives. Every request and its corresponding response are cached. When the same request is seen again, the response is returned without transferring anything from the Internet.
			setting:
				 HTTPCACHE_POLICY
				 You can also avoid caching a response on every policy using dont_cache meta key equals True
		
		HTTPCache middleware settings
			HTTPCACHE_ENABLED
				Default: False
				Whether the HTTP cache will be enabled.

			HTTPCACHE_EXPIRATION_SECS
				Default: 0
				Expiration time for cached requests, in seconds.
				Cached requests older than this time will be re-downloaded. If zero, cached requests will never expire.

			HTTPCACHE_DIR
				Default: 'httpcache'
				The directory to use for storing the (low-level) HTTP cache. If empty, the HTTP cache will be disabled. If a relative path is given, is taken relative to the project data dir.

			HTTPCACHE_IGNORE_HTTP_CODES
				Default: []
				Don’t cache response with these HTTP codes.
			HTTPCACHE_IGNORE_MISSING
				Default: False
				If enabled, requests not found in the cache will be ignored instead of downloaded.

			HTTPCACHE_IGNORE_SCHEMES
				Default: ['file']
				Don’t cache responses with these URI schemes.

			HTTPCACHE_STORAGE
				Default: 'scrapy.extensions.httpcache.FilesystemCacheStorage'
				The class which implements the cache storage backend.

			HTTPCACHE_DBM_MODULE
				Default: 'anydbm'
				The database module to use in the DBM storage backend. This setting is specific to the DBM backend.

			HTTPCACHE_POLICY
				Default: 'scrapy.extensions.httpcache.DummyPolicy'
				The class which implements the cache policy.

			HTTPCACHE_GZIP
				Default: False
				If enabled, will compress all cached data with gzip. This setting is specific to the Filesystem backend.

			HTTPCACHE_ALWAYS_STORE
				Default: False
				If enabled, will cache pages unconditionally.
				A spider may wish to have all responses available in the cache, for future use with Cache-Control: max-stale, for instance. 
				The DummyPolicy caches all responses but never revalidates them, and sometimes a more nuanced policy is desirable.
				This setting still respects Cache-Control: no-store directives in responses. 
				If you don’t want that, filter no-store out of the Cache-Control headers in responses you feedto the cache middleware.

			HTTPCACHE_IGNORE_RESPONSE_CACHE_CONTROLS
				Default: []
				List of Cache-Control directives in responses to be ignored.
				Sites often set “no-store”, “no-cache”, “must-revalidate”, etc., but get upset at the traffic a spider can generate if it respects those directives. 
				This allows to selectively ignore Cache-Control directives that are known to be unimportant for the sites being crawled.
				We assume that the spider will not issue Cache-Control directives in requests unless it actually needs them, so directives in requests are not filtered.

	class scrapy.downloadermiddlewares.httpcompression.HttpCompressionMiddleware
		This middleware allows compressed (gzip, deflate) traffic to be sent/received from web sites.
		COMPRESSION_ENABLED¶
			Default: True
			Whether the Compression middleware will be enabled.

	class scrapy.downloadermiddlewares.chunked.ChunkedTransferMiddleware
		This middleware adds support for chunked transfer encoding
	
	class scrapy.downloadermiddlewares.httpproxy.HttpProxyMiddleware
		This middleware sets the HTTP proxy to use for requests, by setting the proxy meta value for Request objects.
		Like the Python standard library modules urllib and urllib2, it obeys the following environment variables:
			http_proxy
			https_proxy
			no_proxy
		You can also set the meta key "proxy" per-request, to a value like http://some_proxy_server:port.

	class scrapy.downloadermiddlewares.redirect.RedirectMiddleware
		This middleware handles redirection of requests based on response status.
		The urls which the request goes through (while being redirected) can be found in the redirect_urls Request.meta key.
		setting:
			REDIRECT_ENABLED
				Default: True
				Whether the Redirect middleware will be enabled.
			REDIRECT_MAX_TIMES
				Default: 20
				The maximum number of redirections that will be follow for a single request.
			Request.meta
				dont_redirect=True			:the request will be ignored by this middleware.
				handle_httpstatus_list		:ignore 301 and 302 responses 
				handle_httpstatus_all=True	:allow any response code for a request.
			spider attr
				handle_httpstatus_list
	
	class scrapy.downloadermiddlewares.redirect.MetaRefreshMiddleware
		This middleware handles redirection of requests based on meta-refresh html tag.
		METAREFRESH_ENABLED
			Default: True
			Whether the Meta Refresh middleware will be enabled.
		METAREFRESH_MAXDELAY
			Default: 100
			the maximum meta-refresh delay (in seconds) to follow the redirection. 
			Some sites use meta-refresh for redirecting to a session expired page, 
			so we restrict automatic redirection to the maximum delay

	class scrapy.downloadermiddlewares.retry.RetryMiddleware
		A middleware to retry failed requests that are potentially caused by temporary problems such as a connection timeout or HTTP 500 error
		RETRY_ENABLED
			Default: True
			Whether the Retry middleware will be enabled.

		RETRY_TIMES
			Default: 2
			Maximum number of times to retry, in addition to the first download.

		RETRY_HTTP_CODES
			Default: [500, 502, 503, 504, 408]
			Which HTTP response codes to retry. Other errors (DNS lookup issues, connections lost, etc) are always retried.
			In some cases you may want to add 400 to RETRY_HTTP_CODES because it is a common code used to indicate server overload. 
			It is not included by default because HTTP specs say so.

	class scrapy.downloadermiddlewares.robotstxt.RobotsTxtMiddleware
		This middleware filters out requests forbidden by the robots.txt exclusion standard.
		ROBOTSTXT_OBEY
		If Request.meta has dont_obey_robotstxt key set to True the request will be ignored by this middleware even if ROBOTSTXT_OBEY is enabled.
	
	class scrapy.downloadermiddlewares.stats.DownloaderStats
		Middleware that stores stats of all requests, responses and exceptions that pass through it. 
		To use this middleware you must enable the DOWNLOADER_STATS setting.

	class scrapy.downloadermiddlewares.useragent.UserAgentMiddleware
		Middleware that allows spiders to override the default user agent.
		In order for a spider to override the default user agent, its user_agent attribute must be set.

	class scrapy.downloadermiddlewares.ajaxcrawl.AjaxCrawlMiddleware
		Scrapy finds ‘AJAX crawlable’ pages for URLs like 'http://example.com/!#foo=bar' even without this middleware. AjaxCrawlMiddleware is necessary when URL doesn’t contain '!#'. 
		AJAXCRAWL_ENABLED
			Default: False
			Whether the AjaxCrawlMiddleware will be enabled. You may want to enable it for broad crawls.
