import { Component, OnInit } from '@angular/core';
import { HttpClient } from '@angular/common/http';
import { Shape, interpolate } from 'flubber';
import anime from 'animejs/lib/anime.es';


class ClockGraph {
  private morph?: (t: number) => string;
  private morphs: ((t: number) => string)[] = [];
  private _d: string = '';
  private _t: number = 0;

  constructor(public shapes: Shape[] = []) {
    // generate morphs between the indivual shapes
    for (let i = 0; i < this.shapes.length - 1; i++)
      this.morphs.push(interpolate(this.shapes[i], this.shapes[i + 1]));

    this.morphs.push(interpolate(this.shapes.slice(-1)[0], this.shapes[0]));

    this.morph = (t) => {
      t = t - Math.floor(t / this.morphs.length) * this.morphs.length
      const morph = this.morphs[Math.floor(t)] || this.morphs[0];
      const morphT = t - Math.floor(t);
      return morph(morphT);
    }

    this.t = 0;
  }

  get t() {
    return this._t;
  }

  set t(t: number) {
    this._t = t;
    this._d = this.morph!(t);
  }

  get d() {
    return this._d;
  }
}


@Component({
  selector: 'app-analog-clock',
  templateUrl: './analog-clock.component.html',
  styleUrls: ['./analog-clock.component.scss']
})
export class AnalogClockComponent {
  blobs: any;
  backgroundGraph?: ClockGraph;
  secondsGraph?: ClockGraph;
  minutesGraph?: ClockGraph;
  hoursGraph?: ClockGraph;

  constructor(private http: HttpClient) {
    this.http.get<any>('/assets/blobs.json').subscribe((value) => {
      this.backgroundGraph = new ClockGraph(value.backgrounds);
      this.secondsGraph = new ClockGraph(value.handles_s);
      this.minutesGraph = new ClockGraph(value.handles_m);
      this.hoursGraph = new ClockGraph(value.handles_h);

      setInterval(() => {
        this.backgroundGraph!.t = new Date().getTime() / 5000;
        const now = new Date();

        if (now.getSeconds())
          anime({
            targets: this.secondsGraph!,
            t: -new Date().getSeconds(),
            easing: 'easeOutElastic',
            duration: 500
          }).play();
        else this.secondsGraph!.t = 0;

        if (now.getMinutes())
          anime({
            targets: this.minutesGraph!,
            t: -new Date().getMinutes(),
            duration: 250
          }).play();
        else this.minutesGraph!.t = 0;

        if (now.getHours())
          anime({
            targets: this.hoursGraph!,
            t: -new Date().getHours(),
            duration: 250
          }).play();
        else this.hoursGraph!.t = 0;

      }, 100)
    });
  }

}
