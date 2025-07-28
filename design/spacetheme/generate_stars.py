#!/usr/bin/env python

import os
import svg
import random

height = 1080
width = 1920
n_stars = 128
n_layers = 8

colors = [
  '#9bb0ff',
  '#aabfff',
  '#cad7ff',
  '#f8f7ff',
  '#fff4ea',
  '#ffd2a1',
  '#ffcc6f',
]

def randfloat(a, b):
    return a + (b - a) * random.random()


filters = []
layers = []

for layer in range(n_layers):
    stars = []
    
    filter = svg.Filter(
        id=f'blur{layer}',
        elements=[
            svg.FeDropShadow(
                stdDeviation=randfloat(12, 25),
                flood_color=random.choice(colors),
                dx=0,
                dy=0
            )
        ]
    )
    
    for star in range(n_stars // n_layers):
        size = randfloat(1.5, 4.5)
        y = randfloat(0, height)
        x = randfloat(0, width)

        stars.append(svg.Circle(
            cx=x,
            cy=y,
            r=size,
            fill=random.choice(colors),
        ))

    g = svg.G(elements=stars, filter=f'url(#blur{layer})', )
    filters.append(filter)
    layers.append(g)
    
    canvas = svg.SVG(height=height, width=width, elements=[svg.Defs(elements=filters), g])
    output_directory = 'layers'
    os.makedirs(output_directory, exist_ok=True)
    with open(f'{output_directory}/stars{layer}.svg', 'w') as f:
        f.write(str(canvas))

canvas = svg.SVG(
    height=height,
    width=width,
    elements=[
        svg.Defs(elements=filters),
        svg.Rect(fill='black', width=width, height=height),
        *layers
    ]
)

with open(f'{output_directory}/fullscene.svg', 'w') as f:
    f.write(str(canvas))
