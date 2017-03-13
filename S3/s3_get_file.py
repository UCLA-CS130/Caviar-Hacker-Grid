#!/usr/bin/python

import sys
import boto3
from optparse import OptionParser
ACCESS_KEY = "AKIAI3OWYQBREX3NSAHQ"
SECRET_KEY = "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"
def get_file_from_s3(bucket_name, file_key, download_name):
	s3_client = boto3.client('s3',aws_access_key_id = ACCESS_KEY, aws_secret_access_key= SECRET_KEY)
	try:
		s3_client.download_file(bucket_name, file_key, download_name)
	except Exception:
		#cannot get file
		sys.exit(2)
	

if __name__ == '__main__':
	"""
	usage:
	./s3_get_file.py bucket_name file_key download_name

	exit status:
	0 --- ok
	1 --- more than 1 arguments
	2 --- resource error
	"""
	parser = OptionParser()
	( _, args) = parser.parse_args()
	if len(args) != 3:
		sys.exit(1)
	bucket_name = args[0]
	file_key = args[1]
	download_name = args[2]
	get_file_from_s3(bucket_name,file_key,download_name)
	sys.exit(0)
