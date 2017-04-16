1.Item Loaders
	Items provide the container of scraped data, while Item Loaders provide the mechanism for populating that container.
2.Using Item Loaders to populate items
	from scrapy.loader import ItemLoader
	from myproject.items import Product
	def parse(self, response):
		l = ItemLoader(item=Product(), response=response)
		l.add_xpath('name', '//div[@class="product_name"]')
		l.add_xpath('name', '//div[@class="product_title"]')
		l.add_xpath('price', '//p[@id="price"]')
		l.add_css('stock', 'p#stock]')
		l.add_value('last_updated', 'today') # you can also use literal values
		return l.load_item()
3.Input and Output processors
	input processor	:collect and kept result inside the ItemLoader
	output processor:assign item fileds and return item
	processors are just callable objects,which are called with the data to be parsed, and return a parsed value

4.Declaring Item Loaders
	_in	:input processor
	_out:output processor
	default_input_processor
	default_output_processor
	from scrapy.loader import ItemLoader
	from scrapy.loader.processors import TakeFirst, MapCompose, Join
	class ProductLoader(ItemLoader):
		default_output_processor = TakeFirst()
		name_in = MapCompose(unicode.title)
		name_out = Join()
		price_in = MapCompose(unicode.strip)
5.Declaring Input and Output Processors
	processors:
		declared in the Item Loader definition like above
		declared in the Item Field
			import scrapy
			from scrapy.loader.processors import Join, MapCompose, TakeFirst
			from w3lib.html import remove_tags
			def filter_price(value):
				if value.isdigit():
					return value
			class Product(scrapy.Item):
				name = scrapy.Field(
					input_processor=MapCompose(remove_tags),
					output_processor=Join(),
				)
				price = scrapy.Field(
					input_processor=MapCompose(remove_tags, filter_price),
					output_processor=TakeFirst(),
				)
		precedence order:
			Item Loader field-specific attributes: field_in and field_out (most precedence)
			Field metadata (input_processor and output_processor key)
			Item Loader defaults: ItemLoader.default_input_processor() and ItemLoader.default_output_processor() (least precedence)
6.Item Loader Context
	The Item Loader Context is a dict of arbitrary key/values which is shared among all input and output processors in the Item Loader
	#loader_context telling the Item Loader that it’s able to receive an Item Loader context,
	def parse_length(text, loader_context):
		unit = loader_context.get('unit', 'm')
		# ... length parsing code goes here ...
		return parsed_length
	modify Item Loader context
	1.ItemLoader(product).context['unit'] = 'cm'
	2.loader = ItemLoader(product, unit='cm')
	3.
	class ProductLoader(ItemLoader):
		length_out = MapCompose(parse_length, unit='cm')

7.ItemLoader objects
class scrapy.loader.ItemLoader([item, selector, response, ]**kwargs)
	get_value(value, *processors, **kwargs)
	add_value(field_name, value, *processors, **kwargs)
	replace_value(field_name, value, *processors, **kwargs)o
	get_xpath(xpath, *processors, **kwargs)
	add_xpath(field_name, xpath, *processors, **kwargs)
	replace_xpath(field_name, xpath, *processors, **kwargs)
	get_css(css, *processors, **kwargs)
	add_css(field_name, css, *processors, **kwargs)
	replace_css(field_name, css, *processors, **kwargs)
	load_item()	The data collected is first passed through the output processors to get the final value to assign to each item field
	nested_xpath(xpath)
	nested_css(css)
	get_collected_values(field_name)
	get_output_value(field_name)
	get_input_processor(field_name)
	get_output_processor(field_name)

	item
	context
	default_item_class
	default_input_processor
	default_output_processor
	default_selector_class
	selector
8.Nested Loaders
	<footer>
		<a class="social" href="http://facebook.com/whatever">Like Us</a>
		<a class="social" href="http://twitter.com/whatever">Follow Us</a>
		<a class="email" href="mailto:whatever@example.com">Email Us</a>
	</footer>

	not nested
		loader = ItemLoader(item=Item())
		# load stuff not in the footer
		loader.add_xpath('social', '//footer/a[@class = "social"]/@href')
		loader.add_xpath('email', '//footer/a[@class = "email"]/@href')
		loader.load_item()
	nested
		loader = ItemLoader(item=Item())
		# load stuff not in the footer
		footer_loader = loader.nested_xpath('//footer')
		footer_loader.add_xpath('social', 'a[@class = "social"]/@href')
		footer_loader.add_xpath('email', 'a[@class = "email"]/@href')
		# no need to call footer_loader.load_item()
		loader.load_item()
9.Reusing and extending Item Loaders
	eg:remove 3 dashes ---Plasma TV---
	from scrapy.loader.processors import MapCompose
	from myproject.ItemLoaders import ProductLoader
	def strip_dashes(x):
		return x.strip('-')
	class SiteSpecificLoader(ProductLoader):
    	name_in = MapCompose(strip_dashes, ProductLoader.name_in)
10.Available built-in processors
	class scrapy.loader.processors.Identity
		The simplest processor, which doesn’t do anything.
	class scrapy.loader.processors.TakeFirst
		Returns the first non-null/non-empty value from the values received, so it’s typically used as an output processor to single-valued fields
	class scrapy.loader.processors.Join(separator=u' ')
		Returns the values joined with the separator given in the constructor,
	class scrapy.loader.processors.Compose(*functions, **default_loader_context)
		A processor which is constructed from the composition of the given functions
	class scrapy.loader.processors.MapCompose(*functions, **default_loader_context)
		This processor provides a convenient way to compose functions that only work with single values 
	class scrapy.loader.processors.SelectJmes(json_path)
		Queries the value using the json path provided to the constructor and returns the output. 
		import json
		proc_single_json_str = Compose(json.loads, SelectJmes("foo"))
		proc_single_json_str('{"foo": "bar"}')
		u'bar'
