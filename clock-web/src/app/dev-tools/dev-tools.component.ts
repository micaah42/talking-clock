import { Component } from '@angular/core';
import { RemotingService } from '../services/remoting.service';

@Component({
  selector: 'app-dev-tools',
  standalone: true,
  imports: [],
  templateUrl: './dev-tools.component.html',
  styleUrl: './dev-tools.component.scss'
})
export class DevToolsComponent {
  nextAlarmName: string = ''
  clockTime: Date = new Date(0);
  
  constructor(public remotingService: RemotingService) {
    remotingService.subscribe<string>('alarms.nextAlarm').subscribe(x => console.log('new next alarm:' ,x))
    remotingService.subscribe<string>('alarms.nextAlarm.name').subscribe(x => this.nextAlarmName = x)
    remotingService.subscribe<number>('alarms.now').subscribe(x => this.clockTime = new Date(x))
  }
}
