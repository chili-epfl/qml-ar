== Uninstall (before Qt install)
su -c bash
mount -o rw,remount /system
rm -rf /system/app/base.apk /data/app/org.qtproject.example.ar_chest* /data/app-lib/org.qtproject.example.ar_chest* /data/data/org.qtproject.example.ar_chest*
reboot

== INSTALL (after Qt install)
su -c "bash /storage/emulated/0/install.sh"

== INSTALL 2 (after reboot)
su -c bash
mount -o rw,remount /system
ln -s /system/lib /data/app-lib/org.qtproject.example.ar_chest-1
rm /data/data/org.qtproject.example.ar_chest/lib
ln -s /system/lib /data/data/org.qtproject.example.ar_chest/lib

== launch app
am start -n org.qtproject.example.ar_chest/org.qtproject.qt5.android.bindings.QtActivity

# look for logs
rm /storage/emulated/0/Pictures/*.png; logcat *:V|grep libar-chest

== /storage/emulated/0/install.sh

function run_print() {
    echo "$@"
    $@
}

run_print mount -o rw,remount /system
run_print pm install /data/app/org.qtproject.example.ar_chest-1/base.apk
run_print cp -p /data/app/org.qtproject.example.ar_chest-1/base.apk /system/app
run_print rm /data/app/org.qtproject.example.ar_chest-1/base.apk
run_print cp -p /data/app/org.qtproject.example.ar_chest-1/lib/arm/lib* /system/lib
run_print rm -rf /data/data/org.qtproject.example.ar_chest/lib
run_print ln -s /system/lib /data/data/org.qtproject.example.ar_chest/lib
run_print reboot

# update libqmlar
cp /storage/emulated/0/libqmlarplugin.so /system/lib/libqml_ch_epfl_chili_qmlar_libqmlarplugin.so

# system app
https://stackoverflow.com/questions/24641604/qt-application-as-system-app-on-android
https://stackoverflow.com/questions/5494764/how-to-run-a-specific-android-app-using-terminal
https://stackoverflow.com/questions/6854127/filter-logcat-to-get-only-the-messages-from-my-application-in-android/6854182#6854182
https://stackoverflow.com/questions/6066030/read-only-file-system-on-android

su
mount -o rw,remount /system
vim /system/core/rootdir/etc/public.libraries.android.txt
vim /vendor/etc/public.libraries.android.txt # doesn't boot with just ""

libbacktrace.so
libbase.so
libbinder.so
libc.so
libc++.so
libcutils.so
libdl.so
gralloc.exynos5.so
libgralloc.exynos5.so
libhardware.so
libion.so
liblog.so
liblzma.so
libm.so
libsync.so
libui.so
libunwind.so
libutils.so
