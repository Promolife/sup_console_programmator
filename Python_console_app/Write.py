import datetime as dt
import time

import serial

WRITEFILE = 'write.bin'  # Filename

SendBlockLength = 2048  # The length of the data packet to write in bytes.
Com_Baudrate = 1000000  # Port speed in baud.


def crc16(data: bytes):
    """ Calculate CRC16 check summ. """
    crc = 0
    for x in data:
        crc = crc ^ x
        for i in range(8):
            if (crc & 0x0001):
                crc = (crc >> 1) ^ 0xA001
            else:
                crc = crc >> 1
            crc &= 0xFFFF
    return crc


def get_file_size(data: bytes):
    counter_data_size = 0
    for i in data:
        counter_data_size += 1
    return counter_data_size


def send_write_block(address, data):
    out_data = []
    command = bytes('$WRB', encoding='utf-8')
    counter_data_size = 0
    crc_data_summ = crc16(data)
    for i in data:
        counter_data_size += 1
    for i in command:
        out_data.append(i)
    out_data.append((counter_data_size >> 8) & 0x00FF)
    out_data.append(counter_data_size & 0x00FF)
    out_data.append((address >> 24) & 0x000000FF)
    out_data.append((address >> 16) & 0x000000FF)
    out_data.append((address >> 8) & 0x000000FF)
    out_data.append(address & 0x000000FF)
    for i in data:
        out_data.append(i)
    out_data.append((crc_data_summ >> 8) & 0x00FF)
    out_data.append(crc_data_summ & 0x00FF)
    return out_data


def build_send_pack(data: bytes):
    counterbyte = 0
    counterblock = 0
    counter_byte = 0
    addressblock = 0
    blockarray = []
    for byteappend in data:
        if (counterbyte == SendBlockLength):
            addressblock = int((SendBlockLength / 2) * counterblock)
            print(f'Write address {addressblock:012x}  Block {counterblock}')
            ser.write(send_write_block(addressblock, blockarray))
            while (1):
                answer = ser.readline()
                if (answer == bytes('$OK\r\n', 'UTF-8')):
                    break
                elif (answer == bytes('$ERRCRC\r\n', 'UTF-8')):
                    print('CRC Error. Write stopped')
                    return 0
                elif (answer == bytes('$WRTCRCERR\r\n', 'UTF-8')):
                    print('Block verification error. Write stopped')
                    return 0
            blockarray.clear()
            counterblock += 1
            counterbyte = 0
        blockarray.append(byteappend)
        counterbyte += 1
    if (counterbyte > 0):
        for i in range(counterbyte, SendBlockLength):
            blockarray.append(0xff)
            counter_byte += 1
        addressblock = int((SendBlockLength / 2) * counterblock)
        print(f'Write address {addressblock:012x}  Block {counterblock}')
        ser.write(send_write_block(addressblock, blockarray))
        while (1):
            answer = ser.readline()
            if (answer == bytes('$OK\r\n', 'UTF-8')):
                break
            elif (answer == bytes('$ERRCRC\r\n', 'UTF-8')):
                print('CRC Error. Write stopped')
                return 0
            elif (answer == bytes('$WRTCRCERR\r\n', 'UTF-8')):
                print('Block verification error. Write stopped')
                return 0
    return 1

# -------------------------Main programm-------------------------


if __name__ == "__main__":
    try:
        Flash_File = open(WRITEFILE, 'rb')
    except Exception:
        print(f'File {WRITEFILE} is not found.')
    else:
        Flash_File_buffer_array = Flash_File.read()
        Flash_File.close()
        print('File is load.')
        try:
            ser = serial.Serial('COM3', Com_Baudrate)
        except Exception:
            print('Can\'t open port.')
        else:
            print('Port is open.')
            time.sleep(2)
            ser.write(bytes('$AUT', encoding='utf-8'))
            while (ser.readline() != bytes('$OK\r\n', 'UTF-8')):
                ()
            print('Programmator is ready.')
            print('Erase Flash. Wait, it\'s take a several time.')
            now = dt.datetime.utcnow()
            start_time = now + dt.timedelta(hours=3)
            print('Erase start at', start_time)
            time.sleep(1)
            ser.write(bytes('$ERS', encoding='utf-8'))
            while (ser.readline() != bytes('$OK\r\n', 'UTF-8')):
                ()
            print('Erase complete')
            now = dt.datetime.utcnow()
            finish_time = now + dt.timedelta(hours=3)
            print('Erase end at', finish_time)
            print('Flash erase completed in', finish_time - start_time)
            time.sleep(3)
            FileSize = get_file_size(Flash_File_buffer_array)
            print('File size, byte: ', FileSize)
            print('Blocks is: ', int(FileSize/SendBlockLength))
            time.sleep(5)
            now = dt.datetime.utcnow()
            start_time = now + dt.timedelta(hours=3)
            print('Start write at', start_time)
            build_send_pack(Flash_File_buffer_array)
            now = dt.datetime.utcnow()
            finish_time = now + dt.timedelta(hours=3)
            print('Write end at', finish_time)
            print('Flash write completed in', finish_time - start_time)
            ser.close()
            print('Port close')
