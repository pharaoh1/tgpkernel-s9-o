#!/system/bin/sh
# mwilky

LOGFILE=/data/tgpkernel/dalvik.log
REBOOTLOGFILE=/data/tgpkernel/reboot.log

rm $LOGFILE

log_print() {
  echo "$1"
  echo "$1" >> $LOGFILE
}

rebootlog_print() {
  echo "$1"
  echo "$1" >> $REBOOTLOGFILE
}

log_print "**TGPKernel dalvik script $( date +"%m-%d-%Y %H:%M:%S" )**"

if [ -f /data/magisk.apk ]; then
	pm install /data/magisk.apk
	rm /data/magisk.apk
else 
if [ -f /data/adb/magisk/magisk.apk ]; then
	mv /data/adb/magisk/magisk.apk /data/magisk.apk
	pm install -r /data/magisk.apk
	rm /data/magisk.apk
fi
fi

if [ -z "$(ls -A /data/dalvik-cache/arm64)" ]; then
   rebootlog_print "dalvik cache not built, rebooted at $( date +"%m-%d-%Y %H:%M:%S" )"
   reboot
else
   log_print "dalvik cache already built, nothing to do"
   exit 0
fi

