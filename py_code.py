import time
import pygame
import serial
import serial.tools.list_ports
from time import sleep
from pathlib import Path
from cryptography.fernet import Fernet
from base64 import urlsafe_b64encode
def find_arduino():
    while True:
        for port in list(serial.tools.list_ports.comports()):
            if "Arduino" in port.description or "CH340" in port.description:
                return port.device
        sleep(0.5)
def generate_key_from_string(string_key:str):
    return urlsafe_b64encode(string_key.encode("utf-8"))

def encrypt_file(input_bytes:bytes, custom_string:str):
    return Fernet(generate_key_from_string(custom_string)).encrypt(input_bytes)

def decrypt_file(input_bytes:bytes, custom_string:str):
    return Fernet(generate_key_from_string(custom_string)).decrypt(input_bytes)


pygame.mixer.init()
sounds = {i.name.removesuffix(".wav"):pygame.mixer.Sound(i) for i in Path("music").rglob("*.wav")}
ser = serial.Serial(find_arduino(), 9600)
time.sleep(2)

password = []
while True:
    try:
        if ser.in_waiting > 0:
            line = ser.readline().decode('utf-8').strip()
            if "stopped" in line:
                password_str = " ".join(password)
                password = []
                print("password: ",password_str)
                option = input("Encrypt or Decrypt")
                filepath = input("Enter file path: ")
                if option == "Encrypt":
                    with open(filepath,"rb+") as f:
                        data = encrypt_file(f.read(),password_str)
                        f.seek(0)
                        f.write(data)
                elif option == "Decrypt":
                    with open(filepath,"rb") as f:
                        data = decrypt_file(f.read(),password_str)
                        f.seek(0)
                        f.write(data)

            elif line:
                s,d = line.split(":")
                sounds[s].play()
                password.append(s)

    except KeyboardInterrupt:
        print("\nStopping script.")
        break
    except Exception as e:
        print(f"Error: {e}")


