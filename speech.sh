#! /bin/sh

if [ $# -ne 1 ]; then
    echo "Usage: speech.sh file.txt"
    exit 1
fi

open_jtalk -m /usr/local/share/hts_voice/hts_voice_nitech_jp_atr503_m001-1.05/nitech_jp_atr503_m001.htsvoice -x /usr/local/share/open_jtalk/open_jtalk_dic_utf_8-1.11 -ow `basename $1 .txt`.wav $1
