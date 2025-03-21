import cv2
import serial
from cvzone.HandTrackingModule import HandDetector

def configurar_camera(larg=1280, alt=720, id_cam=0):
    cam = cv2.VideoCapture(id_cam)
    cam.set(3, larg)
    cam.set(4, alt)
    return cam

def validar_estados(est):
    return all(e in {0, 1} for e in est)

def setup_serial(serial_port='COM3', baudrate=9600):
    try:
        arduino = serial.Serial(serial_port, baudrate, timeout=1)
        print(f"Conexão estabelecida com o Arduino em {serial_port} a {baudrate} bps.")
        return arduino
    except serial.SerialException as e:
        print(f"Erro ao conectar com o Arduino: {e}")
        return None

def process_message(estados, arduino):
    valid_messages = {"d0", "d1", "i0", "i1", "m0", "m1", "a0", "a1", "n0", "n1"}
    dedos = ['d', 'i', 'm', 'a', 'n']

    for dedo, estado in zip(dedos, estados):
        mensagem = f"{dedo}{estado}"
        if mensagem in valid_messages:
            arduino.write(mensagem.encode("utf-8"))
            print(f"Mensagem '{mensagem}' enviada ao Arduino.")
        else:
            print(f"Mensagem inválida: {mensagem}")

def lidar_cliente(cam, det, arduino):
    est_ant = None
    try:
        while True:
            ret, img = cam.read()
            if not ret:
                break

            maos, img = det.findHands(img)

            if maos:
                est = det.fingersUp(maos[0])
                if validar_estados(est) and est != est_ant:
                    process_message(est, arduino)
                    est_ant = est

            cv2.imshow('Identificação', cv2.resize(img, (640, 420)))

            if cv2.waitKey(1) & 0xFF == ord('q'):
                break
    finally:
        cam.release()
        cv2.destroyAllWindows()

if __name__ == "__main__":
    cam = configurar_camera()
    det = HandDetector(detectionCon=0.8)
    arduino = setup_serial()
    if arduino:
        lidar_cliente(cam, det, arduino)
