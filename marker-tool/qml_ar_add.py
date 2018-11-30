#!/usr/bin/env python2

# For unique layer names
import uuid

# For random dots
import random

# These two lines are only needed if you don't put the script directly into
# the installation directory
import sys
sys.path.append('/usr/share/inkscape/extensions')

# We will use the inkex module with the predefined Effect base class.
import inkex

# The simplestyle module provides functions for style parsing.
from simplestyle import *

# For obtaining tag names
from lxml import etree

# Size helpers
from qml_ar_size import *

class MarkerAdder(inkex.Effect):
    """
    QML-AR marker plugin
    """
    def __init__(self):
        """
        Constructor.
        """
        # Call the base class constructor.
        inkex.Effect.__init__(self)

        # maximal number of iterations to add a dot
        self.MAXITER = 10000

        # Define int option "--ndots" with "-n" shortcut and default value 40
        self.OptionParser.add_option('-n', '--ndots', action = 'store',
          type = 'int', dest = 'n', default = 40,
          help = 'Number of dots')

        # Define float option "--radius" with "-r" shortcut and default value 5.0
        self.OptionParser.add_option('-r', '--radius', action = 'store',
          type = 'float', dest = 'r', default = 5,
          help = 'Radius of a dot')

        # Define float option "--color" with "-c" shortcut and default value ff0000ff
        self.OptionParser.add_option('-c', '--color', action = 'store',
          type = 'str', dest = 'c', default = str(int(0xff0000ff)),
          help = 'Color of a dot')

        # Define float option "--margin" with "-m" shortcut and default value 0.5
        self.OptionParser.add_option('-m', '--margin', action = 'store',
          type = 'float', dest = 'm', default = 0.5,
          help = 'Minimal margin between dots')

        # Create unique layer uuid
        self.layer_uuid = str(uuid.uuid4())[:8]

    def effect(self):
        """
        Effect behaviour.
        Overrides base class' method and inserts random dots
        """

        # need exactly one selected item in Inkscape
        if len(self.selected) != 1:
            inkex.errormsg('Please select exactly one square')
            return

        # obtain the selected item and its tag
        s_object = list(self.selected.values())[0]
        s_object_tag = etree.QName(s_object).localname

        # selected item should be a rectangle
        if s_object_tag != 'rect':
            inkex.errormsg('Please select a rectangle, selected: %s' % s_object_tag)
            return

        # selected item should be a square
        if s_object.get('height') != s_object.get('width'):
            inkex.errormsg('Please select a square, selected a rectangle %s x %s' % (s_object.get('height'), s_object.get('width')))
            return


        # Access the root tag
        svg = self.document.getroot()

        # Get sizes in pixels and in mm
        width_px, height_px = get_w_h_from_viewbox(svg)
        width_mm, height_mm = get_w_h_mm(svg)

        # Check if sizes are correct
        if not width_mm:
            inkex.errormsg('This SVG file is not in millimeters. Please set the size in millimeters using the plugin')
            return
        assert(width_px > 0 and height_px > 0)
        assert(width_mm > 0 and height_mm > 0)

        # Get mm->px multiplier
        mm_to_px = width_px / width_mm

        # obtaining selected square coordinates
        square_x, square_y = [float(s_object.get(t)) for t in ['x', 'y']]
        square_size = float(s_object.get('width'))

        # Parse color option and create a style with this color as fill color
        # Inkscape color parameter uses 32 bit integer which overflows
        color = int(self.options.c)
        if color < 0: color += 2 ** 32
        color = '#' + "{0:#0{1}x}".format(color, 10)[2:-2]
        style = {'fill': color}

        # Minimal distance between two dots
        margin = self.options.m

        # Get dot size
        r = self.options.r

        # Add converted variables (in px)
        r_px = r * mm_to_px
        margin_px = margin * mm_to_px

        # Check margin
        if 2 * margin_px >= square_size:
            inkex.errormsg('No dots possible in square with size %f px and margins %f px' % (square_size, margin_px))
            return

        # Create maker description string
        marker_name = 'QMLARMarker_v1 {"x_px": %f, "y_px": %f, "size_px": %f, "name": "%s", "ndots": %d, "r_px": %f, "margin_px": %f, "color": "%s"}' % (square_x, square_y, square_size, self.layer_uuid, self.options.n, r_px, margin_px, color)

        # Creating circle coordinates
        circle_coordinates = []
        for i in range(self.options.n):
            # generating random dots until margin constraint is satisfied
            loop_counter = 0
            while True:
                # also not adding dots in square sides
                cx = random.uniform(square_x + margin_px, square_x + square_size - margin_px)
                cy = random.uniform(square_y + margin_px, square_y + square_size - margin_px)

                # loop over all added dots
                distance_sq = [(x - cx) ** 2 + (y - cy) ** 2 for x, y in circle_coordinates]
                if not distance_sq or min(distance_sq) >= margin_px ** 2:
                    circle_coordinates.append((cx, cy))
                    break
                else:
                    loop_counter += 1
                    if loop_counter > self.MAXITER:
                        inkex.errormsg('Cannot obtain random dots in given parameters')
                        return

        # Create a new layer.
        layer = inkex.etree.SubElement(svg, 'g')
        layer.set(inkex.addNS('label', 'inkscape'), marker_name)
        layer.set(inkex.addNS('groupmode', 'inkscape'), 'layer')

        # add title tag to the layer with marker name
        title_tag = inkex.etree.Element(inkex.addNS('title', 'svg'))
        title_tag.text = marker_name
        layer.append(title_tag)

        # Adding dots to the layer
        for cx, cy in circle_coordinates:
            circle = inkex.etree.Element(inkex.addNS('circle', 'svg'))

            # setting new dot parameters
            circle.set('style', formatStyle(style))
            circle.set('cx', str(cx))
            circle.set('cy', str(cy))
            circle.set('r', str(r_px))

            # Connect elements together.
            layer.append(circle)

# Create effect instance and apply it.
effect = MarkerAdder()
effect.affect()
