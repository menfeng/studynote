Scrapy uses Python¡¯s builtin logging system for event logging.
1.Log levels
	logging.CRITICAL - for critical errors (highest severity)
	logging.ERROR - for regular errors
	logging.WARNING - for warning messages
	logging.INFO - for informational messages
	logging.DEBUG - for debugging messages (lowest severity)
2.how
	import logging
	#use the root logger behind the scenes
	logging.warning("This is a warning")
	logging.log(logging.WARNING, "This is a warning")
	#also use the root logger behind the scenes
	logger = logging.getLogger()
	logger.warning("This is a warning")
	#You can use a different logger just by getting its name with the logging.getLogger function:
	logger = logging.getLogger('mycustomlogger')
	logger.warning("This is a warning")
	#so, you can ensure having a custom logger for any module you¡¯re working on by using the __name__ variable
	logger = logging.getLogger(__name__)
	logger.warning("This is a warning")
3.Logging from Spiders
    def parse(self, response):
        self.logger.info('Parse function called on %s', response.url)
4.Logging configuration
	LOG_FILE
	LOG_ENABLED
	LOG_ENCODING
	LOG_LEVEL
	LOG_FORMAT
	LOG_DATEFORMAT
	LOG_STDOUT
5.Command-line options
	--logfile FILE
	Overrides LOG_FILE
	--loglevel/-L LEVEL
	Overrides LOG_LEVEL
	--nolog
	Sets LOG_ENABLED to False
6.scrapy.utils.log.configure_logging(settings=None, install_root_handler=True)
	Initialize logging defaults for Scrapy.
	settings (dict, Settings object or None) ¨C settings used to create and configure a handler for the root logger (default: None).
	install_root_handler (bool) ¨C whether to install root logging handler (default: True)
	if you plan on configuring the handlers yourself is still recommended you call this function, passing install_root_handler=False
	This is an example on how to redirect INFO or higher messages to a file:
		import logging
		from scrapy.utils.log import configure_logging

		configure_logging(install_root_handler=False)
		logging.basicConfig(
			filename='log.txt',
			format='%(levelname)s: %(message)s',
			level=logging.INFO
		)
