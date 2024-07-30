import sensor, image, time, machine

sensor.reset()
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QVGA)
sensor.set_framerate(80)
sensor.skip_frames(time=2000)

# Initialize the display
sensor.set_auto_gain(False)
sensor.set_auto_whitebal(False)

mode = 1
last_pressed = False

'''
button = machine.Pin('P7', machine.Pin.IN, machine.Pin.PULL_UP)  # Change pin according to your setup

def check_mode_switch(img, disp_w, disp_h):
    global mode, last_pressed
    if not button.value():  # Button pressed
        if not last_pressed:
            last_pressed = True
            mode += 1
            if mode > 2:
                mode = 1
    else:
        last_pressed = False
    img.draw_string(2, 10, f"mode {mode}", color=(255, 255, 255), scale=1.5)
    img.draw_rectangle(0, 0, 100, 40, color=(255, 255, 255), thickness=2)
'''


def mash():
    find_center_method = 1  # 1, 2
    area_threshold = 80
    pixels_threshold = 50
    thresholds = [(30, 100, 15, 127, 15, 127)]  # red

    while mode == 1:
        img = sensor.snapshot()
        '''
        check_mode_switch(img, sensor.width(), sensor.height())
        '''

        gray = img.copy().to_grayscale()

        # Edge detection
        edged = gray.find_edges(image.EDGE_CANNY, threshold=(50, 150))

        # Dilate the edges
        edged.dilate(1)

        # Find contours
        blobs = edged.find_blobs([(100, 255)], pixels_threshold=pixels_threshold, area_threshold=area_threshold,
                                 merge=True)
        if blobs:
            largest_blob = max(blobs, key=lambda b: b.area())
            corners = largest_blob.min_corners()

            if len(corners) == 4:
                img.draw_rectangle(largest_blob.rect(), color=(0, 255, 0), thickness=2)
                img.draw_edges(corners, color=(0, 255, 0))

                tl, bl, br, tr = corners

                # Calculate 3x3 grid intersections
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
import sensor, image, time, machine

sensor.reset()
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QVGA)
sensor.set_framerate(80)
sensor.skip_frames(time=2000)

# Initialize the display
sensor.set_auto_gain(False)
sensor.set_auto_whitebal(False)

mode = 1
last_pressed = False

'''
button = machine.Pin('P7', machine.Pin.IN, machine.Pin.PULL_UP)  # Change pin according to your setup

def check_mode_switch(img, disp_w, disp_h):
    global mode, last_pressed
    if not button.value():  # Button pressed
        if not last_pressed:
            last_pressed = True
            mode += 1
            if mode > 2:
                mode = 1
    else:
        last_pressed = False
    img.draw_string(2, 10, f"mode {mode}", color=(255, 255, 255), scale=1.5)
    img.draw_rectangle(0, 0, 100, 40, color=(255, 255, 255), thickness=2)
'''


def mash():
    find_center_method = 1  # 1, 2
    area_threshold = 80
    pixels_threshold = 50
    thresholds = [(30, 100, 15, 127, 15, 127)]  # red

    while mode == 1:
        img = sensor.snapshot()
        '''
        check_mode_switch(img, sensor.width(), sensor.height())
        '''

        gray = img.copy().to_grayscale()

        # Edge detection
        edged = gray.find_edges(image.EDGE_CANNY, threshold=(50, 150))

        # Dilate the edges
        edged.dilate(1)

        # Find contours
        blobs = edged.find_blobs([(100, 255)], pixels_threshold=pixels_threshold, area_threshold=area_threshold,
                                 merge=True)
        if blobs:
            largest_blob = max(blobs, key=lambda b: b.area())
            corners = largest_blob.min_corners()

            if len(corners) == 4:
                img.draw_rectangle(largest_blob.rect(), color=(0, 255, 0), thickness=2)
                img.draw_edges(corners, color=(0, 255, 0))

                tl, bl, br, tr = corners

                # Calculate 3x3 grid intersections
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
