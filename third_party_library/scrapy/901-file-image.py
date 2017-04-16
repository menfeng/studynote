Downloading and processing files and images

1.Using the Files Pipeline
	The typical workflow, when using the FilesPipeline goes like this:
	1.In a Spider, you scrape an item and put the URLs of the desired into a file_urls field.
	2.The item is returned from the spider and goes to the item pipeline.
	3.When the item reaches the FilesPipeline, the URLs in the file_urls field are scheduled for download using the standard Scrapy scheduler and downloader 
	(which means the scheduler and downloader middlewares are reused), but with a higher priority, 
	processing them before other pages are scraped. The item remains “locked” at that particular 
	pipeline stage until the files have finish downloading (or fail for some reason).
	4.When the files are downloaded, another field (files) will be populated with the results. 
	This field will contain a list of dicts with information about the downloaded files, 
	such as the downloaded path, the original scraped url (taken from the file_urls field) , 
	and the file checksum. The files in the list of the files field will retain the same order 
	of the original file_urls field. If some file failed downloading, 
	an error will be logged and the file won’t be present in the files field.

2.Using the Images Pipeline
	Using the ImagesPipeline is a lot like using the FilesPipeline
	you use image_urls for the image URLs of an item and it will populate an images field for the information about the downloaded images.

3.Enabling your Media Pipeline
	ITEM_PIPELINES = {'scrapy.pipelines.images.ImagesPipeline': 1}
	ITEM_PIPELINES = {'scrapy.pipelines.files.FilesPipeline': 1}
	FILES_STORE = '/path/to/valid/dir'
	IMAGES_STORE = '/path/to/valid/dir'

4.Change Field Names
	FILES_URLS_FIELD = 'field_name_for_your_files_urls'
	FILES_RESULT_FIELD = 'field_name_for_your_processed_files'
	IMAGES_URLS_FIELD = 'field_name_for_your_images_urls'
	IMAGES_RESULT_FIELD = 'field_name_for_your_processed_images'

5.Supported Storage
	The files are stored using a SHA1 hash of their URLs for the file names.
	<IMAGES_STORE>/full/3afec3b4765f8f0a07b78f98c07b83f013567a0a.jpg

6.File expiration
	The Image Pipeline avoids downloading files that were downloaded recently. To adjust this retention delay use the FILES_EXPIRES setting (or IMAGES_EXPIRES, in case of Images Pipeline), which specifies the delay in number of days:
	# 120 days of delay for files expiration
	FILES_EXPIRES = 120
	# 30 days of delay for images expiration
	IMAGES_EXPIRES = 30

7.Thumbnail generation for images
	IMAGES_THUMBS = {
		'small': (50, 50),
		'big': (270, 270),
	}
	<IMAGES_STORE>/thumbs/<size_name>/<image_id>.jpg
		<IMAGES_STORE>/full/63bbfea82b8880ed33cdb762aa11fab722a90a24.jpg
		<IMAGES_STORE>/thumbs/small/63bbfea82b8880ed33cdb762aa11fab722a90a24.jpg
		<IMAGES_STORE>/thumbs/big/63bbfea82b8880ed33cdb762aa11fab722a90a24.jpg
8.Filtering out small images
	IMAGES_MIN_HEIGHT = 110
	IMAGES_MIN_WIDTH = 110

9.Extending the Media Pipelines
	item-->FilesPipeline.get_media_requests--all download-->item_completed-->return(drop) item-->subsequent item pipeline
	See here the methods that you can override in your custom Files Pipeline
	class scrapy.pipelines.files.FilesPipeline
		def get_media_requests(self, item, info):
			for file_url in item['file_urls']:
				yield scrapy.Request(file_url)

		def item_completed(self, results, item, info):
			file_paths = [x['path'] for ok, x in results if ok]
			if not file_paths:
				raise DropItem("Item contains no files")
			item['file_paths'] = file_paths
			return item
		results:(success,file_info_or_error)
			url - the url where the file was downloaded from. This is the url of the request returned from the get_media_requests() method.
			path - the path (relative to FILES_STORE) where the file was stored
			checksum - a MD5 hash of the image contents

	class scrapy.pipelines.images.ImagesPipeline
		you can override get_media_requests and item_completed like above
