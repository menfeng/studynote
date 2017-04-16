1.Overview
					 ------>scheduler--------------
					|			|				   |						   
				request			|				   |						   
					|			|					--------->--------------
					|			|											|
	item_pipeline-------------engine---------downloader_middlewares-----downloader--------internet
		|			|			|											|
		|			|			|											|
		|			|			|											|
	   item			|	spider_middlewares	 ---------response------<-------
		|			|			|			|
		|			|			|			|
		 ----------- ---------spider<-------

	Scrapy Engine
		The engine is responsible for controlling the data flow between all components of the system, and triggering events when certain actions occur. 
	Scheduler
		The Scheduler receives requests from the engine and enqueues them for feeding them later  when the engine requests them.
	Downloader
		The Downloader is responsible for fetching web pages and feeding them to the engine which, in turn, feeds them to the spiders.
	Spiders
		Spiders are custom classes written by Scrapy users to parse responses and extract items (aka scraped items) from them or additional URLs (requests) to follow. 
	Item Pipeline
		The Item Pipeline is responsible for processing the items once they have been extracted (or scraped) by the spiders. 
	Downloader middlewares
		Downloader middlewares are specific hooks that sit between the Engine and the Downloader 
		and process requests when they pass from the Engine to the Downloader, 
		and responses that pass from Downloader to the Engine.
		Use a Downloader middleware if you need to do one of the following:
			process a request just before it is sent to the Downloader (i.e. right before Scrapy sends the request to the website);
			change received response before passing it to a spider;
			send a new Request instead of passing received response to a spider;
			pass response to a spider without fetching a web page;
			silently drop some requests.
			For more information see Downloader Middleware.
	Spider middlewares
		Spider middlewares are specific hooks that sit between the Engine and the Spiders 
		and are able to process spider input (responses) and output (items and requests).
		Use a Spider middleware if you need to
			post-process output of spider callbacks - change/add/remove requests or items;
			post-process start_requests;
			handle spider exceptions;
			call errback instead of callback for some of the requests based on response content.

2.Data flow
	rq:request
	rs:response
	it:item
	ak:ask
	1.spide---rq--->engine---rq--->scheduler
	2.engine---ak-rq-->scheduler
	3.scheduler---rq--->engine---rq---middleware--->downloader
	4.downloader---rs--->middleware--->engine
	5.engine--->middleware--->spider
	6.spider---it-rs--->middleware--->engine
	7.engine---it---->pipeline;engine---re---->scheduler
	8.1-7

3.Event-driven networking
	Scrapy is written with Twisted, a popular event-driven networking framework for Python.
	Thus, it¡¯s implemented using a non-blocking (aka asynchronous) code for concurrency.
