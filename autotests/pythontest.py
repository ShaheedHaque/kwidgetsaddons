#!/usr/bin/env python
#-*- coding: utf-8 -*-

from __future__ import print_function

import sys

sys.path.append(sys.argv[1])

from PyQt5 import QtCore
from PyQt5 import QtWidgets

from PyKF5 import KWidgetsAddons

def main():
    app = QtWidgets.QApplication(sys.argv)

    rw = KWidgetsAddons.KRatingWidget()
    rw.setMaxRating(10)
    rw.setRating(11)
    print (rw.rating())
    assert(rw.rating() == 11) # KRatingWidget is broken

if __name__ == '__main__':
    sys.exit(main())
