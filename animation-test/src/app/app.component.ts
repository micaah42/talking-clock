import {Component, OnInit} from '@angular/core';
import {interpolate, Shape} from 'flubber';

@Component({
  selector: 'app-root',
  templateUrl: './app.component.html',
  styleUrls: ['./app.component.scss']
})
export class AppComponent implements OnInit {
  title = 'animation-test';
  backgroundPath: string = '';
  hourPointerPath: string = '';
  t: number = 0;


  constructor() {
    const path1 = this.generatePath(500, 0.8);
    const path2 = this.generatePath(600, 0.8);
    const interpolator0 = interpolate(path1, path2)

    const path3 = this.generatePath(200, 0.9, 0.05);
    const path4 = this.generatePath(200, 0.9, 0.1);
    const interpolator1 = interpolate(path3, path4);
    setInterval(() => {
      this.t += 0.005;
      const x = Math.sin(this.t) ** 2

      this.backgroundPath = interpolator0(x / 5);
      this.hourPointerPath = interpolator1(x)
    })
  }

  ngOnInit() {
  }

  public get hourTransform(): string {
    return `rotate(${360 * (new Date().getSeconds() / 60)}) translate(0, -0.5)`
  }

  public generatePath(numPoints: number, noise: number = 0.5, xFactor: number = 1): Shape {
    let points: Shape = [];
    let radii: number[] = [];

    const shift = 2 * Math.PI * window.crypto.getRandomValues<Int32Array>(new Int32Array(1))[0] / (2**32);
    for (let i = 0; i < numPoints; i++) {
      const t = shift + 2 * Math.PI * i / numPoints;
      const x = xFactor * Math.sin(t);
      const y = Math.cos(t);
      const radius = (1 - noise) + noise * AppComponent.randomGaussian(10)
      points.push([x, y]);
      radii.push(radius);
    }

    for (let j = 0; j < 150; j++) {
      for (let i = 1; i <= numPoints; i++) {
        radii[i] = (radii[(i - 1) % numPoints] + radii[i % numPoints] + radii[(i + 1) % numPoints]) / 3;
      }
    }

    points = points.map((value, i) => {
      return [radii[i] * value[0], radii[i] * value[1]]
    });

    return points;
  }

  private static randomGaussian(n: number): number {
    let sum = 0;
    for (let i = 0; i < n; i++)
      sum += window.crypto.getRandomValues<Int32Array>(new Int32Array(1))[0] / (2**32);
    sum /= n;
    return sum;
  }
}
