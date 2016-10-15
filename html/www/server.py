import BaseHTTPServer
import CGIHTTPServer
import cgitb;

class CServer(object):
	def __init__(self):
		pass

	def Start(self):
		cgitb.enable()#Error reporting
		server=BaseHTTPServer.HTTPServer
		handler=CGIHTTPServer.CGIHTTPRequestHandler
		address=("",8888)
		BaseHTTPServer.HTTPServer(address,handler).serve_forever()

CServer().Start()
