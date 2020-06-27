class Constants:
    #数值定义
    UNDEFINED_NUMBER = -1
    FILE_NUMBER = 20
    MAX_NUMBER = 1145141919810

    #显示相关
    PLAYER_NAME = "Video Player"

    #网络相关
    SERVER_ADDR = "127.0.0.1"
    SERVER_CONTROL_PORT = 10000
    SERVER_DATA_PORT = 20000
    CLIENT_CONTROL_PORT = 10001

    #数据相关
    DATA_PACKET_SIZE = 2560
    DATA_HEADER_SIZE = 12
    CONTROL_SIZE = 1024

    #RTSP 回复
    STATUS_CODE_SUCCESS = 200
    STATUS_CODE_FAIL = 400
    MESSAGE_SUCCESS = "Success"
    MESSAGE_FAIL = "Fail"

    #RTP传输状态
    #init：刚开始，8能传输
    #ready：初始化完毕，但是没开始或者暂停了
    #playing：正在传输
    RTP_TRANSPORT_INIT = 0
    RTP_TRANSPORT_READY = 1
    RTP_TRANSPORT_PLAYING = 2

    #传输状态对应错误码
    RTP_ERROR_INIT = "Wrong Status, The server is not yet set up."
    RTP_ERROR_READY = "Wrong Status, The server is not sending data."
    RTP_ERROR_PLAYING = "Wrong Status, The server is already sending data."


    #RTP数据包宏
    RTP_CURRENT_VERSION = 1
    RTP_PADDLING_TRUE = 1
    RTP_PADDLING_FALSE = 0
    RTP_EXTENSION_TRUE = 1
    RTP_EXTENSION_FALSE = 0
    RTP_CC = 0
    RTP_MARKER_TRUE = 1
    RTP_MARKER_FALSE = 0
    RTP_TYPE_JPEG = 26
    RTP_SSRC = 0
