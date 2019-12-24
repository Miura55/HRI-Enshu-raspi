#!/bin/sh
TMP=/home/pi/key_ws/circit_test/voice.wav

echo "$1" | open_jtalk -x /var/lib/mecab/dic/open-jtalk/naist-jdic \
-m /home/pi/MMDAgent_Example-1.7/Voice/mei/mei_normal.htsvoice \
-ow $TMP -r 1.0

aplay --quiet $TMP
#rm -f $TMP
