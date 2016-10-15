1.Item Pipeline
	item-->pipeline-->handle item
	uses:
	cleansing HTML data
	validating scraped data (checking that the items contain certain fields)
	checking for duplicates (and dropping them)
	storing the scraped item in a database

2.Writing your own item pipeline
	Each item pipeline component is a Python class that must implement the following method
	process_item(self, item, spider)
		This method is called for every item pipeline component and must either return a dict with data, Item (or any descendant class) object or raise a DropItem exception. Dropped items are no longer processed by further pipeline components.
	open_spider(self, spider)
		This method is called when the spider is opened.
	close_spider(self, spider)
		This method is called when the spider is closed.
	from_crawler(cls, crawler)
		If present, this classmethod is called to create a pipeline instance from a Crawler
3.Activating an Item Pipeline component
	the integer(0-1000) determine the executing order
	ITEM_PIPELINES = {
		'myproject.pipelines.PricePipeline': 300,
		'myproject.pipelines.JsonWriterPipeline': 800,
	}
4.Item pipeline example

	Price validation and dropping items with no prices
	from scrapy.exceptions import DropItem
	class PricePipeline(object):
		vat_factor = 1.15
		def process_item(self, item, spider):
			if item['price']:
				if item['price_excludes_vat']:
					item['price'] = item['price'] * self.vat_factor
				return item
			else:
				raise DropItem("Missing price in %s" % item)

	Write items to a JSON file
	import json
	class JsonWriterPipeline(object):
		def __init__(self):
			self.file = open('items.jl', 'wb')
		def process_item(self, item, spider):
			line = json.dumps(dict(item)) + "\n"
			self.file.write(line)
			return item

	Write items to MongoDB
	import pymongo

	class MongoPipeline(object):

		collection_name = 'scrapy_items'

		def __init__(self, mongo_uri, mongo_db):
			self.mongo_uri = mongo_uri
			self.mongo_db = mongo_db

		@classmethod
		def from_crawler(cls, crawler):
			return cls(
				mongo_uri=crawler.settings.get('MONGO_URI'),
				mongo_db=crawler.settings.get('MONGO_DATABASE', 'items')
			)

		def open_spider(self, spider):
			self.client = pymongo.MongoClient(self.mongo_uri)
			self.db = self.client[self.mongo_db]

		def close_spider(self, spider):
			self.client.close()

		def process_item(self, item, spider):
			self.db[self.collection_name].insert(dict(item))
			return item

	Duplicates filter
	from scrapy.exceptions import DropItem

	class DuplicatesPipeline(object):

		def __init__(self):
			self.ids_seen = set()

		def process_item(self, item, spider):
			if item['id'] in self.ids_seen:
				raise DropItem("Duplicate item found: %s" % item)
			else:
				self.ids_seen.add(item['id'])
				return item

