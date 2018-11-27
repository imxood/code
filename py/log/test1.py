#!/usr/bin/env python
# coding: utf-8

import sys
import logging
import datetime


class MyFormatter(logging.Formatter):

    converter = datetime.datetime.fromtimestamp

    def formatTime(self, record, datefmt=None):
        ct = self.converter(record.created)
        if datefmt:
            s = ct.strftime(datefmt)
        else:
            t = ct.strftime("%Y-%m-%d,%H:%M:%S")
            s = "%s,%03d" % (t, record.msecs)
        return s

# 创建一个日志器logger并设置其日志级别为DEBUG
logger = logging.getLogger('simple_logger')
logger.setLevel(logging.DEBUG)

# 创建一个流处理器handler并设置其日志级别为DEBUG
handler = logging.StreamHandler(sys.stdout)
handler.setLevel(logging.DEBUG)

# 创建一个格式器formatter并将其添加到处理器handler
formatter = MyFormatter(fmt='%(asctime)s [%(filename)s][line%(lineno)s][%(levelname)s] - %(message)s',datefmt='%m-%d,%H:%M:%S.%f')
handler.setFormatter(formatter)

# 为日志器logger添加上面创建的处理器handler
logger.addHandler(handler)

# 日志输出
logger.debug('debug message')
logger.info('info message')
logger.warn('warn message')
logger.error('error message')
logger.critical('critical message')
