import sensor, image, time, machine
from machine import UART

# 初始化传感器
sensor.reset()
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QVGA)
sensor.set_framerate(80)
sensor.skip_frames(time=2000)

# 关闭自动增益和白平衡
sensor.set_auto_gain(False)
sensor.set_auto_whitebal(False)

mode = 1
last_pressed = False

uart = UART(2, baudrate=115200)  # 初始化UART，波特率设置为115200


def mash():
    find_center_method = 1  # 1, 2
    area_threshold = 80
    pixels_threshold = 50
    thresholds = [(30, 100, 15, 127, 15, 127)]  # red

    while mode == 1:
        img = sensor.snapshot()

        gray = img.copy().to_grayscale()

        # 边缘检测
        edged = gray.find_edges(image.EDGE_CANNY, threshold=(50, 150))

        # 膨胀边缘
        edged.dilate(1)

        # 找轮廓
        blobs = edged.find_blobs([(100, 255)], pixels_threshold=pixels_threshold, area_threshold=area_threshold,
                                 merge=True)
        if blobs:
            largest_blob = max(blobs, key=lambda b: b.area())
            corners = largest_blob.min_corners()

            if len(corners) == 4:
                img.draw_rectangle(largest_blob.rect(), color=(0, 255, 0), thickness=2)
                img.draw_edges(corners, color=(0, 255, 0))

                tl, bl, br, tr = corners

                # 计算3x3网格交点
                cross_points = []
                for i in range(4):
                    for j in range(4):
                        cross_x = int(
                            (tl[0] * (3 - i) + tr[0] * i) * (3 - j) / 9 + (bl[0] * (3 - i) + br[0] * i) * j / 9)
                        cross_y = int(
                            (tl[1] * (3 - i) + tr[1] * i) * (3 - j) / 9 + (bl[1] * (3 - i) + br[1] * i) * j / 9)
                        cross_points.append((cross_x, cross_y))
                        img.draw_circle(cross_x, cross_y, 3, color=(0, 255, 0), thickness=-1)

                centers = []
                if find_center_method == 1:
                    for i in range(3):
                        for j in range(3):
                            center_x = int((cross_points[i * 4 + j][0] + cross_points[i * 4 + j + 1][0] +
                                            cross_points[(i + 1) * 4 + j][0] + cross_points[(i + 1) * 4 + j + 1][
                                                0]) / 4)
                            center_y = int((cross_points[i * 4 + j][1] + cross_points[i * 4 + j + 1][1] +
                                            cross_points[(i + 1) * 4 + j][1] + cross_points[(i + 1) * 4 + j + 1][
                                                1]) / 4)
                            centers.append((center_x, center_y))
                            img.draw_circle(center_x, center_y, 2, color=(0, 255, 0), thickness=-1)

                elif find_center_method == 2:
                    roi = [min(tl[0], bl[0]), min(tl[1], tr[1]), max(tr[0], br[0]) - min(tl[0], bl[0]),
                           max(bl[1], br[1]) - min(tl[1], tr[1])]
                    img.draw_rectangle(roi[0], roi[1], roi[2], roi[3], color=(255, 255, 255), thickness=2)
                    blobs = img.find_blobs(thresholds, roi=roi, pixels_threshold=pixels_threshold,
                                           area_threshold=area_threshold, merge=True)
                    for b in blobs:
                        centers.append((b.cx(), b.cy()))
                        img.draw_circle(b.cx(), b.cy(), 2, color=(255, 255, 255), thickness=-1)
                else:
                    raise Exception("find_center_method value error")

                # 发送 centers 列表中的每个元素
                for center in centers:
                    center_data = f"{center[0]},{center[1]}\r\n"
                    uart.write(center_data.encode('utf-8'))  # 将中心点坐标转换为字符串并发送

                # 检查UART数据并回显
                uart_num = uart.any()  # 获取当前串口数据数量
                if uart_num:
                    uart_str = uart.read(uart_num)  # 读取UART数据
                    print(uart_str)  # 打印接收到的数据
                    uart.write(uart_str)  # 将读取到的串口数据发回

                if len(centers) == 9:
                    centers = sorted(centers, key=lambda c: (c[1], c[0]))
                    for i, center in enumerate(centers):
                        img.draw_string(center[0], center[1], str(i + 1), color=(255, 255, 255), scale=2, thickness=-1)

        sensor.flush()


def node():
    area_threshold = 300
    pixels_threshold = 300
    thresholds = [(30, 100, 15, 127, 15, 127),  # red
                  (0, 30, 0, 64, -128, 127),  # black
                  (0, 30, 0, 64, 0, 64)]  # white

    while mode == 2:
        img = sensor.snapshot()
        check_mode_switch(img, sensor.width(), sensor.height())

        blobs = img.find_blobs(thresholds, pixels_threshold=pixels_threshold, area_threshold=area_threshold, merge=True)
        for b in blobs:
            corners = b.min_corners()
            if b.code() == 1:
                img.draw_edges(corners, color=(255, 255, 0))
            elif b.code() == 2:
                if b.area() < 800:
                    img.draw_circle(b.cx(), b.cy(), b.enclosing_circle()[2], color=(0, 255, 0), thickness=2)
            elif b.code() == 4:
                img.draw_circle(b.cx(), b.cy(), b.enclosing_circle()[2], color=(255, 0, 0), thickness=2)

        sensor.flush()


def main():
    while True:
        mash()
        node()


if __name__ == '__main__':
    try:
        main()
    except Exception as e:
        print(e)
        while True:
            time.sleep(1000)
