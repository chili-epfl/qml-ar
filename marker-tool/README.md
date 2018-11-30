# Installation
Linux:

```
 $ ./install.sh
```

If the default `python` is `python3` (for example, after installing Anaconda), please force usage of `python2` by Inkscape following the [inkscape manual](http://wiki.inkscape.org/wiki/index.php/Extension_Interpreters#Selecting_a_specific_interpreter_version_.28via_preferences_file.29).

Tested in `Inkscape 0.92.1 r15371`

# Usage

Using Inkscape:
1. Open Inkscape
2. Extensions -> QML-AR -> Set size, Add, Export

The plugin allows adding markers to an existing SVG document and then exporting them to a format which is recognized by AR software.
{F5348894}

Resulting image and `markers.json`:
{F5348906} {F5348905}
{F5348915}

The `markers.json` file further can be loaded with the QMLAR application:
```
1514579081423	+1 ms	[virtual void MarkerStorage::readConfig(QJsonObject)@../src/markers/markerstorage.cpp:32]	Marker [de5ba603] #0 position (39.0214, 25.4542) mm size 84.9917
1514579081423	+0 ms	[virtual void MarkerStorage::readConfig(QJsonObject)@../src/markers/markerstorage.cpp:32]	Marker [c019f143] #1 position (28.8651, 148.933) mm size 87.1299
1514579081423	+0 ms	[virtual void MarkerStorage::readConfig(QJsonObject)@../src/markers/markerstorage.cpp:32]	Marker [e682df81] #2 position (136.842, 193.834) mm size 33.1414
```

# Known issues

Cannot work if the rectangle to be filled is inside a layer with transform attribute (the attribute is ignored, which results in misplaced marker)

# Output JSON fields

The resulting file `~/markers.json` (in the home directory) has the following structure:
1. Top-level dictionary contains `markers`, which is an array
2. Each marker has `dots_uchiya`, which is an array of dictionaries, each having `x` and `y` fields
3. Each marker has additional scalar fields:
 1. `id` -- int, a unique identifier of a marker in file (second marker with same id will be ignored by the qml-ar software!)
 2. `name` -- string, a unique identifier of a marker in file
 3. `ndots` -- int, the number of dots in the marker
 4. `color` -- string of the form '#ff0011', color of the dots in RGB format
 5. `size_mm` -- float, size of rectangle containing the marker in mm
 6. `size_px` -- float, size of rectangle containing the marker in pixels
 6. `margin_px` -- float, margin between dots and between dot and a side
 7. `x_mm` and `y_mm` -- float, position of the rectangle containing the marker in mm (same corner as for Uchiya)
 8. `x_px` and `y_px` -- float, position of the rectangle containing the marker in pixels (same corner as for Uchiya)
 9. `r_px` -- float, radius of a dot in px

## Example:

```
{
  "markers": [
    {
      "dots_uchiya": [
        {
          "y": 123.0,
          "x": 321.0
        }
      ],
      "id": 0,
      "name": "11122233",
      "ndots": 1,
      "color": "#ff0011",
      "size_mm": 129.13786,
      "size_px": 129.13786,
      "margin_px": 10.0,
      "x_mm": 21.491632,
      "y_mm": 18.615673,
      "x_px": 21.491632,
      "y_px": 18.615673,
      "r_px": 4.0
    }
  ]
}
```

# Screenshots
<img height="300px" src="https://raw.githubusercontent.com/chili-epfl/qml-ar/master/marker-tool/notebooks/inks1.png" /> <img height="300px" src="https://raw.githubusercontent.com/chili-epfl/qml-ar/master/marker-tool/notebooks/inks2.png" />
