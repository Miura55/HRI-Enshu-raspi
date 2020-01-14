# 参考：https://note.com/mokuichi/n/n70d61237e6c7
# coding: utf-8

import pyaudio
import numpy as np

CHUNK = 512
RATE = 48000

P = pyaudio.PyAudio()

stream = P.open(format=pyaudio.paInt16, channels=1, rate=RATE, frames_per_buffer=CHUNK, input=True, output=False)

while stream.is_active():
    try:
        input = stream.read(CHUNK, exception_on_overflow=False)
        # bufferからndarrayに変換
        x = np.frombuffer(input, dtype='int16') / 32768.0

        # 配列の最大値を出力
        print(x.max())

    except KeyboardInterrupt:
        break

stream.stop_stream()
stream.close()
P.terminate()

print('Stop Streaming')
