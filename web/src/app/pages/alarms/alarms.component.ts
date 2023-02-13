import { Component, OnDestroy, OnInit } from '@angular/core';
import { Subscription } from "rxjs";

import { Alarm } from "src/models/alarm";
import { Info, RemotingService } from "src/app/remoting/remoting.service";

@Component({
  selector: 'app-alarms',
  templateUrl: './alarms.component.html',
  styleUrls: ['./alarms.component.scss']
})
export class AlarmsComponent implements OnInit, OnDestroy {
  private _subs: Subscription[] = [];
  public sounds: string[] = []
  public weekDays: string[] = ['Mon', 'Tue', 'Wed', 'Thu', 'Fri', 'Sat', 'Sun'];

  constructor(private remoting: RemotingService) {
  }

  private _alarms: Alarm[] = [];

  get alarms(): Alarm[] {
    return this._alarms;
  }

  ngOnInit(): void {
    this._subs = [
      this.remoting.subscribe<Alarm>('alarms.model#*', (message: Info<Alarm>) => {
        const [_, id] = message.key.split('#');
        console.log('alarm changed:', id, message.value)
        const index = id as unknown as number;
        if (this._alarms.length <= index) {
          this._alarms.push({} as Alarm);
        }

        const value = message.value as Partial<Alarm>;
        if (value === null) {
          this._alarms.splice(index, 1);
        } else {
          this._alarms[index] = { ...this._alarms[index], ...value };
        }
      }),

      this.remoting.subscribe<string[]>('sounds.availableSounds', (message: Info<string[]>) => {
        this.sounds = message.value
      })
    ]

    this.remoting.cmd<any>('remoting', 'value', ['alarms.model']).then(alarms => {
      this._alarms = alarms['#'];
    })

    this.remoting.cmd<string[]>('remoting', 'value', ['sounds.availableSounds']).then(sounds => {
      this.sounds = sounds;
    })
  }

  onAlarmChanged(id: number): void {
    const alarm = this._alarms[id];
    this.remoting.cmd<null>('alarms.model', 'set', [id, alarm]);
  }

  addAlarm(): void {
    this.remoting.cmd<null>('alarms.model', 'push', []).then();
  }

  removeAlarm(id: number): void {
    this.remoting.cmd<null>('alarms.model', 'remove', [id]).then();
  }

  ngOnDestroy() {
    this._subs.forEach(sub => sub.unsubscribe());
  }
}
