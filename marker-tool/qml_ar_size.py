# Get width and height from viewbox (pixels)
def get_w_h_from_viewbox(svg):
    x1, y1, x2, y2 = [float(x) for x in svg.get('viewBox').split()]
    return x2 - x1, y2 - y1

# Get width and height from svg tag (must be in mm, returns (None, None) otherwise)
def get_w_h_mm(svg):
    vals = [svg.get(prop) for prop in ['width', 'height']]
    if not all([val[-2:] == 'mm' for val in vals]):
        return None, None
    w, h = [float(val[:-2]) for val in vals]
    return w, h

# Get mm coordinates from pixel coordinates (x axis)
def get_x_in_mm(svg, x_px):
    w_mm, _ = get_w_h_mm(svg)
    x1, _, x2, _ = [float(x) for x in svg.get('viewBox').split()]
    x_mm = (x_px - x1) / (x2 - x1) * w_mm
    return x_mm

# Get mm coordinates from pixel coordinates (y axis)
def get_y_in_mm(svg, y_px):
    _, h_mm = get_w_h_mm(svg)
    _, y1, _, y2 = [float(x) for x in svg.get('viewBox').split()]
    y_mm = (y_px - y1) / (y2 - y1) * h_mm
    return y_mm
