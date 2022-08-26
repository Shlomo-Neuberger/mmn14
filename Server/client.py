import selectors
import socket
from random import random
from enum import IntEnum
from typing import Optional, Union
from pprint import pprint

VERSION = 0  # Version number of the protocol
SEND_BIG = True  # if to send int in big endian


class RequestType(IntEnum):
    GET = 200
    PUT = 100
    DELETE = 201
    LIST = 202


class ResponseType(IntEnum):
    OK = 210
    OK_LIST = 211
    ERROR_FILE_NOT_EXSISTS = 1001
    ERROR_NO_FILE_ON_SERVER = 1002
    ERROR_GENERAL = 1003


class Request:
    def __init__(self, req_type: RequestType, file_name: Optional[str] = None, uid: Optional[Union[str, int]] = None) -> None:
        self.op = req_type
        self.version = VERSION
        self.file_name = file_name
        uid = uid if uid else int(random()*9999)
        try:
            if int(uid) <= 9999:
                self.uid = int(uid)
        except Exception as e:
            self.uid = int(random() % 9999)
        if not file_name and req_type not in [RequestType.LIST]:
            raise Exception(f"For {req_type} you must pass file")

    def __read_file(self):
        mybytearray = bytearray()
        with open(self.file_name, "rb") as fp:
            b = fp.read(1)
            while b:
                mybytearray += b
                b = fp.read(1)

        return mybytearray

    def to_buffer(self):
        bytes_array = bytearray()
        bytes_array += self.uid.to_bytes(4, 'big' if SEND_BIG else 'little')
        bytes_array += self.version.to_bytes(1,
                                             'big' if SEND_BIG else 'little')
        bytes_array += self.op.to_bytes(1, 'big' if SEND_BIG else 'little')
        if self.op in [RequestType.PUT, RequestType.GET, RequestType.DELETE]:
            bytes_array += len(self.file_name).to_bytes(2,
                                                        'big' if SEND_BIG else 'little')
            bytes_array += self.file_name.encode('utf-8')
        if self.op == RequestType.PUT:
            file_bytes = self.__read_file()
            bytes_array += len(file_bytes).to_bytes(4,
                                                    'big' if SEND_BIG else 'little')
            bytes_array += file_bytes
        return bytes_array


class Response:
    # DEFINES CONSTANTS
    SIZE_OF_VERSION = 1
    SIZE_OF_STATUS = 2
    SIZE_OF_FILE_NAME = 2
    SIZE_OF_PAYLOAD_SIZE = 4

    def __init__(self,
                 status: ResponseType = None,
                 name_len: int = None,
                 name: str = None,
                 file_size: int = None,
                 payload: bytes = None,
                 ):
        self.version = VERSION
        self.status = status,
        self.name_len = name_len
        self.name = name
        self.file_size = file_size
        self.payload = payload

    def from_bytes(self, src: bytes):
        if len(src) < 1:
            raise Exception("No bytes to parse")

        start = 0
        end = start+Response.SIZE_OF_VERSION
        self.version = src[start]

        start = end
        end = start + Response.SIZE_OF_STATUS

        if len(src) < end:
            raise Exception("Malformed response")
        self.status = int.from_bytes(src[start:end], "big")

        if self.status in [ResponseType.ERROR_NO_FILE_ON_SERVER, ResponseType.ERROR_GENERAL]:
            return
        if self.status == ResponseType.ERROR_FILE_NOT_EXSISTS or self.status in [ResponseType.OK_LIST, ResponseType.OK]:
            start = end
            end = start + Response.SIZE_OF_FILE_NAME
            if len(src) < end:
                raise Exception("Malformed response")
            self.name_len = int.from_bytes(src[start:end], "big")
            start = end
            if len(src) < end:
                raise Exception("Malformed response")
            self.name = str(src[start:start+self.name_len], 'utf-8')
            start += self.name_len
        if self.status in [ResponseType.OK_LIST, ResponseType.OK]:
            start = end
            end = start + Response.SIZE_OF_PAYLOAD_SIZE
            self.file_size = int.from_bytes(src[start:end], "big")
            start = end
            end = start+self.file_size
            self.payload = src[start:end]

    def __str__(self):
        pinted_pyaload = ''
        try:
            if self.payload:
                pinted_pyaload = str(self.payload, 'utf8')
        except Exception as e:
            pinted_pyaload = self.payload
        version = f'version:\t{self.version}' if self.version is not None else None
        status = f'op:\t{self.status}' if self.status is not None else None
        name_len = f'name_len:\t{self.name_len}' if self.name_len is not None else None
        name = f'name:\t{self.name}' if self.name is not None else None
        file_size = f'file_size:\t{self.file_size}' if self.file_size is not None else None
        payload = f'payload:\t\n{f"{pinted_pyaload[:100]}..." if len(pinted_pyaload)>100 else pinted_pyaload}' if self.payload is not None else None
        res = ''
        for value in [version, status, name_len, name, file_size, payload]:
            if value:
                res += value+"\n"
        return f"Response\n{res}\n"


if __name__ == "__main__":
    uid = int(random()*9999)
    files_list = []
    serverinfo = None
    with open('backup.info', 'rt') as file_source:
        files_list.extend(file_source.read().split('\n'))
    with open('server.info', 'rt') as file_source:
        a, b = tuple(file_source.readline().split(':'))
        serverinfo = (a, int(b))
    print("Saving ", files_list[0])
    req = Request(RequestType.PUT, files_list[0], uid)
    req2 = Request(RequestType.PUT, files_list[1], uid)
    req3 = Request(RequestType.LIST, uid=uid)
    req4 = Request(RequestType.GET, files_list[0], req.uid)
    req5 = Request(RequestType.DELETE, files_list[0], req.uid)
    for _req in [req, req2, req3, req4, req5]:
        with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
            res = Response()
            s.connect(serverinfo)
            buff = _req.to_buffer()
            buff_ans = bytes()
            s.send(buff)
            buff = s.recv(1024)
            while buff:
                buff_ans += buff
                buff = s.recv(1024)
            print('raw answer:', f'{buff_ans[:100]}...' if len(
                buff_ans) > 100 else buff_ans)
            res.from_bytes(buff_ans)
            print('formaatted response', res)
            if _req == req4:
                with open('tmp', 'wb',) as out:
                    out.write(res.payload)
                print("saved", res.name, "at", 'tmp')
