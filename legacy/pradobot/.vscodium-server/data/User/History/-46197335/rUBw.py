import serial
import time



class LidarManager:
    def __init__(self, port, baudrate):
        self.ser = serial.Serial('/dev/ttyACM0', 115200, timeout=1)
        print("Serial connection opened.")
        start_lidar()

    def __del__(self):
        close_connection()

    def start_lidar(self):
        # Comienza la comunicación
        self.ser.write(b'ST\n')
        time.sleep(0.1)
        # Cambia al modo SCIP 2.0
        self.ser.write(b'SCIP2.0\n')
        time.sleep(0.1)
        # Establece el rango de medición
        self.ser.write(b'MD0044072500001\n')

    def get_data(self):
        # Obtiene datos de escaneo
        ser.write(b'GD\n')
        data = ser.readline().decode().strip()
        return data

    def process_data(self, data):
        distances = []
        angles = []
        for i in range(0, len(data), 3):
            distance = ord(data[i]) + (ord(data[i+1]) << 8)
            angle = (i // 3 - 384) * 0.352
            distances.append(distance)
            angles.append(angle)
        return (angles,distances)

    def close_connection(self):
        self.ser.write(b'QT\n')
        self.ser.close()
        print("Serial connection closed.")