import scrapy
from tutorial.items import DmozItem

class DmozSpider(scrapy.Spider):
    name = "360"
    allowed_domains = ["360.cn"]
    start_urls = [
        "https://hao.360.cn/",
    ]

    def parse(self, response):
		t=response.xpath('//title/text()').extract()
		d=response.xpath('//meta[@name="description"]/@content').extract()
		l=response.xpath('//link[@rel="dns-prefetch"]/@href').extract()
		item = DmozItem()
		item['title'] = t
		item['desc'] =d
		item['link'] =l 
		yield item
