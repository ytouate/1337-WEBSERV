#!/usr/bin/env python
import cgi

# Create instance of FieldStorage
form = cgi.FieldStorage()

# Get data from form fields
name = form.getvalue('name')
if not name:
    name = "Youssef"
# Print the HTTP header
print("Content-type: text/html\n\n")

# Generate HTML page
print("<html><body>")
print("<h1>Hello, {}!</h1>".format(name))
print("</body></html>")
