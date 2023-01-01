import {Component, OnInit} from '@angular/core';
import {RemotingService} from "../remoting/remoting.service";

@Component({
  selector: 'app-dev-tools',
  templateUrl: './dev-tools.component.html',
  styleUrls: ['./dev-tools.component.scss']
})
export class DevToolsComponent implements OnInit {

  constructor(private remoting: RemotingService) {
  }

  private cmdMap: Map<string, string[]> = new Map<string, string[]>();
  public cmd = {receiver: '', method: '', args: ''};
  public answer = '';
  public error = ''

  ngOnInit(): void {
    this.remoting.cmd<string[]>('remoting', 'receivers', []).then((receivers: string[]) => {
      for (let receiver of receivers) {
        this.remoting.cmd<string[]>('remoting', 'methods', [receiver]).then((methods: string[]) => {
          this.cmdMap.set(receiver, methods);
          console.log(`${receiver}: ${methods}`);
        })
      }
    })
  }

  get cmds(): [string, string[]][] {
    const arr = Array.from(this.cmdMap)
    return arr;
  }

  methodName(method: string): string {
    return method.split(' ')[1].split('(')[0]
  }


  send(): void {
    const args = this.cmd.args.split(',').map(arg => {
      return JSON.parse(arg)
    });

    this.remoting.cmd(this.cmd.receiver, this.cmd.method, args).then(answer => {
      this.answer = JSON.stringify(answer);
      this.error = '';
    }).catch(reason => {
      this.answer = '';
      this.error = reason;
    });
  }

}
