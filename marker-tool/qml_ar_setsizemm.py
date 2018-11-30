#!/usr/bin/env python2

# These two lines are only needed if you don't put the script directly into
# the installation directory
import sys
sys.path.append('/usr/share/inkscape/extensions')

# We will use the inkex module with the predefined Effect base class.
import inkex

# The simplestyle module provides functions for style parsing.
from simplestyle import *

# import etree to get tag names
from lxml import etree

# import size helper
from qml_ar_size import get_w_h_from_viewbox

class Resizer(inkex.Effect):
    """
    QML-AR marker plugin
    """
    def __init__(self):
        """
        Constructor.
        """
        # Call the base class constructor.
        inkex.Effect.__init__(self)

        # Define float option "--width" with "-w" shortcut and default value 0
        self.OptionParser.add_option('-x', '--width', action = 'store',
          type = 'float', dest = 'w', default = 0,
          help = 'Width in mm')

        # Define float option "--height" with "-h" shortcut and default value 0
        self.OptionParser.add_option('-y', '--height', action = 'store',
          type = 'float', dest = 'h', default = 0,
          help = 'Height in mm')

    def effect(self):
        """
        Effect behaviour.
        Overrides base class' method and inserts random dots
        """

        # Check if width and height are correct
        w = self.options.w
        h = self.options.h

        # checking parameters
        if w == 0 and h == 0:
            inkex.errormsg('Please set one of (width, height) to a non-zero value')
            return
        elif w * h != 0:
           inkex.errormsg('Please set exactly one of (width, height) to a non-zero value, now: (%.2f, %.2f)' % (w, h))
           return
        elif w < 0:
           inkex.errormsg('Please set width to a positive value, now: %.2f' % w)
           return
        elif h < 0:
           inkex.errormsg('Please set height to a positive value, now: %.2f' % h)
           return

        # Accessing the root tag
        svg = self.document.getroot()

        # obtaining width and height in pixels
        w_px, h_px = get_w_h_from_viewbox(svg)

        # obtaining h from w or w from h
        if w > 0:
            h = w * h_px / w_px
        elif h > 0:
            w = h * w_px / h_px

        # checking if the values are correct
        assert(w > 0 and h > 0)

        # setting width and height in mm
        svg.set('width',  str(w) + 'mm')
        svg.set('height', str(h) + 'mm')


# Create effect instance and apply it.
effect = Resizer()
effect.affect()
