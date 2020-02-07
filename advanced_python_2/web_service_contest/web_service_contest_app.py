from flask import Flask
from string import Template
import json

app = Flask(__name__)


DATABASE = 'services.json'


def get_services(filename):
    services = []
    with open(filename) as json_file:
        data = json.load(json_file)
        for service in data:
            services.append((service['name'], service['host'], service['port']))

    return services


@app.route('/')
def hello_world():
    table_head  = '''
    
<head>
  <meta charset="utf-8">
  <meta http-equiv="X-UA-Compatible" content="IE=edge">
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <!-- The above 3 meta tags *must* come first in the head; any other head content must come *after* these tags -->
 
        <title>{% block title %}BigData Team | Super Pupper MADE Wikipedia Web Search Engine {% endblock %}</title>
 
  <!-- Google font -->
  <link href="https://fonts.googleapis.com/css?family=Montserrat:400,700,900" rel="stylesheet">
 
  <!-- Custom stlylesheet -->
        {# <link type="text/css" rel="stylesheet" href="{{ url_for('static', filename='css/style.css') }}" /> #}
 
        <style type="text/css">
            table {
                border-collapse: collapse;
                border-spacing: 0;
            }
            table {
                border: 2px solid;
            }
            table td, table th {
                border: 1px solid;
            }
        </style>
 
  <!-- HTML5 shim and Respond.js for IE8 support of HTML5 elements and media queries -->
  <!-- WARNING: Respond.js doesn't work if you view the page via file:// -->
  <!--[if lt IE 9]>
      <script src="https://oss.maxcdn.com/html5shiv/3.7.3/html5shiv.min.js"></script>
      <script src="https://oss.maxcdn.com/respond/1.4.2/respond.min.js"></script>
    <![endif]-->
 
</head>
    
    <table style="width:100%">
  <tr>
    <th>Service</th>
    <th>Host</th> 
    <th>Port</th>
    <th>Status</th>
    <th>request nums</th>
  </tr>'''

    table_end = '''</table>'''

    table_line = Template('''
    <tr>
    <td><center>$name</center></td>
    <td><center>$host</center></td>
    <td><center>$port</center></td>
    <td>$status</td>
    <td>$requests</td>
  </tr>
  ''')

    #services = [('wiki','127.0.0.0.1','80',10)]
    services = get_services(DATABASE)
    table_lines = ''
    for service in services:
        table_lines += table_line.substitute(name=service[0], host=service[1], port=service[2], status="GOOD", requests=10)
    return table_head + table_lines + table_end
