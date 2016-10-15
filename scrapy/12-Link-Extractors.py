1.Link Extractors
	Link extractors are objects whose only purpose is to extract links from web pages 
2.Built-in link extractors reference
	class scrapy.linkextractors.LinkExtractor
	class scrapy.linkextractors.lxmlhtml.LxmlLinkExtractor(
		allow=(), deny=(), allow_domains=(), deny_domains=(), deny_extensions=None, restrict_xpaths=(), restrict_css=(), 
		tags=('a', 'area'), attrs=('href', ), canonicalize=True, unique=True, process_value=None)

		allow (a regular expression (or list of))
		deny (a regular expression (or list of))
		allow_domains (str or list)
		deny_domains (str or list)
		deny_extensions (list)
			If not given, it will default to the IGNORED_EXTENSIONS list defined in the scrapy.linkextractors package.
		restrict_xpaths (str or list)
		restrict_css (str or list)
		tags (str or list)
			a tag or a list of tags to consider when extracting links. Defaults to ('a', 'area').
		attrs (list)
			defaults to ('href',)
		canonicalize (boolean)
			canonicalize each extracted url (using scrapy.utils.url.canonicalize_url). Defaults to True.
		unique (boolean)
			whether duplicate filtering should be applied to extracted links.
		process_value (callable) 
			defaults to lambda x: x
