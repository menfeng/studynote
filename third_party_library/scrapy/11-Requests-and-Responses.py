1.Request objects
class scrapy.http.Request(url[, callback, method='GET', headers, body, cookies, meta, encoding='utf-8', priority=0, dont_filter=False, errback])
	url(string)				the URL of this request
	callback (callable) 	the function that will be called
	method (string)			the HTTP method of this request. Defaults to 'GET'.
	meta (dict)  			the initial values for the Request.meta attribute. If given, the dict passed in this parameter will be shallow copied.
	body (str or unicode)  	the request body
	headers (dict)  		the headers of this request
	cookies (dict or list) 
							Using a dict:
								request_with_cookies = Request(url="http://www.example.com",cookies={'currency': 'USD', 'country': 'UY'})
							Using a list of dicts: this form allows for customizing the domain and path attributes of the cookie. This is only useful if the cookies are saved for later requests.
								request_with_cookies = Request(url="http://www.example.com", cookies=[{'name': 'currency', 'value': 'USD', 'domain': 'example.com', 'path': '/currency'}])

							When some site returns cookies (in a response) those are stored in the cookies for that domain and will be sent again in future requests. 
							avoid merging with existing cookies
								request_with_cookies = Request(url="http://www.example.com", cookies={'currency': 'USD', 'country': 'UY'}, meta={'dont_merge_cookies': True})
	encoding (string) 		the encoding of this request (defaults to 'utf-8'). This encoding will be used to percent-encode the URL and to convert the body to str (if given as unicode).
	priority (int)  		the priority of this request (defaults to 0). 
							The priority is used by the scheduler to define the order used to process requests. 
							Requests with a higher priority value will execute earlier. 
							Negative values are allowed in order to indicate relatively low-priority.
	dont_filter (boolean)	indicates that this request should not be filtered by the scheduler. 
							This is used when you want to perform an identical request multiple times, to ignore the duplicates filter. 
							Use it with care, or you will get into crawling loops. Default to False.
	errback (callable)  	a function that will be called if any exception was raised while processing the request. 
							This includes pages that failed with 404 HTTP errors and such. 
							It receives a Twisted Failure instance as first parameter.
	------------------------
	url
	method
	headers
	body
	meta
		This dict is shallow copied when the request is cloned using the copy() or replace() methods, and can also be accessed, in your spider, from the response.meta attribute.
	copy()
	replace([url, method, headers, body, cookies, meta, encoding, dont_filter, callback, errback])

2.Passing additional data to callback functions
	Here¡¯s an example of how to pass an item using this mechanism, to populate different fields from different pages:
	def parse_page1(self, response):
		item = MyItem()
		item['main_url'] = response.url
		request = scrapy.Request("http://www.example.com/some_page.html", callback=self.parse_page2)
		request.meta['item'] = item
		return request

	def parse_page2(self, response):
		item = response.meta['item']
		item['other_url'] = response.url
		return item

3.Using errbacks to catch exceptions in request processing
	Here¡¯s an example spider logging all errors and catching some specific errors if needed
	import scrapy
	from scrapy.spidermiddlewares.httperror import HttpError
	from twisted.internet.error import DNSLookupError
	from twisted.internet.error import TimeoutError, TCPTimedOutError

	class ErrbackSpider(scrapy.Spider):
		name = "errback_example"
		start_urls = [
			"http://www.httpbin.org/",              # HTTP 200 expected
			"http://www.httpbin.org/status/404",    # Not found error
			"http://www.httpbin.org/status/500",    # server issue
			"http://www.httpbin.org:12345/",        # non-responding host, timeout expected
			"http://www.httphttpbinbin.org/",       # DNS error expected
		]

		def start_requests(self):
			for u in self.start_urls:
				yield scrapy.Request(u, callback=self.parse_httpbin,
										errback=self.errback_httpbin,
										dont_filter=True)

		def parse_httpbin(self, response):
			self.logger.info('Got successful response from {}'.format(response.url))
			# do something useful here...

		def errback_httpbin(self, failure):
			# log all failures
			self.logger.error(repr(failure))

			# in case you want to do something special for some errors,
			# you may need the failure's type:

			if failure.check(HttpError):
				# these exceptions come from HttpError spider middleware
				# you can get the non-200 response
				response = failure.value.response
				self.logger.error('HttpError on %s', response.url)

			elif failure.check(DNSLookupError):
				# this is the original request
				request = failure.request
				self.logger.error('DNSLookupError on %s', request.url)

			elif failure.check(TimeoutError, TCPTimedOutError):
				request = failure.request
				self.logger.error('TimeoutError on %s', request.url)


4.Request.meta special keys
	The Request.meta attribute can contain any arbitrary data, but there are some special keys recognized by Scrapy and its built-in extensions.
	Those are:
		dont_redirect
		dont_retry
		handle_httpstatus_list
		handle_httpstatus_all
		dont_merge_cookies (see cookies parameter of Request constructor)
		cookiejar
		dont_cache
		redirect_urls
		bindaddress
			The IP of the outgoing IP address to use for the performing the request.
		dont_obey_robotstxt
			The amount of time (in secs) that the downloader will wait before timing out.
		download_maxsize
		proxy

5.Request subclasses
	FormRequest objects
		It uses lxml.html forms to pre-populate form fields with form data from Response objects.
		class scrapy.http.FormRequest(url[, formdata, ...])
			formdata (dict or iterable of tuples) is a dictionary (or iterable of (key, value) tuples) 
			containing HTML Form data which will be url-encoded and assigned to the body of the request.
			classmethod from_response(response[, formname=None, formnumber=0, formdata=None, formxpath=None, formcss=None, clickdata=None, dont_click=False, ...])
				response(Response object)	the response containing a HTML form which will be used to pre-populate the form fields
				formname (string)  			if given, the form with name attribute set to this value will be used.
				formxpath (string)  		if given, the first form that matches the xpath will be used.
				formcss (string)  			if given, the first form that matches the css selector will be used.
				formnumber (integer)  		the number of form to use, when the response contains multiple forms. The first one (and also the default) is 0.
				formdata (dict)  			fields to override in the form data. If a field was already present in the response <form> element, its value is overridden by the one passed in this parameter.
				clickdata (dict)  			attributes to lookup the control clicked. 
											If it¡¯s not given, the form data will be submitted simulating a click on the first clickable element. 
											In addition to html attributes, the control can be identified by its zero-based index relative to other submittable inputs inside the form, via the nr attribute.
				dont_click (boolean)  		If True, the form data will be submitted without clicking in any element.

6.Request usage examples
	Using FormRequest to send data via HTTP POST
		simulate a HTML Form POST in your spider and send a couple of key-value fields
		return [FormRequest(url="http://www.example.com/post/action", formdata={'name': 'John Doe', 'age': '27'}, callback=self.after_post)]
	Using FormRequest.from_response() to simulate a user login
		import scrapy
		class LoginSpider(scrapy.Spider):
			name = 'example.com'
			start_urls = ['http://www.example.com/users/login.php']

			def parse(self, response):
				return scrapy.FormRequest.from_response(
					response,
					formdata={'username': 'john', 'password': 'secret'},
					callback=self.after_login
				)

			def after_login(self, response):
				# check login succeed before going on
				if "authentication failed" in response.body:
					self.logger.error("Login failed")
					return

				# continue scraping with authenticated session...

7.Response objects
	class scrapy.http.Response(url[, status=200, headers, body, flags])
		url (string)  		the URL of this response
		headers (dict)  	the headers of this response. The dict values can be strings (for single valued headers) or lists (for multi-valued headers).
		status (integer)  	the HTTP status of the response. Defaults to 200.
		body (str)  		the response body. It must be str, not unicode, unless you¡¯re using a encoding-aware Response subclass, such as TextResponse.
		meta (dict)  		the initial values for the Response.meta attribute. If given, the dict will be shallow copied.
		flags (list)  		is a list containing the initial values for the Response.flags attribute. If given, the list will be shallow copied.
	--------------
	url
	status
	headers
	body
	request
		The Request object that generated this response
	meta
	flags
		A list that contains flags for this response,example: ¡®ached, ¡®edirected¡®, etc
	copy()
	replace([url, status, headers, body, request, flags, cls])
	urljoin(url)
		like urlparse.urljoin(response.url, url)

8.Response subclasses
	class scrapy.http.TextResponse(url[, encoding[, ...]])
		TextResponse objects adds encoding capabilities to the base Response class, which is meant to be used only for binary data, such as images, sounds or any media file.
		encoding
			if you create a TextResponse object with a unicode body, it will be encoded using this encoding	
		----------------
		text
			the same as response.body.decode(response.encoding)
		encoding
			The encoding is resolved by trying the following mechanisms, in order:
				the encoding passed in the constructor encoding argument
				the encoding declared in the Content-Type HTTP header.
				the encoding declared in the response body.
				the encoding inferred by looking at the response body.
		selector
			A Selector instance using the response as target
		xpath(query)
		css(query)

	class scrapy.http.HtmlResponse(url[, ...])
		The HtmlResponse class is a subclass of TextResponse which adds encoding auto-discovering support by looking into the HTML meta http-equiv attribute.
	class scrapy.http.XmlResponse(url[, ...])
		The XmlResponse class is a subclass of TextResponse which adds encoding auto-discovering support by looking into the XML declaration line
