import { Component, OnDestroy, OnInit } from '@angular/core';
import { animate, style, transition, trigger } from '@angular/animations';
import { MatCardModule } from '@angular/material/card';
import { Observable, Subject, Subscription, takeUntil } from "rxjs";

import { AnalogClockComponent } from '../analog-clock/analog-clock.component';
import { CommonModule, DatePipe } from '@angular/common';
import { RemotingService } from '../services/remoting.service';
import { ValueDisplayComponent } from '../components/value-display/value-display.component';
import { RemoteObject } from '../services/remoteobject';

//import { RemotingService, Info } from "src/app/remoting/remoting.service";
//import { Alarm } from "src/models/alarm";

class Alarm {
  static __typeId = 65583
  constructor(
    public activated: boolean,
    public name: string,
    public nextTimeout: number,
    public repeatRule: boolean[],
    public sound: string,
    public time: string,
  ) { }
}


class ActionDay {
  constructor(
    public date: number,
    public name: string,
    public desc: string,
    public link: string
  ) { }
}


@Component({
  selector: 'app-overview',
  templateUrl: './overview.component.html',
  styleUrls: ['./overview.component.scss'],
  imports: [MatCardModule, AnalogClockComponent, CommonModule, ValueDisplayComponent],
  standalone: true,
  animations: [
    trigger('rocket', [
      transition(':enter', [style({ 'left': "-100%" }),
      animate(10000, style({ 'left': "200%" })),])
    ]),
  ]
})
export class OverviewComponent implements OnInit, OnDestroy {
  private destroy$ = new Subject<void>();
  public actionDays: RemoteObject<ActionDay[]>;
  public nextAlarm: RemoteObject<Alarm>;
  public nextIds: number[] = [];
  public timeLeft: number = 0;


  public timeToFly: boolean = true;

  constructor(private remoting: RemotingService) {
    setInterval(() => {
      this.timeToFly = !this.timeToFly;
      setTimeout(() => this.timeToFly = !this.timeToFly, 100)
    }, 10000);

    this.actionDays = new RemoteObject('actiondays.days', remoting, this.destroy$);
    this.nextAlarm = new RemoteObject('alarms.nextAlarm', remoting, this.destroy$);
  }

  ngOnInit(): void {

  }

  ngOnDestroy() {
    this.destroy$.next()
    this.destroy$.complete()
  }

}
