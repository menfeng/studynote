# -*- coding: utf-8 -*-  
import cgi  
              
header = 'Content-Type: text/html\n\n'  
              
html = '<h3>接受处理表单数据\n</h3>'  
#打印返回的内容  
print header  
print html  
# 接受表达提交的数据  
form = cgi.FieldStorage()  
              
print '接收表达get的数据 ：',form    
              
print '<p />'  
              
# 解析处理提交的数据  
content = form['text'].value  
              
formhtml = ''''' 
<label for="">you say:</label><input type="text" value="%s"> 
'''  
              
print formhtml % (content)  
