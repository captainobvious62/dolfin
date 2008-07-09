"""Run all tests, including unit tests and regression tests"""

__author__ = "Anders Logg (logg@simula.no)"
__date__ = "2007-06-09 -- 2008-04-09"
__copyright__ = "Copyright (C) 2007-2008 Anders Logg"
__license__  = "GNU LGPL version 2.1"

from os import system
from commands import getoutput
import re, sys

# Tests to run
tests = ["unit", "system"]

failed = []

# Command to run
command = "cd %s; python test.py" + " " + " ".join(sys.argv[1:])

# Run tests
for test in tests:
    print "Running tests: %s" % test
    print "----------------------------------------------------------------------"
    fail = system(command % test)
    if fail:
        failed.append(fail)
    print ""

sys.exit(len(failed))
