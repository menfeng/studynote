1.Scrapy shell
	The Scrapy shell is an interactive shell where you can try and debug your scraping code very quickly, without having to run the spider
	recommend ipython
2.Configuring the shell
	1.SCRAPY_PYTHON_SHELL environment variable
	2.scrapy.cfg
		[settings]
		shell = bpython
3.Launch the shell
	scrapy shell <url>
4.Available Shortcuts
	shelp()
	fetch(request_or_url)
	view(response)	:open the given response in your local web browser
5.Available Scrapy objects
	crawler
	spider
	request
	response
	settings
6.Invoking the shell from spiders to inspect responses
	after you leave the shell, the spider will continue crawling where it stopped
	def parse(self, response):
		if ".org" in response.url:
			from scrapy.shell import inspect_response
			inspect_response(response, self)

