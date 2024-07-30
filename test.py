import sensor, image, time, machine
from machine import UART

# 定义黑色和白色的颜色阈值
# 黑色阈值 (L Min, L Max, A Min, A Max, B Min, B Max)
black_threshold = (0, 30, -128, 127, -128, 127)
# 白色阈值
white_threshold = (50, 255, -128, 127, -128, 127)

# 初始化传感器
sensor.reset()
sensor.set_pixformat(sensor.RGB565)  # 设置传感器的像素格式为RGB565
sensor.set_framesize(sensor.QVGA)  # 设置传感器的帧尺寸为QVGA（320x240）
sensor.set_framerate(80)  # 设置帧率为80帧每秒
sensor.skip_frames(time=2000)  # 跳过2000毫秒的帧来稳定传感器

# 关闭自动增益和白平衡
sensor.set_auto_gain(False)  # 关闭自动增益
sensor.set_auto_whitebal(False)  # 关闭自动白平衡

mode = 1  # 初始模式设置为1
last_pressed = False  # 初始化按钮状态

uart = UART(2, baudrate=115200)  # 初始化UART，波特率设置为115200


def mash():
    find_center_method = 1  # 使用的方法，1或2
    area_threshold = 80  # 区域阈值
    pixels_threshold = 50  # 像素阈值
    thresholds = [(30, 100, 15, 127, 15, 127)]  # 红色阈值

    while mode == 1:
        img = sensor.snapshot()  # 拍摄一张图片

        gray = img.copy().to_grayscale()  # 转换为灰度图像

        # 边缘检测
        edged = gray.find_edges(image.EDGE_CANNY, threshold=(50, 150))

        # 膨胀边缘：对边缘图像进行膨胀操作，使边缘更加明显。这一步可以填充边缘中的小孔，使边缘更加连续
        edged.dilate(1)

        # 找轮廓
        blobs = edged.find_blobs([(100, 255)], pixels_threshold=pixels_threshold, area_threshold=area_threshold,
                                 merge=True)
        if blobs:
            largest_blob = max(blobs, key=lambda b: b.area())  # 找到最大的轮廓
            corners = largest_blob.min_corners()  # 获取最小包围四边形的角点

            if len(corners) == 4:
                img.draw_rectangle(largest_blob.rect(), color=(0, 255, 0), thickness=2)  # 画出轮廓矩形
                img.draw_edges(corners, color=(0, 255, 0))  # 画出轮廓的边缘

                tl, bl, br, tr = corners  # 分别取出四个角点

                # 计算3x3网格交点
                cross_points = []
                for i in range(4):
                    for j in range(4):
                        cross_x = int(
                            (tl[0] * (3 - i) + tr[0] * i) * (3 - j) / 9 + (bl[0] * (3 - i) + br[0] * i) * j / 9)
                        cross_y = int(
                            (tl[1] * (3 - i) + tr[1] * i) * (3 - j) / 9 + (bl[1] * (3 - i) + br[1] * i) * j / 9)
                        cross_points.append((cross_x, cross_y))
                        img.draw_circle(cross_x, cross_y, 3, color=(0, 255, 0), thickness=-1)  # 画出交点

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
                            img.draw_circle(center_x, center_y, 2, color=(0, 255, 0), thickness=-1)  # 画出中心点

                elif find_center_method == 2:
                    roi = [min(tl[0], bl[0]), min(tl[1], tr[1]), max(tr[0], br[0]) - min(tl[0], bl[0]),
                           max(bl[1], br[1]) - min(tl[1], tr[1])]
                    img.draw_rectangle(roi[0], roi[1], roi[2], roi[3], color=(255, 255, 255), thickness=2)  # 画出感兴趣区域
                    blobs = img.find_blobs(thresholds, roi=roi, pixels_threshold=pixels_threshold,
                                           area_threshold=area_threshold, merge=True)
                    for b in blobs:
                        centers.append((b.cx(), b.cy()))
                        img.draw_circle(b.cx(), b.cy(), 2, color=(255, 255, 255), thickness=-1)  # 画出中心点
                else:
                    raise Exception("find_center_method value error")  # 如果方法值不正确，抛出异常

                # 发送 centers 列表中的每个元素，并标记1到9
                for i, center in enumerate(centers):
                    center_data = f"{i + 1}: {center[0]},{center[1]}\r\n"
                    uart.write(center_data.encode('utf-8'))  # 将中心点坐标转换为字符串并发送

                '''

                # 检查UART数据并回显
                uart_num = uart.any()  # 获取当前串口数据数量
                if uart_num:
                    uart_str = uart.read(uart_num)  # 读取UART数据
                    print(uart_str)  # 打印接收到的数据
                    uart.write(uart_str)  # 将读取到的串口数据发回
                '''

                if len(centers) == 9:
                    # 对中心点进行排序，首先按y坐标排序，然后按x坐标排序
                    centers = sorted(centers, key=lambda c: (c[1], c[0]))  # 按y和x坐标排序中心点
                    for i, center in enumerate(centers):
                        img.draw_string(center[0], center[1], str(i + 1), color=(255, 255, 255), scale=2,
                                        thickness=-1)  # 标记中心点

        sensor.flush()  # 刷新传感器


def node():
    area_threshold = 300  # 区域阈值
    pixels_threshold = 300  # 像素阈值
    thresholds = [(30, 100, 15, 127, 15, 127),  # 红色
                  (0, 30, 0, 64, -128, 127),  # 黑色
                  (0, 30, 0, 64, 0, 64)]  # 白色

    # 插入
    # 查找黑色和白色的色块
    for blob in img.find_blobs([black_threshold, white_threshold], pixels_threshold=200, area_threshold=200):
        # 检查是否为圆形
        if blob.roundness() > 0.5:  # 通过圆度来判断
            if blob.code() == 1:  # 黑色
                img.draw_circle(blob.cx(), blob.cy(), int(blob.w() / 2), color=(0, 0, 0))  # 绘制黑色圆
            elif blob.code() == 2:  # 白色
                img.draw_circle(blob.cx(), blob.cy(), int(blob.w() / 2), color=(255, 255, 255))  # 绘制白色圆
            img.draw_cross(blob.cx(), blob.cy())  # 绘制中心点
    # print(clock.fps()) # 打印每秒帧数（FPS）
    #

    while mode == 2:
        img = sensor.snapshot()  # 拍摄一张图片
        check_mode_switch(img, sensor.width(), sensor.height())  # 检查模式切换

        blobs = img.find_blobs(thresholds, pixels_threshold=pixels_threshold, area_threshold=area_threshold, merge=True)
        for b in blobs:
            corners = b.min_corners()  # 获取最小包围四边形的角点
            if b.code() == 1:  # 如果是红色
                img.draw_edges(corners, color=(255, 255, 0))  # 画出边缘
            elif b.code() == 2:  # 如果是黑色
                if b.area() < 800:  # 面积小于800
                    img.draw_circle(b.cx(), b.cy(), b.enclosing_circle()[2], color=(0, 255, 0), thickness=2)  # 画出圆形
            elif b.code() == 4:  # 如果是白色
                img.draw_circle(b.cx(), b.cy(), b.enclosing_circle()[2], color=(255, 0, 0), thickness=2)  # 画出圆形

        sensor.flush()  # 刷新传感器


def main():
    while True:
        mash()  # 执行mash函数
        node()  # 执行node函数


if __name__ == '__main__':
    try:
        main()  # 运行主函数
    except Exception as e:
        print(e)  # 打印异常信息
        while True:
            time.sleep(1000)  # 进入休眠状态
