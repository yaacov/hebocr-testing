# -*- coding: utf-8 -*-
from setuptools import setup, find_packages

try:
    long_description = open("README.rst").read()
except IOError:
    long_description = ""

setup(
    name="hebocr",
    version="0.14.0",
    description="hebOCR is a Hebrew character recognition library",
    license="GPL",
    author="yaacov",
    packages=find_packages(),
    install_requires=[
        "opencv-python",
        "keras",
        'pylint',
    ],
    long_description=long_description,
    entry_points = {
        'console_scripts': ['hebocr=hebocr.hebocr:main'],
    },
    classifiers=[
        "Programming Language :: Python",
        "Programming Language :: Python :: 2.7",
    ]
)
