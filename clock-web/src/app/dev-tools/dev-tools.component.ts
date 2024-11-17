import { Component, OnDestroy } from '@angular/core';
import { RemotingService } from '../services/remoting.service';
import { Subject } from 'rxjs';

@Component({
  selector: 'app-dev-tools',
  standalone: true,
  imports: [],
  templateUrl: './dev-tools.component.html',
  styleUrl: './dev-tools.component.scss'
})
export class DevToolsComponent implements OnDestroy {
  nextAlarmName: string = ''
  clockTime: Date = new Date(0);
  private destroy$ = new Subject<void>();


  constructor(public remotingService: RemotingService) {
    remotingService.subscribe<string>('alarms.nextAlarm', this.destroy$).subscribe(x => console.log('new next alarm:', x))
    remotingService.subscribe<string>('alarms.nextAlarm.name', this.destroy$).subscribe(x => this.nextAlarmName = x)
    remotingService.subscribe<number>('alarms.now', this.destroy$).subscribe(x => this.clockTime = new Date(x))
  }

  ngOnDestroy(): void {
    this.destroy$.next()
  }
}
