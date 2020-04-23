import socket
import re
import os
import sys
import random

BUFFER_SIZE=1024

cmds = ["USER","PASS","STOR","RETR","QUIT","SYST","TYPE","PORT",
        "PASV","LIST","REST","RNFR","RNTO","MKD","CWD","PWD","RMD",
        "login","ls","upload","download","mv","pwd"]
state = [""]

class Client:
    def __init__(self):
        self.command = ""
        self.cmd_conn = None  #命令传输链接
        self.data_conn = None  #数据传输链接
        self.port_conn = None  #port模式下监听链接
        self.trans_mode = "NONE"
        self.write_mode = "wb"

    def get_response(self):
        f = self.cmd_conn.makefile()
        line = f.readline()
        if line[3] == '-':
            code = line[:3]
            while True:
                nextline = f.readline()
                line = line +  nextline
                if nextline[:3] == code and nextline[3] != '-':
                    break
        return line

    def handle_COMMON(self):
        command = self.command.strip()+"\r\n"
        if(command.split(" ")[0] == "REST"):
            self.write_mode = "ab"
        self.cmd_conn.send(command.encode('utf8'))
        reply = self.get_response()
        sys.stdout.write(reply)
        return reply

    def handle_PASV(self):
        command = self.command.strip()+"\r\n"
        try:
            self.cmd_conn.send(command.encode('utf8'))
            reply = self.get_response()
            sys.stdout.write(reply)
            m = re.search("\((\d+),(\d+),(\d+),(\d+),(\d+),(\d+)\)", reply)
            psv_ip = "%s.%s.%s.%s"%(m.group(1),m.group(2),m.group(3),m.group(4))
            psv_port = int(m.group(5))*256 + int(m.group(6))
            self.data_conn = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            self.data_conn.connect((psv_ip, psv_port))
            self.trans_mode = "PASV"
            return reply
        except Exception as e:
            print("Error: ", e)
            return "error"

    def handle_PORT(self):
        command = self.command.strip()+"\r\n"
        try:
            m = re.search("(\d+),(\d+),(\d+),(\d+),(\d+),(\d+)", command)
            port_ip = "%s.%s.%s.%s" % (m.group(1), m.group(2), m.group(3), m.group(4))
            port_port = int(m.group(5)) * 256 + int(m.group(6))
            self.port_conn = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            self.port_conn.bind((port_ip, port_port))
            self.port_conn.listen()

            self.cmd_conn.send(command.encode('utf8'))
            reply = self.get_response()
            sys.stdout.write(reply)
            self.trans_mode = "PORT"
            return reply
        except Exception as e:
            print("Error: ",e)
            return "error"

    def handle_STOR(self):
        command = self.command.strip()+"\r\n"
        if (len(command.split(" "))==1):
            print("Error: Need file name")
            return "error"
        file_path = command.split(" ")[1][:-2]
        try:
            f = open(file_path, "rb")
            f.close()
        except Exception as e:
            print(e)
            return "error"
        if self.trans_mode == "PASV":  #PASV模式数据传输通道已经建立
            self.cmd_conn.send(command.encode('utf8'))
        elif self.trans_mode == "PORT":  #PORT模式需建立数据传输通道
            self.cmd_conn.send(command.encode('utf8'))
            self.data_conn,_ = self.port_conn.accept()
            self.port_conn.close()
            self.port_conn = None
        else:
            print("Error: PORT or PASV mode is needed")
            return "error"
        sys.stdout.write(self.get_response())
        with open(file_path,"rb") as f:
            while True:
                data = f.read(BUFFER_SIZE)
                if not data:
                    break
                self.data_conn.send(data)
        self.data_conn.close()
        self.data_conn = None
        reply = self.get_response()
        sys.stdout.write(reply)
        self.trans_mode = "NONE"
        return reply

    def handle_RETR(self, local_path =""):
        command = self.command.strip()+"\r\n"
        if (len(command.split(" "))==1):
            print("Error: Need remote file name")
            return "error"
        remote_file = command.split(" ")[1][:-2]

        if self.trans_mode == "PASV":  #PASV模式数据传输通道已经建立
            self.cmd_conn.send(command.encode('utf8'))
        elif self.trans_mode == "PORT":  #PORT模式需建立数据传输通道
            self.cmd_conn.send(command.encode('utf8'))
            self.data_conn,_ = self.port_conn.accept()
            self.port_conn.close()
            self.port_conn = None
        else:
            print("Error: PORT or PASV mode is needed")
            return "error"
        sys.stdout.write(self.get_response())
        local_file = local_path + "/" + remote_file.split('/')[-1]

        with open(local_file, self.write_mode) as f:
            while True:
                data = self.data_conn.recv(BUFFER_SIZE)
                f.write(data)
                if not data:
                    break
        self.data_conn.close()
        self.data_conn = None
        response = self.get_response()
        self.write_mode = "wb"
        if(not "226" in response):
            os.remove(local_file)
            return "error"
        sys.stdout.write(response)
        self.trans_mode = "NONE"
        return response

    def handle_LIST(self):
        command = self.command.strip()+"\r\n"
        self.list_result = ""
        if self.trans_mode == "PASV":  #PASV模式数据传输通道已经建立
            self.cmd_conn.send(command.encode('utf8'))
        elif self.trans_mode == "PORT":  #PORT模式需建立数据传输通道
            self.cmd_conn.send(command.encode('utf8'))
            self.data_conn,_ = self.port_conn.accept()
            self.port_conn.close()
            self.port_conn = None
        else:
            print("Error: PORT or PASV mode is needed")
            return "error"
        sys.stdout.write(self.get_response())
        while True:
            data = self.data_conn.recv(BUFFER_SIZE)
            sys.stdout.write(data.decode('utf8'))
            self.list_result += data.decode('utf8')
            if not data:
                break
        response = self.get_response()
        sys.stdout.write(response)
        self.trans_mode = "NONE"
        return response

    def handle_login(self):
        command = self.command.strip()+"\r\n"
        cmd_lsit = command.split(" ")
        if(len(cmd_lsit)<3):
            print("Error: Need username and password")
            return "error"
        self.command = "USER "+cmd_lsit[1]+"\r\n";
        r1 = self.handle_COMMON()
        self.command = "PASS "+cmd_lsit[2];
        r2 = self.handle_COMMON()
        return r1 + r2

    def handle_mv(self):
        command = self.command.strip()+"\r\n"
        cmd_lsit = command.split(" ")
        if(len(cmd_lsit)<3):
            print("Error: Need target file name and new name")
            return "error"
        self.command = "RNFR "+cmd_lsit[1]+"\r\n";
        r1 = self.handle_COMMON()
        self.command = "RNTO "+cmd_lsit[2];
        r2 = self.handle_COMMON()
        return r1 + r2

    def handle_ls(self, mode="PASV"):
        command = self.command.strip()+"\r\n"
        if mode=="PASV":
            self.command = "PASV\r\n"
            r1 = self.handle_PASV()
        elif mode=="PORT":
            ip = self.cmd_conn.getsockname()[0].replace('.', ',')
            #p1, p2 = produce_random_port(ip)
            p1 = "456"
            p2 = "456"
            self.command = "PORT %s,%s,%s\r\n"%(ip,p1,p2)
            r1 = self.handle_PORT()
        else:
            return "error"
        self.command = "LIST"+command[2:]
        r2 = self.handle_LIST()
        return r1 + r2

    def handle_upload(self, mode="PASV"):
        command = self.command.strip()+"\r\n"
        if mode=="PASV":
            self.command = "PASV\r\n"
            r1 = self.handle_PASV()
        elif mode=="PORT":
            ip = self.cmd_conn.getsockname()[0].replace('.', ',')
            p1, p2 = produce_random_port(ip)
            self.command = "PORT %s,%s,%s\r\n"%(ip,p1,p2)
            r1 = self.handle_PORT()
        else:
            return "error"
        self.command = "STOR"+command[6:]
        r2 = self.handle_STOR()
        return r1 + r2

    def handle_download(self, mode="PASV"):
        command = self.command.strip()
        cmds = self.command.strip().split(" ")
        if mode=="PASV":
            self.command = "PASV\r\n"
            r1 = self.handle_PASV()
        elif mode=="PORT":
            ip = self.cmd_conn.getsockname()[0].replace('.', ',')
            p1, p2 = produce_random_port(ip)
            self.command = "PORT %s,%s,%s\r\n"%(ip,p1,p2)
            r1 = self.handle_PORT()
        else:
            return "error"
        self.command = "RETR "+cmds[1] + "\r\n"
        r2 = self.handle_RETR(cmds[2])
        return r1 + r2

    def handle_pwd(self):
        self.command = "PWD\r\n"
        path = self.handle_COMMON().strip()[22:-1]
        return path

client = Client()

def handle(cmd):
    if cmd in ["USER","PASS","QUIT","SYST","TYPE","REST",
               "RNFR","RNTO","MKD","CWD","PWD","RMD"]:
        client.handle_COMMON()
    elif cmd == "PASV":
        client.handle_PASV()
    elif cmd == "PORT":
        client.handle_PORT()
    elif cmd == "STOR":
        client.handle_STOR()
    elif cmd == "RETR":
        client.handle_RETR()
    elif cmd == "LIST":
        client.handle_LIST()
    elif cmd == "ls":
        client.handle_ls()
    elif cmd == "upload":
        client.handle_upload()
    elif cmd == "download":
        client.handle_download()
    elif cmd == "login":
        client.handle_login()
    elif cmd == "mv":
        client.handle_mv()
    elif cmd == "pwd":
        client.handle_pwd()

def start(ip, port):
    print("Start connection to %s:%s" %(ip,port))
    try:
        client.cmd_conn = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        client.cmd_conn.connect((ip,int(port)))
        recv_line = client.cmd_conn.makefile().readline()
        sys.stderr.write(recv_line)
    except:
        sys.stderr.write("Error: Fail to connect %s:%s\n" %(ip,port))
        return
    while True:
        input_data = input("FTP> ")
        cmd = input_data.split(" ")[0]
        if (cmd in cmds):
            client.command = input_data+"\r\n"
            handle(cmd)
        else:
            sys.stderr.write("Error: invalid command\n")
        if cmd == "QUIT":
            break
            
def is_port_used(ip, port):
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    try:
        s.connect((ip, port))
        return True
    except OSError:
        return False
    finally:
        s.close()

def produce_random_port(ip):
    port = random.randint(20001, 65534)
    while is_port_used(ip, port):
        port = random.randint(20001, 65534)
    p1 = int(port / 256)
    p2 = port - p1 * 256
    return (p1, p2)

if __name__ == '__main__':
    argv = sys.argv
    ip = '127.0.0.1'
    port = 21
    if(len(argv) >= 3):
        port = int(argv[2])
    if(len(argv) >= 2):
        ip = argv[1]
    start(ip,port)