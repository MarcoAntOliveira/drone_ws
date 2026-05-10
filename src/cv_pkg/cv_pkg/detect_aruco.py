#!/usr/bin/env python3
import rclpy
from rclpy.node import Node
from px4_msgs.srv import DetectAruco # Certifique-se que o .srv está compilado
import cv2
import numpy as np

class DetectArucoServerNode(Node):
    def __init__(self):
        super().__init__("detect_aruco_server")
        
        # Cria o serviço
        self.server_ = self.create_service(
            DetectAruco, "detect_aruco", self.callback_detect_aruco)
        
        # Inicializa a câmera
        self.cap = cv2.VideoCapture(0)
        self.cap.set(cv2.CAP_PROP_FRAME_WIDTH, 320)
        self.cap.set(cv2.CAP_PROP_FRAME_HEIGHT, 240)
        
        # Configuração ArUco
        self.aruco_dict = cv2.aruco.getPredefinedDictionary(cv2.aruco.DICT_4X4_50)
        self.parameters = cv2.aruco.DetectorParameters()
        
        self.get_logger().info("Serviço de Detecção ArUco iniciado.")

    def get_polygon_name(self, corners):
        """ Detecta a forma geométrica ao redor do ArUco """
        # Criamos uma máscara ou aproximamos o contorno dos cantos do ArUco
        # Para simplificar na SAE: 3=Triângulo, 4=Quadrado, 5=Pentágono, 6=Hexágono
        # Aqui você usaria cv2.approxPolyDP no contorno externo ao marcador
        return "hexagon" # Exemplo baseado na sua imagem

    def callback_detect_aruco(self, request, response):
        if not request.start:
            response.target_detected = False
            return response

        ret, frame = self.cap.read()
        if not ret:
            self.get_logger().error("Falha ao capturar frame da câmera")
            response.target_detected = False
            return response

        gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
        corners, ids, _ = cv2.aruco.detectMarkers(gray, self.aruco_dict, parameters=self.parameters)

        if ids is not None:
            # Pegamos o primeiro marcador detectado para a resposta
            marker_id = int(ids[0][0])
            
            # Cálculo do centro (pixels)
            cX = int(np.mean(corners[0][0][:, 0]))
            cY = int(np.mean(corners[0][0][:, 1]))

            # Preenche a resposta conforme seu .srv
            response.target_number = marker_id
            response.target_poligon = self.get_polygon_name(corners[0])
            
            # Normalização para o Offboard (-1.0 a 1.0)
            response.target_x = float((cX - 160) / 160.0)
            response.target_y = float((cY - 120) / 120.0)
            response.target_detected = True
            
            self.get_logger().info(f"Detectado ID: {marker_id} na posição ({cX}, {cY})")
        else:
            response.target_detected = False

        return response

def main(args=None):
    rclpy.init(args=args)
    node = DetectArucoServerNode()
    try:
        rclpy.spin(node)
    except KeyboardInterrupt:
        pass
    finally:
        node.cap.release()
        rclpy.shutdown()

if __name__ == "__main__":
    main()
