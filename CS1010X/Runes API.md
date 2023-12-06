## Run Python in Interactive Shell Mode
Required to run runes API if running from CLI

```
python -i <file.py>
```


## Random Tips
- If stacking, when wrong flipping, don't save and flip, instead swap `quarter_turn_right` with `quarter_turn_left`


## Primitive Runes

| | | | |
|:---:|:---:|:---:|:---:|
|![[rcross_bb.png]] |![[nova_bb.png]] |![[sail_bb.png]] |![[heart_bb.png]] |
| `rcross_bb` |`nova_bb` | `sail_bb`|`heart_bb`|
| ![[corner_bb.png]]| ![[blank_bb.png]]|![[black_bb.png]] | |
|`corner_bb` |`blank_bb` |`black_bb` | |




## Given Methods
### Generic
`clear_all()`
Clear all items 

`show(rune)`
Display on screen. Multiple shows will overlap the images on the same plane

### 2D Methods
`quarter_turn_right(rune)`
Rotate rune clockwise by 90 degrees

`quarter_turn_left(rune)`
Rotate rune anticlockwise by 90 degrees

`stack(rune1, rune2)`
Create a column, with `rune1` filling the top half and `rune2` filling the bottom half

`stack_frac(frac, rune1, rune2)`
Create a `stack` with `rune1` filling `frac` of the top area

`stackn(n, rune)`
Create a `stack` of "`n`" `runes`

`beside(rune1, rune2)`
Create a row, with `rune1` filling the left half and `rune2` filling the right half

`make_cross(rune)`
Place 4 of `rune` in a 2x2 grid, rotating around the center to make up a cross


### 3D Methods
`overlay(rune1, rune2)`
Place `rune` at the surface, and `rune2` at 50% depth - `rune1` takes up 50% of the height

`overlay_frac(frac, rune1, rune2)`
Similar to `overlay` but `rune1` takes up `frac` of the height while `rune2` takes up the remainder

`stereogram(rune)`
Display stereogram - pixelated colourful distorted thing

`anaglyph(rune)`
Display `rune` as anaglyph - Red blue plane split which can be viewed using 3d glasses

`hollusion(rune)`
Display `rune` as hollusion - Moving image to create illusion of 3D



## Derived Methods
`besiden(n, rune)`
To be used like `stackn`
```python
def besiden(n, rune):
	return quarter_turn_left(stackn(n, quarter_turn_right(rune)))
```

