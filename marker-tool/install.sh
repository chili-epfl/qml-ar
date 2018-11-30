#!/bin/bash

if [ ! -f README.md ]
then
    echo -e "Please run $0 from the qml-ar-inkscape GIT repository folder, like below:\n"
    echo "qml-ar-inkscape $ ./install.sh"
    exit 1
fi

echo "Creating extensions directory"
mkdir -p ~/.config/inkscape/extensions

echo "Removing old versions"
rm -rf ~/.config/inkscape/extensions/qml_ar*

echo "Creating symbolic links"
ln -s $(pwd)/qml_ar* ~/.config/inkscape/extensions

echo "Script done"
