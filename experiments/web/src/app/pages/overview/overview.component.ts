import { Component, OnDestroy, OnInit } from '@angular/core';
import { animate, style, transition, trigger } from '@angular/animations';
import { Subscription } from "rxjs";

import { RemotingService, Info } from "src/app/remoting/remoting.service";
import { Alarm } from "src/models/alarm";

@Component({
  selector: 'app-overview',
  templateUrl: './overview.component.html',
  styleUrls: ['./overview.component.scss'],
  animations: [
    trigger('rocket', [
      transition(':enter', [style({ 'left': "-100%" }),
      animate(10000, style({ 'left': "200%" })),])
    ]),
  ]
})
export class OverviewComponent implements OnInit, OnDestroy {
  private _subs: Subscription[] = [];
  public nextAlarms: Alarm[] = [];
  public nextIds: number[] = [];
  public timeLeft: number = 0;

  public timeToFly: boolean = true;

  constructor(private remoting: RemotingService) {
    setInterval(() => {
      this.timeToFly = !this.timeToFly;
      setTimeout(() => this.timeToFly = !this.timeToFly, 100)
    }, 10000);
  }


  ngOnInit(): void {
    this._subs = []
    this.remoting.cmd<number[]>('remoting', 'value', ['alarms.nextIds']).then(value => {
      value.forEach(id => this.remoting.cmd<Alarm>('alarms.model', 'at', [id]).then(alarm => {
        this.nextAlarms.push(alarm);
      }))
    })
  }

  ngOnDestroy() {
    this._subs.forEach(sub => sub.unsubscribe());
  }

}
