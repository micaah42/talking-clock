import { HttpClient } from '@angular/common/http';
import { Component, OnInit } from '@angular/core';
import { interpolate, Point, Shape, toCircle } from 'flubber';
import { firstValueFrom } from 'rxjs'

import { r1, r2 } from './graphs/backgroundblob';

class PathMachine {
  public shapes: string[] = [];
  private morph?: (t: number) => string;
  private morphs: ((t: number) => string)[] = [];
  private _d: string = '';

  constructor(private http: HttpClient, files: string[]) {
    this.init(files).then();
  }

  async init(files: string[]) {
    // fetch files
    const paths = await Promise.all(files.map(async (file, i) => {
      return { key: i, value: await firstValueFrom(this.http.get<Shape>(file)) };
    }));

    paths.sort((a, b) => b.key - a.key)
    console.log(paths[0].value)
    this.shapes = paths.map(p => toCircle(p.value, 0, 0, 1)(0));

    // generate morphs between the indivual shapes
    for (let i = 0; i < paths.length - 1; i++) {
      this.morphs.push(interpolate(paths[i].value, paths[i + 1].value));
    }
    this.morphs.push(interpolate(this.shapes.slice(-1)[0], this.shapes[0]));

    // create a function that maps t: [0,1] -> t: [0, morphs] and uses the right interpolation
    this.morph = (t) => {
      const position = paths.length * t;
      const morph = this.morphs[Math.floor(position)];
      const morphT = position - Math.floor(position);
      return morph(morphT);
    }
  }

  set t(t: number) {
    this._d = this.morph!(t);
  }

  get d() {
    return this._d;
  }
}

@Component({
  selector: 'app-animated-svg',
  templateUrl: './animated-svg.component.html',
  styleUrls: ['./animated-svg.component.css']
})
export class AnimatedSvgComponent implements OnInit {
  private backgroundFn?: (t: number) => any;

  d: string = '';
  rotation: string = 'rotate(0deg)';

  turnDurationMs: number = 120000;
  morphDurationMs: number = 60000;

  secondsHandle: PathMachine = new PathMachine(this.http, Array.from({ length: 60 }, (_, k) => `/assets/clock-blobs/seconds-hand/${k}.json`));
  minutesHandle: PathMachine = new PathMachine(this.http, Array.from({ length: 60 }, (_, k) => `/assets/clock-blobs/minutes-hand/${k}.json`));
  hoursHandle: PathMachine = new PathMachine(this.http, Array.from({ length: 12 }, (_, k) => `/assets/clock-blobs/hours-hand/${k}.json`))


  constructor(private http: HttpClient) {
  }

  ngOnInit(): void {
    const t = Array.from({ length: r1.length }, (v, i) => 2 * Math.PI * (i / r1.length));
    console.log('r1', Math.min(...r1), Math.max(...r1));
    console.log('r2', Math.min(...r2), Math.max(...r2));
    console.log('t', Math.min(...t), Math.max(...t));

    const shape1 = r1.map((r, i) => [r * Math.cos(t[i]), r * Math.sin(t[i])]) as Point[];
    const shape2 = r2.map((r, i) => [r * Math.cos(t[i]), r * Math.sin(t[i])]) as Point[];
    const backgroundFn = interpolate(shape1, shape2, { string: false });

    for (let tt of Array.from({ length: 100 }, (v, k) => k / 100)) {
      const p = backgroundFn(tt);
      const x = p.map(p => p[0]);
      const y = p.map(p => p[1]);
      console.log('x', Math.min(...x), Math.max(...x));
      console.log('y', Math.min(...y), Math.max(...y));
    }

    this.backgroundFn = interpolate(shape1, shape2);
    setInterval(() => {
      const time = new Date();

      const deg = -360 * (time.getTime() % this.turnDurationMs) / this.turnDurationMs;
      this.rotation = `rotate(${deg}deg)`

      const morph = (1 + Math.sin(Math.PI * (time.getTime() / this.morphDurationMs))) / 2;
      this.d = this.backgroundFn!(morph);

      const h = time.getHours();
      const m = time.getMinutes();
      const s = time.getSeconds();
      const ms = time.getMilliseconds();
      this.hoursHandle.t = (1 - ((h % 12) + this.step(m / 60)) / 12)
      this.minutesHandle.t = (1 - (m + this.step(s / 60)) / 60);
      this.secondsHandle.t = (1 - (s + this.step(ms / 1000)) / 60)
    }, 40);
  }

  private step(x: number) {
    return (1 - x) * Math.pow(x, 3) + x * (1 + Math.pow(x - 1, 3));
  }
}
