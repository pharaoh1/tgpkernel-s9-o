# ------------------------------
# TGPKERNEL INSTALLER 6.0.1
#
# Anykernel2 created by @osm0sis
# Everything else done by @djb77
# ------------------------------

## AnyKernel setup
properties() {
kernel.string=
do.devicecheck=1
do.modules=0
do.cleanup=1
do.cleanuponabort=1
device.name1=starlte
device.name2=star2lte
device.name3=
device.name4=
device.name5=
}

# Shell Variables
block=/dev/block/platform/11120000.ufs/by-name/BOOT
ramdisk=/tmp/anykernel/ramdisk
split_img=/tmp/anykernel/split_img
patch=/tmp/anykernel/patch
is_slot_device=0
ramdisk_compression=auto

## AnyKernel methods (DO NOT CHANGE)
# import patching functions/variables - see for reference
. /tmp/anykernel/tools/ak2-core.sh

## AnyKernel install
ui_print "- Extracing Boot Image"
dump_boot

# Ramdisk changes - SELinux Enforcing Mode
if egrep -q "install=1" "/tmp/aroma/selinux.prop"; then
	ui_print "- Enabling SELinux Enforcing Mode"
	replace_string $ramdisk/sbin/tgpkernel.sh "echo \"1\" > /sys/fs/selinux/enforce" "echo \"0\" > /sys/fs/selinux/enforce" "echo \"1\" > /sys/fs/selinux/enforce"
	replace_string $ramdisk/sbin/tgpkernel.sh "chmod 644 /sys/fs/selinux/enforce" "chmod 640 /sys/fs/selinux/enforce" "chmod 644 /sys/fs/selinux/enforce"
fi

# Ramdisk changes - Deodexed ROM
if egrep -q "install=1" "/tmp/aroma/deodexed.prop"; then
	ui_print "- Patching for Deodexed ROM"
	insert_line $ramdisk/init.services.rc "	exec u:r:su:s0 root root -- /sbin/init.dalvik.sh" after "exec u:r:supersu:s0 root root -- /sbin/tgpkernel.sh" "	exec u:r:su:s0 root root -- /sbin/init.dalvik.sh"
	insert_line $ramdisk/init.services.rc "	exec u:r:supersu:s0 root root -- /sbin/init.dalvik.sh" after "exec u:r:su:s0 root root -- /sbin/init.dalvik.sh" "	exec u:r:supersu:s0 root root -- /sbin/init.dalvik.sh"
	cp -rf $patch/dalvik/* $ramdisk
	chmod 644 $ramdisk/sbin/init.dalvik.sh
fi

## AnyKernel file attributes
# set permissions/ownership for included ramdisk files
chmod 644 $ramdisk/default.prop
chmod 755 $ramdisk/init.rc
chmod 755 $ramdisk/sbin/tgpkernel.sh
chown -R root:root $ramdisk/*

# End ramdisk changes
ui_print "- Writing Boot Image"
write_boot

## End install
ui_print "- Done"

