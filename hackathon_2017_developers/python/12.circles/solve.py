# Author: Maxim Yurchuk

EPS = 1e-06

circles = []


def is_in_circle((r, x, y), (x0, y0)):
    dx = abs(x0 - x)
    dy = abs(y0 - y)
    return dx ** 2 + dy ** 2 < r ** 2


def is_intersect_with_circle((r, x, y), ((x0, y0), (x1, y1))):
    if x0 == x1:
        # vertical line
        if (y0 < y and y1 < y) or (y0 > y and y1 > y):
            return is_in_circle((r, x, y), (x0, y0)) or is_in_circle((r, x, y), (x1, y1))
        else:
            return abs(x0 - x) <= r
    elif y0 == y1:
        # horizontal line
        return is_intersect_with_circle((r, y, x), ((y0, x0), (y1, x1)))
        pass
    else:
        # unsupported
        raise RuntimeError("unsupported")


def get_square_corners((x0, y0), (x1, y1)):
    return [
        (x0, y0),
        (x0, y1),
        (x1, y0),
        (x1, y1),
    ]


def get_area(((x0, y0), (x2, y2))):
    """
    
    :return: (area, inaccuracy) 
    """

    square_area = (x2-x0) * (y2 - y0)
    if square_area < EPS:
        return (square_area/2, square_area/2)



    for circle in circles:
        corners = get_square_corners((x0, y0), (x2, y2))
        all_corners_in_circle = True
        for corner in corners:
            if not is_in_circle(circle, corner):
                all_corners_in_circle = False
                break
        if all_corners_in_circle:
            return (square_area, 0)

    # there in no circle that cover whole square
    # let check, maybe this whole square do not intersect with any circle

    is_square_intersect_with_circle = False
    square_segments = [
        ((x0, y0), (x2, y0)),
        ((x2, y0), (x2, y2)),
        ((x2, y2), (x0, y2)),
        ((x0, y2), (x0, y0)),
    ]

    for circle in circles:
        r, x, y = circle
        if x0 < x < x2 and y0 < y < y2:
            is_square_intersect_with_circle = True
            break
        for segment in square_segments:
            if is_intersect_with_circle(circle, segment):
                is_square_intersect_with_circle = True
                break

    if not is_square_intersect_with_circle:
        return (0, 0)

    # let divide
    x1 = (x0 + x2) / 2
    y1 = (y0 + y2) / 2
    sub_squares = [
        ((x0, y0), (x1, y1)),
        ((x1, y1), (x2, y2)),
        ((x0, y1), (x1, y2)),
        ((x1, y0), (x2, y1)),
    ]

    result = [0 for i in range(2)]
    for sub_square in sub_squares:
        sub_result = get_area(sub_square)
        for i in range(len(sub_result)):
            result[i] += sub_result[i]
    return tuple(result)

max_c = - 10 ** 9
min_c = 10 ** 9


with open("input.txt") as f:
    lines = f.readlines()
    N = int(lines[0].strip())
    for i in range(1, N+1):
        r, x, y = [int(x) for x in lines[i].strip().split()]
        circles.append((r, x, y))
        max_c = max(max_c, x+r, y+r)
        min_c = min(min_c, x-r, y-r)


min_c = float(min_c)
max_c = float(max_c)

area, inaccuracy = get_area(((min_c, min_c), (max_c, max_c)))

print int(round(area))



