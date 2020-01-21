lw
ls
logout
ls
cd 
cd /home/
ls
cp pi/ koki/
cd pi
ls
cd raspidemo/
ls
sudo ./voicetester 
cd ..
ls
sudo ./voicetester 
cd raspidemo/
sudo ./voicetester 
vim voice
vim voicetester.cpp 
./testall 
sudo ./testall 
clear
ls
cd ..
amixer -c1 sset Headphone 100%,50% unmute
amixer 
ls
cd raspidemo/
vim voicetester.cpp 
logout
clear
ls
vim voicetester.cpp 
logout
ls
clear
ls
amixer -c0 sset PCM 60
cd /home/pi/raspidemo/
sudo ./voicetester 
amixer -c0 sset PCM 40
sudo ./voicetester 
history 
clear
amixer -c1 sset Headphone 50%,50% unmute
sudo ./voicetester 
roscore
clear
ls
aplay How2compileREADME.
aplay How2compileREADME.wav 
history 
amixer -c0 sset PCM 20
amixer -c0 sset PCM 20%
aplay How2compileREADME.wav 
amixer -c0 sset PCM 50%
aplay How2compileREADME.wav 
amixer -c0 sset PCM 80%
aplay How2compileREADME.wav 
sudo ./voicetester 
sudo apt-get update 
clear
history 
clear
sudo ./voicetester 
vim voicetester.cpp 
cd /tmp/
ls
aplay buf.wav 
cd ..
cd /home/pi/
ls
cd raspidemo/
ls /tmp/systemd-private-517e92304edb4ffb9ba149f32507d5c6-systemd-resolved.service-KIZLWu
ls
sudo vim voicetester.cpp 
cp -r ./ /home/koki/
cd /home/koki/
ls
vim voice.cpp 
g++ -o voicetester voicetester.cpp
vim voice.cpp 
g++ -o voicetester voicetester.cpp voice.cpp -lwiringPi -lpthread -lm 
g++ -o voicetester voicetester.cpp
sudo vim voicetester.cpp 
ls
vim speech.sh 
cd /home/pi/
ls
cd raspidemo/
ls
cd ..
cd speech/
ls
bash speech.sh 
bash speech.sh zzz.
bash speech.sh zzz.txt 
vim zzz.
vim zzz.txt 
sudo shutdown -h now
ls
./speech.sh 
sudo ./speech.sh 
pwd
cd misc/
ls
cd sample/
ls
cd ..
ls
cd ..
ls
sudo ./testall 
sudo shutdown now
clear
https://github.com/Miura55/webcamapp-hri.git
git clone https://github.com/Miura55/webcamapp-hri.git
rm -r webcamapp-hri/
rm -rf webcamapp-hri/
clear
ls
git clone https://github.com/Miura55/webcamapp-hri.git
python
python3
clear
ls
rm -rf webcamapp-hri/
git clone https://github.com/Miura55/webcamapp-hri.git Webapp
ls
cd Webapp/
python3 app.py 
ls
pip3 -v
cle
clear
pytho
pip3 install -r requirements.txt 
python3 app.py 
ifconfig 
python3 app.py 
python3 get_volume.py 
sudo apt-get install python3-pyaudio
sudo apt-get install python3-numpy
python3 get_volume.py 
投稿
見出し画像
リアルタイムで音声波形の取得【PyAudio】
4
もくいち
2018/12/29 23:03 フォローする
目次
環境
使用したライブラリ
USBマイクの接続
リアルタイムで波形取得
環境
・RaspberryPi Zero W (OS: Raspbian)
・USBマイク
・Python 3.5
使用したライブラリ
・numpy
・PyAudio
USBマイクの接続
ラズパイのUSBにUSBマイクを接続します。
LXTarminalで以下のコマンドを入力してちゃんと接続できているかを確認します。
$ arecord -l
下の画像のように表示されていればOKです。
リアルタイムで波形取得
まず、PyAudioをインストールします。
$ sudo apt install python3-pyaudio
numpyをインストールしていない人は以下のコマンドでいれてください。
$ sudo apt install python3-numpy
次に以下のようなコードを書き、実行してみましょう。
# coding: utf-8
import pyaudio
import numpy as np
CHUNK = 1024
RATE = 44100　# サンプリング周波数
P = pyaudio.PyAudio()
stream = P.open(format=pyaudio.paInt16, channels=1, rate=RATE, frames_per_buffer=CHUNK, input=True, output=False)
while stream.is_active():
#output = np.array(a, dtype='int16').tobytes()
stream.stop_stream() stream.close()
P.terminate()
clear
ls
cd /home/yasu/
ls
cd flask-video-streaming/
ls
sudo python app.py 
sudo python3 app.py 
pip3 install python-opencv
pip3 install opencv-python
clear
pip flask
pip install flask
python app.py 
python
clear
vim app.py 
cd
cd /home/yasu/flask-video-streaming/
la
ls
python app.py 
sudo python app.py 
clear
cd
ls
rm -rf Webapp/
git init 
git add .
git remote add origin https://github.com/Miura55/HRI-Enshu-raspi.git
git push -u origin master
git commit -m "First commit"
git config --local "acordion.piano@gmail.com"
git config --local "Miura55"
git config --local user.email "acordion.piano@gmail.com"
git config --local user.name "Miura55"
git commit -m "First commit"
git remote add origin src refspec master does not match any
git remote add origin https://github.com/Miura55/HRI-Enshu-raspi.git
git push 
git push -u origin master
clear
ls
cd misc/
ls
cd sample/
ls
cd ..
cd
./speech.sh 
upower
sudo apt-get install upower
sudo apt-get install python3-numpy
sudo shutdown  now
clear
ls
cd misc/
ls
cd sample/
cd ../..
ls
./testall 
sudo ./testall 
ls
./speech.sh 
vim README.txt 
vim speak.txt
./speech.sh speak.txt 
vim speech.sh 
speaker-test 
vim show_wave.py 
git fetch
git pull 
ks
ls
rm speak.txt 
aplay speak.wav 
amixer -c0 sset PCM 70%
clear
cd /home/pi/
ls
vim komori.py 
clear
ls
git pull
git checkout --theirs get_volume.py
git pull
git fetch
git pull origin master
git pull
cp /home/pi/raspidemo/servotester.cpp ./
ls
vim servotester.cpp 
g++ -o servotester servotester.cpp io_input.cpp rs405cb.cpp -lwiringPi -lpthread -lm
sudo ./servotester 
git add .
git rm --cached Webapp/
git add .
git commit -am "Added servo test"
git push origin
clear
