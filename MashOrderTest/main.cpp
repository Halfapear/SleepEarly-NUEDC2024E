#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>

struct Point {
    int x, y;
    int label;
};

// 计算两个点之间的欧氏距离
double distance(const Point& p1, const Point& p2) {
    return std::sqrt(std::pow(p1.x - p2.x, 2) + std::pow(p1.y - p2.y, 2));
}

// 根据旋转后的点，确定九宫格的位置
std::vector<Point> matchPoints(std::vector<Point>& points) {
    // 按照x + y排序，找到1和9
    std::sort(points.begin(), points.end(), [](const Point& p1, const Point& p2) {
        return (p1.x + p1.y) < (p2.x + p2.y);
    });

    points[0].label = 1;
    points[8].label = 9;

    // 确定1的坐标x是否是小值
    if (points[0].x < points[8].x) {
        // 按照y值排序，找到3和7
        std::sort(points.begin(), points.end(), [](const Point& p1, const Point& p2) {
            return p1.y < p2.y;
        });
        points[2].label = 3;
        points[6].label = 7;
    } else {
        // 按照y值排序，找到7和3
        std::sort(points.begin(), points.end(), [](const Point& p1, const Point& p2) {
            return p1.y > p2.y;
        });
        points[2].label = 7;
        points[6].label = 3;
    }

    points[4].label = 5;  // 中心点

    // 计算剩余点的中点
    Point mid1_7 = { (points[0].x + points[6].x) / 2, (points[0].y + points[6].y) / 2, 0 };
    Point mid3_9 = { (points[2].x + points[8].x) / 2, (points[2].y + points[8].y) / 2, 0 };
    Point mid7_9 = { (points[6].x + points[8].x) / 2, (points[6].y + points[8].y) / 2, 0 };
    Point mid1_3 = { (points[0].x + points[2].x) / 2, (points[0].y + points[2].y) / 2, 0 };

    // 匹配2, 4, 6, 8
    for (auto& point : points) {
        if (point.label == 0) {
            if (distance(point, mid1_3) < distance(point, mid3_9) && distance(point, mid1_3) < distance(point, mid1_7))
                point.label = 2;
            else if (distance(point, mid7_9) < distance(point, mid1_3) && distance(point, mid7_9) < distance(point, mid3_9))
                point.label = 8;
            else if (distance(point, mid1_7) < distance(point, mid1_3) && distance(point, mid1_7) < distance(point, mid7_9))
                point.label = 4;
            else
                point.label = 6;
        }
    }

    return points;
}

int main() {
    std::vector<Point> points(9);

    std::cout << "请输入9个点的坐标 (x, y)，每个坐标用空格分隔:" << std::endl;
    for (int i = 0; i < 9; ++i) {
        std::cout << "输入点 " << i + 1 << " 的坐标: ";
        std::cin >> points[i].x >> points[i].y;
        points[i].label = 0;
    }

    std::vector<Point> matchedPoints = matchPoints(points);

    for (const auto& point : matchedPoints) {
        std::cout << "Label: " << point.label << ", X: " << point.x << ", Y: " << point.y << std::endl;
    }

    return 0;
}
