"""
1.Configuration settings
	scrapy look for scrapy.cfg:
		1./etc/scrapy.cfg or c:\scrapy\scrapy.cfg (system-wide),
		2.~/.config/scrapy.cfg ($XDG_CONFIG_HOME) and ~/.scrapy.cfg ($HOME) for global (user-wide) settings
		3.scrapy.cfg inside a scrapy project’s root (see next section).
		priority:3>2>1
	configured by environment variables
		SCRAPY_SETTINGS_MODULE
		SCRAPY_PROJECT
		SCRAPY_PYTHON_SHELL

2.Default structure of Scrapy projects
	scrapy.cfg
	myproject/
		__init__.py
		items.py
		pipelines.py
		settings.py
		spiders/
			__init__.py
			spider1.py
			spider2.py
			...
	The directory where the scrapy.cfg file resides is known as the project root directory

3.Using the scrapy tool
	1.no arguments
		Scrapy 1.0.5 - project: tutorial
		we can get the version and the current project
	2.Creating projects
		scrapy startproject myproject
	3.Controlling projects
		create a new spider
			scrapy genspider mydomain mydomain.com
	4.Available tool commands
		scrapy -h
		scrapy <command> -h
		scrapy startproject <project_name>
		scrapy genspider -l
		scrapy genspider [-t template] <name> <domain>
		scrapy crawl <spider>
		scrapy check -l
		scrapy check
		scrapy list
		scrapy edit <spider>[Edit the given spider using the editor defined in the EDITOR setting.]
		scrapy fetch <url>	
			Downloads the given URL using the Scrapy downloader and writes the contents to standard output.
			it fetches the page how the spider would download it
		scrapy view <url>
			Opens the given URL in a browser,
		scrapy shell [url]
		scrapy parse <url> [options]
			Fetches the given URL and parses it with the spider that handles it
			--spider=SPIDER: bypass spider autodetection and force use of specific spider
			--a NAME=VALUE: set spider argument (may be repeated)
			--callback or -c: spider method to use as callback for parsing the response
			--pipelines: process items through pipelines
			--rules or -r: use CrawlSpider rules to discover the callback (i.e. spider method) to use for parsing the response
			--noitems: don’t show scraped items
			--nolinks: don’t show extracted links
			--nocolour: avoid using pygments to colorize the output
			--depth or -d: depth level for which the requests should be followed recursively (default: 1)
			--verbose or -v: display information for each depth level
		scrapy settings [options]
			scrapy settings --get BOT_NAME
			scrapy settings --get DOWNLOAD_DELAY
		scrapy runspider <spider_file.py>
			Run a spider self-contained in a Python file, without having to create a project.
		crapy version [-v]
		scrapy bench
			Run a quick benchmark test

	5.Custom project commands
		COMMANDS_MODULE = 'mybot.commands'
		this is used to add custom commands for your Scrapy project.
