1.Feed exports
	a export file commonly called export feed
2.Serialization formats
	JSON
		FEED_FORMAT: json
		Exporter used: JsonItemExporter
	JSON lines
		FEED_FORMAT: jsonlines
		Exporter used: JsonLinesItemExporter
	CSV
		FEED_FORMAT: csv
		Exporter used: CsvItemExporter
	XML
		FEED_FORMAT: xml
		Exporter used: XmlItemExporter
	Pickle
		FEED_FORMAT: pickle
		Exporter used: PickleItemExporter
	Marshal
		FEED_FORMAT: marshal
		Exporter used: MarshalItemExporter
3.Storages
	define where to store the feed using a URI (through the FEED_URI setting)
 	Storage backends
		Local filesystem
			URI scheme: file
			Example URI: file:///tmp/export.csv
		FTP
			URI scheme: ftp
			Example URI: ftp://user:pass@ftp.example.com/path/to/export.csv
		S3
			URI scheme: s3
			Example URIs:
				s3://mybucket/path/to/export.csv
				s3://aws_key:aws_secret@mybucket/path/to/export.csv
			Required external libraries: botocore or boto
		Standard output
			URI scheme: stdout
			Example URI: stdout:
	Settings
		FEED_URI
			The URI of the export feed. See Storage backends for supported URI schemes.
		FEED_FORMAT
			The serialization format to be used for the feed. See Serialization formats for possible values.
		FEED_EXPORT_FIELDS
			A list of fields to export, optional. Example: FEED_EXPORT_FIELDS = ["foo", "bar", "baz"]
		FEED_STORE_EMPTY
			Whether to export empty feeds
		FEED_STORAGES
			A dict containing additional feed storage backends supported by your project. The keys are URI schemes and the values are paths to storage classes.
		FEED_STORAGES_BASE
			{
				'': 'scrapy.extensions.feedexport.FileFeedStorage',
				'file': 'scrapy.extensions.feedexport.FileFeedStorage',
				'stdout': 'scrapy.extensions.feedexport.StdoutFeedStorage',
				's3': 'scrapy.extensions.feedexport.S3FeedStorage',
				'ftp': 'scrapy.extensions.feedexport.FTPFeedStorage',
			}
			A dict containing the built-in feed storage backends supported by Scrapy
			You can disable any of these backends by assigning None to their URI scheme in FEED_STORAGES
		FEED_EXPORTERS
		FEED_EXPORTERS_BASE
			{
				'json': 'scrapy.exporters.JsonItemExporter',
				'jsonlines': 'scrapy.exporters.JsonLinesItemExporter',
				'jl': 'scrapy.exporters.JsonLinesItemExporter',
				'csv': 'scrapy.exporters.CsvItemExporter',
				'xml': 'scrapy.exporters.XmlItemExporter',
				'marshal': 'scrapy.exporters.MarshalItemExporter',
				'pickle': 'scrapy.exporters.PickleItemExporter',
			}
			like FEED_STORAGES_BASE and FEED_STORAGES
