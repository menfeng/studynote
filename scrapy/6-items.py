1.Declaring Items
	class Product(scrapy.Item):
		name = scrapy.Field()
		price = scrapy.Field()
		stock = scrapy.Field()
		last_updated = scrapy.Field(serializer=str)
2.Item Fields
	Field objects are used to specify metadata for each field. For example, the serializer function for the last_updated field illustrated in the example above.
3.Working with Items
	use it just like dict in python
		product = Product(name='Desktop PC', price=1000)
		product2 = Product(product)
		product3 = product2.copy()
		dict(product)
		Product({'name': 'Laptop PC', 'price': 1500})
	extending Items:
		class DiscountedProduct(Product):
			discount_percent = scrapy.Field(serializer=str)
			discount_expiration_date = scrapy.Field()
		class SpecificProduct(Product):
			name = scrapy.Field(Product.fields['name'], serializer=my_serializer)
4.Item objects
	class scrapy.item.Item([arg])
		Items replicate the standard dict API, including its constructor. The only additional attribute provided by Items is:
			fields:A dictionary containing all declared fields for this Item, not only those populated. The keys are the field names and the values are the Field objects used in the Item declaration.
5.Field objects
	class scrapy.item.Field([arg])
		Field objects are plain-old Python dicts

