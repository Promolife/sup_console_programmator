import datetime as dt
import time

import serial

READFILE = 'dump.bin'  # Filename.
COMPORT = 'COM3'  # Number of comport.

COM_BAUDRATE = 1000000  # Port speed in baud.
FLASH_ADDRESS_BEGIN = 0x00000000
FLASH_ADDRESS_END = 0x0000FF00


def send_read_command(address_begin, address_end, port):
    out_data = []
    read_data_len = (FLASH_ADDRESS_END - FLASH_ADDRESS_BEGIN) * 2
    command = bytes("$RDR", encoding='utf-8')
    for i in command:
        out_data.append(i)
    out_data.append((address_begin >> 24) & 0x000000FF)
    out_data.append((address_begin >> 16) & 0x000000FF)
    out_data.append((address_begin >> 8) & 0x000000FF)
    out_data.append(address_begin & 0x000000FF)
    out_data.append((address_end >> 24) & 0x000000FF)
    out_data.append((address_end >> 16) & 0x000000FF)
    out_data.append((address_end >> 8) & 0x000000FF)
    out_data.append(address_end & 0x000000FF)
    port.write(out_data)
    while (1):
        answer = ser.readline()
        if (answer == bytes('$OK\r\n', 'UTF-8')):
            print('Dump in progress. Wait...')
            break
    return port.read(read_data_len)

# -------------------------Main logic-------------------------


if __name__ == "__main__":
    try:
        flash_file = open(READFILE, "wb+")
    except Exception:
        print('File is not create.')
    else:
        print('File is create')
        try:
            ser = serial.Serial(COMPORT, COM_BAUDRATE)
        except Exception:
            print("Can't open port")
        else:
            print('Port is open')
            time.sleep(2)
            ser.write(bytes("$AUT", encoding='utf-8'))
            while (ser.readline() != bytes('$OK\r\n', 'UTF-8')):
                ()
            print("Programmator is ready")
            now = dt.datetime.utcnow()
            start_time = now + dt.timedelta(hours=3)
            print('Read start at', start_time)
            write_data = send_read_command(
                    FLASH_ADDRESS_BEGIN,
                    FLASH_ADDRESS_END,
                    ser
            )
            flash_file.write(write_data)
            now = dt.datetime.utcnow()
            finish_time = now + dt.timedelta(hours=3)
            print('Read end at', finish_time)
            print('Read completed in', finish_time - start_time)
            ser.close()
            print('Port is close')
            flash_file.close()
            print('Dump comlete')
