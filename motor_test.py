import serial
import struct
import time

# シリアル通信の開始(UnitV2)
ser = serial.Serial('/dev/ttyS1', baudrate=115200, timeout=0.5)

# 前進コマンド(FWD)の送信
#   [SPD_MOT_0] = 24
#   [SPD_MOT_1] = 24
cmd = b'FWD' + struct.pack('<B', 24) + struct.pack('<B', 24) + b'\n'
ser.write(cmd)
time.sleep(5)

# 後退コマンド(RWD)の送信
#   [SPD_MOT_0] = 24
#   [SPD_MOT_1] = 24
cmd = b'RWD' + struct.pack('<B', 24) + struct.pack('<B', 24) + b'\n'
ser.write(cmd)
time.sleep(5)

# 停止コマンド(STP)の送信
cmd = b'STP\n'
ser.write(cmd)

# シリアル通信の終了
ser.close()
