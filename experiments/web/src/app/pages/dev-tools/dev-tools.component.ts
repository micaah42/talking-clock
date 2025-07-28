import { Component, OnInit } from '@angular/core';
import { async } from '@angular/core/testing';

import { RemotingService } from 'src/app/remoting/remoting.service';

@Component({
  selector: 'app-dev-tools',
  templateUrl: './dev-tools.component.html',
  styleUrls: ['./dev-tools.component.scss']
})
export class DevToolsComponent implements OnInit {
  public properties: string[] = [];
  public propertyValue?: string;

  public receivers: string[] = [];
  public methods: Map<string, string[]> = new Map<string, string[]>();
  public cmd = { receiver: '', method: '', args: '' };
  public answer = '';
  public error = '';

  constructor(private remoting: RemotingService) {
    this.remoting.cmd<string[]>('remoting', 'properties', []).then(props => this.properties = props);

    this.remoting.cmd<string[]>('remoting', 'receivers', []).then(async (receivers: string[]) => {
      const methods = await Promise.all(receivers.map(async receiver => { return await this.remoting.cmd<string[]>('remoting', 'methods', [receiver]) }));
      methods.forEach((v, i) => this.methods.set(receivers[i], v));
      this.receivers = receivers.filter(receiver => this.methods.get(receiver)!.length > 0);
    })
  }

  ngOnInit(): void {

  }

  methodName(method: string): string {
    return method.split(' ')[1].split('(')[0]
  }

  async getPropertyValue(property: string) {
    const value = await this.remoting.cmd('remoting', 'value', [property]);
    this.propertyValue = JSON.stringify(value, undefined, 2).trim();
    console.log(this.propertyValue)
  }


  send(): void {
    const args = this.cmd.args.length > 0 ? this.cmd.args.split(',').map(arg => {
      try {
        return JSON.parse(arg)
      } catch (e) {
        this.error = JSON.stringify(e);
        this.answer = '';
        return;
      }
    }) : [];

    this.remoting.cmd<unknown>(this.cmd.receiver, this.cmd.method, args).then(answer => {
      this.answer = JSON.stringify(answer);
      this.error = '';
    }).catch((reason: any) => {
      console.log(reason)
      this.error = JSON.stringify(reason);
      this.answer = '';
    });
  }

}
