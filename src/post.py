#!/usr/bin/env python
# coding=utf-8

__author__  = "zhouxg@ucweb.com"
__version__ = "0.1"
__data__	= "2013-09-04"
__info__	= "post"

import os, sys
import logging
from datetime import datetime, timedelta
import httplib, urlparse
import urllib, urllib2
import argparse
import ConfigParser

logging.basicConfig(
	level = logging.DEBUG,
	format = "[%(asctime)s] %(levelname)-8s %(message)s",
	stream = sys.stderr
	)
log = logging

from socket import socket
def post(host, port, path, data, type='text/plain'):
	s = socket()
	s.connect((host, port))
	s.send("POST %s HTTP/1.1\r\n" % path)
	s.send("Host:%s\r\n" % host)
	s.send("Content-Type:%s\r\n" % type)
	s.send("Content-Length:%d\r\n\r\n" % len(data))
	s.send(data)
	rsp = ''.join([ln for ln in s.makefile()])
	s.close()
	return rsp

def main():
	log.info('start')
	#txt = urllib2.urlopen(sys.argv[1]).read()
	txt = open('txt', 'rb').read()
	for i in range(10000):
		rsp = post('10.1.74.51', 8081, '/ucltp?', txt)
		#print 'post %8d: rsp = %s' % (i, rsp)
		print 'post %8d' % (i)
	log.info('finished')
	
if __name__ == '__main__':
	main()
