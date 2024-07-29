import sensor, image, time, pyb, random
from machine import UART

# 初始化摄像头
sensor.reset()
sensor.set_pixformat(sensor.RGB565)  # 使用彩色图像
sensor.set_framesize(sensor.QVGA)  # 320x240
sensor.skip_frames(time=2000)

# 定义图像窗口的尺寸
frame_width = 320
frame_height = 240

# 定义每个小方框的尺寸
box_size = 60  # 180像素的ROI区域，分成3x3的方框，每个方框为60x60像素

# 计算ROI区域的左上角坐标，使其位于图像窗口的中央
roi_x = (frame_width - box_size * 3) // 2
roi_y = (frame_height - box_size * 3) // 2

# 创建时间记录器
clock = time.clock()

# 串口初始化
uart = UART(3, 9600)

# 检测棋子的颜色0阈值
black_threshold = [(8, 37, -5, 22, -12, 7)]
white_threshold = [(99, 100, 0, 3, -1, 0)]

# 初始化棋盘，0表示空，1表示白棋，2表示黑棋
board = [[0 for _ in range(3)] for _ in range(3)]
ai_first_move = True  # AI是否是第一次下棋

# 定义时间间隔为5秒
time_interval = 50
last_time = pyb.millis()


def detect_piece(roi):
    # 检测黑棋子
    black_blobs = img.find_blobs(black_threshold, roi=roi, pixels_threshold=50, area_threshold=50, merge=True)
    for blob in black_blobs:
        if 90 < blob.pixels() < 120:
            return 'B'

    # 检测白棋子
    white_blobs = img.find_blobs(white_threshold, roi=roi, pixels_threshold=50, area_threshold=50, merge=True)
    for blob in white_blobs:
        if 90 < blob.pixels() < 120:
            return 'A'

    return None


def check_winner(board):
    # 检查是否有胜利者
    for i in range(3):
        # 检查行
        if board[i][0] == board[i][1] == board[i][2] != 0:
            return board[i][0]
        # 检查列
        if board[0][i] == board[1][i] == board[2][i] != 0:
            return board[0][i]
    # 检查对角线
    if board[0][0] == board[1][1] == board[2][2] != 0:
        return board[0][0]
    if board[0][2] == board[1][1] == board[2][0] != 0:
        return board[0][2]
    # 检查是否平局
    if all(board[i][j] != 0 for i in range(3) for j in range(3)):
        return 3  # 平局
    return 0  # 未分出胜负


def ai_move(board):
    global ai_first_move  # 声明使用全局变量
    # AI下棋逻辑：首先选择中心位置，然后随机选择空位置
    if ai_first_move:
        ai_first_move = False
        return 1, 1  # 中心位置
    empty_positions = [(i, j) for i in range(3) for j in range(3) if board[i][j] == 0]
    if empty_positions:
        return random.choice(empty_positions)
    return None


while (True):
    clock.tick()

    # 捕获图像
    img = sensor.snapshot()

    # 获取当前时间
    current_time = pyb.millis()

    # 检查是否已经过了5秒
    if (current_time - last_time) >= time_interval:
        last_time = current_time

        # 画出3x3的框并检测棋子
        for i in range(3):
            for j in range(3):
                x = roi_x + i * box_size
                y = roi_y + j * box_size
                roi = (x, y, box_size, box_size)

                # 检测棋子
                piece = detect_piece(roi)
                if piece == 'A':
                    board[j][i] = 1
                    img.draw_rectangle(roi, color=(0, 0, 255))  # 蓝色框
                elif piece == 'B':
                    board[j][i] = 2
                    img.draw_rectangle(roi, color=(0, 255, 0))  # 绿色框
                else:
                    img.draw_rectangle(roi, color=(255, 0, 0))  # 红色框，表示空位置

        # 检查白棋是否下了一步棋
        white_move_detected = any(board[j][i] == 1 for j in range(3) for i in range(3))
        if white_move_detected:
            winner = check_winner(board)
            if winner == 1:
                uart.write(b'\x14')  # 白棋赢了，发送0x14
            elif winner == 2:
                uart.write(b'\x12')  # 黑棋赢了，发送0x12
            elif winner == 3:
                uart.write(b'\x13')  # 平局，发送0x13
            else:
                # AI下棋
                ai_pos = ai_move(board)
                if ai_pos:
                    board[ai_pos[1]][ai_pos[0]] = 2
                    position_code = 0x01 + ai_pos[1] * 3 + ai_pos[0]
                    uart.write(bytes([position_code]))  # 发送黑棋位置
                    print(f"AI move to position: {position_code:#04x}")

    # 打印帧率
    print(clock.fps())
