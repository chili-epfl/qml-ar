#!/usr/bin/env python2

# These two lines are only needed if you don't put the script directly into
# the installation directory
import sys
sys.path.append('/usr/share/inkscape/extensions')

# We will use the inkex module with the predefined Effect base class.
import inkex

# The simplestyle module provides functions for style parsing.
from simplestyle import *

# For saving json
import json

# For tag names
from lxml import etree

# For home directory
import os

# Size helpers
from qml_ar_size import *

# Get json string with markers from svg file
def export_markers(svg_file, shift_x_mm, shift_y_mm):
    # size of Uchiya marker in pixels
    UCHIYA_MARKER_SIZE = 600

    # version of the QMLAR library
    VERSION = 'QMLARMarker_v1'

    # resulting array of markers
    markers = []

    # number of found markers in svg file
    n_detected = 0

    # Get sizes in pixels and in mm
    width_px, height_px = get_w_h_from_viewbox(svg_file)
    width_mm, height_mm = get_w_h_mm(svg_file)

    # Check if sizes are correct
    if not width_mm:
        inkex.errormsg('This SVG file is not in millimeters. Please set the size in millimeters using the plugin')
        return
    assert(width_px > 0 and height_px > 0)
    assert(width_mm > 0 and height_mm > 0)

    # Convertor px -> mm
    px_to_mm = width_mm / width_px

    # loop over all tags
    for tag in svg_file.getchildren():
        # looking for groups
        if etree.QName(tag).localname == 'g':
            # array of circles in group
            circles = []

            # parameters of a marker
            params = None

            # subtags
            for stag in tag.getchildren():
                # name of subtag
                stag_name = etree.QName(stag).localname

                # if found a title, fill raw_name and params
                if stag_name == 'title':
                    name = stag.text.split(' ', 1)
                    if len(name) != 2 or name[0] != VERSION:
                        continue
                    params = json.loads(name[1])

                # if found a circle, fill x, y
                elif stag_name == 'circle':
                    circles.append(tuple([float(stag.get(t)) for t in ['cx', 'cy']]))

            # if the tag was a marker
            if len(circles) > 0 and params:
                # create a json object
                marker = {}

                # add circles coordinates in 0-600, 0-600
                marker['dots_uchiya'] = [{"x": (x - params['x_px']) / params['size_px'] * UCHIYA_MARKER_SIZE,
                                         "y": (1. - (y - params['y_px']) / params['size_px']) * UCHIYA_MARKER_SIZE}
                                         for x, y in circles]

                # consecutive ids from 0
                marker['id'] = n_detected
                n_detected += 1

                # top-left corner
                marker['x_mm'] = get_x_in_mm(svg_file, params['x_px']) + shift_x_mm
                marker['y_mm'] = get_y_in_mm(svg_file, params['y_px']) + shift_y_mm

                # size of the side in mm
                marker['size_mm'] = params['size_px'] * px_to_mm

                # adding raw values to resulting json
                for key, value in params.items():
                    assert(key not in marker.keys())
                    marker[key] = value

                # adding the marker
                markers.append(marker)
    return {'markers': markers}

class MarkerExporter(inkex.Effect):
    """
    QML-AR marker plugin
    """
    def __init__(self):
        """
        Constructor.
        """
        # Call the base class constructor.
        inkex.Effect.__init__(self)

        # Define string option "--output" with "-o" shortcut and default value markers.json
        self.OptionParser.add_option('-o', '--output', action = 'store',
          type = 'str', dest = 'o', default = os.environ['HOME'] + '/markers.json',
          help = 'Output json file')

        # Define float option "--shiftxmm" with "-x" shortcut and default value 0
        self.OptionParser.add_option('-x', '--shift_x_mm', action = 'store',
          type = 'float', dest = 'shift_x_mm', default = 0,
          help = 'Shift in x in mm')

        # Define float option "--shiftymm" with "-y" shortcut and default value 0
        self.OptionParser.add_option('-y', '--shift_y_mm', action = 'store',
          type = 'float', dest = 'shift_y_mm', default = 0,
          help = 'Shift in y in mm')

    def effect(self):
        """
        Effect behaviour.
        Overrides base class' method and saves all markers
        """

        # svg -> export_markers -> json -> file
        with open(self.options.o, 'w') as f:
            f.write(json.dumps(export_markers(self.document.getroot(), self.options.shift_x_mm, self.options.shift_y_mm), indent = 2))

# Create effect instance and apply it.
effect = MarkerExporter()
effect.affect()
