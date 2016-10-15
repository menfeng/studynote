"""
libraries to extract data from the HTML source
	BeautifulSoup	:is a very popular web scraping library among Python programmers which constructs a Python object based on the structure of the HTML code and also deals with bad markup reasonably well, but it has one drawback: it¡¯s slow.
	lxml			:is an XML parsing library (which also parses HTML) with a pythonic API based on ElementTree. (lxml is not part of the Python standard library.)
	Scrapy comes with its own mechanism for extracting data--Selector
"""

"""
	Constructing selectors
"""
from scrapy.selector import Selector
from scrapy.http import HtmlResponse
body = '<html><body><span>good</span></body></html>'
Selector(text=body).xpath('//span/text()').extract()

HtmlResponse(url='http://example.com', body=body)
Selector(response=response).xpath('//span/text()').extract()

response.selector.xpath('//span/text()').extract()


"""
Using selectors

<html>
 <head>
  <base href='http://example.com/' />
  <title>Example website</title>
 </head>
 <body>
  <div id='images'>
   <a href='image1.html'>Name: My image 1 <br /><img src='image1_thumb.jpg' /></a>
   <a href='image2.html'>Name: My image 2 <br /><img src='image2_thumb.jpg' /></a>
   <a href='image3.html'>Name: My image 3 <br /><img src='image3_thumb.jpg' /></a>
   <a href='image4.html'>Name: My image 4 <br /><img src='image4_thumb.jpg' /></a>
   <a href='image5.html'>Name: My image 5 <br /><img src='image5_thumb.jpg' /></a>
  </div>
 </body>
</html>

scrapy shell http://doc.scrapy.org/en/latest/_static/selectors-sample1.html

response.selector.xpath('//title/text()')
	[<Selector (text) xpath=//title/text()>]
response.xpath('//title/text()')
	[<Selector (text) xpath=//title/text()>]
response.css('title::text')
	[<Selector (text) xpath=//title/text()>]

css+xpath
response.css('img').xpath('@src').extract()
	[u'image1_thumb.jpg', u'image2_thumb.jpg', u'image3_thumb.jpg', u'image4_thumb.jpg', u'image5_thumb.jpg']

extract_first
response.xpath('//div[@id="images"]/a/text()').extract_first()
	u'Name: My image 1 '
response.xpath('//div[@id="not-exists"]/text()').extract_first() is None
	True
response.xpath('//div[@id="not-exists"]/text()').extract_first(default='not-found')
	'not-found'

attr
response.xpath('//base/@href').extract()
	[u'http://example.com/']
response.css('base::attr(href)').extract()
	[u'http://example.com/']

contains[attr,tag]
response.xpath('//a[contains(@href, "image")]/@href').extract()
	[u'image1.html', u'image2.html', u'image3.html', u'image4.html', u'image5.html']
response.css('a[href*=image]::attr(href)').extract()
	[u'image1.html', u'image2.html', u'image3.html', u'image4.html', u'image5.html']
response.xpath('//a[contains(@href, "image")]/img/@src').extract()
	[u'image1_thumb.jpg', u'image2_thumb.jpg', u'image3_thumb.jpg', u'image4_thumb.jpg', u'image5_thumb.jpg']
response.css('a[href*=image] img::attr(src)').extract()
	[u'image1_thumb.jpg', u'image2_thumb.jpg', u'image3_thumb.jpg', u'image4_thumb.jpg', u'image5_thumb.jpg']

Nesting selectors
	links=response.xpath('//a[contains(@href, "image")]')
	links[0].xpath('@href').extract()

Using selectors with regular expressions
	re() returns a list of unicode strings. So you can¡¯t construct nested .re() calls.
	response.xpath('//a[contains(@href, "image")]/text()').re(r'Name:\s*(.*)')
		[u'My image 1', u'My image 2', u'My image 3', u'My image 4', u'My image 5']
	 response.xpath('//a[contains(@href, "image")]/text()').re_first(r'Name:\s*(.*)')
		u'My image 1'

Working with relative XPaths
Keep in mind that if you are nesting selectors and use an XPath that starts with /, that XPath will be absolute to the document and not relative to the Selector you¡¯re calling it from.
divs = response.xpath('//div') 	#all div in doc
divs[0].xpath('//p')			#all p in doc
divs[0].xpath('.//p')			#all p inside div
divs[0].xpath('p')				#all direct p children
"""

"""
Using EXSLT extensions
	Being built atop lxml, Scrapy selectors also support some EXSLT extensions and come with these pre-registered namespaces to use in XPath expressions
	Regular expressions
		C library libxslt doesn¡¯t natively support EXSLT regular expressions so lxml¡®s implementation uses hooks to Python¡¯s re module. Thus, using regexp functions in your XPath expressions may add a small performance penalty.

	Set operations
	sels=sel.xpath('//div[@itemscope]')
	sels[0].xpath('set:difference(./descendant::*/@itemprop,.//*[@itemscope]/*/@itemprop))
"""
from scrapy import Selector
doc="""
<div>
 <ul>
	 <li class="item-0"><a href="link1.html">first item</a></li>
	 <li class="item-1"><a href="link2.html">second item</a></li>
	 <li class="item-inactive"><a href="link3.html">third item</a></li>
	 <li class="item-1"><a hout/resref="link4.html">fourth item</a></li>
	 <li class="item-0"><a href="link5.html">fifth item</a></li>
 </ul>
</div>
"""
sel = Selector(text=doc, type="html")
sel.xpath('//li//@href').extract()
	[u'link1.html', u'link2.html', u'link3.html', u'link4.html', u'link5.html']
sel.xpath('//li[re:test(@class, "item-\d$")]//@href').extract()
	[u'link1.html', u'link2.html', u'link4.html', u'link5.html']

"""
Some XPath tips
	XPath tutorial:http://www.zvon.org/comp/r/tut-XPath_1.html

	1.Using text nodes in a condition
	sel = Selector(text='<a href="#">Click here to go to the <strong>Next Page</strong></a>')
	sel.xpath('//a//text()').extract()				#take a peek at the node-set
		[u'Click here to go to the ', u'Next Page']
	sel.xpath("string(//a[1]//text())").extract()	#convert it to string
		[u'Click here to go to the ']
	
	sel.xpath("//a[1]").extract() 					#select the first node
		[u'<a href="#">Click here to go to the <strong>Next Page</strong></a>']
	sel.xpath("string(//a[1])").extract() 			#a node converted to a string, however, puts together the text of itself plus of all its descendants
		[u'Click here to go to the Next Page']

	using the .//text() node-set won¡¯t select anything in this case:
	sel.xpath("//a[contains(.//text(), 'Next Page')]").extract()
		[]
	sel.xpath("//a[contains(., 'Next Page')]").extract()
		[u'<a href="#">Click here to go to the <strong>Next Page</strong></a>']
	
	2.Beware of the difference between //node[1] and (//node)[1]
	//node[1] selects all the nodes occurring first under their respective parents.
	(//node)[1] selects all the nodes in the document, and then gets only the first of them
	<ul class="list">
	 <li>1</li>
	 <li>2</li>
	 <li>3</li>
	</ul>
	<ul class="list">
	 <li>4</li>
	 <li>5</li>
	 <li>6</li>
	</ul>
	xp("//li[1]")		#This gets all first <li> elements under whatever it is its parent
		[u'<li>1</li>', u'<li>4</li>']
	xp("(//li)[1]")		#And this gets the first <li> element in the whole document
	 	[u'<li>1</li>']
	xp("//ul/li[1]")	#This gets all first <li> elements under an <ul> parent
		[u'<li>1</li>', u'<li>4</li>']
	xp("(//ul/li)[1]")	#And this gets the first <li> element under an <ul> parent in the whole document
		[u'<li>1</li>']

	3.When querying by class, consider using CSS
	@class='someclass'				:you may end up missing elements that have other classes,
	contains(@class, 'someclass') 	:to make up for that you may end up with more elements that you want(if 'someclass' in @class)
	from scrapy import Selector
	sel = Selector(text='<div class="hero shout"><time datetime="2014-07-23 19:00">Special date</time></div>')
	sel.css('.shout').xpath('./time/@datetime').extract()
		[u'2014-07-23 19:00']

"""


"""
Built-in Selectors reference
1.class scrapy.selector.Selector(response=None, text=None, type=None)
	response is an HtmlResponse or an XmlResponse object that will be used for selecting and extracting data.
	text is a unicode string or utf-8 encoded text for cases when a response isn¡¯t available. Using text and response together is undefined behavior.
	type defines the selector type, it can be "html", "xml" or None (default).
		xpath(query)
		css(query)
		extract()
		re(regex)
		register_namespace(prefix, uri)
		remove_namespaces()
		__nonzero__()
2.class scrapy.selector.SelectorList
	The SelectorList class is a subclass of the builtin list class, which provides a few additional methods
		xpath(query)
		css(query)
		extract()
		re()
		__nonzero__()
	Selector examples on HTML response
		sel = Selector(html_response)
		sel.xpath("//h1")
		sel.xpath("//h1").extract()         # this includes the h1 tag
		sel.xpath("//h1/text()").extract()  # this excludes the h1 tag
"""
