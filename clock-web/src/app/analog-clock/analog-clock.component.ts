import { Component, OnInit } from '@angular/core';
import { HttpClient ,HttpClientModule} from '@angular/common/http';
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

  get d(): string {
    return this._d;
  }
}


@Component({
  selector: 'app-analog-clock',
  templateUrl: './analog-clock.component.html',
  styleUrls: ['./analog-clock.component.scss'],
  imports: [HttpClientModule],
  standalone: true,
})
export class AnalogClockComponent {
  blobs: any;
  backgroundGraph?: ClockGraph;
  secondsGraph?: ClockGraph;
  minutesGraph?: ClockGraph;
  hoursGraph?: ClockGraph;
  now: Date = new Date();

  constructor(private http: HttpClient) {
    this.http.get<any>('blobs.json').subscribe((value) => {
      const now = new Date();
      this.backgroundGraph = new ClockGraph(value.backgrounds);
      this.backgroundGraph.t = -now.getTime() / 8000,
        this.secondsGraph = new ClockGraph(value.handles_s);
      this.secondsGraph.t = -now.getTime() / 1000;
      this.minutesGraph = new ClockGraph(value.handles_m);
      this.minutesGraph.t = -now.getTime() / 60000;
      this.hoursGraph = new ClockGraph(value.handles_h);
      this.hoursGraph.t = -now.getTime() / 3600000;

      setInterval(() => {

        const now = new Date();

        anime({
          targets: this.backgroundGraph!,
          t: -now.getTime() / 8000,
          easing: 'easeOutElastic',
          duration: 500
        }).play();

        anime({
          targets: this.secondsGraph!,
          t: -Math.floor(now.getTime() / 1000),
          easing: 'easeOutElastic',
          duration: 500
        }).play();

        anime({
          targets: this.minutesGraph!,
          t: -Math.floor(now.getTime() / 60000),
          duration: 500
        }).play();

        this.hoursGraph!.t = -(now.getHours() + now.getMinutes() / 60);
        this.now = now;
      }, 100)
    });
  }

}
