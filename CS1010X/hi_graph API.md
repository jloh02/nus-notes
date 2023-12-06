## Drawing
`draw_points(num_points, curve)`

`draw_points_scaled(num_points, curve)`
Scale to viewport

`draw_connected(num_points, curve)`
Connect points when drawn - The higher `num_points`, the higher the accuracy

`draw_connected_scaled(num_points, curve)`

## Points
`make_point`: `(Number, Number) → Point`
Takes 2 inputs and makes a point

`x_of` : `(Point) → Number`
Get x coordinate

`y_of` : `(Point) → Number`
Get y coordinate

## Curves
`Curve` : `(Unit-Interval) → Point`
- `Unit-Interval` should be treated as a percentage of the curve `t` from `0.0` to `1.0`
- Returns the point at each percentage `t`
Example:
```python
def unit_circle(t): 
	return make_point(sin(2*pi*t), cos(2*pi*t))
```


### Transformations

Translations
`translate(x,y)(curve)`

Scaling
`scale_xy(x,y)(curve)`

Rotate
`rotate(theta)(curve)`

Rotate curve by 90 degrees
`rotate_90 : (Curve) → Curve`

Connecting curves
`connect_rigidly(curve1, curve2)`
- `Binary-Transform : (Curve, Curve) → Curve`
- Scales both curves to become `curve1(2t)` and `curve2(2t)` and draws `curve1` then `curve2` with respect to `t`
`connect_ends(curve1,curve2)`
- Connect `curve2` at the end of `curve1`
`revert(curve)`
- Reverse t=0 and t=1 so draw from 1 to 0
- e.g. `connect_ends(revert(curve1),curve2)`

`put_in_standard_position`
- Fit curve into \[0,1\]

### Gospers
```python
def gosperize(curve):
    scaled_curve = scale(sqrt(2)/2)(curve)
    left_curve = rotate(pi/4)(scaled_curve)
    right_curve = translate(0.5,0.5)(rotate(-pi/4)(scaled_curve))
    return connect_rigidly(left_curve, right_curve)

def gosper_curve(level):
    return repeated(gosperize, level)(unit_line)

def show_connected_gosper(level):
    squeezed_curve = squeeze_curve_to_rect(-0.5, -0.5, 1.5, 1.5)(gosper_curve(level))
    draw_connected(200, squeezed_curve)

  def gosperize_with_angle(theta):
    def inner_gosperize(curve):
        scale_factor = (1 / cos(theta)) / 2
        scaled_curve = scale(scale_factor)(curve)
        left_curve = rotate(theta)(scaled_curve)
        right_curve = translate(0.5,sin(theta)*scale_factor)(rotate(-theta)(scaled_curve))
        return connect_rigidly(left_curve, right_curve)
    return inner_gosperize

def gosper_curve_with_angle(level, angle_at_level):
    if level == 0:
        return unit_line
    else:
        angle = angle_at_level(level)
        return gosperize_with_angle(angle)(gosper_curve_with_angle(level-1, angle_at_level))
```